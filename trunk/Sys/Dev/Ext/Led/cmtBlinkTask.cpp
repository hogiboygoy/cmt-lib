//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
void
cmtBlinkTask() {
  //Предварительное выключение
  cmtHalBlinkTaskOn( 0, CMT_BLINK_TASK );
  while(1) {
    uint_8 code;
    for( code = 0; code < CMT_BLINK_TASK; code++ ) {
      cmtHalBlinkTaskOn( 1, code );
      cmtWaitTick( CMT_BLINK_TASK_ON );
      cmtHalBlinkTaskOn( 0, code );
      cmtWaitTick( CMT_BLINK_TASK_OFF );
      }
    cmtWaitTick( CMT_BLINK_TASK_PAUSE );
    }
  }
