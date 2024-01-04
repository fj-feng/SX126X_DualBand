#ifndef _LORA_126X_IT_H__
#define _LORA_126X_IT_H__

#include "stdint.h"
#include <stdbool.h>

/*SX126x中断标志位枚举定义*/
typedef enum
{
    SX126X_IQR_ABANDONED              = ( 1 << 10 ),
    SX126X_IRQ_FLAG_TX_TIMEOUT        = ( 1 << 9 ),
    SX126X_IRQ_FLAG_RX_TIMEOUT        = ( 1 << 8 ),
    SX126X_IRQ_FLAG_CAD_DETECTED      = ( 1 << 7 ),
    SX126X_IRQ_FLAG_CAD_DONE          = ( 1 << 6 ),
    SX126X_IRQ_FLAG_CRC_ERROR         = ( 1 << 5 ),
    SX126X_IRQ_FLAG_HEADER_ERROR      = ( 1 << 4 ),
    SX126X_IRQ_FLAG_HEADER_VALID      = ( 1 << 3 ),
    SX126X_IRQ_FLAG_PREAMBLE_DETECTED = ( 1 << 2 ),
    SX126X_IRQ_FLAG_RX_DONE           = ( 1 << 1 ),
    SX126X_IRQ_FLAG_TX_DONE           = ( 1 << 0 ),
} SX126xIrqFlagType_t;

bool SX126xGetIrqFlag( SX126xIrqFlagType_t irq );
void SX126xClearIrqFlag( SX126xIrqFlagType_t irq );
void SX126xClearAllIrqFlag( void );
void SX126xIrqCallback( void );

#endif /* _LORA_126X_IT_H */
