//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

USB_OTG_PCD_DEV USB_OTG_PCD_dev;

extern USB_OTG_CORE_REGS     USB_OTG_FS_regs;

/* Private function prototypes -----------------------------------------------*/
static USB_OTG_Status OTGD_FS_SetDeviceMode(void);
static USB_OTG_Status OTGD_FS_CoreReset(void);

/*******************************************************************************
* Function Name  : OTGD_FS_EnableGlobalInt
* Description    : Enables the controller's Global Int in the AHB Config reg
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
USB_OTG_Status OTGD_FS_EnableGlobalInt(void)
{
  USB_OTG_Status status = USB_OTG_OK;
  USB_OTG_GAHBCFG_TypeDef  ahbcfg;

  ahbcfg.d32 = 0;
  
  ahbcfg.b.gintmsk = 1; /* Enable interrupts */
  USB_OTG_MODIFY_REG32(&USB_OTG_FS_regs.GREGS->GAHBCFG, 0, ahbcfg.d32);
  return status;
}

/*******************************************************************************
* Function Name  : OTGD_FS_DisableGlobalInt
* Description    : Disables the controller's Global Int in the AHB Config reg
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
USB_OTG_Status OTGD_FS_DisableGlobalInt(void)
{
  USB_OTG_Status status = USB_OTG_OK;
  USB_OTG_GAHBCFG_TypeDef ahbcfg;

  ahbcfg.d32 = 0;
  ahbcfg.b.gintmsk = 1; /* Enable interrupts */
  USB_OTG_MODIFY_REG32(&USB_OTG_FS_regs.GREGS->GAHBCFG, ahbcfg.d32, 0);
  return status;
}

/*******************************************************************************
* Function Name  : OTGD_FS_SetAddress
* Description    : Initialize core registers addresses.
* Input          : BaseAddress
* Output         : None
* Return         : status
*******************************************************************************/
USB_OTG_Status OTGD_FS_SetAddress(uint32_t BaseAddress)
{
  uint32_t i = 0;
  USB_OTG_Status status = USB_OTG_OK;
  USB_OTG_FS_regs.GREGS = (USB_OTG_GREGS *)(BaseAddress + USB_OTG_CORE_GLOBAL_REGS_OFFSET);

  USB_OTG_FS_regs.DEV    =  (USB_OTG_DEV  *)(BaseAddress + USB_OTG_DEV_GLOBAL_REG_OFFSET);

  for (i = 0; i < NUM_TX_FIFOS; i++) {
    USB_OTG_FS_regs.DINEPS[i]  = (USB_OTG_DINEPS *)  (BaseAddress + USB_OTG_DEV_IN_EP_REG_OFFSET + (i * USB_OTG_EP_REG_OFFSET));
    
    USB_OTG_FS_regs.DOUTEPS[i] = (USB_OTG_DOUTEPS *) (BaseAddress + USB_OTG_DEV_OUT_EP_REG_OFFSET + (i * USB_OTG_EP_REG_OFFSET));
    }

  for (i = 0; i < NUM_TX_FIFOS; i++) {
    USB_OTG_FS_regs.FIFO[i] = (uint32_t *)(BaseAddress + USB_OTG_DATA_FIFO_OFFSET + (i * USB_OTG_DATA_FIFO_SIZE));
    }

  USB_OTG_FS_regs.PCGCCTL = (uint32_t *)(BaseAddress + USB_OTG_PCGCCTL_OFFSET);

  return status;
}

/*******************************************************************************
* Function Name  : OTGD_FS_CoreReset
* Description    : Soft reset of the core
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
static USB_OTG_Status OTGD_FS_CoreReset(void)
{
  USB_OTG_Status status = USB_OTG_OK;
  __IO USB_OTG_GRSTCTL_TypeDef greset;
  uint32_t timeout = 0;

  greset.d32 = 0;
  
  /* Wait for AHB master IDLE state. */
  do
  {
    cmtWaitMKS(5);
    greset.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.GREGS->GRSTCTL);
    if (++timeout > USB_OTG_TIMEOUT)
    {
      return USB_OTG_OK;
    }
  }
  while (greset.b.ahbidle == 0);

  /* Core Soft Reset */
  timeout = 0;
  greset.b.csftrst = 1;
  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.GREGS->GRSTCTL, greset.d32 );
  
  do
  {
    greset.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.GREGS->GRSTCTL);
    if (++timeout > USB_OTG_TIMEOUT)
    {
      break;
    }
  }
  while (greset.b.csftrst == 1);

  /* Wait for 3 PHY Clocks*/
  cmtWaitMKS(5);
  return status;
}

/*******************************************************************************
* Function Name  : OTGD_FS_CoreInit
* Description    : Initialize the USB_OTG controller registers and prepares the core
                   for device mode or host mode operation.
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
USB_OTG_Status OTGD_FS_CoreInit(void)
{
  USB_OTG_Status status = USB_OTG_OK;
  USB_OTG_GUSBCFG_TypeDef usbcfg;
  USB_OTG_GCCFG_TypeDef    gccfg;
 
  usbcfg.d32 = 0;
  gccfg.d32  = 0;
  
  usbcfg.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.GREGS->GUSBCFG);
  usbcfg.b.physel = 1;
  USB_OTG_WRITE_REG32 (&USB_OTG_FS_regs.GREGS->GUSBCFG, usbcfg.d32);

  /* init and configure the phy */
  gccfg.d32 = 0;
  gccfg.b.vbussensingB = 1;
  gccfg.b.pwdn = 1;
  USB_OTG_WRITE_REG32 (&USB_OTG_FS_regs.GREGS->GCCFG, gccfg.d32);
  mDELAY(50);

  /* Reset after a PHY select and set Host mode */
  OTGD_FS_CoreReset();

  /* Set Device Mode */
  OTGD_FS_SetDeviceMode();

  return status;
}

/*******************************************************************************
* Function Name  : InitDevSpeed
* Description    : Initializes the DevSpd field of the DCFG register depending
                   on the PHY type and the enumeration speed of the device.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void InitDevSpeed(void)
{
  USB_OTG_DCFG_TypeDef  dcfg;

  dcfg.d32 = 0;
  
  dcfg.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DCFG);
  dcfg.b.devspd = 0x3;  /* Full speed PHY */
  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DEV->DCFG, dcfg.d32);
}

/*******************************************************************************
* Function Name  : OTGD_FS_EnableDevInt
* Description    : Enables the Device mode interrupts
* Input          : None
* Output         : None
* Return         : status
*******************************************************************************/
USB_OTG_Status OTGD_FS_EnableDevInt(void)
{

  USB_OTG_Status status = USB_OTG_OK;
  USB_OTG_GINTMSK_TypeDef intr_mask;
  
  intr_mask.d32 = 0;

  /* Disable all interrupts. */
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.GREGS->GINTMSK, 0);

  /* Clear any pending interrupts */
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.GREGS->GINTSTS, 0xFFFFFFFF);

  /* Enable the defined interrupts in  Device mode */
#ifdef INTR_ISOOUTDROP
  intr_mask.b.isooutdrop = 1;
#endif /* INTR_ISOOUTDROP */
#ifdef INTR_INCOMPLISOIN
  intr_mask.b.incomplisoin = 1;
#endif /* INTR_INCOMPLISOIN */
#ifdef INTR_INCOMPLISOOUT
  intr_mask.b.incomplisoout = 1;
#endif /* INTR_INCOMPLISOOUT */
#ifdef INTR_DISCONNECT
  intr_mask.b.disconnect = 1;
#endif /* INTR_DISCONNECT */
  intr_mask.b.usbsuspend = 1;
  intr_mask.b.wkupintr = 1;
  intr_mask.b.sofintr = 1;
  intr_mask.b.rxstsqlvl = 1;
  intr_mask.b.enumdone = 1;
  intr_mask.b.usbreset = 1;
  intr_mask.b.inepintr = 1;
  intr_mask.b.outepintr = 1;

  USB_OTG_MODIFY_REG32( &USB_OTG_FS_regs.GREGS->GINTMSK, intr_mask.d32, intr_mask.d32);
  return status;
  
}

/*******************************************************************************
* Function Name  : OTGD_FS_FlushTxFifo
* Description    : Flush a Tx FIFO
* Input          : FIFO num
* Output         : None
* Return         : status
*******************************************************************************/
USB_OTG_Status OTGD_FS_FlushTxFifo (uint32_t num )
{

  USB_OTG_Status status = USB_OTG_OK;
  __IO USB_OTG_GRSTCTL_TypeDef greset;
  uint32_t timeout = 0;

  greset.d32 = 0;
    
  greset.b.txfflsh = 1;
  greset.b.txfnum  = num;
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.GREGS->GRSTCTL, greset.d32 );

  do
  {
    greset.d32 = USB_OTG_READ_REG32( &USB_OTG_FS_regs.GREGS->GRSTCTL);
    if (++timeout > USB_OTG_TIMEOUT)
    {
      break;
    }
  }
  while (greset.b.txfflsh == 1);

  /* Wait for 3 PHY Clocks*/
  cmtWaitMKS(5);

  return status;
}

/*******************************************************************************
* Function Name  : OTGD_FS_FlushRxFifo
* Description    : Flush a Rx FIFO
* Input          : None
* Output         : None
* Return         : status
*******************************************************************************/
USB_OTG_Status OTGD_FS_FlushRxFifo( void )
{
  USB_OTG_Status status = USB_OTG_OK;
  __IO USB_OTG_GRSTCTL_TypeDef greset;
  uint32_t timeout = 0;

  greset.d32 = 0;
  
  greset.b.rxfflsh = 1;
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.GREGS->GRSTCTL, greset.d32 );

  do
  {
    greset.d32 = USB_OTG_READ_REG32( &USB_OTG_FS_regs.GREGS->GRSTCTL);
    if (++timeout > USB_OTG_TIMEOUT)
    {
      break;
    }
  }
  while (greset.b.rxfflsh == 1);

  /* Wait for 3 PHY Clocks*/
  cmtWaitMKS(5);

  return status;
}

/*******************************************************************************
* Function Name  : OTGD_FS_SetDeviceMode
* Description    : Set device mode
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
USB_OTG_Status OTGD_FS_SetDeviceMode(void)
{

  USB_OTG_Status status = USB_OTG_OK;
  USB_OTG_GUSBCFG_TypeDef usbcfg ;
  
  usbcfg.d32 = 0;

  usbcfg.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.GREGS->GUSBCFG);

  usbcfg.b.force_dev = 1;

  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.GREGS->GUSBCFG, usbcfg.d32);

  mDELAY(50);

  return status;
}
/*******************************************************************************
* Function Name  : IsDeviceMode
* Description    : check device mode
* Input          : None
* Output         : None
* Return         : current mode
*******************************************************************************/
uint32_t USBD_FS_IsDeviceMode(void)
{
  return ((USB_OTG_READ_REG32(&USB_OTG_FS_regs.GREGS->GINTSTS ) & 0x1) == 0 );
}

/*******************************************************************************
* Function Name  : OTGD_FS_CoreInitDev
* Description    : Initialize the USB_OTG controller registers for device mode
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
USB_OTG_Status OTGD_FS_CoreInitDev (void)
{
  USB_OTG_Status status = USB_OTG_OK;
  USB_OTG_DEPCTLx_TypeDef  depctl;
  USB_OTG_DCFG_TypeDef   dcfg;
  USB_OTG_FIFOSIZ_TypeDef txfifosize0;
  USB_OTG_FIFOSIZ_TypeDef txfifosize;
  uint32_t i = 0;
  
  depctl.d32 = 0;
  dcfg.d32 = 0;
  txfifosize0.d32 = 0;
  txfifosize.d32 = 0;
  
  /* Set device speed */
  InitDevSpeed ();

  /* Restart the Phy Clock */
  USB_OTG_WRITE_REG32(USB_OTG_FS_regs.PCGCCTL, 0);

  /* Device configuration register */
  dcfg.d32 = USB_OTG_READ_REG32( &USB_OTG_FS_regs.DEV->DCFG);
  dcfg.b.perfrint = DCFG_FRAME_INTERVAL_80;
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DEV->DCFG, dcfg.d32 );
  
  /* set Rx FIFO size */
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.GREGS->GRXFSIZ, USB_MAX_PACKET0 );

  /* EP0 TX*/
  txfifosize0.b.depth     = USB_MAX_PACKET0;
  txfifosize0.b.startaddr = USB_MAX_PACKET0;
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.GREGS->DIEPTXF0, txfifosize0.d32 );

  
  /* EP1 TX*/
  txfifosize.b.startaddr = txfifosize0.b.startaddr + txfifosize0.b.depth;
  txfifosize.b.depth = USB_MAX_PACKET0;
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.GREGS->DIEPTXFx[0], txfifosize.d32 );

    
  /* EP2 TX*/
  txfifosize.b.startaddr += txfifosize.b.depth;
  txfifosize.b.depth = USB_MAX_PACKET0;
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.GREGS->DIEPTXFx[1], txfifosize.d32 );

  
  /* EP3 TX*/  
  txfifosize.b.startaddr += txfifosize.b.depth;
  txfifosize.b.depth = USB_MAX_PACKET0;
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.GREGS->DIEPTXFx[2], txfifosize.d32 );

  
  /* Flush the FIFOs */
  OTGD_FS_FlushTxFifo(0x10); /* all Tx FIFOs */
  OTGD_FS_FlushRxFifo();

  /* Clear all pending Device Interrupts */
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DEV->DIEPMSK, 0 );
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DEV->DOEPMSK, 0 );
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DEV->DAINT, 0xFFFFFFFF );
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DEV->DAINTMSK, 0 );

  for (i = 0; i < NUM_TX_FIFOS; i++)
  {
    depctl.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DINEPS[i]->DIEPCTLx);
    if (depctl.b.epena)
    {
      depctl.d32 = 0;
      depctl.b.epdis = 1;
      depctl.b.snak = 1;
    }
    else
    {
      depctl.d32 = 0;
    }

    USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DINEPS[i]->DIEPCTLx, depctl.d32);


    USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DINEPS[i]->DIEPTSIZx, 0);
    USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DINEPS[i]->DIEPINTx, 0xFF);
  }

  for (i = 0; i < 1/* NUM_OUT_EPS*/; i++)
  {
    depctl.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DOUTEPS[i]->DOEPCTLx);
    if (depctl.b.epena)
    {
      depctl.d32 = 0;
      depctl.b.epdis = 1;
      depctl.b.snak = 1;
    }
    else
    {
      depctl.d32 = 0;
    }

    USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DOUTEPS[i]->DOEPCTLx, depctl.d32);

    USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DOUTEPS[i]->DOEPTSIZx, 0);
    USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DOUTEPS[i]->DOEPINTx, 0xFF);
  }
  
  OTGD_FS_EnableDevInt();

  return status;
}
/*******************************************************************************
* Function Name  : PCD_Init
* Description    : Initialize the USB Device portion of the driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PCD_Init(void)
{
  uint32_t i = 0;
  USB_OTG_EP *ep;

  /**** SOFTWARE INIT *****/ 
  
  ep = &USB_OTG_PCD_dev.ep0;

  /* Init ep structure */
  ep->num = 0;
  ep->tx_fifo_num = 0;

  /* Control until ep is activated */
  ep->type = EP_TYPE_CTRL;
  ep->maxpacket = USB_MAX_PACKET0;

  ep->xfer_buff = 0;
  ep->xfer_len = 0;

  for (i = 1; i < NUM_TX_FIFOS ; i++)
  {
    ep = &USB_OTG_PCD_dev.in_ep[i-1];

    /* Init ep structure */
    ep->is_in = 1;
    ep->num = i;
    ep->tx_fifo_num = i;

    /* Control until ep is activated */
    ep->type = EP_TYPE_CTRL;
    ep->maxpacket = USB_MAX_PACKET0;
    ep->xfer_buff = 0;
    ep->xfer_len = 0;
  }

  for (i = 1; i < NUM_TX_FIFOS; i++)
  {
    ep = &USB_OTG_PCD_dev.out_ep[i-1];

    /* Init ep structure */
    ep->is_in = 0;
    ep->num = i;
    ep->tx_fifo_num = i;

    /* Control until ep is activated */
    ep->type = EP_TYPE_CTRL;
    ep->maxpacket = USB_MAX_PACKET0;
    ep->xfer_buff = 0;
    ep->xfer_len = 0;
  }

  USB_OTG_PCD_dev.ep0.maxpacket = USB_MAX_PACKET0;
  USB_OTG_PCD_dev.ep0.type = EP_TYPE_CTRL;

  /**** HARDWARE INIT *****/
  
  /* Set the OTG_USB base registers address */
  OTGD_FS_SetAddress(USB_OTG_FS_BASE_ADDR);
  
  /* Disable all global interrupts */
  OTGD_FS_DisableGlobalInt();

  /*Init the Core */
  OTGD_FS_CoreInit();

  /* Init Device mode*/
  OTGD_FS_CoreInitDev();  
}

/*******************************************************************************
* Function Name  : PCD_GetInEP
* Description    : This function returns pointer to IN EP struct with number ep_num
* Input          : Endpoint Number.
* Output         : None
* Return         : status
*******************************************************************************/
USB_OTG_EP* PCD_GetInEP(uint32_t ep_num)
{
  if (ep_num == 0)
  {
    return &USB_OTG_PCD_dev.ep0;
  }
  else
  {
    return &USB_OTG_PCD_dev.in_ep[ep_num - 1];
  }
}
/*******************************************************************************
* Function Name  : PCD_GetOutEP
* Description    : returns pointer to OUT EP struct with number ep_num
* Input          : Endpoint Number.
* Output         : None
* Return         : USBF_EP
*******************************************************************************/
USB_OTG_EP* PCD_GetOutEP(uint32_t ep_num)
{
  if (ep_num == 0)
  {
    return &USB_OTG_PCD_dev.ep0;
  }
  else
  {
    return &USB_OTG_PCD_dev.out_ep[ep_num - 1];
  }
}

/*******************************************************************************
* Function Name  : OTGD_FS_EPActivate
* Description    : Activates an EP
* Input          : ep
* Output         : None
* Return         : num_in_ep
*******************************************************************************/
USB_OTG_Status OTGD_FS_EPActivate(USB_OTG_EP *ep)
{

  USB_OTG_Status status = USB_OTG_OK;
  USB_OTG_DEPCTLx_TypeDef depctl;
  USB_OTG_DAINT_TypeDef   daintmsk;
  __IO uint32_t *addr;


  depctl.d32 = 0;
  daintmsk.d32 = 0;
  
  /* Read DEPCTLn register */
  if (ep->is_in == 1)
  {
    addr = &USB_OTG_FS_regs.DINEPS[ep->num]->DIEPCTLx;
    daintmsk.ep.in = 1 << ep->num;
  }
  else
  {
    addr = &USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx;
    daintmsk.ep.out = 1 << ep->num;
  }

  /* If the EP is already active don't change the EP Control
   * register. */
  depctl.d32 = USB_OTG_READ_REG32(addr);
  if (!depctl.b.usbactep)
  {
    depctl.b.mps    = ep->maxpacket;
    depctl.b.eptype = ep->type;
    depctl.b.txfnum = ep->tx_fifo_num;
    depctl.b.setd0pid = 1;
    depctl.b.usbactep = 1;
    USB_OTG_WRITE_REG32(addr, depctl.d32);
  }

  /* Enable the Interrupt for this EP */
  USB_OTG_MODIFY_REG32(&USB_OTG_FS_regs.DEV->DAINTMSK, 0, daintmsk.d32);
  return status;
}

/*******************************************************************************
* Function Name  : OTGD_FS_EPDeactivate
* Description    : Deactivates an EP
* Input          : ep
* Output         : None
* Return         : num_in_ep
*******************************************************************************/
USB_OTG_Status OTGD_FS_EPDeactivate(USB_OTG_EP *ep)
{

  USB_OTG_Status status = USB_OTG_OK;
  USB_OTG_DEPCTLx_TypeDef depctl;
  __IO uint32_t *addr;
  USB_OTG_DAINT_TypeDef daintmsk;

  depctl.d32 = 0;
  daintmsk.d32 = 0;
  
  /* Read DEPCTLn register */
  if (ep->is_in == 1)
  {
    addr = &USB_OTG_FS_regs.DINEPS[ep->num]->DIEPCTLx;
    daintmsk.ep.in = 1 << ep->num;
  }
  else
  {
    addr = &USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx;
    daintmsk.ep.out = 1 << ep->num;
  }

  depctl.b.usbactep = 0;
  USB_OTG_WRITE_REG32(addr, depctl.d32);

  /* Disable the Interrupt for this EP */
  USB_OTG_MODIFY_REG32(&USB_OTG_FS_regs.DEV->DAINTMSK, daintmsk.d32, 0);
  return status;
}


/*******************************************************************************
* Function Name  : PCD_EP_Open
* Description    : Configure an Endpoint
* Input          : None
* Output         : None
* Return         : status
*******************************************************************************/
uint32_t PCD_EP_Open(USB_ENDPOINT_DESCRIPTOR *epdesc)
{
  USB_OTG_EP *ep;


  if ((0x80 & epdesc->bEndpointAddress) != 0)
  {
    ep = PCD_GetInEP(epdesc->bEndpointAddress & 0x7F);
    ep->is_in = 1;
  }
  else
  {
    ep = PCD_GetOutEP(epdesc->bEndpointAddress & 0x7F);
    ep->is_in = 0;
  }

  ep->num   = epdesc->bEndpointAddress & 0x7F;
  ep->maxpacket = epdesc->wMaxPacketSize;
  ep->type = epdesc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK;

  if (ep->is_in)
  {
    /* Assign a Tx FIFO */
    ep->tx_fifo_num = ep->num;
  }

  OTGD_FS_EPActivate(ep );

  return 0;
}

/*******************************************************************************
* Function Name  : PCD_EP_Close
* Description    : Called when an EP is disabled
* Input          : Endpoint address.
* Output         : None
* Return         : status
*******************************************************************************/
uint32_t PCD_EP_Close(uint8_t  ep_addr)
{

  USB_OTG_EP *ep;

  if ((0x80 & ep_addr) != 0)
  {
    ep = PCD_GetInEP(ep_addr & 0x7F);
  }
  else
  {
    ep = PCD_GetOutEP(ep_addr & 0x7F);
  }

  ep->num   = ep_addr & 0x7F;
  ep->is_in = (0x80 & ep_addr) != 0;

  OTGD_FS_EPDeactivate(ep );
  return 0;
}

/*******************************************************************************
* Function Name  : OTGD_FS_EP0StartXfer
* Description    : Handle the setup for a data xfer for EP0 and starts the xfer
* Input          : None
* Output         : None
* Return         : status
*******************************************************************************/
USB_OTG_Status OTGD_FS_EP0StartXfer(USB_OTG_EP *ep)
{

  USB_OTG_Status                    status = USB_OTG_OK;
  uint32_t                          fifoemptymsk = 0;
  USB_OTG_DEPCTLx_TypeDef           depctl;
  OTG_FS_DEPTSIZx_TypeDef           deptsiz;
  USB_OTG_DINEPS                    *in_regs ;

  depctl.d32 = 0;
  deptsiz.d32 = 0;
  
  /* IN endpoint */
  if (ep->is_in == 1)
  {
    in_regs = USB_OTG_FS_regs.DINEPS[0];
    depctl.d32  = USB_OTG_READ_REG32(&in_regs->DIEPCTLx);
    deptsiz.d32 = USB_OTG_READ_REG32(&in_regs->DIEPTSIZx);

    /* Zero Length Packet? */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = 0;
      deptsiz.b.pktcnt = 1;
    }
    else
    {
      if (ep->xfer_len > ep->maxpacket)
      {
        ep->xfer_len = ep->maxpacket;
        deptsiz.b.xfersize = ep->maxpacket;
      }
      else
      {
        deptsiz.b.xfersize = ep->xfer_len;
      }
      deptsiz.b.pktcnt = 1;

    }
    USB_OTG_WRITE_REG32(&in_regs->DIEPTSIZx, deptsiz.d32);

    /* EP enable, IN data in FIFO */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32(&in_regs->DIEPCTLx, depctl.d32);

    /* Enable the Tx FIFO Empty Interrupt for this EP */
    if (ep->xfer_len > 0)
    {
      fifoemptymsk |= 1 << ep->num;
      USB_OTG_MODIFY_REG32(&USB_OTG_FS_regs.DEV->DIEPEMPMSK, 0, fifoemptymsk);
    }
  }
  else
  {
    /* OUT endpoint */
    depctl.d32  = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DOUTEPS[0]->DOEPCTLx);
    deptsiz.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DOUTEPS[0]->DOEPTSIZx);

    /* Program the transfer size and packet count as follows:
     * xfersize = N * (maxpacket + 4 - (maxpacket % 4))
     * pktcnt = N           */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = ep->maxpacket;
      deptsiz.b.pktcnt = 1;
    }
    else
    {
      deptsiz.b.pktcnt = (ep->xfer_len + (ep->maxpacket - 1)) / ep->maxpacket;
      deptsiz.b.xfersize = deptsiz.b.pktcnt * ep->maxpacket;
    }

    USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DOUTEPS[0]->DOEPTSIZx, deptsiz.d32);

    /* EP enable */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32 (&(USB_OTG_FS_regs.DOUTEPS[0]->DOEPCTLx), depctl.d32);
  }
  return status;
}

/*******************************************************************************
* Function Name  : OTGD_FS_WritePacket
* Description    : Writes a packet into the Tx FIFO associated with the EP
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
USB_OTG_Status OTGD_FS_WritePacket(uint8_t *src, uint8_t ep_num, uint16_t bytes)
{
  USB_OTG_Status status = USB_OTG_OK;
  uint32_t dword_count = 0 , i = 0;
  __IO uint32_t *fifo;

  /* Find the DWORD length, padded by extra bytes as necessary if MPS
   * is not a multiple of DWORD */
  dword_count =  (bytes + 3) / 4;

  fifo = USB_OTG_FS_regs.FIFO[ep_num];
  uint32 v;
  for (i = 0; i < dword_count; i++ ) {
    v = (src[0] & 0xff) | ((src[1] << 8) & 0xff00) | ((src[2] << 16) & 0xff0000) | ((src[3] << 24) & 0xff000000);
    USB_OTG_WRITE_REG32( fifo, v );
    src += 4;
    }

  return status;
  }
/*******************************************************************************
* Function Name  : OTGD_FS_ReadPacket
* Description    : Reads a packet from the Rx FIFO
* Input          : None
* Output         : None
* Return         : status
*******************************************************************************/
void* OTGD_FS_ReadPacket(uint8_t *dest, uint16_t bytes)
{
  uint32_t i = 0;
  uint32_t word_count = (bytes + 3) / 4;

  __IO uint32_t *fifo = USB_OTG_FS_regs.FIFO[0];
  uint32 v;

  for (i = 0; i < word_count; i++ ) {
    v = USB_OTG_READ_REG32(fifo);
    dest[0] = v;
    dest[1] = v >> 8;
    dest[2] = v >> 16;
    dest[3] = v >> 24;
    dest += 4;
    }

  /* Return the buffer pointer because if the transfer is composed of several packets,
     the data of the next packet must be stored following the previous packet's data         */
  return ((void *)dest);
}


/*******************************************************************************
* Function Name  : OTGD_FS_EPStartXfer
* Description    : Handle the setup for data xfer for an EP and starts the xfer
* Input          : None
* Output         : None
* Return         : status
*******************************************************************************/
USB_OTG_Status OTGD_FS_EPStartXfer(USB_OTG_EP *ep)
{
  USB_OTG_DSTS_TypeDef dsts;  
  USB_OTG_Status status = USB_OTG_OK;
  __IO USB_OTG_DEPCTLx_TypeDef depctl;
  OTG_FS_DEPTSIZx_TypeDef deptsiz;

  depctl.d32 = 0;
  deptsiz.d32 = 0;
  
  /* IN endpoint */
  if (ep->is_in == 1)
  {

    depctl.d32  = USB_OTG_READ_REG32(&(USB_OTG_FS_regs.DINEPS[ep->num]->DIEPCTLx));
    deptsiz.d32 = USB_OTG_READ_REG32(&(USB_OTG_FS_regs.DINEPS[ep->num]->DIEPTSIZx));

    /* Zero Length Packet? */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = 0;
      deptsiz.b.pktcnt = 1;

    }
    else
    {
      /* Program the transfer size and packet count
       * as follows: xfersize = N * maxpacket +
       * short_packet pktcnt = N + (short_packet
       * exist ? 1 : 0)
       */
      deptsiz.b.xfersize = ep->xfer_len;
      deptsiz.b.pktcnt = (ep->xfer_len - 1 + ep->maxpacket) / ep->maxpacket;
      
      if (ep->type == EP_TYPE_ISOC)
      {
        deptsiz.b.mcount = 1;
      }      
    }
    USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DINEPS[ep->num]->DIEPTSIZx, deptsiz.d32);

    if (ep->type != EP_TYPE_ISOC)
    {
      /* Enable the Tx FIFO Empty Interrupt for this EP */
      uint32_t fifoemptymsk = 0;
      fifoemptymsk = 1 << ep->num;
      USB_OTG_MODIFY_REG32(&USB_OTG_FS_regs.DEV->DIEPEMPMSK, 0, fifoemptymsk);
    }
   
    /* EP enable, IN data in FIFO */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    
    if (ep->type == EP_TYPE_ISOC)
    {
      dsts.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DSTS);
      
      if (((dsts.b.soffn)&0x1)==0)
      {
        depctl.b.setoddfrm=1;
      }
      else
      {
        depctl.b.setd0pid=1;
      }
    }  
    
    USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DINEPS[ep->num]->DIEPCTLx, depctl.d32); 
    
    if (ep->type == EP_TYPE_ISOC)
    {
      /*write buffer in TXFIFO*/
      /* user should ensure that ep->xfer_len <= ep->maxpacket */
      OTGD_FS_WritePacket(ep->xfer_buff, ep->num, ep->xfer_len);    
    }
  }
  else
  {
    /* OUT endpoint */
    depctl.d32  = USB_OTG_READ_REG32(&(USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx));
    deptsiz.d32 = USB_OTG_READ_REG32(&(USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPTSIZx));

    /* Program the transfer size and packet count as follows:
     * pktcnt = N
     * xfersize = N * maxpacket
     */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = ep->maxpacket;
      deptsiz.b.pktcnt = 1;
    }
    else
    {
      deptsiz.b.pktcnt = (ep->xfer_len + (ep->maxpacket - 1)) / ep->maxpacket;
      deptsiz.b.xfersize = deptsiz.b.pktcnt * ep->maxpacket;
    }
    USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPTSIZx, deptsiz.d32);

    if (ep->type == EP_TYPE_ISOC)
    {

      if (ep->even_odd_frame)
      {
        depctl.b.setoddfrm = 1;
      }
      else
      {
        depctl.b.setd0pid = 1;
      }
    }

    /* EP enable */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;

    USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx, depctl.d32);

  }
  return status;
}

/*******************************************************************************
* Function Name  : PCD_EP_Read
* Description    : Read data from Fifo
* Input          : Endpoint address.
* Output         : None
* Return         : status
*******************************************************************************/
uint32_t PCD_EP_Read (uint8_t ep_addr, uint8_t *pbuf, uint32_t buf_len)
{
  USB_OTG_EP *ep;
  uint32_t i = 0;

  ep = PCD_GetOutEP(ep_addr & 0x7F);

  /* copy received data into application buffer */
  for (i = 0 ; i < buf_len ; i++)
  {
    pbuf[i] = ep->xfer_buff[i];
  }

  /*setup and start the Xfer */
  ep->xfer_buff = pbuf;
  ep->xfer_len = buf_len;
  ep->xfer_count = 0;
  ep->is_in = 0;
  ep->num = ep_addr & 0x7F;

  if ( ep->num == 0 )
  {
    OTGD_FS_EP0StartXfer(ep);
  }
  else
  {
    OTGD_FS_EPStartXfer( ep );
  }

  return 0;
}


/*******************************************************************************
* Function Name  : USBF_EP_Write
* Description    : Write Data to Fifo
* Input          : ep
* Output         : None
* Return         : status
*******************************************************************************/
uint32_t  PCD_EP_Write (uint8_t ep_addr, uint8_t *pbuf, uint32_t buf_len)
{
  USB_OTG_EP *ep;

  ep = PCD_GetInEP(ep_addr & 0x7f);

  /* assign data to EP structure buffer */
  ep->xfer_buff = pbuf;

  /* Setup and start the Transfer */
  ep->xfer_count = 0;
  ep->xfer_len = buf_len;
  ep->is_in = 1;
  ep->num = ep_addr & 0x7F;
  
  if ( ep->num == 0 )
  {
    OTGD_FS_EP0StartXfer(ep);
  }
  else
  {
    OTGD_FS_EPStartXfer( ep );
  }

  return 0;
}

/*******************************************************************************
* Function Name  : OTGD_FS_EPSetStall
* Description    : Set the EP STALL
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
USB_OTG_Status OTGD_FS_EPSetStall(USB_OTG_EP *ep)
{
  USB_OTG_Status status = USB_OTG_OK;
  USB_OTG_DEPCTLx_TypeDef depctl; 
  __IO uint32_t *depctl_addr;

  depctl.d32 = 0;
  
  
  if (ep->is_in == 1)
  {
    depctl_addr = &(USB_OTG_FS_regs.DINEPS[ep->num]->DIEPCTLx);
    depctl.d32 = USB_OTG_READ_REG32(depctl_addr);

    /* set the disable and stall bits */
    if (depctl.b.epena)
    {
      depctl.b.epdis = 1;
    }
    depctl.b.stall = 1;
    USB_OTG_WRITE_REG32(depctl_addr, depctl.d32);
  }
  else
  {
    depctl_addr = &(USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx);
    depctl.d32 = USB_OTG_READ_REG32(depctl_addr);

    /* set the stall bit */
    depctl.b.stall = 1;
    USB_OTG_WRITE_REG32(depctl_addr, depctl.d32);
  }
  return status;
}

/*******************************************************************************
* Function Name  : OTGD_FS_EPClearStall
* Description    : Clear the EP STALL
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
USB_OTG_Status OTGD_FS_EPClearStall(USB_OTG_EP *ep)
{
  USB_OTG_Status status = USB_OTG_OK;
  USB_OTG_DEPCTLx_TypeDef depctl;
  __IO uint32_t *depctl_addr;

  
  depctl.d32 = 0;
  
  if (ep->is_in == 1)
  {
    depctl_addr = &(USB_OTG_FS_regs.DINEPS[ep->num]->DIEPCTLx);
  }
  else
  {
    depctl_addr = &(USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx);
  }

  
  depctl.d32 = USB_OTG_READ_REG32(depctl_addr);
   
  /* clear the stall bits */
  depctl.b.stall = 0;

  if (ep->type == EP_TYPE_INTR || ep->type == EP_TYPE_BULK) 
  {
    depctl.b.setd0pid = 1; /* DATA0 */
  }

  USB_OTG_WRITE_REG32(depctl_addr, depctl.d32);
  return status;
}

/*******************************************************************************
* Function Name  : PCD_EP_Stall
* Description    : Stall an endpoint.
* Input          : Endpoint Address.
* Output         : None
* Return         : status
*******************************************************************************/
uint32_t  PCD_EP_Stall (uint8_t ep_addr)
{
  USB_OTG_EP *ep;

  if ((0x80 & ep_addr) != 0)
  {
    ep = PCD_GetInEP(ep_addr & 0x7F);
  }
  else
  {
    ep = PCD_GetOutEP(ep_addr & 0x7F);
  }

  ep->num   = ep_addr & 0x7F;
  ep->is_in = ((ep_addr & 0x80) == 0x80) ? 1 : 0;

  OTGD_FS_EPSetStall(ep);
  return (0);
}
/*******************************************************************************
* Function Name  : PCD_EP_ClrStall
* Description    : Clear stall condition on endpoints.
* Input          : Endpoint Address.
* Output         : None
* Return         : status
*******************************************************************************/
uint32_t  PCD_EP_ClrStall (uint8_t ep_addr)
{

  USB_OTG_EP *ep;

  if ((0x80 & ep_addr) != 0)
  {
    ep = PCD_GetInEP(ep_addr & 0x7F);
  }
  else
  {
    ep = PCD_GetOutEP(ep_addr & 0x7F);
  }

  ep->num   = ep_addr & 0x7F;
  ep->is_in = ((ep_addr & 0x80) == 0x80) ? 1 : 0;

  OTGD_FS_EPClearStall(ep);

  return (0);
}

/*******************************************************************************
* Function Name  : USBF_FCD_EP_Flush()
* Description    : This Function flushes the buffer.
* Input          : Endpoint Address.
* Output         : None
* Return         : status
*******************************************************************************/
uint32_t  PCD_EP_Flush (uint8_t ep_addr)
{

  uint8_t  is_out = 0;
  uint8_t  ep_nbr = 0;

  ep_nbr   = ep_addr & 0x7F;
  is_out = ((ep_addr & 0x80) == 0x80) ? 0 : 1;

  if (is_out == 0)
  {
    OTGD_FS_FlushTxFifo(ep_nbr);
  }
  else
  {
    OTGD_FS_FlushRxFifo();
  }
  PCD_EP_ClrStall(ep_addr);
  return (0);
}

/*******************************************************************************
* Function Name  : PCD_EP_SetAddress
* Description    : This Function set USB device address
* Input          : The new device Address to be set.
* Output         : None
* Return         : status
*******************************************************************************/
void  PCD_EP_SetAddress (uint8_t address)
{

  USB_OTG_DCFG_TypeDef dcfg;

  dcfg.d32 = 0;
  
  dcfg.b.devaddr = address;
  USB_OTG_MODIFY_REG32( &USB_OTG_FS_regs.DEV->DCFG, 0, dcfg.d32);
}


/*******************************************************************************
* Function Name  : PCD_DevConnect
* Description    : Connect device
* Input         : None
* Output         : None
* Return         : status
*******************************************************************************/
void  PCD_DevConnect(void)
{

  USB_OTG_DCTL_TypeDef dctl;
  
  dctl.d32 = 0;

  dctl.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DCTL);

  /* Connect device */
  dctl.b.sftdiscon  = 0;
  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DEV->DCTL, dctl.d32);
  mDELAY(25);
}

/*******************************************************************************
* Function Name  : PCD_DevDisconnect
* Description    : Disconnect device
* Input         : None
* Output         : None
* Return         : status
*******************************************************************************/
void  PCD_DevDisconnect (void)
{

  USB_OTG_DCTL_TypeDef dctl;

  dctl.d32 = 0;
  
  dctl.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DCTL);

  /* Disconnect device for 20ms */
  dctl.b.sftdiscon  = 1;
  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DEV->DCTL, dctl.d32);
  mDELAY(25);
}

/*******************************************************************************
* Function Name  : PCD_EP0_OutStart
* Description    : Configures EPO to receive SETUP packets.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PCD_EP0_OutStart(void)
{

  USB_OTG_DOEPTSIZ0_TypeDef doeptsize0;
  doeptsize0.d32 = 0;
  
  
  doeptsize0.b.supcnt = 3;
  doeptsize0.b.pktcnt = 1;
  doeptsize0.b.xfersize = 8 * 3;

  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DOUTEPS[0]->DOEPTSIZx, doeptsize0.d32 );

}

//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================

USB_OTG_CORE_REGS     USB_OTG_FS_regs;


extern uint32_t STM32_PCD_OTG_ISR_Handler (void);

/******************************************************************************/
/*                           Common Core Layer                                */
/******************************************************************************/



/*******************************************************************************
* Function Name  : OTGD_FS_ReadCoreItr
* Description    : returns the Core Interrupt register
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t OTGD_FS_ReadCoreItr(void)
{
  uint32_t v = 0;

  v = USB_OTG_READ_REG32(&USB_OTG_FS_regs.GREGS->GINTSTS);
  v &= USB_OTG_READ_REG32(&USB_OTG_FS_regs.GREGS->GINTMSK);

  return v;
}

/*******************************************************************************
* Function Name  : OTGD_FS_ReadOtgItr
* Description    : returns the USB_OTG Interrupt register
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t OTGD_FS_ReadOtgItr (void)
{
  return (USB_OTG_READ_REG32 (&USB_OTG_FS_regs.GREGS->GOTGINT));
}

/******************************************************************************/
/*                           PCD Core Layer                                   */
/******************************************************************************/

/*******************************************************************************
* Function Name  : OTGD_FS_EP0Activate
* Description    : enables EP0 OUT to receive SETUP packets and configures EP0
                   IN for transmitting packets
* Input          : None
* Output         : None
* Return         : status
*******************************************************************************/
USB_OTG_Status  OTGD_FS_EP0Activate(void)
{
  USB_OTG_Status          status = USB_OTG_OK;
  USB_OTG_DEPCTLx_TypeDef diepctl;
  USB_OTG_DCTL_TypeDef    dctl;

  diepctl.d32 = 0;
  dctl.d32 = 0;
  
  diepctl.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DINEPS[0]->DIEPCTLx);
  diepctl.b.mps = DEP0CTL_MPS_64;
  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DINEPS[0]->DIEPCTLx, diepctl.d32);

  dctl.b.cgnpinnak = 1;
  USB_OTG_MODIFY_REG32(&USB_OTG_FS_regs.DEV->DCTL, dctl.d32, dctl.d32);
  
  return status;
}


/*******************************************************************************
* Function Name  : OTGD_FS_ReadDevAllOutEp_itr
* Description    : returns the OUT endpoint interrupt bits
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t OTGD_FS_ReadDevAllOutEp_itr(void)
{
  uint32_t v = 0;
  
  v  = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DAINT);
  v &= USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DAINTMSK);
  return ((v & 0xffff0000) >> 16);
}

/*******************************************************************************
* Function Name  : OTGD_FS_ReadDevOutEP_itr
* Description    : returns the Device OUT EP Interrupt register
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t OTGD_FS_ReadDevOutEP_itr(USB_OTG_EP *ep)
{
  uint32_t v = 0;
  
  v  = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPINTx);
  v &= USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DOEPMSK);
  return v;
}
/*******************************************************************************
* Function Name  : OTGD_FS_ReadDevAllInEPItr
* Description    : Get int status register
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t OTGD_FS_ReadDevAllInEPItr(void)
{
  uint32_t v = 0;
  
  v = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DAINT);
  v &= USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DAINTMSK);
  return (v & 0xffff);
}

/*******************************************************************************
* Function Name  : OTGD_FS_GetEPStatus
* Description    : returns the EP Status 
* Input          : - ep: pointer to the EP structure
* Output         : None
* Return         : status: DEV_EP_TX_STALL, DEV_EP_TX_VALID, DEV_EP_TX_NAK, 
*                  DEV_EP_RX_STALL, DEV_EP_RX_VALID or DEV_EP_RX_NAK,
*******************************************************************************/
uint32_t OTGD_FS_GetEPStatus(USB_OTG_EP *ep)
{
  USB_OTG_DEPCTLx_TypeDef depctl;
  __IO uint32_t *depctl_addr;
  uint32_t Status = 0;
  
  depctl.d32 = 0;

  if (ep->is_in == 1)
  {
    depctl_addr = &(USB_OTG_FS_regs.DINEPS[ep->num]->DIEPCTLx);
  }
  else
  {
    depctl_addr = &(USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx);
  }

  depctl.d32 = USB_OTG_READ_REG32(depctl_addr);

  /* Process for IN endpoint */
  if (ep->is_in == 1)
  {
    if (depctl.b.stall == 1)  
      Status = DEV_EP_TX_STALL;
    else if (depctl.b.naksts == 1)
      Status = DEV_EP_TX_NAK;
    else 
      Status = DEV_EP_TX_VALID; 
  } 
  /* Process for OUT endpoint */
  else 
  {
    if (depctl.b.stall == 1)  
      Status = DEV_EP_RX_STALL;
    else if (depctl.b.naksts == 1)
      Status = DEV_EP_RX_NAK;
    else 
      Status = DEV_EP_RX_VALID; 
  }
  
  /* Return the current status */
  return Status;
}

/*******************************************************************************
* Function Name  : OTGD_FS_SetEPStatus
* Description    : Sets the EP Status 
* Input          : - ep: pointer to the EP structure
*                  - Status: new status to be set
* Output         : None
* Return         : None
*******************************************************************************/
void OTGD_FS_SetEPStatus(USB_OTG_EP *ep, uint32_t Status)
{
  USB_OTG_DEPCTLx_TypeDef depctl;
  __IO uint32_t *depctl_addr;

  depctl.d32 = 0;
  
  
  if (ep->is_in == 1)
  {
    depctl_addr = &(USB_OTG_FS_regs.DINEPS[ep->num]->DIEPCTLx);
  }
  else
  {
    depctl_addr = &(USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx);
  }

  depctl.d32 = USB_OTG_READ_REG32(depctl_addr);

  /* Process for IN endpoint */
  if (ep->is_in == 1)
  {
    if (Status == DEV_EP_TX_STALL)  
    {
      OTGD_FS_EPSetStall(ep); return;
    }
    else if (Status == DEV_EP_TX_NAK)
      depctl.b.snak = 1;
    else if (Status == DEV_EP_TX_VALID)
    {
      if (depctl.b.stall == 1)
      {  
        ep->even_odd_frame = 0;
        OTGD_FS_EPClearStall(ep);
        return;
      }      
      depctl.b.cnak = 1;
      depctl.b.usbactep = 1; 
      depctl.b.epena = 1;
    }
    else if (Status == DEV_EP_TX_DIS)
      depctl.b.usbactep = 0;
  } 
  else /* Process for OUT endpoint */
  {
    if (Status == DEV_EP_RX_STALL)  {
      depctl.b.stall = 1;
    }
    else if (Status == DEV_EP_RX_NAK)
      depctl.b.snak = 1;
    else if (Status == DEV_EP_RX_VALID)
    {
      if (depctl.b.stall == 1)
      {  
        ep->even_odd_frame = 0;
        OTGD_FS_EPClearStall(ep);
        return;
      }  
      depctl.b.cnak = 1;
      depctl.b.usbactep = 1;    
      depctl.b.epena = 1;
    }
    else if (Status == DEV_EP_RX_DIS)
    {
      depctl.b.usbactep = 0;    
    }
  }

  USB_OTG_WRITE_REG32(depctl_addr, depctl.d32); 
}

/*******************************************************************************
* Function Name  : OTGD_FS_SetRemoteWakeup
* Description    : Enable Remote wakeup signaling
* Input          : None
* Output         : None
* Return         : status
*******************************************************************************/
void OTGD_FS_SetRemoteWakeup()
{
  USB_OTG_DCTL_TypeDef devctl;
  
  devctl.d32 = 0;
  
  devctl.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DCTL);
  
  /* Enable the Remote Wakeup signal */
  devctl.b.rmtwkupsig = 1;
  
  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DEV->DCTL, devctl.d32);
}

/*******************************************************************************
* Function Name  : OTGD_FS_ResetRemoteWakeup
* Description    : Disable Remote wakeup signaling
* Input          : None
* Output         : None
* Return         : status
*******************************************************************************/
void OTGD_FS_ResetRemoteWakeup()
{
 USB_OTG_DCTL_TypeDef devctl;

 
 devctl.d32 = 0;
 
 devctl.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DCTL);
 
 /* Disable the Remote Wakeup signal */
 devctl.b.rmtwkupsig = 0;
 
 USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DEV->DCTL, devctl.d32);
}

//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
/*******************************************************************************
* Function Name  : OTG_DEV_Init
* Description    : Initialize the OTG Device IP and EP0.
* Input          : None.
* Output         : None.
* Return         : None. 
*******************************************************************************/
void OTG_DEV_Init(void)
{
  USB_ENDPOINT_DESCRIPTOR ep_descriptor;
  
  /* Init peripheral driver */
  PCD_Init();
  
  /* Configure and open the IN control EP0 */ 
  ep_descriptor.bEndpointAddress = 0x80;
  ep_descriptor.wMaxPacketSize = 64;  
  ep_descriptor.bmAttributes = USB_ENDPOINT_XFER_CONTROL; 
  PCD_EP_Open(&ep_descriptor);
  
  /* Configure and open the OUT control EP0 */ 
  ep_descriptor.bEndpointAddress = 0x00;
  PCD_EP_Open(&ep_descriptor);    

  OTGD_FS_EPStartXfer(PCD_GetOutEP(0));
  
  /* Enable EP0 to start receiving setup packets */  
  PCD_EP0_OutStart();  
  
  /* Enable USB Global interrupt */
  OTGD_FS_EnableGlobalInt();     
}

/*******************************************************************************
* Function Name  : OTG_DEV_SetEPTxStatus
* Description    : Sets the related endpoint status.
* Input          : - bEpnum: Number of the endpoint.
*                  - Status: New status to be set for the endpoint. It can be
*                    DEV_EP_TX_VALID, DEV_EP_TX_STALL, DEV_EP_TX_NAK or 
*                    DEV_EP_TX_DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/
void OTG_DEV_SetEPTxStatus(uint8_t bEpnum, uint32_t Status) 
{
  USB_OTG_EP *ep;
   
  ep = PCD_GetInEP(bEpnum & 0x7F); 
  
  if ((bEpnum == 0x80) && (Status == DEV_EP_TX_STALL))
  {
    ep->is_in = 1;
  }
  
  OTGD_FS_SetEPStatus(ep, Status); 
}

/*******************************************************************************
* Function Name  : OTG_DEV_EP_Init
* Description    : Initialize the selected endpoint parameters
* Input          : - bEpAdd: address of the endpoint (epnum|epdir) 
*                     example: EP1 OUT -> 0x01 and EP1 IN 0x81.
*                  - bEpType: OTG_DEV_EP_TYPE_CONTROL, OTG_DEV_EP_TYPE_ISOC, 
*                     OTG_DEV_EP_TYPE_BULK, OTG_DEV_EP_TYPE_INT
*                  - wEpMaxPackSize: The EP max packet size.
* Output         : None.
* Return         : Status: New status to be set for the endpoint: 
*******************************************************************************/
void OTG_DEV_EP_Init(uint8_t bEpAdd, uint8_t bEpType, uint16_t wEpMaxPackSize)
{
  USB_ENDPOINT_DESCRIPTOR ep_descriptor;
  USB_OTG_EP *ep;
  
  /* Set the EP parameters in a structure */
  ep_descriptor.bEndpointAddress = bEpAdd;
  ep_descriptor.bmAttributes = bEpType; 
  ep_descriptor.wMaxPacketSize = wEpMaxPackSize;

  PCD_EP_Flush(bEpAdd);
  
  /* Open the EP with entered parameters */   
  PCD_EP_Open(&ep_descriptor); 
  
  /* Activate the EP if it is an OUT EP */
  if ((bEpAdd & 0x80) == 0)
  {
    ep = PCD_GetOutEP(bEpAdd & 0x7F);
    OTGD_FS_EPStartXfer(ep);
  } 
  else
  {
    ep = PCD_GetInEP(bEpAdd & 0x7F);
    ep->even_odd_frame = 0;    
    OTG_DEV_SetEPTxStatus(bEpAdd, DEV_EP_TX_NAK);
  }
  
}

/*******************************************************************************
* Function Name  : OTG_DEV_GetEPTxStatus
* Description    : Set the related endpoint status.
* Input          : Number of the endpoint.
* Output         : None.
* Return         : Status: New status to be set for the endpoint: 
*******************************************************************************/
uint32_t OTG_DEV_GetEPTxStatus(uint8_t bEpnum) 
{
  USB_OTG_EP *ep;
  uint32_t status = 0;
  
  ep = PCD_GetInEP(bEpnum & 0x7F); 
  
  status = OTGD_FS_GetEPStatus(ep); 
  
  return status; 
}

/*******************************************************************************
* Function Name  : OTG_DEV_GetEPRxStatus
* Description    : returns the related endpoint status.
* Input          : Number of the endpoint.
* Output         : None.
* Return         : Status: New status to be set for the endpoint: 
*******************************************************************************/
uint32_t OTG_DEV_GetEPRxStatus(uint8_t bEpnum)
{
  USB_OTG_EP *ep;
  uint32_t status = 0;
  
  ep = PCD_GetOutEP(bEpnum & 0x7F); 
  
  status = OTGD_FS_GetEPStatus(ep); 
  
  return status;
}


/*******************************************************************************
* Function Name  : OTG_DEV_SetEPRxStatus
* Description    : Sets the related endpoint status.
* Input          : - bEpnum: Number of the endpoint.
*                  - Status: New status to be set for the endpoint. It can be
*                    DEV_EP_RX_VALID, DEV_EP_RX_STALL, DEV_EP_RX_NAK or 
*                    DEV_EP_RX_DISABLE.
* Output         : None.
* Return         : None.
*******************************************************************************/
void OTG_DEV_SetEPRxStatus(uint8_t bEpnum, uint32_t Status)                           
{
  USB_OTG_EP *ep;
 
  ep = PCD_GetOutEP(bEpnum & 0x7F); 
  
  OTGD_FS_SetEPStatus(ep, Status); 
}

/*******************************************************************************
* Function Name  : USB_DevDisconnect
* Description    : Disconnect the Pull-up resist.
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_DevDisconnect(void)
{
  PCD_DevDisconnect();
}

/*******************************************************************************
* Function Name  : USB_DevConnect
* Description    : Disconnect the .
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_DevConnect(void)
{
  PCD_DevConnect();
}

/*-*-*-*-*-*-*-*-*-* Replace the usb_regs.h defines -*-*-*-*-*-*-*-*-*-*-*-*-*/

/*******************************************************************************
* Function Name  : SetEPTxStatus
* Description    : Set the status of Tx endpoint.
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPTxStatus(uint8_t bEpNum, uint16_t wState)
{
  _SetEPTxStatus(bEpNum, wState);
}

/*******************************************************************************
* Function Name  : SetEPRxStatus
* Description    : Set the status of Rx endpoint.
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPRxStatus(uint8_t bEpNum, uint16_t wState)
{
  _SetEPRxStatus(bEpNum, wState);
}

/*******************************************************************************
* Function Name  : GetEPTxStatus
* Description    : Returns the endpoint Tx status.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint TX Status
*******************************************************************************/
uint16_t GetEPTxStatus(uint8_t bEpNum) 
{
  return(_GetEPTxStatus(bEpNum));
}

/*******************************************************************************
* Function Name  : GetEPRxStatus
* Description    : Returns the endpoint Rx status.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint RX Status
*******************************************************************************/
uint16_t GetEPRxStatus(uint8_t bEpNum) 
{
  return(_GetEPRxStatus(bEpNum));
}

/*******************************************************************************
* Function Name  : SetEPTxValid
* Description    : Valid the endpoint Tx Status.
* Input          : bEpNum: Endpoint Number.  
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPTxValid(uint8_t bEpNum)
{
  _SetEPTxStatus(bEpNum, EP_TX_VALID);
}

/*******************************************************************************
* Function Name  : SetEPRxValid
* Description    : Valid the endpoint Rx Status.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPRxValid(uint8_t bEpNum)
{
  _SetEPRxStatus(bEpNum, EP_RX_VALID);
}

/*******************************************************************************
* Function Name  : GetTxStallStatus
* Description    : Returns the Stall status of the Tx endpoint.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Tx Stall status.
*******************************************************************************/
uint16_t GetTxStallStatus(uint8_t bEpNum)
{
  return(_GetTxStallStatus(bEpNum));
}

/*******************************************************************************
* Function Name  : GetRxStallStatus
* Description    : Returns the Stall status of the Rx endpoint. 
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Rx Stall status.
*******************************************************************************/
uint16_t GetRxStallStatus(uint8_t bEpNum)
{
  return(_GetRxStallStatus(bEpNum));
}

/*******************************************************************************
* Function Name  : SetEPTxCount.
* Description    : Set the Tx count.
* Input          : bEpNum: Endpoint Number.
*                  wCount: new count value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPTxCount(uint8_t bEpNum, uint16_t wCount)
{
}

/*******************************************************************************
* Function Name  : SetEPRxCount
* Description    : Set the Rx count.
* Input          : bEpNum: Endpoint Number. 
*                  wCount: the new count value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPRxCount(uint8_t bEpNum, uint16_t wCount)
{ 
}

/*******************************************************************************
* Function Name  : ToWord
* Description    : merge two byte in a word.
* Input          : bh: byte high, bl: bytes low.
* Output         : None.
* Return         : resulted word.
*******************************************************************************/
uint16_t ToWord(uint8_t bh, uint8_t bl)
{
  uint16_t wRet = 0;
  wRet = (uint16_t)bl | ((uint16_t)bh << 8);
  return(wRet);
}

/*******************************************************************************
* Function Name  : ByteSwap
* Description    : Swap two byte in a word.
* Input          : wSwW: word to Swap.
* Output         : None.
* Return         : resulted word.
*******************************************************************************/
uint16_t ByteSwap(uint16_t wSwW)
{
  uint8_t bTemp = 0;
  uint16_t wRet = 0;
  
  bTemp = (uint8_t)(wSwW & 0xff);
  wRet =  (wSwW >> 8) | ((uint16_t)bTemp << 8);
  return(wRet);
}

//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
//==================================================================================================================================================================================
uint8_t USBD_Data_Buffer  [USB_MAX_PACKET0*4];
//__IO uint8_t IsocBuff [(ISOC_BUFFER_SZE * NUM_SUB_BUFFERS)];
//__IO uint32_t IsocBufferIdx = 0;

extern USB_OTG_CORE_REGS  USB_OTG_FS_regs;

__IO uint16_t SaveRState;
__IO uint16_t SaveTState;


/* Private function prototypes -----------------------------------------------*/
static uint32_t PCD_ReadDevInEP( USB_OTG_EP *ep);
static uint32_t PCD_WriteEmptyTxFifo(uint32_t epnum);

/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : PCD_ReadDevInEP
* Description    : Reads all the Endpoints flags.
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
static uint32_t PCD_ReadDevInEP( USB_OTG_EP *ep)
{
  uint32_t v = 0, msk = 0, emp=0;
  
  msk = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DIEPMSK);
  emp = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DEV->DIEPEMPMSK);
  msk |= ((emp >> ep->num) & 0x1) << 7;
  v = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DINEPS[ep->num]->DIEPINTx) & msk;
  
  return v;
}

/*******************************************************************************
* Function Name  : PCD_WriteEmptyTxFifo
* Description    : Checks Fifo for the next packet to be loaded.
* Input          : None
* Output         : None
* Return         : Status
*******************************************************************************/
static uint32_t PCD_WriteEmptyTxFifo(uint32_t epnum)
{
  USB_OTG_DTXFSTS_TypeDef txstatus;
  USB_OTG_EP *ep;
  uint32_t len = 0;
  uint32_t dwords = 0;
  uint32_t fifoemptymsk = 0;
  
  txstatus.d32 = 0;
  
  ep = PCD_GetInEP(epnum); 
  
  len = ep->xfer_len - ep->xfer_count;

  if (len > ep->maxpacket)
  {
    len = ep->maxpacket;
  }
  
  dwords = (len + 3) / 4;
  txstatus.d32 = USB_OTG_READ_REG32( &USB_OTG_FS_regs.DINEPS[epnum]->DTXFSTSx);

  
  while( (txstatus.b.txfspcavail > dwords) && (ep->xfer_count < ep->xfer_len) && (ep->xfer_len) != 0 ) {
    len = ep->xfer_len - ep->xfer_count;

    if( len > ep->maxpacket ) {
      len = ep->maxpacket;
      }
    dwords = (len + 3) / 4;

    OTGD_FS_WritePacket(ep->xfer_buff, epnum, len);    
    
    ep->xfer_count += len;
    ep->xfer_buff += len; 

    txstatus.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.DINEPS[epnum]->DTXFSTSx); 
    
    /* Mask the TxFIFOEmpty interrupt to prevent re-entring this routine */
    if (ep->xfer_len == ep->xfer_count) {
      fifoemptymsk = 0x1 << ep->num;
      USB_OTG_MODIFY_REG32(&USB_OTG_FS_regs.DEV->DIEPEMPMSK, fifoemptymsk, 0);    
      }
    }
  
  return 1;
}
