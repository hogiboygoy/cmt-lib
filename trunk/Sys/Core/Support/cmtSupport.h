//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMTSUPPORT_H
#define CMTSUPPORT_H

//Определения для процессоров
#if defined( CMT_STM32F105VCT6 ) || defined( CMT_STM32F103CBT6 ) || defined( CMT_STM32F100C8T )
  //Библиотека поддержки ST
  #include <Sys/Core/Support/cmtSupportSt32f10x.h>

#elif defined(CMT_STM8L151K4)
  //Библиотека поддержки ST
  #include <Sys/Core/Support/cmtSupportStm8l15x.h>

#elif defined(CMT_TI_OMAP3530)
  //Библиотека поддержки TI OMAP3
  #include <Sys/Core/Support/cmtSupportTiOmap3530.h>

#elif defined(CMT_DESCTOP_QT) || defined(CMT_GENERIC)
#else
  #error "cmt: CPU must be defined"
#endif


#endif
