//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//Аппаратные реализации
#ifdef CMT_STM32F105VCT6
  #include <Sys/Dev/Abstract/Dma/cmtSTM32F10x.cpp>
#endif

#ifdef CMT_STM32F103CBT6
  #include <Sys/Dev/Abstract/Dma/cmtSTM32F10x.cpp>
  #include <Sys/Dev/Abstract/Spi/cmtSTM32F103.cpp>
#endif

#ifdef CMT_TI_OMAP3530
  #include <Sys/Dev/Abstract/Dma/cmtTiOmap3530.cpp>
#endif
