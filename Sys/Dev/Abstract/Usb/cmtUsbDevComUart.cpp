//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//
//==============================================================================
/* Основная идея заключается в следующем: По готовности данных в USB (передача из USB в UART)
  данные из буфера USB переписываются в промежуточный буфер mTxBuf и запускается передача блока по
  UART (передача блока осуществляется по прерываниям или по DMA). При этом прием пакетов по USB блокируется
  до завершения передачи. По завершению передачи сенсор разрешает прием пакетов.
  
  Прием байтов из UART осуществляется следующим образом: при приеме каждого байта сенсор завершения приема байта
  вызывает функцию RxByte порта USB, которая заносит принятый байт в буфер приема. Если в буфере приема нет места,
  байт не заносится и теряется. При получении запроса на прием по USB принятые в буфер данные отправляются по USB, 
  счетчик принятых байтов обнуляется и цикл повторяется.
  
  Таким образом, функционирование системы полностью осуществляется по прерываниям и не требует дополнительного программирования.
  */

//! Приемный сенсор, реагирует на прием байта
struct CmtUsbDevComUartRxSensor : public CmtSerialSensor {
  CmtUsbDevComUart *mUsb; //!< Устройство USB для сигнализации завершения приема байта
  
  CmtUsbDevComUartRxSensor( CmtUsbDevComUart *usb ) : CmtSerialSensor(), mUsb(usb) {}
  
  //! Вызывается при завершении операции
  virtual void Complete( CmtSerial *serial );
  };

void
CmtUsbDevComUartRxSensor::Complete(CmtSerial *serial) {
  mUsb->RxByte( serial->GetByte() );
  }

//! Сенсор передатчика, реагирует на завершение передачи блока
struct CmtUsbDevComUartTxSensor : public CmtSerialSensor {
  CmtUsbDevComUart *mUsb; //!< Устройство USB для сигнализации завершения передачи
  
  CmtUsbDevComUartTxSensor( CmtUsbDevComUart *usb ) : CmtSerialSensor(), mUsb(usb) {}
  
  //! Вызывается при завершении операции
  virtual void Complete( CmtSerial *serial );
  };


CmtUsbDevComUart::CmtUsbDevComUart( CmtSerial *serial ) :
  CmtUsbDevCom(), 
  mRxCount(0),
  mSerial( serial ),
  mTxFlag(0),
  mFrameCount(0)
    {
  
  //Подключить сенсоры
  mSerial->mSensReceivByte = new CmtUsbDevComUartRxSensor( this );
  mSerial->mSensTransmitBlock = new CmtUsbDevComUartTxSensor( this );
  
  //Разрешить обслуживание прерываний порта
  mSerial->EnableDisable( CMT_SERIAL_RX_EN );
  }

void
CmtUsbDevComUart::RxByte( uint8 b ) {
  if( mRxCount < CMT_UDCU_MAX_RX_BUF ) {
    mRxBuf[mRxCount++] = b;
    }
  }

void
CmtUsbDevComUart::EndPoint1( uint32 ) { //IN
  //Из буфера приемника переписать в буфер USB
  if( mRxCount ) {
    USB_WriteEP8( 0x81, mRxBuf, mRxCount );
    mRxCount = 0;
    }
  }
  
void
CmtUsbDevComUart::USB_StartOfFrame() {
  CmtUsbDevCom::USB_StartOfFrame();
  mFrameCount++;
  if( mFrameCount > 5 ) {
    mFrameCount = 0;
    if( mRxCount ) {
      USB_WriteEP8( 0x81, mRxBuf, mRxCount );
      mRxCount = 0;
      }
    }
  }    

void
CmtUsbDevComUart::EndPoint3( uint32 ) { //OUT
  if( mTxFlag == 0 ) {
    uint32 txCount = USB_ReadEP8( 0x03, mTxBufA );
    //Передать пакет
    mSerial->PutBlock( mTxBufA, txCount );
    mTxFlag = 1;
    //Запретить прием пакетов пока не завершится передача
    USB_SetNAK( 0x03 );
    }
  else if( mTxFlag == 1 ) {
    mTxCountB = USB_ReadEP8( 0x03, mTxBufB );
    mTxFlag = 2;
    }
  }

void
CmtUsbDevComUartTxSensor::Complete(CmtSerial *serial) {
  if( mUsb->mTxFlag == 2 ) {
    serial->PutBlock( mUsb->mTxBufB, mUsb->mTxCountB );
    mUsb->mTxFlag = 1;
    }
  else {
    //Начать принимать пакеты
    mUsb->USB_ResetNAK( 0x03 );
    mUsb->mTxFlag = 0;
    }
  }

  
void   
CmtUsbDevComUart::SetupComParam() {
  //Установить новые параметры порта в соответствии с принятыми
  CmtSerialParamLevel1 param( mLineCoding.dwDTERate, 8, mLineCoding.bCharFormat + 2, mLineCoding.bParityType );
  mSerial->Setup( &param );
  }
