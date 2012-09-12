//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
CmtSystemTime::CmtSystemTime() :
mYear(2010),
mMonth(1),
mDay(1),
mHour(0),
mMinute(0),
mSecond(0) {
  //GetSystem();
  }

uint16
CmtSystemTime::FFTime() {
  uint16 tm;
  tm = mHour;
  tm <<= 6;
  tm |= mMinute;
  tm <<= 5;
  tm |= mSecond >> 1;
  return tm;
  }

uint16
CmtSystemTime::FFDate() {
  uint16 tm = mYear - 1980;
  tm &= 0x7f;
  tm <<= 4;
  tm |= mMonth;
  tm <<= 5;
  tm |= mDay;
  return tm;
  }

//-------------------------------------------------------------------------------
//                Реализации
#if CMT_SYSTEM_TIME_MAP == 0
//Generic
void
CmtSystemTime::GetSystem() {
  mYear      = 2009;
  mMonth     = 1;
  mDayOfWeek = 0;
  mDay       = 12;
  mHour      = 10;
  mMinute    = 0;
  mSecond    = 0;
  }

void 
cmtInitSystemTime() {
  }
#else //CMT_SYSTEM_TIME_MAP == 0

#if defined(CMT_DESCTOP_QT)
#include <Sys/Core/Calendar/cmtDesctopQt.cpp>
#elif defined(CMT_STM32F105VCT6)
#include <Sys/Core/Calendar/cmtStm32F10x.cpp>
#else
#error "CMT: This calendar map not defined for this CPU"
#endif

#endif

