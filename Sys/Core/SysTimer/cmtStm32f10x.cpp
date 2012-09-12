//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

volatile uint32 cmtTickCount; //Системный таймер. Тик на каждую мс

extern "C" {
  
void 
SysTick_Handler(void) {
  cmtTickCount++;
  }

}

