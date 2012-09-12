//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

bool
CmtLongPress::Step(bool press) {
  bool res = false;
  if( !mPrevSw && press ) {
    //Кнопка нажалась, завести интервалы
    cmtSetCheckTick( mLongTime, mPressInterval );
    mPress = true;
    }
  if( press && mPrevSw && mPress ) {
    //Проверить длинное нажатие
    if( cmtCheckTick(mLongTime) ) {
      //Установить код длинного нажатия
      res = true;
      //Сбросить флаг нажатия
      mPress = false;
      }
    }
  mPrevSw = press;
  return res;
  }
