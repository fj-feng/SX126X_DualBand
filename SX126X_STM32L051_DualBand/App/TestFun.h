#ifndef _TEST_FUN_H__
#define _TEST_FUN_H__
//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include <stdint.h>
#include "LoRa126x_Fun.h"
#include "uartString.h"

//----------------------------------------------------------------
//                     Public Macros
//----------------------------------------------------------------
#define CMD_NEW_FLAG           ( 1 << 0 )
#define CMD_SET_FREQ           ( 1 << 1 )
#define CMD_READ_FREQ          ( 1 << 2 )
#define CMD_SET_BW             ( 1 << 3 )
#define CMD_READ_BW            ( 1 << 4 )
#define CMD_SET_SF             ( 1 << 5 )
#define CMD_READ_SF            ( 1 << 6 )
#define CMD_SET_CR             ( 1 << 7 )
#define CMD_READ_CR            ( 1 << 8 )
#define CMD_SET_POWER          ( 1 << 9 )
#define CMD_READ_POWER         ( 1 << 10 )
#define CMD_SET_PREAMBLE       ( 1 << 11 )
#define CMD_READ_PREAMBLE      ( 1 << 12 )
#define CMD_SET_CAD_TIME       ( 1 << 13 )
#define CMD_READ_CAD_TIME      ( 1 << 14 )
#define CMD_ENTER_P2P_TX       ( 1 << 15 )
#define CMD_ENTER_P2P_RX       ( 1 << 16 )
#define CMD_SEND_WAVE          ( 1 << 17 )
#define CMD_SNED_PREAMBLE      ( 1 << 18 )
#define CMD_ENTER_CAD          ( 1 << 19 )
#define CMD_SEND_WAVE_SLEEP    ( 1 << 20 )
#define CMD_ENTER_RX_SLEEP     ( 1 << 21 )
#define CMD_LORA_SLEEP         ( 1 << 22 )
#define CMD_MCU_SLEEP          ( 1 << 23 )
#define CMD_SEND_SINGLE_PACKET ( 1 << 24 )
#define CMD_ENTER_RX           ( 1 << 25 )
#define CMD_ENTER_SYANDBY      ( 1 << 26 )
#define CMD_SET_P2P_TX_PARAMS  ( 1 << 27 )
#define CMD_SET_BASE_PARAMS    ( 1 << 28 )
#define CMD_SET_PER            ( 1 << 29 )
#define CMD_SET_LOWDROPT       ( 1 << 30 )
#define CMD_CAP                ( 1 << 31 )

//----------------------------------------------------------------
//                    Public Typedefs
//----------------------------------------------------------------
typedef union
{
    struct
    {
        uint32_t newFlag              : 1;
        uint32_t setFreq              : 1;
        uint32_t readFreq             : 1;
        uint32_t SetBw                : 1;
        uint32_t readBw               : 1;
        uint32_t setSf                : 1;
        uint32_t readSf               : 1;
        uint32_t setCr                : 1;
        uint32_t readCr               : 1;
        uint32_t setPower             : 1;
        uint32_t readPower            : 1;
        uint32_t setPreamble          : 1;
        uint32_t readPreamble         : 1;
        uint32_t setCadTime           : 1;
        uint32_t readCadTime          : 1;
        uint32_t enterP2pTx           : 1;
        uint32_t enterP2pRx           : 1;
        uint32_t sendWave             : 1;
        uint32_t sendInfinitePreamble : 1;
        uint32_t enterCad             : 1;
        uint32_t sendWaveWithMcuSleep : 1;
        uint32_t enterRxWithMcuSleep  : 1;
        uint32_t LoRaEnterSleep       : 1;
        uint32_t mcuSleep             : 1;
        uint32_t sendSingleData       : 1;
        uint32_t enterRx              : 1;
        uint32_t enterStandby         : 1;
        uint32_t setP2pTxParams       : 1;
        uint32_t setBaseParams        : 1;
        uint32_t setPER               : 1;
        uint32_t setLowDrOpt          : 1;
        uint32_t cap                  : 1;
    } flag;
    uint32_t state;
} TestFun_s;

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------

//----------------------------------------------------------------
//              Public Function Declarations
//----------------------------------------------------------------
void                   UserUartProcess( void );
void                   SetCmdFlag( T_CommandList cmd );
bool                   GetCmdFlag( uint32_t cmd );
SX126xLoRaBandwidths_t CmdChangeBW( uint32_t cmdData );
float                  GetBw( SX126xLoRaBandwidths_t bw );
bool                   checkBw( uint16_t bw );
void                   RfP2pTxProcess( void );
void                   RfP2pRxProcess( void );
void                   RfCadOnlyProcess( void );
void                   RfCadRxProcess( void );
void                   RfTest( void );
void                   RfTestCmdProcess( void );
void                   RfConfigCmdProcess( void );
void                   RfCmdProcess( void );
void                   McuSleep( bool isloraSleep );

#endif    // _TEST_FUN_H__
