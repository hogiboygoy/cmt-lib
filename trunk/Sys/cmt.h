//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// ��������
//   ���� �������� ����������� ��� ������������ ���������� ��������
//==============================================================================
#ifndef CMT_H
#define CMT_H

#if CMT_VERSION != 4
  #error "CMT:Wrong cmt version"
#endif

//==========================================================================
//                               ���������� ������� ����� ������
#include <Sys/cmtErrorCode.h>

//==========================================================================
//                               CPU
#include <Sys/Core/Cpu/cmtCpu.h>

//==========================================================================
//                               ����
#include <Sys/Core/cmtType.h>

//==========================================================================
//                               ��������� ����������� ������
#include <Sys/Core/Fs/cmtFile.h>

//==========================================================================
//                               ���������� ���������
#include <Sys/Core/Support/cmtSupport.h>

//==========================================================================
//                               ��������� ��������
#include <Sys/Obj/cmtObject.h>

//���������-����������� ����
#ifdef CMT_KERNEL_MAP
  #include <Sys/Core/cmtCore.h>
#endif

//==========================================================================
//                               ��������� ������
#ifdef CMT_SYS_TIMER_MAP
  #include <Sys/Core/SysTimer/cmtSysTimer.h>
#endif

//==========================================================================
//                               ���������� NLib
#ifdef CMT_NDATE
  #include <NLib/Include/NDate.h>
#endif

//==========================================================================
//                               ���� � ���������
#ifdef CMT_SYSTEM_TIME_MAP
  #include <Sys/Core/Calendar/cmtSystemTime.h>
#endif

//==========================================================================
//                               ���������� �����
//���������� ���� �� ���������
#ifdef CMT_MEMORY_LOC //����������� �������� ������������� (����������� ������ � 32 ��������� ������)
  #include <Sys/Core/Mem/cmtMemory.h>
  #define CMT_MEMORY
#endif

#ifdef CMT_MEMORY_STD      //����������� �������� ������������� (������ ���� � ������������ �������)
  #ifdef CMT_MEMORY
    #error "You must declare only one memory system"
  #endif
  #include <Sys/Core/Mem/cmtMemory.h>
  #define CMT_MEMORY
#endif

//==========================================================================
//                               ����� ��������������� �������
#ifdef CMT_FHELPER
  #include <Sys/Core/Misc/cmtFHelper.h>
#endif

//==========================================================================
//                               ����� ����������� �������
#ifdef CMT_BUFFER
  #include <Sys/Core/Misc/cmtBuffer.h>
#endif

//==========================================================================
//                               �������
#ifdef CMT_QUEUE
  #include <Sys/Core/Misc/cmtQueue.h>
#endif

//==========================================================================
//                               �����
#ifdef CMT_STACK
  #include <Sys/Core/Misc/cmtStack.h>
#endif

//==========================================================================
//                               ������
#ifdef CMT_STRING
  #include <Sys/Core/Misc/cmtString.h>
#endif

//==========================================================================
//                               ������������, ���, CRC
#ifdef CMT_CRYPTO
  #include <Sys/Core/Misc/cmtCrypto.h>
#endif

//==========================================================================
//                               ������ �������� �����
#ifdef CMT_BLINK_TASK
  #ifndef CMT_SYS_TIMER_MAP
    #error "You must declare system timer"
  #endif
  #include <Sys/Dev/Ext/Led/cmtBlinkTask.h>
#endif

//==========================================================================
//                            ������������ ���������
#ifdef CMT_LED
  #ifndef CMT_SYS_TIMER_MAP
    #error "You must declare system timer"
  #endif
  #include <Sys/Dev/Ext/Led/cmtLed.h>
#endif

//==========================================================================
//                            ������������ ��������� ���
#ifdef CMT_LED_PWM
  #ifndef CMT_SYS_TIMER_MAP
    #error "You must declare system timer"
  #endif
  #include <Sys/Dev/Ext/Led/cmtLedPwm.h>
#endif

//==========================================================================
//                               ������ ������ ����������
#ifdef CMT_KBD_TASK
  #ifndef CMT_SYS_TIMER_MAP
    #error "You must declare system timer"
  #endif
  #include <Sys/Dev/Ext/Keyboard/cmtKbdTask.h>
#endif

#ifdef CMT_SLIDER     //������������� ���������
  #include <Sys/Dev/Ext/Keyboard/cmtSlider.h>
#endif

#ifdef CMT_ROLL_SLIDER //������������� ��������� � ��������������
  #include <Sys/Dev/Ext/Keyboard/cmtRollSlider.h>
#endif

#ifdef CMT_LONG_PRESS //������������� �������� �������
  #include <Sys/Dev/Ext/Keyboard/cmtLongPress.h>
#endif

//==========================================================================
//                               �������
#if defined(CMT_PARSER) || defined(CMT_PARSER_BIN_COPY) || defined(CMT_PARSER_ELF)
  #include <Sys/Dev/Parser/cmtParser.h>
#endif

//==========================================================================
//                               ����������� ����������
#ifdef CMT_DEV
  #include <Sys/Dev/cmtDev.h>
#endif

//==========================================================================
//                               ����������� ���������� DMA
#ifdef CMT_DMA
  #include <Sys/Dev/Abstract/Dma/cmtDmaBase.h>
#endif

//==========================================================================
//                               ����������� ���������������� ����������
#ifdef CMT_SERIAL
  #include <Sys/Dev/Abstract/Serial/cmtSerial.h>
#endif

//==========================================================================
//                               ���� UART
#ifdef CMT_UART
  #include <Sys/Dev/Abstract/Uart/cmtUart.h>
#endif


//==========================================================================
//                               ���� SPI
#ifdef CMT_SPI
  #include <Sys/Dev/Abstract/Spi/cmtSpi.h>
#endif

//==========================================================================
//                               ���� I2C
#ifdef CMT_I2C
  #include <Sys/Dev/Abstract/I2c/cmtI2c.h>
#endif

//==========================================================================
//                               ����������� ������
#ifdef CMT_MEMORY_BASE
  #include <Sys/Dev/Abstract/Memory/cmtMemoryBase.h>
#endif

//==========================================================================
//                               ������� ����������
#ifdef CMT_BLOCK_DEVICE
  #include <Sys/Dev/Abstract/Block/cmtBlockDevice.h>
#endif

//==========================================================================
//                               ������� RAM-�����
#ifdef CMT_MEMORY_DISK
  #include <Sys/Dev/Abstract/Block/cmtMemoryDisk.h>
#endif

//==========================================================================
//                               ������� SD �����
//������ ���� ��������� ������� ����������
#ifdef CMT_SPI_SD_CARD
  #ifndef CMT_SPI
    #error "You must declare SPI"
  #endif
  #ifndef CMT_BLOCK_DEVICE
    #error "You must declare block devices"
  #endif
  #ifndef CMT_MEMORY
    #error "You must declare one memory system"
  #endif
  #include <Sys/Dev/Abstract/Sd/cmtSpiSdCard.h>
#endif

#ifdef CMT_SD_CARD
  #ifndef CMT_BLOCK_DEVICE
    #error "You must declare block devices"
  #endif
  #ifndef CMT_MEMORY
    #error "You must declare one memory system"
  #endif
  #include <Sys/Dev/Abstract/Sd/cmtSdCard.h>
#endif

//==========================================================================
//                               �������� �������
#ifdef CMT_FS
  #include <Sys/Dev/Abstract/Fs/cmtFileSystemBase.h>
#endif

//==========================================================================
//                               USB
#ifdef CMT_USB
  #include <Sys/Dev/Abstract/Usb/cmtUsb.h>
#endif

//==========================================================================
//                               USB DEVICE
#ifdef CMT_USB_DEV
  #ifndef CMT_USB
    #error "You must declare usb"
  #endif
  #include <Sys/Dev/Abstract/Usb/cmtUsbDev.h>
#endif

#ifdef CMT_USB_DEV_COM
  #ifndef CMT_USB_DEV
    #error "You must declare usb device"
  #endif
  #include <Sys/Dev/Abstract/Usb/cmtUsbDevCom.h>
#endif

#ifdef CMT_USB_DEV_COM_UART
  #ifndef CMT_USB_DEV_COM
    #error "You must declare usb com device"
  #endif
  #include <Sys/Dev/Abstract/Usb/cmtUsbDevComUart.h>
#endif

#ifdef CMT_USB_DEV_BULK
  #ifndef CMT_USB_DEV
    #error "You must declare usb device"
  #endif
  #include <Sys/Dev/Abstract/Usb/cmtUsbDevBulk.h>
#endif

#ifdef CMT_USB_DEV_MASS
  #ifndef CMT_USB_DEV
    #error "You must declare usb device"
  #endif
  #include <Sys/Dev/Abstract/Usb/cmtUsbDevMsc.h>
#endif

//==========================================================================
//                               USB HOST
#ifdef CMT_USB_HOST
  #include <Sys/Dev/Abstract/Usb/cmtUsbHost.h>
#endif

//==========================================================================
//                               USART � UART
#ifdef CMT_SI_UART
  #ifndef CMT_SERIAL
    #error "You must declare serial devices"
  #endif
#endif

//==========================================================================
//                            ���������� �������� �����������
#ifdef CMT_STEP_MOTOR
  #include <Sys/Dev/Ext/StepMotor/cmtStepMotor.h>
#endif

//==========================================================================
//                            ��������� �� ���������
#ifdef CMT_GEOMETRY
  #include <Sys/Gui/cmtGeometry.h>
#endif

//==========================================================================
//                            ����������� ��������
#ifdef CMT_IMAGE
  #ifndef CMT_GEOMETRY
    #error "You must declare CMT_GEOMETRY"
  #endif
  #include <Sys/Gui/cmtImage.h>
#endif

//==========================================================================
//                            ����������� �������� ���������
#ifdef CMT_CONTEXT
  #ifndef CMT_GEOMETRY
    #error "You must declare CMT_GEOMETRY"
  #endif
  #include <Sys/Gui/cmtContext.h>
#endif

//==========================================================================
//                            ����������� �����
#ifdef CMT_SCREEN
  #ifndef CMT_GEOMETRY
    #error "You must declare CMT_GEOMETRY"
  #endif
  #ifndef CMT_DEV
    #error "You must declare CMT_DEV"
  #endif
  #ifndef CMT_CONTEXT
    #error "You must declare CMT_CONTEXT"
  #endif
#endif

#ifdef CMT_SCREEN_WEG010016
  #ifndef CMT_SCREEN
    #error "You must declare CMT_SCREEN"
  #endif
  #include <Sys/Gui/cmtScreenWEG010016.h>
#endif

#if defined(CMT_SCREEN_TI_OMAP3530) || defined(CMT_SCREEN_TI_OMAP3530_ROTATE)
  #ifndef CMT_SCREEN
    #error "You must declare CMT_SCREEN"
  #endif
  #include <Sys/Gui/cmtScreenTiOmap3530.h>
#endif

#ifdef CMT_SCREEN_WIN32
  #ifndef CMT_SCREEN
    #error "You must declare CMT_SCREEN"
  #endif
  #include <Sys/Gui/cmtScreenWin32.h>
#endif


#endif

//==========================================================================
//                            ������� ��������� �������
#ifdef CMT_CONSOLE
  #ifndef CMT_SCREEN
    #error "You must declare CMT_SCREEN"
  #endif
  #include <Sys/Gui/cmtConsole.h>
#endif


//==========================================================================
//                            Widjet �������
#ifdef CMT_WIDJET_SYSTEM
  #ifndef CMT_SCREEN
    #error "You must declare CMT_SCREEN"
  #endif
  #include <Sys/Gui/cmtWidjetSystem.h>
#endif

//==========================================================================
//                               �������
//#define CMT_PARSER
#ifdef CMT_ELF_PARSER
  #ifndef CMT_FS
    #error "CMT:You must define file system"
  #endif
  #include <Sys/Dev/Parser/cmtParser.h>
#endif

//==============================================================================================================================================
//                                ������� ���������� � ����������

//==========================================================================
//                               GPS - ��������� NMEA
#ifdef CMT_GPS
  #ifndef CMT_SERIAL
    #error "You must declare serial devices"
  #endif
  #include <Sys/Dev/Ext/Gps/cmtGps.h>
#endif

//==========================================================================
//                               GSM - ��������� 
#ifdef CMT_GSM
  #ifndef CMT_QUEUE
    #error "You must declare queue"
  #endif
  #ifndef CMT_SERIAL
    #error "You must declare serial devices"
  #endif
  #include <Sys/Dev/Ext/Gsm/cmtGsm.h>
#endif

//==========================================================================
//                            ���������� AMIS-30522
// ������� �������� ���������
#ifdef AMIS_30522
  #include <Sys/Dev/Ext/StepMotor/cmtAmis30522.h>
#endif

//==========================================================================
//                            ���������� L3G4200
// �������� MEMS
#ifdef ST_L3G4200
  #include <Sys/Dev/Ext/StMems/cmtStMems.h>
  #include <Sys/Dev/Ext/StMems/cmtStL3g4200.h>
#endif

//==========================================================================
//                            ���������� LIS3DH
// ������������ MEMS
#ifdef ST_LIS3DH
  #include <Sys/Dev/Ext/StMems/cmtStMems.h>
  #include <Sys/Dev/Ext/StMems/cmtStLis3dh.h>
#endif

//==========================================================================
//                            ���������� CAT24Cxx
// ���������������� ������ EEPROM
#ifdef ONSEMI_CAT24Cxx
  #ifndef CMT_FHELPER
    #error "CMT: You must define CMT_FHELPER"
  #endif
  #ifndef CMT_MEMORY_BASE
    #error "CMT: You must define CMT_MEMORY_BASE"
  #endif

  #include <Sys/Dev/Ext/Eeprom/cat24cxx.h>
#endif

//==========================================================================
//                            ��������������� ���������� ����� (����)
//
#if defined(CMT_PLIS_ALTERA) //���� ����� Altera
  #include <Sys/Dev/Ext/Plis/cmtPlis.h>
#endif

//==========================================================================
//                            Board Support Packet
void cmtInit(); //��������������� ������������� (����������� ��� ������ �������)

