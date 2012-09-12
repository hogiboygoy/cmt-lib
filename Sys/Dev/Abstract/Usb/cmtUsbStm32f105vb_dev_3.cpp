//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

void
CmtUsbDev::USB_WakeUp() {
  }

void
CmtUsbDev::USB_Suspend() {
  }

void
CmtUsbDev::USB_StartOfFrame() {
  }

void
CmtUsbDev::IrqHandler() {
  USB_OTG_GINTSTS_TypeDef gintr_status;

  if (USBD_FS_IsDeviceMode()) /* ensure that we are in device mode */
  {
    gintr_status.d32 = OTGD_FS_ReadCoreItr();

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    
    /* If there is no interrupt pending exit the interrupt routine */
    if( !gintr_status.d32 ) return;

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Wakeup or RemoteWakeup interrupt */
    if (gintr_status.b.wkupintr) {
      /* Update the Resume State machine */
      //Resume(RESUME_EXTERNAL);  
      
      USB_OTG_GINTSTS_TypeDef gintsts;

      gintsts.d32 = 0;
      /* Call user function */
      USB_WakeUp();
  
      /* Clear interrupt */
      gintsts.b.wkupintr = 1;
      USB_OTG_WRITE_REG32 (&USB_OTG_FS_regs.GREGS->GINTSTS, gintsts.d32);
      }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* Suspend interrupt */
    if (gintr_status.b.usbsuspend) { 
      USB_OTG_GINTSTS_TypeDef gintsts;  
  
      gintsts.d32 = 0;
      /* Call user function */
      USB_Suspend();
  
      /* Clear interrupt */
      gintsts.b.usbsuspend = 1;
      USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.GREGS->GINTSTS, gintsts.d32);
      }

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* Start of Frame interrupt */
    if( gintr_status.b.sofintr ) {
      USB_OTG_GINTSTS_TypeDef GINTSTS ;
      GINTSTS.d32 = 0;
  
      /* Call user function */
      USB_StartOfFrame();
    
      /* Clear interrupt */
      GINTSTS.b.sofintr = 1;
      USB_OTG_WRITE_REG32 (&USB_OTG_FS_regs.GREGS->GINTSTS, GINTSTS.d32);
      }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* Receive FIFO Queue Status Level interrupt */
    if (gintr_status.b.rxstsqlvl) {
      USB_OTG_GINTMSK_TypeDef int_mask;
      USB_OTG_GRXSTSP_TypeDef status;
      USB_OTG_EP *ep;

      int_mask.d32 = 0;
      status.d32 = 0;
  
      /* Disable the Rx Status Queue Level interrupt */
      int_mask.b.rxstsqlvl = 1;
      USB_OTG_MODIFY_REG32( &USB_OTG_FS_regs.GREGS->GINTMSK, int_mask.d32, 0);

      /* Get the Status from the top of the FIFO */
      status.d32 = USB_OTG_READ_REG32( &USB_OTG_FS_regs.GREGS->GRXSTSP );

      /* Get the related endpoint structure */
      ep = PCD_GetOutEP(status.b.epnum);

      switch (status.b.pktsts) {
        case STS_GOUT_NAK:
          break;
        case STS_DATA_UPDT:
          if (status.b.bcnt) {
            if (ep->type == EP_TYPE_ISOC) {
#ifdef MMM
              /* Call user function */
              INTR_RXSTSQLVL_ISODU_Callback();         
          
              /* Copy the received buffer to the RAM */
             OTGD_FS_ReadPacket((uint8_t*)(IsocBuff + (ISOC_BUFFER_SZE * IsocBufferIdx)), status.b.bcnt);
             ep->xfer_buff = (uint8_t*)(IsocBuff + (ISOC_BUFFER_SZE * IsocBufferIdx));  
          
             /* Check if the end of the global buffer has been reached */
             if (IsocBufferIdx == (NUM_SUB_BUFFERS - 1)) {
               /* Reset the buffer index */
               IsocBufferIdx = 0;                         
               }
             else {
               /* Increment the buffer index */
               IsocBufferIdx ++;
               }          
#endif //MMM
             }
           else {
             /* Copy the received buffer to the RAM */
             OTGD_FS_ReadPacket(USBD_Data_Buffer, status.b.bcnt);
             ep->xfer_buff = USBD_Data_Buffer;
             }
        
           /* Update the endpoint structure */
           ep->xfer_len  = status.b.bcnt;
           ep->xfer_count += status.b.bcnt;        
           }
         else {
           ep->xfer_len  = status.b.bcnt;
           }
         break;
       case STS_XFER_COMP:
         break;
       case STS_SETUP_COMP:
         break;
       case STS_SETUP_UPDT:
         /* Copy the setup packet received in Fifo into the setup buffer in RAM */
         OTGD_FS_ReadPacket(USBD_Data_Buffer, status.b.bcnt); 
         ep->xfer_buff = USBD_Data_Buffer;
         ep->xfer_count += status.b.bcnt;
         ep->xfer_len  = status.b.bcnt;
         break;
       default:
         break;
       }
  
    /* Enable the Rx Status Queue Level interrupt */
    USB_OTG_MODIFY_REG32( &USB_OTG_FS_regs.GREGS->GINTMSK, 0, int_mask.d32);
  
    /* Clear interrupt: this is a read only bit, it cannot be cleared by register 
     access */
    }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* Enumeration Done interrupt */
    if (gintr_status.b.enumdone) {
      USB_OTG_GINTSTS_TypeDef gintsts;
      USB_OTG_GUSBCFG_TypeDef gusbcfg;

      gintsts.d32 = 0;
      gusbcfg.d32 = 0;
  
      OTGD_FS_EP0Activate();

      /* Set USB turnaround time */
      gusbcfg.d32 = USB_OTG_READ_REG32(&USB_OTG_FS_regs.GREGS->GUSBCFG);
      gusbcfg.b.usbtrdtim = 9;
      USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.GREGS->GUSBCFG, gusbcfg.d32);

      /* Clear interrupt */
      gintsts.b.enumdone = 1;
      USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.GREGS->GINTSTS, gintsts.d32 );
      }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* Reset interrupt */
    if (gintr_status.b.usbreset) {
      USB_Reset();
      }    
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
    /* IN Endpoint interrupt */
    if (gintr_status.b.inepint) {
      USB_OTG_DIEPINTx_TypeDef diepint;

      uint32_t ep_intr = 0;
      uint32_t epnum = 0;
      USB_OTG_EP *ep;
      uint32_t fifoemptymsk = 0;

      diepint.d32 = 0;  
      ep_intr = OTGD_FS_ReadDevAllInEPItr();
      while ( ep_intr ) {
        if (ep_intr&0x1) /* In ITR */  {
          ep = PCD_GetInEP(epnum);
          diepint.d32 = PCD_ReadDevInEP(ep); /* Get In ITR status */
          if ( diepint.b.xfercompl ) {
            fifoemptymsk = 0x1 << ep->num;
            USB_OTG_MODIFY_REG32(&USB_OTG_FS_regs.DEV->DIEPEMPMSK, fifoemptymsk, 0);

            /* Clear the Interrupt flag */ 
            CLEAR_IN_EP_INTR(epnum, xfercompl);
        
            /* Call the relative IN endpoint callback */
            switch( epnum ) {
              case 0  : 
                EndPoint0( USB_EVT_IN ); 
                /* before terminate set Tx & Rx status */
                OTG_DEV_SetEPRxStatus(epnum, SaveRState);
                OTG_DEV_SetEPTxStatus(epnum, SaveTState);
                break;
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
          if( diepint.b.timeout ) {
            CLEAR_IN_EP_INTR(epnum, timeout);
            }
          if( diepint.b.intktxfemp ) {
            CLEAR_IN_EP_INTR(epnum, intktxfemp);
            }
          if( diepint.b.inepnakeff ) {
            CLEAR_IN_EP_INTR(epnum, inepnakeff);
            }
          if( diepint.b.txfempty ) {      
            //if( (epnum == 0) || (OTG_DEV_GetEPTxStatus(epnum) == DEV_EP_TX_VALID) ) {
            //  PCD_WriteEmptyTxFifo(epnum);          
            //  }

            CLEAR_IN_EP_INTR(epnum, txfempty);          
            }
          if( diepint.b.epdis ) { 
            /* Reset Endpoint Frame ID to 0 */
            ep->even_odd_frame = 0;

            CLEAR_IN_EP_INTR(epnum, epdis);
            }      
          }
        epnum++;
        ep_intr >>= 1;
        }
      }
    
   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* OUT Endpoint interrupt */
    if( gintr_status.b.outepintr ) {
      uint32_t ep_intr = 0;
      USB_OTG_DOEPINTx_TypeDef doepint;
      uint32_t epnum = 0;
      USB_OTG_EP *ep;
  
      doepint.d32 = 0;

      /* Read in the device interrupt bits */
      ep_intr = OTGD_FS_ReadDevAllOutEp_itr();
  
      while ( ep_intr ) {
        if( ep_intr & 0x1 ) {
          /* Get EP pointer */
          ep = PCD_GetOutEP(epnum);
          doepint.d32 = OTGD_FS_ReadDevOutEP_itr(ep);

          /* Setup Phase Done (control EPs) */
          if( doepint.b.setup ) {
            if( epnum == 0 ) {        
              /* Call the SETUP process for the EP0 */
              EndPoint0( USB_EVT_SETUP );

              /* Before exit, update the Tx status */
              OTG_DEV_SetEPTxStatus(0x80, SaveTState); 
              }
            else {
              /* Other control endpoints */
              }  
        
            /* Clear the EP Interrupt */
            CLEAR_OUT_EP_INTR(epnum, setup);
            }
          /* Back to back setup received */
          if( doepint.b.b2bsetup ) {
            if( epnum == 0 ) {        
              /* Call the SETUP process for the EP0 */
              EndPoint0( USB_EVT_SETUP );

              /* Before exit, update the Tx status */
              OTG_DEV_SetEPTxStatus(0x80, SaveTState);  
              }
            }
          /* Transfer complete */
          if ( doepint.b.xfercompl ) {
            /* Clear the bit in DOEPINTn for this interrupt */
            CLEAR_OUT_EP_INTR(epnum, xfercompl);
        
            switch( epnum ) {
              case 0  : EndPoint0( USB_EVT_OUT ); break;
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
          /* Endpoint disable  */
          if( doepint.b.epdis ) {
            /* Clear the bit in DOEPINTn for this interrupt */
            CLEAR_OUT_EP_INTR(epnum, epdis);
            }
          }
        epnum++;
        ep_intr >>= 1;
        }
      }
 



   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Isochronous Out packet Dropped interrupt */
#ifdef INTR_ISOOUTDROP
    if (gintr_status.b.isooutdrop)
    {
      retval |= OTGD_FS_Handle_IsoOutDrop_ISR();
    }
#endif /* INTR_ISOOUTDROP */  

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Endpoint Mismatch error interrupt */
#ifdef INTR_EPMISMATCH
    if (gintr_status.b.epmismatch)
    {
      retval |= OTGD_FS_Handle_EPMismatch_ISR();
    }
#endif /* INTR_EPMISMATCH */  

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Incomplete Isochronous IN transfer error interrupt */
#ifdef INTR_INCOMPLISOIN
    if (gintr_status.b.incomplisoin)
    {
      retval |= OTGD_FS_Handle_IncomplIsoIn_ISR();
    }
#endif /* INTR_INCOMPLISOIN */  

   /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/    
    /* Incomplete Isochronous OUT transfer error interrupt */
#ifdef INTR_INCOMPLISOOUT
    if (gintr_status.b.outepintr)
    {
      retval |= OTGD_FS_Handle_IncomplIsoOut_ISR();
    }
#endif /* INTR_INCOMPLISOOUT */  
  
  }
  }

void 
CmtUsbDev::USB_Reset() {
  USB_OTG_DAINT_TypeDef daintmsk;
  USB_OTG_DOEPMSKx_TypeDef doepmsk;
  USB_OTG_DIEPMSKx_TypeDef diepmsk;
  USB_OTG_DCFG_TypeDef dcfg;
  USB_OTG_DCTL_TypeDef dctl;
  USB_OTG_GINTSTS_TypeDef gintsts;
  uint32_t i = 0;
  
  daintmsk.d32 = 0;
  doepmsk.d32 = 0;
  diepmsk.d32 = 0;
  dcfg.d32 =0;
  dctl.d32 = 0;
  gintsts.d32 = 0;

  /* Clear the Remote Wakeup Signalling */
  dctl.b.rmtwkupsig = 1;
  USB_OTG_MODIFY_REG32(&USB_OTG_FS_regs.DEV->DCTL, dctl.d32, 0 );

  /* Flush the NP Tx FIFO */
  OTGD_FS_FlushTxFifo( 0 );
  
  /* clear pending interrupts */
  for (i = 0; i < NUM_TX_FIFOS ; i++)
  {
    USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DINEPS[i]->DIEPINTx, 0xFF);
    USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DOUTEPS[i]->DOEPINTx, 0xFF);
  }
  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DEV->DAINT, 0xFFFFFFFF );  

  daintmsk.ep.in = 1;
  daintmsk.ep.out = 1;
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DEV->DAINTMSK, daintmsk.d32 );

  doepmsk.b.setup = 1;
  doepmsk.b.b2bsetup = 1;
  doepmsk.b.xfercompl = 1;
  doepmsk.b.epdis = 1;
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DEV->DOEPMSK, doepmsk.d32 );

  diepmsk.b.xfercompl = 1;
  diepmsk.b.timeout = 1;
  diepmsk.b.epdis = 1;
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DEV->DIEPMSK, diepmsk.d32 );

  /* Reset Device Address */
  dcfg.d32 = USB_OTG_READ_REG32( &USB_OTG_FS_regs.DEV->DCFG);
  dcfg.b.devaddr = 0;
  USB_OTG_WRITE_REG32( &USB_OTG_FS_regs.DEV->DCFG, dcfg.d32);

  /* setup EP0 to receive SETUP packets */
  PCD_EP0_OutStart();

  /* Clear interrupt */
  gintsts.d32 = 0;
  gintsts.b.usbreset = 1;
  USB_OTG_WRITE_REG32 (&USB_OTG_FS_regs.GREGS->GINTSTS, gintsts.d32);
  
  USB_ResetCore();
  }

void 
CmtUsbDev::Init() {
  OTG_DEV_Init();
  }

void 
CmtUsbDev::USB_Connect (int con) {
  if (con) {
    USB_DevConnect();
    } 
  else {
    USB_DevDisconnect();
    }
  }

void 
CmtUsbDev::USB_Configure (uint_8 cfg) {
  
  }
  
void 
CmtUsbDev::USB_SetAddressPre (uint8 adr) {
  PCD_EP_SetAddress( adr );
  }


void 
CmtUsbDev::USB_SetAddress (uint8 adr) {
  }

void 
CmtUsbDev::USB_ClrStallEP( uint8 EPNum ) {
  PCD_EP_ClrStall( EPNum );
  }

void
CmtUsbDev::USB_SetStallEP( uint8 EPNum ) {
  PCD_EP_Stall( EPNum );
  }

void 
CmtUsbDev::USB_DisableEP( uint8 EPNum ) {
  USB_OTG_EP *ep;

  if ((0x80 & EPNum) != 0)  ep = PCD_GetInEP(EPNum & 0x7F);
  else                      ep = PCD_GetOutEP(EPNum & 0x7F);

  OTGD_FS_EPDeactivate( ep );  
  }

void 
CmtUsbDev::USB_EnableEP( uint8 EPNum ) {
  USB_OTG_EP *ep;

  if ((0x80 & EPNum) != 0)  ep = PCD_GetInEP(EPNum & 0x7F);
  else                      ep = PCD_GetOutEP(EPNum & 0x7F);

  OTGD_FS_EPActivate( ep );  
  }


void 
CmtUsbDev::USB_ConfigEP( USB_ENDPOINT_DESCRIPTOR *pEPD ) {
  PCD_EP_Open( pEPD );
  }

uint32 
CmtUsbDev::USB_WriteEP8( uint8 EPNum, uint8 *pData, uint32 cnt ) {
  USB_OTG_DEPCTLx_TypeDef           depctl;
  OTG_FS_DEPTSIZx_TypeDef           deptsiz;
  USB_OTG_DINEPS                    *in_regs ;

  OTGD_FS_DisableGlobalInt();
  in_regs = USB_OTG_FS_regs.DINEPS[EPNum & 0xf];
  depctl.d32  = USB_OTG_READ_REG32(&in_regs->DIEPCTLx);
  deptsiz.d32 = USB_OTG_READ_REG32(&in_regs->DIEPTSIZx);

  deptsiz.b.xfersize = cnt;
  deptsiz.b.pktcnt = 1;
  
  USB_OTG_WRITE_REG32(&in_regs->DIEPTSIZx, deptsiz.d32);

  /* EP enable, IN data in FIFO */
  depctl.b.cnak = 1;
  depctl.b.epena = 1;
  USB_OTG_WRITE_REG32(&in_regs->DIEPCTLx, depctl.d32);
  
  //Записать данные в FIFO
  if( cnt ) OTGD_FS_WritePacket( pData, EPNum & 0xf, cnt );
  OTGD_FS_EnableGlobalInt();
  
  return (cnt);
  }

uint32 
CmtUsbDev::USB_ReadEP8 (uint8 EPNum, uint8 *pData) {
  uint32_t DataLength = 0;
  
  USB_OTG_EP *ep;

  /* Get the structure pointer of the selected Endpoint */
  ep = PCD_GetOutEP(EPNum & 0xf);
  
  /* Get the number of received data */
  DataLength = ep->xfer_len;
  
  /* Use the PCD interface layer function to read the selected endpoint */
  PCD_EP_Read (EPNum, pData, DataLength);
  
  return (DataLength);
  }

void 
CmtUsbDev::USB_DataInEnd() {
  //Разрешить прием следующих данных
//  PCD_EP0_OutStart();

  //PCD_EP_Read ( 0, 0, 0 );
  }

void
CmtUsbDev::USB_Error( uint8 ) {
  }

void
CmtUsbDev::USB_ResetEP( uint8 EPNum ) {
  //PCD_EP_Close( EPNum );
  }

void
CmtUsbDev::USB_SetNAK( uint8 EPNum ) {
  __IO USB_OTG_DEPCTLx_TypeDef depctl;
  USB_OTG_EP *ep;
  //Для нулевой EP не работает
  if( (EPNum & 0x7f) == 0 ) return;
  
  ep = PCD_GetOutEP(EPNum & 0x7F);

  depctl.d32  = USB_OTG_READ_REG32(&(USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx));
  depctl.b.snak = 1;

  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx, depctl.d32);
  }

void
CmtUsbDev::USB_ResetNAK( uint8 EPNum ) {
  __IO USB_OTG_DEPCTLx_TypeDef depctl;
  USB_OTG_EP *ep;
  //Для нулевой EP не работает
  if( (EPNum & 0x7f) == 0 ) return;

  ep = PCD_GetOutEP(EPNum & 0x7F);

  depctl.d32  = USB_OTG_READ_REG32(&(USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx));
  depctl.b.cnak = 1;

  USB_OTG_WRITE_REG32(&USB_OTG_FS_regs.DOUTEPS[ep->num]->DOEPCTLx, depctl.d32);
  }

  
