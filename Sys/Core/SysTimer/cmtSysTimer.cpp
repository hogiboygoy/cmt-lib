//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//ru Событие проверки таймера с учетом заворачивания
//ru Максимальное время 3 дня
uint_8 
cmtSignalTick( void *ptr ) {
  return cmtTickCount - *(uint32*)ptr < 0x10000000;
  }

//ru Ожидание заданный интервал (максимум 3 дня)
void 
cmtWaitTick( uint32 tick ) {
  tick += cmtTickCount;
  cmtWaitEvent( cmtSignalTick, &tick );
  }

//Реализации
#if defined(CMT_LPC1768)

#elif defined(CMT_ATMEGA32)
  #include <Sys/Core/SysTimer/cmtAtMega32.cpp>

#elif defined(CMT_STM32F103CBT6) || defined(CMT_STM32F105VCT6) || defined(CMT_STM32F100C8T)
  #include <Sys/Core/SysTimer/cmtStm32f10x.cpp>

#elif defined(CMT_TI_OMAP3530)
  #include <Sys/Core/SysTimer/cmtTiOMAP3530.cpp>

#elif defined(CMT_STM8L151K4)
  #include <Sys/Core/SysTimer/cmtStm8l15x.cpp>
  
#elif defined(CMT_QT)
#else
  #error "CMT:No system timer defined for this CPU"
#endif
