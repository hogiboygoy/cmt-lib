//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! USB Device
struct CmtUsbDev : public CmtUsb {
  uint16           USB_DeviceStatus;  //!< ��������� ����������
  uint8            USB_DeviceAddress; //!< ����������� ����� ��� ����������
  uint8            USB_Configuration; //!< ����� ��������� ������������
  uint32           USB_EndPointMask;  //!< 
  uint32           USB_EndPointHalt;
  uint8            USB_NumInterfaces;
  uint8            USB_AltSetting[USB_IF_NUM];
  uint8           *pData; //!< ������ ��� ������ �� ep0
  uint16           Count; //!< ���������� ������ � ������ �� ep0
  USB_SETUP_PACKET SetupPacket;
  uint8            EP0Buf[USB_MAX_PACKET0];
 
  CmtUsbDev();
  
  //! ����������
  virtual void IrqHandler();
  
  //! ��������� �������
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
  
  //����������� ������� EP0
  
  //! ������ �� ��������� ���������
  virtual uint_8 RequestGetStatus();  
  
  //! ������ �� ������� �����������
  virtual uint_8 RequestClearFeature();

  //! ������ �� ��������� �����������
  virtual uint_8 RequestSetFeature();
  
  //! ������ �� ��������� �����������
  virtual uint_8 RequestGetDescriptor();
  
  //! ������ �� ��������� ������������
  virtual uint_8 RequestSetConfig( uint_8 configIndex );
  
  //! ������ �� ��������� ����������
  virtual uint_8 RequestSetInterface();
  
  //! ������ � ������
  virtual uint_8 RequestClass() { return CMTE_FAIL; }
  
  //! ������ � �������������
  virtual uint_8 RequestVendor() { return CMTE_FAIL; }
  
  //! HID
  virtual uint_8 HID_SetReport() { return CMTE_FAIL; }
  
  //�����������
  virtual uint8* GetDeviceDescriptor() = 0;
  virtual uint8* GetConfigDescriptor( uint_8 configIndex ) = 0;
  virtual uint8* GetStringDescriptor( uint_8 stringIndex ) = 0;

  //! ���������������� USB ����������        
  void Init();

  //! ��������� ���� ���������� � �������� ���������
  void USB_ResetCore();
  
  //! ���� �������� ������ �� ������ (EP0)
  void USB_DataInStage();
  
  //! ���� �������� ������ �� ������ (EP0) ��������
  void USB_DataInEnd();

  //! ���� ������ ������ � ������ (EP0)
  void USB_DataOutStage();
  
  //! ���� �������� ��������� (EP0)
  void USB_StatusInStage();
  
  //! ���� ������ ��������� (EP0)
  void USB_StatusOutStage();
  
  //! ������ ���� ������ ������ � ����� (EP0)
  void USB_DataOut( void *data, uint16 count ) { pData = (uint8*)data; Count = count; }
  
  //! �������� ������ � EP
  /*! \param EPNum ����� EP
    \param pData ������������ ������
    \param cnt ���������� ������������ ������
    \return ���������� ���������� ������ */
  uint32 USB_WriteEP8 (uint8 EPNum, uint8 *pData, uint32 cnt);
  
  //! ��������� ������ �� EP
  /*! \param EPNum ����� EP
    \param pData ����� ��� ������
    \return ���������� ����������� ������ */
  uint32 USB_ReadEP8 (uint8 EPNum, uint8 *pData);
  
  //! �������� NAK �� OUT ������ (���������� ����� ������� OUT)
  /*! \param EPNum ����� EP ����� �������*/
  void   USB_SetNAK( uint8 EPNum );
  
  //! ��������� �������� NAK �� OUT ������ (������ ����� ������� OUT)
  /*! \param EPNum ����� EP ����� �������*/
  void   USB_ResetNAK( uint8 EPNum );
  
  //! �������� stall ��� EP
  /*! \param EPNum ����� EP */
  void USB_ClrStallEP( uint8 EPNum );
  
  
  //! ���������� stall ��� EP
  /*! \param EPNum ����� EP */
  void USB_SetStallEP( uint8 EPNum );
  
  //! ����� EP
  /*! \param EPNum ����� EP */
  void USB_ResetEP( uint8 EPNum );
  
  //! ��������� EP
  /*! \param EPNum ����� EP */
  void USB_DisableEP( uint8 EPNum );
  
  //! ��������� EP
  /*! \param EPNum ����� EP */
  void USB_EnableEP (uint8 EPNum);
  
  //! ���������� ����������� ������ ����������� EP
  /*! \param dir Out (dir == 0), In (dir <> 0) */
  void USB_DirCtrlEP( uint8 dir );
  
  //! ���������������� EP
  /*! \param pEPD ��������� �� ���������� EP */
  void USB_ConfigEP( USB_ENDPOINT_DESCRIPTOR *pEPD );
  
  //! ��������� ���� "����������������"
  /*! \param cfg Configure/Deconfigure */
  void USB_Configure (uint_8 cfg);

  //! ���������� ����������� ����� ���������� � ���� ������� �� ��������� ������
  /*! \param adr ����� ���������� */
  void USB_SetAddressPre( uint8 adr );
  
  //! ���������� ����������� ����� ���������� ����� ���� �������� �������
  /*! \param adr ����� ���������� */
  void USB_SetAddress (uint8 adr);
  
  //! ��������� ����������� ����������
  /*! \param con Connect/Disconnect */
  void USB_Connect (int con);
  
  //! ����� ���������� USB
  virtual void USB_Reset();
  
  //! ������� � ������ �����
  virtual void USB_Suspend();
  
  //! "����������"
  virtual void USB_WakeUp();
  
  //! ������ ������
  virtual void USB_StartOfFrame();
  
  //! ��������� ������
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

//! ������� ��������� ������������
#define USB_STR_DESC_LANG_ID      0
#define USB_STR_DESC_MANUFACTURER 1
#define USB_STR_DESC_PRODUCT      2
#define USB_STR_DESC_SERIAL       3
#define USB_STR_DESC_INTERF0      4
#define USB_STR_DESC_INTERF1      5
#define USB_STR_DESC_INTERF2      6
#define USB_STR_DESC_INTERF3      7

void cmtInitUsb();
