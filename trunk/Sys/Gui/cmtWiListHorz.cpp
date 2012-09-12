//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

void
CmtWiListHorz::SetCellParam(CmtPoint size, uint32 gap, uint32 cellCount, uint32 curCellPos) {
  Setup( size.x, gap, cellCount, curCellPos );
  }

CmtPoint
CmtWiListHorz::CellPos(int cellIndex) {
  CmtPoint p(mGap,mGap);
  p.x += CellPosBase(cellIndex);
  return p;
  }

bool
CmtWiListHorz::Event( CmtEventType evType, int32 evCode, CmtPoint pos ) {
  if( evCode == cvkUp || evCode == cvkDown ) return false;
  if( evCode == cvkLeft ) evCode = cvkUp;
  if( evCode == cvkRight ) evCode = cvkDown;
  return CmtWiListBase::Event( evType, evCode, pos );
  }
