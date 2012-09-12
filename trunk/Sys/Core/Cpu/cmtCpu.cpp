//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#if defined( CMT_LPC1768 )
  #include <Sys/Core/cmtCortexM3.cpp>

#elif defined( CMT_ATMEGA32 )
  #include <Sys/Core/cmtAvr8.cpp>

#elif defined( CMT_STM32F103CBT6 )
  #include <Sys/Core/Cpu/cmtCortexM3.cpp>
  #include <Sys/Core/Cpu/cmtStm32f103cbt.cpp>

#elif defined( CMT_STM32F100C8T )
  #include <Sys/Core/Cpu/cmtCortexM3.cpp>
  #include <Sys/Core/Cpu/cmtStm32f100c8t.cpp>

#elif defined( CMT_STM32F105VCT6 )
  #include <Sys/Core/Cpu/cmtCortexM3.cpp>
  #include <Sys/Core/Cpu/cmtStm32f105vct.cpp>

#elif defined( CMT_STM8L151K4 )
  #include <Sys/Core/Cpu/cmtStm8.cpp>

#elif defined( CMT_TI_OMAP3530 )
  #include <Sys/Core/Cpu/cmtCortexA8.cpp>
  #include <Sys/Core/Cpu/cmtTiOmap3530.cpp>

#elif defined( CMT_DESCTOP_QT ) || defined( CMT_GENERIC ) 
  #include <Sys/Core/Cpu/cmtDesctopQt.cpp>

#else
  #error "CMT:No CPU.cpp for this config"
#endif
