//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include "gpio.h"
#include "LoRa126x_it.h"

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------

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
void GpioInit( void )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // USART1_TRX_GPIO_CLK_ENABLE();
    USART2_TRX_GPIO_CLK_ENABLE();

    /*-----------------UART GPIO INIT------------------------*/
    /* UART1 TX GPIO pin configuration  */
    // GPIO_InitStruct.Pin       = USART1_TX_PIN;
    // GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    // GPIO_InitStruct.Pull      = GPIO_PULLUP;
    // GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    // GPIO_InitStruct.Alternate = USART1_TX_AF;
    // HAL_GPIO_Init( USART1_TX_GPIO_PORT, &GPIO_InitStruct );

    // /* UART1 RX GPIO pin configuration  */
    // GPIO_InitStruct.Pin       = USART1_RX_PIN;
    // GPIO_InitStruct.Pull      = GPIO_PULLUP;
    // GPIO_InitStruct.Alternate = USART1_RX_AF;
    // HAL_GPIO_Init( USART1_RX_GPIO_PORT, &GPIO_InitStruct );

    /* UART2 TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USART2_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = USART2_TX_AF;
    HAL_GPIO_Init( USART2_TX_GPIO_PORT, &GPIO_InitStruct );

    /* UART2 RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USART2_RX_PIN;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = USART2_RX_AF;
    HAL_GPIO_Init( USART2_RX_GPIO_PORT, &GPIO_InitStruct );
}

void GpioSleep( bool closeSpi )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    HAL_PWREx_EnableUltraLowPower();

    /* UART2 TX GPIO pin configuration  */
    GPIO_InitStruct.Pin  = USART2_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( USART2_TX_GPIO_PORT, &GPIO_InitStruct );

    /* UART2 RX GPIO pin configuration  */
    GPIO_InitStruct.Pin  = USART2_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( USART2_RX_GPIO_PORT, &GPIO_InitStruct );

    GPIO_InitStruct.Pin  = SX126X_RF_DIO1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( SX126X_RF_DIO1_GPIO_PORT, &GPIO_InitStruct );

    GPIO_InitStruct.Pin  = SX126X_RF_BUSY_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( SX126X_RF_BUSY_GPIO_PORT, &GPIO_InitStruct );

    HAL_GPIO_WritePin( SX126X_SPI1_NSS_GPIO_PORT, SX126X_SPI1_NSS_GPIO_PIN, GPIO_PIN_SET );
    GPIO_InitStruct.Pin   = SX126X_SPI1_NSS_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init( SX126X_SPI1_NSS_GPIO_PORT, &GPIO_InitStruct );

    if( closeSpi == true )
    {
        GPIO_InitStruct.Pin  = SX126X_SPI1_SCK_GPIO_PIN | SX126X_SPI1_MISO_GPIO_PIN | SX126X_SPI1_MOSI_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( SX126X_SPI1_SCK_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct );
    }

    __GPIOA_CLK_DISABLE();
    __GPIOB_CLK_DISABLE();
}

void SX126xGpioInit( void )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    HAL_PWREx_EnableUltraLowPower();

    SX126X_RF_SW_GPIO_CLK_ENABLE();
    SX126X_RF_DIO1_GPIO_CLK_ENABLE();
    SX126X_RF_BUSY_GPIO_CKL_ENABLE();
    SX126X_RF_RESET_GPIO_CKL_ENABLE();

	/*
    HAL_GPIO_WritePin( SX126X_RF_SW_TX_GPIO_PORT, SX126X_RF_SW_TX_GPIO_PIN, GPIO_PIN_RESET );
    GPIO_InitStruct.Pin   = SX126X_RF_SW_TX_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init( SX126X_RF_SW_TX_GPIO_PORT, &GPIO_InitStruct );

    HAL_GPIO_WritePin( SX126X_RF_SW_RX_GPIO_PORT, SX126X_RF_SW_RX_GPIO_PIN, GPIO_PIN_RESET );
    GPIO_InitStruct.Pin   = SX126X_RF_SW_RX_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init( SX126X_RF_SW_RX_GPIO_PORT, &GPIO_InitStruct );
	*/
	
//开关控制IO初始化
		HAL_GPIO_WritePin( SX126X_RF_SW_TXSELECT_GPIO_PORT, SX126X_RF_SW_TXSELECT_GPIO_PIN, GPIO_PIN_RESET );
    GPIO_InitStruct.Pin   = SX126X_RF_SW_TXSELECT_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init( SX126X_RF_SW_TXSELECT_GPIO_PORT, &GPIO_InitStruct );

    HAL_GPIO_WritePin( SX126X_RF_SW_POWER_GPIO_PORT, SX126X_RF_SW_POWER_GPIO_PIN, GPIO_PIN_RESET );
    GPIO_InitStruct.Pin   = SX126X_RF_SW_POWER_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init( SX126X_RF_SW_POWER_GPIO_PORT, &GPIO_InitStruct );
		
		HAL_GPIO_WritePin( SX126X_RF_SW_V1_GPIO_PORT, SX126X_RF_SW_V1_GPIO_PIN, GPIO_PIN_RESET );
    GPIO_InitStruct.Pin   = SX126X_RF_SW_V1_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init( SX126X_RF_SW_V1_GPIO_PORT, &GPIO_InitStruct );
		
		HAL_GPIO_WritePin( SX126X_RF_SW_V1_GPIO_PORT, SX126X_RF_SW_V2_GPIO_PIN, GPIO_PIN_RESET );
    GPIO_InitStruct.Pin   = SX126X_RF_SW_V2_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init( SX126X_RF_SW_V2_GPIO_PORT, &GPIO_InitStruct );
		
		
    GPIO_InitStruct.Pin  = SX126X_RF_DIO1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init( SX126X_RF_DIO1_GPIO_PORT, &GPIO_InitStruct );

    GPIO_InitStruct.Pin  = SX126X_RF_BUSY_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init( SX126X_RF_BUSY_GPIO_PORT, &GPIO_InitStruct );

    HAL_GPIO_WritePin( SX126X_RF_RESET_GPIO_PORT, SX126X_RF_RESET_GPIO_PIN, GPIO_PIN_SET );
    GPIO_InitStruct.Pin   = SX126X_RF_RESET_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init( SX126X_RF_RESET_GPIO_PORT, &GPIO_InitStruct );

    /*注意，PA4上电默认是输入模式，不使用的情况下主动配置成模拟避免产生意外功耗*/
    GPIO_InitStruct.Pin   = GPIO_PIN_4;
    GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

    HAL_NVIC_SetPriority( EXTI4_15_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( EXTI4_15_IRQn );
}

void SX126xSpi1GpioInit( void )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    SX126X_SPI1_GPIO_CLK_EBABLE();

    HAL_GPIO_WritePin( SX126X_SPI1_NSS_GPIO_PORT, SX126X_SPI1_NSS_GPIO_PIN, GPIO_PIN_SET );
    GPIO_InitStruct.Pin   = SX126X_SPI1_NSS_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init( SX126X_SPI1_NSS_GPIO_PORT, &GPIO_InitStruct );

    GPIO_InitStruct.Pin       = SX126X_SPI1_SCK_GPIO_PIN | SX126X_SPI1_MISO_GPIO_PIN | SX126X_SPI1_MOSI_GPIO_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = SX126X_SPI1_AF;
    HAL_GPIO_Init( SX126X_SPI1_SCK_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct );
}

bool SX126xReadBusy( void )
{
    if( HAL_GPIO_ReadPin( SX126X_RF_BUSY_GPIO_PORT, SX126X_RF_BUSY_GPIO_PIN ) == GPIO_PIN_RESET )
        return false;
    else
        return true;
}

void SX126xSetRfSw( uint8_t swType )
{
	/*
    switch( swType )
    {
        case RF_SWITCH_OFF:
            HAL_GPIO_WritePin( SX126X_RF_SW_TX_GPIO_PORT, SX126X_RF_SW_TX_GPIO_PIN, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( SX126X_RF_SW_RX_GPIO_PORT, SX126X_RF_SW_RX_GPIO_PIN, GPIO_PIN_RESET );
            break;

        case RF_SWITCH_TX:
            HAL_GPIO_WritePin( SX126X_RF_SW_TX_GPIO_PORT, SX126X_RF_SW_TX_GPIO_PIN, GPIO_PIN_SET );
            HAL_GPIO_WritePin( SX126X_RF_SW_RX_GPIO_PORT, SX126X_RF_SW_RX_GPIO_PIN, GPIO_PIN_RESET );
            break;

        case RF_SWITCH_RX:
            HAL_GPIO_WritePin( SX126X_RF_SW_TX_GPIO_PORT, SX126X_RF_SW_TX_GPIO_PIN, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( SX126X_RF_SW_RX_GPIO_PORT, SX126X_RF_SW_RX_GPIO_PIN, GPIO_PIN_SET );
            break;

        default:
            HAL_GPIO_WritePin( SX126X_RF_SW_TX_GPIO_PORT, SX126X_RF_SW_TX_GPIO_PIN, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( SX126X_RF_SW_RX_GPIO_PORT, SX126X_RF_SW_RX_GPIO_PIN, GPIO_PIN_RESET );
            break;
    }
		*/
		
		switch( swType )
    {
        case RF_SWITCH_OFF:
            HAL_GPIO_WritePin( SX126X_RF_SW_POWER_GPIO_PORT, SX126X_RF_SW_POWER_GPIO_PIN, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( SX126X_RF_SW_TXSELECT_GPIO_PORT, SX126X_RF_SW_TXSELECT_GPIO_PIN, GPIO_PIN_RESET );
						HAL_GPIO_WritePin( SX126X_RF_SW_V1_GPIO_PORT, SX126X_RF_SW_V1_GPIO_PIN, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( SX126X_RF_SW_V2_GPIO_PORT, SX126X_RF_SW_V2_GPIO_PIN, GPIO_PIN_RESET );
            break;

        case RF_SWITCH_TX_LB:
          	HAL_GPIO_WritePin( SX126X_RF_SW_POWER_GPIO_PORT, SX126X_RF_SW_POWER_GPIO_PIN, GPIO_PIN_SET );
            HAL_GPIO_WritePin( SX126X_RF_SW_TXSELECT_GPIO_PORT, SX126X_RF_SW_TXSELECT_GPIO_PIN, GPIO_PIN_RESET );
						HAL_GPIO_WritePin( SX126X_RF_SW_V1_GPIO_PORT, SX126X_RF_SW_V1_GPIO_PIN, GPIO_PIN_SET );
            HAL_GPIO_WritePin( SX126X_RF_SW_V2_GPIO_PORT, SX126X_RF_SW_V2_GPIO_PIN, GPIO_PIN_SET );
            break;

        case RF_SWITCH_RX_LB:
            HAL_GPIO_WritePin( SX126X_RF_SW_POWER_GPIO_PORT, SX126X_RF_SW_POWER_GPIO_PIN, GPIO_PIN_SET );
            HAL_GPIO_WritePin( SX126X_RF_SW_TXSELECT_GPIO_PORT, SX126X_RF_SW_TXSELECT_GPIO_PIN, GPIO_PIN_RESET );
						HAL_GPIO_WritePin( SX126X_RF_SW_V1_GPIO_PORT, SX126X_RF_SW_V1_GPIO_PIN, GPIO_PIN_SET );
            HAL_GPIO_WritePin( SX126X_RF_SW_V2_GPIO_PORT, SX126X_RF_SW_V2_GPIO_PIN, GPIO_PIN_RESET );
            break;
				
				  case RF_SWITCH_TX_HB:
            HAL_GPIO_WritePin( SX126X_RF_SW_POWER_GPIO_PORT, SX126X_RF_SW_POWER_GPIO_PIN, GPIO_PIN_SET );
            HAL_GPIO_WritePin( SX126X_RF_SW_TXSELECT_GPIO_PORT, SX126X_RF_SW_TXSELECT_GPIO_PIN, GPIO_PIN_SET );
						HAL_GPIO_WritePin( SX126X_RF_SW_V1_GPIO_PORT, SX126X_RF_SW_V1_GPIO_PIN, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( SX126X_RF_SW_V2_GPIO_PORT, SX126X_RF_SW_V2_GPIO_PIN, GPIO_PIN_RESET );
            break;

        case RF_SWITCH_RX_HB:
            HAL_GPIO_WritePin( SX126X_RF_SW_POWER_GPIO_PORT, SX126X_RF_SW_POWER_GPIO_PIN, GPIO_PIN_SET );
            HAL_GPIO_WritePin( SX126X_RF_SW_TXSELECT_GPIO_PORT, SX126X_RF_SW_TXSELECT_GPIO_PIN, GPIO_PIN_SET );
						HAL_GPIO_WritePin( SX126X_RF_SW_V1_GPIO_PORT, SX126X_RF_SW_V1_GPIO_PIN, GPIO_PIN_RESET );
            HAL_GPIO_WritePin( SX126X_RF_SW_V2_GPIO_PORT, SX126X_RF_SW_V2_GPIO_PIN, GPIO_PIN_SET );
            break;

        default:
            HAL_GPIO_WritePin( SX126X_RF_SW_POWER_GPIO_PORT, SX126X_RF_SW_POWER_GPIO_PIN, GPIO_PIN_SET );
            HAL_GPIO_WritePin( SX126X_RF_SW_TXSELECT_GPIO_PORT, SX126X_RF_SW_TXSELECT_GPIO_PIN, GPIO_PIN_RESET );
						HAL_GPIO_WritePin( SX126X_RF_SW_V1_GPIO_PORT, SX126X_RF_SW_V1_GPIO_PIN, GPIO_PIN_SET );
            HAL_GPIO_WritePin( SX126X_RF_SW_V2_GPIO_PORT, SX126X_RF_SW_V2_GPIO_PIN, GPIO_PIN_RESET );
            break;
    }
		
		
}

void SX126xSetReset( bool state )
{
    GPIO_PinState pinState;

    pinState = state ? GPIO_PIN_SET : GPIO_PIN_RESET;

    HAL_GPIO_WritePin( SX126X_RF_RESET_GPIO_PORT, SX126X_RF_RESET_GPIO_PIN, pinState );
}

void SX126xSetNss( bool state )
{
    GPIO_PinState pinState;

    pinState = state ? GPIO_PIN_SET : GPIO_PIN_RESET;

    HAL_GPIO_WritePin( SX126X_SPI1_NSS_GPIO_PORT, SX126X_SPI1_NSS_GPIO_PIN, pinState );
}

void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
    if( GPIO_Pin == SX126X_RF_DIO1_GPIO_PIN )
    {
        SX126xIrqCallback();
    }
}
