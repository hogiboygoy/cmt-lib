//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#if CMT_SYS_TIMER_MAP == 0
#define cmtTickCount T0TC

//ru Инициализировать системный таймер
//ru Предделитель обеспечивает 1мс
//ru Начинаем считать с нуля
#define cmtInitSysTimer() \
  T0PR = (CMT_CPU_FRIQ / 4 / 1000) - 1; \
  T0TCR = 2; \
  T0TCR = 1;   //Запустить таймер
#else
  #error "CMT: This map not defined for this CPU"
#endif
