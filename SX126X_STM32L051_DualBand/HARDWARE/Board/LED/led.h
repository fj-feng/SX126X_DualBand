#ifndef __LED_H
#define __LED_H	 

#define LED_MODE_CLK_ENABLE()  		__HAL_RCC_GPIOA_CLK_ENABLE()
#define LED_BUSY_CLK_ENABLE()  		__HAL_RCC_GPIOA_CLK_ENABLE()
#define LED_STAT_CLK_ENABLE()  		__HAL_RCC_GPIOA_CLK_ENABLE()

#define LEDGPIO_MODE				GPIOA
#define LEDGPIO_BUSY				GPIOA
#define LEDGPIO_STAT				GPIOA

#define LEDPIN_MODE					GPIO_PIN_4	 
#define LEDPIN_BUSY					GPIO_PIN_5
#define LEDPIN_STAT					GPIO_PIN_6

#define GET_LED_MODE_LEVEL			HAL_GPIO_ReadPin( LEDGPIO_MODE, LEDPIN_MODE )
#define GET_LED_BUSY_LEVEL			HAL_GPIO_ReadPin( LEDGPIO_BUSY, LEDPIN_BUSY )
#define GET_LED_STAT_LEVEL			HAL_GPIO_ReadPin( LEDGPIO_STAT, LEDPIN_STAT )

#define LEDMODE_ON   				HAL_GPIO_WritePin(LEDGPIO_MODE, LEDPIN_MODE, GPIO_PIN_RESET)
#define LEDMODE_OFF  				HAL_GPIO_WritePin(LEDGPIO_MODE, LEDPIN_MODE, GPIO_PIN_SET)

#define LEDBUSY_ON   				HAL_GPIO_WritePin(LEDGPIO_BUSY, LEDPIN_BUSY, GPIO_PIN_RESET)
#define LEDBUSY_OFF  				HAL_GPIO_WritePin(LEDGPIO_BUSY, LEDPIN_BUSY, GPIO_PIN_SET)

#define LEDSTAT_ON   				HAL_GPIO_WritePin(LEDGPIO_STAT, LEDPIN_STAT, GPIO_PIN_RESET)
#define LEDSTAT_OFF  				HAL_GPIO_WritePin(LEDGPIO_STAT, LEDPIN_STAT, GPIO_PIN_SET)

#define LEDALL_ON    				do{                	\
										LEDMODE_ON;     \
										LEDBUSY_ON;     \
										LEDSTAT_ON;     \
									}while(0)

#define LEDALL_OFF   				do{                 \
										LEDMODE_OFF;    \
										LEDBUSY_OFF;    \
										LEDSTAT_OFF;    \
									}while(0)

void LED_Init(void);//≥ı ºªØ

		 				    
#endif
