//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include "main.h"

#include "timer.h"
#include "uart.h"
#include "rcc.h"
#include "gpio.h"
#include "dma.h"
#include "log.h"
#include "LoRa126x_Fun.h"
#include "LoRa126x_Hal.h"
#include "spi.h"
#include "TestFun.h"

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------
extern __IO uint32_t uwTick;
bool                 tim6Flag = false;
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
static void SystemClock_Config( void );
void        Error_Handler( void );

//----------------------------------------------------------------
//                   Function Definitions
//----------------------------------------------------------------

//
int main( void )
{
    HAL_Init();
    SystemClock_Config();

    GpioInit();
    Tim6MsInit( 1 );
    Tim6Start();
    DmaInit();
    // Usart1Init( 921600 );
    Usart2Init( 921600 );
    LogFunDeliver( Usart2SendByte, (uint32_t*) &uwTick );

    /*传递RF硬件相关和时间相关的函数功能*/
    SX126xHardwareDeliver( SX126xGpioInit,
                           SX126xReadBusy,
                           SX126xSetRfSw,
                           SX126xSetReset,
                           SX126xSpi1Init,
                           SX126xSpi1ReadWriteByte,
                           SX126xSetNss,
                           HAL_Delay );

    PrintDebug( "Program start!\n" );

    /*用于配置WB25-96BN4模组*/
    if( SX126X_FREQ_LOWER_BOUND_IN_MHZ > 800 )
    {
        /*首次配置LoRa参数*/
        LoRaUserConfig( 876480000,
                        SX126X_LORA_BW_125,
                        SX126X_LORA_SF7,
                        SX126X_LORA_CR_4_5,
                        22,
                        SX126X_LORA_PKT_EXPLICIT,
                        SX126X_LORA_CRC_ON,
                        SX162X_LORA_IQ_NORMAL,
                        8,
                        10 );
    }
    /*用于配置WB25-75BN4模组*/
    else
    {
        /*首次配置LoRa参数*/
        LoRaUserConfig( 483690000,
                        SX126X_LORA_BW_125,
                        SX126X_LORA_SF7,
                        SX126X_LORA_CR_4_5,
                        17,
                        SX126X_LORA_PKT_EXPLICIT,
                        SX126X_LORA_CRC_ON,
                        SX162X_LORA_IQ_NORMAL,
                        8,
                        10 );
    }

    /*LoRa初始化*/
    if( SX126xLoRaInit() != SX126X_LORA_INIT_OK )
    {
        PrintDebug( "LoRa init failed!\n" );
        while( 1 )
            ;
    }

    PrintDebug( "LoRa init successfully!\n" );

    g_uart2State.recFlag = false;

    while( 1 )
    {
        UserUartProcess();    // 串口处理进程
        RfCmdProcess();       // 射频指令处理进程
        RfTest();             // 射频功能测试进程
    }
}

// systemcoreclock init
static void SystemClock_Config( void )
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    __PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE1 );

    HAL_RCC_GetOscConfig( &RCC_OscInitStruct );

    /* Enable HSI Oscillator and activate PLL with HSI as source */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLLMUL_4;
    RCC_OscInitStruct.PLL.PLLDIV          = RCC_PLLDIV_2;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    HAL_RCC_OscConfig( &RCC_OscInitStruct );

    if( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK )
    {
        Error_Handler();
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
    clocks dividers */
    RCC_ClkInitStruct.ClockType =
        ( RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 );
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;    // PLLCLK:32MHz
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_1 );
}

void Error_Handler( void )
{
    while( 1 )
    {
    }
}
