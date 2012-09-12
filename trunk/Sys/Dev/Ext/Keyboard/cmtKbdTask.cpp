//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
uint32 cmtKbdKey = 0;

uint32 
cmtKbdPeekKey() {  //Возвращает код нажатой клавиши (0-если нет нажатия)
  return cmtKbdKey;
  }

uint_8
cmtKbdGetKey() {  //Возвращает код нажатой клавиши и сбрасывает клавишу (если нет нажатия - ожидает нажатия)
  uint32 key;
  if( cmtKbdKey == 0 ) {
    //Ожидаем нажатие
    cmtWaitSetUInt32( &cmtKbdKey );
    }
  key = cmtKbdKey;
  cmtKbdKey = 0;
  return key;
  }

void
cmtKbdTask() {
  static uint32 curKey = 0;
  static uint_8 drimmleCount = 0;
  static uint_8 repeateCount;
  while(1) {
    uint32 key = cmtHalKbdGetPress();
    if( key == curKey ) {
      if( drimmleCount == 51 ) {
        //Ожидаем первую паузу
        if( repeateCount == (CMT_KBD_REPEATE_PERIOD / CMT_KBD_PERIOD) ) {
          cmtKbdKey = curKey;
          repeateCount = 0;
          }
        else repeateCount++;
        }
      else if( drimmleCount == 50 ) {
        //Ожидаем первую паузу
        if( repeateCount == (CMT_KBD_REPEATE_PAUSE / CMT_KBD_PERIOD) ) {
          cmtKbdKey = curKey;
          drimmleCount = 51;
          repeateCount = 0;
          }
        else repeateCount++;
        }
      else if( drimmleCount == 4 ) {
        //Код стабилен
        if( curKey != 0 ) {
          cmtKbdKey = curKey;
          drimmleCount = 50;
          repeateCount = 0;
          }
        }
      else drimmleCount++;
      }
    else {
      //Код сменился
      curKey = key;
      drimmleCount = 0;
      }
    cmtWaitTick( CMT_KBD_PERIOD ); //Опрос клавиатуры через каждые 10мс
    }
  }
