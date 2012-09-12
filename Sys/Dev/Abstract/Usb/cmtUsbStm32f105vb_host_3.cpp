//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#define HCFG_48_MHZ                            1
#define RX_FIFO_SIZE                          128
#define TXH_NP_FIFOSIZ                         96
#define TXH_P_FIFOSIZ                          96

struct Cmt

void
CmtUsbHost::Init() {
  int i = 0;

  USB_OTGH_GUSBCFG_TypeDef  usbcfg;
  USB_OTG_GCCFG_TypeDef     cfgctl;
  usbcfg.d32 = 0;
  cfgctl.d32 = 0;

  //==========================================================  
  //                   USB_OTG_CoreInit
  pdevRegs.GREGS = (USB_OTGH_GREGS *)(USB_OTG_FS_BASE_ADDR + USB_OTG_CORE_GLOBAL_REGS_OFFSET);
  
  pdevRegs.HREGS = (USB_OTG_HREGS *)(USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_GLOBAL_REG_OFFSET);
  
  pdevRegs.HPRT0 = (uint32_t *)(USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_PORT_REGS_OFFSET);
  
  for(i = 0; i < USB_OTG_HOST_CHANNEL_NBR; i++) {
    pdevRegs.HC_REGS[i] = (USB_OTG_HC_REGS *)(USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_CHAN_REGS_OFFSET + (i * USB_OTG_CHAN_REGS_OFFSET));
    }
  for(i = 0; i < USB_OTG_HOST_CHANNEL_NBR; i++) {
    pdevRegs.DFIFO[i] = (uint32_t *)(USB_OTG_FS_BASE_ADDR + USB_OTG_DATA_FIFO_OFFSET + (i * USB_OTG_DATA_FIFO_SIZE));
    }
  pdevRegs.PCGCCTL = (uint32_t *)(USB_OTG_FS_BASE_ADDR + USB_OTG_PCGCCTL_OFFSET);
  
  USB_OTGH_DisableGlobalInt();
  
  usbcfg.d32 = USB_OTG_READ_REG32(&pdevRegs.GREGS->GUSBCFG);;
  usbcfg.b.physel  = 1; /* FS Interface */
  USB_OTG_WRITE_REG32 (&pdevRegs.GREGS->GUSBCFG, usbcfg.d32);
  /* Reset after a PHY select and set Host mode */
  USB_OTGH_CoreReset();
  /* Deactivate the power down and enable the sensing*/
  cfgctl.d32 = 0;
//  cfgctl.b.pwdn = 1;
//  cfgctl.b.vbussensingA = 1 ;
//  cfgctl.b.vbussensingB = 1 ;
//  cfgctl.b.SOFouten = 1;   
  
  USB_OTG_WRITE_REG32 (&pdevRegs.GREGS->GCCFG, cfgctl.d32);
  cmtWaitMKS( 20 );
//  USB_OTG_BSP_mDelay(20);
  /* Program GUSBCFG.OtgUtmifsSel to I2C*/
  usbcfg.d32 = USB_OTG_READ_REG32(&pdevRegs.GREGS->GUSBCFG);
  usbcfg.b.otgutmifssel = 0;
  
  USB_OTG_WRITE_REG32 (&pdevRegs.GREGS->GUSBCFG, usbcfg.d32);
  
  //==========================================================  
  //                Force Host Mode
  USB_OTG_SetHostMode();

  //==========================================================  
  //             USB_OTG_CoreInitHost(pdev);
  USB_OTGH_FSIZ_TypeDef            nptxfifosize;
  USB_OTGH_FSIZ_TypeDef            ptxfifosize;  
  USB_OTGH_HCCHAR_TypeDef          hcchar;
  USB_OTGH_HCFG_TypeDef            hcfg;
  
  
  nptxfifosize.d32 = 0;  
  ptxfifosize.d32 = 0;
  
  hcfg.d32 = 0;
  
  
  /* Restart the Phy Clock */
  USB_OTG_WRITE_REG32(pdevRegs.PCGCCTL, 0);
  
  /* Initialize Host Configuration Register */
  USB_OTGH_InitFSLSPClkSel( HCFG_48_MHZ ); 
  
  hcfg.d32 = USB_OTG_READ_REG32(&pdevRegs.HREGS->HCFG);
  hcfg.b.fslssupp = 1;
  USB_OTG_WRITE_REG32(&pdevRegs.HREGS->HCFG, hcfg.d32);
  
  /* Configure data FIFO sizes */
  /* Rx FIFO */
  USB_OTG_WRITE_REG32(&pdevRegs.GREGS->GRXFSIZ, RX_FIFO_SIZE);
  
  /* Tx FIFO */
  nptxfifosize.b.depth     = TXH_NP_FIFOSIZ;
  nptxfifosize.b.startaddr = RX_FIFO_SIZE;
  USB_OTG_WRITE_REG32(&pdevRegs.GREGS->HNPTXFSIZ, nptxfifosize.d32);
  
  ptxfifosize.b.startaddr = RX_FIFO_SIZE + TXH_NP_FIFOSIZ;
  ptxfifosize.b.depth     = TXH_P_FIFOSIZ;
  USB_OTG_WRITE_REG32(&pdevRegs.GREGS->HPTXFSIZ, ptxfifosize.d32);  
  
  
  
  /* Make sure the FIFOs are flushed. */
  USB_OTGH_FlushTxFifo( 0x10 );         /* all Tx FIFOs */
  USB_OTGH_FlushRxFifo();
  
  
  for( i = 0; i < USB_OTG_HOST_CHANNEL_NBR; i++ ) {
    hcchar.d32 = USB_OTG_READ_REG32(&pdevRegs.HC_REGS[i]->HCCHAR);
    hcchar.b.chen = 0;
    hcchar.b.chdis = 1;
    hcchar.b.epdir = 0;
    USB_OTG_WRITE_REG32(&pdevRegs.HC_REGS[i]->HCCHAR, hcchar.d32);
    
    /* Clear all pending HC Interrupts */
    USB_OTG_WRITE_REG32( &pdevRegs.HC_REGS[i]->HCINT, 0xFFFFFFFF );
    }
  
  /* Halt all channels to put them into a known state. */
  for( i = 0; i < USB_OTG_HOST_CHANNEL_NBR; i++ ) {
    hcchar.d32 = USB_OTG_READ_REG32(&pdevRegs.HC_REGS[i]->HCCHAR);
    hcchar.b.chen = 1;
    hcchar.b.chdis = 1;
    hcchar.b.epdir = 0;
    USB_OTG_WRITE_REG32(&pdevRegs.HC_REGS[i]->HCCHAR, hcchar.d32);
    do {
      USB_OTG_READ_REG32(&pdevRegs.GREGS->GRXSTSP);
      hcchar.d32 = USB_OTG_READ_REG32(&pdevRegs.HC_REGS[i]->HCCHAR);
      cmtWaitMKS( 20 );
      }
    while (hcchar.b.chen);
    }
  /* Disable HALT interrupt Masks */
  for( i = 0; i < USB_OTG_HOST_CHANNEL_NBR; i++ ) {
    USB_OTGH_HCGINTMSK_TypeDef  hcintmsk;
    hcintmsk.d32 = USB_OTG_READ_REG32(&pdevRegs.HC_REGS[i]->HCGINTMSK);
    hcintmsk.b.chhltd = 0;
    USB_OTG_WRITE_REG32(&pdevRegs.HC_REGS[i]->HCGINTMSK , hcintmsk.d32);
    }
  
  USB_OTG_EnableHostInt();
  
  USB_OTGH_EnableGlobalInt();
  
  }

void
CmtUsbHost::IrqHandler() {
  USB_OTGH_GINTSTS_TypeDef  gintsts;
  
  gintsts.d32 = 0;
  

  gintsts.d32 = USB_OTGH_ReadCoreItr();
  if( !gintsts.d32 ) return;
    
  if( gintsts.b.sofintr ) {
    USB_OTGH_GINTSTS_TypeDef gintstsA;
    
    
    gintstsA.d32 = 0;
    /* Clear interrupt */
    gintstsA.b.sofintr = 1;
    USB_OTG_WRITE_REG32(&pdevRegs.GREGS->GINTSTS, gintstsA.d32);
    }
    
  if( gintsts.b.rxstsqlvl ) {
    //retval |= USB_OTG_USBH_handle_rx_qlvl_ISR (pdev);
    }
    
  if( gintsts.b.nptxfempty ) {
    //retval |= USB_OTG_USBH_handle_nptxfempty_ISR (pdev);
    }
    
  if( gintsts.b.ptxfempty ) {
    //retval |= USB_OTG_USBH_handle_ptxfempty_ISR (pdev);
    }    
    
  if( gintsts.b.hcintr ) {
    //retval |= USB_OTG_USBH_handle_hc_ISR (pdev);
    }
    
  if( gintsts.b.portintr ) {
    //retval |= USB_OTG_USBH_handle_port_ISR (pdev);
    }
    
  if( gintsts.b.disconnect ) {
    USB_OTGH_GINTSTS_TypeDef gintstsA;
    
    
    gintstsA.d32 = 0;
    /* Clear interrupt */
    gintstsA.b.disconnect = 1;
    USB_OTG_WRITE_REG32(&pdevRegs.GREGS->GINTSTS, gintstsA.d32);
    //retval |= USB_OTG_USBH_handle_Disconnect_ISR (pdev);  
    }
    
  if( gintsts.b.incomplisoout ) {
    //retval |= USB_OTG_USBH_handle_IncompletePeriodicXfer_ISR (pdev);
    }
  }
