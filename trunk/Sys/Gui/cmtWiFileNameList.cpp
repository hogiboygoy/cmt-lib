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
  //Выделить путь из шаблона поиска
  mPath = fnamePat;
  mPath.CutPath();
  if( mList ) {
    //Установить родителя
    mList->SetParent( this );
    //Накопить файлы в список
    for( CmtFileFinder finder(fnamePat); finder.IsGood(); finder.Next() )
      mList->Add( (cpchar)(finder.mAttr.cAlternateFileName) );
    //Установить параметры списка
    mList->SetupTextListParam( cellSize, gap, curCellPos );
    //Получить текущий файл
    if( mInfo ) {
      mInfo->SetParent( this );
      CmtPath fname( mPath );
      fname.Add( mList->GetCurrentCellText() );
      CmtFileRead is( fname.Buf() );
      if( is.Good() )
        mInfo->SetFile( is );
      //Механизм проявления
      mInfoExhibition = new CmtWaExhibition();
      mInfo->SetAnimation( mInfoExhibition );
      }
    }
  }

void
CmtWiFileNameList::OnNextFrame() {
  if( mList == 0 || mInfo == 0 || mInfoExhibition == 0 ) return;
  //Проверить, есть ли движение
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
        //Выбор сделан, вычислить индекс текущего выбора
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

