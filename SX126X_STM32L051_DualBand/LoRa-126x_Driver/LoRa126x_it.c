#include "LoRa126x_it.h"
#include "LoRa126x_Fun.h"

#define BIT_SET   1
#define BIT_RESET 0

/**
 * @brief  SX126x中断状态联合体
 */
typedef union
{
    struct
    {
        uint16_t txDoneFlag         : 1;    // 发送完成标志
        uint16_t rxDoneFlag         : 1;    // 接收完成标志
        uint16_t preambleDetFlag    : 1;    // 前导码检测标志
        uint16_t headerValidFlag    : 1;    // 包头有效标志
        uint16_t headerErrorFlag    : 1;    // 包头错误标志
        uint16_t crcErrorFlag       : 1;    // CRC错误标志
        uint16_t cadDoneFlag        : 1;    // cad完成标志
        uint16_t cadDetectedFlag    : 1;    // cadDetected标志
        uint16_t rxTimeOutFlag      : 1;    // RX超时标志
        uint16_t txTimeoutFlag      : 1;    // TX超时标志
        uint16_t rfIrqAbandonedFlag : 1;    // 射频中断舍弃标志,该标志需要手动清除
        uint16_t                    : 5;
    } bit;
    uint16_t sx126xIrq;
} SX126xIrqFlag_s;

static SX126xIrqFlag_s s_sx126xIrqFlag = { 0 };    // SX126x中断标志信息

static void            SX126xSetIrqFlag( SX126xIrqFlagType_t irq );

/**
 * @brief  获取应用层IRQ标志
 * @param  irq IQR类型
 * @return true 状态高
 * @return false 状态低
 */
bool SX126xGetIrqFlag( SX126xIrqFlagType_t irq )
{
    return ( ( s_sx126xIrqFlag.sx126xIrq & irq ) ? true : false );
}

/**
 * @brief  清除对应的IRQ标志
 * @param  irq IQR类型
 */
void SX126xClearIrqFlag( SX126xIrqFlagType_t irq )
{
    s_sx126xIrqFlag.sx126xIrq &= ~irq;
}

/**
 * @brief  清除全部中断标志(除了射频舍弃标志外)
 */
void SX126xClearAllIrqFlag( void )
{
    s_sx126xIrqFlag.sx126xIrq &= SX126X_IQR_ABANDONED;
}

/**
 * @brief  设置对应的IRQ标志
 * @param  irq IQR类型
 */
static void SX126xSetIrqFlag( SX126xIrqFlagType_t irq )
{
    s_sx126xIrqFlag.sx126xIrq |= irq;
}

/**
 * @brief   SX126x中断服务程序
 *          需要将此函数放置在实际硬件DIO1连接的MCU的IO的外部中断服务程序中。
 */
void SX126xIrqCallback( void )
{
    uint16_t irqFlags;

    /*如果有进程在操作SPI，为了不引起SPI异常，中断将被抛弃并设置标志位信息*/
    if( GetSX126xSpiState() == true )
    {
        s_sx126xIrqFlag.bit.rfIrqAbandonedFlag = true;
        return;
    }

    irqFlags = SX126xGetIrqStatus();
    SX126xClearIrqStatus( SX126X_IRQ_ALL );

    if( irqFlags & SX126X_IRQ_PREAMBLE_DETECTED )
    {
        SX126xSetIrqFlag( SX126X_IRQ_FLAG_PREAMBLE_DETECTED );
        /*User Code Start*/

        /*User Code End*/
    }
    if( irqFlags & SX126X_IRQ_HEADER_VALID )
    {
        SX126xSetIrqFlag( SX126X_IRQ_FLAG_HEADER_VALID );
        /*User Code Start*/

        /*User Code End*/
    }
    else if( irqFlags & SX126X_IRQ_HEADER_ERROR )
    {
        SX126xSetIrqFlag( SX126X_IRQ_FLAG_HEADER_ERROR );
        /*User Code Start*/

        /*User Code End*/
    }

    if( irqFlags & SX126X_IRQ_TX_DONE )
    {
        SX126xSetIrqFlag( SX126X_IRQ_FLAG_TX_DONE );
        g_rfState = RF_STANDBY;
        /*User Code Start*/

        /*User Code End*/
    }
    else if( irqFlags & SX126X_IRQ_RX_DONE )
    {
        if( SX126xGetHeaderType() == SX126X_LORA_PKT_IMPLICIT )
            SX126xClearTimeoutEvent();

        SX126xSetIrqFlag( SX126X_IRQ_FLAG_RX_DONE );

        if( SX126xGetCrc() == SX126X_LORA_CRC_ON )
        {
            if( irqFlags & SX126X_IRQ_CRC_ERROR )
            {
                SX126xSetIrqFlag( SX126X_IRQ_FLAG_CRC_ERROR );
                /*User Code Start*/

                /*User Code End*/
            }
            else
            {
                /*接收完成且CRC正确，读取数据*/
                SX126xGetRxPkt();
                /*User Code Start*/

                /*User Code End*/
            }
        }
        else
            SX126xGetRxPkt();

        g_rfState = RF_STANDBY;
        /*User Code Start*/

        /*User Code End*/
    }
    else if( irqFlags & SX126X_IRQ_CAD_DONE )
    {
        SX126xSetIrqFlag( SX126X_IRQ_FLAG_CAD_DONE );
        if( irqFlags & SX126X_IRQ_CAD_DETECTED )
        {
            SX126xSetIrqFlag( SX126X_IRQ_FLAG_CAD_DETECTED );

            /*CAD_RX模式下，检测到活跃信号后会自动进入RX*/
            if( SX126xGetCadExitModes() == SX126X_CAD_RX )
                g_rfState = RF_RX_ING;
            else
            {
                g_rfState = RF_STANDBY;
                /*User Code Start*/

                /*User Code End*/
            }
        }
        else
        {
            g_rfState = RF_STANDBY;
            /*User Code Start*/

            /*User Code End*/
        }
    }
    else if( irqFlags & SX126X_IRQ_TIMEOUT )
    {
        if( g_rfState == RF_TX_ING )
        {
            SX126xSetIrqFlag( SX126X_IRQ_FLAG_TX_TIMEOUT );
            /*User Code Start*/

            /*User Code End*/
        }
        else if( g_rfState == RF_RX_ING )
        {
            SX126xSetIrqFlag( SX126X_IRQ_FLAG_RX_TIMEOUT );
            /*User Code Start*/

            /*User Code End*/
        }
        g_rfState = RF_STANDBY;
        /*User Code Start*/

        /*User Code End*/
    }
}