//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#define CMT_ST_L3G4200_DUMMY 0x33

//Регистры
#define CMT_ST_L3G4200_WHO_AM_I         0x0f
#define CMT_ST_L3G4200_CTRL_REG1        0x20
#define CMT_ST_L3G4200_CTRL_REG2        0x21
#define CMT_ST_L3G4200_CTRL_REG3        0x22
#define CMT_ST_L3G4200_CTRL_REG4        0x23
#define CMT_ST_L3G4200_CTRL_REG5        0x24

#define CMT_ST_L3G4200_OUT_TEMP        0x26
#define CMT_ST_L3G4200_STATUS_REG       0x27

#define CMT_ST_L3G4200_OUT_X_L          0x28
#define CMT_ST_L3G4200_OUT_X_H          0x29
#define CMT_ST_L3G4200_OUT_Y_L          0x2a
#define CMT_ST_L3G4200_OUT_Y_H          0x2b
#define CMT_ST_L3G4200_OUT_Z_L          0x2c
#define CMT_ST_L3G4200_OUT_Z_H          0x2d
#define CMT_ST_L3G4200_FIFO_CTRL_REG    0x2e
#define CMT_ST_L3G4200_FIFO_SRC_REG     0x2f

#define CMT_ST_L3G4200_INT1_CFG         0x30
#define CMT_ST_L3G4200_INT1_SRC         0x31
#define CMT_ST_L3G4200_INT1_TSH_XH      0x32
#define CMT_ST_L3G4200_INT1_TSH_XL      0x33
#define CMT_ST_L3G4200_INT1_TSH_YH      0x34
#define CMT_ST_L3G4200_INT1_TSH_YL      0x35
#define CMT_ST_L3G4200_INT1_TSH_ZH      0x36
#define CMT_ST_L3G4200_INT1_TSH_ZL      0x37
#define CMT_ST_L3G4200_INT1_DURATION    0x38

