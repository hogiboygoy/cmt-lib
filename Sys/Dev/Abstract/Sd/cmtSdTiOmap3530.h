//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

typedef uint_8 (*CmtSdIsConnect)();    //!< Функция определения подключения карты CMTE_OK - подключена, CMTE_BD_NO_DEV

//! SD карта подключенная через специализированный интерфейс
struct CmtSdCard : public CmtBlockDevice {
    OMAP_MMCHS_REGS *mMmc;               //!< Специализированное устройство для интерфейса с картой
    int              mSdFactor;          //!< 0-для SDHC и 9-для SD
    CmtSdIsConnect   fnSdIsConnect;      //!< Функция определения подключения карты CMTE_OK - подключена, CMTE_BD_NO_DEV
#ifdef CMT_DMA
    CmtDmaBase      *mDmaRx;             //!< DMA канал на прием
    CmtDmaBase      *mDmaTx;             //!< DMA канал на передачу
#endif
  
  //! Конструктор
#ifdef CMT_DMA
  CmtSdCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect isConnect, int sdFactor, uint32 blockSize, uint32 blockCount, CmtDmaBase *dmaRx = 0, CmtDmaBase *dmaTx = 0 );
#else
  CmtSdCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect isConnect, int sdFactor, uint32 blockSize, uint32 blockCount );
#endif

  //! Чтение блоков
  //! \param dest - указатель на буфер для чтения данных
  //! \param block - номер начального блока данных
  //! \param count - количество блоков данных размером данного устройства
  //! \return 0-нет ошибки, иначе код ошибки
  virtual uint_8 Read( uint32 *dest, uint32 block, uint32 count );

  //! Запись блоков
  //! \param src - указатель на буфер данных для записи
  //! \param block - номер начального блока данных
  //! \param count - количество блоков данных размером данного устройства
  //! \return 0-нет ошибки, иначе код ошибки
  virtual uint_8 Write( const uint32 *src, uint32 block, uint32 count );
  
#ifdef CMT_DMA
  static CmtSdCard* CreateCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect  isConnect, CmtDmaBase *dmaRx = 0, CmtDmaBase *dmaTx = 0 );
#else
  static CmtSdCard* CreateCard( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect  isConnect );
#endif
  
protected:
  //! Чтение одного блока
  //! \param dest - указатель на буфер для чтения данных
  //! \param block - номер блока данных
  //! \return 0-нет ошибки, иначе код ошибки
  uint_8 ReadBlock( uint32 *dest, uint32 block );

  //! Запись одного блока
  //! \param src - указатель на буфер данных для записи
  //! \param block - номер блока данных
  //! \return 0-нет ошибки, иначе код ошибки
  uint_8 WriteBlock( const uint32 *src, uint32 block );
  
  //! Получение ответа карты
  //! \param response ожидаемый ответ карты
  //! \result CMTE_OK если ответ пришел или CMTE_FAIL если не пришел
  //static uint_8 Response( CmtSpiBase *spi, uint8 response );
  
  //! Ожидание завершения записи
  //! \result CMTE_OK если запись успешна или CMTE_FAIL если нет
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
  
//! Класс мониторинга SD карты
struct CmtSdDevPolling : public CmtDevPolling {
  CmtSdCard       *mSdCard;               //!< Подключенное устройство
  CmtSdIsConnect   fnIsConnect;           //!< Функция определения подключения карты CMTE_OK - подключена, CMTE_BD_NO_DEV
  OMAP_MMCHS_REGS *mMmc;                  //!< Специальное устройство для интерфейса с картой
  
  CmtSdDevPolling( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect  isConnect );
  
  //! Менеджемент (проверка) устройства
  virtual void Poll();
  
  //! Событие, выполняемое при подключении карты (карта mSdCard)
  virtual void OnConnect() {}
  
  //! Событие, выполняемое при отключении карты
  virtual void OnDisconnect() {}
  };    
  
#ifdef CMT_FS
//! Мониторинг SD карты с автоматическим подключением файловой системы
struct CmtSdFsDevPolling : public CmtSdDevPolling {
  CmtSdFsDevPolling( OMAP_MMCHS_REGS *mmc, CmtSdIsConnect  isConnect ) :
    CmtSdDevPolling( mmc, isConnect ) {}
  
  //! Событие, выполняемое при подключении карты (карта mSdCard)
  virtual void OnConnect();
  
  //! Событие, выполняемое при отключении карты
  virtual void OnDisconnect();
  };    
    
#endif
