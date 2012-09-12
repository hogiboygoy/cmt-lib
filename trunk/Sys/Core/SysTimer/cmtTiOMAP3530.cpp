//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#if CMT_SYS_TIMER_MAP == 0
//Реализация на таймере 1 GPTIMER1
void
cmtInitSysTimer() {
  //Определить исходную частоту (32.768кГц)
  __raw_clearl( MBIT0, CM_CLKSEL_WKUP );
  //Разрешить тактирование
  PRCM_Clock( GPTIMER1_BASE, true );
  //Настроить    ST     div 32
  __raw_writel( MBIT0 | (4 << 2) | MBIT5, GPTIMER1_BASE + TCLR );
  }
#else
  #error "CMT: This map not defined for this CPU"
#endif
