#include "spi.h"

#include "gpio.h"
#include "stm32l0xx_hal.h"

SPI_HandleTypeDef g_spi1Handle;

/**
 * @brief
 */
void SX126xSpi1Init( void )
{
    SX126xSpi1GpioInit();

    __HAL_RCC_SPI1_CLK_ENABLE();

    g_spi1Handle.Instance               = SPI1;
    g_spi1Handle.Init.Mode              = SPI_MODE_MASTER;
    g_spi1Handle.Init.Direction         = SPI_DIRECTION_2LINES;
    g_spi1Handle.Init.DataSize          = SPI_DATASIZE_8BIT;
    g_spi1Handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    g_spi1Handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    g_spi1Handle.Init.NSS               = SPI_NSS_SOFT;
    g_spi1Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    g_spi1Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    g_spi1Handle.Init.TIMode            = SPI_TIMODE_DISABLE;
    g_spi1Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    g_spi1Handle.Init.CRCPolynomial     = 7;

    if( HAL_SPI_Init( &g_spi1Handle ) != HAL_OK )
    {
    }
    __HAL_SPI_ENABLE( &g_spi1Handle );
}

uint8_t SX126xSpi1ReadWriteByte( uint8_t data )
{
    uint8_t RxDat;
    HAL_SPI_TransmitReceive( &g_spi1Handle, &data, &RxDat, 1, 1000 );
    return RxDat;
}
