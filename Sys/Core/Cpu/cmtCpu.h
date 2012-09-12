//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMTCPU_H
#define CMTCPU_H

//==============================================================================
//                            �������� �������
//****** ����� 0
#if CMT_CPU_CLK_MAP == 0
//����� 0 ������������� ������
#if defined(CMT_STM32F105VCT6)
  #define CMT_XTAL_CLK 12000000 //������� �������� ��������� �������
  #define CMT_CORE_CLK 72000000 //�������� ������� ����
  #define CMT_USB_CLK  48000000 //�������� ������� USB

  //��������� ������� ��� ����������� ������������� ����������� STM
  #define HSE_VALUE CMT_XTAL_CLK //������� �������� �����������

#elif defined(CMT_STM32F103CBT6)
  #define CMT_XTAL_CLK  8000000 //������� �������� ��������� �������
  #define CMT_CORE_CLK 72000000 //�������� ������� ����
  #define CMT_USB_CLK  48000000 //�������� ������� USB

  //��������� ������� ��� ����������� ������������� ����������� STM
  #define HSE_VALUE CMT_XTAL_CLK //������� �������� �����������
  
#elif defined(CMT_STM32F100C8T)
  #define CMT_XTAL_CLK  8000000 //������� �������� ��������� �������
  #define CMT_CORE_CLK 24000000 //�������� ������� ����

  //��������� ������� ��� ����������� ������������� ����������� STM
  #define HSE_VALUE CMT_XTAL_CLK //������� �������� �����������

#elif defined(CMT_STM8L151K4)
  #define CMT_XTAL_CLK  16000000 //������� ����������� ��������� �������
  #define CMT_CORE_CLK  16000000 //�������� ������� ����

#elif defined(CMT_TI_OMAP3530)
  #define CMT_XTAL_CLK  26000000 //������� �������� ��������� �������
  #define CMT_CORE_CLK 600000000 //�������� ������� ����

  //#define PRCM_CLK_CFG2_332MHZ     1    /* VDD2=1.15v - 166MHz DDR */
  //#define PRCM_PCLK_OPP2           1    /* ARM=381MHz - VDD1=1.20v */
#elif defined( CMT_DESCTOP_QT )
#elif defined( CMT_GENERIC )
#else
  #error "NotReleased for this CPU"
#endif


//****** ����� 1
#elif CMT_CPU_CLK_MAP == 1
//����� 1 ������������� ������
#if defined(CMT_STM32F105VCT6)
  #define CMT_XTAL_CLK  8000000 //������� �������� ��������� �������
  #define CMT_CORE_CLK 72000000 //�������� ������� ����
  #define CMT_USB_CLK  48000000 //�������� ������� USB

  //��������� ������� ��� ����������� ������������� ����������� STM
  #define HSE_VALUE CMT_XTAL_CLK //������� �������� �����������
#else
  #error "NotReleased for this CPU"
#endif

//****** ����� �� ����������
#else //�������� �������
  #error "Invalid value for CMT_CPU_CLK_MAP: cpu friquensy map"
#endif

//==============================================================================
//                        ����������� ��� �����������
#if defined( CMT_STM32F105VCT6 )
  //���������� ����������� ST
  #define STM32F10X_CL

  //��������� ����������� ����� CortexM
  #include <Sys/Core/Cpu/cmtFamCortexM.h>

#elif defined( CMT_STM32F103CBT6 )
  //���������� ����������� ST
  #define STM32F10X_MD

  //��������� ����������� ����� CortexM
  #include <Sys/Core/Cpu/cmtFamCortexM.h>

#elif defined( CMT_STM32F100C8T )
  //���������� ����������� ST
  #define STM32F10X_MD_VL

  //��������� ����������� ����� CortexM
  #include <Sys/Core/Cpu/cmtFamCortexM.h>

#elif defined( CMT_STM8L151K4 )
  //���������� ����������� ST
  #define STM8L15X_LD

  //��������� ����������� ����� STM8
  #include <Sys/Core/Cpu/cmtFamStm8.h>

#elif defined(CMT_TI_OMAP3530)
  //��������� ����������� ����� CortexA
  #include <Sys/Core/Cpu/cmtFamCortexA.h>

#elif defined(CMT_DESCTOP_QT) || defined(CMT_GENERIC)
  #include <Sys/Core/Cpu/cmtFamIntel86-32.h>

#else
  #error "cmt: CPU must be defined"
#endif

void cmtInitSystem();

#endif // CMTCPU_H
