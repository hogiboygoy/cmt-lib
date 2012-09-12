//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

void
CmtWiFileInfoText::SetFile(CmtFile &is) {
  //Прочитать содержимое файла в буфер
  CmtBuffer8 buf( is.Size() );
  is.Read( buf.Buffer(), is.Size() );
  //Проверить наличие фонового контекста
  SetContext();
  if( mContext ) {
    mContext->Clear( MC_WHITE32 );
    mContext->SetFont(10);
    mContext->Text( MC_BLACK32, CmtPoint(0,0), buf.Str() );
    }
  }

