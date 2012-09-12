//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! USB �����
struct CmtUsbPipe : public CmtDev {
  uint8  bEndpointAddress; //!< ����� EP
  uint8  bmAttributes;     //!< �������� EP ������� ���
  uint16 wMaxPacketSize;   //!< ������������ ������ ������
  uint8  bInterval;        //!< �������������
  
  CmtUsbPipe() : CmtDev() {}
  
  virtual uint32 Read( pvoid dest, uint32 size ) { return 0; }
  
  virtual uint32 Write( cpvoid sour, uint32 size ) { return 0; }
  };

typedef CmtUsbPipe *CmtUsbPipePtr;

//! USB Slot ������ ��� ����������� ���������� USB
struct CmtUsbSlot : public CmtDevPolling {
  CmtDev     *mDev;    //!< ����������, ������������ � �����
  uint8       mDevAdr; //!< �����, ����������� ����������, ������������� � ������� �����
  
  CmtUsbSlot( uint8 adr ) : CmtDevPolling(), mDev(0), mDevAdr(adr) {}
  ~CmtUsbSlot();

  //��������� ������� ���������
#ifdef CMT_USB_HOST_MASS
  CmtDev* CreateMassStorage( uint8 *cfgDescr );
#endif
#ifdef CMT_USB_HOST_COM
  CmtDev* CreateCDC( uint8 *cfgDescr );
#endif
  
  //! ������� ����������
  /*! ������� ���������� �� ���� ��������������� ������������� � ��������.
    ��� ������ ���������� ������ ���� ���������� ������� ��������.
    ���� ��� ����������� ������� ��������, �� ���������� ��������� �� ���� ������ ����������.
    \param idVendor ������������� �������������
    \param idProduct ������������� �������� */
  CmtDev* CreateDev( uint16 idVendor, uint16 idProduct );
  
  //! ����������� (��������) ����������
  virtual void Poll();
  
  //! �������� ���������� � slot
  virtual void Reset() = 0;
  
  //! ������� ����������� ��������� ���������� �������
  /*! \param frame ���������� �������, ������� ����� �������� */
  void WaitFrame( uint32 frame );
  
  void SetupControlMaxPacket( uint32 maxPacket );
  
  void SetupControlAddress( uint8 addr );
  
  //! ����� �� EP0
  /*! ������������ ���� ������ �� EP0 ��������� setup � ������� (�� �����������) dest
    \param setup �����, ���������� ����� setup
    \param setupSize ������ ������
    \param dest ����� ��� ������ (���� ����, ������ ������ ���)
    \param destSize ������ ������ ��� ������
    \return ��� ������, CMTE_OK - ����� ��� ������ */
  virtual uint_8 Control( cpvoid setup, uint32 setupSize, pvoid dest, uint32 destSize ) = 0;
  
  //! ������ �� EP0
  /*! ��������� ��������� ������� (����� SETUP), ������������ �����
    \param requestType ��� �������
    \param request ����� ������� ��������� ����
    \param value ��������
    \param index ������
    \param len ����� �������������� ������
    \param dest ��������� �� ����� ��� ����������� ������
    \return ��� ������, CMTE_OK - ����� ��� ������ */
  uint_8 Query( uint8 requestType, uint8 request, uint16 value, uint16 index, uint16 len, void *dest );
  
  //! �������� ����������
  /*! �������� ���������� ��������� ����
    \param descriptorType ��� �������������� �����������
    \param len ������ ������ ��� ���������� �����������
    \param dest ����� ��� �����������
    \return ��� ������, CMTE_OK - ����� ��� ������ */
  uint_8 GetDescriptor( uint16 descriptorType, uint16 len, void *dest );
  
  //! ���������� ������������
  /*! ������������� ������������ � �������� ������� ��� �������
    \param confNum ����� ������������, ������� ����� ����������
    \return ��� ������, CMTE_OK - ����� ��� ������ */
  uint_8 SetConfiguration( uint16 confNum );
  
  //! �������� ������� ���������� � �����
  /*! ��������� ������� ���������� � �����
    \return ������, ����� ���������� ������������, ���� � ��������� ������ */
  virtual bool   IsConnect() = 0;
  
  //! ������� ����� ������
  /*! ������� ����� ������ ��� EP �� �����������
    \param descrPtr ��������� �� ���������� EP
    \return ��������� �� ����� ������ */
  virtual CmtUsbPipe* CreatePipe( uint8 EPNum, uint8 EPType, uint8 wMaxData ) = 0;
  
  };

typedef CmtUsbSlot *CmtUsbSlotPtr;

//! USB Host
struct CmtUsbHost : public CmtUsb {
 
  void Init();
  
  //! ����������
  virtual void IrqHandler();
  };

