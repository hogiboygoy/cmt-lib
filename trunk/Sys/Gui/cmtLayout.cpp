//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#if 0
CmtLayout::~CmtLayout() {
  }

void
CmtLayout::Paint( CmtPoint ctxPos, CmtPoint size, CmtContext *ctx ) {
  ctxPos += mPos;
  size -= mPos;
  if( size.x > mSize.x ) size.x = mSize.x;
  if( size.y > mSize.y ) size.y = mSize.y;
  for( CmtWidjet *ptr = mChild; ptr; ptr = ptr->mNext )
    ptr->Paint( ctxPos, size, ctx );
  }

void
CmtLayout::NextFrame() {
  for( CmtWidjet *ptr = mChild; ptr; ptr = ptr->mNext )
    ptr->NextFrame();
  }
#endif
