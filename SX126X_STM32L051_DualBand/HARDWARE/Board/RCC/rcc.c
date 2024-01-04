/*******************************************************************************
** ��Ȩ:		    ������Ƽ����޹�˾
** �ļ���: 	    rcc.c
** �汾��  	    V1.00   
** ��������: 	  RealView MDK-ARM V5.11
** ����: 		    ydc
** ����:		    RCC����					
** ����: 	      2014-08-19
** �޸���־��	  2014-12-04
                  1���������ע��
** ��Ȩ����     (C)2014-2015 ������Ƽ����޹�˾
*******************************************************************************/

#include "rcc.h"

/*******************************************************************************
** ����	      ����ϵͳʱ��
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
** ����       ��
** ����ֵ	    ��
** ע��
** �޸���־
*******************************************************************************/
//void SystemClock_Config(void)
//{
//  RCC_ClkInitTypeDef RCC_ClkInitStruct;
//  RCC_OscInitTypeDef RCC_OscInitStruct;

//  /* ʹ��PWR����ʱ�� */
//  __PWR_CLK_ENABLE();

//  /* ���õ�ѹ��������ѹ��Χ  */
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//����Ϊ��Χ1

//  /* ͨ��RCCֵ��ȡ��ǰ���������� */
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
** ����	      ��STOPģʽ�˳����ʱ������
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
** ����       ��
** ����ֵ	    ��
** ע��
** �޸���־
*******************************************************************************/
void SystemClockConfig_STOP(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* ʹ��PWR����ʱ�� */
  __PWR_CLK_ENABLE();

  /* ���õ�ѹ��������ѹ��Χ  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//����Ϊ��Χ1

  /* ͨ��RCCֵ��ȡ��ǰ���������� */
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
** ����	      ʹ���ⲿ���پ���LSE              
** ����       ��
** ����ֵ	    ��
** ע��
** �޸���־
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
** ����	      ʹ���ڲ����پ���LSI              
** ����       ��
** ����ֵ	    ��
** ע��
** �޸���־
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
** ����	      ʹ���ڲ����پ���HSI16,4DIV              
** ����       ��
** ����ֵ	    ��
** ע��
** �޸���־
*******************************************************************************/
void HSI_ClockEnable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable LSE clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
}

