#ifndef _LOG_H__
#define _LOG_H__
//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//----------------------------------------------------------------
//                     Public Macros
//----------------------------------------------------------------
/*输出带[DEBUG]字样，系统时间，log执行时所在的函数名的log内容*/
#define PrintDebugTitle( format, ... )                                                                                 \
    g_enableLog ? printf( "\r\n[DEBUG] %10d <%s> " format "", *g_logSysTime, __FUNCTION__, ##__VA_ARGS__ ) : NULL

/*正常的printf功能*/
#define PrintDebug( format, ... ) g_enableLog ? printf( "" format "", ##__VA_ARGS__ ) : NULL

//----------------------------------------------------------------
//                    Public Typedefs
//----------------------------------------------------------------

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------
extern bool      g_enableLog;
extern uint32_t* g_logSysTime;

//----------------------------------------------------------------
//              Public Function Declarations
//----------------------------------------------------------------
void LogFunDeliver( void ( *UartTransmit )( uint8_t data ), uint32_t* sysTime );
int  fputc( int ch, FILE* f );
void PrintDebugHex( uint8_t* buf, uint8_t size );

#endif
