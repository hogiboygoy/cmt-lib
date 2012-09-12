//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiMenuWListHelp::CmtWiMenuWListHelp( CmtWidjet *back, CmtWiListMenuItem *items, CmtPoint menuPos, uint32 menuCurPos, CmtPoint helpPos, uint32 menuGap, uint32 sel ) :
  CmtWiEditorA( back, items, menuPos, menuCurPos, helpPos, menuGap, sel ) {
  //Механизм проявления
  mHelpExhibition =  new CmtWaExhibition();
  mHelp->SetAnimation( mHelpExhibition );
  }

CmtWiMenuWListHelp::~CmtWiMenuWListHelp() {
  }

void
CmtWiMenuWListHelp::OnNextFrame() {
  CmtWiEditorA::OnNextFrame();
  //Проверить, есть ли движение
  if( mMenu->IsMove() ) mHelpExhibition->Hide( false );
  else {
    if( mHelpExhibition->Level() == 0 ) {
      mHelpExhibition->Show( true );
      }
    }
  }

