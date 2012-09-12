//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiListVEngine::CmtWiListVEngine(CmtWidjet *parent, uint32 *scroll) :
  CmtWiListEngine(parent),
  mScrollImage(scroll),
  mScrollPos(0),
  mScrollStart(0),
  mScrollStop(0),
  mContensPos(),
  mCellSize(),
  mCellGap() {}

void
CmtWiListVEngine::PaintItem(CmtContext *ctx, int32 pos, int32 index) {
  //Рассчитать фактическое положение ячейки в контексте
  CmtPoint p( mContensPos );
  p.y += (mCellSize.y + mCellGap.y) * pos;
  PaintCell( ctx, p, index );
  }

void
CmtWiListVEngine::PaintScroll(CmtContext *ctx) {
  //Определить положение скролла в пикселях
  if( mCount - 1 > 0 ) {
    int32 pos = mScrollStart + (mScrollStop - mScrollStart) * mCur / (mCount - 1);
    //Нарисовать
    uint32 *frame = mScrollImage.Frame();
    if( frame ) {
      CmtPoint s = mScrollImage.Size();
      ctx->CopyTranspRectSafe( frame[0], s, frame, s.x, CmtPoint( mScrollPos - s.x / 2, pos - s.y / 2 ) );
      }
    }
  }

