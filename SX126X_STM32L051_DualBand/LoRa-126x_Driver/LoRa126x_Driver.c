#include "LoRa126x_Driver.h"
#include "LoRa126x_Hal.h"
#include "LoRa126x_Regs.h"
#include "LoRa126x_UserConfig.h"

#define SX126X_IMAGE_CALIBRATION_STEP_IN_MHZ 4    ///< 频率校准步进

/**
 * Commands Interface
 */
typedef enum sx126x_commands_e
{
    SX126X_SET_SLEEP                  = 0x84,    ///< Operational Modes Functions(below)
    SX126X_SET_STANDBY                = 0x80,
    SX126X_SET_FS                     = 0xC1,
    SX126X_SET_TX                     = 0x83,
    SX126X_SET_RX                     = 0x82,
    SX126X_SET_STOP_TIMER_ON_PREAMBLE = 0x9F,
    SX126X_SET_RX_DUTY_CYCLE          = 0x94,
    SX126X_SET_CAD                    = 0xC5,
    SX126X_SET_TX_CONTINUOUS_WAVE     = 0xD1,
    SX126X_SET_TX_INFINITE_PREAMBLE   = 0xD2,
    SX126X_SET_REGULATOR_MODE         = 0x96,
    SX126X_CALIBRATE                  = 0x89,
    SX126X_CALIBRATE_IMAGE            = 0x98,
    SX126X_SET_PA_CFG                 = 0x95,
    SX126X_SET_RX_TX_FALLBACK_MODE    = 0x93,
    SX126X_WRITE_REGISTER             = 0x0D,    ///< Registers and buffer Access(below)
    SX126X_READ_REGISTER              = 0x1D,
    SX126X_WRITE_BUFFER               = 0x0E,
    SX126X_READ_BUFFER                = 0x1E,
    SX126X_SET_DIO_IRQ_PARAMS         = 0x08,    ///< DIO and IRQ Control Functions(below)
    SX126X_GET_IRQ_STATUS             = 0x12,
    SX126X_CLR_IRQ_STATUS             = 0x02,
    SX126X_SET_DIO2_AS_RF_SWITCH_CTRL = 0x9D,
    SX126X_SET_DIO3_AS_TCXO_CTRL      = 0x97,
    SX126X_SET_RF_FREQUENCY           = 0x86,    ///< RF Modulation and Packet-Related Functions(below)
    SX126X_SET_PKT_TYPE               = 0x8A,
    SX126X_GET_PKT_TYPE               = 0x11,
    SX126X_SET_TX_PARAMS              = 0x8E,
    SX126X_SET_MODULATION_PARAMS      = 0x8B,
    SX126X_SET_PKT_PARAMS             = 0x8C,
    SX126X_SET_CAD_PARAMS             = 0x88,
    SX126X_SET_BUFFER_BASE_ADDRESS    = 0x8F,
    SX126X_SET_LORA_SYMB_NUM_TIMEOUT  = 0xA0,
    SX126X_GET_STATUS                 = 0xC0,    ///< Communication Status Information(below)
    SX126X_GET_RX_BUFFER_STATUS       = 0x13,
    SX126X_GET_PKT_STATUS             = 0x14,
    SX126X_GET_RSSI_INST              = 0x15,
    SX126X_GET_STATS                  = 0x10,
    SX126X_RESET_STATS                = 0x00,
    SX126X_GET_DEVICE_ERRORS          = 0x17,    ///< Miscellaneous(below)
    SX126X_CLR_DEVICE_ERRORS          = 0x07,
} sx126xCommands_t;

/**
 * Commands Interface buffer sizes
 */
typedef enum sx126x_commands_size_e
{
    SX126X_SIZE_SET_SLEEP                  = 1,    ///< Operational Modes Functions(below)
    SX126X_SIZE_SET_STANDBY                = 1,
    SX126X_SIZE_SET_FS                     = 0,
    SX126X_SIZE_SET_TX                     = 3,
    SX126X_SIZE_SET_RX                     = 3,
    SX126X_SIZE_SET_STOP_TIMER_ON_PREAMBLE = 1,
    SX126X_SIZE_SET_RX_DUTY_CYCLE          = 6,
    SX126X_SIZE_SET_CAD                    = 0,
    SX126X_SIZE_SET_TX_CONTINUOUS_WAVE     = 0,
    SX126X_SIZE_SET_TX_INFINITE_PREAMBLE   = 0,
    SX126X_SIZE_SET_REGULATOR_MODE         = 1,
    SX126X_SIZE_CALIBRATE                  = 1,
    SX126X_SIZE_CALIBRATE_IMAGE            = 2,
    SX126X_SIZE_SET_PA_CFG                 = 4,
    SX126X_SIZE_SET_RX_TX_FALLBACK_MODE    = 1,
    SX126X_SIZE_SET_DIO_IRQ_PARAMS         = 8,    ///< DIO and IRQ Control Functions(below)
    SX126X_SIZE_GET_IRQ_STATUS             = 2,
    SX126X_SIZE_CLR_IRQ_STATUS             = 2,
    SX126X_SIZE_SET_DIO2_AS_RF_SWITCH_CTRL = 1,
    SX126X_SIZE_SET_DIO3_AS_TCXO_CTRL      = 4,
    SX126X_SIZE_SET_RF_FREQUENCY           = 4,    ///< RF Modulation and Packet-Related Functions(below)
    SX126X_SIZE_SET_PKT_TYPE               = 1,
    SX126X_SIZE_GET_PKT_TYPE               = 1,
    SX126X_SIZE_SET_TX_PARAMS              = 2,
    SX126X_SIZE_SET_MODULATION_PARAMS_GFSK = 8,
    SX126X_SIZE_SET_MODULATION_PARAMS_LORA = 4,
    SX126X_SIZE_SET_PKT_PARAMS_GFSK        = 9,
    SX126X_SIZE_SET_PKT_PARAMS_LORA        = 6,
    SX126X_SIZE_SET_CAD_PARAMS             = 7,
    SX126X_SIZE_SET_BUFFER_BASE_ADDRESS    = 2,
    SX126X_SIZE_SET_LORA_SYMB_NUM_TIMEOUT  = 1,
    SX126X_SIZE_GET_STATUS                 = 0,    ///< Communication Status Information(below)
    SX126X_SIZE_GET_RX_BUFFER_STATUS       = 2,
    SX126X_SIZE_GET_PKT_STATUS             = 3,
    SX126X_SIZE_GET_RSSI_INST              = 1,
    SX126X_SIZE_GET_STATS                  = 6,
    SX126X_SIZE_RESET_STATS                = 6,
    SX126X_SIZE_GET_DEVICE_ERRORS          = 2,    ///< Miscellaneous(below)
    SX126X_SIZE_CLR_DEVICE_ERRORS          = 2,
} sx126x_commands_size_t;

/*芯片状态,LoRa在TX RXCAD等模式产生中断后自动退出到standby后，
该变量不会改变，仍会保持为TX RX CAD，但不影响本变量的实际作用，本
变量实际用于standby切换时的避免重复切换，TX RX CAD完成退出到
standby后，最多多做一次设置standby操作，不会带来其他影响
请不要修改本变量的静态特性且用于全局调用，避免产生其他问题*/
static SX126xOperatingModes_t s_sx126xOperatingMode  = SX126X_MODE_STDBY_RC;

static uint8_t                s_sx126xTxFifoBaseAddr = 0x00;     ///< TX数据在FIFO的缓存基地址
static uint8_t                s_sx126xRxFifoBaseAddr = 0x00;     ///< RX数据在FIFO的缓存基地址
static bool                   s_sx126xSpiUseFlag     = false;    ///< SPI操作标志
static bool                   s_waitBusyTimeout      = false;    ///< BUSY等待超时

static void                   SX126xSetTx( uint32_t timeout );
static void                   SX126xWriteRegister( uint16_t address, uint8_t buffer );
static uint8_t                SX126xReadRegister( uint16_t address );
static void                   SX126xWriteCommand( sx126xCommands_t command, uint8_t* buffer, uint8_t size );
static void                   SX126xReadCommand( sx126xCommands_t command, uint8_t* buffer, uint16_t size );
static void                   SX126xGetFreqCalibration( uint8_t* freqStep1, uint8_t* freqStep2 );
static void                   SX126xCheckDeviceReady( void );
static void                   SX126xSetPaConfig( uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut );
static void                   SX126xSetTxParams( int8_t power, sx126xRampTimes_t rampTime );
static SX126xOperatingModes_t SX126xGetOperatingMode( void );

/**
 * @brief  获取126x当前的状态
 * @return SX126xOperatingModes_t 126x状态
 */
static SX126xOperatingModes_t SX126xGetOperatingMode( void )
{
    return s_sx126xOperatingMode;
}

/**
 * @brief  126x唤醒
 */
void SX126xWakeup( void )
{
    s_sx126xSpiUseFlag = true;

    g_sx126xHardware.SetSX126xSpiNssPin( RF_RESET );
    g_sx126xHardware.SX126xSpiReadWriteByte( SX126X_GET_STATUS );
    g_sx126xHardware.SX126xSpiReadWriteByte( 0x00 );
    g_sx126xHardware.SetSX126xSpiNssPin( RF_SET );

    if( s_sx126xOperatingMode == SX126X_MODE_SLEEP )
        s_sx126xOperatingMode = SX126X_MODE_STDBY_RC;

    /* Wait for chip to be ready. */
    s_waitBusyTimeout  = SX126xWaitBusy();

    s_sx126xSpiUseFlag = false;
}

/**
 * @brief  检查126x芯片是否待命
 */
static void SX126xCheckDeviceReady( void )
{
    if( ( SX126xGetOperatingMode() == SX126X_MODE_SLEEP ) || ( SX126xGetOperatingMode() == SX126X_MODE_RX_DC ) )
    {
        SX126xWakeup();
    }

    /* Wait for chip to be ready. */
    s_waitBusyTimeout = SX126xWaitBusy();
}

/**
 * @brief  126x写寄存器
 * @param  address 寄存器地址
 * @param  buffer 寄存器写入内容
 */
static void SX126xWriteRegister( uint16_t address, uint8_t buffer )
{
    SX126xCheckDeviceReady();

    s_sx126xSpiUseFlag = true;

    g_sx126xHardware.SetSX126xSpiNssPin( RF_RESET );
    g_sx126xHardware.SX126xSpiReadWriteByte( SX126X_WRITE_REGISTER );
    g_sx126xHardware.SX126xSpiReadWriteByte( ( address & 0xFF00 ) >> 8 );
    g_sx126xHardware.SX126xSpiReadWriteByte( address & 0x00FF );
    g_sx126xHardware.SX126xSpiReadWriteByte( buffer );
    g_sx126xHardware.SetSX126xSpiNssPin( RF_SET );

    /* Wait for chip to be ready. */
    s_waitBusyTimeout  = SX126xWaitBusy();

    s_sx126xSpiUseFlag = false;
}

/**
 * @brief  126x读寄存器
 * @param  address 寄存器地址
 * @return uint8_t 读取到的寄存器值
 */
static uint8_t SX126xReadRegister( uint16_t address )
{
    uint8_t data;

    SX126xCheckDeviceReady();

    s_sx126xSpiUseFlag = true;

    g_sx126xHardware.SetSX126xSpiNssPin( RF_RESET );
    g_sx126xHardware.SX126xSpiReadWriteByte( SX126X_READ_REGISTER );
    g_sx126xHardware.SX126xSpiReadWriteByte( ( address & 0xFF00 ) >> 8 );
    g_sx126xHardware.SX126xSpiReadWriteByte( address & 0x00FF );
    g_sx126xHardware.SX126xSpiReadWriteByte( 0x00 );
    data = g_sx126xHardware.SX126xSpiReadWriteByte( 0x00 );
    g_sx126xHardware.SetSX126xSpiNssPin( RF_SET );

    /* Wait for chip to be ready. */
    s_waitBusyTimeout  = SX126xWaitBusy();

    s_sx126xSpiUseFlag = false;

    return data;
}

/**
 * @brief  126x写命令
 * @param  command 命令
 * @param  buffer 写入数据首地址
 * @param  size 写入数据长度
 */
static void SX126xWriteCommand( sx126xCommands_t command, uint8_t* buffer, uint8_t size )
{
    if( buffer == NULL )
        size = 0;

    SX126xCheckDeviceReady();

    s_sx126xSpiUseFlag = true;

    g_sx126xHardware.SetSX126xSpiNssPin( RF_RESET );
    g_sx126xHardware.SX126xSpiReadWriteByte( (uint8_t) command );
    for( uint8_t i = 0; i < size; i++ )
    {
        g_sx126xHardware.SX126xSpiReadWriteByte( buffer[ i ] );
    }
    g_sx126xHardware.SetSX126xSpiNssPin( RF_SET );

    if( command != SX126X_SET_SLEEP )
    {
        /* Wait for chip to be ready. */
        s_waitBusyTimeout = SX126xWaitBusy();
    }

    s_sx126xSpiUseFlag = false;
}

/**
 * @brief  126x读命令
 * @param  command 命令
 * @param  buffer 读取的数据存放首地址
 * @param  size 读取的数据长度
 */
static void SX126xReadCommand( sx126xCommands_t command, uint8_t* buffer, uint16_t size )
{
    if( buffer == NULL || size == 0 )
        return;

    SX126xCheckDeviceReady();

    s_sx126xSpiUseFlag = true;

    g_sx126xHardware.SetSX126xSpiNssPin( RF_RESET );
    g_sx126xHardware.SX126xSpiReadWriteByte( (uint8_t) command );
    g_sx126xHardware.SX126xSpiReadWriteByte( 0 );
    for( uint16_t i = 0; i < size; i++ )
    {
        buffer[ i ] = g_sx126xHardware.SX126xSpiReadWriteByte( 0x00 );
    }
    g_sx126xHardware.SetSX126xSpiNssPin( RF_SET );

    /* Wait for chip to be ready. */
    s_waitBusyTimeout  = SX126xWaitBusy();

    s_sx126xSpiUseFlag = false;
}

/**
 * @brief  126x写FIFO
 * @param  offset 地址偏移
 * @param  buffer 数据内容首地址
 * @param  size 数据长度
 */
void SX126xWriteFifo( uint8_t offset, uint8_t* buffer, uint8_t size )
{
    if( buffer == NULL || size == 0 )
        return;

    SX126xCheckDeviceReady();

    s_sx126xSpiUseFlag = true;

    g_sx126xHardware.SetSX126xSpiNssPin( RF_RESET );
    g_sx126xHardware.SX126xSpiReadWriteByte( SX126X_WRITE_BUFFER );
    g_sx126xHardware.SX126xSpiReadWriteByte( offset );
    for( uint16_t i = 0; i < size; i++ )
    {
        g_sx126xHardware.SX126xSpiReadWriteByte( buffer[ i ] );
    }
    g_sx126xHardware.SetSX126xSpiNssPin( RF_SET );

    /* Wait for chip to be ready. */
    s_waitBusyTimeout  = SX126xWaitBusy();

    s_sx126xSpiUseFlag = false;
}

/**
 * @brief  126x读Fifo
 * @param  offset 地址偏移
 * @param  buffer 读取的的数据内容存放地址
 * @param  size 数据长度
 */
void SX126xReadFifo( uint8_t offset, uint8_t* buffer, uint8_t size )
{
    if( buffer == NULL || size == 0 )
        return;

    SX126xCheckDeviceReady();

    s_sx126xSpiUseFlag = true;

    g_sx126xHardware.SetSX126xSpiNssPin( RF_RESET );
    g_sx126xHardware.SX126xSpiReadWriteByte( SX126X_READ_BUFFER );
    g_sx126xHardware.SX126xSpiReadWriteByte( offset );
    g_sx126xHardware.SX126xSpiReadWriteByte( 0 );
    for( uint16_t i = 0; i < size; i++ )
    {
        buffer[ i ] = g_sx126xHardware.SX126xSpiReadWriteByte( 0x00 );
    }
    g_sx126xHardware.SetSX126xSpiNssPin( RF_SET );

    /* Wait for chip to be ready. */
    s_waitBusyTimeout  = SX126xWaitBusy();

    s_sx126xSpiUseFlag = false;
}

/**
 * @brief  126x进入standby模式
 * @param  standbyConfig standby类型，RC or XOSC
 */
void SX126xSetStandby( SX126xStandbyModes_t standbyConfig )
{
    uint8_t buf[ SX126X_SIZE_SET_STANDBY ];

    buf[ 0 ] = (uint8_t) standbyConfig;

    if( standbyConfig == SX126X_STDBY_RC )
    {
        if( s_sx126xOperatingMode == SX126X_MODE_STDBY_RC )
            return;
        s_sx126xOperatingMode = SX126X_MODE_STDBY_RC;
    }
    else
    {
        if( s_sx126xOperatingMode == SX126X_MODE_STDBY_XOSC )
            return;
        s_sx126xOperatingMode = SX126X_MODE_STDBY_XOSC;
    }

    SX126xWriteCommand( SX126X_SET_STANDBY, buf, SX126X_SIZE_SET_STANDBY );
}

/**
 * @brief  配置126x调制模式，配置操作必须在standby_RC下进行
 * @param  packetType 调制模式，GFSK or LoRa
 */
void SX126xSetPacketType( SX126xPktType_t packetType )
{
    uint8_t buf[ SX126X_SIZE_SET_PKT_TYPE ];

    buf[ 0 ] = packetType;

    if( packetType == SX126X_PKT_TYPE_GFSK )
    {
        SX126xWriteRegister( SX126X_REG_BIT_SYNC, 0x00 );
    }
    SX126xWriteCommand( SX126X_SET_PKT_TYPE, buf, SX126X_SIZE_SET_PKT_TYPE );
}

/**
 * @brief  获取126x包类型
 * @return SX126xPktType_t LoRa or GFSK
 */
SX126xPktType_t SX126xGetPacketType( void )
{
    uint8_t buf[ SX126X_SIZE_GET_PKT_TYPE ];

    SX126xReadCommand( SX126X_GET_PKT_TYPE, buf, SX126X_SIZE_GET_PKT_TYPE );

    return buf[ 0 ];
}

/**
 * @brief  配置126x电源类型
 * @param  mode 对应类型，LDO or DCDC
 */
void SX126xSetRegulatorMode( SX126xRegulatorMode_t mode )
{
    uint8_t buf[ SX126X_SIZE_SET_REGULATOR_MODE ];

    buf[ 0 ] = (uint8_t) mode;
    SX126xWriteCommand( SX126X_SET_REGULATOR_MODE, buf, SX126X_SIZE_SET_REGULATOR_MODE );
}

/**
 * @brief  配置FIFO中TX和RX缓存基地址
 * @param  txBaseAddress TX缓存基地址
 * @param  rxBaseAddress RX缓存基地址
 */
void SX126xSetBufferBaseAddress( uint8_t txBaseAddress, uint8_t rxBaseAddress )
{
    uint8_t buf[ SX126X_SIZE_SET_BUFFER_BASE_ADDRESS ];

    buf[ 0 ] = txBaseAddress;
    buf[ 1 ] = rxBaseAddress;
    SX126xWriteCommand( SX126X_SET_BUFFER_BASE_ADDRESS, buf, SX126X_SIZE_SET_BUFFER_BASE_ADDRESS );
}

/**
 * @brief  配置FIFO中TX和RX缓存基地址
 */
void SX126xSetFifoBaseAddr( void )
{
    SX126xSetBufferBaseAddress( s_sx126xTxFifoBaseAddr, s_sx126xRxFifoBaseAddr );
}

/**
 * @brief  126x执行功能块校准，该函数只能在standby-rc模式下进行
 * @param  calibParam 要执行的校准功能块
 */
void SX126xCalibrate( SX126xCalibrationParams_t calibParam )
{
    uint8_t buf[ SX126X_SIZE_CALIBRATE ];

    buf[ 0 ] = calibParam.sx126xCalValue;
    SX126xWriteCommand( SX126X_CALIBRATE, buf, SX126X_SIZE_CALIBRATE );
}

/**
 * @brief  126x频率校正
 * @param  freq 频率值
 */
void SX126xCalibrateImage( void )
{
    uint8_t buf[ SX126X_SIZE_CALIBRATE_IMAGE ];

    SX126xGetFreqCalibration( &buf[ 0 ], &buf[ 1 ] );

    SX126xWriteCommand( SX126X_CALIBRATE_IMAGE, buf, SX126X_SIZE_CALIBRATE_IMAGE );
}

/**
 * @brief  获取频率校准值
 * @param  freqStep1 频率校准值1
 * @param  freqStep2 频率校准值2
 */
static void SX126xGetFreqCalibration( uint8_t* freqStep1, uint8_t* freqStep2 )
{
    *freqStep1 = SX126X_FREQ_LOWER_BOUND_IN_MHZ / SX126X_IMAGE_CALIBRATION_STEP_IN_MHZ;
    *freqStep2 = ( SX126X_FREQ_UPPER_BOUND_IN_MHZ + SX126X_IMAGE_CALIBRATION_STEP_IN_MHZ - 1 ) /
                 SX126X_IMAGE_CALIBRATION_STEP_IN_MHZ;
}

/**
 * @brief  126x频率配置
 * @param  frequency 频率值
 * @return SX126xError_t 错误状态
 */
SX126xError_t SX126xSetRfFrequency( uint32_t frequency )
{
    uint32_t freq;
    uint8_t  buf[ SX126X_SIZE_SET_RF_FREQUENCY ];

#if( defined SX1261 ) || ( defined SX1262 ) || ( defined LLCC68 )
    if( ( frequency > 960000000 ) || ( frequency < 150000000 ) )
        return SX126X_PARAMETER_INVALID;

#elif defined SX1268
    if( ( frequency > 810000000 ) || ( frequency < 410000000 ) )
        return SX126X_PARAMETER_INVALID;
#endif

    /*频率校准时，32MHz的时钟晶振不能来自于TCXO，因此在STDBY_RC状态下进行*/
    SX126xSetStandby( SX126X_STDBY_RC );
    SX126xCalibrateImage();

    freq     = (uint32_t) ( (double) frequency / (double) FREQ_STEP );
    buf[ 0 ] = (uint8_t) ( ( freq >> 24 ) & 0xFF );
    buf[ 1 ] = (uint8_t) ( ( freq >> 16 ) & 0xFF );
    buf[ 2 ] = (uint8_t) ( ( freq >> 8 ) & 0xFF );
    buf[ 3 ] = (uint8_t) ( freq & 0xFF );
    SX126xWriteCommand( SX126X_SET_RF_FREQUENCY, buf, SX126X_SIZE_SET_RF_FREQUENCY );

    return SX126X_NORMAL;
}

/**
 * @brief  配置126xDIO中断类型
 * @param  irqMask 启用的中断类型在对应位置1，否则置0
 * @param  dio1Mask DIO1映射的中断类型
 * @param  dio2Mask DIO2映射的中断类型
 * @param  dio3Mask DIO3映射的中断类型
 */
void SX126xSetDioIrqParams( uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask )
{
    uint8_t buf[ SX126X_SIZE_SET_DIO_IRQ_PARAMS ];

    buf[ 0 ] = (uint8_t) ( ( irqMask >> 8 ) & 0xFF );
    buf[ 1 ] = (uint8_t) ( irqMask & 0xFF );
    buf[ 2 ] = (uint8_t) ( ( dio1Mask >> 8 ) & 0xFF );
    buf[ 3 ] = (uint8_t) ( dio1Mask & 0xFF );
    buf[ 4 ] = (uint8_t) ( ( dio2Mask >> 8 ) & 0xFF );
    buf[ 5 ] = (uint8_t) ( dio2Mask & 0xFF );
    buf[ 6 ] = (uint8_t) ( ( dio3Mask >> 8 ) & 0xFF );
    buf[ 7 ] = (uint8_t) ( dio3Mask & 0xFF );

    SX126xWriteCommand( SX126X_SET_DIO_IRQ_PARAMS, buf, SX126X_SIZE_SET_DIO_IRQ_PARAMS );
}

/**
 * @brief  配置SX126x PA参数
 * @param  paDutyCycle 详见126x数据手册
 * @param  hpMax 详见126x数据手册
 * @param  deviceSel 详见126x数据手册
 * @param  paLut 详见126x数据手册
 */
static void SX126xSetPaConfig( uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut )
{
    uint8_t buf[ SX126X_SIZE_SET_PA_CFG ];

    buf[ 0 ] = paDutyCycle;
    buf[ 1 ] = hpMax;
    buf[ 2 ] = deviceSel;
    buf[ 3 ] = paLut;

    SX126xWriteCommand( SX126X_SET_PA_CFG, buf, SX126X_SIZE_SET_PA_CFG );
}

/**
 * @brief  126x设置TX参数
 * @param  power 功率
 * @param  rampTime 功率斜坡时间
 */
static void SX126xSetTxParams( int8_t power, sx126xRampTimes_t rampTime )
{
    uint8_t buf[ SX126X_SIZE_SET_TX_PARAMS ];

    if( power > POWER_MAX )
    {
        power = POWER_MAX;
    }
    else if( power < POWER_MIN )
    {
        power = POWER_MIN;
    }

    if( power <= SX126X_USER_POWER_MAX )
    {
        /*确保power最大值不超过22*/
        power = power + SX126X_POWER_ADDITION;

        if( power > POWER_MAX )
        {
            power = POWER_MAX;
        }
        SX126xSetPaConfig( SX126X_PADUTYCYCLE, SX126X_HPMAX, SX126X_DEVICESEL, SX126X_PALUT );
    }
    /*防止错误配置，正常状态下不应该进入else*/
    else
    {
        power = POWER_MAX;
        SX126xSetPaConfig( 0x04, 0x07, 0x00, 0x01 );
    }

    SX126xWriteRegister( SX126X_REG_OCP, SX126X_OCP_140MA );    ///< 最大140mA电流

    buf[ 0 ] = power;
    buf[ 1 ] = (uint8_t) rampTime;

    SX126xWriteCommand( SX126X_SET_TX_PARAMS, buf, SX126X_SIZE_SET_TX_PARAMS );
}

/**
 * @brief  126x无视硬件匹配的PA优化参数设置最大输出功率
 * @param  rampTime 功率斜坡时间
 */
void SX126xSetTxMaxWithoutOptimalPaParams( sx126xRampTimes_t rampTime )
{
    uint8_t buf[ SX126X_SIZE_SET_TX_PARAMS ];
    uint8_t power = POWER_MAX;

    SX126xSetPaConfig( 0x04, 0x07, 0x00, 0x01 );

    SX126xWriteRegister( SX126X_REG_OCP, SX126X_OCP_140MA );    ///< 最大140mA电流

    buf[ 0 ] = power;
    buf[ 1 ] = (uint8_t) rampTime;

    SX126xWriteCommand( SX126X_SET_TX_PARAMS, buf, SX126X_SIZE_SET_TX_PARAMS );
}

/**
 * @brief  设置功率值
 * @param  power 功率
 */
void SX126xSetRfTxPower( int8_t power )
{
    SX126xSetTxParams( power, SX126X_RAMP_40_US );
}

/**
 * @brief  126x LoRa调制参数配置(SF,BW,CR,lowDataRateOpt)
 * @param  sf SF
 * @param  bw BW
 * @param  cr CR
 * @param  lowDataRateOpt 低速率优化，ture开启，false关闭
 */
void SX126xSetLoraModulationParams( SX126xLoRaSpreadingFactors_t sf,
                                    SX126xLoRaBandwidths_t       bw,
                                    SX126xLoRaCodingRates_t      cr,
                                    bool                         lowDataRateOpt )
{
    uint8_t buf[ SX126X_SIZE_SET_MODULATION_PARAMS_LORA ];
    uint8_t value = 0;

    buf[ 0 ]      = (uint8_t) sf;
    buf[ 1 ]      = (uint8_t) bw;
    buf[ 2 ]      = (uint8_t) cr;
    buf[ 3 ]      = lowDataRateOpt ? SX126X_LOW_DATARATE_OPT_ON : SX126X_LOW_DATARATE_OPT_OFF;

    /*LoRa BW=500时，寄存器0x0889的bit2需要设置0，其他BW设置1，详细见规格书15.1章节描述*/
    if( bw == SX126X_LORA_BW_500 )
    {
        value = SX126xReadRegister( SX126X_REG_TX_MODULATION );
        value = value & 0xFB;
        SX126xWriteRegister( SX126X_REG_TX_MODULATION, value );
    }
    else
    {
        value = SX126xReadRegister( SX126X_REG_TX_MODULATION );
        value = value | 0x04;
        SX126xWriteRegister( SX126X_REG_TX_MODULATION, value );
    }

    SX126xWriteCommand( SX126X_SET_MODULATION_PARAMS, buf, SX126X_SIZE_SET_MODULATION_PARAMS_LORA );
}

/**
 * @brief  126x LoRa包参数配置（前导码长度，包头，载荷，CRC和IQ）
 */
void SX126xSetLoraPacketParams( uint16_t                preamble,
                                SX126xLoraPktHeadType_t headerType,
                                uint8_t                 payloadLen,
                                SX126xLoRaCrcModes_t    crcMode,
                                SX162xLoRaIQModes_t     invertIqMode )
{
    uint8_t buf[ SX126X_SIZE_SET_PKT_PARAMS_LORA ];

    buf[ 0 ] = (uint8_t) ( ( preamble >> 8 ) & 0xFF );
    buf[ 1 ] = (uint8_t) ( preamble & 0xFF );
    buf[ 2 ] = (uint8_t) headerType;
    buf[ 3 ] = payloadLen;
    buf[ 4 ] = (uint8_t) crcMode;
    buf[ 5 ] = (uint8_t) invertIqMode;

    SX126xWriteCommand( SX126X_SET_PKT_PARAMS, buf, SX126X_SIZE_SET_PKT_PARAMS_LORA );
}

/**
 * @brief 选择是否允许在前导码监测到时是否停止RX超时计时
 * @param  enable true表示选择在前导码监测到时停止RX超时计时
 *                false表示选择包头监测到时停止计时
 */
void SX126xSetStopRxTimerOnPreambleDetect( bool enable )
{
    uint8_t buf[ SX126X_SIZE_SET_STOP_TIMER_ON_PREAMBLE ];

    buf[ 0 ] = enable ? 0x01 : 0x00;

    SX126xWriteCommand( SX126X_SET_STOP_TIMER_ON_PREAMBLE, buf, SX126X_SIZE_SET_STOP_TIMER_ON_PREAMBLE );
}

/**
 * @brief  配置126x调制解调器用于验证成功接收的符号数量
 * @param  SymbNum 符号数量
 */
void SX126xSetLoRaSymbNumTimeout( uint8_t SymbNum )
{
    uint8_t buf[ SX126X_SIZE_SET_LORA_SYMB_NUM_TIMEOUT ];

    buf[ 0 ] = SymbNum;

    SX126xWriteCommand( SX126X_SET_LORA_SYMB_NUM_TIMEOUT, buf, SX126X_SIZE_SET_LORA_SYMB_NUM_TIMEOUT );
}

/**
 * @brief  清除126x中断标志
 * @param  irq 中断类型
 */
void SX126xClearIrqStatus( uint16_t irq )
{
    uint8_t buf[ SX126X_SIZE_CLR_IRQ_STATUS ];

    buf[ 0 ] = (uint8_t) ( ( irq >> 8 ) & 0xFF );
    buf[ 1 ] = (uint8_t) ( irq & 0xFF );

    SX126xWriteCommand( SX126X_CLR_IRQ_STATUS, buf, SX126X_SIZE_CLR_IRQ_STATUS );
}

/**
 * @brief  获取126x中断标志状态
 * @return uint16_t 126x中断标志
 */
uint16_t SX126xGetIrqStatus( void )
{
    uint8_t buf[ SX126X_SIZE_GET_IRQ_STATUS ];

    SX126xReadCommand( SX126X_GET_IRQ_STATUS, buf, SX126X_SIZE_GET_IRQ_STATUS );

    return ( (uint16_t) buf[ 0 ] << 8 ) | buf[ 1 ];
}

/**
 * @brief  获取126x芯片错误标志
 * @return SX126xDeviceError_t 错误标志
 */
SX126xDeviceError_t SX126xGetDeviceErrors( void )
{
    uint8_t             err[ SX126X_SIZE_GET_DEVICE_ERRORS ];
    SX126xDeviceError_t error = { .sx126xErrValue = 0 };

    SX126xReadCommand( SX126X_GET_DEVICE_ERRORS, err, SX126X_SIZE_GET_DEVICE_ERRORS );

    error.Fields.sx126xPaRampErr     = ( err[ 0 ] & ( 1 << 0 ) ) >> 0;
    error.Fields.sx126xPllLockErr    = ( err[ 1 ] & ( 1 << 6 ) ) >> 6;
    error.Fields.sx126xXoscStartErr  = ( err[ 1 ] & ( 1 << 5 ) ) >> 5;
    error.Fields.sx126xImgCalibErr   = ( err[ 1 ] & ( 1 << 4 ) ) >> 4;
    error.Fields.sx126xAdcCalibErr   = ( err[ 1 ] & ( 1 << 3 ) ) >> 3;
    error.Fields.sx126xPllCalibErr   = ( err[ 1 ] & ( 1 << 2 ) ) >> 2;
    error.Fields.sx126xRC13mCalibErr = ( err[ 1 ] & ( 1 << 1 ) ) >> 1;
    error.Fields.sx126xRC64kCalibErr = ( err[ 1 ] & ( 1 << 0 ) ) >> 0;

    return error;
}

/**
 * @brief  清除126x芯片错误标志
 */
void SX126xClearDeviceErrors( void )
{
    uint8_t buf[ SX126X_SIZE_CLR_DEVICE_ERRORS ] = { 0x00, 0x00 };

    SX126xWriteCommand( SX126X_CLR_DEVICE_ERRORS, buf, 2 );
}

/**
 * @brief  It is advised to add the following commands after ANY Rx with
 * Timeoutactive sequence, which stop the RTC and clear the timeout event, if
 * any. The register at address 0x0902 will be used to stop the counter, while
 * the register at address 0x0944 will clear the potential event.
 */
void SX126xClearTimeoutEvent( void )
{
    uint8_t reg_value = 0;

    /* WORKAROUND - Implicit Header Mode Timeout Behavior, see DS_SX1261-2_V1.2
    datasheet chapter 15.3 RegRtcControl = @address 0x0902 */
    SX126xWriteRegister( SX126X_REG_RTC_CTRL, reg_value );

    /* RegEventMask = @address 0x0944 */
    reg_value = SX126xReadRegister( SX126X_REG_EVT_CLR );
    reg_value |= SX126X_REG_EVT_CLR_TIMEOUT_MASK;
    SX126xWriteRegister( SX126X_REG_EVT_CLR, reg_value );
    /* WORKAROUND END */
}

/**
 * @brief  126x设置DIO2作为高频开关控制源
 * @param  enable 0:不作为控制源
 *                1:作为控制源
 *                  DIO2 = 0 in SLEEP, STDBY_RX, SX126X_STDBY_XOSC, FS and RX modes
 *                  DIO2 = 1 in TX mode
 */
void SX126xSetDio2AsRfSwitchCtrl( bool enable )
{
    uint8_t buf[ SX126X_SIZE_SET_DIO2_AS_RF_SWITCH_CTRL ];

    buf[ 0 ] = ( enable ? 1 : 0 );

    SX126xWriteCommand( SX126X_SET_DIO2_AS_RF_SWITCH_CTRL, buf, SX126X_SIZE_SET_DIO2_AS_RF_SWITCH_CTRL );
}

/**
 * @brief  126x设置DIO3作为有源晶振电压源
 * @param  tcxoVoltage 电压
 * @param  timeout 超时时间
 */
void SX126xSetDio3AsTcxoCtrl( SX126xTcxoCtrlVoltage_t tcxoVoltage, uint32_t timeout )
{
    uint8_t buf[ SX126X_SIZE_SET_DIO3_AS_TCXO_CTRL ];

    if( timeout > 0xFFFFFF )
        timeout = 0xFFFFFF;

    buf[ 0 ] = tcxoVoltage & 0x07;
    buf[ 1 ] = (uint8_t) ( ( timeout >> 16 ) & 0xFF );
    buf[ 2 ] = (uint8_t) ( ( timeout >> 8 ) & 0xFF );
    buf[ 3 ] = (uint8_t) ( timeout & 0xFF );

    SX126xWriteCommand( SX126X_SET_DIO3_AS_TCXO_CTRL, buf, SX126X_SIZE_SET_DIO3_AS_TCXO_CTRL );
}

/**
 * @brief  获取126x接收模式下的瞬时RSSI
 * @return int8_t 瞬时RSSI值
 */
int8_t SX126xGetRssiInst( void )
{
    uint8_t buf[ SX126X_SIZE_GET_RSSI_INST ];
    int8_t  rssi = 0;

    SX126xReadCommand( SX126X_GET_RSSI_INST, buf, SX126X_SIZE_GET_RSSI_INST );
    rssi = (int8_t) ( -buf[ 0 ] >> 1 );

    return rssi;
}

/**
 * @brief
 * @param  payloadLength 数据长度
 */

/**
 * @brief  获取126xRxBuffer信息
 * @param  bufferStatus 数据长度+数据地址偏移量
 */
void SX126xGetRxBufferStatus( SX126xRxBufferStatus_t* bufferStatus )
{
    uint8_t buf[ SX126X_SIZE_GET_RX_BUFFER_STATUS ];

    SX126xReadCommand( SX126X_GET_RX_BUFFER_STATUS, buf, SX126X_SIZE_GET_RX_BUFFER_STATUS );

    bufferStatus->sx126xPayloadLen         = buf[ 0 ];
    bufferStatus->sx126xBufferStartPointer = buf[ 1 ];

    // /*In case of LORA fixed header, the payloadLength is obtained by reading
    // the register SX126X_REG_LR_PAYLOADLENGTH*/
    // //这里的判断其实和外层判断报头为显隐性是一样的，这两个寄存器规格书中未给出，但是验证是等效成立的。
    // if( ( SX126xReadRegister( SX126X_REG_LR_PACKETPARAMS ) >> 7 == 1 ) )
    // {
    //     bufferStatus->sx126xPayloadLen = SX126xReadRegister( SX126X_REG_LR_PAYLOADLENGTH );
    // }
    // else
    // {
    //     *payloadLength = status[ 0 ];
    // }
}

/**
 * @brief  该函数主要用于防止当低温等情况出现PLL Lock等异常时，
 * 可能出现RF无法工作，及时清除错误标志及进行校准
 */
void SX126xCalibrateErr( void )
{
    SX126xDeviceError_t       error;
    SX126xCalibrationParams_t calibParam;

    error = SX126xGetDeviceErrors();
    if( error.sx126xErrValue != 0 )
    {
        SX126xClearDeviceErrors();

        calibParam.sx126xCalValue = 0x7F;
        SX126xCalibrate( calibParam );
    }
}

/**
 * @brief  126x进入TX
 * @param  timeout TX超时时间
 */
static void SX126xSetTx( uint32_t timeout )
{
    uint8_t buf[ SX126X_SIZE_SET_TX ];

    if( timeout > 0xFFFFFF )
        timeout = 0xFFFFFF;

    buf[ 0 ] = (uint8_t) ( ( timeout >> 16 ) & 0xFF );
    buf[ 1 ] = (uint8_t) ( ( timeout >> 8 ) & 0xFF );
    buf[ 2 ] = (uint8_t) ( timeout & 0xFF );

    SX126xWriteCommand( SX126X_SET_TX, buf, SX126X_SIZE_SET_TX );
    s_sx126xOperatingMode = SX126X_MODE_TX;
}

/**
 * @brief  126x进入RX
 * @param  timeout RX超时时间,单位1为15.625us。0表示单次接收，接收完成后
 * 自动退出RX，0xFFFFFF表示持续接收，不会超时，其他表示超时时间，超时后
 * 会退出RX
 */
void SX126xSetRx( uint32_t timeout )
{
    uint8_t buf[ SX126X_SIZE_SET_RX ];

    if( timeout > 0xFFFFFF )
        timeout = 0xFFFFFF;

    buf[ 0 ] = (uint8_t) ( ( timeout >> 16 ) & 0xFF );
    buf[ 1 ] = (uint8_t) ( ( timeout >> 8 ) & 0xFF );
    buf[ 2 ] = (uint8_t) ( timeout & 0xFF );

    SX126xWriteCommand( SX126X_SET_RX, buf, SX126X_SIZE_SET_RX );
    s_sx126xOperatingMode = SX126X_MODE_RX;
}

/**
 * @brief  126x RX电路形式选择设置
* @param  frequency RX工作的频率。根据频率设置接收电路，Low Band(430~510)设置为单端 RFI_N电路模式;High band(902~928MH)设置为单端 RFI_P电路模式;

 */
void SX126xSetRxSelect( uint32_t frequency )
{
  	if(frequency < 525 * 1000000 )
		{
			SX126xWriteRegister( 0x08E2, (SX126xReadRegister(0x08E2)|0x01) & 0xFD);//接收配置成单端 RFI_N电路模式,1:0=0x1；硬件电路决定的;
//		 SX126xWriteRegister( 0x08E2, (SX126xReadRegister(0x08E2)|0x03) );//接收配置成差分,1:0=0x3；SX126X默认适配差分电路;
		}
	else
		{
			 SX126xWriteRegister( 0x08E2, (SX126xReadRegister(0x08E2)|0x02) & 0xFE);//接收配置成单端 RFI_P电路模式,1:0=0x2；
//			 SX126xWriteRegister( 0x08E2, (SX126xReadRegister(0x08E2)|0x03) );//接收配置成差分,1:0=0x3；SX126X默认适配差分电路;
		}  
}


/**
 * @brief  126x发送数据
 * @param  payload 数据包首地址
 * @param  size 数据包大小
 * @param  timeout 发送超时时间
 */
void SX126xSendPayload( uint8_t* payload, uint8_t size, uint32_t timeout )
{
    if( payload == NULL || size == 0 )
        return;

    if( timeout > 0xFFFFFF )
        timeout = 0xFFFFFF;

    SX126xWriteFifo( s_sx126xTxFifoBaseAddr, payload, size );
    SX126xSetTx( timeout );
}

/**
 * @brief  126x发射连续单载波
 */
void SX126X_TxContinuousWave( void )
{
    SX126xWriteCommand( SX126X_SET_TX_CONTINUOUS_WAVE, NULL, SX126X_SIZE_SET_TX_CONTINUOUS_WAVE );
    s_sx126xOperatingMode = SX126X_MODE_TX;
}

/**
 * @brief  126X发送连续前导码
 */
void SX1262_TxInfinitePreamble( void )
{
    SX126xWriteCommand( SX126X_SET_TX_INFINITE_PREAMBLE, NULL, SX126X_SIZE_SET_TX_INFINITE_PREAMBLE );
    s_sx126xOperatingMode = SX126X_MODE_TX;
}

/**
 * @brief  126x获取FIFO内的数据
 * @param  buffer 数据存放的首地址
 * @param  size 数据大小
 */
void SX126xGetPayload( uint8_t* buffer, uint8_t size )
{
    if( buffer == NULL || size == 0 )
        return;

    SX126xReadFifo( s_sx126xRxFifoBaseAddr, buffer, size );
}

/**
 * @brief  获取数据包的RSSI和SNR信息
 * @param  avgPktRssi avgRSSI数据地址。实际信号的RSSI，当有用信号强敌低于底噪，该RSSI会是噪声的RSSI
 * @param  lastPktRssi lastRSSI数据底噪。解调后有用信号的RSSI评估，当信号强度高于底噪，该RSSI接近与
 * avgPktRssi，当有用信号低于底噪时，该RSSI更接近真实有用信号的RSSI
 * @param  snr 信噪比数据地址
 */
void SX126xGetPktStatus( SX126xLoraPktStatus_t* pktStatus )
{
    uint8_t buf[ SX126X_SIZE_GET_PKT_STATUS ];

    if( pktStatus == NULL )
        return;

    SX126xReadCommand( SX126X_GET_PKT_STATUS, buf, 3 );

    pktStatus->sx126xLoRaPktRssi       = (int8_t) ( -buf[ 0 ] >> 1 );
    pktStatus->sx126xLoRaPktSnr        = ( ( (int8_t) buf[ 1 ] ) + 2 ) >> 2;
    pktStatus->sx126xLoRaSignalPktRssi = (int8_t) ( -buf[ 2 ] >> 1 );
}

/**
 * @brief  SX126x进入休眠
 * @param  sleepMode 休眠模式，冷启动或者热启动，冷启动会丢失所有寄存器信息，唤醒后需要重新初始化参数
 */
void SX126xSleep( SX162xSleepStartType_t sleepMode )
{
    SX126xSleepParams_t sleepParams = { 0 };
    uint8_t             buf[ SX126X_SIZE_SET_SLEEP ];

    sleepParams.Fields.sx126xWarmStart = sleepMode;
    buf[ 0 ]                           = sleepParams.sx126xSleepValue;

    SX126xWriteCommand( SX126X_SET_SLEEP, buf, SX126X_SIZE_SET_SLEEP );
    s_sx126xOperatingMode = SX126X_MODE_SLEEP;
}

/**
 * @brief  配置内部晶振负载电容
 * @param  xtal_cap  晶振负载电容值，XTA和XTB采用同一值
 */
void SX126xSetXtalCap( uint8_t xtalCap )
{
    SX126xWriteRegister( SX126X_REG_XTATRIM, xtalCap );
    SX126xWriteRegister( SX126X_REG_XTBTRIM, xtalCap );
}

/**
 * @brief Configure the Tx PA clamp
 *
 * @remark Workaround - With a SX1262, during the chip initialization, calling this function optimizes the PA clamping
 * threshold. The call must be done after a Power On Reset or a wake-up from cold start (see DS_SX1261-2_V1.2 datasheet
 * chapter 15.2)
 */
void SX126xTxClampConfig( void )
{
    uint8_t regValue;

    regValue = SX126xReadRegister( SX126X_REG_TX_CLAMP_CFG );

    regValue |= SX126X_REG_TX_CLAMP_CFG_MASK;
    SX126xWriteRegister( SX126X_REG_TX_CLAMP_CFG, regValue );
}

/**
 * @brief  SX126X SPI通信测试，对某个寄存器写入一个值后读取这个寄存器，
 * 读取返回的内容和写入内容一直则表示SPI通信正常，反之异常,此函数请确保
 * 在芯片参数配置前进行。
 * @return true SPI通信正常
 * @return false SPI通信异常
 */
bool SX126xSpiTest( void )
{
    uint8_t regValue = 0;

    SX126xWriteRegister( SX126X_REG_LR_PAYLOADLENGTH, 0x19 );
    regValue = SX126xReadRegister( SX126X_REG_LR_PAYLOADLENGTH );

    return ( ( regValue == 0x19 ) ? true : false );
}

/**
 * @brief  SX126x开启Rx Boosted gain
 */
void SX126xEnableRxBootedGain( void )
{
    SX126xWriteRegister( SX126X_REG_RXGAIN, 0x96 );

    /*将RX增益开启功能保存，避免休眠唤醒后RX增益关闭*/
    SX126xWriteRegister( SX126X_REG_RETENTION_LIST_BASE_ADDRESS + 0, 0x01 );
    SX126xWriteRegister( SX126X_REG_RETENTION_LIST_BASE_ADDRESS + 1, 0x08 );
    SX126xWriteRegister( SX126X_REG_RETENTION_LIST_BASE_ADDRESS + 2, 0xAC );
}

/**
 * @brief  Optimizing the Inverted IQ Operation, see datasheet DS_SX1261-2_V1.2 §15.4
 * @param  iq IQ模式
 */
void SX126xIQConfig( SX162xLoRaIQModes_t iq )
{
    uint8_t reg_value;

    reg_value = SX126xReadRegister( SX126X_REG_IQ_POLARITY );

    if( iq == SX162X_LORA_IQ_NORMAL )
        reg_value &= ~( 1 << 2 );    ///< Bit 2 set to 0 when using inverted IQ polarity
    else
        reg_value |= ( 1 << 2 );    ///< Bit 2 set to 1 when using standard IQ polarity

    SX126xWriteRegister( SX126X_REG_IQ_POLARITY, reg_value );
}

/**
 * @brief  SX126x配置CAD相关参数
 * @param  cadSymbs     监测的LoRa symble数量，详见SX126x数据手册
 * @param  cadDetPeak   影响CAD灵敏度，见SX126x规格书说明和勘误手册
 * @param  cadDetMin    影响CAD灵敏度，见SX126x规格书说明和勘误手册
 * @param  cadExitMode  CAD监测到活跃信号后的模式状态，详见SX126x数据手册
 * @param  cadTimeOut   当@cadExitMode选择SX126X_CAD_RX后有效，详见SX126x数据手册
 */
void SX126xSetCadParams( SX126xCadSymbs_t     cadSymbs,
                         uint8_t              cadDetPeak,
                         uint8_t              cadDetMin,
                         SX126xCadExitModes_t cadExitMode,
                         uint32_t             cadTimeOut )
{
    uint8_t buf[ SX126X_SIZE_SET_CAD_PARAMS ];

    if( cadTimeOut > 0xFFFFFF )
        cadTimeOut = 0xFFFFFF;

    buf[ 0 ] = (uint8_t) cadSymbs;
    buf[ 1 ] = cadDetPeak;
    buf[ 2 ] = cadDetMin;
    buf[ 3 ] = (uint8_t) cadExitMode;
    buf[ 4 ] = (uint8_t) ( cadTimeOut >> 16 );
    buf[ 5 ] = (uint8_t) ( cadTimeOut >> 8 );
    buf[ 6 ] = (uint8_t) ( cadTimeOut >> 0 );

    SX126xWriteCommand( SX126X_SET_CAD_PARAMS, buf, SX126X_SIZE_SET_CAD_PARAMS );
}

/**
 * @brief  SX126x 进入CAD模式
 */
void SX126xSetCad( void )
{
    SX126xWriteCommand( SX126X_SET_CAD, NULL, SX126X_SIZE_SET_CAD );
    s_sx126xOperatingMode = SX126X_MODE_CAD;
}

/**
 * @brief  SX126x 设置网络类型
 * @param  network 网络类型
 */
void SX126xSetNetworkType( SX126xLoRaNetwork_t network )
{
    SX126xWriteRegister( SX126X_REG_LR_SYNCWORD, ( network >> 8 ) & 0xFF );
    SX126xWriteRegister( SX126X_REG_LR_SYNCWORD + 1, network & 0xFF );
}

/**
 * @brief  获取126x SPI工作状态
 * @return true SPI正在被占用
 * @return false SPI未被占用
 */
bool GetSX126xSpiState( void )
{
    return s_sx126xSpiUseFlag;
}

/**
 * @brief  SX126x 设置芯片在TX RX后自动回到哪个模式
 * @param  fallbackMode FALLBACK_FS/ FALLBACK_STDBY_XOSC/ FALLBACK_STDBY_RC
 */
void SX126xSetFallBackMode( SX126xFallBackMode_t fallbackMode )
{
    uint8_t buf[ SX126X_SIZE_SET_RX_TX_FALLBACK_MODE ];

    buf[ 0 ] = fallbackMode;

    SX126xWriteCommand( SX126X_SET_RX_TX_FALLBACK_MODE, buf, SX126X_SIZE_SET_RX_TX_FALLBACK_MODE );
}

/**
 * @brief  获取BUSY是否存在等待超时
 * @return true BUSY在上次指令出现了超时
 * @return false BUSY未出现超时
 */
bool SX126xGetIsBusyTimeout( void )
{
    return ( ! s_waitBusyTimeout );
}
