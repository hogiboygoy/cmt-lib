//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiListVMenu::CmtWiListVMenu( CmtImagePtr *def, CmtImagePtr *act, uint32 gap, uint32 cellCount, uint32 cellPos ) :
  CmtWiListBase(),
  mDefault(def),
  mActive(act)
  {
  SetCellParam( mDefault[0]->mSize, gap, cellCount, cellPos );
  }

void
CmtWiListVMenu::PaintCell( int cellIndex, CmtContext *ctx, int ) {
  if( cellIndex >= 0 ) {
    if( (uint32)cellIndex == CurrentCellIndex() && mActive[cellIndex] != 0 ) ctx->Image( CellPos(cellIndex), mActive[cellIndex], 0 );
    else ctx->Image( CellPos(cellIndex), mDefault[cellIndex], 0 );
    }
  }

