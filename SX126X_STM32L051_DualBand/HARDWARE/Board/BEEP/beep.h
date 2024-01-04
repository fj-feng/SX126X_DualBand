#ifndef __BEEP_H
#define __BEEP_H	 
#include "stm32l4xx_hal.h"

#define  beep_off()    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10,GPIO_PIN_RESET);
#define  beep_on()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10,GPIO_PIN_SET);

void BEEP_Init(void);//≥ı ºªØ

		 				    
#endif
