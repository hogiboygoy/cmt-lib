//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWidjet::~CmtWidjet() {
  //������ ������ �� ������������� �������
  if( mParent ) mParent->ChildRemove(this);

  //������� ��� �������� �������
  CmtWidjet *dptr;
  for( CmtWidjet *ptr = mChild; ptr; ptr = dptr ) {
    ptr->mParent = 0;
    dptr = ptr->mNext;
    delete ptr;
    }
  
  //������ �������� ���������, ���� ����
  if( mContext ) delete mContext;
  
  //������ ��������, ���� ����
  if( mAnimation ) delete mAnimation;
  
  //������ ��������������
  //if( mLayout ) delete mLayout;
  }

void
CmtWidjet::SetPos( CmtPoint pos ) {
  mPos = pos;
  SetDirty();
  }

void
CmtWidjet::SetSize( CmtPoint size ) {
  mSize = size;
  //���� ���� �����, �������� ��� ������
  if( mContext ) mContext->SetSize( mSize );
  SetDirty();
  }

void
CmtWidjet::SetParent( CmtWidjet *parent ) {
  if( mParent ) mParent->ChildRemove(this);
  mParent = parent;
  if( mParent ) mParent->ChildAdd(this);
  }

void
CmtWidjet::SetDirty() {
  if( mParent ) mParent->SetDirty();
  mFlags |= CMT_WF_DIRTY;
  }

CmtWidjet*
CmtWidjet::ChildAdd(CmtWidjet *child) {
  CmtWidjetPtr *ptr;
  for( ptr = &mChild; (*ptr); ptr = &((*ptr)->mNext) );
  *ptr = child;
  child->mNext = 0;
  return child;
  }

void
CmtWidjet::ChildRemove(CmtWidjet *child) {
  for( CmtWidjetPtr *ptr = &mChild; (*ptr); ptr = &((*ptr)->mNext) )
    if( *ptr == child ) {
      *ptr = child->mNext;
      child->mNext = 0;
      return;
      }  
  }

void
CmtWidjet::SetChildFocus( CmtWidjet *widjet ) {
  //��������, �������� �� widjet �������� ��� �������
  if( widjet && widjet->Parent() == this ) {
    mChildFocus = widjet;
    while( (mChildFocus->Flags() & CMT_WF_FOCUS) == 0 ) {
      //������� � ���������� �������
      mChildFocus = mChildFocus->mNext;
      if( mChildFocus == 0 ) return;
      }
    }
  }

void
CmtWidjet::KillChildFocus() {
  mChildFocus = 0;
  }

void
CmtWidjet::NextChildFocus(bool round) {
  //���������� ������� � ����������� �������
  CmtWidjet *next;
  if( mChildFocus ) {
    next = mChildFocus;
    while(1) {
      next = next->Next(round);
      if( next == mChildFocus || next == 0 ) break;
      if( next->Flags() & CMT_WF_FOCUS ) {
        SetChildFocus( next );
        break;
        }
      }
    }
  else SetChildFocus( mChild );
  }

void
CmtWidjet::PrevChildFocus(bool round) {
  CmtWidjet *prev;
  if( mChildFocus ) {
    prev = mChildFocus;
    while(1) {
      prev = prev->Prev( round );
      if( prev == mChildFocus || prev == 0 ) break;
      if( prev->Flags() & CMT_WF_FOCUS ) {
        SetChildFocus( prev );
        break;
        }
      }
    }
  }

CmtWidjet*
CmtWidjet::Prev(bool round) {
  if( mParent ) {
    if( mParent->Child() == this ) {
      if( round ) {
        //��� ����� ��������� �������
        CmtWidjet *ptr;
        for( ptr = mParent->Child(); ptr->mNext; ptr = ptr->mNext );
        return ptr;
        }
      return this;
      }
    for(CmtWidjet *ptr = mParent->Child(); ptr; ptr = ptr->mNext )
      if( ptr->mNext == this ) return ptr;
    }
  return this;
  }

CmtWidjet*
CmtWidjet::Next(bool round) {
  if( mNext ) return mNext;
  if( round ) {
    if( mParent ) return mParent->Child();
    }
  return this;
  }

void
CmtWidjet::NextFrame() {
  //���������� � ����� ������
  for( CmtWidjet *ptr = mChild; ptr; ptr = ptr->mNext )
    ptr->NextFrame();
  //��������� ��������� �������� �� ����� ������
  OnNextFrame();
  //��������� ����� ������ ��� ��������
  if( mAnimation )
    mAnimation->OnNextFrame( this );
  }

void
CmtWidjet::Paint(CmtContext *ctx) {
  if( mFlags & CMT_WF_VISIBLE ) {
    //���� ������ �������, �� ���������� �������� ���������
    CmtContextInfo info;
    if( !ctx->Push( info, mPos, mSize, mContext ) ) return;
    
    //���� ������� - ������������
    if( mFlags & CMT_WF_DIRTY || mContext == 0 ) {
      //�������� ����������
      OnPaint( ctx );
      //�������� ���� ���������
      mFlags &= ~CMT_WF_DIRTY;
      //�������� �������� ��������
      for( CmtWidjet *ptr = mChild; ptr; ptr = ptr->mNext )
        ptr->Paint( ctx );
      }
   
    ctx->Pop( info );
    
    if( mContext ) {
      if( mAnimation )
        mAnimation->OnPaint( this, ctx, mContext );
      else
        ctx->Copy( mContext, mPos, mSize );
      }
    }
  }

void
CmtWidjet::SetContext() {
  if( mContext == 0 ) {
    mContext = new CmtContext( mSize, 0 );
    }
  }

void       
CmtWidjet::SetAnimation( CmtAnimation *ani ) { 
  if( mAnimation != ani && mAnimation ) delete mAnimation; 
  mAnimation = ani;
  if( mAnimation ) mAnimation->OnSetup( this );
  }

bool
CmtWidjet::Event(CmtEventType evType, int32 evCode, CmtPoint pos) {
  //pos - ������� � ������������ �������
  pos -= mPos; //��������������� �� ������� ������ �������
  if( mChildFocus == 0 || !mChildFocus->Event(evType, evCode, pos) ) {
    //�������� �������� �� ���������, ���������� ��������� (���� ����)
    if( mAnimation && mAnimation->Event( evType, evCode, pos ) ) return true;
    //�������� ���������� ����� ��������� ����������
    for( CmtWidjet *ptr = mChild; ptr; ptr = ptr->mNext )
      if( ptr->Event( evType, evCode, pos) ) return true;
    return false;
    }
  return true;
  }

void
CmtWidjet::SetIndirect( uint8 xDiv, uint8 xPos, uint8 xSize, uint8 yDiv, uint8 yPos, uint8 ySize, int32 align, int32 xGap, int32 yGap ) {
  if( mParent ) {
    //�������� ������ ������������� ����
    CmtPoint size( mParent->Size() );
    //�������� ������� �� ����
    if( xDiv == 0 ) xDiv = 1;
    if( yDiv == 0 ) yDiv = 1;
    //��������� ������ ����� �����
    size.x /= xDiv;
    size.y /= yDiv;
    //���������
    CmtPoint pos(size.x * xPos, size.y * yPos );
    //������
    //��������� ������� ������
    if( xSize == 0 ) xSize = 1;
    if( ySize == 0 ) ySize = 1;
    size.x *= xSize;
    size.y *= ySize;
    //���������� ������
    CmtPoint s( Size() );
    if( align & MP_SIZE_HORZ ) {
      s.x = size.x - 2 * xGap;
      pos.x += xGap;
      }
    if( align & MP_SIZE_VERT ) {
      s.y = size.y - 2 * yGap;
      pos.y += yGap;
      }
    if( align & (MP_SIZE_HORZ | MP_SIZE_VERT) ) SetSize( s );
    //���������� ���������
    if( align & MP_ALIGN_HORZ ) {
      switch( align & (MP_HORZ_MASK | MP_SIZE_HORZ) ) {
        case MP_CENTER : pos.x += (size.x - s.x) / 2; break;
        case MP_RIGHT  : pos.x += (size.x - s.x - xGap); break;
        case MP_LEFT   : pos.x += xGap; break;
        }
      }
    if( align & MP_ALIGN_VERT ) {
      switch( align & (MP_VERT_MASK | MP_SIZE_VERT) ) {
        case MP_MIDDLE : pos.y += (size.y - s.y) / 2; break;
        case MP_TOP    : pos.y += yGap; break;
        case MP_BOTTOM : pos.y += (size.y - s.y - yGap); break;
        }
      }
    SetPos( pos );
    }
  }
