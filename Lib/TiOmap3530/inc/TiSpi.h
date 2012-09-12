//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   SPI
//==============================================================================
#ifndef TI_SPI_H
#define TI_SPI_H

#define MCSPI1_BASE 0x48098000
#define MCSPI2_BASE 0x4809A000
#define MCSPI3_BASE 0x480B8000
#define MCSPI4_BASE 0x480BA000

#define MCSPI_REVISION       0x00
#define MCSPI_SYSCONFIG      0x10
#define MCSPI_SYSSTATUS      0x14
#define MCSPI_IRQSTATUS      0x18
#define MCSPI_IRQENABLE      0x1c
#define MCSPI_WAKEUPENABLE   0x20
#define MCSPI_SYST           0x24
#define MCSPI_MODULCTRL      0x28
#define MCSPI_CHxCONF(x)     (0x2c + 0x14*(x))
#define MCSPI_CHxSTAT(x)     (0x30 + 0x14*(x))
#define MCSPI_CHxCTRL(x)     (0x34 + 0x14*(x))
#define MCSPI_TXx(x)         (0x38 + 0x14*(x))
#define MCSPI_RXx(x)         (0x3c + 0x14*(x))
#define MCSPI_XFERLEVEL      0x7c

#endif

