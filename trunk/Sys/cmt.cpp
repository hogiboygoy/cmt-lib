//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//==========================================================================
//                               Debug
#include <Sys/cmtDebug.cpp>

//==========================================================================
//                               CPU
#include <Sys/Core/Cpu/cmtCpu.cpp>

//==========================================================================
//                               �������������
#include <Sys/Core/Misc/cmtInit.cpp>

//==========================================================================
//                               ���������� ��������
#ifdef CMT_KERNEL_MAP
  #include <Sys/Core/Task/cmtTask.cpp>
  #include <Sys/Core/Task/cmtEvent.cpp>
#endif

//==========================================================================
//                               �������
#ifdef CMT_EVENT_UINT32
  #include <Sys/Core/Task/cmtEventUInt32.cpp>
#endif
#ifdef CMT_EVENT_UINT_8
  #include <Sys/Core/Task/cmtEventUInt_8.cpp>
#endif

//==========================================================================
//                               ��������� ������
#ifdef CMT_SYS_TIMER_MAP
  #include <Sys/Core/SysTimer/cmtSysTimer.cpp>
#endif

//==========================================================================
//                               ���������� NLib
#ifdef CMT_NDATE
  #include <NLib/Src/NDate.cpp>
#endif

//==========================================================================
//                               ��������� � ����
#ifdef CMT_SYSTEM_TIME_MAP
  #include <Sys/Core/Calendar/cmtSystemTime.cpp>
#endif

//==========================================================================
//                               ���������� �����
#ifdef CMT_MEMORY_LOC
  #include <Sys/Core/Mem/cmtMemoryLoc.cpp>
#endif

#ifdef CMT_MEMORY_STD
  #include <Sys/Core/Mem/cmtMemoryStd.cpp>
#endif

//==========================================================================
//                               ����� ��������������� �������
#ifdef CMT_FHELPER
  #include <string.h>
  #include <Sys/Core/Misc/cmtFHelper.cpp>
#endif

//==========================================================================
//                               ����� ����������� �������
#ifdef CMT_BUFFER
  #include <Sys/Core/Misc/cmtBuffer.cpp>
#endif

//==========================================================================
//                               �������
#ifdef CMT_QUEUE
  #include <Sys/Core/Misc/cmtQueue.cpp>
#endif

//==========================================================================
//                               �����
#ifdef CMT_STACK
  #include <Sys/Core/Misc/cmtStack.cpp>
#endif

//==========================================================================
//                               ������
#ifdef CMT_STRING
  #include <Sys/Core/Misc/cmtString.cpp>
  #include <Sys/Core/Misc/cmtPath.cpp>
#endif

//==========================================================================
//                               ������������, ���, CRC
#ifdef CMT_CRYPTO
  #include <Sys/Core/Misc/cmtCrypto.cpp>
#endif

//==========================================================================
//                               ������ ���������� �������
#ifdef CMT_PRINT_PHIS
  #ifndef CMT_STRING
    #error "CMT:You must declare string support"
  #endif
  #include <Sys/Core/Misc/cmtPrintPhis.cpp>
#endif

//==========================================================================
//                               ������ �������� �����
#ifdef CMT_BLINK_TASK
#include <Sys/Dev/Ext/Led/cmtBlinkTask.cpp>
#endif

//==========================================================================
//                            ������������ ���������
#ifdef CMT_LED
  #ifndef CMT_SYS_TIMER_MAP
    #error "You must declare system timer"
  #endif
  #include <Sys/Dev/Ext/Led/cmtLedArrayBase.cpp>
#endif

//==========================================================================
//                            ������������ ��������� ���
#ifdef CMT_LED_PWM
  #include <Sys/Dev/Ext/Led/cmtLedPwm.cpp>
#endif

//==========================================================================
//                               ������ ������ ����������
#ifdef CMT_KBD_TASK
  #include <Sys/Dev/Ext/Keyboard/cmtKbdTask.cpp>
#endif

#ifdef CMT_SLIDER     //������������� ���������
  #include <Sys/Dev/Ext/Keyboard/cmtSlider.cpp>
#endif

#ifdef CMT_ROLL_SLIDER //������������� ��������� � ��������������
  #include <Sys/Dev/Ext/Keyboard/cmtRollSlider.cpp>
#endif

#ifdef CMT_LONG_PRESS //������������� �������� �������
  #include <Sys/Dev/Ext/Keyboard/cmtLongPress.cpp>
#endif

//==========================================================================
//                               �������
#if defined(CMT_PARSER_BIN_COPY) || defined(CMT_PARSER_ELF)
#include <Sys/Dev/Parser/cmtParseBinCopy.cpp>
#endif

#ifdef CMT_PARSER_ELF
#include <Sys/Dev/Parser/cmtElfAbsParser.cpp>
#endif

//==========================================================================
//                               ����������� ����������
#ifdef CMT_DEV
  #include <Sys/Dev/cmtDev.cpp>
#endif

//==========================================================================
//                               ����������� ���������� DMA
#ifdef CMT_DMA
  #include <Sys/Dev/Abstract/Dma/cmtDmaBase.cpp>
#endif

//==========================================================================
//                               ����������� ���������������� ����������
#ifdef CMT_SERIAL
  #include <Sys/Dev/Abstract/Serial/cmtSerial.cpp>
#endif

//==========================================================================
//                               ���� SPI
#ifdef CMT_SPI
  #include <Sys/Dev/Abstract/Spi/cmtSpi.cpp>
#endif

//==========================================================================
//                               ���� I2C
#ifdef CMT_I2C
  #include <Sys/Dev/Abstract/I2c/cmtI2c.cpp>
#endif

//==========================================================================
//                               ����������� ������
#ifdef CMT_MEMORY_BASE
  #include <Sys/Dev/Abstract/Memory/cmtMemoryBase.cpp>
#endif

//==========================================================================
//                               ������� ����������
#ifdef CMT_BLOCK_DEVICE
  #include <Sys/Dev/Abstract/Block/cmtBlockDevice.cpp>
#endif

//==========================================================================
//                               ������� RAM-�����
#ifdef CMT_MEMORY_DISK
  #include <Sys/Dev/Abstract/Block/cmtMemoryDisk.cpp>
#endif

//==========================================================================
//                               ������� SD �����
#ifdef CMT_SD_CARD
  #include <Sys/Dev/Abstract/Sd/cmtSDcard.cpp>
#endif

#ifdef CMT_SPI_SD_CARD
  #include <Sys/Dev/Abstract/Sd/cmtSpiSdCard.cpp>
#endif

//==========================================================================
//                               �������� �������
#ifdef CMT_FS_FAT               
  #ifndef CMT_FS
    #error "You must declare base file system"
  #endif
  #ifndef CMT_SYSTEM_TIME_MAP
    #error "You must declare system time and calendar"
  #endif
  #include <Sys/Dev/Abstract/Fs/Fat/cmtFatDir.cpp>
  #include <Sys/Dev/Abstract/Fs/Fat/cmtFat.cpp>
  #include <Sys/Dev/Abstract/Fs/Fat/cmtFatFile.cpp>
#endif

#ifdef CMT_FS_FAT12
  #ifndef CMT_FS_FAT
    #error "You must declare FAT file system"
  #endif
  #include <Sys/Dev/Abstract/Fs/Fat/cmtFat12.cpp>
#endif

#ifdef CMT_FS_FAT16
  #ifndef CMT_FS_FAT
    #error "You must declare FAT file system"
  #endif
  #ifndef CMT_FS_FAT12
    #error "You must declare FAT12 file system"
  #endif
  #include <Sys/Dev/Abstract/Fs/Fat/cmtFat16.cpp>
#endif

#ifdef CMT_FS_FAT32
  #ifndef CMT_FS_FAT
    #error "You must declare FAT file system"
  #endif
  #ifndef CMT_FS_FAT16
    #error "You must declare FAT16 file system"
  #endif
  #include <Sys/Dev/Abstract/Fs/Fat/cmtFat32.cpp>
#endif

#ifdef CMT_FS_WIN
  #include <Sys/Dev/Abstract/Fs/cmtWinFs.cpp>
#else
  #ifdef CMT_FS
    #include <Sys/Dev/Abstract/Fs/cmtFileSystemBase.cpp>
  #endif
#endif

//==========================================================================
//                               USB DEVICE
#ifdef CMT_USB_DEV
  #include <Sys/Dev/Abstract/Usb/cmtUsbDev.cpp>
#endif

#ifdef CMT_USB_DEV_COM
  #include <Sys/Dev/Abstract/Usb/cmtUsbDevCom.cpp>
#endif

#ifdef CMT_USB_DEV_COM_UART
#include <Sys/Dev/Abstract/Usb/cmtUsbDevComUart.cpp>
#endif

#ifdef CMT_USB_DEV_BULK
  #include <Sys/Dev/Abstract/Usb/cmtUsbDevBulk.cpp>
#endif

#ifdef CMT_USB_DEV_MASS
  #include <Sys/Dev/Abstract/Usb/cmtUsbDevMsc.cpp>
#endif

//==========================================================================
//                               USB HOST
#ifdef CMT_USB_HOST
  #include <Sys/Dev/Abstract/Usb/cmtUsbHost.cpp>
#endif


//==========================================================================
//                               USART � UART
#ifdef CMT_UART
  #include <Sys/Dev/Abstract/Uart/cmtUart.cpp>
#endif

//==========================================================================
//                            ��������� �� ���������
#ifdef CMT_GEOMETRY
  #include <Sys/Gui/cmtGeometry.cpp>
#endif

//==========================================================================
//                            ����������� ��������
#ifdef CMT_IMAGE
  #include <Sys/Gui/cmtImage.cpp>

  #ifdef CMT_IMAGE_SAVE
    #include <Sys/Gui/cmtImageSave.cpp>
  #endif
#endif

//==========================================================================
//                            ����������� �������� ���������
#ifdef CMT_CONTEXT
  #include <Sys/Gui/cmtContext.cpp>
  #ifdef CMT_DRAW_LINEAR_GRAPHIC
    #include <Sys/Gui/cmtDrawLinearGraphic.cpp>
  #endif
#endif

//==========================================================================
//                            ����������� �����
#ifdef CMT_SCREEN_WEG010016
  #include <Sys/Gui/cmtScreenWEG010016.cpp>
#endif

#if defined(CMT_SCREEN_TI_OMAP3530)
  #include <Sys/Gui/cmtScreenTiOmap3530.cpp>
#endif

#ifdef CMT_SCREEN_WIN32
  #include <Sys/Gui/cmtScreenWin32.cpp>
#endif

//==========================================================================
//                            ������� ��������� �������
#ifdef CMT_CONSOLE
  #include <Sys/Gui/cmtConsole.cpp>
#endif


//==========================================================================
//                            Widjet �������
#ifdef CMT_WIDJET_SYSTEM
  #include <Sys/Gui/cmtAnimation.cpp>
  #include <Sys/Gui/cmtWaMove.cpp>
  #include <Sys/Gui/cmtWaMoveCell.cpp>
  #include <Sys/Gui/cmtWaExhibition.cpp>

  #include <Sys/Gui/cmtWidjet.cpp>
  #include <Sys/Gui/cmtWiBack.cpp>
  #include <Sys/Gui/cmtWiListEngine.cpp>
  #include <Sys/Gui/cmtWiListVEngine.cpp>
  #include <Sys/Gui/cmtWiListBase.cpp>
  #include <Sys/Gui/cmtWiListHorz.cpp>
  #include <Sys/Gui/cmtWiBigListBase.cpp>
  #include <Sys/Gui/cmtWiListVMenu.cpp>
  #include <Sys/Gui/cmtWiMenuWListHelp.cpp>
  #include <Sys/Gui/cmtWiImage.cpp>
  #include <Sys/Gui/cmtWiTranspImage.cpp>
  #include <Sys/Gui/cmtWiImageButton.cpp>
  #include <Sys/Gui/cmtWiPtrImage.cpp>
  #include <Sys/Gui/cmtWiFileInfoText.cpp>
  #include <Sys/Gui/cmtWiTextListBase.cpp>
  #include <Sys/Gui/cmtWiTextList.cpp>
  #include <Sys/Gui/cmtWiFileNameList.cpp>
  #include <Sys/Gui/cmtWiEditorA.cpp>


  #include <Sys/Gui/cmtWidjetSystem.cpp>

  #include <Sys/Gui/cmtLayout.cpp>
  #ifdef CMT_WI_CONSOLE
    #include <Sys/Gui/cmtWiConsole.cpp>
  #endif
#endif

//==========================================================================
//                               �������
//#define CMT_PARSER
#ifdef CMT_ELF_PARSER
  #include <Sys/Dev/Parser/cmtElfAbsParser.cpp>
#endif

//==============================================================================================================================================
//                                ������� ���������� � ����������
//==========================================================================
//                               GPS - ��������� NMEA
#ifdef CMT_GPS
  #include <Sys/Dev/Ext/Gps/cmtGps.cpp>
#endif

//==========================================================================
//                               GSM - ��������� 
#ifdef CMT_GSM
  #include <Sys/Dev/Ext/Gsm/cmtGsm.cpp>
#endif

//==========================================================================
//                            ���������� �������� �����������
#ifdef CMT_STEP_MOTOR
  #include <Sys/Dev/Ext/StepMotor/cmtStepMotor.cpp>
#endif

//==========================================================================
//                            ���������� AMIS-30522
// ������� �������� ���������
#ifdef AMIS_30522
  #include <Sys/Dev/Ext/StepMotor/cmtAmis30522.cpp>
#endif

//==========================================================================
//                            ���������� ST MEMS
#if defined( ST_L3G4200 ) || defined( ST_LIS3DH )
  #include <Sys/Dev/Ext/StMems/cmtStMems.cpp>
#endif

//==========================================================================
//                            ���������� CAT24Cxx
// ���������������� ������ EEPROM
#ifdef ONSEMI_CAT24Cxx
  #include <Sys/Dev/Ext/Eeprom/cat24cxx.cpp>
#endif


//==========================================================================
//                            ��������������� ���������� ����� (����)
//
#ifdef CMT_PLIS_ALTERA //���� ����� Altera
  #include <Sys/Dev/Ext/Plis/cmtPlisAltera.cpp>
#endif

