//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//-----------------------------------------------------------------------------
// ControlPacket
//-----------------------------------------------------------------------------
// bmRequestType (BMRT)
//-----------------------------------------------------------------------------
#define BMRT_HOST_TO_DEVICE  0x00U
#define BMRT_DEVICE_TO_HOST  0x80U
//and
#define BMRT_STANDARD        0x00U
#define BMRT_CLASS           0x20U
#define BMRT_VENDOR          0x40U
//and
#define BMRT_DEVICE          0x00U
#define BMRT_INTERFACE       0x01U
#define BMRT_ENDPOINT        0x02U
#define BMRT_OTHER           0x03U

//-----------------------------------------------------------------------------
// bRequest (BR)
//-----------------------------------------------------------------------------
#define BR_GET_STATUS         0
#define BR_CLEAR_FEATURE      1
#define BR_CR_RESERVED2       2
#define BR_SET_FEATURE        3
#define BR_CR_RESERVED4       4
#define BR_SET_ADDRESS        5
#define BR_GET_DESCRIPTOR     6
#define BR_SET_DESCRIPTOR     7
#define BR_GET_CONFIGURATION  8
#define BR_SET_CONFIGURATION  9
#define BR_GET_INTERFACE     10
#define BR_SET_INTERFACE     11
#define BR_SYNCH_FRAME       12

//-----------------------------------------------------------------------------
//wValue
//-----------------------------------------------------------------------------
#define WV_DEVICE_DESCRIPTOR        (1<<8)
#define WV_CONFIGURATION_DESCRIPTOR (2<<8)
#define WV_STRING_DESCRIPTOR        (3<<8)
#define WV_INTERFACE_DESCRIPTOR     (4<<8)
#define WV_ENDPOINT_DESCRIPTOR      (5<<8)
#define WV_DEVICE_QUALIFIER         (6<<8)
#define WV_OTHER_SPEED_CONFIG       (7<<8)
#define WV_INTERFACE_POWER          (8<<8)


CmtUsbSlot::~CmtUsbSlot() {
  if( mDev ) {
    mDev->DecRef();
    mDev = 0;
    }
  }


void
CmtUsbSlot::Poll() {
  if( mDev ) {
   //���������� ���� ����������, ��������� ����������
   if( !IsConnect() ) {
     //���������� �����������
     mDev->DecRef();
     mDev = 0;
     }
    }
  else {
    //���������� ���� ���������, ��������� �����������
    if( IsConnect() ) {
      //��������� �����������, ��������� ����������

      //���������� ����������
      USB_DEVICE_DESCRIPTOR devDPR;
      int code;
      //�������� ����� ����������
      WaitFrame( 200 );
      Reset();
      WaitFrame( 200 );
      
      //�������� ���������� ����������
      code = GetDescriptor( WV_DEVICE_DESCRIPTOR, sizeof(USB_DEVICE_DESCRIPTOR), &devDPR );
      if( code == CMTE_OK ) {
        //���������� ������������ ������ ������ ��� EP0
        SetupControlMaxPacket( devDPR.bMaxPacketSize0 );
        
        mDev = CreateDev( devDPR.idVendor, devDPR.idProduct );
        }
      
      //��������� �������������� ����������
      if( mDev == 0 ) {
        //���������� �� ������������ �� ������� ������, ������������ ������� ����������
        //��� ���������� ����������� ������� ������� ����������
        mDev = new CmtDev();
        }
      }
    }
  }

CmtDev*
CmtUsbSlot::CreateDev( uint16 idVendor, uint16 idProduct ) {
  //�������� �� ����������� ����������
  // if( idVendor == xxxx && idProduct == xxxx )
  
  //����������� ���������� �� �������, ���������� ���������� �� ������ � ������ ����������
  
  //����� �����
  WaitFrame( 200 );
  Reset();
  WaitFrame( 200 );

  //���������� ����� ����������
  if( Query( BMRT_HOST_TO_DEVICE|BMRT_STANDARD|BMRT_DEVICE, BR_SET_ADDRESS, mDevAdr, 0, 0, 0 ) != CMTE_OK ) return 0;
  
  SetupControlAddress( mDevAdr );
  
  //�������� ������������
  USB_CONFIGURATION_DESCRIPTOR confDPR;
  
  if( GetDescriptor( WV_CONFIGURATION_DESCRIPTOR, 9, &confDPR ) != CMTE_OK ) return 0;
  
  uint8 *descrBuf = new uint8[confDPR.wTotalLength];
  
  //�������� ������������ �� ����� ������������� �������������
  if( GetDescriptor( WV_CONFIGURATION_DESCRIPTOR, confDPR.wTotalLength, descrBuf ) != CMTE_OK ) {
    delete descrBuf;
    return 0;
    }
  
  //��������� � ����������
  uint8 *ptr = descrBuf + descrBuf[0];
  
  //��������������, ��� ��� ���������
  if( ptr[1] != USB_INTERFACE_DESCRIPTOR_TYPE ) {
    delete descrBuf;
    return 0;
    }
  
  //������������ ���������� �� ������ ������ ����������
  CmtDev *dev = 0;
  switch( ptr[5] ) {
#ifdef CMT_USB_HOST_MASS
    case USB_DEVICE_CLASS_STORAGE : 
      dev = CreateMassStorage( descrBuf );
      break;
#endif
      
#ifdef CMT_USB_HOST_COM
    case USB_DEVICE_CLASS_COMMUNICATIONS :
      dev = CreateCDC( descrBuf );
      break;
#endif
    
    }
  
  delete descrBuf;
  return dev;
  }

#pragma pack(1)
struct USB_CONTROL_PACKET {  // 8 bytes
  uint8  mRequestType;
  uint8  mRequest;
  uint16 mValue;
  uint16 mIndex;
  uint16 mLength;

  USB_CONTROL_PACKET( uint8 requestType, uint8 request, uint16 value, uint16 index, uint16 len ) :
    mRequestType(requestType),
    mRequest(request),
    mValue(value),
    mIndex(index),
    mLength(len) {}
  };
#pragma pack()

 uint_8 
 CmtUsbSlot::Query( uint8 requestType, uint8 request, uint16 value, uint16 index, uint16 len, void *dest ) {
   //������������ �����
   USB_CONTROL_PACKET ctrlPack( requestType, request, value, index, len );
   
   //��������� �����
   return Control( &ctrlPack, 8, dest, len );   
   }

 uint_8
 CmtUsbSlot::GetDescriptor( uint16 value, uint16 len, void *dest ) {
   return Query( BMRT_DEVICE_TO_HOST|BMRT_STANDARD|BMRT_DEVICE, BR_GET_DESCRIPTOR, value, 0, len, dest );
   }
 
 uint_8 
 CmtUsbSlot::SetConfiguration( uint16 confNum ) {
   return Query( BMRT_HOST_TO_DEVICE|BMRT_STANDARD|BMRT_DEVICE, BR_SET_CONFIGURATION, confNum, 0, 0, 0 );
   }
 
//���������� ����������
#ifdef CMT_STM32F105VCT6_LIB0
  #include <usb/cmtUsbStm32f105vb.cpp>
  #include <usb/cmtUsbStm32f105vb_host_1.cpp>
  #include <usb/cmtUsbStm32f105vb_host_2.cpp>
  #include <usb/cmtUsbStm32f105vb_host_3.cpp>
#endif
 
