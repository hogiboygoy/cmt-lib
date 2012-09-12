//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

void
CmtSystemTime::GetSystem() {
  SYSTEMTIME st;
  GetLocalTime( &st );
  mYear      = st.wYear;
  mMonth     = (uint8)st.wMonth;
  mDayOfWeek = (uint8)st.wDayOfWeek;
  mDay       = (uint8)st.wDay;
  mHour      = (uint8)st.wHour;
  mMinute    = (uint8)st.wMinute;
  mSecond    = (uint8)st.wSecond;
  }
