//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#if CMT_SYS_TIMER_MAP == 0
//���������� �� ������� 1 GPTIMER1
void
cmtInitSysTimer() {
  //���������� �������� ������� (32.768���)
  __raw_clearl( MBIT0, CM_CLKSEL_WKUP );
  //��������� ������������
  PRCM_Clock( GPTIMER1_BASE, true );
  //���������    ST     div 32
  __raw_writel( MBIT0 | (4 << 2) | MBIT5, GPTIMER1_BASE + TCLR );
  }
#else
  #error "CMT: This map not defined for this CPU"
#endif
