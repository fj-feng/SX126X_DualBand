//----------------------------------------------------------------
//                       Includes
//----------------------------------------------------------------
#include "timer.h"

#include <stdint.h>

//----------------------------------------------------------------
//                    Public Variables
//----------------------------------------------------------------
TIM_HandleTypeDef g_tim2Handle;
TIM_HandleTypeDef g_tim6Handle;
extern bool       tim6Flag;
extern uint32_t   g_TestTimeCount;

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

//----------------------------------------------------------------
//                   Function Definitions
//----------------------------------------------------------------

/**
 * @brief  初始化TIM2
 * @param  time 定时器定时周期，单位us
 */
void Tim2UsInit( uint32_t time )
{
    TIM_ClockConfigTypeDef  sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;

    if( time == 0 )
        return;

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if( HAL_TIM_ConfigClockSource( &g_tim2Handle, &sClockSourceConfig ) != HAL_OK )
    {
        return;
    }

    __HAL_RCC_TIM2_CLK_ENABLE();

    g_tim2Handle.Instance           = TIM2;
    g_tim2Handle.Init.Prescaler     = 32 - 1;
    g_tim2Handle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    g_tim2Handle.Init.Period        = time - 1;
    g_tim2Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    if( HAL_TIM_Base_Init( &g_tim2Handle ) != HAL_OK )
    {
        return;
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if( HAL_TIMEx_MasterConfigSynchronization( &g_tim2Handle, &sMasterConfig ) != HAL_OK )
    {
        return;
    }

    HAL_NVIC_SetPriority( TIM2_IRQn, 0, 0 );    // TM2�жϣ������ȼ�0�������ȼ�0
    HAL_NVIC_EnableIRQ( TIM2_IRQn );            // IRQͨ����ʹ��
}

/**
 * @brief  初始化TIM6
 * @param  time 定时器定时周期，单位ms
 */
void Tim6MsInit( uint32_t time )
{
    TIM_ClockConfigTypeDef sClockSourceConfig;

    if( time == 0 )
        return;

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if( HAL_TIM_ConfigClockSource( &g_tim2Handle, &sClockSourceConfig ) != HAL_OK )
    {
        return;
    }

    __HAL_RCC_TIM6_CLK_ENABLE();

    g_tim6Handle.Instance           = TIM6;
    g_tim6Handle.Init.Prescaler     = 3200 - 1;
    g_tim6Handle.Init.Period        = time * 10 - 1;
    g_tim6Handle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    g_tim6Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    if( HAL_TIM_Base_Init( &g_tim6Handle ) != HAL_OK )
    {
        return;
    }

    HAL_NVIC_SetPriority( TIM6_IRQn, 1, 1 );
    HAL_NVIC_EnableIRQ( TIM6_IRQn );
}

/**
 * @brief  启动TIM6计时
 */
void Tim6Start( void )
{
    HAL_TIM_Base_Start_IT( &g_tim6Handle );
}

void Tim6Stop( void )
{
    HAL_TIM_Base_Stop_IT( &g_tim6Handle );
}

void HAL_TIM_PeriodElapsedCallback( TIM_HandleTypeDef* htim )
{
    static uint32_t tim6Counter = 0;
    if( htim->Instance == TIM6 )
    {
        tim6Counter++;
        g_TestTimeCount++;
    }
}
