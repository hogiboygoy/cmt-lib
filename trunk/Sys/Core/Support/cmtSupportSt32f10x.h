//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include "lib/stm32/inc/stm32f10x_adc.h"
#include "lib/stm32/inc/stm32f10x_bkp.h"
#include "lib/stm32/inc/stm32f10x_can.h"
#include "lib/stm32/inc/stm32f10x_cec.h"
#include "lib/stm32/inc/stm32f10x_crc.h"
#include "lib/stm32/inc/stm32f10x_dac.h"
#include "lib/stm32/inc/stm32f10x_dbgmcu.h"
#include "lib/stm32/inc/stm32f10x_dma.h"
#include "lib/stm32/inc/stm32f10x_exti.h"
#include "lib/stm32/inc/stm32f10x_flash.h"
#include "lib/stm32/inc/stm32f10x_fsmc.h"
#include "lib/stm32/inc/stm32f10x_gpio.h"
#include "lib/stm32/inc/stm32f10x_i2c.h"
#include "lib/stm32/inc/stm32f10x_iwdg.h"
#include "lib/stm32/inc/stm32f10x_pwr.h"
#include "lib/stm32/inc/stm32f10x_rcc.h"
#include "lib/stm32/inc/stm32f10x_rtc.h"
#include "lib/stm32/inc/stm32f10x_sdio.h"
#include "lib/stm32/inc/stm32f10x_spi.h"
#include "lib/stm32/inc/stm32f10x_tim.h"
#include "lib/stm32/inc/stm32f10x_usart.h"
#include "lib/stm32/inc/stm32f10x_wwdg.h"
#include "lib/stm32/inc/misc.h"  /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */
