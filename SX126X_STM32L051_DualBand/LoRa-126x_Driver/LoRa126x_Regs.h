#ifndef _LORA_126X_REGS_H__
#define _LORA_126X_REGS_H__

/*!
 * The address of the register holding Bit Sync configuration
 */
#define SX126X_REG_BIT_SYNC                    0x06AC

/**
 * @brief The address of the register holding the first byte defining the CRC seed
 */
#define SX126X_REG_CRCSEEDBASEADDRESS          0x06BC

/**
 * @brief The address of the register holding the first byte defining the CRC polynomial
 */
#define SX126X_REG_CRCPOLYBASEADDRESS          0x06BE

/**
 * @brief The address of the register holding the first byte defining the whitening seed
 */
#define SX126X_REG_WHITSEEDBASEADDRESS         0x06B8

/**
 * @brief The addresses of the registers holding SyncWords values
 */
#define SX126X_REG_SYNCWORDBASEADDRESS         0x06C0

/*!
 * \brief The address of the register holding the payload size
 */
#define SX126X_REG_LR_PAYLOADLENGTH            0x0702

/*!
 * \brief The address of the register holding the packet configuration
 */
#define SX126X_REG_LR_PACKETPARAMS             0x0704

/**
 * @brief The addresses of the register holding LoRa Modem SyncWord value
 *        0x1424: LoRaWAN private network,
 *        0x3444: LoRaWAN public network
 */
#define SX126X_REG_LR_SYNCWORD                 0x0740

/**
 * @brief The address of the register holding the coding rate configuration extracted from a received LoRa header
 */
#define SX126X_REG_LR_HEADER_CR                0x0749
#define SX126X_REG_LR_HEADER_CR_POS            ( 4U )
#define SX126X_REG_LR_HEADER_CR_MASK           ( 0x07UL << SX126X_REG_LR_HEADER_CR_POS )

/**
 * @brief The address of the register holding the CRC configuration extracted from a received LoRa header
 */
#define SX126X_REG_LR_HEADER_CRC               0x076B
#define SX126X_REG_LR_HEADER_CRC_POS           ( 4U )
#define SX126X_REG_LR_HEADER_CRC_MASK          ( 0x01UL << SX126X_REG_LR_HEADER_CRC_POS )

/*!
 * The address of the register giving a 32-bit random number
 */
#define SX126X_REG_RNGBASEADDRESS              0x0819

/*!
 * The address of the register used to disable the LNA
 */
#define SX126X_REG_ANA_LNA                     0x08E2

/*!
 * The address of the register used to disable the mixer
 */
#define SX126X_REG_ANA_MIXER                   0x08E5

/*!
 * The address of the register holding RX Gain value
 *     0x94: power saving,
 *     0x96: rx boosted
 */
#define SX126X_REG_RXGAIN                      0x08AC

/**
 * @brief Change the value on the device internal trimming capacitor
 */
#define SX126X_REG_XTATRIM                     0x0911
#define SX126X_REG_XTBTRIM                     0x0912

/**
 * @brief Set the current max value in the over current protection
 */
#define SX126X_REG_OCP                         0x08E7

/*!
 * Set the PA clamping threshold
 */
#define SX162X_REG_TxClampConfig               0x08D8

/**
 * @brief WORKAROUND - Optimizing the Inverted IQ Operation, see DS_SX1261-2_V1.2 datasheet chapter 15.4
 */
#define SX126X_REG_IQ_POLARITY                 0x0736

/**
 * @brief WORKAROUND - Modulation Quality with 500 kHz LoRa Bandwidth, see DS_SX1261-2_V1.2 datasheet chapter 15.1
 */
#define SX126X_REG_TX_MODULATION               0x0889

/**
 * @brief WORKAROUND - Better resistance to antenna mismatch, see DS_SX1261-2_V1.2 datasheet chapter 15.2
 */
#define SX126X_REG_TX_CLAMP_CFG                0x08D8
#define SX126X_REG_TX_CLAMP_CFG_POS            ( 1U )
#define SX126X_REG_TX_CLAMP_CFG_MASK           ( 0x0FUL << SX126X_REG_TX_CLAMP_CFG_POS )

/**
 * @brief RTC control
 */
#define SX126X_REG_RTC_CTRL                    0x0902

/**
 * @brief Event clear
 */
#define SX126X_REG_EVT_CLR                     0x0944
#define SX126X_REG_EVT_CLR_TIMEOUT_POS         ( 1U )
#define SX126X_REG_EVT_CLR_TIMEOUT_MASK        ( 0x01UL << SX126X_REG_EVT_CLR_TIMEOUT_POS )

/**
 * @brief RX address pointer
 */
#define SX126X_REG_RX_ADDRESS_POINTER          0x0803

/**
 * @brief RX/TX payload length
 */
#define SX126X_REG_RXTX_PAYLOAD_LEN            0x06BB

/**
 * @brief Output disable
 */
#define SX126X_REG_OUT_DIS_REG                 0x0580
#define SX126X_REG_OUT_DIS_REG_DIO3_POS        ( 3U )
#define SX126X_REG_OUT_DIS_REG_DIO3_MASK       ( 0x01UL << SX126X_REG_OUT_DIS_REG_DIO3_POS )

/**
 * @brief Input enable
 */
#define SX126X_REG_IN_EN_REG                   0x0583
#define SX126X_REG_IN_EN_REG_DIO3_POS          ( 3U )
#define SX126X_REG_IN_EN_REG_DIO3_MASK         ( 0x01UL << SX126X_REG_IN_EN_REG_DIO3_POS )

/**
 * @brief TX bitbang A
 */
#define SX126X_REG_BITBANG_A_REG               0x0680
#define SX126X_REG_BITBANG_A_REG_ENABLE_POS    ( 4U )
#define SX126X_REG_BITBANG_A_REG_ENABLE_MASK   ( 0x07UL << SX126X_REG_BITBANG_A_REG_ENABLE_POS )
#define SX126X_REG_BITBANG_A_REG_ENABLE_VAL    ( 0x01UL << SX126X_REG_BITBANG_A_REG_ENABLE_POS )

/**
 * @brief TX bitbang B
 */
#define SX126X_REG_BITBANG_B_REG               0x0587
#define SX126X_REG_BITBANG_B_REG_ENABLE_POS    ( 0U )
#define SX126X_REG_BITBANG_B_REG_ENABLE_MASK   ( 0x0FUL << SX126X_REG_BITBANG_B_REG_ENABLE_POS )
#define SX126X_REG_BITBANG_B_REG_ENABLE_VAL    ( 0x0CUL << SX126X_REG_BITBANG_B_REG_ENABLE_POS )

/**
 * @brief Number of symbols given as SX126X_REG_LR_SYNCH_TIMEOUT[7:3] * 2 ^ (2*SX126X_REG_LR_SYNCH_TIMEOUT[2:0] + 1)
 */
#define SX126X_REG_LR_SYNCH_TIMEOUT            0x0706

/**
 * @brief Base address of the register retention list
 */
#define SX126X_REG_RETENTION_LIST_BASE_ADDRESS 0x029F

#endif    // _LORA_126X_REGS_H
