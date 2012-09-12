//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_LED_PWM_H
#define CMT_LED_PWM_H

//������ ���������� ��������� ������������
void cmtLedPwmTask();

//������ ���� ����������
void cmtHalLedPwmTaskOn( uint_8 code );

//��� �������� - ������
#define CMT_LED_PWM_TASK_STEP( ledCode, LedOn, LedOff ) \
  if( ledCode > code ) LedOn; \
  else                 LedOff;

//CMT_TASK_ADD( cmtLedPwmTask, 0, 100 );

#endif
