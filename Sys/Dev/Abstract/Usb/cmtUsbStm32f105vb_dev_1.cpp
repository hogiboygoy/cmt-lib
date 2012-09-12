//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

/*******************************************************************************
 * USB_OTG Core registers .
 * The USB_OTG_USB_OTG_FS_REGS structure defines the size
 * and relative field offsets for the Core Global registers.
 ******************************************************************************/
typedef struct _USB_OTG_GREGS  //000h
{

  __IO uint32_t GOTGCTL;      /* USB_OTG Control and Status reg     000h*/
  __IO uint32_t GOTGINT;      /* USB_OTG Interrupt Register         004h*/
  __IO uint32_t GAHBCFG;      /* Core AHB Configuration Register    008h*/
  __IO uint32_t GUSBCFG;      /* Core USB Configuration Register    00Ch*/
  __IO uint32_t GRSTCTL;      /* Core Reset Register                010h*/
  __IO uint32_t GINTSTS;      /* Core Interrupt Register            014h*/
  __IO uint32_t GINTMSK;      /* Core Interrupt Mask Register       018h*/
  __IO uint32_t GRXSTSR;      /* Receive Sts Q Read Register        01Ch*/
  __IO uint32_t GRXSTSP;      /* Receive Sts Q Read & POP Register  020h*/
  __IO uint32_t GRXFSIZ;      /* Receive FIFO Size Register         024h*/
  __IO uint32_t DIEPTXF0;      /* EP0 Tx FIFO Size Register          028h*/
  __IO uint32_t HNPTXSTS;     /* Non Periodic Tx FIFO/Queue Sts reg 02Ch*/
  uint32_t Reserved30[2];     /* Reserved                           030h*/
  __IO uint32_t GCCFG;        /* General Core configuration reg     038h*/
  __IO uint32_t CID;          /* User ID Register                   03Ch*/
  uint32_t  reserved[48];          /* Reserved                 040h-0FFh*/
  __IO uint32_t HPTXFSIZ;     /* Host Periodic Tx FIFO Size Reg     100h*/
  __IO uint32_t DIEPTXFx[NUM_TX_FIFOS - 1];        /* dev Transmit FIFO */

}
USB_OTG_GREGS;

/*******************************************************************************
 * dev Registers
 * dev Global Registers : Offsets 800h-BFFh
 * The following structures define the size and relative field offsets
 * for the dev Mode Registers.
 * These registers are visible only in dev mode and must not be
 * accessed in Host mode, as the results are unknown
 ******************************************************************************/
typedef struct _USB_OTG_DEV // 800h
{

  __IO uint32_t DCFG;           /* dev Configuration Register   800h*/
  __IO uint32_t DCTL;           /* dev Control Register         804h*/
  __IO uint32_t DSTS;           /* dev Status Register (RO)     808h*/
  uint32_t reservedC;           /* Reserved                     80Ch*/
  __IO uint32_t DIEPMSK;        /* dev IN Endpoint Mask         810h*/
  __IO uint32_t DOEPMSK;        /* dev OUT Endpoint Mask        814h*/
  __IO uint32_t DAINT;          /* dev All Endpoints Itr Reg    818h*/
  __IO uint32_t DAINTMSK;       /* dev All Endpoints Itr Mask   81Ch*/
  uint32_t  Reserved20[2];      /* Reserved                820h-824h*/
  __IO uint32_t DVBUSDIS;       /* dev VBUS discharge Register  828h*/
  __IO uint32_t DVBUSPULSE;     /* dev VBUS Pulse Register      82Ch*/
  __IO uint32_t Reserved30;     /* Reserved                     830h*/
  __IO uint32_t DIEPEMPMSK;     /*  IN EP FIFO empty int mask   834h*/

}
USB_OTG_DEV;
/*******************************************************************************
 * dev Logical IN Endpoint-Specific Registers: Offsets 900h-AFCh
 * There will be one set of endpoint registers per logical endpoint implemented.
 * These registers are visible only in dev mode and must not be
 * accessed in Host mode, as the results are unknown
*******************************************************************************/
typedef struct _USB_OTG_DINEPS
{
  __IO uint32_t DIEPCTLx;     /* dev IN EP Ctl Reg 900h + (ep_num * 20h) + 00h*/
  uint32_t reserved04;        /* Reserved          900h + (ep_num * 20h) + 04h*/
  __IO uint32_t DIEPINTx;     /* dev IN EP Itr Reg 900h + (ep_num * 20h) + 08h*/
  uint32_t reserved0C;        /* Reserved          900h + (ep_num * 20h) + 0Ch*/
  __IO uint32_t DIEPTSIZx;    /* dev IN EP Tx Size 900h + (ep_num * 20h) + 10h*/
  uint32_t reserved14;        /* Reserved          900h + (ep_num * 20h) + 14h*/
  __IO uint32_t DTXFSTSx;     /* IN EP TxFIFO Sts  900h + (ep_num * 20h) + 18h*/
  uint32_t reserved18;        /* Reserved          900h + (ep_num * 20h) + 1Ch*/

}
USB_OTG_DINEPS;

/*******************************************************************************
 * dev Logical OUT Endpoint-Specific Registers Offsets: B00h-CFCh
 * There will be one set of endpoint registers per logical endpoint
 * implemented.
 * These registers are visible only in dev mode and must not be
 * accessed in Host mode, as the results are unknown
******************************************************************************/
typedef struct _USB_OTG_DOUTEPS
{
  __IO uint32_t DOEPCTLx;        /* OUT EP Ctl Reg B00h + (ep_num * 20h) + 00h*/
  uint32_t reserved04;           /* Reserved       B00h + (ep_num * 20h) + 04h*/
  __IO uint32_t DOEPINTx;        /* OUT EP Itr Reg B00h + (ep_num * 20h) + 08h*/
  uint32_t reserved0C;           /* Reserved       B00h + (ep_num * 20h) + 0Ch*/
  __IO uint32_t DOEPTSIZx;       /* OUT EP Tx Size B00h + (ep_num * 20h) + 10h*/
  __IO uint32_t Reserved14[3];   /* Reserved       B00h + (ep_num * 20h) + 14h*/
}
USB_OTG_DOUTEPS;



/*******************************************************************************
 * otg Core registers .
 * The USB_OTG_USB_OTG_FS_REGS structure defines the size
 * and relative field offsets for the Core Global registers.
 ******************************************************************************/
typedef struct USB_OTG_USB_OTG_FS_REGS  //000h
{
  USB_OTG_GREGS         *GREGS;
  USB_OTG_DEV           *DEV;
  USB_OTG_DINEPS        *DINEPS[NUM_TX_FIFOS];
  USB_OTG_DOUTEPS       *DOUTEPS[NUM_TX_FIFOS];
  __IO uint32_t         *FIFO[NUM_TX_FIFOS];
  __IO uint32_t         *PCGCCTL;

}
USB_OTG_CORE_REGS , *pUSB_OTG_CORE_REGS;
/******************************************************************************/
typedef union _USB_OTG_GAHBCFG_TypeDef
{
  uint32_t d32;
  struct
  {
uint32_t gintmsk :
    1;
uint32_t reserved1 :
    6;
uint32_t txfemplvl :
    1;
uint32_t reserved8_31 :
    24;
  }
  b;
} 
USB_OTG_GAHBCFG_TypeDef;
/******************************************************************************/

typedef union _USB_OTG_GUSBCFG_TypeDef
{
  uint32_t d32;
  struct
  {
uint32_t toutcal :
    3;
uint32_t Reserved3_5 :
    3;
uint32_t physel : /* MUST be always 1 because the phy is embedded*/
    1;
uint32_t Reserved7 :
    1;
uint32_t srpcap :
    1;
uint32_t hnpcap :
    1;
uint32_t usbtrdtim :
    4;
uint32_t reserved15_30 :
    15;
uint32_t force_host :
    1;
uint32_t force_dev :
    1;
uint32_t corrupt_tx :
    1;
  }
  b;
} USB_OTG_GUSBCFG_TypeDef;
/******************************************************************************/

typedef union _USB_OTG_GRSTCTL_TypeDef
{
  uint32_t d32;
  struct
  {
uint32_t csftrst :
    1;
uint32_t hsftrst :
    1;
uint32_t hstfrm :
    1;
uint32_t reserved3 :
    1;
uint32_t rxfflsh :
    1;
uint32_t txfflsh :
    1;
uint32_t txfnum :
    5;
uint32_t reserved11_30 :
    20;
uint32_t ahbidle :
    1;
  }
  b;
} USB_OTG_GRSTCTL_TypeDef;

/******************************************************************************/
typedef union _USB_OTG_GINTMSK_TypeDef
{
  uint32_t d32;
  struct
  {
uint32_t reserved0 :
    1;
uint32_t modemismatch :
    1;
uint32_t otgintr :
    1;
uint32_t sofintr :
    1;
uint32_t rxstsqlvl :
    1;
uint32_t reserved5 :
    1;
uint32_t ginnakeff :
    1;
uint32_t goutnakeff :
    1;
uint32_t reserved8_9 :
    2;
uint32_t erlysuspend :
    1;
uint32_t usbsuspend :
    1;
uint32_t usbreset :
    1;
uint32_t enumdone :
    1;
uint32_t isooutdrop :
    1;
uint32_t eopframe :
    1;
uint32_t reserved16 :
    1;
uint32_t epmismatch :
    1;
uint32_t inepintr :
    1;
uint32_t outepintr :
    1;
uint32_t incomplisoin :
    1;
uint32_t incomplisoout :
    1;
uint32_t reserved22_23 :
    2;
uint32_t portintr :
    1;
uint32_t hcintr :
    1;
uint32_t ptxfempty :
    1;
uint32_t reserved27 :
    1;
uint32_t conidstschng :
    1;
uint32_t disconnect :
    1;
uint32_t sessreqintr :
    1;
uint32_t wkupintr :
    1;
  }
  b;
} USB_OTG_GINTMSK_TypeDef;

/******************************************************************************/


typedef union _USB_OTG_GINTSTS_TypeDef
{
  uint32_t d32;
  struct
  {
uint32_t curmode :
    1;
uint32_t modemismatch :
    1;
uint32_t otgintr :
    1;
uint32_t sofintr :
    1;
uint32_t rxstsqlvl :
    1;
uint32_t reserved5 :
    1;
uint32_t ginnakeff :
    1;
uint32_t goutnakeff :
    1;
uint32_t reserved8_9 :
    2;
uint32_t erlysuspend :
    1;
uint32_t usbsuspend :
    1;
uint32_t usbreset :
    1;
uint32_t enumdone :
    1;
uint32_t isooutdrop :
    1;
uint32_t eopframe :
    1;
uint32_t Reserved16_17 :
    2;
uint32_t inepint:
    1;
uint32_t outepintr :
    1;
uint32_t incomplisoin :
    1;
uint32_t incomplisoout :
    1;
uint32_t reserved22_23 :
    2;
uint32_t portintr :
    1;
uint32_t hcintr :
    1;
uint32_t ptxfempty :
    1;
uint32_t reserved27 :
    1;
uint32_t conidstschng :
    1;
uint32_t disconnect :
    1;
uint32_t sessreqintr :
    1;
uint32_t wkupintr :
    1;
  }
  b;
} USB_OTG_GINTSTS_TypeDef;

/******************************************************************************/
typedef union _USB_OTG_GRXSTSP_TypeDef
{

  uint32_t d32;
  struct
  {
uint32_t epnum :
    4;
uint32_t bcnt :
    11;
uint32_t dpid :
    2;
uint32_t pktsts :
    4;
uint32_t frmnum :
    4;
uint32_t reserved :
    7;
  }
  b;
} USB_OTG_GRXSTSP_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_FIFOSIZ_TypeDef
{
  uint32_t d32;
  struct
  {
uint32_t startaddr :
    16;
uint32_t depth :
    16;
  }
  b;
} USB_OTG_FIFOSIZ_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_DTXFSTS_TypeDef
{
  uint32_t d32;
  struct
  {
uint32_t txfspcavail :
    16;
uint32_t reserved :
    16;
  }
  b;
} USB_OTG_DTXFSTS_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_GCCFG_TypeDef
{

  uint32_t d32;
  struct
  {

uint32_t reserved0 :
    16;
uint32_t pwdn :
    1;
uint32_t reserved17 :
    1;
uint32_t vbussensingA :
    1;
uint32_t vbussensingB :
    1;
uint32_t SOFouten :
    1;
uint32_t reserved21 :
    11;
  }
  b;
} USB_OTG_GCCFG_TypeDef;


/******************************************************************************/
typedef union _USB_OTG_DCFG_TypeDef
{

  uint32_t d32;
  struct
  {
uint32_t devspd :
    2;
uint32_t nzstsouthshk :
    1;
uint32_t reserved3 :
    1;
uint32_t devaddr :
    7;
uint32_t perfrint :
    2;
uint32_t reserved13_31 :
    19;
  }
  b;
} USB_OTG_DCFG_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_DCTL_TypeDef
{

  uint32_t d32;
  struct
  {
uint32_t rmtwkupsig :
    1;
uint32_t sftdiscon :
    1;
uint32_t gnpinnaksts :
    1;
uint32_t goutnaksts :
    1;
uint32_t tstctl :
    3;
uint32_t sgnpinnak :
    1;
uint32_t cgnpinnak :
    1;
uint32_t sgoutnak :
    1;
uint32_t cgoutnak :
    1;
uint32_t pwronprgdone :
    1;  
uint32_t reserved :
    20;
  }
  b;
} USB_OTG_DCTL_TypeDef;

/******************************************************************************/
typedef union _USB_OTG_DSTS_TypeDef
{

  uint32_t d32;
  struct
  {
uint32_t suspsts :
    1;
uint32_t enumspd :
    2;
uint32_t errticerr :
    1;
uint32_t reserved4_7:
    4;
uint32_t soffn :
    14;
uint32_t reserved22_31 :
    10;
  }
  b;
} USB_OTG_DSTS_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_DIEPINTx_TypeDef
{

  uint32_t d32;
  struct
  {
uint32_t xfercompl :
    1;
uint32_t epdis :
    1;
uint32_t Reserved2 :
    1;
uint32_t timeout :
    1;
uint32_t intktxfemp :
    1;
uint32_t reserved5 :
    1;
uint32_t inepnakeff :
    1;
uint32_t txfempty :
    1;
uint32_t reserved08_31 :
    24;
  }
  b;
} USB_OTG_DIEPINTx_TypeDef;


typedef union _USB_OTG_DIEPINTx_TypeDef  USB_OTG_DIEPMSKx_TypeDef;
/******************************************************************************/

typedef union _USB_OTG_DOEPINTx_TypeDef
{

  uint32_t d32;
  struct
  {
uint32_t xfercompl :
    1;
uint32_t epdis :
    1;
uint32_t reserved2 :
    1;
uint32_t setup :   /* for EP0 only */
    1;
uint32_t outtokenepdis :
    1;
uint32_t reserved5 :
    1;   
uint32_t b2bsetup :
    1;      
uint32_t reserved07_31 :
    25;    
  }
  b;
} USB_OTG_DOEPINTx_TypeDef;


typedef union _USB_OTG_DOEPINTx_TypeDef  USB_OTG_DOEPMSKx_TypeDef;

/******************************************************************************/
typedef union _USB_OTG_DAINT_TypeDef
{

  uint32_t d32;
  struct
  {
uint32_t in :
    16;
uint32_t out :
    16;
  }
  ep;
} USB_OTG_DAINT_TypeDef;

/******************************************************************************/
typedef union _USB_OTG_DEPCTLx_TypeDef
{

  uint32_t d32;
  struct
  {
uint32_t mps :
    11;
uint32_t Reserved11_14 :
    4;
uint32_t usbactep :
    1;
uint32_t dpid :
    1;
uint32_t naksts :
    1;
uint32_t eptype :
    2;
uint32_t Reserved20 :
    1;
uint32_t stall :
    1;
uint32_t txfnum :
    4;
uint32_t cnak :
    1;
uint32_t snak :
    1;
uint32_t setd0pid :
    1;
uint32_t setoddfrm :
    1;
uint32_t epdis :
    1;
uint32_t epena :
    1;
  }
  b;
} USB_OTG_DEPCTLx_TypeDef;

/******************************************************************************/

typedef union _OTG_FS_DEPTSIZx_TypeDef
{

  uint32_t d32;
  struct
  {
uint32_t xfersize :
    19;
uint32_t pktcnt :
    10;
uint32_t mcount :
    2;
uint32_t reserved :
    1;
  }
  b;
} 
OTG_FS_DEPTSIZx_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_DOEPTSIZ0_TypeDef
{

  uint32_t d32;
  struct
  {
uint32_t xfersize :
    7;
uint32_t reserved7_18 :
    12;
uint32_t pktcnt :
    1;
uint32_t reserved20_28 :
    9;
uint32_t supcnt :
    2;
    uint32_t reserved31;
  }
  b;
} USB_OTG_DOEPTSIZ0_TypeDef;

/******************************************************************************/

typedef union _OTG_FS_PCGCCTL_TypeDef
{
  uint32_t d32;
  struct
  {
uint32_t stoppclk :
    1;
uint32_t gatehclk :
    1;
uint32_t reserved3 :
    30;
  }
  b;
} OTG_FS_PCGCCTL_TypeDef;


//#define MAX_EP0_SIZE                    0x40
//#define MAX_PACKET_SIZE                 0x400

#define USB_ENDPOINT_XFER_CONTROL       0
#define USB_ENDPOINT_XFER_ISOC          1
#define USB_ENDPOINT_XFER_BULK          2
#define USB_ENDPOINT_XFER_INT           3
#define USB_ENDPOINT_XFERTYPE_MASK      3


/********************************************************************************
                              ENUMERATION TYPE
********************************************************************************/
enum usb_device_speed {
  USB_SPEED_UNKNOWN = 0,
  USB_SPEED_LOW, 
  USB_SPEED_FULL,
  USB_SPEED_HIGH
};

typedef enum {

  USB_OTG_OK,
  USB_OTG_FAIL

}
USB_OTG_Status;

typedef struct USB_OTG_ep
{
  uint8_t        num;
  uint8_t        is_in;
  uint32_t       tx_fifo_num;
  uint32_t       type;
  uint8_t        even_odd_frame;
  uint32_t       maxpacket;
  uint8_t        *xfer_buff;
  uint32_t       xfer_len;
  uint32_t       xfer_count;
}
USB_OTG_EP , *PUSB_OTG_EP;

/********************************************************************************
                     USBF LAYER UNION AND STRUCTURES
********************************************************************************/
typedef struct USB_OTG_USBF
{

  USB_OTG_EP ep0;
  USB_OTG_EP in_ep[ NUM_TX_FIFOS - 1];
  USB_OTG_EP out_ep[ NUM_TX_FIFOS - 1];
}
USB_OTG_PCD_DEV , *USB_OTG_PCD_PDEV;

/* Exported macro ------------------------------------------------------------*/
#define _GetEPTxStatus(bEpNum)        ((uint16_t)OTG_DEV_GetEPTxStatus(bEpNum))
#define _GetEPRxStatus(bEpNum)        ((uint16_t)OTG_DEV_GetEPRxStatus(bEpNum))

#define _SetEPTxStatus(bEpNum,wState) (OTG_DEV_SetEPTxStatus(bEpNum, wState))
#define _SetEPRxStatus(bEpNum,wState) (OTG_DEV_SetEPRxStatus(bEpNum, wState))

#define _SetEPTxValid(bEpNum)         (OTG_DEV_SetEPTxStatus(bEpNum, EP_TX_VALID))
#define _SetEPRxValid(bEpNum)         (OTG_DEV_SetEPRxStatus(bEpNum, EP_RX_VALID))

#define _GetTxStallStatus(bEpNum)     (OTG_DEV_GetEPTxStatus(bEpNum) == EP_TX_STALL)
#define _GetRxStallStatus(bEpNum)     (OTG_DEV_GetEPRxStatus(bEpNum) == EP_RX_STALL) 

#define DEV_EP_TX_DIS       0x0000
#define DEV_EP_TX_STALL     0x0010
#define DEV_EP_TX_NAK       0x0020
#define DEV_EP_TX_VALID     0x0030
 
#define DEV_EP_RX_DIS       0x0000
#define DEV_EP_RX_STALL     0x1000
#define DEV_EP_RX_NAK       0x2000
#define DEV_EP_RX_VALID     0x3000

#define USB_OTG_TIMEOUT     200000
/*****************          GLOBAL DEFINES          ***************************/

#define GAHBCFG_TXFEMPTYLVL_EMPTY              1
#define GAHBCFG_TXFEMPTYLVL_HALFEMPTY          0

#define GAHBCFG_GLBINT_ENABLE                  1
#define GAHBCFG_INT_DMA_BURST_SINGLE           0
#define GAHBCFG_INT_DMA_BURST_INCR             1
#define GAHBCFG_INT_DMA_BURST_INCR4            3
#define GAHBCFG_INT_DMA_BURST_INCR8            5
#define GAHBCFG_INT_DMA_BURST_INCR16           7
#define GAHBCFG_DMAENABLE                      1
#define GAHBCFG_TXFEMPTYLVL_EMPTY              1
#define GAHBCFG_TXFEMPTYLVL_HALFEMPTY          0

#define GRXSTS_PKTSTS_IN                       2
#define GRXSTS_PKTSTS_IN_XFER_COMP             3
#define GRXSTS_PKTSTS_DATA_TOGGLE_ERR          5
#define GRXSTS_PKTSTS_CH_HALTED                7

/*****************           DEVICE DEFINES         ***************************/

#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ     0
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ     1
#define DSTS_ENUMSPD_LS_PHY_6MHZ               2
#define DSTS_ENUMSPD_FS_PHY_48MHZ              3

#define DCFG_FRAME_INTERVAL_80                 0
#define DCFG_FRAME_INTERVAL_85                 1
#define DCFG_FRAME_INTERVAL_90                 2
#define DCFG_FRAME_INTERVAL_95                 3

#define DEP0CTL_MPS_64                         0
#define DEP0CTL_MPS_32                         1
#define DEP0CTL_MPS_16                         2
#define DEP0CTL_MPS_8                          3

#define EP_SPEED_LOW                           0
#define EP_SPEED_FULL                          1
#define EP_SPEED_HIGH                          2

#define EP_TYPE_CTRL                           0
#define EP_TYPE_ISOC                           1
#define EP_TYPE_BULK                           2
#define EP_TYPE_INTR                           3

#define STS_GOUT_NAK                           1
#define STS_DATA_UPDT                          2
#define STS_XFER_COMP                          3
#define STS_SETUP_COMP                         4
#define STS_SETUP_UPDT                         6

/********************************************************************************
                                      MACRO'S
********************************************************************************/

#define CLEAR_IN_EP_INTR(epnum,intr) \
  diepint.d32=0; \
  diepint.b.intr = 1; \
  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DINEPS[epnum]->DIEPINTx,diepint.d32);

#define CLEAR_OUT_EP_INTR(epnum,intr) \
  doepint.d32=0; \
  doepint.b.intr = 1; \
  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DOUTEPS[epnum]->DOEPINTx,doepint.d32);


#define USB_OTG_READ_REG32(reg)  (*(__IO uint32_t *)reg)

#define USB_OTG_WRITE_REG32(reg,value) (*(__IO uint32_t *)reg = value)

#define USB_OTG_MODIFY_REG32(reg,clear_mask,set_mask) \
  USB_OTG_WRITE_REG32(reg, (((USB_OTG_READ_REG32(reg)) & ~clear_mask) | set_mask ) )


//#define uDELAY(usec)  USB_OTG_BSP_uDelay(usec)
#define mDELAY(msec)  cmtWaitTick(msec)

#define _OTGD_FS_GATE_PHYCLK     *(__IO uint32_t*)(0x50000E00) = 0x03
#define _OTGD_FS_UNGATE_PHYCLK   *(__IO uint32_t*)(0x50000E00) = 0x00

/* EP Transmit status defines */
#define EP_TX_DIS              DEV_EP_TX_DIS)  /* EndPoint TX DISabled */
#define EP_TX_STALL            DEV_EP_TX_STALL /* EndPoint TX STALLed */
#define EP_TX_NAK              DEV_EP_TX_NAK   /* EndPoint TX NAKed */
#define EP_TX_VALID            DEV_EP_TX_VALID /* EndPoint TX VALID */

/* EP Transmit status defines */
#define EP_RX_DIS              DEV_EP_RX_DIS   /* EndPoint RX DISabled */
#define EP_RX_STALL            DEV_EP_RX_STALL /* EndPoint RX STALLed */
#define EP_RX_NAK              DEV_EP_RX_NAK   /* EndPoint RX NAKed */
#define EP_RX_VALID            DEV_EP_RX_VALID /* EndPoint RX VALID */
