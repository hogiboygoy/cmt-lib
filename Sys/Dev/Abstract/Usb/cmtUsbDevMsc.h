//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <sys/dev/abstract/usb/cmtUsbMsc.h>

#define MSC_MAX_PACKET 64

/* MSC Bulk-only Stage */
#define MSC_BS_CBW                      0       /* Command Block Wrapper */
#define MSC_BS_DATA_OUT                 1       /* Data Out Phase */
#define MSC_BS_DATA_IN                  2       /* Data In Phase */
#define MSC_BS_DATA_IN_LAST             3       /* Data In Last Phase */
#define MSC_BS_DATA_IN_LAST_STALL       4       /* Data In Last Phase with Stall */
#define MSC_BS_CSW                      5       /* Command Status Wrapper */
#define MSC_BS_ERROR                    6       /* Error */



/* MSC In/Out Endpoint Address */
#define MSC_EP_IN       0x81
#define MSC_EP_OUT      0x03

struct CmtUsbDevMsc;

#define MSC_DEVQR_READ              1  //! ��������� �����
#define MSC_DEVQR_WRITE             2  //! �������� �����
#define MSC_DEVQR_VERIFY            3  //! ��������� ����� ��� ���������
#define MSC_DEVQR_FORMAT_CAPASITY   4  //! �������� ���������� �� ���������� (mBlockStart �������� ������ ����� ����������)
#define MSC_DEVQR_READ_CAPASITY     5  //! �������� ���������� �� ���������� (mBlockStart �������� ������ ����� ����������)

//! ��������� �������� �� ���������� ��� �������� ���������
struct CmtUsbDevMscQuery {
  uint32        mQueryCode;   //!< ��� �������
  uint32        mDevice;      //!< ����� ����������
  uint8        *mBuf;         //!< ��������� �� ����� ������
  uint32        mBlockStart;  //!< ����� ���������� ����� (�� ������� Info ���������� ������ �����)
  uint32        mBlockCount;  //!< ���������� ������
  };


//! ���������� �������� ������
struct CmtUsbDevMsc : public CmtUsbDev {
  uint8             mBulkStage;              //!< ��������� �������� ������
  MSC_CBW           CBW;                     //!< Command Block Wrapper
  MSC_CSW           CSW;                     //!< Command Status Wrapper
  uint8             BulkBuf[MSC_MAX_PACKET]; //!< Bulk In/Out Buffer
  uint8             BulkLen;                 //!< Bulk In/Out Length
  uint32            mBlockSize;              //!< ������ ����� ������ ����������

  uint8            *mBuf;                    //!< ����� ��� �����
  uint32            mBufBlockCount;          //!< ���������� ������ ������������ � ����� (������� ��������)
  uint32            mBlockStart;             //!< ����� ����� ��� ������
  uint32            mBlockCount;             //!< ���������� ������ ��� ������
  bool              mMemOk;                  //!< ���� ��������
  
  uint8            *mTransferBuf;
  uint32            mTransferLen;
  CmtUsbDevMscQuery mQuery;
  volatile bool     mTransferInt;
  volatile bool     mReceivInt;
  
  CmtUsbDevMsc( uint32 bufBlockCount );
  ~CmtUsbDevMsc();
  
  virtual uint8* GetDeviceDescriptor();
  virtual uint8* GetConfigDescriptor( uint_8 configIndex );
  virtual uint8* GetStringDescriptor( uint_8 stringIndex );

  //! �� ������ (IN)
  virtual void EndPoint1( uint32 ev );
  
  //! �� ����� (OUT)
  virtual void EndPoint3( uint32 ev );
  
  //! ���������� ��������
  /*! ���������� ������� ������� ��� ������������ ���������� �����������
    ��������.
    \param result ��������� ���������� �������� */
  void QueryComplete( uint_8 result );

protected:
  /* MSC Requests Callback Functions */
  bool MSC_Reset();
  bool MSC_GetMaxLUN();
  
  /* MSC Bulk Callback Functions */
  void MSC_GetCBW();             //!< MSC Get Command Block Wrapper Callback
  void MSC_SetCSW();             //!< MSC Set Command Status Wrapper Callback
  
  /* MSC SCSI Callback Functions */
  void MSC_ReadFormatCapacity(); //!< MSC SCSI Read Format Capacity Callback
  void MSC_ReadCapacity();       //!< MSC SCSI Read Capacity Callback
  void MSC_ModeSense10();        //!< MSC SCSI Mode Sense (10-Byte) Callback
  void MSC_ModeSense6();         //!< MSC SCSI Mode Sense (6-Byte) Callback
  void MSC_Inquiry();            //!< MSC SCSI Inquiry Callback
  void MSC_RequestSense();       //!< MSC SCSI Request Sense Callback
  void MSC_TestUnitReady();      //!< MSC SCSI Test Unit Ready Callback
  void DataInTransfer();         //!< Perform Data IN Transfer
  bool DataInFormat();           //!< Check Data IN Format \return TRUE - Success, FALSE - Error
  bool MSC_RWSetup();            //!< MSC SCSI Read/Write Setup Callback
  void MSC_MemoryVerify();       //!< MSC Memory Verify Callback
  void MSC_MemoryRead();         //!< MSC Memory Read Callback
  void MSC_MemoryWrite();        //!< MSC Memory Write Callback
  void MSC_QueryRead();          //!< ������� ������ �� ������
  };

