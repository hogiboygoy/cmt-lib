//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//Считаем количество секунд с 2010
void
CmtSystemTime::GetSystem() {
  NDate d( 1, 1, 2010 );
  uint32 days = RTC_GetCounter();
  uint32 time = days % (24 * 3600);
  days /= 24 * 3600; //Количество дней с 1.01.2010
  d += days;
  mYear      = d.Year();
  mMonth     = d.Month();
  mDayOfWeek = d.WeekDay();
  mDay       = d.DayOfMonth();
  mHour      = time / 3600;
  time %= 3600;
  mMinute    = time / 60;
  mSecond    = time % 60;
  }

void
CmtSystemTime::SetSystem() {
  NDate base( 1, 1, 2010 );
  NDate d( mDay, mMonth, mYear );
  uint32 days = d - base;
  days *= (24 * 3600);
  days += mHour * 3600;
  days += mMinute * 60;
  days += mSecond;
  RTC_SetCounter( days );
  }

void 
cmtInitSystemTime() {
  }

