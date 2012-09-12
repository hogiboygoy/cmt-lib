//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//Задача управления мигающими светодиодами
void cmtBlinkTask();

//Должна быть определена
void cmtHalBlinkTaskOn( uint_8 on, uint_8 code );

//Для простоты - макрос
#define CMT_BLINK_TASK_STEP( ledCode, LedOn, LedOff ) \
  if( on && ledCode > code ) LedOn; \
  if( !on && ledCode != CMT_BLINK_TASK ) LedOff;
