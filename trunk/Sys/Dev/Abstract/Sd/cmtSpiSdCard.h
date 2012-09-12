//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

typedef uint_8 (*CmtSdIsConnect)();

//! ����� ������ SPI 8 ���, ��� ���������� 48 ��� - 6 ����
#define MMC_CMD_SIZE 6


//! SD ����� ������������ ����� ��������� SPI
struct CmtSpiSdCard : public CmtBlockDevice {
  CmtSpiPipeBase *mSpi;                  //!< spi ���������� ��� ���������� � ������
  uint8           MMCCmd[MMC_CMD_SIZE];  //!< ���� ������ ��� ������ ���������
  int             mSdFactor;             //!< 0-��� SDHC � 9-��� SD
  CmtSdIsConnect  fnIsConnect;           //!< ������� ����������� ����������� ����� CMTE_OK - ����������, CMTE_BD_NO_DEV
  
  //! �����������
  CmtSpiSdCard( CmtSpiPipeBase *spi, CmtSdIsConnect isConnect, int sdFactor, uint32 blockSize, uint32 blockCount );
  
  //! ������ ������
  //! \param dest - ��������� �� ����� ��� ������ ������
  //! \param block - ����� ���������� ����� ������
  //! \param count - ���������� ������ ������ �������� ������� ����������
  //! \return 0-��� ������, ����� ��� ������
  virtual uint_8 Read( uint32 *dest, uint32 block, uint32 count );

  //! ������ ������
  //! \param src - ��������� �� ����� ������ ��� ������
  //! \param block - ����� ���������� ����� ������
  //! \param count - ���������� ������ ������ �������� ������� ����������
  //! \return 0-��� ������, ����� ��� ������
  virtual uint_8 Write( const uint32 *src, uint32 block, uint32 count );
  
  static CmtSpiSdCard* CreateCard( CmtSpiPipeBase *spi, CmtSdIsConnect isConnect );
  
protected:
  //! ������ ������ �����
  //! \param dest - ��������� �� ����� ��� ������ ������
  //! \param block - ����� ����� ������
  //! \return 0-��� ������, ����� ��� ������
  uint_8 ReadBlock( uint32 *dest, uint32 block );

  //! ������ ������ �����
  //! \param src - ��������� �� ����� ������ ��� ������
  //! \param block - ����� ����� ������
  //! \return 0-��� ������, ����� ��� ������
  uint_8 WriteBlock( const uint32 *src, uint32 block );
  
  //! ��������� ������ �����
  //! \param response ��������� ����� �����
  //! \result CMTE_OK ���� ����� ������ ��� CMTE_FAIL ���� �� ������
  static uint_8 Response( CmtSpiBase *spi, uint8 response );
  
  //! �������� ���������� ������
  //! \result CMTE_OK ���� ������ ������� ��� CMTE_FAIL ���� ���
  uint_8 WaitForWriteFinish();
  };
  
//! ����� ����������� SD �����
struct CmtSpiSdDevPolling : public CmtDevPolling {
  CmtSpiSdCard   *mSdCard;               //!< ������������ ����������
  CmtSdIsConnect  fnIsConnect;           //!< ������� ����������� ����������� ����� CMTE_OK - ����������, CMTE_BD_NO_DEV
  CmtSpiPipeBase *mSpi;                  //!< spi ���������� ��� ���������� � ������
  
  CmtSpiSdDevPolling( CmtSpiPipeBase *spi, CmtSdIsConnect  isConnect );
  
  //! ����������� (��������) ����������
  virtual void Poll();
  
  //! �������, ����������� ��� ����������� ����� (����� mSdCard)
  virtual void OnConnect() {}
  
  //! �������, ����������� ��� ���������� �����
  virtual void OnDisconnect() {}
  };    
  
#ifdef CMT_FS
//! ���������� SD ����� � �������������� ������������ �������� �������
struct CmtSpiSdFsDevPolling : public CmtSpiSdDevPolling {
  CmtSpiSdFsDevPolling( CmtSpiPipeBase *spi, CmtSdIsConnect  isConnect ) :
    CmtSpiSdDevPolling( spi, isConnect ) {}
  
  //! �������, ����������� ��� ����������� ����� (����� mSdCard)
  virtual void OnConnect();
  
  //! �������, ����������� ��� ���������� �����
  virtual void OnDisconnect();
  };    
    
#endif
