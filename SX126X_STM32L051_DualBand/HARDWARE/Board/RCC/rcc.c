/*******************************************************************************
** 版权:		    利尔达科技有限公司
** 文件名: 	    rcc.c
** 版本：  	    V1.00   
** 工作环境: 	  RealView MDK-ARM V5.11
** 作者: 		    ydc
** 功能:		    RCC配置					
** 日期: 	      2014-08-19
** 修改日志：	  2014-12-04
                  1、整理、添加注释
** 版权所有     (C)2014-2015 利尔达科技有限公司
*******************************************************************************/

#include "rcc.h"

/*******************************************************************************
** 功能	      配置系统时钟
              System Clock source            = PLL (HSI)
              SYSCLK(Hz)                     = 32000000
              HCLK(Hz)                       = 32000000
              AHB Prescaler                  = 1
              APB1 Prescaler                 = 1
              APB2 Prescaler                 = 1
              HSI Frequency(Hz)              = 16000000
              PLL_MUL                        = 4
              PLL_DIV                        = 2
              Flash Latency(WS)              = 1
              Main regulator output voltage  = Scale1 mode                       
** 参数       无
** 返回值	    无
** 注意
** 修改日志
*******************************************************************************/
//void SystemClock_Config(void)
//{
//  RCC_ClkInitTypeDef RCC_ClkInitStruct;
//  RCC_OscInitTypeDef RCC_OscInitStruct;

//  /* 使能PWR外设时钟 */
//  __PWR_CLK_ENABLE();

//  /* 配置电压调整器电压范围  */
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//配置为范围1

//  /* 通过RCC值获取当前的振荡器配置 */
//  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

//  /* Enable HSI Oscillator and activate PLL with HSI as source */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
//  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
//  RCC_OscInitStruct.HSICalibrationValue = 0x10;
//  HAL_RCC_OscConfig(&RCC_OscInitStruct);  

//  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
//   clocks dividers */
//  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
//  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
//}

/*******************************************************************************
** 功能	      从STOP模式退出后的时钟配置
              System Clock source            = PLL (HSI)
              SYSCLK(Hz)                     = 32000000
              HCLK(Hz)                       = 32000000
              AHB Prescaler                  = 1
              APB1 Prescaler                 = 1
              APB2 Prescaler                 = 1
              HSI Frequency(Hz)              = 16000000
              PLL_MUL                        = 4
              PLL_DIV                        = 2
              Flash Latency(WS)              = 1
              Main regulator output voltage  = Scale1 mode                       
** 参数       无
** 返回值	    无
** 注意
** 修改日志
*******************************************************************************/
void SystemClockConfig_STOP(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* 使能PWR外设时钟 */
  __PWR_CLK_ENABLE();

  /* 配置电压调整器电压范围  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//配置为范围1

  /* 通过RCC值获取当前的振荡器配置 */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  /* After wake-up from STOP reconfigure the system clock: Enable HSI and PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
  }
}

/*******************************************************************************
** 功能	      使能外部低速晶振LSE              
** 参数       无
** 返回值	    无
** 注意
** 修改日志
*******************************************************************************/
void LSE_ClockEnable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable LSE clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
}

/*******************************************************************************
** 功能	      使能内部低速晶振LSI              
** 参数       无
** 返回值	    无
** 注意
** 修改日志
*******************************************************************************/
void LSI_ClockEnable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable LSE clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSEState = RCC_LSI_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
}

/*******************************************************************************
** 功能	      使能内部高速晶振HSI16,4DIV              
** 参数       无
** 返回值	    无
** 注意
** 修改日志
*******************************************************************************/
void HSI_ClockEnable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable LSE clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
}

