//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
uint32 cmtKbdGetKey();      //���������� ��� ������� ������� � ���������� ������� (���� ��� ������� - ������� �������)
uint32 cmtKbdPeekKey();     //���������� ��� ������� ������� (0-���� ��� �������)

uint32 cmtHalKbdGetPress(); //���������� ��� ����������

void   cmtKbdTask();        //������� ������ ������ ����������