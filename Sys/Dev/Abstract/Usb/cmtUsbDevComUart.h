//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#define CMT_UDCU_MAX_RX_BUF 64 //������ ��������� ������
#define CMT_UDCU_MAX_TX_BUF 64 //������ ����������� ������

//! ���� USB ����������� ���� � ���������� ���������������� ���� ����������
struct CmtUsbDevComUart : public CmtUsbDevCom {
  CmtSerial *mSerial;                       //!< ���������� ���������������� �����
  uint8      mTxBufA[CMT_UDCU_MAX_TX_BUF];  //!< ����� �����������
  uint8      mTxBufB[CMT_UDCU_MAX_TX_BUF];  //!< ����� �����������
  uint_8     mTxFlag;                       //!< 0-����� A ����, 1-����� B ����, 2 ��� ������ ���������
  uint32     mTxCountB;                     //!< ���������� ������ � ������ B
  uint32     mRxCount;                      //!< ������� �������� ������
  uint8      mRxBuf[CMT_UDCU_MAX_RX_BUF];   //!< �������� �����
  uint32     mFrameCount;                   //!< ������� �������
  
  CmtUsbDevComUart( CmtSerial *serial );
  
  virtual void   SetupComParam(); 
  virtual void   EndPoint1( uint32 ev );
  virtual void   EndPoint3( uint32 ev );
  virtual void   USB_StartOfFrame();
  //virtual void   UartTransmit( uint_8 on ) {}
          void   RxByte( uint8 b );
  };

