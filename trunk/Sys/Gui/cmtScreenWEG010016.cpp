//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

void
CmtWEG010016::Init() {
  
  //FunctionSet
  //WriteC( 0x3e ); //Разрядность дисплея
  WriteC( 0x38 );

  //Проверить флаг занятости
  while( ReadC() & 0x80 );
    //cmtWaitTick(1);
  
  //Display On
  WriteC( 0x0c );

  //Проверить флаг занятости
  while( ReadC() & 0x80 );
  
  //Display Clear
  WriteC( 0x01 );

  //Проверить флаг занятости
  while( ReadC() & 0x80 );
  
  WriteC( 0x06 );
  
  //Перейти в графический режим
  WriteC( 0x1f );
  }

//DDRAM-X
//CGRAM-Y
void
CmtWEG010016::Clear( uint32 color ) {
  if( color == 0xffffffff ) color = 0;
  else                      color = 0xff;
  //Для двух строк
  int i;
  //Записать Y=0
  WriteC( 0x40 );
  WriteC( 0x80 );
  for( i = 0; i < 100; i++ )
    WriteD( color );
  //Записать Y=1
  WriteC( 0x41 );
  WriteC( 0x80 );
  for( i = 0; i < 100; i++ )
    WriteD( color );
  }

void
CmtWEG010016::SetMode( int mode ) {
  if( mode == CMT_WEG010016_ON ) {
    //Display On
    WriteC( 0x0c );
    }
  if( mode == CMT_WEG010016_OFF ) {
    //Display Off
    WriteC( 0x08 );
    }
  }

void
CmtWEG010016::Copy( CmtContext *src, CmtPoint pos, CmtPoint size, CmtPoint exPos, CmtPoint exSize ) {
  }

void
CmtWEG010016::CopyFull( CmtContext *src ) {
  //Копировать полностью
  //Записать Y=0
  WriteC( 0x40 );
  WriteC( 0x80 );
  uint32 *ptr, x, color, y;
  int line = src->GetSize().x;
  uint8 mask;
  for( x = 0; x < 100; x++ ) {
    color = 0;
    ptr = src->GetData( CmtPoint(x,0) );
    mask = 1;
    for( y = 0; y < 8; y++ ) {
      if( *ptr != 0xffffffff ) color |= mask;
      mask <<= 1;
      ptr += line;
      }
    WriteD( color );
    }
    
  WriteC( 0x41 );
  WriteC( 0x80 );
  for( x = 0; x < 100; x++ ) {
    color = 0;
    ptr = src->GetData( CmtPoint(x,8) );
    mask = 1;
    for( y = 0; y < 8; y++ ) {
      if( *ptr != 0xffffffff ) color |= mask;
      mask <<= 1;
      ptr += line;
      }
    WriteD( color );
    }
  }

