//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

void
CmtI2c::SetupMasterAddr(uint8 adr) {
  //Нужно выпобнить последовательность:
  // старт
  // отправка адреса
  // получение подтверждения
  
  }

void
CmtI2c::SetupSlaveAddr(uint8 index, uint8 addr) {
  if( index ) mPort->OAR2 = addr << 1;
  else {
    mPort->OARL = addr << 1;
    mPort->OARH = MBIT6;
    }
  }


#ifdef CMT_I2C_INT_TX_BYTE
extern "C" {
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler, 29) {
  uint8 v;
  /* Read SR2 register to get I2C error */
  if (I2C_ReadRegister(I2C1, I2C_Register_SR2))
  {
    /* Clears SR2 register */
    I2C1->SR2 = 0;

  }
  switch ( I2C_GetLastEvent(I2C1) )
  {
      /******* Slave transmitter ******/
      /* check on EV1 */
    case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:
//      Tx_Idx = 0;
      break;

      /* check on EV3 */
    case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:
      /* Transmit data */
      CMT_I2C_INT_TX_BYTE( &v );
      I2C_SendData(I2C1, v);
      break;
      /******* Slave receiver **********/
      /* check on EV1*/
    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
      break;

      /* Check on EV2*/
    case I2C_EVENT_SLAVE_BYTE_RECEIVED:
//      Slave_Buffer_Rx[Rx_Idx++] = I2C_ReceiveData(I2C1);
      break;

      /* Check on EV4 */
    case (I2C_EVENT_SLAVE_STOP_DETECTED):
            /* write to CR2 to clear STOPF flag */
            I2C1->CR2 |= I2C_CR2_ACK;
      break;

    default:
      break;
  }
}
  
}
#endif

uint_8
CmtI2cSlave::ReadBlock(pvoid dest, uint32 count, CmtEvent *pBreak) {
  uint8 *ptr = (uint8*)dest;
  //Очистить флаги
  GetByte();
  uint8 tmp = mPort->SR1;
  mPort->SR2 = 0;
  while( count-- ) {
    //Ожидаем получения байта, завершения транзакции или прекращения операции
    if( (cmtWaitSetBit_8( (uint8*)&(mPort->SR1), MBIT6 | MBIT4 ) & MBIT6) == 0 ) {
      //Обнаружено прекращение операции
      mPort->CR2 = mPort->CR2 & 0xff;
      return CMTE_FAIL;
      }
    *ptr++ = GetByte();
    }
  //Ожидать установки флага завершения операции
  cmtWaitSetBit_8( (uint8*)&(mPort->SR1), MBIT4 );
  //Записать в CR2 для очистки флага
  mPort->CR2 = mPort->CR2 & 0xff;
  return CMTE_OK;
  }

uint_8
CmtI2cSlave::WriteBlock(cpvoid src, uint32 count, CmtEvent *pBreak) {
#ifdef CMT_I2C_INT_TX_BYTE
  //Инициализировать передачу
  mTrans.Setup( (const uint8*)src, count );
  uint8 tmp = mPort->SR1;
  mPort->SR2 = 0;
  GPIOD->ODR &= ~(MBIT0 | MBIT1);
  //Разрешить прерывания
  
  
#else
  const uint8 *ptr = (const uint8*)src;
  uint8 tmp = mPort->SR1;
  mPort->SR2 = 0;
  tmp = mPort->SR3;
  CmtEvSetBit8 ev( (uint8*)(&(mPort->SR1)), MBIT7 | MBIT4 | MBIT2 | MBIT1 );
  posit = 3;
  while( count-- ) {
    //Ожидаем завершения передачи байта, завершения транзакции или прекращения операции
    ev.Wait2( pBreak );
    if( pBreak && pBreak->IsSignaled() )
      return CMTE_BREAK;
    posit = 7;
//    while( (mPort->SR1 & (MBIT7 | MBIT4 | MBIT2)) == 0 );
    if( (mPort->SR1 & (MBIT7 | MBIT2)) == 0 ) {
    //if( (cmtWaitSetBit_8( (uint8*)&(mPort->SR1), MBIT7 | MBIT4 | MBIT2) & (MBIT7 | MBIT2)) == 0 ) {
      //Обнаружено прекращение операции
      return CMTE_FAIL;
      }
    PutByte( *ptr++ );
    }
  return CMTE_OK;
#endif
  }

uint_8
CmtI2cSlave::WaitQuery() {
  //Ожидать совпадения адреса
  cmtWaitSetBit_8( (uint8*)&(mPort->SR1), MBIT1 );
  return mPort->SR3 & MBIT2 ? 1 : 0;
  }
