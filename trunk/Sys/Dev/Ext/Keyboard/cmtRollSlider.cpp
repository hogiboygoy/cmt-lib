//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

CmtRollSlider::CmtRollSlider( const uint8 *seq, uint8 sensitivity ) : 
  mTime(0), 
  mSequence(seq),
  mAbsSpeed(128),
  mPrevState(0), 
  mSpeed(0),
  mSensitivity(sensitivity) {
    mMask = 0;
    for( uint_8 i = 0; seq[i]; i++ )
      mMask |= seq[i];
    }

void
CmtRollSlider::Step(uint8 state) {
  uint8 newState = 0;
  state &= mMask;
  if( state ) {
    //Последовательная позиция кнопки в цепочке
    for( uint8 i = 0; mSequence[i]; i++ )
      if( state == mSequence[i] ) {
        newState = i + 1;
        break;
        }
    }
  if( mPrevState != newState ) {
    //Определить направление движения и установить движение на единицу (одна позиция)
    if( mPrevState && newState ) {
      if( mPrevState < newState ) mSpeed = 0x81;
      if( mPrevState > newState ) mSpeed = 0x01;

      //Кнопка нажата, вычислить период между изменениями состояний
      mTime = (cmtTickCount - mTime);
      //Изменения с периодом больше 300мс (низкая скорость) - блокируем
      if( mTime > 300 ) mAbsSpeed = 64;
      else mAbsSpeed = mTime >> 3;
//      if( mAbsSpeed < mSensitivity ) {
//        mSpeed &= 0x80;
//        mAbsSpeed = 64;
//        }
      }
    if( newState ) 
      mTime = cmtTickCount; //Кнопка нажата, инициализировать определитель скорости
    if( mPrevState == 0 )
      mAbsSpeed = 64;
    mPrevState = newState;
    }
//  else mSpeed &= 0x80; //Движения нет
  
  if( mPrevState  == 0 ) {
    //Слайдер отпущен, движение по инерции
//    if( mAbsSpeed < 64 ) {
//      mSpeed &= 0x80;
//      mSpeed |= 64 - mAbsSpeed;
//      mAbsSpeed = 64;
//      }
    //else mSpeed = 0;
    //mSpeed = mAbsSpeed;
    }
  }
