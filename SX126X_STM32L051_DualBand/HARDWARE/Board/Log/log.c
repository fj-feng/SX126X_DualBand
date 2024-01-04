//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include "log.h"

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------
bool g_enableLog = true;    // true开启log，false关闭log
//----------------------------------------------------------------
//                     Private Macros
//----------------------------------------------------------------
uint32_t* g_logSysTime = NULL;

//----------------------------------------------------------------
//                    Private Typedefs
//----------------------------------------------------------------

//----------------------------------------------------------------
//                   Private Variables
//----------------------------------------------------------------

//----------------------------------------------------------------
//              Private Function Declarations
//----------------------------------------------------------------
static void ( *UartTransmitOneByte )( uint8_t data );

//----------------------------------------------------------------
//                   Function Definitions
//----------------------------------------------------------------

/**
 * @brief  传递printf函数需要的底层资源
 * @param  UartTransmit 串口单字节发送函数
 * @param  sysTime 系统ms级计数值，可用系统滴答定时器值
 */
void LogFunDeliver( void ( *UartTransmit )( uint8_t data ), uint32_t* sysTime )
{
    UartTransmitOneByte = UartTransmit;
    g_logSysTime        = sysTime;
}

/**
 * @brief  打印HEX格式的数据
 * @param  buf 	数据地址
 * @param  size 数据长度
 */
void PrintDebugHex( uint8_t* buf, uint8_t size )
{
    if( buf == NULL || size == 0 )
        return;

    uint8_t i;

    for( i = 0; i < size; i++ )
    {
        PrintDebug( "%02X ", buf[ i ] );
    }
}

/**
 * @brief  重定义fputc函数
 */
int fputc( int ch, FILE* f )
{
    UartTransmitOneByte( (uint8_t) ch );
    return ch;
}
