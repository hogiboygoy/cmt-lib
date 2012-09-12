//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//Аппаратные реализации
#if defined(CMT_STM32F105VCT6) || defined(CMT_STM32F103CBT6) || defined(CMT_STM32F100C8T)
  #include <Sys/Dev/Abstract/I2c/cmtSTM32F10x.cpp>
  
#elif defined( CMT_TI_OMAP3530 )
  #include <Sys/Dev/Abstract/I2c/cmtTiOmap3530.cpp>
  
#elif defined( CMT_STM8L151K4 )
  #include <Sys/Dev/Abstract/I2c/cmtStm8l15x.cpp>
  
#else
  #error "CMT:I2c not defined for this CPU"
#endif

