//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include "dma.h"
#include "uart.h"

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------
DMA_HandleTypeDef g_DmaUart1RxHandle;
DMA_HandleTypeDef g_DmaUart1TxHandle;
DMA_HandleTypeDef g_DmaUart2RxHandle;
DMA_HandleTypeDef g_DmaUart2TxHandle;

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

void DmaInit( void )
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel4_5_6_7_IRQn interrupt configuration */
    HAL_NVIC_SetPriority( DMA1_Channel4_5_6_7_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( DMA1_Channel4_5_6_7_IRQn );
    /* DMA interrupt init */
    /* DMA1_Channel2_3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority( DMA1_Channel2_3_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( DMA1_Channel2_3_IRQn );
}

void DmaUartInit( UART_HandleTypeDef* uartHandle )
{
    if( uartHandle->Instance == USART1 )
    {
        g_DmaUart1RxHandle.Instance                 = DMA1_Channel3;
        g_DmaUart1RxHandle.Init.Request             = DMA_REQUEST_4;
        g_DmaUart1RxHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        g_DmaUart1RxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
        g_DmaUart1RxHandle.Init.MemInc              = DMA_MINC_ENABLE;
        g_DmaUart1RxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        g_DmaUart1RxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        g_DmaUart1RxHandle.Init.Mode                = DMA_NORMAL;
        g_DmaUart1RxHandle.Init.Priority            = DMA_PRIORITY_LOW;

        if( HAL_DMA_Init( &g_DmaUart1RxHandle ) != HAL_OK )
        {
            while(1);
        }
        __HAL_LINKDMA( uartHandle, hdmarx, g_DmaUart1RxHandle );

        g_DmaUart1TxHandle.Instance                 = DMA1_Channel2;
        g_DmaUart1TxHandle.Init.Request             = DMA_REQUEST_4;
        g_DmaUart1TxHandle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        g_DmaUart1TxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
        g_DmaUart1TxHandle.Init.MemInc              = DMA_MINC_ENABLE;
        g_DmaUart1TxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        g_DmaUart1TxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        g_DmaUart1TxHandle.Init.Mode                = DMA_NORMAL;
        g_DmaUart1TxHandle.Init.Priority            = DMA_PRIORITY_LOW;

        if( HAL_DMA_Init( &g_DmaUart1TxHandle ) != HAL_OK )
        {
            while(1);
        }
        __HAL_LINKDMA( uartHandle, hdmatx, g_DmaUart1TxHandle );
    }
    else if( uartHandle->Instance == USART2 )
    {
        g_DmaUart2RxHandle.Instance                 = DMA1_Channel5;
        g_DmaUart2RxHandle.Init.Request             = DMA_REQUEST_4;
        g_DmaUart2RxHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        g_DmaUart2RxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
        g_DmaUart2RxHandle.Init.MemInc              = DMA_MINC_ENABLE;
        g_DmaUart2RxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        g_DmaUart2RxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        g_DmaUart2RxHandle.Init.Mode                = DMA_NORMAL;
        g_DmaUart2RxHandle.Init.Priority            = DMA_PRIORITY_LOW;

        if( HAL_DMA_Init( &g_DmaUart2RxHandle ) != HAL_OK )
        {
            while(1);
        }
        __HAL_LINKDMA( uartHandle, hdmarx, g_DmaUart2RxHandle );

        g_DmaUart2TxHandle.Instance                 = DMA1_Channel4;
        g_DmaUart2TxHandle.Init.Request             = DMA_REQUEST_4;
        g_DmaUart2TxHandle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        g_DmaUart2TxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
        g_DmaUart2TxHandle.Init.MemInc              = DMA_MINC_ENABLE;
        g_DmaUart2TxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        g_DmaUart2TxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        g_DmaUart2TxHandle.Init.Mode                = DMA_NORMAL;
        g_DmaUart2TxHandle.Init.Priority            = DMA_PRIORITY_LOW;

        if( HAL_DMA_Init( &g_DmaUart2TxHandle ) != HAL_OK )
        {
            while(1);
        }
        __HAL_LINKDMA( uartHandle, hdmatx, g_DmaUart2TxHandle );
    }
}
