#include "led.h"
#include "stm32l0xx_hal.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
		
	/* GPIO Ports Clock Enable */
	LED_MODE_CLK_ENABLE();
	LED_BUSY_CLK_ENABLE();
	LED_STAT_CLK_ENABLE();
	
	GPIO_InitStruct.Pin 	= 	LEDPIN_MODE;
	GPIO_InitStruct.Mode 	= 	GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull 	= 	GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= 	GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(LEDGPIO_MODE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 	= 	LEDPIN_STAT;
	GPIO_InitStruct.Mode 	= 	GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull 	= 	GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= 	GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(LEDGPIO_STAT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 	= 	LEDPIN_BUSY;
	GPIO_InitStruct.Mode 	= 	GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull 	= 	GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= 	GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(LEDGPIO_BUSY, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LEDGPIO_MODE, LEDPIN_MODE, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LEDGPIO_STAT, LEDPIN_STAT, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LEDGPIO_BUSY, LEDPIN_BUSY, GPIO_PIN_SET);
}


