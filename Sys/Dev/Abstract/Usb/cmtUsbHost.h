//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! USB канал
struct CmtUsbPipe : public CmtDev {
  uint8  bEndpointAddress; //!< адрес EP
  uint8  bmAttributes;     //!< атрибуты EP включая тип
  uint16 wMaxPacketSize;   //!< максимальный размер пакета
  uint8  bInterval;        //!< периодичность
  
  CmtUsbPipe() : CmtDev() {}
  
  virtual uint32 Read( pvoid dest, uint32 size ) { return 0; }
  
  virtual uint32 Write( cpvoid sour, uint32 size ) { return 0; }
  };

typedef CmtUsbPipe *CmtUsbPipePtr;

//! USB Slot разъем для подключения устройства USB
struct CmtUsbSlot : public CmtDevPolling {
  CmtDev     *mDev;    //!< Устройство, подключенной к слоту
  uint8       mDevAdr; //!< Адрес, назначаемый устройству, подключаемому к данному слоту
  
  CmtUsbSlot( uint8 adr ) : CmtDevPolling(), mDev(0), mDevAdr(adr) {}
  ~CmtUsbSlot();

  //Поддержка классов устройств
#ifdef CMT_USB_HOST_MASS
  CmtDev* CreateMassStorage( uint8 *cfgDescr );
#endif
#ifdef CMT_USB_HOST_COM
  CmtDev* CreateCDC( uint8 *cfgDescr );
#endif
  
  //! Создать устройство
  /*! Создает устройство на базе идентификаторов производителя и продукта.
    Для каждой комбинации должна быть определена функция создания.
    Если нет специфичной функции создания, то устройство создается на базе класса устройства.
    \param idVendor идентификатор производителя
    \param idProduct идентификатор продукта */
  CmtDev* CreateDev( uint16 idVendor, uint16 idProduct );
  
  //! Менеджемент (проверка) устройства
  virtual void Poll();
  
  //! Сбросить устройство и slot
  virtual void Reset() = 0;
  
  //! Ожидать прохождения заданного количества фреймов
  /*! \param frame количество фреймов, которое нужно прождать */
  void WaitFrame( uint32 frame );
  
  void SetupControlMaxPacket( uint32 maxPacket );
  
  void SetupControlAddress( uint8 addr );
  
  //! Обмен по EP0
  /*! Осуществляет цикл обмена по EP0 отправляя setup и получая (не обязательно) dest
    \param setup буфер, содержащий пакет setup
    \param setupSize размер буфера
    \param dest буфер для ответа (если нуль, значит ответа нет)
    \param destSize размер буфера для ответа
    \return код ошибки, CMTE_OK - когда нет ошибок */
  virtual uint_8 Control( cpvoid setup, uint32 setupSize, pvoid dest, uint32 destSize ) = 0;
  
  //! Запрос по EP0
  /*! Формирует структуру запроса (пакет SETUP), осуществляет обмен
    \param requestType тип запроса
    \param request номер запроса заданного типа
    \param value значение
    \param index индекс
    \param len длина дополнительных данных
    \param dest указатель на буфер для принимаемых данных
    \return код ошибки, CMTE_OK - когда нет ошибок */
  uint_8 Query( uint8 requestType, uint8 request, uint16 value, uint16 index, uint16 len, void *dest );
  
  //! Получить дескриптор
  /*! Получает дескриптор заданного типа
    \param descriptorType тип запрашиваемого дескриптора
    \param len размер буфера для размещения дескриптора
    \param dest буфер для дескриптора
    \return код ошибки, CMTE_OK - когда нет ошибок */
  uint_8 GetDescriptor( uint16 descriptorType, uint16 len, void *dest );
  
  //! Установить конфигурацию
  /*! Устанавливает конфигурацию с заданным номером как рабочую
    \param confNum номер конфигурации, которую нужно установить
    \return код ошибки, CMTE_OK - когда нет ошибок */
  uint_8 SetConfiguration( uint16 confNum );
  
  //! Проверка наличия устройства в слоте
  /*! Проверяет наличие устройства в слоте
    \return истина, когда устройство присутствует, ложь в противном случае */
  virtual bool   IsConnect() = 0;
  
  //! Создает канал обмена
  /*! Создает канал обмена для EP по дескриптору
    \param descrPtr указатель на дескриптор EP
    \return указатель на канал обмена */
  virtual CmtUsbPipe* CreatePipe( uint8 EPNum, uint8 EPType, uint8 wMaxData ) = 0;
  
  };

typedef CmtUsbSlot *CmtUsbSlotPtr;

//! USB Host
struct CmtUsbHost : public CmtUsb {
 
  void Init();
  
  //! Прерывание
  virtual void IrqHandler();
  };

