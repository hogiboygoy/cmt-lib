//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMTSDCARD_H
#define CMTSDCARD_H


#if defined(CMT_TI_OMAP3530)
  #include <Sys/Dev/Abstract/Sd/cmtSdTiOmap3530.h>
#else
  #error "CMT: Not released for this CPU"
#endif

#endif // CMTSDCARD_H
