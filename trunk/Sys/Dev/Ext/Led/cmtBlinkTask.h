//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//������ ���������� ��������� ������������
void cmtBlinkTask();

//������ ���� ����������
void cmtHalBlinkTaskOn( uint_8 on, uint_8 code );

//��� �������� - ������
#define CMT_BLINK_TASK_STEP( ledCode, LedOn, LedOff ) \
  if( on && ledCode > code ) LedOn; \
  if( !on && ledCode != CMT_BLINK_TASK ) LedOff;
