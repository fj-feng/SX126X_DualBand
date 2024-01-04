#include "LoRa126x_Hal.h"

#include "LoRa126x_UserConfig.h"

SX126xHardware_s g_sx126xHardware = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

/**
 * @brief   初始化SX126x硬件，将对应功能的函数作为参数引入(传入函数名即可)，
 *  函数样式请见@SX126xHardware_s，控制高频开关的引脚若由硬
 *  件控制，则对应函数使用NULL作为参数
 * @param  GpioInit
 *      *****************************
 *      *  @brief  与126x相关的IO初始化
 *      *  void GpioInit( void );
 *      ******************************
 * @param  ReadBusy
 *      *****************************
 *      *  @brief  读取busy引脚状态
 *      *  @param [OUT] 引脚电平，true为高电平，false为低电平
 *      *  bool ReadBusy( void );
 *      ******************************
 * @param  SetSwCtrl 若无此函数功能（例如控制由硬件决定），传入NULL
 *      *****************************
 *      *  @brief  配置高频开关状态
 *      *  @param [In] swType 控制状态，0表示OFF，1表示TX，2表示RX
 *      *  void SetSwCtrl( uint8_t swType );
 *      ******************************
 * @param  SetReset
 *      *****************************
 *      *  @brief  配置126x reset引脚
 *      *  @param [In] state 引脚电平
 *      *  void SetReset( bool state );
 *      ******************************
 * @param  SpiInit
 *      *****************************
 *      *  @brief  与126x的SPI初始化,SPI的NSS设置为软件控制
 *      *  void SpiInit( void );
 *      ******************************
 * @param  SpiRW
 *      *****************************
 *      *  @brief  126xSPI单字节数据读写
 *      *          此函数内不包含对NSS的控制
 *      *  @param [IN] data SPI写入的数据
 *      *  @param [OUT] SPI读出的数据
 *      *  uint8_t SpiRW( uint8_t data );
 *      ******************************
 * @param  SetNss
 *      *****************************
 *      *  @brief  配置SPI NSS引脚电平
 *      *  @param [In] state 引脚电平
 *      *  void SetNss( bool state );
 *      ******************************
 * @param  DelayTimeMs
 *      *****************************
 *      *  @brief  延迟函数
 *      *  @param [IN] delay 延迟时间，单位ms
 *      *  void DelayTimeMs( uint32_t delay );
 *      ******************************
 */
void SX126xHardwareDeliver( void ( *GpioInit )( void ),
                            bool ( *ReadBusy )( void ),
                            void ( *SetSwCtrl )( uint8_t ),
                            void ( *SetReset )( bool ),
                            void ( *SpiInit )( void ),
                            uint8_t ( *SpiRW )( uint8_t ),
                            void ( *SetNss )( bool ),
                            void ( *DelayTimeMs )( uint32_t ) )
{
    g_sx126xHardware.SX126xGpioInit         = GpioInit;
    g_sx126xHardware.ReadBusyState          = ReadBusy;
    g_sx126xHardware.SetSwitchCtrl          = SetSwCtrl;
    g_sx126xHardware.SetSX126xResetPin      = SetReset;
    g_sx126xHardware.SX126xSpiInit          = SpiInit;
    g_sx126xHardware.SX126xSpiReadWriteByte = SpiRW;
    g_sx126xHardware.SetSX126xSpiNssPin     = SetNss;
    g_sx126xHardware.SX126xDelayMs          = DelayTimeMs;
}

/**
 * @brief  SX126X硬件资源初始化
 */
void SX126xHardwareInit( void )
{
    g_sx126xHardware.SX126xGpioInit();
    g_sx126xHardware.SX126xSpiInit();
}

/**
 * @brief  等待BUSY释放
 * @param  timeout  超时时间
 * @return true     BUSY正常释放
 * @return false    BUSY超时依旧未释放
 */
bool SX126xWaitBusy( void )
{
    volatile uint32_t conut = BUSY_TIMEOUT_COUNT;

    while( conut-- )
    {
        if( g_sx126xHardware.ReadBusyState() == false )
            return true;
    }
    return false;
}

/**
 * @brief  126x硬件复位
 */
void SX126xReset( void )
{
    g_sx126xHardware.SetSX126xResetPin( RF_RESET );
    g_sx126xHardware.SX126xDelayMs( 20 );
    g_sx126xHardware.SetSX126xResetPin( RF_SET );
    g_sx126xHardware.SX126xDelayMs( 10 );
}
