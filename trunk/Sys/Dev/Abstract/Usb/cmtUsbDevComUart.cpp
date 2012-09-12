//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//
//==============================================================================
/* �������� ���� ����������� � ���������: �� ���������� ������ � USB (�������� �� USB � UART)
  ������ �� ������ USB �������������� � ������������� ����� mTxBuf � ����������� �������� ����� ��
  UART (�������� ����� �������������� �� ����������� ��� �� DMA). ��� ���� ����� ������� �� USB �����������
  �� ���������� ��������. �� ���������� �������� ������ ��������� ����� �������.
  
  ����� ������ �� UART �������������� ��������� �������: ��� ������ ������� ����� ������ ���������� ������ �����
  �������� ������� RxByte ����� USB, ������� ������� �������� ���� � ����� ������. ���� � ������ ������ ��� �����,
  ���� �� ��������� � ��������. ��� ��������� ������� �� ����� �� USB �������� � ����� ������ ������������ �� USB, 
  ������� �������� ������ ���������� � ���� �����������.
  
  ����� �������, ���������������� ������� ��������� �������������� �� ����������� � �� ������� ��������������� ����������������.
  */

//! �������� ������, ��������� �� ����� �����
struct CmtUsbDevComUartRxSensor : public CmtSerialSensor {
  CmtUsbDevComUart *mUsb; //!< ���������� USB ��� ������������ ���������� ������ �����
  
  CmtUsbDevComUartRxSensor( CmtUsbDevComUart *usb ) : CmtSerialSensor(), mUsb(usb) {}
  
  //! ���������� ��� ���������� ��������
  virtual void Complete( CmtSerial *serial );
  };

void
CmtUsbDevComUartRxSensor::Complete(CmtSerial *serial) {
  mUsb->RxByte( serial->GetByte() );
  }

//! ������ �����������, ��������� �� ���������� �������� �����
struct CmtUsbDevComUartTxSensor : public CmtSerialSensor {
  CmtUsbDevComUart *mUsb; //!< ���������� USB ��� ������������ ���������� ��������
  
  CmtUsbDevComUartTxSensor( CmtUsbDevComUart *usb ) : CmtSerialSensor(), mUsb(usb) {}
  
  //! ���������� ��� ���������� ��������
  virtual void Complete( CmtSerial *serial );
  };


CmtUsbDevComUart::CmtUsbDevComUart( CmtSerial *serial ) :
  CmtUsbDevCom(), 
  mRxCount(0),
  mSerial( serial ),
  mTxFlag(0),
  mFrameCount(0)
    {
  
  //���������� �������
  mSerial->mSensReceivByte = new CmtUsbDevComUartRxSensor( this );
  mSerial->mSensTransmitBlock = new CmtUsbDevComUartTxSensor( this );
  
  //��������� ������������ ���������� �����
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
  //�� ������ ��������� ���������� � ����� USB
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
    //�������� �����
    mSerial->PutBlock( mTxBufA, txCount );
    mTxFlag = 1;
    //��������� ����� ������� ���� �� ���������� ��������
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
    //������ ��������� ������
    mUsb->USB_ResetNAK( 0x03 );
    mUsb->mTxFlag = 0;
    }
  }

  
void   
CmtUsbDevComUart::SetupComParam() {
  //���������� ����� ��������� ����� � ������������ � ���������
  CmtSerialParamLevel1 param( mLineCoding.dwDTERate, 8, mLineCoding.bCharFormat + 2, mLineCoding.bParityType );
  mSerial->Setup( &param );
  }
