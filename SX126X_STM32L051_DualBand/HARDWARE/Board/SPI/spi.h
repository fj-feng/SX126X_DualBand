#ifndef __SPI_H_
#define __SPI_H_

#include "stm32l0xx_hal.h"
#include "stdint.h"

void    SX126xSpi1Init( void );
uint8_t SX126xSpi1ReadWriteByte( uint8_t data );

#endif
