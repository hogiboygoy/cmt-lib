//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

extern volatile uint32 cmtTickCount; //��������� ������. ��� �� ������ ��

#define cmtInitSysTimer()   SysTick_Config(CMT_CORE_CLK / 1000)
