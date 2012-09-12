//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_USB_STM32F105VB
#define CMT_USB_STM32F105VB

#define USB_OTG_FS_BASE_ADDR             0x50000000

#define USB_OTG_CORE_GLOBAL_REGS_OFFSET      0x0000
#define USB_OTG_DEV_GLOBAL_REG_OFFSET        0x0800
#define USB_OTG_DEV_IN_EP_REG_OFFSET         0x0900
#define USB_OTG_EP_REG_OFFSET                0x0020
#define USB_OTG_DEV_OUT_EP_REG_OFFSET        0x0B00
#define USB_OTG_PCGCCTL_OFFSET               0x0E00
#define USB_OTG_DATA_FIFO_OFFSET             0x1000
#define USB_OTG_DATA_FIFO_SIZE               0x1000

#define USB_OTG_HOST_GLOBAL_REG_OFFSET       0x400
#define USB_OTG_HOST_PORT_REGS_OFFSET        0x440
#define USB_OTG_HOST_CHAN_REGS_OFFSET        0x500
#define USB_OTG_CHAN_REGS_OFFSET             0x20

#define NUM_TX_FIFOS                         4

CmtUsb* CmtUsb::usb;

//Обработчик прерывания
extern "C" {

void
OTG_FS_IRQHandler() {
  if( CmtUsb::usb )
    CmtUsb::usb->IrqHandler();
  }

}


void 
cmtInitUsb() {
  /* Select USBCLK source */
  RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

  /* Enable the USB clock */ 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;

  
  /* Enable USB Interrupts */
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USB Interrupts */
  NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  }

#endif
