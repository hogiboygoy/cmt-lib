//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

/*������� ������������� ������� (��������) ������ � ������� ���������� ���������.
  ��� ���� ���������� ������� ������ ����� ��������� (����������� ���������) ���
  �� ��������� � ������������ ��� ��������. ��� ����� ����� ������������ �����
  � ������� �� ������.
  */
void
CmtWidjetSystem::SetWidjet(CmtWidjet *widjet, bool prevDelete) {
  //��������� ������������� ��������
  if( prevDelete ) {
    if( widjet && mWidjet && widjet != mWidjet->Calling() )
      widjet->SetCalling( mWidjet->Calling() );
    //��������� ����������� �������� � ��������� ��������
    if( mWidjet && mWidjet != widjet ) {
      if( mDelete ) delete mDelete;
      mDelete = mWidjet;
      }
    }
  else {
    if( mWidjet )
      widjet->SetCalling( mWidjet );
    }
  //��������� ����� ������
  mWidjet = widjet;
  
  if( mWidjet ) {
    //��� �����-������������ ������� ��������� ���������� ������ ����� � �����������
    mWidjet->SetDirty();
    }
  }

void
CmtWidjetSystem::Loop() {
  if( mScreen ) {
    //������� ���������� ������
    mScreen->WaitEndOfFrame();
    
    if( mWidjet ) {
      //��������� ������� ������� ��� ��������
      if( mDelete ) {
        delete mDelete;
        mDelete = 0;
        }

      //���������� � ����� ������
      mWidjet->NextFrame();

      //��������� ������� ������� ��� ��������
      if( mDelete ) {
        delete mDelete;
        mDelete = 0;
        }

      //���� ��������� ����������� - ������������
      if( mWidjet->Flags() & CMT_WF_DIRTY ) {
        //������ ������ ���������� �� ������
        mWidjet->Paint( mScreen );
        //��������� ����� � ���������� ���������.
        //����� ����� ���������� ����� ������� ��� ������������ �������� �� ������ ������ � ����������
        mScreen->FrameComplete();
        }
      }
    }
  }

