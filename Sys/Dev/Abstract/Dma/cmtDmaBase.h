//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
struct CmtDmaBase : public CmtTransfer {

  };

//Реализации
#if defined(CMT_TI_OMAP3530)
  #include <Sys/Dev/Abstract/Dma/cmtTiOmap3530.h>
#elif defined(CMT_STM32F105VCT6)
  #include <Sys/Dev/Abstract/Dma/cmtStm32f10x.h>
#else
  #error "CMT: DMA not supported for this CPU"
#endif
