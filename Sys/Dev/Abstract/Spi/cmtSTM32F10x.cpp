//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

uint_8
CmtSpi::Transfer(cpvoid src, pvoid dst, uint32 count, CmtEvent *pBreak) {
  if( (src == 0 && dst == 0) || count == 0 ) return CMTE_BAD_PARAM;
  if( count > 1 && ((src && dst && mRxTransfer && mTxTransfer) || (dst == 0 && src && mTxTransfer)) ) {
    //Возможна пакетная операция
    if( dst == 0 ) {
      //Ожидать готовности передатчика
      while( SPI_I2S_GetFlagStatus( mSpi, SPI_I2S_FLAG_TXE ) == RESET );

      //Только передача
      mTxTransfer->InitTransfer( (void*)src, count, 1 );
      //Ожидать завершения передачи
      mTxTransfer->WaitTransfer( pBreak );
      if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;

      //Ожидать завершения передачи
      while( SPI_I2S_GetFlagStatus( mSpi, SPI_I2S_FLAG_BSY ) == SET );
      }
    else {
      //Прием и передача

      //Ожидать готовности передатчика
      if( SPI_I2S_GetFlagStatus( mSpi, SPI_I2S_FLAG_TXE ) == RESET )
        CmtEvSetBit16( (uint16*)(&(mSpi->SR)), MBIT1 ).Wait2( pBreak );

      if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;

      //Фиктивное чтение для снятия флага готовности
      SPI_I2S_ReceiveData(mSpi);
      //Подготовить и начать прием
      mRxTransfer->InitTransfer( dst, count, 1 );
      //Подготовить и начать передачу
      mTxTransfer->InitTransfer( (void*)src, count, 1 );
      //Ожидать завершения приема
      mRxTransfer->WaitTransfer( pBreak );
      //Передача приемника прекратится автоматически, а передачу передатчика нужно прекратить явно
      mTxTransfer->StopTransfer();
      if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;

      }
    }
  else {
    const uint8 *sour = (const uint8*)src;
    uint8* dest = (uint8*)dst;
    //Пакетная операция не возможна
    while( count-- ) {
      //Ожидать готовности передатчика
      if( SPI_I2S_GetFlagStatus( mSpi, SPI_I2S_FLAG_TXE ) == RESET )
        CmtEvSetBit16( (uint16*)(&(mSpi->SR)), MBIT1 ).Wait2( pBreak );

      if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;

      //Фиктивное чтение для снятия флага готовности
      SPI_I2S_ReceiveData(mSpi);

      //Начать передачу данных
      if( sour ) SPI_I2S_SendData( mSpi, *sour++ );
      else       SPI_I2S_SendData( mSpi, 0xff );

      //Ожидать готовности приемника
      CmtEvSetBit16( (uint16*)(&(mSpi->SR)), MBIT0 ).Wait2( pBreak );
      if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;

      //Принять данные
      if( dest ) *dest++ = SPI_I2S_ReceiveData(mSpi);
      else       SPI_I2S_ReceiveData(mSpi);
      }
    }
  return CMTE_OK;
  }


CmtSpiPipe::CmtSpiPipe( CmtSpiBase *spi, GPIO_TypeDef *gpio, uint32 pin ) :
  CmtSpiPipeLinked( spi ), mGpio(gpio), mPin(pin) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( gpio, &GPIO_InitStructure);
  
    GPIO_SetBits( gpio, pin );
    }
    
