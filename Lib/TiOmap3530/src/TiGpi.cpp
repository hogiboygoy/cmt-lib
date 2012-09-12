//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   General-Purpose Interface
//==============================================================================
#include <Sys/Core/Cpu/cmtCortexA8.h>
#include <Sys/Core/Support/cmtSupportTiOmap3530.h>

static const uint32 tiGpioBase[6] = {
    GPIO1_BASE,
    GPIO2_BASE,
    GPIO3_BASE,
    GPIO4_BASE,
    GPIO5_BASE,
    GPIO6_BASE };

uint32 GPIO_PinBase( uint32 pin ) {
  return tiGpioBase[ pin >> 5];
  }

void 
GPIO_PinDirection( uint32 pin, bool asInput ) {
  GPIO_Direction( tiGpioBase[ pin >> 5 ], 1 << (pin & 0x1f), asInput );
  }
  
void 
GPIO_PinSet( uint32 pin ) {
  GPIO_SetBits( tiGpioBase[ pin >> 5 ], 1 << (pin & 0x1f) );
  }

void 
GPIO_PinReset( uint32 pin ) {
  GPIO_ResetBits( tiGpioBase[ pin >> 5 ], 1 << (pin & 0x1f) );
  }

bool 
GPIO_PinState( uint32 pin ) {
  return __raw_readl( tiGpioBase[ pin >> 5 ] + GPIO_DATAIN ) & (1 << (pin & 0x1f));
  }

