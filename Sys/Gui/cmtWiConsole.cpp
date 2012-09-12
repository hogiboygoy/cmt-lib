//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiConsole::CmtWiConsole( int fontSize, CmtWidjet *parent ) :
  CmtWidjet( parent ),
  mFontColor(MC_BLACK32),
  mBackColor(MC_WHITE32),
  mCon(0),
  mFontSize(fontSize) {
    }

CmtWiConsole::~CmtWiConsole() {
  if( mCon ) delete mCon;
  }

void
CmtWiConsole::ScrollStrs() {
  //Продвинуть все строки
  for( int i = 0; i < mSymSize.y - 1; i++ ) {
    //Копируем строки
    memcpy( mCon + i * mSymSize.x, mCon + (i + 1) * mSymSize.x, mSymSize.x );
    }
  }

void
CmtWiConsole::PrintS_W(cpchar str) {
  ScrollStrs();
  //Копируем строку
  memcpy( mCon + (mSymSize.y - 1) * mSymSize.x, str, mSymSize.x );
  mCon[mSymSize.x * mSymSize.y - 1] = 0;
  SetDirty();
  }

void
CmtWiConsole::PrintS(cpchar str) {
  while( str ) {
    cpchar ptr = strchr( str, '\n' );
    int len;
    if( ptr ) len = (ptr - str) - 1;
    else len = strlen( str );
    int cplen = len >= mSymSize.x ? mSymSize.x - 1 : len;

    ScrollStrs();
    memcpy( mCon + (mSymSize.y - 1) * mSymSize.x, str, cplen );
    mCon[(mSymSize.y - 1) * mSymSize.x + cplen] = 0;

    if( cplen == len ) {
      if( ptr ) str = ptr + 1;
      else str = 0;
      }
    else {
      str += cplen;
      }
    }
  SetDirty();
  }

void
CmtWiConsole::SetSize(CmtPoint size) {
  if( mCon ) delete mCon;
  CmtWidjet::SetSize(size);
  mSymSize.y = size.y / (mFontSize + 4) - 1;
  mSymSize.x = size.x / (mFontSize);
  mCon = new uint8[ mSymSize.x * mSymSize.y ];
  memset( mCon, 0, mSymSize.x * mSymSize.y );
  }

void
CmtWiConsole::OnPaint(CmtContext *ctx) {
  CmtPoint pos;
  if( mContext == 0 ) pos = mPos;
  if( mBackColor != TRANSPARENT32 )
    ctx->FilledRect( pos, mSize, mBackColor );
  ctx->SetFont( mFontSize );
  //Отобразить текст
  for( int y = 0; y < mSymSize.y; y++ )
    ctx->Text( mFontColor, CmtPoint( mFontSize + pos.x, y * (mFontSize + 4) + pos.y ), (cpchar)(mCon + y * mSymSize.x) );
  }

