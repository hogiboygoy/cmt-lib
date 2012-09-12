//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#include <Sys/Core/Cpu/cmtArch32bit.h>

#define cmtFlash

typedef uint32 CmtPortStackPtr;

#define cmtWaitMKS( mks )

inline uint16 cmtRead16( uint8 *base ) { return *(uint16*)base; }
inline uint32 cmtRead32( uint8 *base ) { return *(uint32*)base; }
