//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
CmtDmaTransfer::CmtDmaTransfer(DMA_Channel_TypeDef *dma, uint32 dmaFlag, uint32 periphAdr, bool dirToPeriph ) :
  CmtTransfer(),
  mDma(dma),
  mDmaFlag(dmaFlag) {

  //Настройка канала DMA
  mDmaInit.DMA_DIR                = dirToPeriph ? DMA_DIR_PeripheralDST : DMA_DIR_PeripheralSRC;
  mDmaInit.DMA_M2M                = DMA_M2M_Disable;
  mDmaInit.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
  mDmaInit.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  mDmaInit.DMA_Mode               = DMA_Mode_Normal;
  mDmaInit.DMA_PeripheralBaseAddr = periphAdr;
  mDmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  mDmaInit.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  mDmaInit.DMA_Priority           = DMA_Priority_Low;
  }

void
CmtDmaTransfer::InitTransfer(void *adr, uint32 elemCount, uint32 frameCount) {
  DMA_ClearFlag( mDmaFlag );
  //Настроить канал
  mDmaInit.DMA_BufferSize = elemCount * frameCount;
  mDmaInit.DMA_MemoryBaseAddr = (uint32_t)adr;
  DMA_Init( mDma, &mDmaInit );
  DMA_Cmd( mDma, ENABLE );
  }

struct CmtEvDma : public CmtEvent {
    uint32               mDmaFlag; //!< Индекс флага завершения обмена

    CmtEvDma( uint32 dmaFlag ) : CmtEvent(), mDmaFlag(dmaFlag) {}

    virtual bool IsSignaled() { return DMA_GetFlagStatus( mDmaFlag ) != RESET; }
  };

uint_8
CmtDmaTransfer::WaitTransfer(CmtEvent *pBreak) {
  CmtEvDma( mDmaFlag ).Wait2( pBreak );
  StopTransfer();
  return pBreak && pBreak->IsSignaled() ? CMTE_BREAK : CMTE_OK;
  }

void
CmtDmaTransfer::StopTransfer() {
  DMA_Cmd( mDma, DISABLE );
  }

