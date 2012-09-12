//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

CmtAmis30522::CmtAmis30522( CmtSpiPipeBase *spi ) :
  //CmtDev(),
  crwd(0),
  cr0(0),
  cr1(0),
  cr2(0),
  mSpi(spi) {
  
  }

uint8 
CmtAmis30522::Cmd( uint8 cmd, uint8 val ) {
  mSpi->Select();
  mSpi->Transfer( cmd, &cmd, 0 );
  val =  mSpi->Transfer( val, &val, 0 );
  mSpi->UnSelect();
  return val;
  }


void
CmtAmis30522::Setup() {
  //Записываем все регистры через SPI
  Cmd( 0x80 | 0, crwd );
  Cmd( 0x80 | 1, cr0 );
  Cmd( 0x80 | 2, cr1 );
  Cmd( 0x80 | 3, cr2 );
  }
  
void
CmtAmis30522::SetRunCurrent() {
  SetCurrent( mRunCurrent );
  }

void
CmtAmis30522::SetHoldCurrent() {
  SetCurrent( mHoldCurrent );
  }
  
void
CmtAmis30522::SetCurrent( uint8 current ) {
  if( (cr0 & 0x1f) < current ) {
    //С низкого на высокий идем сразу
    cr0 &= 0xe0;
    cr0 |= current;
    Cmd( 0x80 + 1, cr0 );
    }
  else {
    //С высокого на низкий идем поэтапно
    if( current < 23 ) {
      cr0 &= 0xe0;
      cr0 |= 23;
      Cmd( 0x80 + 1, cr0 );
      cmtWaitTick( 12 );
      }
    else {
      cr0 &= 0xe0;
      cr0 |= current;
      Cmd( 0x80 + 1, cr0 );
      cmtWaitTick( 12 );
      return;
      }
    if( current < 16 ) {
      cr0 &= 0xe0;
      cr0 |= 16;
      Cmd( 0x80 + 1, cr0 );
      cmtWaitTick( 8 );
      }
    else {
      cr0 &= 0xe0;
      cr0 |= current;
      Cmd( 0x80 + 1, cr0 );
      cmtWaitTick( 8 );
      return;
      }
    if( current < 9 ) {
      cr0 &= 0xe0;
      cr0 |= 9;
      Cmd( 0x80 + 1, cr0 );
      cmtWaitTick( 6 );
      }
    else {
      cr0 &= 0xe0;
      cr0 |= current;
      Cmd( 0x80 + 1, cr0 );
      cmtWaitTick( 6 );
      return;
      }
    cr0 &= 0xe0;
    cr0 |= current;
    Cmd( 0x80 + 1, cr0 );
    cmtWaitTick( 4 );
    }
  }

uint8
CmtAmis30522::RdCrwd() {
  return Cmd( 0, 0 );
  }

uint8
CmtAmis30522::RdCr0() {
  return Cmd( 1, 0 );
  }

uint8
CmtAmis30522::RdCr1() {
  return Cmd( 2, 0 );
  }

uint8
CmtAmis30522::RdCr2() {
  return Cmd( 3, 0 );
  }

uint8
CmtAmis30522::Sr0() {
  return Cmd( 4, 0 );
  }

uint8
CmtAmis30522::Sr1() {
  return Cmd( 5, 0 );
  }

uint8
CmtAmis30522::Sr2() {
  return Cmd( 6, 0 );
  }

uint8
CmtAmis30522::Sr3() {
  return Cmd( 7, 0 );
  }
