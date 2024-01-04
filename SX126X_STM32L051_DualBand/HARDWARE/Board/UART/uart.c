//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include "uart.h"

#include "dma.h"
#include "rcc.h"
#include <stdarg.h>

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------
UART_HandleTypeDef g_uart1Handle;
UART_HandleTypeDef g_uart2Handle;
UartState_s        g_uart1State;
UartState_s        g_uart2State;

//----------------------------------------------------------------
//                     Private Macros
//----------------------------------------------------------------

//----------------------------------------------------------------
//                    Private Typedefs
//----------------------------------------------------------------

//----------------------------------------------------------------
//                   Private Variables
//----------------------------------------------------------------

//----------------------------------------------------------------
//              Private Function Declarations
//----------------------------------------------------------------

//----------------------------------------------------------------
//                   Function Definitions
//----------------------------------------------------------------

/**
 * @brief  USART初始化
 */
void Usart1Init( uint32_t baudRate )
{
    RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    RCC_PeriphCLKInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig( &RCC_PeriphCLKInitStruct );

    /* Enable USART1 clock */
    __USART1_CLK_ENABLE();

    g_uart1Handle.Instance                    = USART1;
    g_uart1Handle.Init.BaudRate               = baudRate;
    g_uart1Handle.Init.WordLength             = UART_WORDLENGTH_8B;
    g_uart1Handle.Init.StopBits               = UART_STOPBITS_1;
    g_uart1Handle.Init.Parity                 = UART_PARITY_NONE;
    g_uart1Handle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    g_uart1Handle.Init.Mode                   = UART_MODE_TX_RX;
    g_uart1Handle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    g_uart1Handle.Init.OverSampling           = UART_OVERSAMPLING_16;
    g_uart1Handle.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    g_uart1Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if( HAL_UART_Init( &g_uart1Handle ) != HAL_OK )
    {
    }
    DmaUartInit( &g_uart1Handle );

    HAL_NVIC_SetPriority( USART1_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( USART1_IRQn );
    __HAL_UART_ENABLE_IT( &g_uart1Handle, UART_IT_IDLE );
    HAL_UART_Receive_DMA( &g_uart1Handle, g_uart1State.rxBuffer, USART_RX_RECEIVE_MAX_LEN );
}

/**
 * @brief  UART1发送单字节数据
 * @param  byte 数据
 */
void Usart1SendByte( uint8_t byte )
{
    while( ( USART1->ISR & UART_FLAG_TXE ) != UART_FLAG_TXE )
    {
    }
    HAL_UART_Transmit( &g_uart1Handle, &byte, 1, 1000 );
}

/**
 * @brief  UART1发送数据串,DMA传输方式
 * @param  data 数据首地址
 * @param  len  数据长度
 */
void Usart1SendDataByDma( uint8_t* data, uint16_t len )
{
    if( data == NULL || len == 0 )
        return;

    HAL_UART_Transmit_DMA( &g_uart1Handle, data, len );
}

/**
 * @brief  UART1发送数据串,阻塞的方式
 * @param  data 数据首地址
 * @param  len  数据长度
 */
void Usart1SendDataByBlock( uint8_t* data, uint16_t len )
{
    if( data == NULL || len == 0 )
        return;

    while( len > 0 )
    {
        Usart1SendByte( *data );
        data++;
        len--;
    }
}

/**
 * @brief  USAR2初始化
 */
void Usart2Init( uint32_t baudRate )
{
    RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    RCC_PeriphCLKInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig( &RCC_PeriphCLKInitStruct );

    /* Enable USART1 clock */
    __USART2_CLK_ENABLE();

    g_uart2Handle.Instance                    = USART2;
    g_uart2Handle.Init.BaudRate               = baudRate;
    g_uart2Handle.Init.WordLength             = UART_WORDLENGTH_8B;
    g_uart2Handle.Init.StopBits               = UART_STOPBITS_1;
    g_uart2Handle.Init.Parity                 = UART_PARITY_NONE;
    g_uart2Handle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    g_uart2Handle.Init.Mode                   = UART_MODE_TX_RX;
    g_uart2Handle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    g_uart2Handle.Init.OverSampling           = UART_OVERSAMPLING_16;
    g_uart2Handle.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    g_uart2Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if( HAL_UART_Init( &g_uart2Handle ) != HAL_OK )
    {
    }
    DmaUartInit( &g_uart2Handle );

    HAL_NVIC_SetPriority( USART2_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( USART2_IRQn );
    __HAL_UART_ENABLE_IT( &g_uart2Handle, UART_IT_IDLE );
    HAL_Delay( 10 );
    HAL_UART_Receive_DMA( &g_uart2Handle, g_uart2State.rxBuffer, USART_RX_RECEIVE_MAX_LEN );
}

/**
 * @brief  UART2发送单字节数据
 * @param  byte 数据
 */
void Usart2SendByte( uint8_t byte )
{
    while( ( USART2->ISR & UART_FLAG_TXE ) != UART_FLAG_TXE )
    {
    }
    HAL_UART_Transmit( &g_uart2Handle, &byte, 1, 1000 );
}

/**
 * @brief  UART2发送数据串,DMA传输方式
 * @param  data 数据首地址
 * @param  len  数据长度
 */
void Usart2SendDataByDma( uint8_t* data, uint16_t len )
{
    if( data == NULL || len == 0 )
        return;

    HAL_UART_Transmit_DMA( &g_uart2Handle, data, len );
}

/**
 * @brief  UART1发送数据串,阻塞的方式
 * @param  data 数据首地址
 * @param  len  数据长度
 */
void Usart2SendDataByBlock( uint8_t* data, uint16_t len )
{
    if( data == NULL || len == 0 )
        return;

    while( len > 0 )
    {
        Usart2SendByte( *data );
        data++;
        len--;
    }
}

/**
 * @brief 串口空闲中断处理函数
 * @param  huart
 */
void UartReceiveIdle( UART_HandleTypeDef* huart )
{
    uint32_t temp;

    if( ( __HAL_UART_GET_FLAG( huart, UART_FLAG_IDLE ) != RESET ) )
    {
        __HAL_UART_CLEAR_IDLEFLAG( huart );
        HAL_UART_DMAStop( huart );
        temp = huart->hdmarx->Instance->CNDTR;

        if( huart->Instance == USART1 )
        {
            g_uart1State.rxLen = USART_RX_RECEIVE_MAX_LEN - temp;
            if( g_uart1State.rxLen != 0 )
                g_uart1State.recFlag = true;

            HAL_UART_Receive_DMA( huart, g_uart1State.rxBuffer, USART_RX_RECEIVE_MAX_LEN );
        }
        else if( huart->Instance == USART2 )
        {
            g_uart2State.rxLen = USART_RX_RECEIVE_MAX_LEN - temp;
            if( g_uart2State.rxLen != 0 )
                g_uart2State.recFlag = true;

            HAL_UART_Receive_DMA( huart, g_uart2State.rxBuffer, USART_RX_RECEIVE_MAX_LEN );
        }
    }
    if( __HAL_UART_GET_FLAG( huart, UART_FLAG_ORE ) != RESET )
    {
        __HAL_UART_CLEAR_OREFLAG( huart );
        g_uart2State.recFlag = false;
        HAL_UART_DMAStop( huart );
        HAL_UART_Receive_DMA( huart, g_uart2State.rxBuffer, USART_RX_RECEIVE_MAX_LEN );
    }
}
