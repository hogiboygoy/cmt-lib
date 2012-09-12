//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_LED_PWM_H
#define CMT_LED_PWM_H

//Задача управления мигающими светодиодами
void cmtLedPwmTask();

//Должна быть определена
void cmtHalLedPwmTaskOn( uint_8 code );

//Для простоты - макрос
#define CMT_LED_PWM_TASK_STEP( ledCode, LedOn, LedOff ) \
  if( ledCode > code ) LedOn; \
  else                 LedOff;

//CMT_TASK_ADD( cmtLedPwmTask, 0, 100 );

#endif
