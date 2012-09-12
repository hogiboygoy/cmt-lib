//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

CmtUsbDevMsc::CmtUsbDevMsc( uint32 bufBlockCount ) :
  CmtUsbDev(),
  mBulkStage(MSC_BS_CBW),
  mBlockSize(0),
  mBuf(0),
  mBufBlockCount(bufBlockCount),
  mTransferBuf(0),
  mTransferLen(0) {
    mQuery.mQueryCode = 0;
    //mQuery.mUsb = this;
    }

CmtUsbDevMsc::~CmtUsbDevMsc() {
  if( mBuf ) delete mBuf;
  }

void
CmtUsbDevMsc::EndPoint1( uint32 ev ) {
  switch (mBulkStage) {
    case MSC_BS_DATA_IN:
      switch (CBW.CB[0]) {
        case SCSI_READ10:
          MSC_MemoryRead();
          break;
        }
      break;
    case MSC_BS_DATA_IN_LAST:
      MSC_SetCSW();
      break;
    case MSC_BS_DATA_IN_LAST_STALL:
      USB_SetStallEP(MSC_EP_IN);
      MSC_SetCSW();
      break;
    case MSC_BS_CSW:
      mBulkStage = MSC_BS_CBW;
      break;
    }
  }

void
CmtUsbDevMsc::EndPoint3( uint32 ev ) {
  BulkLen = USB_ReadEP8(MSC_EP_OUT, BulkBuf);
  switch( mBulkStage ) {
    case MSC_BS_CBW:
      MSC_GetCBW();
      break;
    case MSC_BS_DATA_OUT:
      switch (CBW.CB[0]) {
        case SCSI_WRITE10:
          MSC_MemoryWrite();
          break;
        case SCSI_VERIFY10:
          MSC_MemoryVerify();
          break;
        }
      break;
    default:
      USB_SetStallEP(MSC_EP_OUT);
      CSW.bStatus = CSW_PHASE_ERROR;
      MSC_SetCSW();
      break;
    }
  }

void
CmtUsbDevMsc::MSC_SetCSW() {
  CSW.dSignature = MSC_CSW_Signature;
  USB_WriteEP8( MSC_EP_IN, (uint8*)&CSW, sizeof(CSW) );
  mBulkStage = MSC_BS_CSW;
  }

void
CmtUsbDevMsc::MSC_GetCBW() {
  uint32 n;

  for (n = 0; n < BulkLen; n++) {
    *((uint8*)&CBW + n) = BulkBuf[n];
    }
  if ((BulkLen == sizeof(CBW)) && (CBW.dSignature == MSC_CBW_Signature)) {
    /* Valid CBW */
    CSW.dTag = CBW.dTag;
    CSW.dDataResidue = CBW.dDataLength;
    if ((CBW.bLUN != 0) || (CBW.bCBLength < 1) || CBW.bCBLength > 16) {
fail: CSW.bStatus = CSW_CMD_FAILED;
      MSC_SetCSW();
      } 
    else {
      switch (CBW.CB[0]) {
        case SCSI_TEST_UNIT_READY:
          MSC_TestUnitReady();
          break;
        case SCSI_REQUEST_SENSE:
          MSC_RequestSense();
          break;
        case SCSI_FORMAT_UNIT:
          goto fail;
        case SCSI_INQUIRY:
          MSC_Inquiry();
          break;
        case SCSI_START_STOP_UNIT:
          goto fail;
        case SCSI_MEDIA_REMOVAL:
          goto fail;
        case SCSI_MODE_SELECT6:
          goto fail;
        case SCSI_MODE_SENSE6:
          MSC_ModeSense6();
          break;
        case SCSI_MODE_SELECT10:
          goto fail;
        case SCSI_MODE_SENSE10:
          MSC_ModeSense10();
          break;
        case SCSI_READ_FORMAT_CAPACITIES:
          MSC_ReadFormatCapacity();
          break;
        case SCSI_READ_CAPACITY:
          MSC_ReadCapacity();
          break;
        case SCSI_READ10:
          if (MSC_RWSetup()) {
            if ((CBW.bmFlags & 0x80) != 0) {
              mBulkStage = MSC_BS_DATA_IN;
              mMemOk = true;
              mTransferLen = 0;
              mTransferInt = true;
              MSC_QueryRead();
              } 
            else {
              USB_SetStallEP(MSC_EP_OUT);
              CSW.bStatus = CSW_PHASE_ERROR;
              MSC_SetCSW();
              }
            }
          break;
        case SCSI_WRITE10:
          if (MSC_RWSetup()) {
            if ((CBW.bmFlags & 0x80) == 0) {
              mBulkStage = MSC_BS_DATA_OUT;
              mMemOk = true;
              if( mBufBlockCount * mBlockSize < CBW.dDataLength ) mTransferLen = mBufBlockCount * mBlockSize;
              else mTransferLen = CBW.dDataLength;
              mTransferBuf = mBuf;
              mReceivInt = false;
              } 
            else {
              USB_SetStallEP(MSC_EP_IN);
              CSW.bStatus = CSW_PHASE_ERROR;
              MSC_SetCSW();
              }
            }
          break;
        case SCSI_VERIFY10:
          if (MSC_RWSetup()) {
            if ((CBW.bmFlags & 0x80) == 0) {
              mBulkStage = MSC_BS_DATA_OUT;
              mMemOk = true;
              mTransferLen = 0;
              MSC_MemoryVerify();
              } 
            else {
              USB_SetStallEP(MSC_EP_IN);
              CSW.bStatus = CSW_PHASE_ERROR;
              MSC_SetCSW();
              }
            }
          break;
        default:
          goto fail;
        }
      }
    } 
  else {
    /* Invalid CBW */
    USB_SetStallEP(MSC_EP_IN);
    USB_SetStallEP(MSC_EP_OUT);
    mBulkStage = MSC_BS_ERROR;
    }
  
  }

void 
CmtUsbDevMsc::MSC_ReadFormatCapacity() {
  if (!DataInFormat()) return;
  
  mQuery.mQueryCode = MSC_DEVQR_FORMAT_CAPASITY;
  mQuery.mDevice = CBW.bLUN;
  }

void 
CmtUsbDevMsc::MSC_ReadCapacity() {
  if (!DataInFormat()) return;

  mQuery.mQueryCode = MSC_DEVQR_READ_CAPASITY;
  mQuery.mDevice = CBW.bLUN;
  }

void 
CmtUsbDevMsc::MSC_ModeSense10() {
  if (!DataInFormat()) return;

  BulkBuf[ 0] = 0x00;
  BulkBuf[ 1] = 0x06;
  BulkBuf[ 2] = 0x00;
  BulkBuf[ 3] = 0x00;
  BulkBuf[ 4] = 0x00;
  BulkBuf[ 5] = 0x00;
  BulkBuf[ 6] = 0x00;
  BulkBuf[ 7] = 0x00;

  BulkLen = 8;
  DataInTransfer();
  }

void 
CmtUsbDevMsc::MSC_ModeSense6() {
  if (!DataInFormat()) return;

  BulkBuf[ 0] = 0x03;
  BulkBuf[ 1] = 0x00;
  BulkBuf[ 2] = 0x00;
  BulkBuf[ 3] = 0x00;

  BulkLen = 4;
  DataInTransfer();
  }

void 
CmtUsbDevMsc::MSC_Inquiry() {
  if (!DataInFormat()) return;

  BulkBuf[ 0] = 0x00;          /* Direct Access Device */
  BulkBuf[ 1] = 0x80;          /* RMB = 1: Removable Medium */
  BulkBuf[ 2] = 0x00;          /* Version: No conformance claim to standard */
  BulkBuf[ 3] = 0x01;

  BulkBuf[ 4] = 36-4;          /* Additional Length */
  BulkBuf[ 5] = 0x80;          /* SCCS = 1: Storage Controller Component */
  BulkBuf[ 6] = 0x00;
  BulkBuf[ 7] = 0x00;

  BulkBuf[ 8] = 'R';           /* Vendor Identification */
  BulkBuf[ 9] = 'D';
  BulkBuf[10] = 'M';
  BulkBuf[11] = '-';
  BulkBuf[12] = 'C';
  BulkBuf[13] = ' ';
  BulkBuf[14] = ' ';
  BulkBuf[15] = ' ';

  BulkBuf[16] = 'R';           /* Product Identification */
  BulkBuf[17] = 'D';
  BulkBuf[18] = 'M';
  BulkBuf[19] = '-';
  BulkBuf[20] = '2';
  BulkBuf[21] = ' ';
  BulkBuf[22] = 'N';
  BulkBuf[23] = 'A';
  BulkBuf[24] = 'V';
  BulkBuf[25] = 'I';
  BulkBuf[26] = ' ';
  BulkBuf[27] = 'D';
  BulkBuf[28] = 'I';
  BulkBuf[29] = 'S';
  BulkBuf[30] = 'K';
  BulkBuf[31] = ' ';

  BulkBuf[32] = '1';           /* Product Revision Level */
  BulkBuf[33] = '.';
  BulkBuf[34] = '0';
  BulkBuf[35] = ' ';

  BulkLen = 36;
  DataInTransfer();
  }

void 
CmtUsbDevMsc::MSC_RequestSense() {
  if (!DataInFormat()) return;

  BulkBuf[ 0] = 0x70;          /* Response Code */
  BulkBuf[ 1] = 0x00;
  BulkBuf[ 2] = 0x02;          /* Sense Key */
  BulkBuf[ 3] = 0x00;
  BulkBuf[ 4] = 0x00;
  BulkBuf[ 5] = 0x00;
  BulkBuf[ 6] = 0x00;
  BulkBuf[ 7] = 0x0A;          /* Additional Length */
  BulkBuf[ 8] = 0x00;
  BulkBuf[ 9] = 0x00;
  BulkBuf[10] = 0x00;
  BulkBuf[11] = 0x00;
  BulkBuf[12] = 0x30;          /* ASC */
  BulkBuf[13] = 0x01;          /* ASCQ */
  BulkBuf[14] = 0x00;
  BulkBuf[15] = 0x00;
  BulkBuf[16] = 0x00;
  BulkBuf[17] = 0x00;

  BulkLen = 18;
  DataInTransfer();
  }

void 
CmtUsbDevMsc::MSC_TestUnitReady() {
  if (CBW.dDataLength != 0) {
    if ((CBW.bmFlags & 0x80) != 0) {
      USB_SetStallEP(MSC_EP_IN);
      } 
    else {
      USB_SetStallEP(MSC_EP_OUT);
      }
    }

  CSW.bStatus = CSW_CMD_PASSED;
  MSC_SetCSW();
  }

void 
CmtUsbDevMsc::DataInTransfer() {
  if (BulkLen > CBW.dDataLength) {
    BulkLen = CBW.dDataLength;
    }

  USB_WriteEP8( MSC_EP_IN, BulkBuf, BulkLen );
  mBulkStage = MSC_BS_DATA_IN_LAST;

  CSW.dDataResidue -= BulkLen;
  CSW.bStatus = CSW_CMD_PASSED;
  }

bool 
CmtUsbDevMsc::DataInFormat() {
  if (CBW.dDataLength == 0) {
    CSW.bStatus = CSW_PHASE_ERROR;
    MSC_SetCSW();
    return false;
    }
  if ((CBW.bmFlags & 0x80) == 0) {
    USB_SetStallEP(MSC_EP_OUT);
    CSW.bStatus = CSW_PHASE_ERROR;
    MSC_SetCSW();
    return false;
    }
  return true;
  }

  uint32 Offset;
bool 
CmtUsbDevMsc::MSC_RWSetup() {
  /* Logical Block Address of First Block */
  mBlockStart = (CBW.CB[2] << 24) |
                (CBW.CB[3] << 16) |
                (CBW.CB[4] <<  8) |
                (CBW.CB[5] <<  0);

  /* Number of Blocks to transfer */
  mBlockCount = (CBW.CB[7] <<  8) |
                (CBW.CB[8] <<  0);
  
  Offset = mBlockStart * 512;

  if( CBW.dDataLength != mBlockCount * mBlockSize ) {
    USB_SetStallEP(MSC_EP_IN);
    USB_SetStallEP(MSC_EP_OUT);
    CSW.bStatus = CSW_PHASE_ERROR;
    MSC_SetCSW();
    return false;
    }

  return true;
  }

void 
CmtUsbDevMsc::MSC_MemoryVerify() {
  uint32 n;
  if( mTransferLen != 0 ) {
    //������� ������ ��� ���������
    for( n = 0; n < BulkLen && mTransferLen; n++ ) {
      if( mTransferBuf[n] != BulkBuf[n]) {
        mMemOk = false;
        break;
        }
      }
    mTransferLen -= BulkLen;
    mTransferBuf += BulkLen;
    CSW.dDataResidue -= BulkLen;
    }
  if( mTransferLen == 0 ) {
    //��������� ��������� ��������� ������ ������
    if( mBlockCount ) {
      if( mBufBlockCount < mBlockCount ) mQuery.mBlockCount = mBufBlockCount;
      else mQuery.mBlockCount = mBlockCount;
      
      mQuery.mDevice     = CBW.bLUN;
      mQuery.mBlockStart = mBlockStart;
      mQuery.mBuf        = mBuf;
      mQuery.mQueryCode  = MSC_DEVQR_VERIFY;

      mBlockCount -= mQuery.mBlockCount;
      mBlockStart += mQuery.mBlockCount;
      }
    }
  if( (CSW.dDataResidue == 0) || (mBulkStage == MSC_BS_CSW) ) {
    CSW.bStatus = (mMemOk) ? CSW_CMD_PASSED : CSW_CMD_FAILED;
    MSC_SetCSW();
    }
  }

void 
CmtUsbDevMsc::MSC_QueryRead() {
  //��������� ��������� ��������� ������ ������
  if( mBlockCount ) {
    if( mBufBlockCount < mBlockCount ) mQuery.mBlockCount = mBufBlockCount;
    else mQuery.mBlockCount = mBlockCount;
      
    mQuery.mDevice     = CBW.bLUN;
    mQuery.mBlockStart = mBlockStart;
    mQuery.mBuf        = mBuf;
    mQuery.mQueryCode  = MSC_DEVQR_READ;
    mBlockCount -= mQuery.mBlockCount;
    mBlockStart += mQuery.mBlockCount;
    }
  }
  
void 
CmtUsbDevMsc::MSC_MemoryRead() {
  uint32 n;
  mTransferInt = true;
  if( mTransferLen != 0 ) {
    mTransferInt = false;
    //������� ����������� ������ ��� ��������
    if( mTransferLen > MSC_MAX_PACKET ) n = MSC_MAX_PACKET;
    else n = mTransferLen;

    USB_WriteEP8( MSC_EP_IN, mTransferBuf, n );

    mTransferLen -= n;
    mTransferBuf += n;
    if( n > CSW.dDataResidue ) CSW.dDataResidue = 0;
    else                       CSW.dDataResidue -= n;

    if( mTransferLen == 0 ) 
      MSC_QueryRead();
    if( CSW.dDataResidue == 0 ) {
      mBulkStage = MSC_BS_DATA_IN_LAST;
      }

    if( mBulkStage != MSC_BS_DATA_IN ) {
      CSW.bStatus = mMemOk ? CSW_CMD_PASSED : CSW_CMD_FAILED;
      }
    }
  }

void 
CmtUsbDevMsc::MSC_MemoryWrite() {
  //������������ �� �������� ������ � �����
  if( mTransferLen ) {
    //������ ����� �������� ����� ���� ������ ��� �� ������
    uint32 n;
    //����������� �������� ���� � ����� ����� ������ �� ����������
    // ������� ���� ������ �� ���������, �� ������ (���� ���������� ������ ��� ����� ����� ������)
    for (n = 0; n < BulkLen; n++) {
      mTransferBuf[n] = BulkBuf[n];
      }
    //��������������� ��������� � ������� ������
    mTransferBuf += BulkLen;
    mTransferLen -= BulkLen;

    //��������� ���������� ������ ��� ������ �� ����
    if( mTransferLen == 0 ) {
      //����� ����� ��� ������, ����������� ������� �� ������
      mQuery.mBlockStart = mBlockStart;
      mQuery.mBlockCount = ((mTransferBuf - mBuf) + (mBlockSize - 1)) / mBlockSize;
      mQuery.mBuf        = mBuf;
      mQuery.mQueryCode  = MSC_DEVQR_WRITE;

      //��������������� ������� ����� � ����� ���������� �����
      mBlockCount -= mQuery.mBlockCount;
      mBlockStart += mQuery.mBlockCount;
      //��������� ����� ������� �� ����� ������
      USB_SetNAK( MSC_EP_OUT );
      }
  
    //��������������� ����� ����� ���������� ��� ������ ������
    if( BulkLen > CSW.dDataResidue ) CSW.dDataResidue = 0;
    else                             CSW.dDataResidue -= BulkLen;
    }
  else {
    //������ �������� �����, ����� ����� ��� ������ � ������ ���, �����
    //��������� ����, ��� �������� ������
    if( mReceivInt ) {
      //���� ��� ��� ��������� �����, ����� ����� ������������, ��������� ���� ������ ��������
      mMemOk = false;
      GPIO_SetBits( GPIOC, MBIT1 );
      }
    else mReceivInt = true;
    //��������� ����� ������� �� ����� ������ (����� ��������� � ���� �������� - 2 ���)
    USB_SetNAK( MSC_EP_OUT );
    }

  if( mBulkStage == MSC_BS_CSW ) {
    CSW.bStatus = mMemOk ? CSW_CMD_PASSED : CSW_CMD_FAILED;
    MSC_SetCSW();
    }
  }

void 
CmtUsbDevMsc::QueryComplete( uint_8 result ) {
  OTGD_FS_DisableGlobalInt();
  uint32 code = mQuery.mQueryCode;
  mQuery.mQueryCode = 0;
  switch( code ) {
    case MSC_DEVQR_WRITE :
      if( result != CMTE_OK ) mMemOk = false;
      if( (CSW.dDataResidue == 0) ) {
        CSW.bStatus = mMemOk ? CSW_CMD_PASSED : CSW_CMD_FAILED;
        MSC_SetCSW();
        }
      else {
        if( mBufBlockCount * mBlockSize < CSW.dDataResidue ) mTransferLen = mBufBlockCount * mBlockSize;
        else mTransferLen = CSW.dDataResidue;
        mTransferBuf = mBuf;
        //����������� ���� ������ ��������� �����
        if( mReceivInt ) {
          uint32 n;
          for (n = 0; n < BulkLen; n++) {
            mTransferBuf[n] = BulkBuf[n];
            }
          mTransferBuf += BulkLen;
          mTransferLen -= BulkLen;
          if( BulkLen > CSW.dDataResidue ) CSW.dDataResidue = 0;
          else                             CSW.dDataResidue -= BulkLen;
          mReceivInt = false;
          }
        }
      //��������� ����� �������
      USB_ResetNAK( MSC_EP_OUT );
      break;
    case MSC_DEVQR_READ :
      if( result != CMTE_OK ) {
        mMemOk = false;
        }
      mTransferBuf = mQuery.mBuf;
      mTransferLen = mQuery.mBlockCount * mBlockSize;
      if( mTransferInt )
        MSC_MemoryRead();
      break;
    case MSC_DEVQR_VERIFY :
      mTransferLen = mQuery.mBlockCount * mBlockSize;
      mTransferBuf = mQuery.mBuf;
      if( result != CMTE_OK ) {
        mMemOk = false;
        }
      break;
    case MSC_DEVQR_FORMAT_CAPASITY :
      if( result == CMTE_OK ) {
        uint32 MSC_BlockCount = mQuery.mBlockCount;
        mBlockSize  = mQuery.mBlockStart;

        if( mBuf == 0 ) mBuf = new uint8[mBlockSize * mBufBlockCount];
        
        BulkBuf[ 0] = 0x00;
        BulkBuf[ 1] = 0x00;
        BulkBuf[ 2] = 0x00;
        BulkBuf[ 3] = 0x08;          /* Capacity List Length */
      
        /* Block Count */
        BulkBuf[ 4] = (MSC_BlockCount >> 24) & 0xFF;
        BulkBuf[ 5] = (MSC_BlockCount >> 16) & 0xFF;
        BulkBuf[ 6] = (MSC_BlockCount >>  8) & 0xFF;
        BulkBuf[ 7] = (MSC_BlockCount >>  0) & 0xFF;
      
        /* Block Length */
        BulkBuf[ 8] = 0x02;          /* Descriptor Code: Formatted Media */
        BulkBuf[ 9] = (mBlockSize >> 16) & 0xFF;
        BulkBuf[10] = (mBlockSize >>  8) & 0xFF;
        BulkBuf[11] = (mBlockSize >>  0) & 0xFF;
      
        BulkLen = 12;
        DataInTransfer();
        }
      break;
    case MSC_DEVQR_READ_CAPASITY :
      if( result == CMTE_OK ) {
        uint32 MSC_BlockCount = mQuery.mBlockCount;
        mBlockSize  = mQuery.mBlockStart;
        
        if( mBuf == 0 ) mBuf = new uint8[mBlockSize * mBufBlockCount];
        
        /* Last Logical Block */
        BulkBuf[ 0] = ((MSC_BlockCount - 1) >> 24) & 0xFF;
        BulkBuf[ 1] = ((MSC_BlockCount - 1) >> 16) & 0xFF;
        BulkBuf[ 2] = ((MSC_BlockCount - 1) >>  8) & 0xFF;
        BulkBuf[ 3] = ((MSC_BlockCount - 1) >>  0) & 0xFF;
      
        /* Block Length */
        BulkBuf[ 4] = (mBlockSize >> 24) & 0xFF;
        BulkBuf[ 5] = (mBlockSize >> 16) & 0xFF;
        BulkBuf[ 6] = (mBlockSize >>  8) & 0xFF;
        BulkBuf[ 7] = (mBlockSize >>  0) & 0xFF;
      
        BulkLen = 8;
        DataInTransfer();
        }
      break;
    }
  OTGD_FS_EnableGlobalInt();
  }

#define WBVAL(x) (x & 0xFF),((x >> 8) & 0xFF)

/* USB Standard Device Descriptor */
const uint8 MscDeviceDescriptor[] =
  {
    0x12,   /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    0x00,
    0x02,   /* bcdUSB = 2.00 */
    0x00,   /* bDeviceClass: MSC */
    0x00,   /* bDeviceSubClass */
    0x00,   /* bDeviceProtocol */
    USB_MAX_PACKET0,           /* bMaxPacketSize0 */
    WBVAL(0xC251),             /* idVendor = 0xC251 */
    WBVAL(0x1C03),             /* idProduct = 0x1C03 */
    0x00,
    0x02,   /* bcdDevice = 2.00 */
    1,      /* Index of string descriptor describing manufacturer */
    2,      /* Index of string descriptor describing product */
    3,      /* Index of string descriptor describing the device's serial number */
    0x01    /* bNumConfigurations */
  };

#define MSC_SIZ_CONFIG_DESC        32

const uint8 MscConfigDescriptor[] = {

  /*Configuration Descriptor*/
    0x09,   /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
    MSC_SIZ_CONFIG_DESC,       /* wTotalLength:no of returned bytes */
    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: Configuration value */
    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
    0xC0,   /* bmAttributes: self powered */
    0x32,   /* MaxPower 0 mA */
  
  /*Interface Descriptor*/
    0x09,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: 2 endpoints used */
    USB_DEVICE_CLASS_STORAGE,   /* bInterfaceClass: Mass Storage Class */
    MSC_SUBCLASS_SCSI,          /* bInterfaceSubClass: SCSI */
    MSC_PROTOCOL_BULK_ONLY,     /* bInterfaceProtocol: BULK ONLY */
    0x00,   /* iInterface: */

  /*Endpoint 3 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x03,   /* bEndpointAddress: (OUT3) */
    0x02,   /* bmAttributes: Bulk */
    64,     /* wMaxPacketSize: */
    0x00,
    0x00,   /* bInterval: ignore for Bulk transfer */
  
  /*Endpoint 1 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x81,   /* bEndpointAddress: (IN1) */
    0x02,   /* bmAttributes: Bulk */
    64,     /* wMaxPacketSize: */
    0x00,
    0x00,    /* bInterval */
    0x00     //stop
  };

/* USB String Descriptors */
const uint8 MscStringLangID[4] =
  {
    4,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04 /* LangID = 0x0409: U.S. English */
  };

const uint8 MscStringVendor[24] =
  {
    24,     /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType*/
    /* Manufacturer: "RDM-contact" */
    'R', 0, 'D', 0, 'M', 0, '-', 0, 'c', 0, 'o', 0, 'n', 0, 't', 0,
    'a', 0, 'c', 0, 't', 0
  };

const uint8 MscStringProduct[32] =
  {
    32,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    /* Product name: "RDM-C Bulk" */
    'R', 0, 'D', 0, 'M', 0, '-', 0, 'C', 0, ' ', 0, 'N', 0, 'a', 0,
    'v', 0, 'i', 0, 'g', 0, 'a', 0, 't', 0, 'o', 0, 'r', 0
  };

uint8 MscStringSerial[16] =
  {
    16,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    'R', 0, 'D', 0, 'M', 0, '-', 0, 'C', 0, '0', 0, '1', 0
  };

uint8* 
CmtUsbDevMsc::GetDeviceDescriptor() {
  return (uint8*)MscDeviceDescriptor;
  }

uint8* 
CmtUsbDevMsc::GetConfigDescriptor( uint_8 configIndex ) {
  return (uint8*)MscConfigDescriptor;
  }

uint8* 
CmtUsbDevMsc::GetStringDescriptor( uint_8 stringIndex ) {
  switch( stringIndex ) {
    case 1 : return (uint8*)MscStringVendor;
    case 2 : return (uint8*)MscStringProduct;
    case 3 : return (uint8*)MscStringSerial;
    }
  return (uint8*)MscStringLangID;
  }


#ifdef CMT_USB_DEV_MASS_MEM

#define MSC_ImageSize   0x00002000

const unsigned char DiskImage[MSC_ImageSize] = {
0xEB,0x3C,0x90,0x4D,0x53,0x44,0x4F,0x53,0x35,0x2E,0x30,0x00,0x02,0x01,0x01,0x00,
0x01,0x10,0x00,0x40,0x00,0xF8,0x02,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x74,0x19,0x02,0x27,0x53,0x54,0x4D,0x33,0x32,
0x78,0x5F,0x55,0x53,0x42,0x20,0x46,0x41,0x54,0x31,0x32,0x20,0x20,0x20,0x33,0xC9,
0x8E,0xD1,0xBC,0xF0,0x7B,0x8E,0xD9,0xB8,0x00,0x20,0x8E,0xC0,0xFC,0xBD,0x00,0x7C,
0x38,0x4E,0x24,0x7D,0x24,0x8B,0xC1,0x99,0xE8,0x3C,0x01,0x72,0x1C,0x83,0xEB,0x3A,
0x66,0xA1,0x1C,0x7C,0x26,0x66,0x3B,0x07,0x26,0x8A,0x57,0xFC,0x75,0x06,0x80,0xCA,
0x02,0x88,0x56,0x02,0x80,0xC3,0x10,0x73,0xEB,0x33,0xC9,0x8A,0x46,0x10,0x98,0xF7,
0x66,0x16,0x03,0x46,0x1C,0x13,0x56,0x1E,0x03,0x46,0x0E,0x13,0xD1,0x8B,0x76,0x11,
0x60,0x89,0x46,0xFC,0x89,0x56,0xFE,0xB8,0x20,0x00,0xF7,0xE6,0x8B,0x5E,0x0B,0x03,
0xC3,0x48,0xF7,0xF3,0x01,0x46,0xFC,0x11,0x4E,0xFE,0x61,0xBF,0x00,0x00,0xE8,0xE6,
0x00,0x72,0x39,0x26,0x38,0x2D,0x74,0x17,0x60,0xB1,0x0B,0xBE,0xA1,0x7D,0xF3,0xA6,
0x61,0x74,0x32,0x4E,0x74,0x09,0x83,0xC7,0x20,0x3B,0xFB,0x72,0xE6,0xEB,0xDC,0xA0,
0xFB,0x7D,0xB4,0x7D,0x8B,0xF0,0xAC,0x98,0x40,0x74,0x0C,0x48,0x74,0x13,0xB4,0x0E,
0xBB,0x07,0x00,0xCD,0x10,0xEB,0xEF,0xA0,0xFD,0x7D,0xEB,0xE6,0xA0,0xFC,0x7D,0xEB,
0xE1,0xCD,0x16,0xCD,0x19,0x26,0x8B,0x55,0x1A,0x52,0xB0,0x01,0xBB,0x00,0x00,0xE8,
0x3B,0x00,0x72,0xE8,0x5B,0x8A,0x56,0x24,0xBE,0x0B,0x7C,0x8B,0xFC,0xC7,0x46,0xF0,
0x3D,0x7D,0xC7,0x46,0xF4,0x29,0x7D,0x8C,0xD9,0x89,0x4E,0xF2,0x89,0x4E,0xF6,0xC6,
0x06,0x96,0x7D,0xCB,0xEA,0x03,0x00,0x00,0x20,0x0F,0xB6,0xC8,0x66,0x8B,0x46,0xF8,
0x66,0x03,0x46,0x1C,0x66,0x8B,0xD0,0x66,0xC1,0xEA,0x10,0xEB,0x5E,0x0F,0xB6,0xC8,
0x4A,0x4A,0x8A,0x46,0x0D,0x32,0xE4,0xF7,0xE2,0x03,0x46,0xFC,0x13,0x56,0xFE,0xEB,
0x4A,0x52,0x50,0x06,0x53,0x6A,0x01,0x6A,0x10,0x91,0x8B,0x46,0x18,0x96,0x92,0x33,
0xD2,0xF7,0xF6,0x91,0xF7,0xF6,0x42,0x87,0xCA,0xF7,0x76,0x1A,0x8A,0xF2,0x8A,0xE8,
0xC0,0xCC,0x02,0x0A,0xCC,0xB8,0x01,0x02,0x80,0x7E,0x02,0x0E,0x75,0x04,0xB4,0x42,
0x8B,0xF4,0x8A,0x56,0x24,0xCD,0x13,0x61,0x61,0x72,0x0B,0x40,0x75,0x01,0x42,0x03,
0x5E,0x0B,0x49,0x75,0x06,0xF8,0xC3,0x41,0xBB,0x00,0x00,0x60,0x66,0x6A,0x00,0xEB,
0xB0,0x4E,0x54,0x4C,0x44,0x52,0x20,0x20,0x20,0x20,0x20,0x20,0x0D,0x0A,0x52,0x65,
0x6D,0x6F,0x76,0x65,0x20,0x64,0x69,0x73,0x6B,0x73,0x20,0x6F,0x72,0x20,0x6F,0x74,
0x68,0x65,0x72,0x20,0x6D,0x65,0x64,0x69,0x61,0x2E,0xFF,0x0D,0x0A,0x44,0x69,0x73,
0x6B,0x20,0x65,0x72,0x72,0x6F,0x72,0xFF,0x0D,0x0A,0x50,0x72,0x65,0x73,0x73,0x20,
0x61,0x6E,0x79,0x20,0x6B,0x65,0x79,0x20,0x74,0x6F,0x20,0x72,0x65,0x73,0x74,0x61,
0x72,0x74,0x0D,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xAC,0xCB,0xD8,0x55,0xAA,
0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
'R','D','M','-','C',0x20,0x20,0x20,0x20,0x20,0x20,0x28,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xE5,0x45,0x41,0x44,0x4D,0x45,0x20,0x20,0x54,0x58,0x54,0x20,0x00,0x00,0x00,0x00,
0x21,0x00,0xBB,0x32,0x00,0x00,0xDC,0x83,0xBB,0x32,0x02,0x00,0x57,0x00,0x00,0x00,
0xE5,0x52,0x00,0x65,0x00,0x61,0x00,0x64,0x00,0x4D,0x00,0x0F,0x00,0x73,0x65,0x00,
0x2E,0x00,0x74,0x00,0x78,0x00,0x74,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
0xE5,0x45,0x41,0x44,0x4D,0x45,0x20,0x20,0x54,0x58,0x54,0x20,0x00,0x87,0x99,0x71,
0x45,0x37,0x45,0x37,0x00,0x00,0x4F,0x68,0x45,0x37,0x02,0x00,0x5E,0x00,0x00,0x00,
0x41,0x52,0x00,0x65,0x00,0x61,0x00,0x64,0x00,0x4D,0x00,0x0F,0x00,0x73,0x65,0x00,
0x2E,0x00,0x74,0x00,0x78,0x00,0x74,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
0x52,0x45,0x41,0x44,0x4D,0x45,0x20,0x20,0x54,0x58,0x54,0x20,0x00,0x87,0x99,0x71,
0x45,0x37,0x4C,0x37,0x00,0x00,0x32,0x4D,0x4C,0x37,0x02,0x00,0x5F,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
'A','l','l','-',0x20,'O','K',0x20,0x61,0x20,0x55,0x53,0x42,0x20,0x4D,0x65,
0x6D,0x6F,0x72,0x79,0x20,0x44,0x65,0x76,0x69,0x63,0x65,0x20,0x64,0x65,0x6D,0x6F,
0x6E,0x73,0x74,0x72,0x61,0x74,0x69,0x6F,0x6E,0x20,0x66,0x6F,0x72,0x0D,0x0A,0x74,
0x68,0x65,0x20,0x4B,0x65,0x69,0x6C,0x20,0x4D,0x43,0x42,0x53,0x54,0x4D,0x33,0x32,
0x20,0x42,0x6F,0x61,0x72,0x64,0x20,0x77,0x69,0x74,0x68,0x20,0x53,0x54,0x20,0x53,
0x54,0x4D,0x33,0x32,0x46,0x31,0x30,0x33,0x52,0x42,0x54,0x36,0x2E,0x0D,0x0A,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

void
cmtUsbMscMemTask() {
  CmtUsbDevMsc *usb = (CmtUsbDevMsc*)(cmtTaskParam());
  uint_8 result;
  while(1) {
    //������� ������
    cmtWaitSetUInt32( &(usb->mQuery.mQueryCode) );
    switch( usb->mQuery.mQueryCode ) {
      case 10 :
        result = CMTE_OK;
        break;
      case MSC_DEVQR_VERIFY :
        //��������� �������� ������ ��� ���������
      case MSC_DEVQR_READ :
        //��������� �������� ������ � mQuery.mBuf
        //��������� ���� mQuery.mBlockStart
        //���������� ������ mQuery.mBlockCount
        if( (usb->mQuery.mBlockStart + usb->mQuery.mBlockCount) * 512 <= MSC_ImageSize ) {
          memcpy( usb->mQuery.mBuf, DiskImage + usb->mQuery.mBlockStart * 512, usb->mQuery.mBlockCount * 512 );
          result = CMTE_OK;
          }
        else result = CMTE_FAIL;
        break;
      case MSC_DEVQR_WRITE :
        //��������� �������� ������ �� mQuery.mBuf
        //��������� ���� mQuery.mBlockStart
        //���������� ������ mQuery.mBlockCount
        result = CMTE_FAIL;
        break;
      case MSC_DEVQR_READ_CAPASITY :
      case MSC_DEVQR_FORMAT_CAPASITY :
        //��������� ��������� mQuery ����������� ����������
        usb->mQuery.mBlockStart = 512; //������ �����
        usb->mQuery.mBlockCount = MSC_ImageSize / 512;
        result = CMTE_OK;
        break;
      default :
        result = CMTE_FAIL;
      }
    //���������� � ���������� ��������
    usb->QueryComplete( result );
    }
  }
#endif //CMT_USB_DEV_MASS_MEM
