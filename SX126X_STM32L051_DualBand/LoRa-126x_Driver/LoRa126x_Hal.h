#ifndef _LORA_126X_HAL_H__
#define _LORA_126X_HAL_H__

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

#define RF_SET        true
#define RF_RESET      false

#define RF_SWITCH_OFF 0
#define RF_SWITCH_TX  1
#define RF_SWITCH_RX  2

/*LoRa硬件资源结构体*/
typedef struct
{
    /**
     * @brief  与126x相关的IO初始化
     */
    void ( *SX126xGpioInit )( void );

    /**
     * @brief  读取busy引脚状态
     * @param [OUT] 引脚电平，true为高电平，false为低电平
     */
    bool ( *ReadBusyState )( void );

    /**
     * @brief  配置高频开关的状态
     * @param [IN] swType 高频开关状态，0表示关闭，1表示TX通路，2表示RX通路
     */
    void ( *SetSwitchCtrl )( uint8_t swType );

    /**
     * @brief  配置126x reset引脚
     * @param [In] state 引脚电平
     */
    void ( *SetSX126xResetPin )( bool state );

    /**
     * @brief  与126x的SPI初始化,SPI的NSS设置为软件控制
     */
    void ( *SX126xSpiInit )( void );

    /**
     * @brief  126xSPI单字节数据读写
     *         !!此函数内不包含对NSS的控制，否则会导致LoRa驱动SPI交互出错!!
     * @param [IN] data SPI写入的数据
     * @param [OUT] SPI读出的数据
     */
    uint8_t ( *SX126xSpiReadWriteByte )( uint8_t data );

    /**
     * @brief  配置SPI NSS引脚电平
     * @param [In] state 引脚电平
     */
    void ( *SetSX126xSpiNssPin )( bool state );

    /**
     * @brief  延迟函数
     * @param [IN] delay 延迟时间，单位ms
     */
    void ( *SX126xDelayMs )( uint32_t delay );

} SX126xHardware_s;

extern SX126xHardware_s g_sx126xHardware;

//
bool SX126xWaitBusy( void );
void SX126xReset( void );
void SX126xHardwareInit( void );
void SX126xHardwareDeliver( void ( *GpioInit )( void ),
                            bool ( *ReadBusy )( void ),
                            void ( *SetSwCtrl )( uint8_t ),
                            void ( *SetReset )( bool ),
                            void ( *SpiInit )( void ),
                            uint8_t ( *SpiRW )( uint8_t ),
                            void ( *SetNss )( bool ),
                            void ( *DelayTimeMs )( uint32_t ) );

#endif
