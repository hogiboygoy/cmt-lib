//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

bool
CmtLongPress::Step(bool press) {
  bool res = false;
  if( !mPrevSw && press ) {
    //������ ��������, ������� ���������
    cmtSetCheckTick( mLongTime, mPressInterval );
    mPress = true;
    }
  if( press && mPrevSw && mPress ) {
    //��������� ������� �������
    if( cmtCheckTick(mLongTime) ) {
      //���������� ��� �������� �������
      res = true;
      //�������� ���� �������
      mPress = false;
      }
    }
  mPrevSw = press;
  return res;
  }
