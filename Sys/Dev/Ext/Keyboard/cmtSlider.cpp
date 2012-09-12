//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
CmtSlider::CmtSlider( const uint8 *seq ) :
  mPrevState(0),
  mMask(0),
  mSequence(seq),
  mTime(0),
  mSpeed(0) {
    for( uint_8 i = 0; seq[i]; i++ )
      mMask |= seq[i];
    }

void
CmtSlider::Step( uint8 state ) {
  state &= mMask; //Выделить только интересующие биты
  if( mPrevState != state ) {
    //Произошло изменение
    if( state == 0 ) {
      //Кнопки отпущены, обнулить скорость
      mSpeed = 0;
      }
    else if( mPrevState == 0 ) {
      //Кнопки вновь нажаты (ранее отпущены)
      mSpeed = 0;
      mTime = cmtTickCount;
      }
    else {
      //Кнопки нажаты, выполнить пересчет скорости
      
      //Определить направление
      bool dir = false;
      for( uint_8 i = 0; mSequence[i]; i++ ) {
        if( ((state & mSequence[i]) && (mPrevState & mSequence[i]) == 0) ) {
          mSpeed = dir ? 0 : 0x80; //Направление
          break;
          }
        if( ((state & mSequence[i]) == 0 && (mPrevState & mSequence[i])) ) {
          mSpeed = dir ? 0x80 : 0; //Направление
          break;
          }
        if( mPrevState & mSequence[i] ) dir = true;
        }
      
      //Вычислить скорость
      uint32 speed = cmtTickCount - mTime;
      speed >>= 2;
      mTime = cmtTickCount;
      if( speed < 0x7f ) mSpeed |= 0x7f - speed;
      }
    mPrevState = state;
    }
  }
