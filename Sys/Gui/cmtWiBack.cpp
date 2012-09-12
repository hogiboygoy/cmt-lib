//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

void
CmtWiBack::SetBack(CmtWidjet *back) {
  if( mBack && back != mBack ) delete mBack;
  mBack = back;
  if( mBack ) mBack->SetSize( Size() );
  }

