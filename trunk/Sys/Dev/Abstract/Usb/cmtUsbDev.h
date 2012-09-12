//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! USB Device
struct CmtUsbDev : public CmtUsb {
  uint16           USB_DeviceStatus;  //!< Состояние устройства
  uint8            USB_DeviceAddress; //!< Назначенный адрес для устройства
  uint8            USB_Configuration; //!< Номер выбранной конфигурации
  uint32           USB_EndPointMask;  //!< 
  uint32           USB_EndPointHalt;
  uint8            USB_NumInterfaces;
  uint8            USB_AltSetting[USB_IF_NUM];
  uint8           *pData; //!< Данные для обмена по ep0
  uint16           Count; //!< Количество данных в байтах по ep0
  USB_SETUP_PACKET SetupPacket;
  uint8            EP0Buf[USB_MAX_PACKET0];
 
  CmtUsbDev();
  
  //! Прерывание
  virtual void IrqHandler();
  
  //! Обработка событий
  virtual void EndPoint0( uint32 ev );
  virtual void EndPoint0OutComplete() {}
  virtual void EndPoint1( uint32 ev ) {}
  virtual void EndPoint2( uint32 ev ) {}
  virtual void EndPoint3( uint32 ev ) {}
  virtual void EndPoint4( uint32 ev ) {}
  virtual void EndPoint5( uint32 ev ) {}
  virtual void EndPoint6( uint32 ev ) {}
  virtual void EndPoint7( uint32 ev ) {}
  virtual void EndPoint8( uint32 ev ) {}
  virtual void EndPoint9( uint32 ev ) {}
  virtual void EndPoint10( uint32 ev ) {}
  virtual void EndPoint11( uint32 ev ) {}
  virtual void EndPoint12( uint32 ev ) {}
  virtual void EndPoint13( uint32 ev ) {}
  virtual void EndPoint14( uint32 ev ) {}
  virtual void EndPoint15( uint32 ev ) {}
  
  //Стандартные запросы EP0
  
  //! Запрос на получение состояния
  virtual uint_8 RequestGetStatus();  
  
  //! Запрос на очистку возможности
  virtual uint_8 RequestClearFeature();

  //! Запрос на установку возможности
  virtual uint_8 RequestSetFeature();
  
  //! Запрос на получение дескриптора
  virtual uint_8 RequestGetDescriptor();
  
  //! Запрос на установку конфигурации
  virtual uint_8 RequestSetConfig( uint_8 configIndex );
  
  //! Запрос на установку интерфейса
  virtual uint_8 RequestSetInterface();
  
  //! Запрос к классу
  virtual uint_8 RequestClass() { return CMTE_FAIL; }
  
  //! Запрос к производителю
  virtual uint_8 RequestVendor() { return CMTE_FAIL; }
  
  //! HID
  virtual uint_8 HID_SetReport() { return CMTE_FAIL; }
  
  //Дескрипторы
  virtual uint8* GetDeviceDescriptor() = 0;
  virtual uint8* GetConfigDescriptor( uint_8 configIndex ) = 0;
  virtual uint8* GetStringDescriptor( uint_8 stringIndex ) = 0;

  //! Инициализировать USB устройство        
  void Init();

  //! Перевести ядро устройства в исходное состояние
  void USB_ResetCore();
  
  //! Этап передачи данных из буфера (EP0)
  void USB_DataInStage();
  
  //! Этап передачи данных из буфера (EP0) завершен
  void USB_DataInEnd();

  //! Этап приема данных в буфера (EP0)
  void USB_DataOutStage();
  
  //! Этап передачи состояния (EP0)
  void USB_StatusInStage();
  
  //! Этап приема состояния (EP0)
  void USB_StatusOutStage();
  
  //! Начать этап приема данных в буфер (EP0)
  void USB_DataOut( void *data, uint16 count ) { pData = (uint8*)data; Count = count; }
  
  //! Записать данные в EP
  /*! \param EPNum номер EP
    \param pData записываемые данные
    \param cnt количество записываемых байтов
    \return количество записанных байтов */
  uint32 USB_WriteEP8 (uint8 EPNum, uint8 *pData, uint32 cnt);
  
  //! Прочитать данные из EP
  /*! \param EPNum номер EP
    \param pData буфер для данных
    \return количество прочитанных данных */
  uint32 USB_ReadEP8 (uint8 EPNum, uint8 *pData);
  
  //! Отвечать NAK на OUT пакеты (прекратить прием пакетов OUT)
  /*! \param EPNum номер EP кроме нулевой*/
  void   USB_SetNAK( uint8 EPNum );
  
  //! Перестать отвечать NAK на OUT пакеты (начать прием пакетов OUT)
  /*! \param EPNum номер EP кроме нулевой*/
  void   USB_ResetNAK( uint8 EPNum );
  
  //! Очистить stall для EP
  /*! \param EPNum номер EP */
  void USB_ClrStallEP( uint8 EPNum );
  
  
  //! Установить stall для EP
  /*! \param EPNum номер EP */
  void USB_SetStallEP( uint8 EPNum );
  
  //! Сброс EP
  /*! \param EPNum номер EP */
  void USB_ResetEP( uint8 EPNum );
  
  //! Запретить EP
  /*! \param EPNum номер EP */
  void USB_DisableEP( uint8 EPNum );
  
  //! Разрешить EP
  /*! \param EPNum номер EP */
  void USB_EnableEP (uint8 EPNum);
  
  //! Установить направление работы контрольной EP
  /*! \param dir Out (dir == 0), In (dir <> 0) */
  void USB_DirCtrlEP( uint8 dir );
  
  //! Сконфигурировать EP
  /*! \param pEPD указатель на дескриптор EP */
  void USB_ConfigEP( USB_ENDPOINT_DESCRIPTOR *pEPD );
  
  //! Поставить флаг "Сконфигурировано"
  /*! \param cfg Configure/Deconfigure */
  void USB_Configure (uint_8 cfg);

  //! Установить собственный адрес устройства в фазе запроса на установку адреса
  /*! \param adr адрес устройства */
  void USB_SetAddressPre( uint8 adr );
  
  //! Установить собственный адрес устройства после фазы отправки статуса
  /*! \param adr адрес устройства */
  void USB_SetAddress (uint8 adr);
  
  //! Разрешить подключение устройства
  /*! \param con Connect/Disconnect */
  void USB_Connect (int con);
  
  //! Сброс устройства USB
  virtual void USB_Reset();
  
  //! Переход в спящий режим
  virtual void USB_Suspend();
  
  //! "Проснуться"
  virtual void USB_WakeUp();
  
  //! Начало фрейма
  virtual void USB_StartOfFrame();
  
  //! Обработка ошибок
  virtual void USB_Error( uint8 errCode );
  
  void StallEp0();
  };

/* USB Enpoint Callback Events */
#define USB_EVT_SETUP       1
#define USB_EVT_OUT         2
#define USB_EVT_IN          3
#define USB_EVT_OUT_NAK     4
#define USB_EVT_IN_NAK      5
#define USB_EVT_OUT_STALL   6
#define USB_EVT_IN_STALL    7

//! Индексы строковых дескрипторов
#define USB_STR_DESC_LANG_ID      0
#define USB_STR_DESC_MANUFACTURER 1
#define USB_STR_DESC_PRODUCT      2
#define USB_STR_DESC_SERIAL       3
#define USB_STR_DESC_INTERF0      4
#define USB_STR_DESC_INTERF1      5
#define USB_STR_DESC_INTERF2      6
#define USB_STR_DESC_INTERF3      7

void cmtInitUsb();
