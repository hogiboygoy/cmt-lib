//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

volatile uint32 cmtTickCount; //��������� ������. ��� �� ������ ��

extern "C" {
  
void 
SysTick_Handler(void) {
  cmtTickCount++;
  }

}

