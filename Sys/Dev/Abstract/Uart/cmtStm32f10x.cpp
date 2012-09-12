//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

CmtUart::CmtUart( USART_TypeDef *uart, uint32 baudRate, uint16 parity, uint16 stopBits, CmtTransfer *rx, CmtTransfer *tx ) :
  CmtUartBase( rx, tx ),
  mUart(uart)
{
  USART_InitStructure.USART_BaudRate = baudRate; //9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = stopBits; //USART_StopBits_1;
  USART_InitStructure.USART_Parity = parity; //USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(mUart, &USART_InitStructure);

  USART_Cmd(mUart, ENABLE);
  }

void
CmtUart::SetBaudRate(uint32 baudRate) {
  if( USART_InitStructure.USART_BaudRate != baudRate ) {
    USART_InitStructure.USART_BaudRate = baudRate;
    USART_Init(mUart, &USART_InitStructure);
    }
  }

uint_8
CmtUart::ReadBlock(pvoid dest, uint32 count, CmtEvent *pBreak) {
  if( count == 0 || dest == 0 ) return CMTE_BAD_PARAM;
  uint8 *dst = (uint8*)dest;
  if( count == 1 || mRxTransfer == 0 ) {
    //Одиночное чтение байта
    while( count-- ) {
      //Проверить готовность байта
      if( (mUart->SR & MBIT5) == 0 )
        //Ожидать установки бита
        CmtEvSetBit16( (uint16*)(&(mUart->SR)), MBIT5 ).Wait2( pBreak );
      if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;
      *dst++ = mUart->DR;
      }
    }
  else {
    //Множественное чтение байтов
    mRxTransfer->InitTransfer( dest, count, 1 );
    mRxTransfer->WaitTransfer( pBreak );
    }
  return pBreak && pBreak->IsSignaled() ? CMTE_BREAK : CMTE_OK;
  }

uint_8
CmtUart::WriteBlock(cpvoid src, uint32 count, CmtEvent *pBreak) {
  if( count == 0 || src == 0 ) return CMTE_BAD_PARAM;
  const uint8 *sour = (const uint8*)src;
  if( count == 1 || mTxTransfer == 0 ) {
    //Одиночная запись байта
    while( count-- ) {
      //Проверить наличие свободного места в передатчике
      if( (mUart->SR & MBIT7) == 0 )
        //Ожидать установки бита
        CmtEvSetBit16( (uint16*)(&(mUart->SR)), MBIT7 ).Wait2( pBreak );
      mUart->DR = *sour++;
      if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;
      }
    }
  else {
    //Множественная запись байтов
    mTxTransfer->InitTransfer( (void*)src, count, 1 );
    mTxTransfer->WaitTransfer( pBreak );
    }
  return pBreak && pBreak->IsSignaled() ? CMTE_BREAK : CMTE_OK;
  }
