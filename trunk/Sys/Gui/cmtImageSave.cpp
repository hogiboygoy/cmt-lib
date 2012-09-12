//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

/* Формат
  Команда
  5-0 атр
  7-6 код команды
      00 повтор атр+1 раз следующего за командой цвета)
      01 группа в количестве атр+2, +1 байт смещение
      10 клон по смещению назад атр+1
      11 клон из предыдущей строки по смещению атр+1-32
  Байт смещения группы (назад)
  5-0 знач
  6 - 0-текущая страница
      1-предыдущая страница
  7 - 0-длина группы + знач
      1-длина группы + знач - 64 + длина строки

  */
bool
CmtImageCmiTestLine( uint32 *ptr, int ref, int len ) {
  //Проверить на совпадение линий ref назад длиной len
  for( int i = 0; i < len; i++ )
    if( ptr[i-ref] != ptr[i] ) return false;
  return true;
  }



bool
CmtImage::SaveCmt( CmtFile &os ) {
  int32 count = mFrameCount * mFrameSize;
  if( count == 0 || mFrames == 0 ) return false;
  //Записать сигнатуру
  os.WriteBinU32( CMT_IMAGE_SIGNATURE );
  //Сохранить размерности
  os.WriteBinU32( mFrameCount );
  os.WriteBinU32( mFrameSize );
  os.WriteBinU32( mSize.x );
  os.WriteBinU32( mSize.y );
  //Нулевой пиксел
  os.WriteBinU8( 0 );
  uint32 color = mFrames[0] & 0xffffff;
  os.Write( &color, 3 );
  //Начать процесс передачи
  for( int32 i = 1; i < count; ) {
    //Вариант 0 (копирование)
    uint8 atr0 = 0;
    color = mFrames[i] & 0xffffff;
    while( atr0 < 63 ) {
      if( i + atr0 + 1 >= count || color != (mFrames[i+atr0+1] & 0xffffff) ) break;
      atr0 += 1;
      }

    //Вариант 2
    uint8 atr2;
    for( atr2 = 0; atr2 < 64 && (mFrames[i-atr2-1] & 0xffffff) != color; atr2++ )
      if( i == atr2 + 1 ) atr2 = 63;

    //Вариант 3
    uint8 atr3 = 64;
    if( atr2 > 63 && i > mSize.x + 32 ) {
      if( mSize.x > 32 ) {
        for( atr3 = 0; atr3 < 64 && (mFrames[i+32-mSize.x-atr3] & 0xffffff) != color; atr3++ )
          if( i <= mSize.x - 32 + atr3 ) atr3 = 63;
        }
      else {
        for( atr3 = 33 - mSize.x; atr3 < 32 + mSize.x && (mFrames[i+32-mSize.x-atr3] & 0xffffff) != color; atr3++ )
          if( i <= mSize.x - 32 + atr3 ) atr3 = 63;
        if( atr3 >= 32 + mSize.x ) atr3 = 64;
        }
      }

    //Вариант 1
    uint8 atr1 = 64;
    uint8 boff = 0;
    if( mSize.x > 32 ) {
      for( uint8 atr1t = i + 65 > count ? count - i : 65; atr1t > 1 && atr1 == 64; atr1t-- ) {
        //Проверить все варианты смещения
        for( int offs = 0; offs < 256; offs++ ) {
          int back;
          if( offs & 0x80 ) {
            //Предыдущая страница
            if( offs & 0x40 ) {
              //На предыдущей строке предыдущей страницы
              back = atr1t + (offs & 0x3f) - 32 + mSize.x + mFrameSize;
              }
            else {
              //На текущей строке предыдущей страницы
              back = atr1t + (offs & 0x3f) + mFrameSize;
              }
            }
          else {
            //Текущая страница
            if( offs & 0x40 ) {
              //Предыдущая строка
              back = atr1t + (offs & 0x3f) - 32 + mSize.x;
              }
            else {
              //Текущая строка
              back = atr1t + (offs & 0x3f);
              }
            }
          if( back <= i ) {
            if( CmtImageCmiTestLine( mFrames + i, back, atr1t ) ) {
              atr1 = atr1t - 2;
              boff = (uint8)offs;
              break;
              }
            }
          else break;
          }
        }
      }

    //Выбор варианта
    if( atr1 < 64 ) {
      if( atr0 < atr1+2 ) {
        //Вариант 1
        os.WriteBinU8( atr1 | 0x40 );
        os.WriteBinU8( boff );
        i += atr1+2;
        }
      else {
        //Вариант 0
        os.WriteBinU8( atr0 );
        os.Write( &color, 3 );
        i += atr0+1;
        }
      }
    else {
      if( (atr2 < 64 || atr3 < 64) && atr0 < 3 ) {
        if( atr2 < 64 ) {
          //Вариант 2
          os.WriteBinU8( atr2 | 0x80 );
          }
        else {
          //Вариант 3
          os.WriteBinU8( atr3 | 0xc0 );
          }
        i++;
        }
      else {
        //Вариант 0
        os.WriteBinU8( atr0 );
        os.Write( &color, 3 );
        i += atr0+1;
        }
      }

    }
  return true;
  }
