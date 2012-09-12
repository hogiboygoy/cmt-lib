//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiTextList::CmtWiTextList( CmtContext *cash, uint32 color ) :
  CmtWiTextListBase( cash, color ),
  mBuffer( 1024 ),
  mDelta( 2048 ),
  mPos(0),
  mStringCount(0),
  mAddEnable(true ) {
  
  }

void
CmtWiTextList::Add(cpchar str) {
  if( str == 0 || str[0] == 0 ) return;
  //����� ����������� ������
  uint32 len = strlen(str) + 1;
  //��������� ������ �� ����������� ������ � �����
  if( len + mPos > mBuffer.Size() ) 
    if( !mBuffer.Resize( mBuffer.Size() + mDelta + len ) ) return;
  //���������� ������ � �����
  strcpy( mBuffer.Str() + mPos, str );
  //�������� ������� � ������ ��� ����� �����
  mPos += len;
  //��������� �������� �����
  mStringCount++;
  }

void
CmtWiTextList::SetupTextListParam(CmtPoint cellSize, uint32 gap, uint32 curCellPos) {
  if( mAddEnable ) {
    //���������� ���������, ���������������� ������ �����������
    Alloc( mStringCount + 1 );
    //���������� ��������� �� ������ ������
    uint32 k = 0;
    for( uint32 i = 0; i < mPos; i++ ) {
      //��������� �� ����������� ������
      cpchar ptr = (cpchar)(mBuffer.Buffer() + i);
      //�������� ������
      SetCellText( k++, ptr ) ;
      i += strlen( ptr );
      }
    //��������� ������ �������
    SetCellText( mStringCount, 0 );
    //��� ������ ���������, ���������� ���������
    SetupTextPtrParam( cellSize, gap, curCellPos );
    //������������� ����������� ���������� �����
    mAddEnable = false;
    }
  }
