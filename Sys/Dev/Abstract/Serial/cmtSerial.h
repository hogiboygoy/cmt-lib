//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! ���������� � ���������������� ��������
/*! ������� ���������� � ���������������� ��������. ����� ������������ ����� ����������
  �� ��������� � ������� �� ������ � �� �����������. ���������� ����� ����������������
  ������������ ���������� ��������. */
struct CmtSerial : public CmtDev {
  CmtTransfer *mRxTransfer; //!< �������� (DMA ��� Interrupt)
  CmtTransfer *mTxTransfer; //!< ���������� (DMA ��� Interrupt)
  
  //! �����������
  CmtSerial( CmtTransfer *rx = 0, CmtTransfer *tx = 0 ) : CmtDev(), mRxTransfer(rx), mTxTransfer(tx) {}
  
  //! ��������� �����
  /*! ������������� ����� �������� ������
    \param param ��������� �� ��������� ���������� #CmtSerialParam */
  virtual void SetBaudRate( uint32 baudRate ) {}
  
  //! �������� ��������� �����
  /*! ���������� ��������� �����.
    \return ����, ���� ��� ��������� � ��� ������ � ��������� ������ */
  virtual uint_8 GetState() { return CMTE_OK; }
  
  //! �������� ���� ������
  /*! ��������� ������� �� ����� ����� ������ � ��������� �����.
    \param dest - ����� ��� �������� ������
    \param count - ���������� ����������� ������
    \param pBreak ������ ����������� ��������
    \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
  virtual uint_8 ReadBlock( pvoid dest, uint32 count, CmtEvent* pBreak ) = 0;
  
  //! �������� ���� ������
  /*! ��������� ������� �� �������� ����� ������ �� ���������� ������.
    \param src - ����� ������ ��� ��������
    \param count - ���������� ������������ ������
    \param pBreak ������ ����������� ��������
    \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
  virtual uint_8 WriteBlock( cpvoid src, uint32 count, CmtEvent* pBreak ) = 0;
  
};

//! ���������������� �������� � ����������� ����� (��������� ��� ����������)
struct CmtSerialReceiver : public CmtTransfer {
    uint8       *mDst;
    uint32       mDstCount;
    
    CmtSerialReceiver() : CmtTransfer(), mDst(0), mDstCount(0) {}
    
    void ReceivByte( uint8 val ) { if( mDstCount ) { *mDst++ = val; mDstCount--; } }

    void Setup( uint8 *dst, uint32 count ) { mDst = dst; mDstCount = count; }

    //! ������������ �����
    /*! ����������� ����� � ������������ � ��������� ����������� � ���������
      ������ ������-����������� (������-���������� ������ � ����� �������� �������)
      \param adr ����� ������ (�������� ��� ��������, � ����������� �� ������������ ������)
      \param elemCount ���������� ��������� �� ������
      \param frameCount ���������� ������� */
    virtual void   InitTransfer( void *adr, uint32 elemCount, uint32 frameCount ) = 0;

    //! ������� ���������� ������
    /*! ������� ���������� ������ � ������������ ����������� ��������.
      \param pBreak ��������� �� �������, �������������� ����������� ��������. �������� ������� ���������,
      ���� ����������� �������� �� ���������.
      \return ��� ������, ���������� ��������� ����������� �������� */
    virtual uint_8 WaitTransfer( CmtEvent *pBreak = 0 ) = 0;

    //! ����������� ������
    /*! ���������� ��������. ����� ����� ������ �������� ��������� ������������� ��������.
        *** � ������� ������ ��������� ���������� �� ��������� */
    virtual void   StopTransfer() = 0;

  };

//! ���������������� ���������� �� ������������ ������ (��������� ��� ����������)
struct CmtSerialTransmitter : public CmtTransfer {
    const uint8 *mSrc;
    uint32       mSrcCount;

    CmtSerialTransmitter() : CmtTransfer(), mSrc(0), mSrcCount(0) {}
    
    void TransmitByte( uint8 *val ) { if( mSrcCount ) { *val = *mSrc++; mSrcCount--; } }
    
    void Setup( const uint8 *src, uint32 count ) { mSrc = src; mSrcCount = count; }

    //! ������������ �����
    /*! ����������� ����� � ������������ � ��������� ����������� � ���������
      ������ ������-����������� (������-���������� ������ � ����� �������� �������)
      \param adr ����� ������ (�������� ��� ��������, � ����������� �� ������������ ������)
      \param elemCount ���������� ��������� �� ������
      \param frameCount ���������� ������� */
    virtual void   InitTransfer( void *adr, uint32 elemCount, uint32 frameCount ) = 0;

    //! ������� ���������� ������
    /*! ������� ���������� ������ � ������������ ����������� ��������.
      \param pBreak ��������� �� �������, �������������� ����������� ��������. �������� ������� ���������,
      ���� ����������� �������� �� ���������.
      \return ��� ������, ���������� ��������� ����������� �������� */
    virtual uint_8 WaitTransfer( CmtEvent *pBreak = 0 ) = 0;

    //! ����������� ������
    /*! ���������� ��������. ����� ����� ������ �������� ��������� ������������� ��������.*/
    virtual void   StopTransfer() = 0;
  };

class CmtFileSerial : public CmtFile {
    CmtSerial *mSerial;
  public:
    CmtFileSerial( CmtSerial *serial ) : CmtFile(), mSerial(serial) {}

    //������� ����-�����
    virtual void  Read( pvoid dest, uint32 size )  { mSerial->ReadBlock( dest, size, mBreak ); }
    virtual void  Write( cpvoid src, uint32 size ) { mSerial->WriteBlock( src, size, mBreak ); }
    
    //��������
    virtual bool Good() { return mSerial != 0 && mSerial->GetState() == CMTE_OK; }
    
    //���������������� �� ��������������
  };
