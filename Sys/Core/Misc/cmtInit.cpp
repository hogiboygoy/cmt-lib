//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

void
cmtInit() {
  
  //��������� ������ ����������, ������������� ������ � �.�.
#ifdef CMT_CPU_INIT
  cmtInitSystem();
#endif
  
  //���������������� ��������� ������������� ����������
#ifdef CMT_CPU_USER_INIT
  CMT_CPU_USER_INIT();
#endif
  
  //������
#ifdef CMT_MEMORY_LOC
  cmtInitHeap();
#endif

  //Tasking
#ifdef CMT_KERNEL_MAP
  cmtInitTask();
#endif
  
  //��������� ������
#ifdef CMT_SYS_TIMER_MAP
  cmtInitSysTimer();
#endif
  
  //���� � ���������
#ifdef CMT_SYSTEM_TIME
  cmtInitSystemTime();
#endif
  
  }
