//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#define CMT_UDCU_MAX_RX_BUF 64 //Размер приемного буфера
#define CMT_UDCU_MAX_TX_BUF 64 //Размер передающего буфера

//! Мост USB виртуальный порт и физический последовательный порт процессора
struct CmtUsbDevComUart : public CmtUsbDevCom {
  CmtSerial *mSerial;                       //!< Физический последовательный канал
  uint8      mTxBufA[CMT_UDCU_MAX_TX_BUF];  //!< Буфер передатчика
  uint8      mTxBufB[CMT_UDCU_MAX_TX_BUF];  //!< Буфер передатчика
  uint_8     mTxFlag;                       //!< 0-Буфер A пуст, 1-Буфер B пуст, 2 оба буфера заполнены
  uint32     mTxCountB;                     //!< Количество байтов в буфере B
  uint32     mRxCount;                      //!< Счетчик принятых байтов
  uint8      mRxBuf[CMT_UDCU_MAX_RX_BUF];   //!< Приемный буфер
  uint32     mFrameCount;                   //!< Счетчик фрэймов
  
  CmtUsbDevComUart( CmtSerial *serial );
  
  virtual void   SetupComParam(); 
  virtual void   EndPoint1( uint32 ev );
  virtual void   EndPoint3( uint32 ev );
  virtual void   USB_StartOfFrame();
  //virtual void   UartTransmit( uint_8 on ) {}
          void   RxByte( uint8 b );
  };

