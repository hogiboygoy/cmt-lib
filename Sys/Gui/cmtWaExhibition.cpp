//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

void
CmtWaExhibition::Show(bool smoothly) {
  mDir = true;
  if( smoothly ) {
    if( mLevel < 256 ) mLevel += mStep;
    if( mLevel > 256 ) mLevel = 256;
    }
  else mLevel = 256;
  }

void
CmtWaExhibition::Hide(bool smoothly) {
  mDir = false;
  if( smoothly ) {
    if( mLevel ) mLevel -= mStep;
    if( mLevel < 0 ) mLevel = 0;
    }
  else mLevel = 0;
  }

void
CmtWaExhibition::OnNextFrame(CmtWidjet *widjet) {
  if( mDir ) {
    if( mLevel < 256 ) {
      mLevel += mStep;
      if( mLevel > 256 ) mLevel = 256;
      if( widjet && widjet->Parent() ) widjet->Parent()->SetDirty();
      }
    }
  else {
    if( mLevel ) {
      mLevel -= mStep;
      if( mLevel < 0 ) mLevel = 0;
      if( widjet && widjet->Parent() ) widjet->Parent()->SetDirty();
      }
    }
  }

void
CmtWaExhibition::OnPaint(CmtWidjet *widjet, CmtContext *dest, CmtContext *src) {
  if( mLevel == 0 ) return;
  if( mLevel == 256 ) {
    dest->Copy( src, widjet->Pos(), widjet->Size() );
    }
  else {
    //Выполнить отсечение
    dest->AddRectSafe( mLevel, widjet->Size(), src->GetData(), src->GetSize().x, widjet->Pos() );
    }
  }

