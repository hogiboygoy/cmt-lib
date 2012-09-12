//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   General-Purpose Interface
//==============================================================================
#ifndef TI_GPI_H
#define TI_GPI_H

//Pin definition
#define GPIO0            (0x1 << 0)
#define GPIO1            (0x1 << 1)
#define GPIO2            (0x1 << 2)
#define GPIO3            (0x1 << 3)
#define GPIO4            (0x1 << 4)
#define GPIO5            (0x1 << 5)
#define GPIO6            (0x1 << 6)
#define GPIO7            (0x1 << 7)
#define GPIO8            (0x1 << 8)
#define GPIO9            (0x1 << 9)
#define GPIO10            (0x1 << 10)
#define GPIO11            (0x1 << 11)
#define GPIO12            (0x1 << 12)
#define GPIO13            (0x1 << 13)
#define GPIO14            (0x1 << 14)
#define GPIO15            (0x1 << 15)
#define GPIO16            (0x1 << 16)
#define GPIO17            (0x1 << 17)
#define GPIO18            (0x1 << 18)
#define GPIO19            (0x1 << 19)
#define GPIO20            (0x1 << 20)
#define GPIO21            (0x1 << 21)
#define GPIO22            (0x1 << 22)
#define GPIO23            (0x1 << 23)
#define GPIO24            (0x1 << 24)
#define GPIO25            (0x1 << 25)
#define GPIO26            (0x1 << 26)
#define GPIO27            (0x1 << 27)
#define GPIO28            (0x1 << 28)
#define GPIO29            (0x1 << 29)
#define GPIO30            (0x1 << 30)
#define GPIO31            (0x1 << 31)

/* OMAP3 GPIO registers */
#define GPIO1_BASE      0x48310000
#define GPIO2_BASE      0x49050000
#define GPIO3_BASE      0x49052000
#define GPIO4_BASE      0x49054000
#define GPIO5_BASE      0x49056000
#define GPIO6_BASE      0x49058000


#define GPIO_REVISION         0x0000
#define GPIO_SYSCONFIG        0x0010
#define GPIO_SYSSTATUS        0x0014
#define GPIO_IRQSTATUS1       0x0018
#define GPIO_IRQSTATUS2       0x0028
#define GPIO_IRQENABLE2       0x002c
#define GPIO_IRQENABLE1       0x001c
#define GPIO_WAKE_EN          0x0020
#define GPIO_CTRL             0x0030
#define GPIO_OE               0x0034
#define GPIO_DATAIN           0x0038
#define GPIO_DATAOUT          0x003c
#define GPIO_LEVELDETECT0     0x0040
#define GPIO_LEVELDETECT1     0x0044
#define GPIO_RISINGDETECT     0x0048
#define GPIO_FALLINGDETECT    0x004c
#define GPIO_DEBOUNCE_EN      0x0050
#define GPIO_DEBOUNCE_VAL     0x0054
#define GPIO_CLEARIRQENABLE1  0x0060
#define GPIO_SETIRQENABLE1    0x0064
#define GPIO_CLEARWKUENA      0x0080
#define GPIO_SETWKUENA        0x0084
#define GPIO_CLEARDATAOUT     0x0090
#define GPIO_SETDATAOUT       0x0094

inline void
GPIO_Direction( uint32 gpio, uint32 bits, bool asInput ) {
  gpio += GPIO_OE; //Адрес регистра
  uint32 v = __raw_readl( gpio );
  if( asInput ) v |= bits;
  else          v &= ~bits;
  __raw_writel( v, gpio );
  }

inline void
GPIO_SetBits( uint32 gpio, uint32 bits ) {
  __raw_writel( bits, gpio + GPIO_SETDATAOUT );
  }

inline void
GPIO_ResetBits( uint32 gpio, uint32 bits ) {
  __raw_writel( bits, gpio + GPIO_CLEARDATAOUT );
  }

uint32 GPIO_PinBase( uint32 pin );

void GPIO_PinDirection( uint32 pin, bool asInput );

void GPIO_PinSet( uint32 pin );

void GPIO_PinReset( uint32 pin );   

bool GPIO_PinState( uint32 pin );
#endif
