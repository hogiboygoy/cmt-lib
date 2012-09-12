//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

void cmtInitSysTimer();

#if CMT_SYS_TIMER_MAP == 0
  #define cmtTickCount __raw_readl( GPTIMER1_BASE + TCRR )
#else
  #error "CMT: Map not defined for this CPU"
#endif
