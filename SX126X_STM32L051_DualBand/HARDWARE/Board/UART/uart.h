#ifndef __UART_H
#define __UART_H
//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include "stm32l0xx_hal.h"
#include <stdio.h>
#include <stdbool.h>

//----------------------------------------------------------------
//                     Public Macros
//----------------------------------------------------------------
#define USART_RX_RECEIVE_MAX_LEN 1024

//----------------------------------------------------------------
//                    Public Typedefs
//----------------------------------------------------------------
typedef struct
{
    bool     recFlag;
    uint8_t  sendFlag;
    uint16_t rxLen;
    uint8_t  rxBuffer[ USART_RX_RECEIVE_MAX_LEN ];
} UartState_s;

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------
extern UART_HandleTypeDef g_uart1Handle;
extern UART_HandleTypeDef g_uart2Handle;
extern UartState_s        g_uart1State;
extern UartState_s        g_uart2State;
//----------------------------------------------------------------
//              Public Function Declarations
//----------------------------------------------------------------
void Usart1Init( uint32_t baudRate );
void Usart1SendByte( uint8_t byte );
void Usart1SendDataByDma( uint8_t* data, uint16_t len );
void Usart1SendDataByBlock( uint8_t* data, uint16_t len );
void Usart2Init( uint32_t baudRate );
void Usart2SendByte( uint8_t byte );
void Usart2SendDataByDma( uint8_t* data, uint16_t len );
void Usart2SendDataByBlock( uint8_t* data, uint16_t len );
void UartReceiveIdle( UART_HandleTypeDef* huart );

#endif
