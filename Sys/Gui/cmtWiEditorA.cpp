//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiEditorA::CmtWiEditorA(CmtWidjet *back, CmtWiListMenuItem *items, CmtPoint menuPos, uint32 menuCurPos, CmtPoint helpPos, uint32 menuGap, uint32 sel ) :
  CmtWiBack(back),
  mMenu(0),
  mHelp(0),
  mCurIndex(0xffffffff) {
  //Определить количество пунктов меню
  for( mItemCount = 0; items[mItemCount].mDefImageFName; mItemCount++ );
  
  //Выделить память для указателей на картинки
  mDefaultImgs = new CmtImagePtr[mItemCount];
  mActiveImgs = new CmtImagePtr[mItemCount];
  mHelpImgs =  new CmtImagePtr[mItemCount];
  
  //Выполнить загрузку картинок
  for( uint32 i = 0; i < mItemCount; i++ ) {
    //Картинка для кнопки по умолчанию
    mDefaultImgs[i] = new CmtImage( (uint32*)(items[i].mDefImageFName) );
    //Картинка для активной кнопки
    if( items[i].mActiveImageFName )
      mActiveImgs[i] = new CmtImage( (uint32*)(items[i].mDefImageFName) );
    else mActiveImgs[i] = 0;
    //Картинка для помощи
    if( items[i].mHelpImageFName )
      mHelpImgs[i] = new CmtImage( (uint32*)(items[i].mHelpImageFName) );
    else mHelpImgs[i] = 0;
    }
  
  //Создать меню
  mMenu = new CmtWiListVMenu( mDefaultImgs, mActiveImgs, menuGap, mItemCount, menuCurPos );
  mMenu->SetPos( menuPos );
  mMenu->SetParent(this);
  //Установить выбор по умолчанию
  mMenu->SetCurrentCellIndex( sel );
  
  
  mHelp = new CmtWiPtrImage( this );
  mHelp->SetPos( helpPos );
  mHelp->SetParent( this );
  }

CmtWiEditorA::~CmtWiEditorA() {
  //Удалить картинки
  for( uint32 i = 0; i < mItemCount; i++ ) {
    if( mDefaultImgs[i] ) delete mDefaultImgs[i];
    if( mActiveImgs[i] ) delete mActiveImgs[i];
    if( mHelpImgs[i] ) delete mHelpImgs[i];
    }
  //Удалить указатели картинок
  delete mDefaultImgs;
  delete mActiveImgs;
  delete mHelpImgs;
  }

void
CmtWiEditorA::OnSelect(uint32 index) {
  if( index < mItemCount )
    mHelp->SetImage( mHelpImgs[index], false );
  else
    mHelp->SetImage( 0, false );
  }

void
CmtWiEditorA::OnNextFrame() {
  //Проверить, есть ли движение
  uint32 index = mMenu->CurrentCellIndex();
  if( mCurIndex != index ) {
    mCurIndex = index;
    OnSelect( index ); 
    }
  }
  
bool
CmtWiEditorA::Event( CmtEventType evType, int32 evCode, CmtPoint pos ) {
  if( mMenu ) {
    if( evType == cetClick ) {
      if( evCode == cvkEnter ) {
        //Выбор сделан, вычислить индекс текущего выбора
        Select( mMenu->CurrentCellIndex() );
        return true;
        }
      }
    return mMenu->Event( evType, evCode, pos );
    }
  return false;
  }

