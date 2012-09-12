//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_FAM_AVR8_H
#define CMT_FAM_AVR8_H

#include <Sys/Core/Cpu/cmtArch8bit.h>

#define cmtFlash       __flash

typedef uint8 CmtPortStackPtr[4];

#define cmtWaitMKS( mks ) __delay_cycles( (CMT_CORE_CLK / 1000000) * mks )
#endif
