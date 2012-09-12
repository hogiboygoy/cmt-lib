//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   I2C
//==============================================================================
#ifndef TI_I2C_H
#define TI_I2C_H

#define I2C1_BASE 0x48070000
#define I2C2_BASE 0x48072000
#define I2C3_BASE 0x48060000

#define I2C_REV     0x00
#define I2C_IE      0x04
#define I2C_STAT    0x08
#define I2C_WE      0x0c
#define I2C_SYSS    0x10
#define I2C_BUF     0x14
#define I2C_CNT     0x18
#define I2C_DATA    0x1c
#define I2C_SYSC    0x20
#define I2C_CON     0x24
#define I2C_OA0     0x28
#define I2C_SA      0x2c
#define I2C_PSC     0x30
#define I2C_SCLL    0x34
#define I2C_SCLH    0x38
#define I2C_SYSTEST 0x3c
#define I2C_BUFSTAT 0x40
#define I2C_OA1     0x44
#define I2C_OA2     0x48
#define I2C_OA3     0x4c
#define I2C_ACTOA   0x50
#define I2C_SBLOCK  0x54


#endif

