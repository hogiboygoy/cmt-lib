//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_FAM_CORTEX_M_H
#define CMT_FAM_CORTEX_M_H

#include <Sys/Core/Cpu/cmtArch32bit.h>

#define cmtFlash

typedef uint32 CmtPortStackPtr;

#ifdef __cplusplus
extern "C" {
#endif

extern void cmtWaitMKS( uint32 mks );

#ifdef __cplusplus
}
#endif

//Чтение невыровненных данных
#ifdef __cplusplus
inline uint16 cmtRead16( uint8 *base ) { return base[0] + (base[1] << 8); }
inline uint32 cmtRead32( uint8 *base ) { return base[0] + (base[1] << 8) + (base[2] << 16) + (base[3] << 24); }
#else
#define cmtRead16( base )   (((uint8*)base)[0] + (((uint8*)base)[1] << 8))
#define cmtRead32( base )   (((uint8*)base)[0] + (((uint8*)base)[1] << 8) + (((uint8*)base)[2] << 16) + (((uint8*)base)[3] << 24))
#endif

#endif
