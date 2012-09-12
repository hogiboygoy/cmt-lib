//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
struct CmtUsart : public CmtSerial {
  USART_TypeDef     *mUart;
  USART_InitTypeDef  USART_InitStructure;
  uint8             *mDest;
  uint32             mReceivCount;
  const uint8       *mSrc;
  uint32             mTransmitCount;
  
  CmtUsart( USART_TypeDef *uart );
  
  //! ��������� �����
  /*! ������������� ����� ��������� ������
    \param param ��������� �� ��������� ���������� #CmtSerialParam */
  virtual void Setup( CmtSerialParam *param );

  //! ����������/���������� ������� ����������������� ������
  /*! ��������� ��� ��������� ������ ����������������� ������
    \param module ���� ��� ��������� ��������� ������ */
  virtual void EnableDisable( uint_8 module );
  
  //! �������� ��������� �����
  /*! ���������� ��������� �����.
    \return ����, ���� ��� ��������� � ��� ������ � ��������� ������ */
  virtual uint_8 GetState() { return CMTE_OK; }
  
  //! �������� �������� ����.
  /*! ���������� ���� �� ��������� ������. 
      \return ���� �� �������� ������ */  
  virtual uint8 GetByte() { return (uint8)USART_ReceiveData(mUart); }
  
  //! ������� �������� �����
  /*! ������� ������� ���������� ����� � ������ ����-���� 
    \param timeOut �������� ��������
    \return ����, ���� ���� ������ � ��� ������ � ��������� ������ */
  virtual uint_8 WaitReceivByte( uint32 timeOut );
  
  //! ���������� ���� � ����� �����������
  /*! �������� ������������ ���� � ������ �����������.
    \param byte ���� ��� �������� */
  virtual void PutByte( uint8 byte ) { USART_SendData(mUart,byte); }
  
  //! ������� �������� �����
  /*! ������� ���������� �������� ���������� ����� � ������ ����-����.
    \param timeOut �������� ��������
    \return ����, ���� ���� ������ � ��� ������ � ��������� ������ */
  virtual uint_8 WaitTransmitByte( uint32 timeOut );
  
  //! �������� ���� ������
  /*! ��������� ������� �� ����� ����� ������ � ��������� �����.
    \param dest - ����� ��� �������� ������
    \param count - ���������� ����������� ������
    \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
  virtual uint_8 GetBlock( pvoid dest, uint32 count );
  
  //! ������� ���������� ������ �����
  /*! ������� ������� ����� ������ � ������ ����-���� 
    \param timeOut �������� ��������
    \return ����, ���� ���� ������ � ��� ������ � ��������� ������ */
  virtual uint_8 WaitReceivBlock( uint32 timeOut );
  
  //! �������� ���� ������
  /*! ��������� ������� �� �������� ����� ������ �� ���������� ������.
    \param src - ����� ������ ��� ��������
    \param count - ���������� ������������ ������
    \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
  virtual uint_8 PutBlock( cpvoid src, uint32 count );
  
  //! ������� ���������� �������� �����
  /*! ������� �������� ����� ������ � ������ ����-���� 
    \param timeOut �������� ��������
    \return ����, ���� ���� ������� � ��� ������ � ��������� ������ */
  virtual uint_8 WaitTransmitBlock( uint32 timeOut );
  
  void Handler();
  };

CmtUsart::CmtUsart(USART_TypeDef *uart) :
  CmtSerial(),
  mUart(uart),
  mReceivCount(0),
  mTransmitCount(0) 
{
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(mUart, &USART_InitStructure);

  USART_Cmd(mUart, ENABLE);
  }

struct CmtUsartSignal {
  uint32         mTimeOut;
  USART_TypeDef *mUart;
  };

uint_8 cmtUsartRxByteSignal( void *ptr ) {
  CmtUsartSignal *signal = (CmtUsartSignal*)ptr;
  return USART_GetFlagStatus( signal->mUart, USART_FLAG_RXNE ) != RESET || cmtCheckTick(signal->mTimeOut);
  }

uint_8 cmtUsartTxByteSignal( void *ptr ) {
  CmtUsartSignal *signal = (CmtUsartSignal*)ptr;
  return USART_GetFlagStatus( signal->mUart, USART_FLAG_TXE ) != RESET || cmtCheckTick(signal->mTimeOut);
  }

uint_8
CmtUsart::WaitReceivByte( uint32 timeOut ) {
  CmtUsartSignal signal;
  cmtSetCheckTick( signal.mTimeOut, timeOut );
  signal.mUart = mUart;
  cmtWaitEvent( cmtUsartRxByteSignal, &signal );
  return cmtCheckTick(signal.mTimeOut) ? CMTE_TIME_OUT : CMTE_OK;
  }

uint_8
CmtUsart::WaitTransmitByte( uint32 timeOut ) {
  CmtUsartSignal signal;
  cmtSetCheckTick( signal.mTimeOut, timeOut );
  signal.mUart = mUart;
  cmtWaitEvent( cmtUsartTxByteSignal, &signal );
  return cmtCheckTick(signal.mTimeOut) ? CMTE_TIME_OUT : CMTE_OK;
  }

uint_8
CmtUsart::WaitReceivBlock( uint32 timeOut ) {
  return cmtWaitCountOrTick( &mReceivCount, timeOut );
  }

uint_8
CmtUsart::WaitTransmitBlock( uint32 timeOut ) {
  return cmtWaitCountOrTick( &mTransmitCount, timeOut );
  }

void
CmtUsart::EnableDisable(uint_8 module) {
  //disable
  if( module & CMT_SERIAL_RX_DIS ) USART_ITConfig(mUart, USART_IT_RXNE, DISABLE);
  if( module & CMT_SERIAL_TX_DIS ) USART_ITConfig(mUart, USART_IT_TXE, DISABLE);
  //enable
  if( module & CMT_SERIAL_RX_EN ) USART_ITConfig(mUart, USART_IT_RXNE, ENABLE);
  if( module & CMT_SERIAL_TX_EN ) USART_ITConfig(mUart, USART_IT_TXE, ENABLE);
  }

void
CmtUsart::Setup(CmtSerialParam *param) {
  //������� �������
  USART_InitStructure.USART_BaudRate = param->mBaudRate;
  if( param->mLevel >= 1 ) {
    CmtSerialParamLevel1 *lev1 = (CmtSerialParamLevel1*)param;
    if( lev1->mWordLength == 9 || lev1->mParity ) USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    else                         USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    switch( lev1->mStopBits ) {
      case 1 : USART_InitStructure.USART_StopBits = USART_StopBits_0_5; break;
      case 2 : USART_InitStructure.USART_StopBits = USART_StopBits_1; break;
      case 3 : USART_InitStructure.USART_StopBits = USART_StopBits_1_5; break;
      default : USART_InitStructure.USART_StopBits = USART_StopBits_2; break;
      }
    switch( lev1->mParity ) {
      case 1 : USART_InitStructure.USART_Parity = USART_Parity_Odd; break;
      case 2 : USART_InitStructure.USART_Parity = USART_Parity_Even; break;
      default : USART_InitStructure.USART_Parity = USART_Parity_No;
      }
    }
  //USART_Cmd(mUart, DISABLE);
  USART_Init(mUart, &USART_InitStructure);
  //USART_Cmd(mUart, ENABLE);
  }

uint_8
CmtUsart::GetBlock(pvoid dest, uint32 count) {
  mDest = (uint8*)dest;
  mReceivCount = count;
  return CMTE_OK;
  }

uint_8
CmtUsart::PutBlock(cpvoid src, uint32 count) {
  mSrc = (uint8*)src;
  mTransmitCount = count;
  USART_SendData( mUart, *mSrc++ );
  USART_ITConfig(mUart, USART_IT_TXE, ENABLE);
  return CMTE_OK;
  }

void
CmtUsart::Handler() {
  if(USART_GetITStatus(mUart, USART_IT_RXNE) != RESET) {
    //��������� ����� ����� ������ ��� ���������� ������
    if( (mUart->SR & 2) == 0 ) {
      if( mReceivCount ) {
        *mDest++ = (uint8)USART_ReceiveData(mUart);
        mReceivCount--;
        if( mReceivCount == 0 && mSensReceivBlock )
          mSensReceivBlock->Complete( this );
        }
      else {
        //������ ��������� ����, ������� ������
        if( mSensReceivByte )
          mSensReceivByte->Complete( this );
        }
      }
    else
      USART_ReceiveData(mUart); //��������� ������ ����� ��� ������ �������� ������
    }
  
  if(USART_GetITStatus( mUart, USART_IT_TXE) != RESET) {
    if( mTransmitCount ) {
      mTransmitCount--;
      if( mTransmitCount == 0 ) {
        USART_ITConfig(mUart, USART_IT_TXE, DISABLE);
        if( mSensTransmitBlock ) mSensTransmitBlock->Complete( this );
        }
      else USART_SendData( mUart, *mSrc++ );
      }
    else {
      //��������� ���� �������, ������� ������
      if( mSensTransmitByte )
        mSensTransmitByte->Complete( this );
      }
    }  
  
  }

#ifdef CMT_SI_UART4

#ifdef __cplusplus
extern "C" {
#endif

void UART4_IRQHandler() {
  ((CmtUsart*)(CmtSerial::cmtSerials[CMT_SI_UART4]))->Handler();
  }

#ifdef __cplusplus
};
#endif

#endif

#ifdef CMT_SI_UART3

#ifdef __cplusplus
extern "C" {
#endif

void USART3_IRQHandler() {
  ((CmtUsart*)(CmtSerial::cmtSerials[CMT_SI_UART3]))->Handler();
  }

#ifdef __cplusplus
};
#endif
#endif

#ifdef CMT_SI_UART2

#ifdef __cplusplus
extern "C" {
#endif

void USART2_IRQHandler() {
  ((CmtUsart*)(CmtSerial::cmtSerials[CMT_SI_UART2]))->Handler();
  }

#ifdef __cplusplus
};
#endif
#endif


void
CmtInitUsart() {
  NVIC_InitTypeDef NVIC_InitStructure;
  
#ifdef CMT_SI_UART4
  CmtSerial::cmtSerials[CMT_SI_UART4] = new CmtUsart( UART4 );
  
  //����������
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
#endif

#ifdef CMT_SI_UART3
  CmtSerial::cmtSerials[CMT_SI_UART3] = new CmtUsart( USART3 );
  
  //����������
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
#endif

#ifdef CMT_SI_UART2
  CmtSerial::cmtSerials[CMT_SI_UART2] = new CmtUsart( USART2 );
  
  //����������
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
#endif
  }
