#ifndef _LORA_126X_FUN_H__
#define _LORA_126X_FUN_H__

#include "LoRa126x_Driver.h"
#include "LoRa126x_it.h"
#include "LoRa126x_UserConfig.h"

#define MAX_RXDATA_LEN                 255    ///< 最大接收长度
#define MAX_TXDATA_LEN                 255    ///< 最大发射长度

#define RF_RX_CONTINUE_TIME            262143    ///< RX持续模式时间

#define SX126X_TRX_TIMEOUT_PER_TIME_US 15.625    ///< TX RX超时每单位1的时间(us)

#define SX126X_TX_TIMEOUT_50_S         ( uint32_t )( 50 * 1000000 / SX126X_TRX_TIMEOUT_PER_TIME_US )    ///< TX50秒超时

/*有源晶振起振超时时间*/
#define SX126X_TCXO_TIMEOUT            ( uint32_t )( LORA_TCXO_TIMEOUT_TIME_MS * 1000 / SX126X_TRX_TIMEOUT_PER_TIME_US )

/*SX126X IRQ配置*/
/*TX模式IRQ功能和映射*/
#define SX126X_TX_IRQ_ENABLE           ( SX126X_IRQ_TX_DONE | SX126X_IRQ_TIMEOUT )
#define SX126X_TX_IRQ_DIO1             ( SX126X_IRQ_TX_DONE | SX126X_IRQ_TIMEOUT )
#define SX126X_TX_IRQ_DIO2             ( SX126X_IRQ_NONE )
#define SX126X_TX_IRQ_DIO3             ( SX126X_IRQ_NONE )
/*RX模式IRQ功能和映射*/
#define SX126X_RX_IRQ_ENABLE           ( SX126X_IRQ_RX_DONE | SX126X_IRQ_CRC_ERROR | SX126X_IRQ_TIMEOUT )
#define SX126X_RX_IRQ_DIO1             ( SX126X_IRQ_RX_DONE | SX126X_IRQ_TIMEOUT )
#define SX126X_RX_IRQ_DIO2             ( SX126X_IRQ_NONE )
#define SX126X_RX_IRQ_DIO3             ( SX126X_IRQ_NONE )
/*CAD_ONLY模式IRQ功能和映射*/
#define SX126X_CAD_ONLY_IRQ_ENABLE     ( SX126X_IRQ_CAD_DONE | SX126X_IRQ_CAD_DETECTED )
#define SX126X_CAD_ONLY_IQR_DI01       ( SX126X_IRQ_CAD_DONE )
#define SX126X_CAD_ONLY_IQR_DI02       ( SX126X_IRQ_NONE )
#define SX126X_CAD_ONLY_IQR_DI03       ( SX126X_IRQ_NONE )
/*CAD_RX模式IRQ功能和映射*/
#define SX126X_CAD_RX_IRQ_ENABLE                                                                                       \
    ( SX126X_IRQ_RX_DONE | SX126X_IRQ_TIMEOUT | SX126X_IRQ_CAD_DONE | SX126X_IRQ_CAD_DETECTED )
#define SX126X_CAD_RX_IQR_DI01     ( SX126X_IRQ_RX_DONE | SX126X_IRQ_TIMEOUT | SX126X_IRQ_CAD_DONE )
#define SX126X_CAD_RX_IQR_DI02     ( SX126X_IRQ_NONE )
#define SX126X_CAD_RX_IQR_DI03     ( SX126X_IRQ_NONE )

/*CadDetPeak默认值*/
/*！！请不要修改本参数内容！！*/
#define SX126X_LORA_SF05_CADPEAK   22
#define SX126X_LORA_SF06_CADPEAK   22
#define SX126X_LORA_SF07_CADPEAK   22
#define SX126X_LORA_SF08_CADPEAK   23
#define SX126X_LORA_SF09_CADPEAK   25
#define SX126X_LORA_SF10_CADPEAK   26
#define SX126X_LORA_SF11_CADPEAK   26
#define SX126X_LORA_SF12_CADPEAK   30

/*CAD symbol number 默认值*/
/*！！请不要修改本参数内容！！*/
#define SX126X_LORA_SF05_CAD_SYMBS SX126X_CAD_04_SYMB
#define SX126X_LORA_SF06_CAD_SYMBS SX126X_CAD_02_SYMB
#define SX126X_LORA_SF07_CAD_SYMBS SX126X_CAD_02_SYMB
#define SX126X_LORA_SF08_CAD_SYMBS SX126X_CAD_02_SYMB
#define SX126X_LORA_SF09_CAD_SYMBS SX126X_CAD_02_SYMB
#define SX126X_LORA_SF10_CAD_SYMBS SX126X_CAD_02_SYMB
#define SX126X_LORA_SF11_CAD_SYMBS SX126X_CAD_02_SYMB
#define SX126X_LORA_SF12_CAD_SYMBS SX126X_CAD_02_SYMB

/*射频数据包结构体*/
typedef struct
{
    bool    recFlag;                      ///< 接收到新数据后会置为true，需要使用者手动置false
    uint8_t size;                         ///< payload大小
    uint8_t payload[ MAX_RXDATA_LEN ];    ///< 数据
    int8_t  rssi;                         ///< 当前包的rssi
    int8_t  snr;                          ///< 当前包的snr
} LoRaRxPkt_s;

/*射频状态枚举定义*/
typedef enum
{
    RF_STANDBY,    ///< idle(standby)状态
    RF_RX_ING,     ///< 接收状态
    RF_TX_ING,     ///< 发射状态
    RF_CAD_ING,    ///< CAD状态
    RF_SLEEP,      ///< 休眠状态
} rfState_t;

/*LoRa基础参数结构体*/
typedef struct
{
    uint32_t                     freq;           ///< 频率，单位Hz
    SX126xLoRaBandwidths_t       BW;             ///< BW
    SX126xLoRaSpreadingFactors_t SF;             ///< SF
    SX126xLoRaCodingRates_t      CR;             ///< CR
    int8_t                       power;          ///< 功率，单位dbm
    SX126xLoraPktHeadType_t      header;         ///< 包头类型
    SX126xLoRaCrcModes_t         crc;            ///< CRC开关
    SX162xLoRaIQModes_t          invertIQ;       ///< IQ模式
    uint16_t                     preambleLen;    ///< 前导码长度
    uint8_t                      payloadLen;     ///< 数据载荷长度
} SX126xLoRaConfig_s;

/*SX126X初始化API返回状态*/
typedef enum
{
    SX126X_LORA_INIT_OK,              ///< 初始化正常
    SX126X_LORA_INIT_SPI_ERROR,       ///< 初始化SPI通信异常
    SX126X_LORA_INIT_FREQ_INVALID,    ///< 初始化频率参数无效
    SX126X_LORA_BUSY_STATE_ERROR,     ///< BUSY状态读取异常
    SX126X_LORA_BUSY_TIMEOUT,         ///< BUSY等待超时
} SX126xLoRaInitStatus_t;

extern LoRaRxPkt_s g_LoRaRxPkt;
extern rfState_t   g_rfState;

//
SX126xLoRaInitStatus_t       SX126xLoRaInit( void );
void                         SX126xEnterStandby( void );
void                         SX126xSendData( uint8_t* data, uint8_t len );
void                         SX126xEnterRx( uint32_t timeout );
void                         SX126xGetRxPkt( void );
void                         SX126xStartCadOnly( void );
void                         SX126xStartCadRx( uint32_t rxTimeOut );
void                         SX126xEnterSleep( void );
void                         SX126xExitSleep( void );
bool                         SX126xSetFreq( uint32_t freq );
uint32_t                     SX126xGetFreq( void );
void                         SX126xSetPower( int8_t power );
void                         SX126xLowDataRateOptConfig( bool lowDataRateOpt );
void                         SX126xLoRaPreambleConfig( uint16_t preamble );
void                         SX126xLoRaCadPeakConfig( SX126xLoRaSpreadingFactors_t sf, uint8_t cadPeak );
uint8_t                      SX126xGetCadPeak( SX126xLoRaSpreadingFactors_t sf );
void                         SX126xLoRaCadSymbsConfig( SX126xLoRaSpreadingFactors_t sf, SX126xCadSymbs_t symbs );
SX126xCadSymbs_t             SX126xGetCadSymbs( SX126xLoRaSpreadingFactors_t sf );
SX126xCadExitModes_t         SX126xGetCadExitModes( void );
SX126xLoraPktHeadType_t      SX126xGetHeaderType( void );
bool                         SX126xGetIrqFlag( SX126xIrqFlagType_t irq );
void                         SX126xSendCarrierWave( void );
void                         SX126xSendLoRaInfinitePreamble( void );
int8_t                       SX126xGetInstantRssi( void );
void                         SX126xForceSetMaxPower( void );
SX126xLoRaBandwidths_t       SX126xGetBw( void );
SX126xLoRaSpreadingFactors_t SX126xGetSf( void );
SX126xLoRaCodingRates_t      SX126xGetCr( void );
int8_t                       SX126xGetPower( void );
SX126xLoraPktHeadType_t      SX126xGetHeader( void );
SX126xLoRaCrcModes_t         SX126xGetCrc( void );
SX162xLoRaIQModes_t          SX126xGetIqMode( void );
uint16_t                     SX126xGetPreambleLen( void );
bool                         SX126xGetLowDataOptState( void );
void                         SX126xSetSymbNumTimeout( uint8_t num );
void                         SX126xLoRaPacketConfig( uint16_t                preamble,
                                                     SX126xLoraPktHeadType_t headerType,
                                                     uint8_t                 payloadLen,
                                                     SX126xLoRaCrcModes_t    crcMode,
                                                     SX162xLoRaIQModes_t     invertIqMode );
bool                         SX126xLoRaModulationConfig( SX126xLoRaSpreadingFactors_t sf,
                                                         SX126xLoRaBandwidths_t       bw,
                                                         SX126xLoRaCodingRates_t      cr );
void                         LoRaUserConfig( uint32_t                     freq,
                                             SX126xLoRaBandwidths_t       bw,
                                             SX126xLoRaSpreadingFactors_t sf,
                                             SX126xLoRaCodingRates_t      cr,
                                             int8_t                       power,
                                             SX126xLoraPktHeadType_t      header,
                                             SX126xLoRaCrcModes_t         crc,
                                             SX162xLoRaIQModes_t          iq,
                                             uint16_t                     preambleLen,
                                             uint8_t                      payloadLen );
#ifdef SX126X_CRYSTAL_INTERNAL_CAP
uint8_t SX126xGetInternalXoCap( void );
void    SX126xSetInternalXoCap( uint8_t cap );
#endif

#endif
