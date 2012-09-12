//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_FAM_STM8_H
#define CMT_FAM_STM8_H

#include <Sys/Core/Cpu/cmtArch8bit.h>

#define cmtFlash

typedef uint16 CmtPortStackPtr;

#ifdef __cplusplus
extern "C" {
#endif

extern void cmtWaitMKS( uint32 mks );

#ifdef __cplusplus
}
#endif

//Чтение невыровненных данных
#ifdef __cplusplus
inline uint16 cmtRead16( uint8 *base ) { return *((uint16*)base); }
inline uint32 cmtRead32( uint8 *base ) { return *((uint32*)base); }
#else
#define cmtRead16( base )   (*((uint16*)base))
#define cmtRead32( base )   (*((uint32*)base))
#endif

#endif
