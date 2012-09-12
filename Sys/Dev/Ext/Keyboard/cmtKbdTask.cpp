//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
uint32 cmtKbdKey = 0;

uint32 
cmtKbdPeekKey() {  //���������� ��� ������� ������� (0-���� ��� �������)
  return cmtKbdKey;
  }

uint_8
cmtKbdGetKey() {  //���������� ��� ������� ������� � ���������� ������� (���� ��� ������� - ������� �������)
  uint32 key;
  if( cmtKbdKey == 0 ) {
    //������� �������
    cmtWaitSetUInt32( &cmtKbdKey );
    }
  key = cmtKbdKey;
  cmtKbdKey = 0;
  return key;
  }

void
cmtKbdTask() {
  static uint32 curKey = 0;
  static uint_8 drimmleCount = 0;
  static uint_8 repeateCount;
  while(1) {
    uint32 key = cmtHalKbdGetPress();
    if( key == curKey ) {
      if( drimmleCount == 51 ) {
        //������� ������ �����
        if( repeateCount == (CMT_KBD_REPEATE_PERIOD / CMT_KBD_PERIOD) ) {
          cmtKbdKey = curKey;
          repeateCount = 0;
          }
        else repeateCount++;
        }
      else if( drimmleCount == 50 ) {
        //������� ������ �����
        if( repeateCount == (CMT_KBD_REPEATE_PAUSE / CMT_KBD_PERIOD) ) {
          cmtKbdKey = curKey;
          drimmleCount = 51;
          repeateCount = 0;
          }
        else repeateCount++;
        }
      else if( drimmleCount == 4 ) {
        //��� ��������
        if( curKey != 0 ) {
          cmtKbdKey = curKey;
          drimmleCount = 50;
          repeateCount = 0;
          }
        }
      else drimmleCount++;
      }
    else {
      //��� ��������
      curKey = key;
      drimmleCount = 0;
      }
    cmtWaitTick( CMT_KBD_PERIOD ); //����� ���������� ����� ������ 10��
    }
  }
