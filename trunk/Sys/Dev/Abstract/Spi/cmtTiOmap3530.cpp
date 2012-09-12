//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

CmtSpiPipeHard::CmtSpiPipeHard( uint32 base, uint32 cs, CmtTransfer *rx, CmtTransfer *tx ) :
  CmtSpiPipeBase( rx, tx ),
  mBase(base),
  mCs(cs) {
    }

void
CmtSpiPipeHard::Select() {
  __raw_clearl( 1 << mCs, mBase + MCSPI_SYST );
  }

void
CmtSpiPipeHard::UnSelect() {
  __raw_setl( 1 << mCs, mBase + MCSPI_SYST );
  }

uint8 spiPipeDummi;

uint_8
CmtSpiPipeHard::Transfer(cpvoid src, pvoid dst, uint32 count, CmtEvent *pBreak) {
  if( (src == 0 && dst == 0) || count == 0 ) return CMTE_BAD_PARAM;
  if( count > 1 && ((src && dst && mRxTransfer && mTxTransfer) || (dst == 0 && src && mTxTransfer)) ) {
    //Возможна пакетная операция
    if( dst == 0 ) {

      //Для того, чтобы не мешался приемник, его нужно отключить
      __raw_setl( MBIT13, mBase + MCSPI_CHxCONF(mCs) );

      //Ожидать готовности передатчика
      while( (__raw_readl( mBase + MCSPI_CHxSTAT(mCs) ) & MBIT1) == 0 );

      //Только передача
      mTxTransfer->InitTransfer( (void*)src, count, 1 );
      //Ожидать завершения передачи
      mTxTransfer->WaitTransfer( pBreak );
      if( pBreak && pBreak->IsSignaled() ) {
        __raw_clearl( MBIT13, mBase + MCSPI_CHxCONF(mCs) );
        return CMTE_BREAK;
        }

      //Ожидать завершения передачи
      while( (__raw_readl( mBase + MCSPI_CHxSTAT(mCs) ) & MBIT1) == 0 );
      //Снова разрешить работу приемника
      __raw_clearl( MBIT13, mBase + MCSPI_CHxCONF(mCs) );
      }
    else {
      //Прием и передача

//      dindex = 8;
      //Ожидать готовности передатчика
      while( (__raw_readl( mBase + MCSPI_CHxSTAT(mCs) ) & MBIT1) == 0 );

//      dindex = 9;
      //Фиктивное чтение для снятия флага готовности
      __raw_readl( mBase + MCSPI_RXx(mCs) );
      //Подготовить и начать прием
      mRxTransfer->InitTransfer( dst, count, 1 );
      //Подготовить и начать передачу
//      dindex = 10;
      mTxTransfer->InitTransfer( (void*)src, count, 1 );
      //Ожидать завершения приема
//      dindex = 11;
      mRxTransfer->WaitTransfer( pBreak );
      //Передача приемника прекратится автоматически, а передачу передатчика нужно прекратить явно
//      dindex = 12;
//      mTxTransfer->StopTransfer();
//      mRxTransfer->StopTransfer();
//      dindex = 13;
      if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;

      }
    }
  else {
    const uint8 *sour = (const uint8*)src;
    uint8* dest = (uint8*)dst;
//    dindex = 14;
    //Пакетная операция не возможна
    while( count-- ) {
      //Ожидать готовности передатчика
      if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;

      //Сбросить флаг приема
      __raw_writel( MBIT2 << (mCs * 4), mBase + MCSPI_IRQSTATUS );

      //Начать передачу данных
      if( sour ) __raw_writel( *sour++, mBase + MCSPI_TXx(mCs) );
      else       __raw_writel( 0xff, mBase + MCSPI_TXx(mCs) );

      //Ожидать готовности приемника
      CmtEvSetBit32( (uint32*)(mBase + MCSPI_IRQSTATUS),  MBIT2 << (mCs * 4) ).Wait2( pBreak );
      if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;

      //Принять данные
      if( dest ) *dest++ = __raw_readl( mBase + MCSPI_RXx(mCs) );
      else       spiPipeDummi = __raw_readl( mBase + MCSPI_RXx(mCs) );
      }
    }
  return CMTE_OK;
  }

CmtSpiPipeSoft::CmtSpiPipeSoft( CmtSpiPipeHard *pipe, uint32 pin ) :
  CmtSpiPipeLinked( pipe ), 
  mCsPin(pin) {
    //Установить в 1
    GPIO_PinSet( pin );
  }

void 
CmtSpiPipeSoft::Select() {
  GPIO_PinReset( mCsPin );
  }

void 
CmtSpiPipeSoft::UnSelect() {
  GPIO_PinSet( mCsPin );
  }
