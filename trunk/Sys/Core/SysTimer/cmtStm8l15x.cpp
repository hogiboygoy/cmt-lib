//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

volatile uint32 cmtTickCount; //Системный таймер. Тик на каждую мс

extern "C" {
  
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler, 23) {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  TIM1_ClearITPendingBit( TIM1_IT_Update );
  cmtTickCount++;
  }

}

void 
cmtInitSysTimer() {
  //Разрешить тактирование таймера 1
  CLK->PCKENR2 |= MBIT1;
  
  /* Time base configuration */
  TIM1_TimeBaseInit( 0, TIM1_CounterMode_Up, CMT_CORE_CLK / 1000, 0 );
  
  //Разрешить прерывания при авто-перезагрузке
  TIM1_ITConfig( TIM1_IT_Update, ENABLE );
  
  //Разрешить счет таймера
  TIM1_Cmd( ENABLE );
  }
