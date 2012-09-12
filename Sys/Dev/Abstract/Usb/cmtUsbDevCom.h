//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#define USB_POWER_EVENT     0
#define USB_RESET_EVENT     1
#define USB_SUSPEND_EVENT   0
#define USB_RESUME_EVENT    0
#define USB_WAKEUP_EVENT    0
#define USB_SOF_EVENT       0
#define USB_ERROR_EVENT     0

#define USB_EP_NUM          4

//! data structure for GET_LINE_CODING / SET_LINE_CODING class requests
struct CmtUsbComLineCoding{
  uint32 dwDTERate;
  uint8  bCharFormat;
  uint8  bParityType;
  uint8  bDataBits;
  };

//static TLineCoding LineCoding = {256000, 0, 0, 8};
#define   SET_LINE_CODING      0x20
#define   GET_LINE_CODING      0x21
#define   SET_CONTROL_LINE_STATE   0x22

//! USB ���������� ����������� COM-����
struct CmtUsbDevCom : public CmtUsbDev {
  
  CmtUsbComLineCoding mLineCoding;    //!< ������� ��������� �����
  uint8               mClassDataCode; //!< ��� ������� � ������, �������� ������������� ���� ������
  
  CmtUsbDevCom();
  
  virtual uint_8 RequestClass();
  virtual uint8* GetDeviceDescriptor();
  virtual uint8* GetConfigDescriptor( uint_8 configIndex );
  virtual uint8* GetStringDescriptor( uint_8 stringIndex );
  virtual void   EndPoint0OutComplete();

  virtual void   SetupComParam() {} //���������� ����� ��������� �����
  //virtual void EndPoint1( uint32 ev );
  //virtual void EndPoint2( uint32 ev );
  //virtual void EndPoint3( uint32 ev );
  };
