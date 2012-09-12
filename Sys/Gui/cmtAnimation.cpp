//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

void
CmtAnimation::OnPaint(CmtWidjet *widjet, CmtContext *dest, CmtContext *src) {
  dest->Copy( src, widjet->Pos(), widjet->Size() );
  }

