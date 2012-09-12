//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMTCPU_H
#define CMTCPU_H

//==============================================================================
//                            Тактовые частоты
//****** Карта 0
#if CMT_CPU_CLK_MAP == 0
//Карта 0 распределения частот
#if defined(CMT_STM32F105VCT6)
  #define CMT_XTAL_CLK 12000000 //Частота внешнего тактового сигнала
  #define CMT_CORE_CLK 72000000 //Тактовая частота ядра
  #define CMT_USB_CLK  48000000 //Тактовая частота USB

  //Назначить частоты для внутреннего использования библиотекой STM
  #define HSE_VALUE CMT_XTAL_CLK //Частота внешнего осциллятора

#elif defined(CMT_STM32F103CBT6)
  #define CMT_XTAL_CLK  8000000 //Частота внешнего тактового сигнала
  #define CMT_CORE_CLK 72000000 //Тактовая частота ядра
  #define CMT_USB_CLK  48000000 //Тактовая частота USB

  //Назначить частоты для внутреннего использования библиотекой STM
  #define HSE_VALUE CMT_XTAL_CLK //Частота внешнего осциллятора
  
#elif defined(CMT_STM32F100C8T)
  #define CMT_XTAL_CLK  8000000 //Частота внешнего тактового сигнала
  #define CMT_CORE_CLK 24000000 //Тактовая частота ядра

  //Назначить частоты для внутреннего использования библиотекой STM
  #define HSE_VALUE CMT_XTAL_CLK //Частота внешнего осциллятора

#elif defined(CMT_STM8L151K4)
  #define CMT_XTAL_CLK  16000000 //Частота внутреннего тактового сигнала
  #define CMT_CORE_CLK  16000000 //Тактовая частота ядра

#elif defined(CMT_TI_OMAP3530)
  #define CMT_XTAL_CLK  26000000 //Частота внешнего тактового сигнала
  #define CMT_CORE_CLK 600000000 //Тактовая частота ядра

  //#define PRCM_CLK_CFG2_332MHZ     1    /* VDD2=1.15v - 166MHz DDR */
  //#define PRCM_PCLK_OPP2           1    /* ARM=381MHz - VDD1=1.20v */
#elif defined( CMT_DESCTOP_QT )
#elif defined( CMT_GENERIC )
#else
  #error "NotReleased for this CPU"
#endif


//****** Карта 1
#elif CMT_CPU_CLK_MAP == 1
//Карта 1 распределения частот
#if defined(CMT_STM32F105VCT6)
  #define CMT_XTAL_CLK  8000000 //Частота внешнего тактового сигнала
  #define CMT_CORE_CLK 72000000 //Тактовая частота ядра
  #define CMT_USB_CLK  48000000 //Тактовая частота USB

  //Назначить частоты для внутреннего использования библиотекой STM
  #define HSE_VALUE CMT_XTAL_CLK //Частота внешнего осциллятора
#else
  #error "NotReleased for this CPU"
#endif

//****** Карта не определена
#else //Тактовые частоты
  #error "Invalid value for CMT_CPU_CLK_MAP: cpu friquensy map"
#endif

//==============================================================================
//                        Определения для процессоров
#if defined( CMT_STM32F105VCT6 )
  //Управление библиотекой ST
  #define STM32F10X_CL

  //Процессор управляется ядром CortexM
  #include <Sys/Core/Cpu/cmtFamCortexM.h>

#elif defined( CMT_STM32F103CBT6 )
  //Управление библиотекой ST
  #define STM32F10X_MD

  //Процессор управляется ядром CortexM
  #include <Sys/Core/Cpu/cmtFamCortexM.h>

#elif defined( CMT_STM32F100C8T )
  //Управление библиотекой ST
  #define STM32F10X_MD_VL

  //Процессор управляется ядром CortexM
  #include <Sys/Core/Cpu/cmtFamCortexM.h>

#elif defined( CMT_STM8L151K4 )
  //Управление библиотекой ST
  #define STM8L15X_LD

  //Процессор управляется ядром STM8
  #include <Sys/Core/Cpu/cmtFamStm8.h>

#elif defined(CMT_TI_OMAP3530)
  //Процессор управляется ядром CortexA
  #include <Sys/Core/Cpu/cmtFamCortexA.h>

#elif defined(CMT_DESCTOP_QT) || defined(CMT_GENERIC)
  #include <Sys/Core/Cpu/cmtFamIntel86-32.h>

#else
  #error "cmt: CPU must be defined"
#endif

void cmtInitSystem();

#endif // CMTCPU_H
