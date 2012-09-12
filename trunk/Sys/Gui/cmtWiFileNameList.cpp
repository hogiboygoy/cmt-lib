//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiFileNameList::CmtWiFileNameList(CmtWiTextList *list, CmtWiFileInfo *info, cpchar fnamePat, CmtPath *res, CmtPoint cellSize, uint32 gap, uint32 curCellPos ) :
  CmtWiBack(0),
  mList(list),
  mInfo(info),
  mInfoExhibition(0),
  mResult(res) {
  //�������� ���� �� ������� ������
  mPath = fnamePat;
  mPath.CutPath();
  if( mList ) {
    //���������� ��������
    mList->SetParent( this );
    //�������� ����� � ������
    for( CmtFileFinder finder(fnamePat); finder.IsGood(); finder.Next() )
      mList->Add( (cpchar)(finder.mAttr.cAlternateFileName) );
    //���������� ��������� ������
    mList->SetupTextListParam( cellSize, gap, curCellPos );
    //�������� ������� ����
    if( mInfo ) {
      mInfo->SetParent( this );
      CmtPath fname( mPath );
      fname.Add( mList->GetCurrentCellText() );
      CmtFileRead is( fname.Buf() );
      if( is.Good() )
        mInfo->SetFile( is );
      //�������� ����������
      mInfoExhibition = new CmtWaExhibition();
      mInfo->SetAnimation( mInfoExhibition );
      }
    }
  }

void
CmtWiFileNameList::OnNextFrame() {
  if( mList == 0 || mInfo == 0 || mInfoExhibition == 0 ) return;
  //���������, ���� �� ��������
  if( mList->IsMove() ) mInfoExhibition->Hide( false );
  else {
    if( mInfoExhibition->Level() == 0 ) {
      mInfoExhibition->Show( true );
      CmtPath fname( mPath );
      fname.Add( mList->GetCurrentCellText() );
      CmtFileRead is( fname.Buf() );
      if( is.Good() )
        mInfo->SetFile( is );
      }
    }
  }

bool
CmtWiFileNameList::Event( CmtEventType evType, int32 evCode, CmtPoint pos ) {
  if( mList ) {
    if( evType == cetClick ) {
      if( evCode == cvkEnter ) {
        //����� ������, ��������� ������ �������� ������
        Select( mList->GetCurrentCellText() );
        return true;
        }
      if( evCode == cvkEsc ) {
        Select( 0 );
        return true;
        }
      }
    return mList->Event( evType, evCode, pos );
    }
  return false;
  }

