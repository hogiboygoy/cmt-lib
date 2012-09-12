//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

CmtUsbDev::CmtUsbDev() :
  CmtUsb() {
    USB_ResetCore();
    }

//! Перевести ядро устройства в исходное состояние
void 
CmtUsbDev::USB_ResetCore() {
  USB_DeviceStatus   = USB_POWER;
  USB_DeviceAddress  = 0;
  USB_Configuration  = 0;
  USB_EndPointMask   = 0;
  USB_EndPointHalt   = 0;
  USB_NumInterfaces  = 0;
  pData = 0; //!< Данные для обмена по ep0
  Count = 0; //!< Количество данных в байтах по ep0
  }

//! Этап передачи данных из буфера 
void 
CmtUsbDev::USB_DataInStage() {
  uint32 cnt;

  if( Count > USB_MAX_PACKET0 ) cnt = USB_MAX_PACKET0;
  else                          cnt = Count;

  cnt = USB_WriteEP8(0x80, pData, cnt );
  pData += cnt;
  Count -= cnt;
  }

//! Этап приема данных в буфера 
void 
CmtUsbDev::USB_DataOutStage() {
  uint32 cnt;

  cnt = USB_ReadEP8( 0x00, pData );
  pData += cnt;
  Count -= cnt;
  }

//! Этап передачи состояния (EP0)
void 
CmtUsbDev::USB_StatusInStage() {
  USB_WriteEP8(0x80, NULL, 0);
  }


//! Этап приема состояния (EP0)
void 
CmtUsbDev::USB_StatusOutStage() {
  USB_ReadEP8(0x00, EP0Buf);
  }

uint_8
CmtUsbDev::RequestGetStatus() {
  uint32 n, m;
  //Чье состояние требуется
  switch (SetupPacket.bmRequestType.BM.Recipient) {
    
    //Запрос к устройству
    case REQUEST_TO_DEVICE:
      pData = (uint8 *)&USB_DeviceStatus;
      USB_DataInStage();
      break;
      
    //Запрос к интерфейсу
    case REQUEST_TO_INTERFACE:
      if ((USB_Configuration != 0) && (SetupPacket.wIndex.WB.L < USB_NumInterfaces)) {
        EP0Buf[0] = EP0Buf[1] = 0; //16 битный нуль
        pData = EP0Buf;
        USB_DataInStage();
        } 
      else return CMTE_FAIL;
      break;
      
    //Запрос к конечной точке
    case REQUEST_TO_ENDPOINT:
      n = SetupPacket.wIndex.WB.L & 0x8F; //Номер EP
      m = (n & 0x80) ? ((1 << 16) << n) : (1 << n);
      if (((USB_Configuration != 0) || ((n & 0x0F) == 0)) && (USB_EndPointMask & m)) {
        //*((uint16 *)EP0Buf)
        EP0Buf[0] = (USB_EndPointHalt & m) ? 1 : 0;
        EP0Buf[1] = 0;
        pData = EP0Buf;
        USB_DataInStage();
        } 
      else return CMTE_FAIL;
      break;
    default:
      return CMTE_FAIL;
    }
  return CMTE_OK;
  }

//! Запросить очистку возможности
uint_8
CmtUsbDev::RequestClearFeature() {
  uint32 n, m;
  //Чью возможность требуется очистить
  switch (SetupPacket.bmRequestType.BM.Recipient) {
    
    //Запрос к устройству
    case REQUEST_TO_DEVICE:
      if(SetupPacket.wValue.W == USB_FEATURE_REMOTE_WAKEUP) {
        USB_DeviceStatus &= ~USB_GETSTATUS_REMOTE_WAKEUP;
        }
      else return CMTE_FAIL;
      break;
    
    //Запрос к интерфейсу
    case REQUEST_TO_INTERFACE:
      return CMTE_FAIL;
      
    //Запрос к конечной точке
    case REQUEST_TO_ENDPOINT:
      n = SetupPacket.wIndex.WB.L & 0x8F; //Номер конечной точки
      m = (n & 0x80) ? ((1 << 16) << n) : (1 << n);
      if ((USB_Configuration != 0) && ((n & 0x0F) != 0) && (USB_EndPointMask & m)) {
        if (SetupPacket.wValue.W == USB_FEATURE_ENDPOINT_STALL) {
          USB_ClrStallEP(n);
          USB_EndPointHalt &= ~m;
          }
        else return CMTE_FAIL;
        }
      else return CMTE_FAIL;
      break;
    default:
      return CMTE_FAIL;
    }
  return CMTE_OK;
  }

//! Запросить установку возможности
uint_8
CmtUsbDev::RequestSetFeature() {
  uint32 n, m;
  //Чью возможность требуется очистить
  switch (SetupPacket.bmRequestType.BM.Recipient) {
    
    //Запрос к устройству
    case REQUEST_TO_DEVICE:
      if( SetupPacket.wValue.W == USB_FEATURE_REMOTE_WAKEUP ) {
        USB_DeviceStatus |=  USB_GETSTATUS_REMOTE_WAKEUP;
        }
      else return CMTE_FAIL;
      break;

    //Запрос к интерфейсу
    case REQUEST_TO_INTERFACE:
      return CMTE_FAIL;

    //Запрос к конечной точке
    case REQUEST_TO_ENDPOINT:
      n = SetupPacket.wIndex.WB.L & 0x8F; //Номер EP
      m = (n & 0x80) ? ((1 << 16) << n) : (1 << n);
      if( (USB_Configuration != 0) && ((n & 0x0F) != 0) && (USB_EndPointMask & m) ) {
        if (SetupPacket.wValue.W == USB_FEATURE_ENDPOINT_STALL) {
          USB_SetStallEP(n);
          USB_EndPointHalt |=  m;
          }
        else return CMTE_FAIL;
        }
      else return CMTE_FAIL;
      break;
    default:
      return CMTE_FAIL;
    }
  USB_StatusInStage();
  return CMTE_OK;
  }

uint_8 
CmtUsbDev::RequestGetDescriptor() {
  uint32 len;

  switch (SetupPacket.bmRequestType.BM.Recipient) {
    
    //Запрос к устройству
    case REQUEST_TO_DEVICE:
      //Виды дескрипторов устройства
      switch (SetupPacket.wValue.WB.H) {
        
        //Дескриптор устройства
        case USB_DEVICE_DESCRIPTOR_TYPE:
          pData = GetDeviceDescriptor();
          len = USB_DEVICE_DESC_SIZE;
          break;
          
        //Дескриптор конфигурации (номер нужной конфигурации)
        case USB_CONFIGURATION_DESCRIPTOR_TYPE:
          pData = GetConfigDescriptor( SetupPacket.wValue.WB.L );
          len = cmtRead16( pData + 2 ) & 0xffff; //((USB_CONFIGURATION_DESCRIPTOR *)pD)->wTotalLength;
          break;
          
        //Строковый дескриптор
        case USB_STRING_DESCRIPTOR_TYPE:
          pData = GetStringDescriptor( SetupPacket.wValue.WB.L );
          //EP0Data.pData = (uint8 *)USB_StringDescriptor + SetupPacket.wValue.WB.L;
          len = pData[0]; //((USB_STRING_DESCRIPTOR *)EP0Data.pData)->bLength;
          break;
        default: return CMTE_FAIL;
        }
      break;

    default:
      return CMTE_FAIL;
    }

  if( Count > len ) Count = len;
  USB_DataInStage();

  return CMTE_OK;
  }

uint_8
CmtUsbDev::RequestSetConfig( uint_8 confIndex ) {
  uint32                 alt, n, m;
  if( confIndex ) {
    USB_CONFIGURATION_DESCRIPTOR *cd = (USB_CONFIGURATION_DESCRIPTOR*)GetConfigDescriptor( confIndex - 1 );
    if( cd ) {
      USB_Configuration = confIndex;
      USB_NumInterfaces = cd->bNumInterfaces;
      for (n = 0; n < USB_IF_NUM; n++) {
        USB_AltSetting[n] = 0;
        }
      for( n = 1; n < 16; n++ ) {
        if( USB_EndPointMask & (1 << n) ) {
          USB_DisableEP(n);
          }
        if( USB_EndPointMask & ((1 << 16) << n) ) {
          USB_DisableEP(n | 0x80);
          }
        }
      USB_EndPointMask = 0x00010001;
      USB_EndPointHalt = 0x00000000;
      USB_Configure(1);
      if( cd->bmAttributes & USB_CONFIG_SELF_POWERED ) USB_DeviceStatus |=  USB_GETSTATUS_SELF_POWERED;
      else                                             USB_DeviceStatus &= ~USB_GETSTATUS_SELF_POWERED;
      
      //Просканировать пристегнутые дескрипторы
      for( uint8 *ptr = ((uint8*)cd) + cd->bLength; *ptr; ptr += *ptr ) {
        //По типу дескриптора
        switch( ptr[1] ) {
          case USB_INTERFACE_DESCRIPTOR_TYPE:
            alt = ((USB_INTERFACE_DESCRIPTOR *)ptr)->bAlternateSetting;
            break;
          case USB_ENDPOINT_DESCRIPTOR_TYPE:
            if (alt == 0) {
              n = ((USB_ENDPOINT_DESCRIPTOR *)ptr)->bEndpointAddress & 0x8F;
              m = (n & 0x80) ? ((1 << 16) << (n & 0xf)) : (1 << n);
              USB_EndPointMask |= m;
              USB_ConfigEP( (USB_ENDPOINT_DESCRIPTOR*)ptr );
              USB_EnableEP( n );
              USB_ResetEP( n );
              }
            break;
          }
        }
      }
    }
  else {
    USB_Configuration = 0;
    for (n = 1; n < 16; n++) {
      if (USB_EndPointMask & (1 << n)) {
        USB_DisableEP(n);
        }
      if (USB_EndPointMask & ((1 << 16) << n)) {
        USB_DisableEP(n | 0x80);
        }
      }
    USB_EndPointMask  = 0x00010001;
    USB_EndPointHalt  = 0x00000000;
    USB_Configure(0);
    }

  if( USB_Configuration == SetupPacket.wValue.WB.L ) return CMTE_OK;
  return CMTE_FAIL;  
  }

uint_8
CmtUsbDev::RequestSetInterface() {
  uint32                 ifn, alt, old, msk, n, m;
  uint_8                 set = CMTE_FAIL;

  //Если конфигурация не установлена, то интерфейс установить не возможно
  if( USB_Configuration == 0 ) return CMTE_FAIL;

  USB_CONFIGURATION_DESCRIPTOR *cd = (USB_CONFIGURATION_DESCRIPTOR*)GetConfigDescriptor( USB_Configuration - 1 );
  
  if( cd ) {
    //Просканировать пристегнутые дескрипторы
    for( uint8 *ptr = ((uint8*)cd) + cd->bLength; *ptr; ptr += *ptr ) {
      //По типу дескриптора
      switch( ptr[1] ) {
        case USB_INTERFACE_DESCRIPTOR_TYPE:
          ifn = ((USB_INTERFACE_DESCRIPTOR *)ptr)->bInterfaceNumber;
          alt = ((USB_INTERFACE_DESCRIPTOR *)ptr)->bAlternateSetting;
          msk = 0;
          if( (ifn == SetupPacket.wIndex.WB.L) && (alt == SetupPacket.wValue.WB.L) ) {
            set = CMTE_OK;
            old = USB_AltSetting[ifn];
            USB_AltSetting[ifn] = (uint8)alt;
            }
          break;
        case USB_ENDPOINT_DESCRIPTOR_TYPE:
          if( ifn == SetupPacket.wIndex.WB.L ) {
            n = ((USB_ENDPOINT_DESCRIPTOR *)ptr)->bEndpointAddress & 0x8F;
            m = (n & 0x80) ? ((1 << 16) << n) : (1 << n);
            if (alt == SetupPacket.wValue.WB.L) {
              USB_EndPointMask |=  m;
              USB_EndPointHalt &= ~m;
              USB_ConfigEP( (USB_ENDPOINT_DESCRIPTOR *)ptr );
              USB_EnableEP(n);
              USB_ResetEP(n);
              msk |= m;
              }
            else if( (alt == old) && ((msk & m) == 0) ) {
              USB_EndPointMask &= ~m;
              USB_EndPointHalt &= ~m;
              USB_DisableEP(n);
              }
            }
          break;
        }
      }
    }
  
  return set;
  }

void
CmtUsbDev::StallEp0() { 
  USB_SetStallEP(0x80);
  Count = 0;
  }
      
//! Обработка событий для EP0
void 
CmtUsbDev::EndPoint0 (uint32 event) {
  switch (event) {
    //Событие настройки
    case USB_EVT_SETUP:
      //Получить пакет установки
      USB_ReadEP8(0x00, (uint8 *)&SetupPacket );
      //Длина ответа
      Count = SetupPacket.wLength;
      //Тип запроса
      switch (SetupPacket.bmRequestType.BM.Type) {

        //Стандартные запросы
        case REQUEST_STANDARD:
          //Типы запросов
          switch (SetupPacket.bRequest) {

            //Получить состояние
            case USB_REQUEST_GET_STATUS:
              if( RequestGetStatus() != CMTE_OK ) {
                StallEp0();
                return;
                }
              break;

            //Сбросить возможность
            case USB_REQUEST_CLEAR_FEATURE:
              if( RequestClearFeature() != CMTE_OK ) {
                StallEp0();
                return;
                }
              break;

            //Установить возможность
            case USB_REQUEST_SET_FEATURE:
              if( RequestSetFeature() != CMTE_OK ) {
                StallEp0();
                return;
                }
              break;

            //Установка адреса устройства
            case USB_REQUEST_SET_ADDRESS:
              if( SetupPacket.bmRequestType.BM.Recipient != REQUEST_TO_DEVICE ) {
                StallEp0();
                return;
                }
              //Запрос к устройству
              USB_DeviceAddress = 0x80 | SetupPacket.wValue.WB.L;
              USB_SetAddressPre( SetupPacket.wValue.WB.L );
              USB_StatusInStage();
              break;

            //Запрос на получение дескриптора
            case USB_REQUEST_GET_DESCRIPTOR:
             if( RequestGetDescriptor() != CMTE_OK ) {
                StallEp0();
                return;
                }
              break;

            //Запрос на установку дескриптора
            case USB_REQUEST_SET_DESCRIPTOR:
              USB_SetStallEP(0x00);
              Count = 0;
              break;

            //Запрос номера выбранной конфигурации
            case USB_REQUEST_GET_CONFIGURATION:
              if( SetupPacket.bmRequestType.BM.Recipient != REQUEST_TO_DEVICE ) {
                StallEp0();
                return;
                }
              pData = &USB_Configuration;
              USB_DataInStage();
             break;

            //Запрос на установку конфигурации
            case USB_REQUEST_SET_CONFIGURATION:
              if( SetupPacket.bmRequestType.BM.Recipient != REQUEST_TO_DEVICE ) {
                StallEp0();
                return;
                }
              if( RequestSetConfig(SetupPacket.wValue.WB.L) != CMTE_OK ) {
                StallEp0();
                return;
                }
              USB_StatusInStage();
              break;

            //Запрос на получение интерфейса
            case USB_REQUEST_GET_INTERFACE:
              if( SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_INTERFACE ) {
                if( (USB_Configuration != 0) && (SetupPacket.wIndex.WB.L < USB_NumInterfaces) ) {
                  pData = USB_AltSetting + SetupPacket.wIndex.WB.L;
                  USB_DataInStage();
                  break;
                  }
                }
              StallEp0();
              return;

            //Запрос на установку интерфейса
            case USB_REQUEST_SET_INTERFACE:
              if( SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_INTERFACE && RequestSetInterface() == CMTE_OK ) {
                USB_StatusInStage();
                break;
                }
              StallEp0();
              return;

            default:
              StallEp0();
              return;

          }
          break;

        //Запрос классу
        case REQUEST_CLASS:
          if( RequestClass() != CMTE_OK ) {
            StallEp0();
            return;
            }
          break;
          
        //Запрос производителю
        case REQUEST_VENDOR:
          if( RequestVendor() != CMTE_OK ) {
            StallEp0();
            return;
            }
          break;

        //Прочие запросы
        default:
          StallEp0();
          return;
        }
      break;

    //Событие приема данных
    case USB_EVT_OUT:
      if( SetupPacket.bmRequestType.BM.Dir == 0 ) {
        if( Count ) {
          //Пока не приняты все данные, принять
          USB_DataOutStage();
          if( Count == 0 ) {
            EndPoint0OutComplete();
#ifdef MMM            
            //Приняты все данные, специальный случай для HID устройств
            if( SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_INTERFACE && SetupPacket.bmRequestType.BM.Type == REQUEST_CLASS && SetupPacket.bRequest == HID_REQUEST_SET_REPORT ) {
              if( HID_SetReport() != CMTE_OK ) {
                StallEp0();
                return;
                }
              USB_StatusInStage();
              }
#endif
            }
          }
        } 
      else USB_StatusOutStage();
      break;

    //Событие выдачи данных
    case USB_EVT_IN:
      if( SetupPacket.bmRequestType.BM.Dir == 1 ) {
        if( Count ) USB_DataInStage();
        else        USB_DataInEnd();
        }
      else {
        if( USB_DeviceAddress & 0x80 ) {
          USB_DeviceAddress &= 0x7F;
          USB_SetAddress( USB_DeviceAddress );
          }
        }
      break;

    //Очистить stall для выдачи
    case USB_EVT_IN_STALL:
      USB_ClrStallEP(0x80);
      break;

    //Очистить stall для приема
    case USB_EVT_OUT_STALL:
      USB_ClrStallEP(0x00);
      break;

    }
  }

//Аппаратные реализации
#if defined( CMT_STM32F103CBT6 )
  #include <Sys/Dev/Abstract/Usb/cmtUsbStm32f103cb.cpp>
#elif defined( CMT_STM32F105VCT6 )
  #include <Sys/Dev/Abstract/Usb/cmtUsbStm32f105vb.cpp>
  #include <Sys/Dev/Abstract/Usb/cmtUsbStm32f105vb_dev_1.cpp>
  #include <Sys/Dev/Abstract/Usb/cmtUsbStm32f105vb_dev_2.cpp>
  #include <Sys/Dev/Abstract/Usb/cmtUsbStm32f105vb_dev_3.cpp>
#else
  #error "CMT:not defined USB device for this CPU"
#endif

