//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiListEngine::CmtWiListEngine(CmtWidjet *parent) :
  CmtWidjet(parent),
  mCount(0),
  mStart(0),
  mPage(0),
  mView(0),
  mCur(0) {

  }

bool
CmtWiListEngine::Event(CmtEventType evType, int32 evCode, CmtPoint pos) {
  if( evType == cetClick ) {
    switch( evCode ) {
      case cvkUp     : mCur--; break;
      case cvkDown   : mCur++; break;
      case cvkPgUp   : mCur -= mPage; break;
      case cvkPgDown : mCur += mPage; break;
      default : return false;
      }
    mCur = cmtILimit( mCur, 0, mCount - 1 );
    if( mCur < mStart ) mStart = mCur;
    if( mCur > (mStart + mView - 1) ) mStart = mCur - (mView - 1);
    SetDirty();
    return true;
    }
  return false;
  }

void
CmtWiListEngine::PaintContens(CmtContext *ctx) {
  //Рисовать видимые элементы
  for( int32 i = mStart, j = 0; i < mCount && j < mView; i++, j++)
    PaintItem( ctx, j, i );
  //Рисовать скроллер
  PaintScroll( ctx );
  }

