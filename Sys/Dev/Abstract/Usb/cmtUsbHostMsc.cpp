//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <usb/cmtUsbMsc.h>

//! ���������� USB MSC
struct CmtUsbMsc : public CmtBlockDevice {
  MSC_CBW     mCbw;   //!< ��������� ������� SCSI
  MSC_CSW     mCsw;   //!< ��������� ������ SCSI
  uint32      mTag;   //!< ����� ������
  CmtUsbPipe *mIn;    //!< ������� �����
  CmtUsbPipe *mOut;   //!< �������� �����
  CmtUsbSlot *mSlot;  //!< ������� ����
  
  CmtUsbMsc( CmtUsbSlot *slot, CmtUsbPipe *in, CmtUsbPipe *out );
  ~CmtUsbMsc();
  
  bool Init();
  
  bool RequestSense( uint8 *capasity );
  
  //! �������� ������ �� USB
  /*! ���������� �������� cbw (������� ������ ���� �������������� ��������), 
    ������������ ����� ������ � ����� �������� ��������, ��������� ������������� ������
    \param cbw.cb[] ��������� ������ ���� �������������� ��������� ����� �������, ������� ��������������� � size
    \param dest ����� ������ ��� ������ ������
    \param size ������ ������ ������
    \return ��� ������ ��� CMTE_OK � ������ ���������� ������ */
  uint_8 ReadSCSI( uint8 *dest, uint32 size );
  
  //! �������� ������ � USB
  /*! ���������� �������� cbw (������� ������ ���� �������������� ��������),
    ������������ �������� ������ ��������� �������, ��������� ������������� ������
    \param cbw.cb[] ��������� ������ ���� �������������� ��������� ����� �������, ������� ��������������� � size
    \param src ����� ������ ������ ��� ��������
    \param size ������ ������ ������
    \return ��� ������ ��� CMTE_OK � ������ ���������� ������ */
  uint_8 WriteSCSI( const uint8 *src, uint32 size );

  //! ������ ������
  //! \param dest - ��������� �� ����� ��� ������ ������
  //! \param block - ����� ���������� ����� ������
  //! \param count - ���������� ������ ������ �������� ������� ����������
  //! \return 0-��� ������, ����� ��� ������
  virtual uint_8 Read( uint32 *dest, uint32 block, uint32 count );

  //! ������ ������
  //! \param src - ��������� �� ����� ������ ��� ������
  //! \param block - ����� ���������� ����� ������
  //! \param count - ���������� ������ ������ �������� ������� ����������
  //! \return 0-��� ������, ����� ��� ������
  virtual uint_8 Write( const uint32 *src, uint32 block, uint32 count );
  };

CmtUsbMsc::CmtUsbMsc(CmtUsbSlot *slot, CmtUsbPipe *in, CmtUsbPipe *out) :
  CmtBlockDevice(0,0),
  mIn(in),
  mOut(out),
  mSlot(slot) {
  //���������� �������������
  mSlot->AddRef();
  mIn->AddRef();
  mOut->AddRef();
  }

CmtUsbMsc::~CmtUsbMsc() {
  mIn->DecRef();
  mOut->DecRef();
  mSlot->DecRef();
  }

bool
CmtUsbMsc::RequestSense(uint8 *capasity) {
  memset( &mCbw, 0, sizeof(MSC_CBW) );
  mCbw.bCBLength   = 0xc;
  mCbw.CB[0]       = SCSI_REQUEST_SENSE;
  mCbw.CB[4]       = 0x12;

  return ReadSCSI( capasity, 0x12 ) == CMTE_OK;
  }

bool
CmtUsbMsc::Init() {
  uint8   capasity[36];

  //������ ����-������ INQURY
  memset( &mCbw, 0, sizeof(MSC_CBW) );
  mCbw.bCBLength   = 6;
  mCbw.CB[0]       = SCSI_INQUIRY;
  mCbw.CB[4]       = 36;

  if( ReadSCSI( capasity, 36 ) != CMTE_OK ) return false;
  
  //������ ����-������ SENCE
  if( !RequestSense( capasity ) ) return false;
  
  //READ_FORMAT_CAPASITY
  memset( &mCbw, 0, sizeof(MSC_CBW) );
  mCbw.bCBLength   = 10;
  mCbw.CB[0]       = SCSI_READ_FORMAT_CAPACITIES;
  mCbw.CB[8]       = 12;

  if( ReadSCSI( capasity, 12 ) != CMTE_OK ) {
    //��� ������ ���������� Flash ������ SCSI_REQUEST_SENSE    
    if( !RequestSense( capasity ) ) return false;
    
    //��������� CAPASITY
    memset( &mCbw, 0, sizeof(MSC_CBW) );
    mCbw.bCBLength   = 10;
    mCbw.CB[0]       = SCSI_READ_FORMAT_CAPACITIES;
    mCbw.CB[8]       = 12;
    
    if( ReadSCSI( capasity, 12 ) != CMTE_OK ) return false; //��������� CAPASITY ����������
    }
  
  if( capasity[8] == 2 ) {
    mBlockSize = capasity[11] | (capasity[10] << 8) | (capasity[9] << 16);
    mBlockCount = capasity[7] | (capasity[6] << 8) | (capasity[5] << 16) | (capasity[4] << 24);
    }
  else {
    //������������� ������ ��������
    //CAPASITY
    memset( &mCbw, 0, sizeof(MSC_CBW) );
    mCbw.bCBLength   = 8;
    mCbw.CB[0]       = SCSI_READ_CAPACITY;
    if( ReadSCSI( capasity, 8 ) != CMTE_OK ) {
      //��� ������ ���������� Flash ������ SCSI_REQUEST_SENSE
      if( !RequestSense( capasity ) ) return false; //� Sense �� �����

      memset( &mCbw, 0, sizeof(MSC_CBW) );
      mCbw.bCBLength   = 8;
      mCbw.CB[0]       = SCSI_READ_CAPACITY;
      if( ReadSCSI( capasity, 8 ) != CMTE_OK ) return false; //��������� CAPASITY ����������
      }
    mBlockSize = capasity[7] | (capasity[6] << 8) | (capasity[5] << 16);
    mBlockCount = capasity[3] | (capasity[2] << 8) | (capasity[1] << 16) | (capasity[0] << 24);
    }
  //������������� ������ �������
  return true;
  }

uint_8
CmtUsbMsc::Read(uint32 *dest, uint32 block, uint32 count) {
  //��������� CBW
  memset( &mCbw, 0, sizeof(MSC_CBW) );
  mCbw.bCBLength   = 12;
  mCbw.CB[0]       = SCSI_READ10;
  mCbw.CB[2]       = (block >> 24) & 0xff;
  mCbw.CB[3]       = (block >> 16) & 0xff;
  mCbw.CB[4]       = (block >>  8) & 0xff;
  mCbw.CB[5]       = (block >>  0) & 0xff;
  mCbw.CB[6]       = 0;
  mCbw.CB[7]       = (count >> 8) & 0xff;
  mCbw.CB[8]       = (count >> 0) & 0xff;

  return ReadSCSI( (uint8*)dest, count * mBlockSize );
  }

uint_8
CmtUsbMsc::Write(const uint32 *src, uint32 block, uint32 count) {
  //��������� CBW
  memset( &mCbw, 0, sizeof(MSC_CBW) );
  mCbw.bCBLength   = 12;
  mCbw.CB[0]       = SCSI_WRITE10;
  mCbw.CB[2]       = (block >> 24) & 0xff;
  mCbw.CB[3]       = (block >> 16) & 0xff;
  mCbw.CB[4]       = (block >>  8) & 0xff;
  mCbw.CB[5]       = (block >>  0) & 0xff;
  mCbw.CB[6]       = 0;
  mCbw.CB[7]       = (count >> 8) & 0xff;
  mCbw.CB[8]       = (count >> 0) & 0xff;

  return WriteSCSI( (const uint8*)src, count * mBlockSize );
  }

uint_8
CmtUsbMsc::ReadSCSI(uint8 *dest, uint32 size) {
  mCbw.dSignature  = MSC_CBW_Signature;
  mCbw.dTag = mTag++;
  mCbw.dDataLength = size;
  mCbw.bmFlags = 0x80;
  mCbw.bLUN = 0;
  if( mOut->Write( &mCbw, 31 ) != CMTE_OK ) return CMTE_FAIL;
  if( mIn->Read( dest, size ) != CMTE_OK ) return CMTE_FAIL;
  if( mIn->Read( &mCsw, 13 ) != CMTE_OK ) return CMTE_FAIL;
  //�������� ��������
  if( mCbw.dTag != mCsw.dTag || mCsw.dDataResidue != 0 || mCsw.dSignature != MSC_CSW_Signature ) return CMTE_FAIL;
  return CMTE_OK;
  }

uint_8
CmtUsbMsc::WriteSCSI(const uint8 *src, uint32 size) {
  mCbw.dSignature  = MSC_CBW_Signature;
  mCbw.dTag = mTag++;
  mCbw.dDataLength = size;
  mCbw.bmFlags = 0x80;
  mCbw.bLUN = 0;
  if( mOut->Write( &mCbw, 31 ) != CMTE_OK ) return CMTE_FAIL;
  if( mOut->Write( src, size ) != CMTE_OK ) return CMTE_FAIL;
  if( mIn->Read( &mCsw, 13 ) != CMTE_OK ) return CMTE_FAIL;
  //�������� ��������
  if( mCbw.dTag != mCsw.dTag || mCsw.dDataResidue != 0 || mCsw.dSignature != MSC_CSW_Signature ) return CMTE_FAIL;
  return CMTE_OK;
  }

CmtDev* 
CmtUsbSlot::CreateMassStorage( uint8 *cfgDescr ) {
  uint8 *ptr = cfgDescr + cfgDescr[0]; //��������� �� ���������
  CmtUsbMsc *dev = 0;
  
  //��������� �������� (�������������� ������ BulkOnly)
  if( ptr[7] != MSC_PROTOCOL_BULK_ONLY || ptr[5] != MSC_SUBCLASS_SCSI ) return 0;
  
  //���������
  
  //���������� ������������
  if( Query( BMRT_HOST_TO_DEVICE|BMRT_STANDARD|BMRT_DEVICE, BR_SET_CONFIGURATION, 1, 0, 0, 0 ) != CMTE_OK ) return 0;
  //���������� ���������
  if( Query( controlEd, BMRT_HOST_TO_DEVICE|BMRT_STANDARD|BMRT_INTERFACE, BR_SET_INTERFACE, 0, 0, 0, 0 ) != CMTE_OK ) return 0;
  //�.�. ��� �����
  if( Query( controlEd, BMRT_HOST_TO_DEVICE|BMRT_CLASS|BMRT_INTERFACE, 254, 0, 0, 0, 0 ) != CMTE_OK ) return 0;
  
  //�������������� ����������� ��� ������ ������������ EP
  int epCount = 0;
  CmtUsbPipePtr pipes[2];
  while( ptr[0] && epCount < 2 ) {
    if( ptr[1] == USB_ENDPOINT_DESCRIPTOR_TYPE ) {
      //���������� ���������, ������� Pipe
      pipes[epCount] = CreatePipe( ptr );
      if( pipes[epCount] ) epCount++;
      }
    ptr += ptr[0]; //������� � ���������� �����������
    }
  
  //� �������� ������������ ������ ���� ������� ��� ������ (���� �� �����, ������ �� ��������)
  if( epCount == 2 ) {
    if( (pipes[0]->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK) != (pipes[1]->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK) ) {
      if( pipes[0]->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK )
        dev = CmtUsbMsc( this, pipes[0], pipes[1] );
      else
        dev = CmtUsbMsc( this, pipes[1], pipes[0] );
      //���������� ������������, ����������������
      if( dev->Init() ) {
        //������������� ������ �������, ���������� �������� �������
#ifdef CMT_FS
        CmtFileSystemBase::CreateFileSystem( dev, 0 );
#endif
        }
      }
    }
  if( epCount ) pipes[0]->DecRef();
  if( epCount == 2 ) pipes[1]->DecRef();
  return dev;
  }
