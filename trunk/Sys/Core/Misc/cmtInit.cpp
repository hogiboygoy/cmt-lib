//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

void
cmtInit() {
  
  //Настройка частот процессора, инициализация памяти и т.п.
#ifdef CMT_CPU_INIT
  cmtInitSystem();
#endif
  
  //Пользовательская процедура инициализации процессора
#ifdef CMT_CPU_USER_INIT
  CMT_CPU_USER_INIT();
#endif
  
  //Память
#ifdef CMT_MEMORY_LOC
  cmtInitHeap();
#endif

  //Tasking
#ifdef CMT_KERNEL_MAP
  cmtInitTask();
#endif
  
  //Системный таймер
#ifdef CMT_SYS_TIMER_MAP
  cmtInitSysTimer();
#endif
  
  //Часы и календарь
#ifdef CMT_SYSTEM_TIME
  cmtInitSystemTime();
#endif
  
  }
