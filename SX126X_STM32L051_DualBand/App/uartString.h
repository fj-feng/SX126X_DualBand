#ifndef _UART_STRING_H__
#define _UART_STRING_H__
//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"

//----------------------------------------------------------------
//                     Public Macros
//----------------------------------------------------------------
#define CHARDATA_MAX_LEN 1024    // 字符串缓存最大长度

//----------------------------------------------------------------
//                    Public Typedefs
//----------------------------------------------------------------
/**
 * @brief  字符串缓存结构体
 */
typedef struct
{
    uint16_t length;
    char     data[ CHARDATA_MAX_LEN ];
} S_CharData;

/**
 * @brief  指令数据内容结构体
 */
typedef struct
{
    uint16_t length;
    uint32_t data[ 512 ];
} S_CommandData;

/**
 * @brief  指令类型，根据实际工程调整
 */
typedef enum
{
    C_None                 = ( 0 << 0 ),
    C_setFreq              = ( 1 << 1 ),
    C_readFreq             = ( 1 << 2 ),
    C_setBw                = ( 1 << 3 ),
    C_readBw               = ( 1 << 4 ),
    C_setSf                = ( 1 << 5 ),
    C_readSf               = ( 1 << 6 ),
    C_setCr                = ( 1 << 7 ),
    C_readCr               = ( 1 << 8 ),
    C_setPower             = ( 1 << 9 ),
    C_readPower            = ( 1 << 10 ),
    C_setPreamble          = ( 1 << 11 ),
    C_readPreamble         = ( 1 << 12 ),
    C_setCadTime           = ( 1 << 13 ),
    C_readCadTime          = ( 1 << 14 ),
    C_enterP2pTx           = ( 1 << 15 ),
    C_enterP2pRx           = ( 1 << 16 ),
    C_sendWave             = ( 1 << 17 ),
    C_sendInfinitePreamble = ( 1 << 18 ),
    C_enterCad             = ( 1 << 19 ),
    C_sendWaveWithMcuSleep = ( 1 << 20 ),
    C_enterRxWithMcuSleep  = ( 1 << 21 ),
    C_LoRaEnterSleep       = ( 1 << 22 ),
    C_mcuSleep             = ( 1 << 23 ),
    C_sendSingleData       = ( 1 << 24 ),
    C_enterRx              = ( 1 << 25 ),
    C_enterStandby         = ( 1 << 26 ),
    C_setP2pTxParams       = ( 1 << 27 ),
    C_setBaseParams        = ( 1 << 28 ),
    C_setPER               = ( 1 << 29 ),
    C_setLowDrOpt          = ( 1 << 30 ),
    C_Cap                  = ( 1 << 31 ),
} T_CommandList;

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------

//----------------------------------------------------------------
//              Public Function Declarations
//----------------------------------------------------------------
T_CommandList DecodeCommand( uint32_t* value, uint16_t* len );
void          CopyUartData( uint8_t* data, uint16_t len );
bool          DecodeChar( uint32_t* value, uint16_t* len, char* cmd, bool hexData );

#endif
