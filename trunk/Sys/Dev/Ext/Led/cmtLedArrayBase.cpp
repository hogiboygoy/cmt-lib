//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Dev/Ext/Led/cmtLed.h>

void
CmtLedArrayBase::Loop() {
  //��������� ����������
  SetRow(0);
  //���������� ����� �������
  SetColon( mColonIndex );
  //�������� ��������������� ����������
  SetRow( mRows[mColonIndex++] );
  //��������� ������������� �������
  if( mColonIndex >= mColonCount )
    mColonIndex = 0;
  }

void 
cmtLedArrayTask() {
  CmtLedArrayBase *ledArray = (CmtLedArrayBase*)(cmtTaskParam());
  while(1) {
    //��������� ����
    ledArray->Loop();
    //�������� �������
    cmtWaitTick(5);
    }
  }
