//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   Timers, WatchDog timers
//==============================================================================
#include <Sys/Core/Cpu/cmtCortexA8.h>
#include <Sys/Core/Support/cmtSupportTiOmap3530.h>

void 
WD_Disable( uint32 wdBase ) {
  __raw_writel( 0x0000aaaa, wdBase + WSPR );
  while( __raw_readl( wdBase + WWPS ) );
  __raw_writel( 0x00005555, wdBase + WSPR );
  }

