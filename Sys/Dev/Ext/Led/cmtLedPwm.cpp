//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Dev/Ext/Led/cmtLedPwm.h>

//«адача управлени€ мигающими светодиодами
void 
cmtLedPwmTask() {
  uint_8 code = 0;
  cmtHalLedPwmTaskOn( code );
  while(1) {
    cmtWaitTick( CMT_LED_PWM_PERIOD_MS / CMT_LED_PWM_GRADE );
    code++;
    if( code >= CMT_LED_PWM_GRADE ) code = 0;
    cmtHalLedPwmTaskOn( code );
    }
  }

