//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#define USB_DBL_BUF_EP      0x0000

#define REG(x)  (*((volatile unsigned int *)(x)))

#define USB_BASE_ADDR   0x40005C00  /* USB Registers Base Address */
#define USB_PMA_ADDR    0x40006000  /* USB Packet Memory Area Address */


/* Common Registers */
#define CNTR    REG(USB_BASE_ADDR + 0x40)   /* Control Register */
#define ISTR    REG(USB_BASE_ADDR + 0x44)   /* Interrupt Status Register */
#define FNR     REG(USB_BASE_ADDR + 0x48)   /* Frame Number Register */
#define DADDR   REG(USB_BASE_ADDR + 0x4C)   /* Device Address Register */
#define BTABLE  REG(USB_BASE_ADDR + 0x50)   /* Buffer Table Address Register */

/* CNTR: Control Register Bit Definitions */
#define CNTR_CTRM       0x8000      /* Correct Transfer Interrupt Mask */
#define CNTR_PMAOVRM    0x4000      /* Packet Memory Aerea Over/underrun Interrupt Mask */
#define CNTR_ERRM       0x2000      /* Error Interrupt Mask */
#define CNTR_WKUPM      0x1000      /* Wake-up Interrupt Mask */
#define CNTR_SUSPM      0x0800      /* Suspend Mode Interrupt Mask  */
#define CNTR_RESETM     0x0400      /* USB Reset Interrupt Mask   */
#define CNTR_SOFM       0x0200      /* Start of Frame Interrupt Mask */
#define CNTR_ESOFM      0x0100      /* Expected Start of Frame Interrupt Mask */
#define CNTR_RESUME     0x0010      /* Resume Request */
#define CNTR_FSUSP      0x0008      /* Force Suspend */
#define CNTR_LPMODE     0x0004      /* Low-power Mode */
#define CNTR_PDWN       0x0002      /* Power Down */
#define CNTR_FRES       0x0001      /* Force USB Reset */

/* ISTR: Interrupt Status Register Bit Definitions */
#define ISTR_CTR        0x8000      /* Correct Transfer */
#define ISTR_PMAOVR     0x4000      /* Packet Memory Aerea Over/underrun */
#define ISTR_ERR        0x2000      /* Error */
#define ISTR_WKUP       0x1000      /* Wake-up */
#define ISTR_SUSP       0x0800      /* Suspend Mode */
#define ISTR_RESET      0x0400      /* USB Reset */
#define ISTR_SOF        0x0200      /* Start of Frame */
#define ISTR_ESOF       0x0100      /* Expected Start of Frame */
#define ISTR_DIR        0x0010      /* Direction of Transaction */
#define ISTR_EP_ID      0x000F      /* EndPoint Identifier */

/* FNR: Frame Number Register Bit Definitions */
#define FNR_RXDP        0x8000      /* D+ Data Line Status */
#define FNR_RXDM        0x4000      /* D- Data Line Status */
#define FNR_LCK         0x2000      /* Locked */
#define FNR_LSOF        0x1800      /* Lost SOF */
#define FNR_FN          0x07FF      /* Frame Number */

/* DADDR: Device Address Register Bit Definitions */
#define DADDR_EF        0x0080      /* Enable Function */
#define DADDR_ADD       0x007F      /* Device Address */


/* EndPoint Registers */
#define EPxREG(x)       REG(USB_BASE_ADDR + 4*(x))

/* EPxREG: EndPoint Registers Bit Definitions */
#define EP_CTR_RX       0x8000      /* Correct RX Transfer */
#define EP_DTOG_RX      0x4000      /* RX Data Toggle */
#define EP_STAT_RX      0x3000      /* RX Status */
#define EP_SETUP        0x0800      /* EndPoint Setup */
#define EP_TYPE         0x0600      /* EndPoint Type */
#define EP_KIND         0x0100      /* EndPoint Kind */
#define EP_CTR_TX       0x0080      /* Correct TX Transfer */
#define EP_DTOG_TX      0x0040      /* TX Data Toggle */
#define EP_STAT_TX      0x0030      /* TX Status */
#define EP_EA           0x000F      /* EndPoint Address */

/* EndPoint Register Mask (No Toggle Fields) */
#define EP_MASK         (EP_CTR_RX|EP_SETUP|EP_TYPE|EP_KIND|EP_CTR_TX|EP_EA)

/* EP_TYPE: EndPoint Types */
#define EP_BULK         0x0000      /* BULK EndPoint */
#define EP_CONTROL      0x0200      /* CONTROL EndPoint */
#define EP_ISOCHRONOUS  0x0400      /* ISOCHRONOUS EndPoint */
#define EP_INTERRUPT    0x0600      /* INTERRUPT EndPoint */

/* EP_KIND: EndPoint Kind */
#define EP_DBL_BUF      EP_KIND     /* Double Buffer for Bulk Endpoint */
#define EP_STATUS_OUT   EP_KIND     /* Status Out for Control Endpoint */

/* EP_STAT_TX: TX Status */
#define EP_TX_DIS       0x0000      /* Disabled */
#define EP_TX_STALL     0x0010      /* Stalled */
#define EP_TX_NAK       0x0020      /* NAKed */
#define EP_TX_VALID     0x0030      /* Valid */

/* EP_STAT_RX: RX Status */
#define EP_RX_DIS       0x0000      /* Disabled */
#define EP_RX_STALL     0x1000      /* Stalled */
#define EP_RX_NAK       0x2000      /* NAKed */
#define EP_RX_VALID     0x3000      /* Valid */


/* Endpoint Buffer Descriptor */
typedef struct _EP_BUF_DSCR {
  uint32 ADDR_TX;
  uint32 COUNT_TX;
  uint32 ADDR_RX;
  uint32 COUNT_RX;
} EP_BUF_DSCR;

#define EP_ADDR_MASK    0xFFFE      /* Address Mask */
#define EP_COUNT_MASK   0x03FF      /* Count Mask */

#define EP_BUF_ADDR     (sizeof(EP_BUF_DSCR)*USB_EP_NUM) /* Endpoint Buffer Start Address */

/* Pointer to Endpoint Buffer Descriptors */
EP_BUF_DSCR *pBUF_DSCR = (EP_BUF_DSCR *)USB_PMA_ADDR;

/* Endpoint Free Buffer Address */
uint16 FreeBufAddr;

/*
 *  Reset Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void EP_Reset( uint8 EPNum ) {
  uint32 num, val;

  num = EPNum & 0x0F;
  val = EPxREG(num);
  if (EPNum & 0x80) {                       /* IN Endpoint */
    EPxREG(num) = val & (EP_MASK | EP_DTOG_TX);
    } 
  else {                                  /* OUT Endpoint */
    EPxREG(num) = val & (EP_MASK | EP_DTOG_RX);
    }
  }


/*
 *  Set Endpoint Status
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     stat: New Status
 *    Return Value:    None
 */

void EP_Status( uint8 EPNum, uint32 stat) {
  uint32 num, val;

  num = EPNum & 0x0F;
  val = EPxREG(num);
  if (EPNum & 0x80) {                       /* IN Endpoint */
    EPxREG(num) = (val ^ (stat & EP_STAT_TX)) & (EP_MASK | EP_STAT_TX);
    } 
  else {                                  /* OUT Endpoint */
    EPxREG(num) = (val ^ (stat & EP_STAT_RX)) & (EP_MASK | EP_STAT_RX);
    }
  }

extern "C" {

void
USB_LP_CAN1_RX0_IRQHandler() {
  if( CmtUsb::usb ) CmtUsb::usb->IrqHandler();
  }

}

void
CmtUsbDev::IrqHandler() {
  uint32 istr, num, val;

  istr = ISTR;

  /* USB Reset Request */
  if (istr & ISTR_RESET) {
    USB_Reset();
    ISTR = ~ISTR_RESET;
    }

  /* USB Suspend Request */
  if (istr & ISTR_SUSP) {
    USB_Suspend();
    ISTR = ~ISTR_SUSP;
    }

  /* USB Wakeup */
  if (istr & ISTR_WKUP) {
    USB_WakeUp();
    ISTR = ~ISTR_WKUP;
    }

  /* Start of Frame */
  if (istr & ISTR_SOF) {
    USB_StartOfFrame();
    ISTR = ~ISTR_SOF;
    }

  /* PMA Over/underrun */
  if (istr & ISTR_PMAOVR) {
    USB_Error(1);
    ISTR = ~ISTR_PMAOVR;
    }

  /* Error: No Answer, CRC Error, Bit Stuff Error, Frame Format Error */
  if (istr & ISTR_ERR) {
    USB_Error(0);
    ISTR = ~ISTR_ERR;
    }

  /* Endpoint Interrupts */
  while( (istr = ISTR) & ISTR_CTR) {
    ISTR = ~ISTR_CTR;

    num = istr & ISTR_EP_ID;

    val = EPxREG(num);
    if (val & EP_CTR_RX) {
      EPxREG(num) = val & ~EP_CTR_RX & EP_MASK;
      switch( num ) {
        case 0  : EndPoint0( val & EP_SETUP ? USB_EVT_SETUP : USB_EVT_OUT ); break;
        case 1  : EndPoint1( USB_EVT_OUT ); break;
        case 2  : EndPoint2( USB_EVT_OUT ); break;
        case 3  : EndPoint3( USB_EVT_OUT ); break;
        case 4  : EndPoint4( USB_EVT_OUT ); break;
        case 5  : EndPoint5( USB_EVT_OUT ); break;
        case 6  : EndPoint6( USB_EVT_OUT ); break;
        case 7  : EndPoint7( USB_EVT_OUT ); break;
        case 8  : EndPoint8( USB_EVT_OUT ); break;
        case 9  : EndPoint9( USB_EVT_OUT ); break;
        case 10 : EndPoint10( USB_EVT_OUT ); break;
        case 11 : EndPoint11( USB_EVT_OUT ); break;
        case 12 : EndPoint12( USB_EVT_OUT ); break;
        case 13 : EndPoint13( USB_EVT_OUT ); break;
        case 14 : EndPoint14( USB_EVT_OUT ); break;
        case 15 : EndPoint15( USB_EVT_OUT ); break;
        }
      }
    if (val & EP_CTR_TX) {
      EPxREG(num) = val & ~EP_CTR_TX & EP_MASK;
      switch( num ) {
        case 0  : EndPoint0( USB_EVT_IN ); break;
        case 1  : EndPoint1( USB_EVT_IN ); break;
        case 2  : EndPoint2( USB_EVT_IN ); break;
        case 3  : EndPoint3( USB_EVT_IN ); break;
        case 4  : EndPoint4( USB_EVT_IN ); break;
        case 5  : EndPoint5( USB_EVT_IN ); break;
        case 6  : EndPoint6( USB_EVT_IN ); break;
        case 7  : EndPoint7( USB_EVT_IN ); break;
        case 8  : EndPoint8( USB_EVT_IN ); break;
        case 9  : EndPoint9( USB_EVT_IN ); break;
        case 10 : EndPoint10( USB_EVT_IN ); break;
        case 11 : EndPoint11( USB_EVT_IN ); break;
        case 12 : EndPoint12( USB_EVT_IN ); break;
        case 13 : EndPoint13( USB_EVT_IN ); break;
        case 14 : EndPoint14( USB_EVT_IN ); break;
        case 15 : EndPoint15( USB_EVT_IN ); break;
        }
      }
    }  
  }

void 
CmtUsbDev::USB_Suspend() {
  CNTR |= CNTR_FSUSP;                       /* Force Suspend */
  CNTR |= CNTR_LPMODE;                      /* Low Power Mode */
  }

void 
CmtUsbDev::USB_WakeUp() {
  CNTR &= ~CNTR_FSUSP;                      /* Clear Suspend */
  }



uint32 
CmtUsbDev::USB_WriteEP8( uint8 EPNum, uint8 *pData, uint32 cnt ) {
  /* Double Buffering is not yet supported              */
  uint32 num, *pv, n;
  
  num = EPNum & 0x0F;
  
  pv  = (uint32*)(USB_PMA_ADDR + 2*((pBUF_DSCR + num)->ADDR_TX));
  for (n = 0; n < (cnt + 1) / 2; n++) {
    *pv++ = (pData[0] & 0xff) | ((pData[1] << 8) & 0xff00);
    pData += 2;
    }
  (pBUF_DSCR + num)->COUNT_TX = cnt;
  EP_Status(EPNum, EP_TX_VALID);
  
  return (cnt);
  }
   
uint32 
CmtUsbDev::USB_ReadEP8 (uint8 EPNum, uint8 *pData) {
  /* Double Buffering is not yet supported              */
  uint32 num, cnt, *pv, n, v;
  
  num = EPNum & 0x0F;
  
  pv  = (uint32*)(USB_PMA_ADDR + 2*((pBUF_DSCR + num)->ADDR_RX));
  cnt = (pBUF_DSCR + num)->COUNT_RX & EP_COUNT_MASK;
  for (n = 0; n < (cnt + 1) / 2; n++) {
    v = *pv++;
    pData[0] = v;
    pData[1] = v >> 8;
    //  *((__packed WORD *)pData) = *pv++;
    pData += 2;
    }
  EP_Status(EPNum, EP_RX_VALID);
   
  return (cnt);
  }

void
CmtUsbDev::USB_SetNAK( uint8 EPNum ) {
  //Только для ненулевой EP
  if( (EPNum & 0x7f) != 0 )
    EP_Status(EPNum, EP_RX_NAK);
  }

void
CmtUsbDev::USB_ResetNAK( uint8 EPNum ) {
  //Только для ненулевой EP
  if( (EPNum & 0x7f) != 0 )
    EP_Status(EPNum, EP_RX_VALID);
  }


void 
CmtUsbDev::USB_ClrStallEP( uint8 EPNum ) {
  EP_Status(EPNum, EP_TX_VALID | EP_RX_VALID);
  }

void
CmtUsbDev::USB_SetStallEP( uint8 EPNum ) {
  EP_Status(EPNum, EP_TX_STALL | EP_RX_STALL);
  }

void 
CmtUsbDev::USB_DisableEP( uint8 EPNum ) {
  EP_Status(EPNum, EP_TX_DIS | EP_RX_DIS);
  }

void 
CmtUsbDev::USB_EnableEP( uint8 EPNum ) {
  EP_Status(EPNum, EP_TX_VALID | EP_RX_VALID);
  }

void 
CmtUsbDev::USB_ResetEP( uint8 EPNum ) {
  EP_Reset(EPNum);
  }

void 
CmtUsbDev::USB_DirCtrlEP( uint8 dir ) {
  
  }

void 
CmtUsbDev::USB_ConfigEP( USB_ENDPOINT_DESCRIPTOR *pEPD ) {
  /* Double Buffering is not yet supported              */
  uint32 num, val;
  
  num = pEPD->bEndpointAddress & 0x0F;
  
  val = pEPD->wMaxPacketSize;
  if( pEPD->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK ) {
    (pBUF_DSCR + num)->ADDR_TX = FreeBufAddr;
    val = (val + 1) & ~1;
    } 
  else {
    (pBUF_DSCR + num)->ADDR_RX = FreeBufAddr;
    if(val > 62) {
      val = (val + 31) & ~31;
      (pBUF_DSCR + num)->COUNT_RX = ((val << 5) - 1) | 0x8000;
      } 
    else {
      val = (val + 1)  & ~1;
      (pBUF_DSCR + num)->COUNT_RX =   val << 9;
      }
    }
  FreeBufAddr += val;
  
  switch (pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK) {
    case USB_ENDPOINT_TYPE_CONTROL:
      val = EP_CONTROL;
      break;
    case USB_ENDPOINT_TYPE_ISOCHRONOUS:
      val = EP_ISOCHRONOUS;
      break;
    case USB_ENDPOINT_TYPE_BULK:
      val = EP_BULK;
      if( USB_DBL_BUF_EP & (1 << num) ) {
        val |= EP_KIND;
        }
      break;
    case USB_ENDPOINT_TYPE_INTERRUPT:
      val = EP_INTERRUPT;
      break;
    }
  val |= num;
  EPxREG(num) = val;
  }

void 
CmtUsbDev::USB_Configure (uint_8 cfg) {
  
  }
  
void
CmtUsbDev::USB_StartOfFrame() {
  }

void 
CmtUsbDev::USB_SetAddress (uint8 adr) {
  DADDR = DADDR_EF | adr;
  }


void 
CmtUsbDev::USB_SetAddressPre (uint8 adr) {
  }

void 
CmtUsbDev::USB_Connect (int con) {
  CNTR = CNTR_FRES;                         /* Force USB Reset */
  ISTR = 0;                                 /* Clear Interrupt Status */
  if (con) {
    CNTR = CNTR_RESETM;                     /* USB Reset Interrupt Mask */
    } 
  else {
    CNTR = CNTR_FRES | CNTR_PDWN;           /* Switch Off USB Device */
    }
  }

void 
CmtUsbDev::USB_Reset() {
  ISTR = 0;                                 /* Clear Interrupt Status */

  CNTR = CNTR_CTRM | CNTR_RESETM |
         (USB_SUSPEND_EVENT ? CNTR_SUSPM   : 0) |
         (USB_WAKEUP_EVENT  ? CNTR_WKUPM   : 0) |
         (USB_ERROR_EVENT   ? CNTR_ERRM    : 0) |
         (USB_ERROR_EVENT   ? CNTR_PMAOVRM : 0) |
         (USB_SOF_EVENT     ? CNTR_SOFM    : 0) |
         (USB_SOF_EVENT     ? CNTR_ESOFM   : 0);

  FreeBufAddr = EP_BUF_ADDR;

  BTABLE = 0x00;                            /* set BTABLE Address */

  /* Setup Control Endpoint 0 */
  
  pBUF_DSCR->ADDR_TX = FreeBufAddr;
  FreeBufAddr += USB_MAX_PACKET0;
  pBUF_DSCR->ADDR_RX = FreeBufAddr;
  FreeBufAddr += USB_MAX_PACKET0;
  if (USB_MAX_PACKET0 > 62) {
    pBUF_DSCR->COUNT_RX = ((USB_MAX_PACKET0 << 5) - 1) | 0x8000;
    } 
  else {
    pBUF_DSCR->COUNT_RX =   USB_MAX_PACKET0 << 9;
    }
  EPxREG(0) = EP_CONTROL | EP_RX_VALID;

  DADDR = DADDR_EF | 0;                     /* Enable USB Default Address */
  
  USB_ResetCore();
  }

void 
CmtUsbDev::Init() {
  USB_Reset();
  }

void 
CmtUsbDev::USB_DataInEnd() {
  }

void
CmtUsbDev::USB_Error( uint8 ) {
  }


void 
cmtInitUsb() {
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  
  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

  
  /* Enable USB Interrupts */
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  }

CmtUsb* CmtUsb::usb;
