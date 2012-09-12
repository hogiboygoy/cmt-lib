//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

typedef uint_8 (*CmtSdIsConnect)();

//! Длина данных SPI 8 бит, ММС использует 48 бит - 6 байт
#define MMC_CMD_SIZE 6


//! SD карта подключенная через интерфейс SPI
struct CmtSpiSdCard : public CmtBlockDevice {
  CmtSpiPipeBase *mSpi;                  //!< spi устройство для интерфейса с картой
  uint8           MMCCmd[MMC_CMD_SIZE];  //!< блок байтов для обмена командами
  int             mSdFactor;             //!< 0-для SDHC и 9-для SD
  CmtSdIsConnect  fnIsConnect;           //!< Функция определения подключения карты CMTE_OK - подключена, CMTE_BD_NO_DEV
  
  //! Конструктор
  CmtSpiSdCard( CmtSpiPipeBase *spi, CmtSdIsConnect isConnect, int sdFactor, uint32 blockSize, uint32 blockCount );
  
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
  
  static CmtSpiSdCard* CreateCard( CmtSpiPipeBase *spi, CmtSdIsConnect isConnect );
  
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
  static uint_8 Response( CmtSpiBase *spi, uint8 response );
  
  //! Ожидание завершения записи
  //! \result CMTE_OK если запись успешна или CMTE_FAIL если нет
  uint_8 WaitForWriteFinish();
  };
  
//! Класс мониторинга SD карты
struct CmtSpiSdDevPolling : public CmtDevPolling {
  CmtSpiSdCard   *mSdCard;               //!< Подключенное устройство
  CmtSdIsConnect  fnIsConnect;           //!< Функция определения подключения карты CMTE_OK - подключена, CMTE_BD_NO_DEV
  CmtSpiPipeBase *mSpi;                  //!< spi устройство для интерфейса с картой
  
  CmtSpiSdDevPolling( CmtSpiPipeBase *spi, CmtSdIsConnect  isConnect );
  
  //! Менеджемент (проверка) устройства
  virtual void Poll();
  
  //! Событие, выполняемое при подключении карты (карта mSdCard)
  virtual void OnConnect() {}
  
  //! Событие, выполняемое при отключении карты
  virtual void OnDisconnect() {}
  };    
  
#ifdef CMT_FS
//! Мониторинг SD карты с автоматическим подключением файловой системы
struct CmtSpiSdFsDevPolling : public CmtSpiSdDevPolling {
  CmtSpiSdFsDevPolling( CmtSpiPipeBase *spi, CmtSdIsConnect  isConnect ) :
    CmtSpiSdDevPolling( spi, isConnect ) {}
  
  //! Событие, выполняемое при подключении карты (карта mSdCard)
  virtual void OnConnect();
  
  //! Событие, выполняемое при отключении карты
  virtual void OnDisconnect();
  };    
    
#endif
