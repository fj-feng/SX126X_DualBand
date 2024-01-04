#ifndef __RCC_H
#define __RCC_H

#include "stm32l0xx_hal.h"

void SystemClockConfig_STOP( void );
void LSE_ClockEnable( void );
void LSI_ClockEnable( void );
void HSI_ClockEnable( void );

#endif
