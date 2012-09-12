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

//! ���������������� ���������� BULK-����
struct CmtUsbDevBulk : public CmtUsbDev {

  CmtUsbDevBulk();
  
  virtual uint8* GetDeviceDescriptor();
  virtual uint8* GetConfigDescriptor( uint_8 configIndex );
  virtual uint8* GetStringDescriptor( uint_8 stringIndex );

  //! �� ������
  //virtual void EndPoint1( uint32 ev );
  
  //! �� �����
  //virtual void EndPoint3( uint32 ev );
  };
