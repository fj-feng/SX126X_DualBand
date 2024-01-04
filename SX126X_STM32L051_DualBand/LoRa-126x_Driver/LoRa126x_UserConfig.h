#ifndef _LORA_126X_USER_CONFIG_H__
#define _LORA_126X_USER_CONFIG_H__

#include "LoRa126x_Driver.h"

/*-------------------------------------------------------*/
/*！！！此段定义使用者需要根据实际情况调整！！！*/

/**
 * @brief  使用频段上下限，单位MHz，如使用的频段复位为470~510MHz
 *  时，上限配置下限配置470，上限配置510
 * @note 修改下方宏定义定义的值
 */
#define SX126X_FREQ_LOWER_BOUND_IN_MHZ 470    // 频率下限，MHz
#define SX126X_FREQ_UPPER_BOUND_IN_MHZ 510    // 频率上限，MHz

/**
 * @brief LoRa芯片型号,三选一，影响频段校验
 * @param SX1261
 * @param SX1262
 * @param SX1268
 * @param LLCC68
 * @note 上述参数选其一替换下方的宏定义
 */
#define SX1262

/**
 * @brief LoRa芯片晶振是否有外挂负载电容(使用有源晶振的情况下，
 * 此选项不会起作用，建议默认选择SX126X_CRYSTAL_EXTERNAL_CAP)
 * @param SX126X_CRYSTAL_EXTERNAL_CAP  LoRa晶振外挂负载电容
 * @param SX126X_CRYSTAL_INTERNAL_CAP  LoRa晶振使用内部负载电容
 * @note 上述参数选其一替换下方宏定义
 */
#define SX126X_CRYSTAL_INTERNAL_CAP

/*以下部分影响函数@SX126xSetTxParams，见LoRa126x_Driver.c*/
/*LoRa PA参数，不同模组参数不同，由模组硬件RD给出实际值，修改宏定义值*/
#define SX126X_PADUTYCYCLE    0x04
#define SX126X_HPMAX          0x07
#define SX126X_DEVICESEL      0x00
#define SX126X_PALUT          0x01
/*实际power上限，和PA参数一样，由硬件RD给出实际调试值*/
#define SX126X_USER_POWER_MAX 22
/*底层相比实际power的增量，和PA参数一样，由硬件RD给出实际调试值*/
#define SX126X_POWER_ADDITION 0
/*SX126X_POWER_ADDITION + SX126X_USER_POWER_MAX <= 22
例如，硬件调试时，底层寄存器写22时，实际输出为17dbm
SX126X_USER_POWER_MAX则设置实际的最大输出17，
SX126X_POWER_ADDITION则为底层寄存器最大值减去实际值22-17=5*/

/**
 * @brief LoRa使用的晶振类型
 * @param SX126X_USE_TCXO  LoRa使用有源晶振（温补晶振）
 * @param SX126X_USE_XTAL  LoRa使用无源晶振
 * @note 上述参数选其一替换下方宏定义
 */
#define SX126X_USE_XTAL

/**
 * @brief  如果使用有源晶振，设定DIO3给予有源晶振的电压（使用无源
 * 晶振的情况下，此选项不会起作用，可不关注）
 * @param SX126X_TCXO_CTRL_1_6V
 * @param SX126X_TCXO_CTRL_1_7V
 * @param SX126X_TCXO_CTRL_1_8V
 * @param SX126X_TCXO_CTRL_2_2V
 * @param SX126X_TCXO_CTRL_2_4V
 * @param SX126X_TCXO_CTRL_2_7V
 * @param SX126X_TCXO_CTRL_3_0V
 * @param SX126X_TCXO_CTRL_3_3V
 * @note 上述参数选其一替换下方宏定义对应的值
 */
#define TCXO_CTRL_VOLTAGE SX126X_TCXO_CTRL_2_7V

/**
 * @brief  使用DIO2脚作为射频开关的控制脚之一
 * 需要注意的是，DIO2作为控制脚，仅在TX时拉高，其他状态均拉低，
 * 请务必将DIO2连接至射频开关正确的引脚。
 * @param SX126X_USE_DIO2_AS_RFSWITCH_CTRL      使用DIO2作为射频开关的控制脚
 * @param SX126X_NOT_USE_DIO2_AS_RFSWITCH_CTRL  不使用DIO2作为射频开关的控制脚
 * @note 上述参数选其一替换下方宏定义
 */
#define SX126X_NOT_USE_DIO2_AS_RFSWITCH_CTRL

/**
 * @brief  电源模式，DC-DC or LDO，没有特殊需求的情况下建议使用DC-DC
 * @param SX126X_USE_DCDC
 * @param SX126X_USE_LDO
 * @note 上述参数选其一替换下方宏定义对应的值
 */
#define REGULATOR_MODE           SX126X_USE_DCDC

/**
 * @brief   休眠启动模式，冷启动方式会丢失寄存器信息，唤醒后需要重新初始化
 *          热启动会保留寄存器信息，唤醒后无需重新初始化，冷启动的功耗比热
 *          启动更低。没有极限功耗需求的情况下，建议使用热启动
 * @param SX126X_SLEEP_COLD_START 冷启动
 * @param SX126X_SLEEP_WARM_START 热启动
 * @note 上述参数选其一替换下方宏定义对应的值
 */
#define SX126X_SLEEP_START_MODE  SX126X_SLEEP_WARM_START

/**
 * @brief  LoRa 同步字，用于区分网络类别，同步字相同的设备才能相互通信
 * @param SX126X_PRIVATE_NETWORK    私有网络
 * @param SX126X_PUBLIC_NETWORK     公有网络，通常用于LoRaWan
 * @note 上述参数选其一替换下方宏定义对应的值
 */
#define SX126X_LORA_NETWORK_MODE SX126X_PRIVATE_NETWORK

/**
 * @brief  SX126x 选择TX/RX后芯片自动回到哪个模式下
 *         需要外部晶振持续工作的情况下，要避免自动回到RC模式下导致晶振关闭
 *         需要选择FALLBAC_MODE为XOSC，就不会自动关闭晶振
 *         当使用有源晶振的情况下，此设置无效，强制使用@FALLBACK_STDBY_XOSC
 * @param FALLBACK_STDBY_RC     standby_RC模式
 * @param FALLBACK_STDBY_XOSC   standby_XOSC模式
 * @param FALLBACK_FS           FS模式
 */
#define SX126X_FALLBACK_MODE     FALLBACK_STDBY_RC

/**
 * @brief  BUSY超时释放的循环计数时间，时间和平台时钟有关,详细见LoRa126x_Hal.c
 *         中的@SX126xWaitBusy函数
 */
#define BUSY_TIMEOUT_COUNT       100000

/**
 * @brief  LoRa初始化的校验测试，用于检查部分功能是否正常
 *         调试完成无异常后可以关闭来节约代码空间
 * @param LORA_INIT_DEBUG_MODE_ENABLE     LoRa初始化校验开启
 * @param LORA_INIT_DEBUG_MODE_DISABLE    LoRa初始化校验关闭
 * @note 上述参数选其一替换下方宏定义
 */
#define LORA_INIT_DEBUG_MODE_ENABLE

/**
 * @brief  有源晶振的起振最大时间，由硬件晶振规格书确定，通常为2ms
 *         此参数对无源晶振不起作用
 */
#define LORA_TCXO_TIMEOUT_TIME_MS 2

/*--------------------------------------------------------*/

#endif /* _LORA_126X_USER_CONFIG_H */
