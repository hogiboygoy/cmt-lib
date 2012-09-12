//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

typedef uint_8 (*CmtSdIsConnect)();    //!< ������� ����������� ����������� ����� CMTE_OK - ����������, CMTE_BD_NO_DEV

//! SD ����� ������������ ����� ������������������ ���������
struct CmtSdCard : public CmtBlockDevice {
    OMAP_MMCHS_REGS *mMmc;               //!< ������������������ ���������� ��� ���������� � ������
    int              mSdFactor;          //!< 0-��� SDHC � 9-��� SD
    CmtSdIsConnect   fnSdIsConnect;      //!< ������� ����������� ����������� ����� CMTE_OK - ����������, CMTE_BD_NO_DEV
#ifdef CMT_DMA
    CmtDmaBase      *mDmaRx;             //!< DMA ����� �� �����
    CmtDmaBase      *mDmaTx;             //!< DMA ����� �� ��������
#endif
  
  //! �����������
#ifdef CMT_DMA
  CmtSdCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect isConnect, int sdFactor, uint32 blockSize, uint32 blockCount, CmtDmaBase *dmaRx = 0, CmtDmaBase *dmaTx = 0 );
#else
  CmtSdCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect isConnect, int sdFactor, uint32 blockSize, uint32 blockCount );
#endif

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
  
#ifdef CMT_DMA
  static CmtSdCard* CreateCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect  isConnect, CmtDmaBase *dmaRx = 0, CmtDmaBase *dmaTx = 0 );
#else
  static CmtSdCard* CreateCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect  isConnect );
#endif
  
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
  //static uint_8 Response( CmtSpiBase *spi, uint8 response );
  
  //! �������� ���������� ������
  //! \result CMTE_OK ���� ������ ������� ��� CMTE_FAIL ���� ���
  uint_8 WaitForWriteFinish();
  
  static void   BoardInit( OMAP_MMCHS_REGS *mmc );
  static void   InitStream( OMAP_MMCHS_REGS *mmc );
  static void   ClockConfig( OMAP_MMCHS_REGS *mmc, uint32 iclk, uint32 clk_div );
  static void   InitSetup( OMAP_MMCHS_REGS *mmc );
  static bool   SendCommand( OMAP_MMCHS_REGS *mmc, uint32 cmd, uint32 arg, uint32 *response, uint32 blkCount = 0 );
  static bool   DetectCard( OMAP_MMCHS_REGS *mmc, bool *sdCardType, uint32 *factor, uint32 *RCA );
  static bool   GetData( OMAP_MMCHS_REGS *mmc, uint32 *output_buf );  
  static bool   PutData( OMAP_MMCHS_REGS *mmc, const uint32 *src );  
  };
  
//! ����� ����������� SD �����
struct CmtSdDevPolling : public CmtDevPolling {
  CmtSdCard       *mSdCard;               //!< ������������ ����������
  CmtSdIsConnect   fnIsConnect;           //!< ������� ����������� ����������� ����� CMTE_OK - ����������, CMTE_BD_NO_DEV
  OMAP_MMCHS_REGS *mMmc;                  //!< ����������� ���������� ��� ���������� � ������
  
  CmtSdDevPolling( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect  isConnect );
  
  //! ����������� (��������) ����������
  virtual void Poll();
  
  //! �������, ����������� ��� ����������� ����� (����� mSdCard)
  virtual void OnConnect() {}
  
  //! �������, ����������� ��� ���������� �����
  virtual void OnDisconnect() {}
  };    
  
#ifdef CMT_FS
//! ���������� SD ����� � �������������� ������������ �������� �������
struct CmtSdFsDevPolling : public CmtSdDevPolling {
  CmtSdFsDevPolling( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect  isConnect ) :
    CmtSdDevPolling( mmc, isConnect ) {}
  
  //! �������, ����������� ��� ����������� ����� (����� mSdCard)
  virtual void OnConnect();
  
  //! �������, ����������� ��� ���������� �����
  virtual void OnDisconnect();
  };    
    
#endif
