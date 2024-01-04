#include "LoRa126x_Fun.h"
#include "LoRa126x_Hal.h"
#include "math.h"

rfState_t   g_rfState   = RF_STANDBY;    ///< rf状态
LoRaRxPkt_s g_LoRaRxPkt = { 0 };         ///< rx数据包

/*LoRa BW寄存器值对应的实际带宽*/
#define SX126X_LORA_BW0_HZ                       7810
#define SX126X_LORA_BW1_HZ                       15630
#define SX126X_LORA_BW2_HZ                       31250
#define SX126X_LORA_BW3_HZ                       62500
#define SX126X_LORA_BW4_HZ                       125000
#define SX126X_LORA_BW5_HZ                       250000
#define SX126X_LORA_BW6_HZ                       500000
#define SX126X_LORA_BW8_HZ                       10420
#define SX126X_LORA_BW9_HZ                       20830
#define SX126X_LORA_BW10_HZ                      41670

/*低速率优化symbol time值，高于此值建议开启*/
#define SX126X_LORA_LOW_DATARATE_OPT_SYMBOL_TIME 16380

/*SX126x cadDetPeak结构体*/
typedef struct
{
    uint8_t sf05CadDetPeak;
    uint8_t sf06CadDetPeak;
    uint8_t sf07CadDetPeak;
    uint8_t sf08CadDetPeak;
    uint8_t sf09CadDetPeak;
    uint8_t sf10CadDetPeak;
    uint8_t sf11CadDetPeak;
    uint8_t sf12CadDetPeak;
} SX126xLoRaCadPeakConfig_s;

/*SX126x CadSymbol结构体*/
typedef struct
{
    SX126xCadSymbs_t sf05Symbs;
    SX126xCadSymbs_t sf06Symbs;
    SX126xCadSymbs_t sf07Symbs;
    SX126xCadSymbs_t sf08Symbs;
    SX126xCadSymbs_t sf09Symbs;
    SX126xCadSymbs_t sf10Symbs;
    SX126xCadSymbs_t sf11Symbs;
    SX126xCadSymbs_t sf12Symbs;
} SX126xLoRaCadSymbs_s;

static bool                      s_SX126xLowDataOpt = false;
static SX126xCadExitModes_t      s_cadExitMode;
static SX126xLoRaConfig_s        s_LoRaConfig   = { 470000000,
                                                    SX126X_LORA_BW_125,
                                                    SX126X_LORA_SF7,
                                                    SX126X_LORA_CR_4_5,
                                                    22,
                                                    SX126X_LORA_PKT_EXPLICIT,
                                                    SX126X_LORA_CRC_ON,
                                                    SX162X_LORA_IQ_NORMAL,
                                                    8,
                                                    255 };
static SX126xLoRaCadPeakConfig_s s_LoRaCadPeak  = { SX126X_LORA_SF05_CADPEAK, SX126X_LORA_SF06_CADPEAK,
                                                    SX126X_LORA_SF07_CADPEAK, SX126X_LORA_SF08_CADPEAK,
                                                    SX126X_LORA_SF09_CADPEAK, SX126X_LORA_SF10_CADPEAK,
                                                    SX126X_LORA_SF11_CADPEAK, SX126X_LORA_SF12_CADPEAK };
static SX126xLoRaCadSymbs_s      s_LoRaCadSymbs = { SX126X_LORA_SF05_CAD_SYMBS, SX126X_LORA_SF06_CAD_SYMBS,
                                                    SX126X_LORA_SF07_CAD_SYMBS, SX126X_LORA_SF08_CAD_SYMBS,
                                                    SX126X_LORA_SF09_CAD_SYMBS, SX126X_LORA_SF10_CAD_SYMBS,
                                                    SX126X_LORA_SF11_CAD_SYMBS, SX126X_LORA_SF12_CAD_SYMBS };
#ifdef SX126X_CRYSTAL_INTERNAL_CAP
static uint8_t s_internalXoCap = 0x1D;    ///< 内部晶振负载电容值
#endif

static void     SX126xTxPrepare( void );
static void     SX126xRxPrepare( void );
static bool     IfEnableLowDataRateOptimize( SX126xLoRaSpreadingFactors_t sf, SX126xLoRaBandwidths_t bw );
static void     SX126xPayloadLenConfig( uint8_t payloadLen );
static void     SX126xCadParamsConfig( SX126xCadExitModes_t cadExitMode, uint32_t timeout );
static void     SX126xCadPrepare( SX126xCadExitModes_t cadExitMode );
static uint32_t FunPow( uint8_t x, uint8_t y );

/**
 * @brief  用户首次初始化LoRa参数设置
 * @param  freq         工作频率,单位Hz
 * @param  bw           BW
 * @param  sf           SF
 * @param  cr           CR
 * @param  power        功率，最大22
 * @param  header       包头模式，显现or隐形,显性包头支持可变长数据通信，隐性为固定长度
 * @param  crc          是否启用CRC
 * @param  iq           IQ模式，无特殊要求默认SX162X_LORA_IQ_NORMAL,发送和接收方必
 *                      须保持一致
 * @param  preambleLen  前导码，SF为5或者6时，前导码需要至少为12,其他配置建议8以上，
 *                      且需要要确保接收端的前导码设置要大于等于发射端的前导码长度
 * @param  payloadLen   数据包长度，隐性包头时需要设定固定值，显性包头不使用该参数，可
 *                      随意设置
 */
void LoRaUserConfig( uint32_t                     freq,
                     SX126xLoRaBandwidths_t       bw,
                     SX126xLoRaSpreadingFactors_t sf,
                     SX126xLoRaCodingRates_t      cr,
                     int8_t                       power,
                     SX126xLoraPktHeadType_t      header,
                     SX126xLoRaCrcModes_t         crc,
                     SX162xLoRaIQModes_t          iq,
                     uint16_t                     preambleLen,
                     uint8_t                      payloadLen )
{
    if( power > 22 )
        power = 22;

    if( sf == SX126X_LORA_SF5 || sf == SX126X_LORA_SF6 )
    {
        if( preambleLen < 12 )
            preambleLen = 12;
    }

    s_LoRaConfig.freq        = freq;
    s_LoRaConfig.BW          = bw;
    s_LoRaConfig.SF          = sf;
    s_LoRaConfig.CR          = cr;
    s_LoRaConfig.power       = power;
    s_LoRaConfig.header      = header;
    s_LoRaConfig.crc         = crc;
    s_LoRaConfig.invertIQ    = iq;
    s_LoRaConfig.preambleLen = preambleLen;
    s_LoRaConfig.payloadLen  = payloadLen;
}

/**
 * @brief   SX126x LoRa初始化，调用此函数前请先调用@LoRaUserConfig函数
 * @return  SX126xLoRaInitStatus_t 初始化结果
 *          -\ref   SX126X_LORA_INIT_OK:初始化成功
 *                  SX126X_LORA_INIT_SPI_ERROR:SPI通信异常
 *                  SX126X_LORA_INIT_FREQ_INVALID:频率非法（超过支持的范围）
 */
SX126xLoRaInitStatus_t SX126xLoRaInit( void )
{
#ifdef SX126X_USE_TCXO
    SX126xCalibrationParams_t calibParam;
#endif

    SX126xHardwareInit();    ///< 硬件初始化（IO和SPI）
    SX126xReset();

#ifdef LORA_INIT_DEBUG_MODE_ENABLE
    /*SPI通信测试*/
    if( SX126xSpiTest() == false )
        return SX126X_LORA_INIT_SPI_ERROR;

    SX126xEnterSleep();
    g_sx126xHardware.SX126xDelayMs( 2 );
    /*LoRa休眠后BUSY必定为高，非高表明BUSY状态读取异常*/
    if( g_sx126xHardware.ReadBusyState() == false )
        return SX126X_LORA_BUSY_STATE_ERROR;

    /*测试BUSY是否存在超时的可能性*/
    SX126xSetStandby( SX126X_STDBY_RC );
    if( SX126xGetIsBusyTimeout() )
        return SX126X_LORA_BUSY_TIMEOUT;
#endif

    /*配置调制方式必须在standby_RC下进行*/
    SX126xSetPacketType( SX126X_PKT_TYPE_LORA );

    /*配置同步字，用于区分网络类别*/
    SX126xSetNetworkType( SX126X_LORA_NETWORK_MODE );

    /*SX126X 芯片PA内部有过压保护，如果失配会导致功率降低问题；
    修改寄存器TxClampConfig来优化PA的阈值*/
    SX126xTxClampConfig();
    SX126xSetRegulatorMode( REGULATOR_MODE );

#ifdef SX126X_USE_TCXO
    /*如果使用DIO3去控制TCXO，那么每当上电之后或者是以Cold-Start的条件从Sleep唤醒，
    需要调用ClearDeviceErrors命令去清除OSC_START_ERR标志如果不清除在低温冷启动时可
    能会出现TCXO不工作情况*/
    SX126xClearDeviceErrors();

    SX126xSetDio3AsTcxoCtrl( TCXO_CTRL_VOLTAGE, SX126X_TCXO_TIMEOUT );
    SX126xSetXtalCap( 0x2F );

    /*Calibrate the RC13, RC64, ADC , Image、PLL
    防止上电之后或者是以Cold-Start的条件从Sleep唤醒，或者低温运行时sleep状态切换过
    来时出现PLLLock，XoscStart等错误现象，导致RF无法工作；*/
    calibParam.sx126xCalValue = SX126X_CALIBRATION_PARAM_ALL_ENABLE;
    SX126xCalibrate( calibParam );

    SX126xSetFallBackMode( FALLBACK_STDBY_XOSC );
#else
    SX126xSetFallBackMode( SX126X_FALLBACK_MODE );
#endif

#ifdef SX126X_USE_DIO2_AS_RFSWITCH_CTRL
    /*使用DIO2作为射频开关的控制源之一*/
    SX126xSetDio2AsRfSwitchCtrl( true );
#endif

    SX126xEnableRxBootedGain();

#ifdef LORA_INIT_DEBUG_MODE_ENABLE
    if( ( SX126xSetRfFrequency( s_LoRaConfig.freq ) == SX126X_PARAMETER_INVALID ) ||
        ( s_LoRaConfig.freq < SX126X_FREQ_LOWER_BOUND_IN_MHZ * 1000000 ) ||
        ( s_LoRaConfig.freq > SX126X_FREQ_UPPER_BOUND_IN_MHZ * 1000000 ) )
        return SX126X_LORA_INIT_FREQ_INVALID;
#else
    SX126xSetRfFrequency( s_LoRaConfig.freq );
#endif

    SX126xSetRfTxPower( s_LoRaConfig.power );
    SX126xLoRaModulationConfig( s_LoRaConfig.SF, s_LoRaConfig.BW, s_LoRaConfig.CR );
    SX126xLoRaPacketConfig( s_LoRaConfig.preambleLen,
                            s_LoRaConfig.header,
                            s_LoRaConfig.payloadLen,
                            s_LoRaConfig.crc,
                            s_LoRaConfig.invertIQ );
    SX126xSetStopRxTimerOnPreambleDetect( false );    ///< 选择包头监测到时停止RX计时
    SX126xSetSymbNumTimeout( 0 );

    return SX126X_LORA_INIT_OK;
}

/**
 * @brief  LoRa进入standby状态
 */
void SX126xEnterStandby( void )
{
    g_rfState = RF_STANDBY;

#if defined( SX126X_USE_XTAL )

    #if defined( SX126X_CRYSTAL_EXTERNAL_CAP )
    SX126xSetStandby( SX126X_STDBY_RC );

    #elif defined( SX126X_CRYSTAL_INTERNAL_CAP )
    SX126xSetStandby( SX126X_STDBY_XOSC );
    SX126xSetXtalCap( s_internalXoCap );    ///< 模块采用芯片内部晶振负载电容，每一次进入standby时都需要重新写入电容值

    #endif
#elif defined( SX126X_USE_TCXO )
    /*使用有源晶振时，进入RX时最好使用STDBY_XOSC，否则启动的晶振的时间会被计算在RX超时时间内*/
    SX126xSetStandby( SX126X_STDBY_XOSC );

#endif
}

#ifdef SX126X_CRYSTAL_INTERNAL_CAP

/**
 * @brief  获取内部晶振负载电容值
 * @return uint8_t 电容值
 */
uint8_t SX126xGetInternalXoCap( void )
{
    return s_internalXoCap;
}

/**
 * @brief  配置内部晶振负载电容值
 * @param  cap 电容值
 */
void SX126xSetInternalXoCap( uint8_t cap )
{
    s_internalXoCap = cap;
}

#endif

/**
 * @brief  射频发送数据
 * @param  data 数据首地址
 * @param  len 数据长度
 */
void SX126xSendData( uint8_t* data, uint8_t len )
{
    if( data == NULL || len == 0 )
        return;

    SX126xEnterStandby();
    SX126xTxPrepare();
    SX126xPayloadLenConfig( len );

    g_rfState = RF_TX_ING;
    SX126xSendPayload( data, len, SX126X_TX_TIMEOUT_50_S );    ///< 为了保险起见，设置了50s的超时
}

/**
 * @brief  126x发送前的预备动作
 */
static void SX126xTxPrepare( void )
{
    g_sx126xHardware.SetSwitchCtrl( RF_SWITCH_TX );
    SX126xCalibrateErr();
    SX126xClearAllIrqFlag();                   ///< 清除应用层IRQ标志
    SX126xClearIrqStatus( SX126X_IRQ_ALL );    ///< 清除硬件底层IRQ标志
    SX126xSetDioIrqParams( SX126X_TX_IRQ_ENABLE, SX126X_TX_IRQ_DIO1, SX126X_TX_IRQ_DIO2, SX126X_TX_IRQ_DIO3 );
    SX126xSetFifoBaseAddr();
}

/**
 * @brief  射频进入接收
 * @param  timeout  接收超时，单位ms，最大超时时长262143ms
 *                  设置0为无超时时间的单次接收，本固件不支持设置连续接收模式
 */
void SX126xEnterRx( uint32_t timeout )
{
    uint32_t loraTimeout = 0;

    if( timeout > RF_RX_CONTINUE_TIME )
        loraTimeout = 0xFFFFFE;
    else
        loraTimeout = (uint32_t) ( ( timeout * 1000 ) / SX126X_TRX_TIMEOUT_PER_TIME_US );

    SX126xEnterStandby();
    SX126xRxPrepare();

    if( s_LoRaConfig.header == SX126X_LORA_PKT_IMPLICIT )
        SX126xPayloadLenConfig( s_LoRaConfig.payloadLen );

    g_rfState = RF_RX_ING;
    SX126xSetRx( loraTimeout );
}

/**
 * @brief  进入RX前的预备动作
 */
static void SX126xRxPrepare( void )
{
    g_sx126xHardware.SetSwitchCtrl( RF_SWITCH_RX );
    SX126xCalibrateErr();
    SX126xClearAllIrqFlag();                   ///< 清除应用层IRQ标志
    SX126xClearIrqStatus( SX126X_IRQ_ALL );    ///< 清除硬件底层IRQ标志
    SX126xSetDioIrqParams( SX126X_RX_IRQ_ENABLE, SX126X_RX_IRQ_DIO1, SX126X_RX_IRQ_DIO2, SX126X_RX_IRQ_DIO3 );
    SX126xSetFifoBaseAddr();
}

/**
 * @brief   获取数据包信息，包含数据包长度，数据包内容，RSSI和SNR，
 *          信息保存在全局变量g_LoRaRxPkt中
 */
void SX126xGetRxPkt( void )
{
    SX126xLoraPktStatus_t  pktStatus;
    SX126xRxBufferStatus_t bufferStatus;

    /*获取RSSI和SNR信息*/
    SX126xGetPktStatus( &pktStatus );
    g_LoRaRxPkt.rssi = pktStatus.sx126xLoRaSignalPktRssi;
    g_LoRaRxPkt.snr  = pktStatus.sx126xLoRaPktSnr;

    /*获取数据包长度，显现包头从寄存器内获取，隐形包头从预设参数获取*/
    if( s_LoRaConfig.header == SX126X_LORA_PKT_EXPLICIT )
    {
        SX126xGetRxBufferStatus( &bufferStatus );
        g_LoRaRxPkt.size = bufferStatus.sx126xPayloadLen;
    }
    else
    {
        g_LoRaRxPkt.size = s_LoRaConfig.payloadLen;
    }
    SX126xGetPayload( g_LoRaRxPkt.payload, g_LoRaRxPkt.size );

    if( g_LoRaRxPkt.size != 0 )
        g_LoRaRxPkt.recFlag = true;
}

/**
 * @brief   SX126x进入CAD_ONLY模式,使用CAD_RX模式请勿使用本函数,本函数仅进入CAD模式，
 *          并不会等待CAD完成后才退出，若有CAD期间休眠MCU的需求，可以在本函数后，
 *          根据g_rfState状态配合RF中断服务程序在CAD_ING期间进行MCU休眠
 */
void SX126xStartCadOnly( void )
{
    s_cadExitMode = SX126X_CAD_ONLY;

    SX126xEnterStandby();
    SX126xCadPrepare( SX126X_CAD_ONLY );

    g_rfState = RF_CAD_ING;
    SX126xSetCad();
}

/**
 * @brief   SX126x进入CAD_RX模式,使用CAD_ONLY模式请勿使用本函数,本函数仅进入CAD模式,CAD监测
 *          到有用信号后会自动进入RX
 * @param  rxTimeOut CAD转入RX后的RX超时时间，单位ms
 */
void SX126xStartCadRx( uint32_t rxTimeOut )
{
    if( g_rfState == RF_CAD_ING )
        return;

    s_cadExitMode = SX126X_CAD_RX;

    g_sx126xHardware.SetSwitchCtrl( RF_SWITCH_RX );

    SX126xCadParamsConfig( SX126X_CAD_RX, rxTimeOut );
    SX126xCadPrepare( SX126X_CAD_RX );

    g_rfState = RF_CAD_ING;
    SX126xSetCad();
}

/**
 * @brief  进入CAD前的预备操作
 * @param  cadExitMode CAD模式，影响IRQ的类型
 */
static void SX126xCadPrepare( SX126xCadExitModes_t cadExitMode )
{
    SX126xClearAllIrqFlag();                   ///< 清除应用层IRQ标志
    SX126xClearIrqStatus( SX126X_IRQ_ALL );    ///< 清除硬件底层IRQ标志
    if( cadExitMode == SX126X_CAD_ONLY )
    {
        SX126xSetDioIrqParams( SX126X_CAD_ONLY_IRQ_ENABLE,
                               SX126X_CAD_ONLY_IQR_DI01,
                               SX126X_CAD_ONLY_IQR_DI02,
                               SX126X_CAD_ONLY_IQR_DI03 );
    }
    else if( cadExitMode == SX126X_CAD_RX )
    {
        SX126xSetDioIrqParams( SX126X_CAD_RX_IRQ_ENABLE,
                               SX126X_CAD_RX_IQR_DI01,
                               SX126X_CAD_RX_IQR_DI02,
                               SX126X_CAD_RX_IQR_DI03 );
    }
}

/**
 * @brief  SX126x进入休眠
 */
void SX126xEnterSleep( void )
{
    g_rfState = RF_SLEEP;
    g_sx126xHardware.SetSwitchCtrl( RF_SWITCH_OFF );
    SX126xSleep( SX126X_SLEEP_START_MODE );
}

/**
 * @brief  SX126x退出休眠模式，进入standby模式
 */
void SX126xExitSleep( void )
{
    g_rfState = RF_STANDBY;
    SX126xWakeup();
}

/**
 * @brief  设置功率
 * @param  power 功率，单位dBm，范围-3~22
 */
void SX126xSetPower( int8_t power )
{
    if( power <= 22 && power >= -3 )
        s_LoRaConfig.power = power;

    if( g_rfState != RF_STANDBY )
        SX126xEnterStandby();

    SX126xSetRfTxPower( s_LoRaConfig.power );
}

/**
 * @brief  配置频点
 * @param  freq 频点，单位Hz
 * @return true 配置成功
 * @return false 配置失败
 */
bool SX126xSetFreq( uint32_t freq )
{
    SX126xLoRaInitStatus_t state = SX126X_LORA_INIT_OK;

    if( g_rfState != RF_STANDBY )
        SX126xEnterStandby();

    if( SX126xSetRfFrequency( freq ) == SX126X_PARAMETER_INVALID )
    {
        return false;
    }

    s_LoRaConfig.freq = freq;

    return true;
}

/**
 * @brief  配置SF BW CR，低速率优化根据SF BW自动推荐设置
 * @param  sf SF
 * @param  bw BW
 * @param  cr CR
 * @return true 配置成功
 * @return false 配置失败
 */
bool SX126xLoRaModulationConfig( SX126xLoRaSpreadingFactors_t sf,
                                 SX126xLoRaBandwidths_t       bw,
                                 SX126xLoRaCodingRates_t      cr )
{
#ifdef LLCC68
    /*LLCC68只支持
    SF 5-6-7-8-9 @BW 125
    SF 5-6-7-8-9-10 @BW 250
    SF 5-6-7-8-9-10-11 @BW 500*/
    switch( bw )
    {
        case SX126X_LORA_BW_500:
            if( sf == SX126X_LORA_SF12 )
                return false;
            break;

        case SX126X_LORA_BW_250:
            if( sf == SX126X_LORA_SF12 || sf == SX126X_LORA_SF11 )
                return false;
            break;

        case SX126X_LORA_BW_125:
            if( sf == SX126X_LORA_SF12 || sf == SX126X_LORA_SF11 || sf == SX126X_LORA_SF10 )
                return false;
            break;

        default:
            return false;
            break;
    }
#endif
    s_LoRaConfig.SF = sf;
    s_LoRaConfig.BW = bw;
    s_LoRaConfig.CR = cr;

    if( g_rfState != RF_STANDBY )
        SX126xEnterStandby();

    s_SX126xLowDataOpt = IfEnableLowDataRateOptimize( s_LoRaConfig.SF, s_LoRaConfig.BW );

    SX126xSetLoraModulationParams( s_LoRaConfig.SF, s_LoRaConfig.BW, s_LoRaConfig.CR, s_SX126xLowDataOpt );

    SX126xCadParamsConfig( SX126X_CAD_ONLY, 0xFF );    ///< CAD模式为CAD_ONLY，后面的超时时间0xFF没有意义

    return true;
}

/**
 * @brief   SX126x CAD参数配置，使用仅CAD模式时，此函数只需在初始化和SF配置后调用即可，
 *          本固件已经在对应位置调用无需用户操作，但如果使用CAD_RX的模式，若需要配置一定
 *          的RX超时,进入CAD调用函数@SX126xStartCadRx
 * @param  cadExitMode  CAD监测到活跃信号后的模式状态，详见SX126x数据手册
 * @param  timeout      CAD进入RX后的超时时间，单位ms,最大超时时间为262143ms
 *                      设置0为无超时时间的单次接收，本固件暂时不支持设置连续接收模式
 */
static void SX126xCadParamsConfig( SX126xCadExitModes_t cadExitMode, uint32_t timeout )
{
    uint32_t         cadRxTimeout;
    uint8_t          cadDetPeak;
    SX126xCadSymbs_t cadSymbs;

    cadDetPeak = SX126xGetCadPeak( s_LoRaConfig.SF );
    cadSymbs   = SX126xGetCadSymbs( s_LoRaConfig.SF );

    if( timeout > RF_RX_CONTINUE_TIME )
        cadRxTimeout = 0xFFFFFE;
    else
        cadRxTimeout = (uint32_t) ( ( timeout * 1000 ) / SX126X_TRX_TIMEOUT_PER_TIME_US );

    SX126xSetCadParams( cadSymbs, cadDetPeak, 10, cadExitMode, cadRxTimeout );
}

/**
 * @brief   低速率优化配置
 *          函数@SX126xLoRaModulationConfig会根据SF BW按照官方推荐自动配置
 *          低速率优化，但有特殊情况下需要无视官方的推荐，自主选择时，可以使用
 *          本函数单独配置低速率优化设置,本函数必须在@SX126xLoRaModulationConfig
 *          之后调用执行，否则会被刷新掉。
 * @param  lowDataRateOpt   ture：开启低速率优化
 *                          false：关闭低速率优化
 */
void SX126xLowDataRateOptConfig( bool lowDataRateOpt )
{
    s_SX126xLowDataOpt = lowDataRateOpt;

    if( g_rfState != RF_STANDBY )
        SX126xEnterStandby();

    SX126xSetLoraModulationParams( s_LoRaConfig.SF, s_LoRaConfig.BW, s_LoRaConfig.CR, s_SX126xLowDataOpt );
}

/**
 * @brief  配置LoRa包参数
 * @param  preamble     前导码，SF为5或者6时，前导码需要至少为12,其他配置建议8以上，且
 *                      需要要确保接收端的前导码设置要大于等于发射端的前导码长度
 * @param  headerType   包头类型
 * @param  payloadLen   数据长度，包头为隐性时需要此参数，且发射端和接收端需要配置相同，
 *                      显示包头不需要使用该参数，可使用任意值
 * @param  crcMode      CRC模式，启用或者不启用
 * @param  invertIqMode IQ模式，无特殊要求默认SX162X_LORA_IQ_NORMAL
 */
void SX126xLoRaPacketConfig( uint16_t                preamble,
                             SX126xLoraPktHeadType_t headerType,
                             uint8_t                 payloadLen,
                             SX126xLoRaCrcModes_t    crcMode,
                             SX162xLoRaIQModes_t     invertIqMode )
{
    if( s_LoRaConfig.SF == SX126X_LORA_SF5 || s_LoRaConfig.SF == SX126X_LORA_SF6 )
    {
        if( preamble < 12 )
            preamble = 12;
    }

    if( g_rfState != RF_STANDBY )
        SX126xEnterStandby();

    s_LoRaConfig.preambleLen = preamble;
    s_LoRaConfig.header      = headerType;
    s_LoRaConfig.payloadLen  = payloadLen;
    s_LoRaConfig.crc         = crcMode;
    s_LoRaConfig.invertIQ    = invertIqMode;

    SX126xSetLoraPacketParams( s_LoRaConfig.preambleLen,
                               s_LoRaConfig.header,
                               s_LoRaConfig.payloadLen,
                               s_LoRaConfig.crc,
                               s_LoRaConfig.invertIQ );

    /*WORKAROUND - Optimizing the Inverted IQ Operation, see datasheet DS_SX1261 - 2_V1.2 §15.4*/
    SX126xIQConfig( s_LoRaConfig.invertIQ );
}

/**
 * @brief  SX126x单独配置前导码长度，有不断修改前导码长度的需求可使用本函数
 * @param  preamble     前导码，SF为5或者6时，前导码需要至少为12,其他配置建议8以上，且
 *                      需要要确保接收端的前导码设置要大于等于发射端的前导码长度
 */
void SX126xLoRaPreambleConfig( uint16_t preamble )
{
    if( s_LoRaConfig.SF == SX126X_LORA_SF5 || s_LoRaConfig.SF == SX126X_LORA_SF6 )
    {
        if( preamble < 12 )
            preamble = 12;
    }

    if( g_rfState != RF_STANDBY )
        SX126xEnterStandby();

    s_LoRaConfig.preambleLen = preamble;

    SX126xSetLoraPacketParams( s_LoRaConfig.preambleLen,
                               s_LoRaConfig.header,
                               s_LoRaConfig.payloadLen,
                               s_LoRaConfig.crc,
                               s_LoRaConfig.invertIQ );

    /*WORKAROUND - Optimizing the Inverted IQ Operation, see datasheet DS_SX1261 - 2_V1.2 §15.4*/
    SX126xIQConfig( s_LoRaConfig.invertIQ );
}

/**
 * @brief   SX126x单独配置数据包长度，用于发射和隐性包头的接收
 *          需要注意的是，本函数引入的数据长度不会修改初始化时设定的隐性
 *          包头的数据长度s_LoRaConfig.payloadLen
 * @param  payloadLen 数据长度
 */
static void SX126xPayloadLenConfig( uint8_t payloadLen )
{
    SX126xSetLoraPacketParams( s_LoRaConfig.preambleLen,
                               s_LoRaConfig.header,
                               payloadLen,
                               s_LoRaConfig.crc,
                               s_LoRaConfig.invertIQ );
}

/**
 * @brief  配置LoRa symbol timeout num值，请确保了此功能后使用
 * @param  num symbol个数
 */
void SX126xSetSymbNumTimeout( uint8_t num )
{
    SX126xSetLoRaSymbNumTimeout( num );
}

/**
 * @brief  判断是否需要开启低速率优化
 * @param  sf       SF
 * @param  bw       BW
 * @return true     需要开启低速率优化
 * @return false    不需要开启低速率优化
 */
static bool IfEnableLowDataRateOptimize( SX126xLoRaSpreadingFactors_t sf, SX126xLoRaBandwidths_t bw )
{
    uint32_t symbolTime  = 0;
    uint32_t bwKhz[ 11 ] = { SX126X_LORA_BW0_HZ, SX126X_LORA_BW1_HZ, SX126X_LORA_BW2_HZ, SX126X_LORA_BW3_HZ,
                             SX126X_LORA_BW4_HZ, SX126X_LORA_BW5_HZ, SX126X_LORA_BW6_HZ, 0,
                             SX126X_LORA_BW8_HZ, SX126X_LORA_BW9_HZ, SX126X_LORA_BW10_HZ };

    symbolTime           = ( FunPow( 2, sf ) * 1000000 ) / bwKhz[ bw ];

    if( symbolTime > SX126X_LORA_LOW_DATARATE_OPT_SYMBOL_TIME )
        return true;
    else
        return false;
}

/**
 * @brief  指数运算，x^y,需要注意到是，结果最大为2^32-1，
 *          超过时可能会导致数据溢出不正确，此函数仅用于
 *          计算2的底数，sf指数使用
 * @param  x 底数
 * @param  y 指数
 * @return uint32_t 结果值
 */
static uint32_t FunPow( uint8_t x, uint8_t y )
{
    uint32_t result = 1;
    uint8_t  i      = 0;

    for( i = 0; i < y; i++ )
    {
        result *= x;
    }

    return result;
}

/**
 * @brief   SX126x LoRa CADPeak参数配置，根据不同SF配置不同peak值，
 *          该值会影响CAD的灵敏度，具体请参见官方稳定，请务必确保了解此参数的
 *          意义和影响，否则请勿随意修改，本固件提供了默认的一组peak值，有特殊
 *          修改需求再使用本函数去修改
 * @param  sf       SF
 * @param  cadPeak  详见SX126x数据手册说明
 */
void SX126xLoRaCadPeakConfig( SX126xLoRaSpreadingFactors_t sf, uint8_t cadPeak )
{
    if( g_rfState != RF_STANDBY )
        SX126xEnterStandby();

    switch( sf )
    {
        case SX126X_LORA_SF5:
            s_LoRaCadPeak.sf05CadDetPeak = cadPeak;
            break;

        case SX126X_LORA_SF6:
            s_LoRaCadPeak.sf06CadDetPeak = cadPeak;
            break;

        case SX126X_LORA_SF7:
            s_LoRaCadPeak.sf07CadDetPeak = cadPeak;
            break;

        case SX126X_LORA_SF8:
            s_LoRaCadPeak.sf08CadDetPeak = cadPeak;
            break;

        case SX126X_LORA_SF9:
            s_LoRaCadPeak.sf09CadDetPeak = cadPeak;
            break;

        case SX126X_LORA_SF10:
            s_LoRaCadPeak.sf10CadDetPeak = cadPeak;
            break;

        case SX126X_LORA_SF11:
            s_LoRaCadPeak.sf11CadDetPeak = cadPeak;
            break;

        case SX126X_LORA_SF12:
            s_LoRaCadPeak.sf12CadDetPeak = cadPeak;
            break;

        default:
            break;
    }
}

/**
 * @brief  获取对应SF SX126x CAD peak detection值
 * @param  sf       SF
 * @return uint8_t  CAD peak detection
 */
uint8_t SX126xGetCadPeak( SX126xLoRaSpreadingFactors_t sf )
{
    switch( sf )
    {
        case SX126X_LORA_SF5:
            return s_LoRaCadPeak.sf05CadDetPeak;
            break;

        case SX126X_LORA_SF6:
            return s_LoRaCadPeak.sf06CadDetPeak;
            break;

        case SX126X_LORA_SF7:
            return s_LoRaCadPeak.sf07CadDetPeak;
            break;

        case SX126X_LORA_SF8:
            return s_LoRaCadPeak.sf08CadDetPeak;
            break;

        case SX126X_LORA_SF9:
            return s_LoRaCadPeak.sf09CadDetPeak;
            break;

        case SX126X_LORA_SF10:
            return s_LoRaCadPeak.sf10CadDetPeak;
            break;

        case SX126X_LORA_SF11:
            return s_LoRaCadPeak.sf11CadDetPeak;
            break;

        case SX126X_LORA_SF12:
            return s_LoRaCadPeak.sf12CadDetPeak;
            break;

        default:
            return s_LoRaCadPeak.sf07CadDetPeak;
            break;
    }
}

/**
 * @brief   SX126x LoRa CAD Symbol Num参数配置，根据不同SF配置不同symbs值，
 *          该值会影响CAD的监测长度，具体请参见官方文档，请务必确保了解此参数的
 *          意义和影响，否则请勿随意修改，本固件提供了默认的一组symbol num值，有特殊
 *          修改需求再使用本函数去修改
 * @param  sf       SF
 * @param  symbs    cad检测的symbol长度
 */
void SX126xLoRaCadSymbsConfig( SX126xLoRaSpreadingFactors_t sf, SX126xCadSymbs_t symbs )
{
    if( g_rfState != RF_STANDBY )
        SX126xEnterStandby();

    switch( sf )
    {
        case SX126X_LORA_SF5:
            s_LoRaCadSymbs.sf05Symbs = symbs;
            break;

        case SX126X_LORA_SF6:
            s_LoRaCadSymbs.sf06Symbs = symbs;
            break;

        case SX126X_LORA_SF7:
            s_LoRaCadSymbs.sf07Symbs = symbs;
            break;

        case SX126X_LORA_SF8:
            s_LoRaCadSymbs.sf08Symbs = symbs;
            break;

        case SX126X_LORA_SF9:
            s_LoRaCadSymbs.sf09Symbs = symbs;
            break;

        case SX126X_LORA_SF10:
            s_LoRaCadSymbs.sf10Symbs = symbs;
            break;

        case SX126X_LORA_SF11:
            s_LoRaCadSymbs.sf11Symbs = symbs;
            break;

        case SX126X_LORA_SF12:
            s_LoRaCadSymbs.sf12Symbs = symbs;
            break;

        default:
            break;
    }
}

/**
 * @brief  获取对应SF SX126x CAD symbol num值
 * @param  sf SF
 * @return SX126xCadSymbs_t CAD symbol num
 */
SX126xCadSymbs_t SX126xGetCadSymbs( SX126xLoRaSpreadingFactors_t sf )
{
    switch( sf )
    {
        case SX126X_LORA_SF5:
            return s_LoRaCadSymbs.sf05Symbs;
            break;

        case SX126X_LORA_SF6:
            return s_LoRaCadSymbs.sf06Symbs;
            break;

        case SX126X_LORA_SF7:
            return s_LoRaCadSymbs.sf07Symbs;
            break;

        case SX126X_LORA_SF8:
            return s_LoRaCadSymbs.sf08Symbs;
            break;

        case SX126X_LORA_SF9:
            return s_LoRaCadSymbs.sf09Symbs;
            break;

        case SX126X_LORA_SF10:
            return s_LoRaCadSymbs.sf10Symbs;
            break;

        case SX126X_LORA_SF11:
            return s_LoRaCadSymbs.sf11Symbs;
            break;

        case SX126X_LORA_SF12:
            return s_LoRaCadSymbs.sf12Symbs;
            break;

        default:
            return s_LoRaCadSymbs.sf07Symbs;
            break;
    }
}

/**
 * @brief  获取CADExitMode
 * @return SX126xCadExitModes_t CADExitMode
 */
SX126xCadExitModes_t SX126xGetCadExitModes( void )
{
    return s_cadExitMode;
}

/**
 * @brief  获取包头类型
 * @return SX126xLoraPktHeadType_t 包头类型
 */
SX126xLoraPktHeadType_t SX126xGetHeaderType( void )
{
    return s_LoRaConfig.header;
}

/**
 * @brief   SX126x发送单载波。
 *          需要注意的是，调用本函数后芯片会持续发送连续的载波，
 *          需要主动调用@SX126xEnterStandby才能退出发射状态。
 */
void SX126xSendCarrierWave( void )
{
    g_sx126xHardware.SetSwitchCtrl( RF_SWITCH_TX );
    SX126xEnterStandby();
    SX126xClearAllIrqFlag();                   ///< 清除应用层IRQ标志
    SX126xClearIrqStatus( SX126X_IRQ_ALL );    ///< 清除硬件底层IRQ标志

    g_rfState = RF_TX_ING;
    SX126X_TxContinuousWave();
}

/**
 * @brief   SX126x发送LoRa前导码。
 *          需要注意的是，调用本函数后芯片会持续发送前导码，
 *          需要主动调用@SX126xEnterStandby才能退出发射状态。
 */
void SX126xSendLoRaInfinitePreamble( void )
{
    g_sx126xHardware.SetSwitchCtrl( RF_SWITCH_TX );
    SX126xEnterStandby();
    SX126xClearAllIrqFlag();                   ///< 清除应用层IRQ标志
    SX126xClearIrqStatus( SX126X_IRQ_ALL );    ///< 清除硬件底层IRQ标志

    g_rfState = RF_TX_ING;
    SX1262_TxInfinitePreamble();
}

/**
 * @brief  获取126x接收模式下的瞬时RSSI，调用此函数需要确保处于RX状态
 * @return int8_t 瞬时RSSI值
 */
int8_t SX126xGetInstantRssi( void )
{
    return SX126xGetRssiInst();
}

/**
 * @brief   SX126x强制输出最大功率，针对低功率优化版本的硬件匹配电路
 *          本函数强制设置最大PA设置和功率输出，后果可能会导致电流过大,
 *          谐波超标,最大功率无法达到理论的最大值等现象，使用前请确定了解。
 */
void SX126xForceSetMaxPower( void )
{
    SX126xSetTxMaxWithoutOptimalPaParams( SX126X_RAMP_40_US );
}

/**
 * @brief  获取频点
 * @return uint32_t 频点，单位Hz
 */
uint32_t SX126xGetFreq( void )
{
    return s_LoRaConfig.freq;
}

/**
 * @brief  获取BW
 * @return SX126xLoRaBandwidths_t BW
 */
SX126xLoRaBandwidths_t SX126xGetBw( void )
{
    return s_LoRaConfig.BW;
}

/**
 * @brief  获取SF
 * @return SX126xLoRaSpreadingFactors_t SF
 */
SX126xLoRaSpreadingFactors_t SX126xGetSf( void )
{
    return s_LoRaConfig.SF;
}

/**
 * @brief  获取CR
 * @return SX126xLoRaCodingRates_t CR
 */
SX126xLoRaCodingRates_t SX126xGetCr( void )
{
    return s_LoRaConfig.CR;
}

/**
 * @brief  获取功率
 * @return int8_t 功率
 */
int8_t SX126xGetPower( void )
{
    return s_LoRaConfig.power;
}

/**
 * @brief  获取包头类型
 * @return SX126xLoraPktHeadType_t 包头类型
 */
SX126xLoraPktHeadType_t SX126xGetHeader( void )
{
    return s_LoRaConfig.header;
}

/**
 * @brief  获取CRC配置
 * @return SX126xLoRaCrcModes_t CRC配置
 */
SX126xLoRaCrcModes_t SX126xGetCrc( void )
{
    return s_LoRaConfig.crc;
}

/**
 * @brief  获取IQ模式
 * @return SX162xLoRaIQModes_t IQ模式
 */
SX162xLoRaIQModes_t SX126xGetIqMode( void )
{
    return s_LoRaConfig.invertIQ;
}

/**
 * @brief  获取前导码长度
 * @return uint16_t 前导码长度
 */
uint16_t SX126xGetPreambleLen( void )
{
    return s_LoRaConfig.preambleLen;
}

/**
 * @brief  获取低速率优化状态
 * @return true 已开启低速率优化
 * @return false 未开启低速率优化
 */
bool SX126xGetLowDataOptState( void )
{
    return s_SX126xLowDataOpt;
}
