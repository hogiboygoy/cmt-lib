//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

USB_OTGH_CORE_REGS pdevRegs;

/**
* @brief  USB_OTG_EnableGlobalInt
*         Enables the controller's Global Int in the AHB Config reg
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
void
USB_OTGH_EnableGlobalInt() {
  USB_OTGH_GAHBCFG_TypeDef  ahbcfg;
  
  ahbcfg.d32 = 0;
  ahbcfg.b.glblintrmsk = 1; /* Enable interrupts */
  USB_OTG_MODIFY_REG32(&pdevRegs.GREGS->GAHBCFG, 0, ahbcfg.d32);
  }

/**
* @brief  USB_OTG_DisableGlobalInt
*         Enables the controller's Global Int in the AHB Config reg
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
void
USB_OTGH_DisableGlobalInt() {
  USB_OTGH_GAHBCFG_TypeDef  ahbcfg;
  ahbcfg.d32 = 0;
  ahbcfg.b.glblintrmsk = 1; /* Enable interrupts */
  USB_OTG_MODIFY_REG32(&pdevRegs.GREGS->GAHBCFG, ahbcfg.d32, 0);
  }

/**
* @brief  USB_OTG_SetHostMode
* @param  pdev : Selected device
* @param  mode : 
* @retval USB_OTG_STS : status
*/
void
USB_OTG_SetHostMode() {
  USB_OTG_GUSBCFG_TypeDef  usbcfg;
  
  usbcfg.d32 = USB_OTG_READ_REG32(&pdevRegs.GREGS->GUSBCFG);
  
  usbcfg.b.force_dev = 0;
  usbcfg.b.force_host = 1;
  
  USB_OTG_WRITE_REG32(&pdevRegs.GREGS->GUSBCFG, usbcfg.d32);
  cmtWaitMKS( 50 );
  }

/**
* @brief  USB_OTG_InitFSLSPClkSel : Initializes the FSLSPClkSel field of the 
*         HCFG register on the PHY type
* @param  pdev : Selected device
* @param  freq : clock frequency
* @retval None
*/
void 
USB_OTGH_InitFSLSPClkSel( uint8_t freq ) {
  USB_OTGH_HCFG_TypeDef   hcfg;
  
  hcfg.d32 = USB_OTG_READ_REG32(&pdevRegs.HREGS->HCFG);
  hcfg.b.fslspclksel = freq;
  USB_OTG_WRITE_REG32(&pdevRegs.HREGS->HCFG, hcfg.d32);
  }

/**
* @brief  USB_OTG_FlushTxFifo : Flush a Tx FIFO
* @param  pdev : Selected device
* @param  num : FO num
* @retval USB_OTG_STS : status
*/
void
USB_OTGH_FlushTxFifo( uint32_t num ) {
  __IO USB_OTG_GRSTCTL_TypeDef  greset;
  
  uint32_t count = 0;
  greset.d32 = 0;
  greset.b.txfflsh = 1;
  greset.b.txfnum  = num;
  USB_OTG_WRITE_REG32( &pdevRegs.GREGS->GRSTCTL, greset.d32 );
  do {
    greset.d32 = USB_OTG_READ_REG32( &pdevRegs.GREGS->GRSTCTL);
    if (++count > 200000) break;
    }
  while( greset.b.txfflsh == 1 );
  /* Wait for 3 PHY Clocks*/
  cmtWaitMKS( 3 );
  }

/**
* @brief  USB_OTG_FlushRxFifo : Flush a Rx FIFO
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
void
USB_OTGH_FlushRxFifo() {
  __IO USB_OTG_GRSTCTL_TypeDef  greset;
  uint32_t count = 0;
  
  greset.d32 = 0;
  greset.b.rxfflsh = 1;
  USB_OTG_WRITE_REG32( &pdevRegs.GREGS->GRSTCTL, greset.d32 );
  do {
    greset.d32 = USB_OTG_READ_REG32( &pdevRegs.GREGS->GRSTCTL);
    if (++count > 200000) break;
    }
  while( greset.b.rxfflsh == 1 );
  /* Wait for 3 PHY Clocks*/
  cmtWaitMKS( 3 );
  }

/**
* @brief  USB_OTG_EnableCommonInt
*         Initializes the commmon interrupts, used in both device and modes
* @param  pdev : Selected device
* @retval None
*/
void 
USB_OTGH_EnableCommonInt() {
  USB_OTG_GINTMSK_TypeDef  int_mask;
  
  int_mask.d32 = 0;
  /* Clear any pending USB_OTG Interrupts */
  USB_OTG_WRITE_REG32( &pdevRegs.GREGS->GOTGINT, 0xFFFFFFFF);
  
  /* Clear any pending interrupts */
  USB_OTG_WRITE_REG32( &pdevRegs.GREGS->GINTSTS, 0xFFFFFFFF);
  
  /* Enable the interrupts in the INTMSK */
  int_mask.b.wkupintr   = 1;
  int_mask.b.usbsuspend = 1;  
  
  USB_OTG_WRITE_REG32( &pdevRegs.GREGS->GINTMSK, int_mask.d32);
  }

/**
* @brief  USB_OTG_EnableHostInt: Enables the Host mode interrupts
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
void
USB_OTG_EnableHostInt() {
  USB_OTG_GINTMSK_TypeDef  intmsk;
  intmsk.d32 = 0;
  /* Disable all interrupts. */
  USB_OTG_WRITE_REG32(&pdevRegs.GREGS->GINTMSK, 0);
  
  /* Clear any pending interrupts. */
  USB_OTG_WRITE_REG32(&pdevRegs.GREGS->GINTSTS, 0xFFFFFFFF);
  
  /* Enable the common interrupts */
  USB_OTGH_EnableCommonInt();
  
  
  intmsk.b.rxstsqlvl  = 1;
  intmsk.b.portintr   = 1;
  intmsk.b.hcintr     = 1;
  intmsk.b.disconnect = 1;  
  intmsk.b.sofintr    = 0;  
  intmsk.b.incomplisoout  = 0; 
  USB_OTG_MODIFY_REG32(&pdevRegs.GREGS->GINTMSK, intmsk.d32, intmsk.d32);
  }

/**
* @brief  USB_OTG_CoreReset : Soft reset of the core
* @param  pdev : Selected device
* @retval USB_OTG_STS : status
*/
void
USB_OTGH_CoreReset() {
  __IO USB_OTG_GRSTCTL_TypeDef  greset;
  uint32_t count = 0;
  
  greset.d32 = 0;
  /* Wait for AHB master IDLE state. */
  do {
    cmtWaitMKS( 3 );
    greset.d32 = USB_OTG_READ_REG32(&pdevRegs.GREGS->GRSTCTL);
    if (++count > 200000) return;
    }
  while (greset.b.ahbidle == 0);
  /* Core Soft Reset */
  count = 0;
  greset.b.csftrst = 1;
  USB_OTG_WRITE_REG32(&pdevRegs.GREGS->GRSTCTL, greset.d32 );
  do {
    greset.d32 = USB_OTG_READ_REG32(&pdevRegs.GREGS->GRSTCTL);
    if (++count > 200000) break;
    }
  while (greset.b.csftrst == 1);
  /* Wait for 3 PHY Clocks*/
  cmtWaitMKS( 3 );
  }

/**
* @brief  USB_OTG_ReadCoreItr : returns the Core Interrupt register
* @param  pdev : Selected device
* @retval Status
*/

uint32_t 
USB_OTGH_ReadCoreItr() {
  uint32_t v = 0;
  v = USB_OTG_READ_REG32(&pdevRegs.GREGS->GINTSTS);
  v &= USB_OTG_READ_REG32(&pdevRegs.GREGS->GINTMSK);
  return v;
  }
