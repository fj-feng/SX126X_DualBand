#ifndef _GPIO_H__
#define _GPIO_H__
//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include "stm32l0xx_hal.h"

//----------------------------------------------------------------
//                     Public Macros
//----------------------------------------------------------------
/* Definition for USART1 GPIO CLK */
#define USART1_TRX_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()
/* Definition for USART1 Pins */
#define USART1_TX_PIN                   GPIO_PIN_6
#define USART1_TX_GPIO_PORT             GPIOB
#define USART1_TX_AF                    GPIO_AF0_USART1
#define USART1_RX_PIN                   GPIO_PIN_7
#define USART1_RX_GPIO_PORT             GPIOB
#define USART1_RX_AF                    GPIO_AF0_USART1

/* Definition for USART2 GPIO CLK */
#define USART2_TRX_GPIO_CLK_ENABLE()    __GPIOA_CLK_ENABLE()
#define USART2_TRX_GPIO_CLK_DSIENABLE() __GPIOA_CLK_DISABLE()
/* Definition for USART2 Pins */
#define USART2_TX_PIN                   GPIO_PIN_2
#define USART2_TX_GPIO_PORT             GPIOA
#define USART2_TX_AF                    GPIO_AF4_USART2
#define USART2_RX_PIN                   GPIO_PIN_3
#define USART2_RX_GPIO_PORT             GPIOA
#define USART2_RX_AF                    GPIO_AF4_USART2

/*Definition for SPI1*/
#define SX126X_SPI1_GPIO_CLK_EBABLE()                                                                                  \
    __HAL_RCC_GPIOA_CLK_ENABLE();                                                                                      \
    __HAL_RCC_GPIOB_CLK_ENABLE()
#define SX126X_SPI1_NSS_GPIO_PIN            GPIO_PIN_7
#define SX126X_SPI1_NSS_GPIO_PORT           GPIOA
#define SX126X_SPI1_SCK_GPIO_PIN            GPIO_PIN_3
#define SX126X_SPI1_MISO_GPIO_PIN           GPIO_PIN_4
#define SX126X_SPI1_MOSI_GPIO_PIN           GPIO_PIN_5
#define SX126X_SPI1_SCK_MISO_MOSI_GPIO_PORT GPIOB
#define SX126X_SPI1_AF                      GPIO_AF0_SPI1

/*Definition for RF SWITCH*/
#define SX126X_RF_SW_GPIO_CLK_ENABLE()                                                                                 \
    __HAL_RCC_GPIOA_CLK_ENABLE();                                                                                      \
    __HAL_RCC_GPIOB_CLK_ENABLE()
#define SX126X_RF_SW_TX_GPIO_PIN          GPIO_PIN_6
#define SX126X_RF_SW_TX_GPIO_PORT         GPIOA
#define SX126X_RF_SW_RX_GPIO_PIN          GPIO_PIN_0
#define SX126X_RF_SW_RX_GPIO_PORT         GPIOB

//20240104 doulband add
#define SX126X_RF_SW_POWER_GPIO_PIN          	GPIO_PIN_1
#define SX126X_RF_SW_POWER_GPIO_PORT          GPIOB
#define SX126X_RF_SW_TXSELECT_GPIO_PIN        GPIO_PIN_6
#define SX126X_RF_SW_TXSELECT_GPIO_PORT       GPIOA
#define SX126X_RF_SW_V1_GPIO_PIN          	 	GPIO_PIN_0
#define SX126X_RF_SW_V1_GPIO_PORT         		GPIOB
#define SX126X_RF_SW_V2_GPIO_PIN          	 	GPIO_PIN_2
#define SX126X_RF_SW_V2_GPIO_PORT         		GPIOB


/*Definition for RF DIO0*/
#define SX126X_RF_DIO1_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE();
#define SX126X_RF_DIO1_GPIO_PIN           GPIO_PIN_12
#define SX126X_RF_DIO1_GPIO_PORT          GPIOA

#define SX126X_RF_BUSY_GPIO_CKL_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE();
#define SX126X_RF_BUSY_GPIO_PIN           GPIO_PIN_15
#define SX126X_RF_BUSY_GPIO_PORT          GPIOA

#define SX126X_RF_RESET_GPIO_CKL_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SX126X_RF_RESET_GPIO_PIN          GPIO_PIN_8
#define SX126X_RF_RESET_GPIO_PORT         GPIOB

#define RF_SWITCH_OFF                     0
#define RF_SWITCH_TX                      1
#define RF_SWITCH_RX                      2
#define RF_SWITCH_TX_LB                      3
#define RF_SWITCH_RX_LB                      4
#define RF_SWITCH_TX_HB                      5
#define RF_SWITCH_RX_HB                      6



//----------------------------------------------------------------
//                    Public Typedefs
//----------------------------------------------------------------

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------

//----------------------------------------------------------------
//              Public Function Declarations
//----------------------------------------------------------------
void GpioInit( void );
void SX126xGpioInit( void );
void SX126xSpi1GpioInit( void );
bool SX126xReadBusy( void );
void SX126xSetRfSw( uint8_t swType );
void SX126xSetReset( bool state );
void SX126xSetNss( bool state );
void GpioSleep( bool closeSpi );

#endif /* _GPIO_H */
