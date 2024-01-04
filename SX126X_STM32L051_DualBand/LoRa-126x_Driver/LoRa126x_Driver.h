#ifndef _LORA_126X_DRIVER_H__
#define _LORA_126X_DRIVER_H__

#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#define POWER_MAX                           22    ///< 功率底层可写入的最大值
#define POWER_MIN                           -3    ///< 功率底层可写入的最小值

#define XTAL_FREQ                           (double) 32000000                    ///< 晶振频率
#define FREQ_DIV                            (double) 33554432                    ///< pow( 2.0, 25.0 )
#define FREQ_STEP                           (double) ( XTAL_FREQ / FREQ_DIV )    ///< 频率步进

/*所有校准功能块均启用校准*/
#define SX126X_CALIBRATION_PARAM_ALL_ENABLE 0x7F

/**
 * @brief  SX126x over current protection enumeration definition
 */
typedef enum
{
    SX126X_OCP_60MA  = 0x18,
    SX126X_OCP_140MA = 0x38,
} SX126xOcpValue_t;

/**
 * @brief  SX126x low data rate optimize enumeration definition
 */
typedef enum
{
    SX126X_LOW_DATARATE_OPT_OFF = 0x00,
    SX126X_LOW_DATARATE_OPT_ON  = 0x01,
} SX126xLowDataRateOptimize_t;

/**
 * @brief  SX126X TCXO control voltages enumeration definition
 */
typedef enum
{
    SX126X_TCXO_CTRL_1_6V = 0x00,
    SX126X_TCXO_CTRL_1_7V = 0x01,
    SX126X_TCXO_CTRL_1_8V = 0x02,
    SX126X_TCXO_CTRL_2_2V = 0x03,
    SX126X_TCXO_CTRL_2_4V = 0x04,
    SX126X_TCXO_CTRL_2_7V = 0x05,
    SX126X_TCXO_CTRL_3_0V = 0x06,
    SX126X_TCXO_CTRL_3_3V = 0x07,
} SX126xTcxoCtrlVoltage_t;

/**
 * @brief  SX126x mode enumeration definition
 */
typedef enum
{
    SX126X_MODE_SLEEP = 0x00,    ///< The radio is in sleep mode
    SX126X_MODE_STDBY_RC,        ///< The radio is in standby mode with RC oscillator
    SX126X_MODE_STDBY_XOSC,      ///< The radio is in standby mode with XOSC oscillator
    SX126X_MODE_FS,              ///< The radio is in frequency synthesis mode
    SX126X_MODE_TX,              ///< The radio is in transmit mode
    SX126X_MODE_RX,              ///< The radio is in receive mode
    SX126X_MODE_RX_DC,           ///< The radio is in receive duty cycle mode
    SX126X_MODE_CAD              ///< The radio is in channel activity detection mode
} SX126xOperatingModes_t;

/*!
 * \brief Represents a calibration configuration
 */
typedef union
{
    struct
    {
        uint8_t sx126xRC64KEnable    : 1;    ///< Calibrate RC64K clock
        uint8_t sx126xRC13MEnable    : 1;    ///< Calibrate RC13M clock
        uint8_t sx126xPLLEnable      : 1;    ///< Calibrate PLL
        uint8_t sx126xADCPulseEnable : 1;    ///< Calibrate ADC Pulse
        uint8_t sx126xADCBulkNEnable : 1;    ///< Calibrate ADC bulkN
        uint8_t sx126xADCBulkPEnable : 1;    ///< Calibrate ADC bulkP
        uint8_t sx126xImgEnable      : 1;
        uint8_t                      : 1;
    } Fields;
    uint8_t sx126xCalValue;
} SX126xCalibrationParams_t;

/**
 * @brief  SX126X APIs return status enumeration definition
 */
typedef enum
{
    SX126X_NORMAL,                 ///< 正常
    SX126X_PARAMETER_INVALID,      ///< 参数不可用
    SX126X_SPI_READCHECK_WRONG,    ///< SPI出错
} SX126xError_t;

/**
 * @brief  SX126X standby modes enumeration definition
 */
typedef enum
{
    SX126X_STDBY_RC   = 0x00,
    SX126X_STDBY_XOSC = 0x01,
} SX126xStandbyModes_t;

/**
 * @brief  SX126X power regulator modes enumeration definition
 */
typedef enum
{
    SX126X_USE_LDO  = 0x00,    ///< default
    SX126X_USE_DCDC = 0x01,
} SX126xRegulatorMode_t;

/**
 * @brief  SX126X power amplifier ramp-up timings enumeration definition
 */
typedef enum
{
    SX126X_RAMP_10_US   = 0x00,
    SX126X_RAMP_20_US   = 0x01,
    SX126X_RAMP_40_US   = 0x02,
    SX126X_RAMP_80_US   = 0x03,
    SX126X_RAMP_200_US  = 0x04,
    SX126X_RAMP_800_US  = 0x05,
    SX126X_RAMP_1700_US = 0x06,
    SX126X_RAMP_3400_US = 0x07,
} sx126xRampTimes_t;

/**
 * @brief SX126X interrupt masks enumeration definition
 */
typedef enum SX126xIrqMasks_e
{
    SX126X_IRQ_NONE              = ( 0 << 0 ),
    SX126X_IRQ_TX_DONE           = ( 1 << 0 ),
    SX126X_IRQ_RX_DONE           = ( 1 << 1 ),
    SX126X_IRQ_PREAMBLE_DETECTED = ( 1 << 2 ),
    SX126X_IRQ_SYNC_WORD_VALID   = ( 1 << 3 ),
    SX126X_IRQ_HEADER_VALID      = ( 1 << 4 ),
    SX126X_IRQ_HEADER_ERROR      = ( 1 << 5 ),
    SX126X_IRQ_CRC_ERROR         = ( 1 << 6 ),
    SX126X_IRQ_CAD_DONE          = ( 1 << 7 ),
    SX126X_IRQ_CAD_DETECTED      = ( 1 << 8 ),
    SX126X_IRQ_TIMEOUT           = ( 1 << 9 ),
    SX126X_IRQ_LR_FHSS_HOP       = ( 1 << 14 ),
    SX126X_IRQ_ALL =
        ( SX126X_IRQ_TX_DONE | SX126X_IRQ_RX_DONE | SX126X_IRQ_PREAMBLE_DETECTED | SX126X_IRQ_SYNC_WORD_VALID |
          SX126X_IRQ_HEADER_VALID | SX126X_IRQ_HEADER_ERROR | SX126X_IRQ_CRC_ERROR | SX126X_IRQ_CAD_DONE |
          SX126X_IRQ_CAD_DETECTED | SX126X_IRQ_TIMEOUT | SX126X_IRQ_LR_FHSS_HOP ),
} SX126xIrqMasks_t;

/**
 * @brief SX126X packet types enumeration definition
 */
typedef enum
{
    SX126X_PKT_TYPE_GFSK    = 0x00,
    SX126X_PKT_TYPE_LORA    = 0x01,
    SX126X_PKT_TYPE_LR_FHSS = 0x03,
} SX126xPktType_t;

/*!
 * \brief Represents the CRC mode for LoRa packet type
 */
typedef enum
{
    SX126X_LORA_CRC_OFF = 0x00,    ///< CRC not used
    SX126X_LORA_CRC_ON  = 0x01,    ///< CRC activated
} SX126xLoRaCrcModes_t;

/*!
 * \brief Represents the IQ mode for LoRa packet type
 */
typedef enum
{
    SX162X_LORA_IQ_NORMAL   = 0x00,
    SX162X_LORA_IQ_INVERTED = 0x01,
} SX162xLoRaIQModes_t;

/**
 * @brief SX126X LoRa packet length enumeration definition
 */
typedef enum
{
    SX126X_LORA_PKT_EXPLICIT = 0x00,    ///< Header included in the packet
    SX126X_LORA_PKT_IMPLICIT = 0x01,    ///< Header not included in the packet
} SX126xLoraPktHeadType_t;

/*!
 * \brief Represents the possible spreading factor values in LoRa packet types
 */
typedef enum
{
    SX126X_LORA_SF5  = 0x05,
    SX126X_LORA_SF6  = 0x06,
    SX126X_LORA_SF7  = 0x07,
    SX126X_LORA_SF8  = 0x08,
    SX126X_LORA_SF9  = 0x09,
    SX126X_LORA_SF10 = 0x0A,
    SX126X_LORA_SF11 = 0x0B,
    SX126X_LORA_SF12 = 0x0C,
} SX126xLoRaSpreadingFactors_t;

/*!
 * \brief Represents the bandwidth values for LoRa packet type
 */
typedef enum
{
    SX126X_LORA_BW_500 = 6,
    SX126X_LORA_BW_250 = 5,
    SX126X_LORA_BW_125 = 4,
    SX126X_LORA_BW_062 = 3,
    SX126X_LORA_BW_041 = 10,
    SX126X_LORA_BW_031 = 2,
    SX126X_LORA_BW_020 = 9,
    SX126X_LORA_BW_015 = 1,
    SX126X_LORA_BW_010 = 8,
    SX126X_LORA_BW_007 = 0,
} SX126xLoRaBandwidths_t;

/*!
 * \brief Represents the coding rate values for LoRa packet type
 */
typedef enum
{
    SX126X_LORA_CR_4_5 = 0x01,
    SX126X_LORA_CR_4_6 = 0x02,
    SX126X_LORA_CR_4_7 = 0x03,
    SX126X_LORA_CR_4_8 = 0x04,
} SX126xLoRaCodingRates_t;

/*!
 * \brief Represents a sleep mode configuration
 */
typedef union
{
    struct
    {
        uint8_t sx126xWakeUpRTC : 1;    ///< Get out of sleep mode if wakeup signal
                                        ///< received from RTC
        uint8_t sx126xReset     : 1;
        uint8_t sx126xWarmStart : 1;
        uint8_t Reserved        : 5;
    } Fields;
    uint8_t sx126xSleepValue;
} SX126xSleepParams_t;

/**
 * @brief SX126X sleep mode configurations definition
 */
typedef enum
{
    SX126X_SLEEP_COLD_START = 0,
    SX126X_SLEEP_WARM_START = 1,
} SX162xSleepStartType_t;

/**
 * @brief SX126X LoRa CAD number of symbols enumeration definition
 *
 * @note Represents the number of symbols to be used for a CAD operation
 */
typedef enum
{
    SX126X_CAD_01_SYMB = 0x00,
    SX126X_CAD_02_SYMB = 0x01,
    SX126X_CAD_04_SYMB = 0x02,
    SX126X_CAD_08_SYMB = 0x03,
    SX126X_CAD_16_SYMB = 0x04,
} SX126xCadSymbs_t;

/**
 * @brief SX126X LoRa CAD exit modes enumeration definition
 *
 * @note Represents the action to be performed after a CAD is done
 */
typedef enum
{
    SX126X_CAD_ONLY = 0x00,
    SX126X_CAD_RX   = 0x01,
    SX126X_CAD_LBT  = 0x10,
} SX126xCadExitModes_t;

typedef enum
{
    SX126X_PUBLIC_NETWORK  = 0x3444,    ///< for LoRaWan
    SX126X_PRIVATE_NETWORK = 0x1424,
} SX126xLoRaNetwork_t;

/**
 * @brief SX126X LoRa packet status structure definition
 */
typedef struct
{
    int8_t sx126xLoRaPktRssi;          ///< RSSI of the last packet
    int8_t sx126xLoRaPktSnr;           ///< SNR of the last packet
    int8_t sx126xLoRaSignalPktRssi;    ///< Estimation of RSSI (after despreading)
} SX126xLoraPktStatus_t;

/*!
 * \brief Represents the possible radio system error states
 */
typedef union
{
    struct
    {
        uint8_t sx126xRC64kCalibErr : 1;    ///< RC 64kHz oscillator calibration failed
        uint8_t sx126xRC13mCalibErr : 1;    ///< RC 13MHz oscillator calibration failed
        uint8_t sx126xPllCalibErr   : 1;    ///< PLL calibration failed
        uint8_t sx126xAdcCalibErr   : 1;    ///< ADC calibration failed
        uint8_t sx126xImgCalibErr   : 1;    ///< Image calibration failed
        uint8_t sx126xXoscStartErr  : 1;    ///< XOSC oscillator failed to start
        uint8_t sx126xPllLockErr    : 1;    ///< PLL lock failed
        uint8_t                     : 1;    ///< Buck converter failed to start
        uint8_t sx126xPaRampErr     : 1;    ///< PA ramp failed
        uint8_t                     : 7;    ///< Reserved
    } Fields;
    uint16_t sx126xErrValue;
} SX126xDeviceError_t;

/**
 * @brief SX126X RX buffer status structure definition
 */
typedef struct
{
    uint8_t sx126xPayloadLen;            ///< Number of bytes available in the buffer
    uint8_t sx126xBufferStartPointer;    ///< Position of the first byte in the buffer
} SX126xRxBufferStatus_t;

typedef enum
{
    FALLBACK_FS         = 0x40,
    FALLBACK_STDBY_XOSC = 0x30,
    FALLBACK_STDBY_RC   = 0x20,
} SX126xFallBackMode_t;

void                SX126xWakeup( void );
void                SX126xWriteFifo( uint8_t offset, uint8_t* buffer, uint8_t size );
void                SX126xReadFifo( uint8_t offset, uint8_t* buffer, uint8_t size );
void                SX126xSetStandby( SX126xStandbyModes_t standbyConfig );
void                SX126xSetPacketType( SX126xPktType_t packetType );
void                SX126xSetRegulatorMode( SX126xRegulatorMode_t mode );
void                SX126xSetBufferBaseAddress( uint8_t txBaseAddress, uint8_t rxBaseAddress );
void                SX126xSetFifoBaseAddr( void );
void                SX126xCalibrate( SX126xCalibrationParams_t calibParam );
void                SX126xCalibrateImage( void );
void                SX126xSetDioIrqParams( uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask );
void                SX126xSetTxMaxWithoutOptimalPaParams( sx126xRampTimes_t rampTime );
void                SX126xSetRfTxPower( int8_t power );
void                SX126xSetStopRxTimerOnPreambleDetect( bool enable );
void                SX126xSetLoRaSymbNumTimeout( uint8_t SymbNum );
void                SX126xClearIrqStatus( uint16_t irq );
void                SX126xClearDeviceErrors( void );
void                SX126xClearTimeoutEvent( void );
void                SX126xSetDio2AsRfSwitchCtrl( bool enable );
void                SX126xSetDio3AsTcxoCtrl( SX126xTcxoCtrlVoltage_t tcxoVoltage, uint32_t timeout );
void                SX126xGetRxBufferStatus( SX126xRxBufferStatus_t* bufferStatus );
void                SX126xSetRx( uint32_t timeout );
void                SX126xSendPayload( uint8_t* payload, uint8_t size, uint32_t timeout );
void                SX126X_TxContinuousWave( void );
void                SX1262_TxInfinitePreamble( void );
void                SX126xGetPayload( uint8_t* buffer, uint8_t size );
void                SX126xGetPktStatus( SX126xLoraPktStatus_t* pktStatus );
void                SX126xSleep( SX162xSleepStartType_t sleepMode );
void                SX126xSetXtalCap( uint8_t xtalCap );
void                SX126xTxClampConfig( void );
bool                SX126xSpiTest( void );
void                SX126xEnableRxBootedGain( void );
void                SX126xIQConfig( SX162xLoRaIQModes_t iq );
void                SX126xSetCad( void );
void                SX126xSetNetworkType( SX126xLoRaNetwork_t network );
void                SX126xCalibrateErr( void );
bool                GetSX126xSpiState( void );
bool                SX126xGetIsBusyTimeout( void );
int8_t              SX126xGetRssiInst( void );
uint16_t            SX126xGetIrqStatus( void );
SX126xError_t       SX126xSetRfFrequency( uint32_t frequency );
SX126xPktType_t     SX126xGetPacketType( void );
SX126xDeviceError_t SX126xGetDeviceErrors( void );
void                SX126xSetFallBackMode( SX126xFallBackMode_t fallbackMode );
void                SX126xSetCadParams( SX126xCadSymbs_t     cadSymbs,
                                        uint8_t              cadDetPeak,
                                        uint8_t              cadDetMin,
                                        SX126xCadExitModes_t cadExitMode,
                                        uint32_t             cadTimeOut );
void                SX126xSetLoraModulationParams( SX126xLoRaSpreadingFactors_t sf,
                                                   SX126xLoRaBandwidths_t       bw,
                                                   SX126xLoRaCodingRates_t      cr,
                                                   bool                         lowDataRateOpt );
void                SX126xSetLoraPacketParams( uint16_t                preamble,
                                               SX126xLoraPktHeadType_t headerType,
                                               uint8_t                 payloadLen,
                                               SX126xLoRaCrcModes_t    crcMode,
                                               SX162xLoRaIQModes_t     invertIqMode );

#endif
