//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_USB_MSC
#define CMT_USB_MSC

/* MSC Subclass Codes */
#define MSC_SUBCLASS_RBC                0x01
#define MSC_SUBCLASS_SFF8020I_MMC2      0x02
#define MSC_SUBCLASS_QIC157             0x03
#define MSC_SUBCLASS_UFI                0x04
#define MSC_SUBCLASS_SFF8070I           0x05
#define MSC_SUBCLASS_SCSI               0x06

/* MSC Protocol Codes */
#define MSC_PROTOCOL_CBI_INT            0x00
#define MSC_PROTOCOL_CBI_NOINT          0x01
#define MSC_PROTOCOL_BULK_ONLY          0x50


/* MSC Request Codes */
#define MSC_REQUEST_RESET               0xFF
#define MSC_REQUEST_GET_MAX_LUN         0xFE

/* Bulk-only Command Block Wrapper */
#pragma pack(1)
typedef struct _MSC_CBW {
  uint32 dSignature;
  uint32 dTag;
  uint32 dDataLength;
  uint8  bmFlags;
  uint8  bLUN;
  uint8  bCBLength;
  uint8  CB[16];
} MSC_CBW;

/*
CB[0] - команда
CB[1]
CB[2-5] - начальный сектор (старшими вперед)
CB[7-8] - количество секторов
*/

/* Bulk-only Command Status Wrapper */
#pragma pack(1)
typedef struct _MSC_CSW {
  uint32 dSignature;
  uint32 dTag;
  uint32 dDataResidue;
  uint8  bStatus;
} MSC_CSW;

#pragma pack()

#define MSC_CBW_Signature               0x43425355
#define MSC_CSW_Signature               0x53425355


/* CSW Status Definitions */
#define CSW_CMD_PASSED                  0x00
#define CSW_CMD_FAILED                  0x01
#define CSW_PHASE_ERROR                 0x02


/* SCSI Commands */
#define SCSI_TEST_UNIT_READY            0x00
#define SCSI_REQUEST_SENSE              0x03
#define SCSI_FORMAT_UNIT                0x04
#define SCSI_INQUIRY                    0x12
#define SCSI_MODE_SELECT6               0x15
#define SCSI_MODE_SENSE6                0x1A
#define SCSI_START_STOP_UNIT            0x1B
#define SCSI_MEDIA_REMOVAL              0x1E
#define SCSI_READ_FORMAT_CAPACITIES     0x23
#define SCSI_READ_CAPACITY              0x25
#define SCSI_READ10                     0x28
#define SCSI_WRITE10                    0x2A
#define SCSI_VERIFY10                   0x2F
#define SCSI_MODE_SELECT10              0x55
#define SCSI_MODE_SENSE10               0x5A

#endif
