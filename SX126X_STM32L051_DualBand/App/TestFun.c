//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include "TestFun.h"
#include "uart.h"
#include "log.h"
#include "gpio.h"
#include "timer.h"

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------
TestFun_s     g_testFun          = { 0 };
uint32_t      g_TestTimeCount    = 0;
const uint8_t s_p2pTxData[ 256 ] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
    0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
    0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B,
    0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
    0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71,
    0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84,
    0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA,
    0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD,
    0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0,
    0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3,
    0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6,
    0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

LoRaRxPkt_s g_tempbuf = { 0 };

//----------------------------------------------------------------
//                     Private Macros
//----------------------------------------------------------------

//----------------------------------------------------------------
//                    Private Typedefs
//----------------------------------------------------------------
typedef union
{
    struct
    {
        uint16_t testSendSinglePacket : 1;
        uint16_t testMcuSleep         : 1;
        uint16_t testLoRaSleep        : 1;
        uint16_t testRxSleep          : 1;
        uint16_t testSendWaveSleep    : 1;
        uint16_t testCad              : 1;
        uint16_t testSendPreamble     : 1;
        uint16_t testSendWave         : 1;
        uint16_t testP2pRx            : 1;
        uint16_t testP2pTx            : 1;
        uint16_t testRx               : 1;
        uint16_t testStandby          : 1;
        uint16_t testSenP2pTxParams   : 1;
        uint16_t testPER              : 1;
        uint16_t                      : 2;
    } testFlag;
    uint16_t testState;
} TestState_s;

//----------------------------------------------------------------
//                   Private Variables
//----------------------------------------------------------------
static S_CommandData s_userCmdData       = { 0 };
static uint16_t      s_cadTime           = 3000;
static TestState_s   s_TestState         = { 0 };
static uint16_t      s_p2pTxSendNum      = 0;
static uint16_t      s_p2pTxRecNum       = 0;
static uint16_t      s_p2pRxSendNum      = 0;
static uint16_t      s_p2pRxRecNum       = 0;
static uint8_t       s_p2pTxDataLen      = 20;
static uint16_t      s_p2pTxSendTotalNum = 100;
static uint32_t      s_p2pTxTime         = 1000;
static uint16_t      s_perRxRecNum       = 0;
static bool          s_firstCad          = false;
static bool          s_forceRx           = false;

//----------------------------------------------------------------
//              Private Function Declarations
//----------------------------------------------------------------

//----------------------------------------------------------------
//                   Function Definitions
//----------------------------------------------------------------
/**
 * @brief  用户串口数据处理
 */
void UserUartProcess( void )
{
    T_CommandList cmd;

    if( g_uart2State.recFlag == false )
        return;

    g_uart2State.recFlag = false;
    CopyUartData( g_uart2State.rxBuffer, g_uart2State.rxLen );           // 搬运数据
    cmd = DecodeCommand( s_userCmdData.data, &s_userCmdData.length );    // 指令解析

    if( cmd != C_None )
        SetCmdFlag( cmd );    // 设置指令功能标志位
    else
        PrintDebug( "Error CMD\n" );
}

/**
 * @brief  设置指令功能标志位
 * @param  cmd
 */
void SetCmdFlag( T_CommandList cmd )
{
    g_testFun.state |= (uint32_t) cmd;
    g_testFun.flag.newFlag = 1;
}

/**
 * @brief  获取对于指令标志,获取后会自动清除标志
 * @param  cmd 指令功能
 * @return true 标志置位
 * @return false 标志未置位
 */
bool GetCmdFlag( uint32_t cmd )
{
    if( ( g_testFun.state & cmd ) == cmd )
    {
        g_testFun.state &= ~cmd;    // 清除标志
        return true;
    }
    else
        return false;
}

/**
 * @brief  RF测试功能
 */
void RfTest( void )
{
    uint8_t data[ 256 ];
    uint8_t i;

    if( s_TestState.testState == 0 )    // 无任何标志直接返回
        return;

    /*单包数据发送测试*/
    if( s_TestState.testFlag.testSendSinglePacket == 1 )
    {
        s_TestState.testFlag.testSendSinglePacket = 0;

        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        for( i = 0; i < s_userCmdData.length; i++ )
        {
            data[ i ] = s_userCmdData.data[ i ];
            if( i == 255 )
                break;
        }

        SX126xSendData( data, i );
        PrintDebug( "Send data: " );
        PrintDebugHex( data, i );
        PrintDebug( "\n" );
    }
    /*单载波测试*/
    else if( s_TestState.testFlag.testSendWave == 1 )
    {
        s_TestState.testFlag.testSendWave = 0;

        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        SX126xSendCarrierWave();
        PrintDebug( "Send wave!\n" );
    }
    /*单载波+MCU休眠测试*/
    else if( s_TestState.testFlag.testSendWaveSleep == 1 )
    {
        s_TestState.testFlag.testSendWaveSleep = 0;

        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        SX126xSendCarrierWave();
        PrintDebug( "LoRa send wave,MCU sleep.\n" );
        HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );
    }
    /*LoRa发送无限前导码测试*/
    else if( s_TestState.testFlag.testSendPreamble == 1 )
    {
        s_TestState.testFlag.testSendPreamble = 0;

        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        SX126xSendLoRaInfinitePreamble();
        PrintDebug( "Send infinite LoRa preamble!\n" );
    }
    /*进入接收*/
    else if( s_TestState.testFlag.testRx == 1 )
    {
        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        if( s_forceRx )
        {
            s_forceRx = false;
            SX126xEnterRx( 0 );
            PrintDebug( "LoRa enter RX.\n" );
        }

        if( g_LoRaRxPkt.recFlag == true )
        {
            g_tempbuf.rssi = g_LoRaRxPkt.rssi;
            g_tempbuf.snr  = g_LoRaRxPkt.snr;
            g_tempbuf.size = g_LoRaRxPkt.size;
            memcpy( g_tempbuf.payload, g_LoRaRxPkt.payload, g_tempbuf.size );
            g_LoRaRxPkt.recFlag = false;
            SX126xEnterRx( 0 );
            PrintDebug( "LoRa receive packet: " );
            PrintDebugHex( g_tempbuf.payload, g_tempbuf.size );
            PrintDebug( "\n" );
            PrintDebug( "RSSI: %d , SNR: %d\n", g_tempbuf.rssi, g_tempbuf.snr );
        }
        else if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR ) == true )
        {
            PrintDebug( "CRC error\n" );
            SX126xEnterRx( 0 );
        }

        if( g_rfState != RF_RX_ING && g_LoRaRxPkt.recFlag == false )
        {
            SX126xEnterRx( 0 );
            PrintDebug( "LoRa enter RX.\n" );
        }
    }
    /*接收+MCU休眠*/
    else if( s_TestState.testFlag.testRxSleep == 1 )
    {
        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        SX126xEnterRx( 0 );
        PrintDebug( "LoRa enter Rx,MCU sleep.\n" );
        HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );
    }
    /*进入standby*/
    else if( s_TestState.testFlag.testStandby == 1 )
    {
        s_TestState.testFlag.testStandby = 0;

        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        SX126xEnterStandby();

        PrintDebug( "LoRa enter standby.\n" );
    }
    /*lora 休眠*/
    else if( s_TestState.testFlag.testLoRaSleep == 1 )
    {
        s_TestState.testFlag.testLoRaSleep = 0;

        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        SX126xEnterSleep();
        PrintDebug( "LoRa enter sleep.\n" );
    }
    /*MCU休眠*/
    else if( s_TestState.testFlag.testMcuSleep == 1 )
    {
        s_TestState.testFlag.testMcuSleep = 0;
        PrintDebug( "MCU sleep.\n" );
        if( g_rfState == RF_SLEEP )
            McuSleep( true );
        else
            McuSleep( false );
    }
    /*P2P通信测试，TX端*/
    else if( s_TestState.testFlag.testP2pTx == 1 )
    {
        RfP2pTxProcess();
    }
    /*P2P通信测试，RX端*/
    else if( s_TestState.testFlag.testP2pRx == 1 )
    {
        RfP2pRxProcess();
    }
    /*CAD测试*/
    else if( s_TestState.testFlag.testCad == 1 )
    {
        RfCadOnlyProcess();
        // RfCadRxProcess();
    }
    else if( s_TestState.testFlag.testPER == 1 )
    {
        if( s_perRxRecNum == 0xFFFF )
        {
            PrintDebug( "Start PER test,rx count reset.\n" );
            s_perRxRecNum = 0;
        }
        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        if( g_LoRaRxPkt.recFlag == true )
        {
            g_LoRaRxPkt.recFlag = false;
            SX126xEnterRx( 0 );
            s_perRxRecNum++;
            PrintDebug( "Receive OK num: %d ,", s_perRxRecNum );
            PrintDebug( "RSSI: %d, SNR: %d\n", g_LoRaRxPkt.rssi, g_LoRaRxPkt.snr );
        }
        else if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR ) == true )
        {
            SX126xClearIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR );
            PrintDebug( "CRC error\n" );
        }

        if( g_rfState != RF_RX_ING && g_LoRaRxPkt.recFlag == false )
        {
            SX126xEnterRx( 0 );
        }
    }
}

/**
 * @brief  RF 点对点通信TX端处理进程
 */
void RfP2pTxProcess( void )
{
    if( s_p2pTxSendNum == 0xFFFF && s_p2pTxRecNum == 0xFFFF )
    {
        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        PrintDebug( "P2P TX mode.Rx and rx count reset.\n" );
        PrintDebug(
            "Notice: one packet with %d bytes of data will be sent every %d ms and stop sending when send %d packets.\n",
            s_p2pTxDataLen,
            s_p2pTxTime,
            s_p2pTxSendTotalNum );
        PrintDebug(
            "If you want change those values,you can use command ' set p2p tx params 'sendTime' 'datalen' 'total packets' '.\n" );
        s_p2pTxSendNum = 0;
        s_p2pTxRecNum  = 0;
        SX126xEnterStandby();
        g_TestTimeCount = s_p2pTxTime;
    }

    if( g_rfState != RF_TX_ING && s_p2pTxSendNum < s_p2pTxSendTotalNum )
    {
        if( g_TestTimeCount >= s_p2pTxTime )
        {
            g_TestTimeCount = 0;
            SX126xSendData( (uint8_t*) s_p2pTxData, s_p2pTxDataLen );
        }
    }
    if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_TX_DONE ) == true )
    {
        SX126xClearIrqFlag( SX126X_IRQ_FLAG_TX_DONE );
        SX126xEnterRx( 0 );
        s_p2pTxSendNum++;
        PrintDebug( "\n [P2P TX]Send num: %d  ", s_p2pTxSendNum );
    }
    if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_RX_DONE ) == true )
    {
        SX126xClearIrqFlag( SX126X_IRQ_FLAG_RX_DONE );
        if( g_LoRaRxPkt.recFlag == true )
        {
            g_LoRaRxPkt.recFlag = false;
            if( g_LoRaRxPkt.size == s_p2pTxDataLen )
            {
                if( memcmp( g_LoRaRxPkt.payload, s_p2pTxData, g_LoRaRxPkt.size ) == 0 )
                {
                    s_p2pTxRecNum++;
                    PrintDebug( "receive num: %d , RSSI:%d , SNR: %d",
                                s_p2pTxRecNum,
                                g_LoRaRxPkt.rssi,
                                g_LoRaRxPkt.snr );
                }
                else
                {
                    PrintDebug( "CRC is right but data compareing to sending is wrong." );
                }
            }
            else
            {
                PrintDebug( "CRC is right but data compareing to sending is wrong." );
            }
        }
        else if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR ) == true )
        {
            SX126xClearIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR );
            PrintDebug( "CRC error." );
        }
    }
}

/**
 * @brief  RF点对点通信RX端处理进程
 */
void RfP2pRxProcess( void )
{
    if( s_p2pRxSendNum == 0xFFFF && s_p2pRxRecNum == 0xFFFF )
    {
        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        PrintDebug( "P2P RX mode.Rx and rx count reset\n" );
        s_p2pRxSendNum = 0;
        s_p2pRxRecNum  = 0;
        SX126xEnterRx( 0 );
    }
    if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_RX_DONE ) == true )
    {
        SX126xClearIrqFlag( SX126X_IRQ_FLAG_RX_DONE );
        if( g_LoRaRxPkt.recFlag == true )
        {
            g_LoRaRxPkt.recFlag = false;
            s_p2pRxRecNum++;
            PrintDebug( "\n [P2P RX]Receive num: %d , RSSI: %d, SNR: %d  ",
                        s_p2pRxRecNum,
                        g_LoRaRxPkt.rssi,
                        g_LoRaRxPkt.snr );
            SX126xSendData( g_LoRaRxPkt.payload, g_LoRaRxPkt.size );
        }
        else if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR ) == true )
        {
            SX126xClearIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR );
            PrintDebug( "\n [P2P RX]CRC error." );
            SX126xEnterRx( 0 );
        }
    }
    if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_TX_DONE ) == true )
    {
        SX126xClearIrqFlag( SX126X_IRQ_FLAG_TX_DONE );
        s_p2pRxSendNum++;
        PrintDebug( "Send num: %d  ", s_p2pRxSendNum );
        SX126xEnterRx( 0 );
    }
}

/**
 * @brief  RF CAD_only功能
 */
void RfCadOnlyProcess( void )
{
    if( s_firstCad == true )
    {
        s_firstCad      = false;
        g_TestTimeCount = 0;

        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        PrintDebug( "Start cad.\n" );
        SX126xStartCadOnly();
    }
    else if( g_TestTimeCount >= s_cadTime )
    {
        g_TestTimeCount = 0;
        SX126xExitSleep();
        PrintDebug( "LoRa wake up,start cad.\n" );
        SX126xStartCadOnly();
    }
    if( g_rfState == RF_RX_ING )
        g_TestTimeCount = 0;
    if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_CAD_DONE ) == true )
    {
        SX126xClearIrqFlag( SX126X_IRQ_FLAG_CAD_DONE );
        if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_CAD_DETECTED ) == true )
        {
            SX126xClearIrqFlag( SX126X_IRQ_FLAG_CAD_DETECTED );
            g_TestTimeCount = 0;
            SX126xEnterRx( s_cadTime );
            PrintDebug( "CAD detected,enter RX\n" );
        }
        else
        {
            PrintDebug( "CAD didn't detect signal,enter sleep.\n" );
            SX126xEnterSleep();
        }
    }
    if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_RX_DONE ) == true )
    {
        SX126xClearIrqFlag( SX126X_IRQ_FLAG_RX_DONE );
        if( g_LoRaRxPkt.recFlag == true )
        {
            g_LoRaRxPkt.recFlag = false;
            PrintDebug( "LoRa receive packet: " );
            PrintDebugHex( g_LoRaRxPkt.payload, g_LoRaRxPkt.size );
            PrintDebug( "\n" );
        }
        else if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR ) == true )
        {
            SX126xClearIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR );
            PrintDebug( "Receive CRC error.\n" );
        }
        PrintDebug( "Enter Sleep.\n" );
        g_TestTimeCount = 0;
        SX126xEnterSleep();
    }
    else if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_RX_TIMEOUT ) == true )
    {
        SX126xClearIrqFlag( SX126X_IRQ_FLAG_RX_TIMEOUT );
        PrintDebug( "Rx timeout,enter sleep.\n" );
        g_TestTimeCount = 0;
        SX126xEnterSleep();
    }
}

/**
 * @brief  RF CAD_rx功能
 */
void RfCadRxProcess( void )
{
    if( s_firstCad == true )
    {
        s_firstCad      = false;
        g_TestTimeCount = 0;

        if( g_rfState == RF_SLEEP )
            SX126xExitSleep();

        PrintDebug( "Start cad.\n" );
        SX126xStartCadRx( s_cadTime + 200 );
    }
    else if( g_TestTimeCount >= s_cadTime )
    {
        g_TestTimeCount = 0;
        SX126xExitSleep();
        PrintDebug( "LoRa wake up,start cad.\n" );
        SX126xStartCadRx( s_cadTime + 200 );
    }
    if( g_rfState == RF_RX_ING )
        g_TestTimeCount = 0;
    if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_CAD_DONE ) == true )
    {
        SX126xClearIrqFlag( SX126X_IRQ_FLAG_CAD_DONE );
        if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_CAD_DETECTED ) != true )
        {
            PrintDebug( "CAD didn't detect signal,enter sleep.\n" );
            SX126xEnterSleep();
        }
        else
        {
            g_TestTimeCount = 0;
            SX126xClearIrqFlag( SX126X_IRQ_FLAG_CAD_DETECTED );
            PrintDebug( "CAD detected.\n" );
        }
    }
    if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_RX_DONE ) == true )
    {
        SX126xClearIrqFlag( SX126X_IRQ_FLAG_RX_DONE );
        if( g_LoRaRxPkt.recFlag == true )
        {
            g_LoRaRxPkt.recFlag = false;
            PrintDebug( "LoRa receive packet: " );
            PrintDebugHex( g_LoRaRxPkt.payload, g_LoRaRxPkt.size );
            PrintDebug( "\n" );
        }
        else if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR ) == true )
        {
            SX126xClearIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR );
            PrintDebug( "Receive CRC error.\n" );
        }
        PrintDebug( "Enter Sleep.\n" );
        SX126xEnterSleep();
        g_TestTimeCount = 0;
    }
    else if( SX126xGetIrqFlag( SX126X_IRQ_FLAG_RX_TIMEOUT ) == true )
    {
        SX126xClearIrqFlag( SX126X_IRQ_FLAG_RX_TIMEOUT );
        PrintDebug( "Rx timeout,enter sleep.\n" );
        SX126xEnterSleep();
        g_TestTimeCount = 0;
    }
}

/**
 * @brief  RF指令处理
 */
void RfCmdProcess( void )
{
    if( GetCmdFlag( CMD_NEW_FLAG ) == 0 )
        return;

    RfTestCmdProcess();
    RfConfigCmdProcess();
}

/**
 * @brief  RF测试指令处理进程
 */
void RfTestCmdProcess( void )
{
    if( s_userCmdData.length != 0 )
    {
        if( GetCmdFlag( CMD_SEND_SINGLE_PACKET ) == true )
        {
            s_TestState.testState                     = 0x00;
            s_TestState.testFlag.testSendSinglePacket = 1;
        }
        else
            return;
    }

    if( GetCmdFlag( CMD_ENTER_P2P_TX ) == true )
    {
        s_TestState.testState          = 0x00;
        s_TestState.testFlag.testP2pTx = 1;
        s_p2pTxSendNum                 = 0xFFFF;
        s_p2pTxRecNum                  = 0XFFFF;
    }
    else if( GetCmdFlag( CMD_ENTER_P2P_RX ) == true )
    {
        s_TestState.testState          = 0x00;
        s_TestState.testFlag.testP2pRx = 1;
        s_p2pRxSendNum                 = 0xFFFF;
        s_p2pRxRecNum                  = 0XFFFF;
    }
    else if( GetCmdFlag( CMD_SEND_WAVE ) == true )
    {
        s_TestState.testState             = 0x00;
        s_TestState.testFlag.testSendWave = 1;
    }
    else if( GetCmdFlag( CMD_SNED_PREAMBLE ) == true )
    {
        s_TestState.testState                 = 0x00;
        s_TestState.testFlag.testSendPreamble = 1;
    }
    else if( GetCmdFlag( CMD_ENTER_CAD ) == true )
    {
        s_TestState.testState        = 0x00;
        s_TestState.testFlag.testCad = 1;
        s_firstCad                   = true;
    }
    else if( GetCmdFlag( CMD_SEND_WAVE_SLEEP ) == true )
    {
        s_TestState.testState                  = 0x00;
        s_TestState.testFlag.testSendWaveSleep = 1;
    }
    else if( GetCmdFlag( CMD_ENTER_RX_SLEEP ) == true )
    {
        s_TestState.testState            = 0x00;
        s_TestState.testFlag.testRxSleep = 1;
    }
    else if( GetCmdFlag( CMD_LORA_SLEEP ) == true )
    {
        s_TestState.testState              = 0x00;
        s_TestState.testFlag.testLoRaSleep = 1;
    }
    else if( GetCmdFlag( CMD_MCU_SLEEP ) == true )
    {
        s_TestState.testState             = 0x00;
        s_TestState.testFlag.testMcuSleep = 1;
    }
    else if( GetCmdFlag( CMD_ENTER_RX ) == true )
    {
        s_TestState.testState       = 0x00;
        s_TestState.testFlag.testRx = 1;
        s_forceRx                   = true;
    }
    else if( GetCmdFlag( CMD_ENTER_SYANDBY ) == true )
    {
        s_TestState.testState            = 0x00;
        s_TestState.testFlag.testStandby = 1;
    }
    else if( GetCmdFlag( CMD_SET_PER ) == true )
    {
        s_TestState.testState        = 0x00;
        s_TestState.testFlag.testPER = 1;
        s_perRxRecNum                = 0xFFFF;
    }
}

/**
 * @brief  RF配置/查询指令处理进程
 */
void RfConfigCmdProcess( void )
{
    if( s_userCmdData.length > 1 )
    {
        if( GetCmdFlag( CMD_SET_P2P_TX_PARAMS ) == true )
        {
            if( s_userCmdData.length != 3 )
            {
                PrintDebug( "Miss params,there should be 3 params for P2P TX.\n" );
                return;
            }

            if( s_userCmdData.data[ 1 ] > 0 && s_userCmdData.data[ 1 ] < 256 )
            {
                s_p2pTxTime         = s_userCmdData.data[ 0 ];
                s_p2pTxDataLen      = s_userCmdData.data[ 1 ];
                s_p2pTxSendTotalNum = s_userCmdData.data[ 2 ];
                PrintDebug( "OK\n" );
                PrintDebug( "Now send time is %d ms,packet size is %d bytes, total packet is %d\n",
                            s_p2pTxTime,
                            s_p2pTxDataLen,
                            s_p2pTxSendTotalNum );
            }
            else
            {
                PrintDebug( "Send data length shoule be 1~255.\n" );
            }
        }
        else if( GetCmdFlag( CMD_SET_BASE_PARAMS ) == true )
        {
            if( s_userCmdData.length != 5 )
            {
                PrintDebug( "Miss params,there should be 5 params.\n" );
                return;
            }

            if( checkBw( s_userCmdData.data[ 3 ] ) != true )
            {
                PrintDebug( "Set failed,invalid params.\n" );
                PrintDebug( "Note: param should be integer.for example,BW 62.5K should send 'bw 62'\n" );
                return;
            }
            if( s_userCmdData.data[ 2 ] > 12 || s_userCmdData.data[ 2 ] < 5 )
            {
                PrintDebug( "Set failed,invalid params.\n" );
                PrintDebug( "SF should be 5~12.\n" );
                return;
            }
            if( s_userCmdData.data[ 1 ] > 22 )
            {
                PrintDebug( "Set failed,invalid params.\n" );
                PrintDebug( "Power should be 0~22.\n" );
                return;
            }
            if( s_userCmdData.data[ 4 ] > 4 || s_userCmdData.data[ 4 ] == 0 )
            {
                PrintDebug( "Set failed,invalid params.\n" );
                PrintDebug( "CR should be 1~4.\n" );
                return;
            }
            if( SX126xSetFreq( s_userCmdData.data[ 0 ] ) != true )
            {
                PrintDebug( "Set failed,invalid frequency\n" );
                return;
            }
            SX126xLoRaModulationConfig( s_userCmdData.data[ 2 ],
                                        CmdChangeBW( s_userCmdData.data[ 3 ] ),
                                        s_userCmdData.data[ 4 ] );
            SX126xSetPower( s_userCmdData.data[ 1 ] );
            PrintDebug( "OK\n" );
            PrintDebug( "Now frequency is %d Hz\n", SX126xGetFreq() );
            PrintDebug( "Now power is %d dBm\n", SX126xGetPower() );
            PrintDebug( "Now SF is %d\n", SX126xGetSf() );
            PrintDebug( "BW is %.2f KHz\n", GetBw( SX126xGetBw() ) );
            PrintDebug( "CR is 4/%d\n", SX126xGetCr() + 4 );
        }
        return;
    }

    if( GetCmdFlag( CMD_SET_FREQ ) == true )
    {
        if( SX126xSetFreq( s_userCmdData.data[ 0 ] ) == true )
        {
            PrintDebug( "OK\n" );
            PrintDebug( "Now frequency is %d Hz\n", SX126xGetFreq() );
        }
        else
            PrintDebug( "Set failed,invalid frequency\n" );
    }
    else if( GetCmdFlag( CMD_READ_FREQ ) == true )
    {
        PrintDebug( "Frequency is %d Hz", SX126xGetFreq() );
    }
    else if( GetCmdFlag( CMD_SET_BW ) == true )
    {
        if( checkBw( s_userCmdData.data[ 0 ] ) == true )
        {
            SX126xLoRaModulationConfig( SX126xGetSf(), CmdChangeBW( s_userCmdData.data[ 0 ] ), SX126xGetCr() );

            // #if defined( SX126X_USE_XTAL )
            //             if( ( SX126xGetSf() == SX126X_LORA_SF12 && SX126xGetBw() == SX126X_LORA_BW_500 ) ||
            //                 ( SX126xGetSf() == SX126X_LORA_SF11 && SX126xGetBw() == SX126X_LORA_BW_250 ) ||
            //                 ( SX126xGetSf() == SX126X_LORA_SF10 && SX126xGetBw() == SX126X_LORA_BW_125 ) ||
            //                 ( SX126xGetSf() == SX126X_LORA_SF9 && SX126xGetBw() == SX126X_LORA_BW_062 ) )
            //                 SX126xLowDataRateOptConfig( true );
            // #elif defined( SX126X_USE_TCXO )
            //             if( ( SX126xGetSf() == SX126X_LORA_SF11 && SX126xGetBw() == SX126X_LORA_BW_250 ) )
            //                 SX126xLowDataRateOptConfig( true );
            // #endif

            PrintDebug( "OK\n" );
            PrintDebug( "Now BW is %.2f KHz\n", GetBw( SX126xGetBw() ) );
        }
        else
        {
            PrintDebug( "Set failed,invalid params.\n" );
            PrintDebug( "Note: param should be integer.for example,BW 62.5K should send 'bw 62'\n" );
        }
    }
    else if( GetCmdFlag( CMD_READ_BW ) == true )
    {
        PrintDebug( "BW is %.2f KHz\n", GetBw( SX126xGetBw() ) );
    }
    else if( GetCmdFlag( CMD_SET_SF ) == true )
    {
        if( s_userCmdData.data[ 0 ] <= 12 && s_userCmdData.data[ 0 ] >= 5 )
        {
            SX126xLoRaModulationConfig( s_userCmdData.data[ 0 ], SX126xGetBw(), SX126xGetCr() );

            // #if defined( SX126X_USE_XTAL )
            //             if( ( SX126xGetSf() == SX126X_LORA_SF12 && SX126xGetBw() == SX126X_LORA_BW_500 ) ||
            //                 ( SX126xGetSf() == SX126X_LORA_SF11 && SX126xGetBw() == SX126X_LORA_BW_250 ) ||
            //                 ( SX126xGetSf() == SX126X_LORA_SF10 && SX126xGetBw() == SX126X_LORA_BW_125 ) ||
            //                 ( SX126xGetSf() == SX126X_LORA_SF9 && SX126xGetBw() == SX126X_LORA_BW_062 ) )
            //                 SX126xLowDataRateOptConfig( true );
            // #elif defined( SX126X_USE_TCXO )
            //             if( ( SX126xGetSf() == SX126X_LORA_SF11 && SX126xGetBw() == SX126X_LORA_BW_250 ) )
            //                 SX126xLowDataRateOptConfig( true );
            // #endif

            PrintDebug( "OK\n" );
            PrintDebug( "Now SF is %d\n", SX126xGetSf() );
        }
        else
        {
            PrintDebug( "SF should be 5~12.\n" );
        }
    }
    else if( GetCmdFlag( CMD_READ_SF ) == true )
    {
        PrintDebug( "SF is %d\n", SX126xGetSf() );
    }
    else if( GetCmdFlag( CMD_SET_CR ) == true )
    {
        if( s_userCmdData.data[ 0 ] <= 4 && s_userCmdData.data[ 0 ] >= 1 )
        {
            SX126xLoRaModulationConfig( SX126xGetSf(), SX126xGetBw(), s_userCmdData.data[ 0 ] );
            PrintDebug( "OK\n" );
            PrintDebug( "Now SF is 4/%d\n", SX126xGetCr() + 4 );
        }
        else
        {
            PrintDebug( "CR should be 1~4.\n" );
            PrintDebug( "Notice: CR '4/5' is 1,'4/6' is 2,'4/7' is 3,'4/8' is 4\n" );
        }
    }
    else if( GetCmdFlag( CMD_READ_CR ) == true )
    {
        PrintDebug( "CR is 4/%d\n", SX126xGetCr() + 4 );
    }
    else if( GetCmdFlag( CMD_SET_POWER ) == true )
    {
        if( s_userCmdData.data[ 0 ] <= 22 )
        {
            SX126xSetPower( s_userCmdData.data[ 0 ] );
            PrintDebug( "OK\n" );
            PrintDebug( "Now power is %d dBm\n", SX126xGetPower() );
        }
        else
        {
            PrintDebug( "Power should be 0~22.\n" );
        }
    }
    else if( GetCmdFlag( CMD_READ_POWER ) == true )
    {
        PrintDebug( "Power is %d dBm\n", SX126xGetPower() );
    }
    else if( GetCmdFlag( CMD_SET_PREAMBLE ) == true )
    {
        if( s_userCmdData.data[ 0 ] <= 65535 && s_userCmdData.data[ 0 ] >= 1 )
        {
            SX126xLoRaPreambleConfig( s_userCmdData.data[ 0 ] );
            PrintDebug( "OK\n" );
            PrintDebug( "Now preamble size is %d\n", SX126xGetPreambleLen() );
        }
        else
        {
            PrintDebug( "Preamble size should be 8~65535\n" );
        }
    }
    else if( GetCmdFlag( CMD_READ_PREAMBLE ) == true )
    {
        PrintDebug( "Preamble size is %d\n", SX126xGetPreambleLen() );
    }
    else if( GetCmdFlag( CMD_SET_CAD_TIME ) == true )
    {
        if( s_userCmdData.data[ 0 ] > 0 && s_userCmdData.data[ 0 ] < 65535 )
        {
            s_cadTime = s_userCmdData.data[ 0 ];
            PrintDebug( "OK\n" );
            PrintDebug( "Now cad time is %d ms\n", s_cadTime );
        }
        else
        {
            PrintDebug( "Set failed,cad time should be less then 65535 ms\n" );
        }
    }
    else if( GetCmdFlag( CMD_READ_CAD_TIME ) == true )
    {
        PrintDebug( "Cad time is %d ms\n", s_cadTime );
    }
    else if( GetCmdFlag( CMD_SET_LOWDROPT ) == true )
    {
        if( s_userCmdData.data[ 0 ] > 2 )
        {
            PrintDebug( "Set failed,invalid params.0 is close low data rate optimization and 1 is open\n" );
        }
        else if( s_userCmdData.data[ 0 ] == 0 )
        {
            SX126xLowDataRateOptConfig( false );
            PrintDebug( "Now low data rate optimization is closed.\n" );
        }
        else if( s_userCmdData.data[ 0 ] == 1 )
        {
            SX126xLowDataRateOptConfig( true );
            PrintDebug( "Now low data rate optimization is opened.\n" );
        }
    }
    else if( GetCmdFlag( CMD_CAP ) == true )
    {
        if( s_userCmdData.length == 0 )
            PrintDebug( "SX126x CAP: 0x%02X\n", SX126xGetInternalXoCap() );
        else if( s_userCmdData.length == 1 )
        {
            PrintDebug( "OK\n" );
            SX126xSetInternalXoCap( s_userCmdData.data[ 0 ] );
            PrintDebug( "Now 126X cap is 0x%02X \n", SX126xGetInternalXoCap() );
        }
        else
            PrintDebug( "Error params,there should be 1 param.\n" );
    }
}

/**
 * @brief  指令BW值转换至寄存器BW值
 * @param  cmdData 指令BW值
 * @return SX126xLoRaBandwidths_t 寄存器BW有效值
 */
SX126xLoRaBandwidths_t CmdChangeBW( uint32_t cmdData )
{
    switch( cmdData )
    {
        case 7:
            return SX126X_LORA_BW_007;
            break;

        case 10:
            return SX126X_LORA_BW_010;
            break;

        case 15:
            return SX126X_LORA_BW_015;
            break;

        case 20:
            return SX126X_LORA_BW_020;
            break;

        case 31:
            return SX126X_LORA_BW_031;
            break;

        case 41:
            return SX126X_LORA_BW_041;
            break;

        case 62:
            return SX126X_LORA_BW_062;
            break;

        case 125:
            return SX126X_LORA_BW_125;
            break;

        case 250:
            return SX126X_LORA_BW_250;
            break;

        case 500:
            return SX126X_LORA_BW_500;
            break;

        default:
            return SX126X_LORA_BW_125;
            break;
    }
}

/**
 * @brief  寄存器BW值转换至数字值
 * @param  bw 寄存器BW值
 * @return float 实际BW数值，单位KHz
 */
float GetBw( SX126xLoRaBandwidths_t bw )
{
    switch( bw )
    {
        case SX126X_LORA_BW_007:
            return 7.81;
            break;

        case SX126X_LORA_BW_010:
            return 10.42;
            break;

        case SX126X_LORA_BW_015:
            return 15.63;
            break;

        case SX126X_LORA_BW_020:
            return 20.83;
            break;

        case SX126X_LORA_BW_031:
            return 31.25;
            break;

        case SX126X_LORA_BW_041:
            return 41.67;
            break;

        case SX126X_LORA_BW_062:
            return 62.5;
            break;

        case SX126X_LORA_BW_125:
            return 125;
            break;

        case SX126X_LORA_BW_250:
            return 250;
            break;

        case SX126X_LORA_BW_500:
            return 500;
            break;

        default:
            return 125;
            break;
    }
}

/**
 * @brief  检查BW值是否有效指令值
 * @param  bw bw值
 * @return true 有效
 * @return false 无效
 */
bool checkBw( uint16_t bw )
{
    switch( bw )
    {
        case 7:
        case 10:
        case 15:
        case 20:
        case 31:
        case 41:
        case 62:
        case 125:
        case 250:
        case 500:
            return true;
            break;

        default:
            return false;
            break;
    }
}

/**
 * @brief  MCU休眠
 */
void McuSleep( bool isloraSleep )
{
    Tim6Stop();
    GpioSleep( isloraSleep );
    HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );
}
