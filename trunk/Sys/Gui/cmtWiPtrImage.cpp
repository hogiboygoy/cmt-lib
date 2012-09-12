//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiPtrImage::CmtWiPtrImage(CmtWidjet *parent) :
  CmtWidjet(parent),
  mImage(0),
  mOwner(false),
  mFrameIndex(0),
  mFrameDisposition(0) {
  }

CmtWiPtrImage::CmtWiPtrImage(CmtWidjet *parent, CmtImage *image, bool owner) :
  CmtWidjet(parent),
  mImage(image),
  mOwner(owner),
  mFrameIndex(0),
  mFrameDisposition(0) {
  if( mImage && mImage->mFrames ) {
    mSize = mImage->mSize;
#ifdef CMT_SYS_TIMER_MAP
    uint32 time = mImage->mFrames ? mImage->mFrames[ 0 ] : 0;
    if( time == 0 ) time = 0x70000000;
    cmtSetCheckTick( mFrameDisposition, time );
#endif
    mContext = new CmtContext( mImage->mSize, mImage->mFrames + 1 );
    }
  }

void
CmtWiPtrImage::SetImage(CmtImage *image, bool owner) {
  Free();
  mImage = image;
  mOwner = owner;
  mFrameIndex = 0;
  if( mImage && mImage->mFrames ) {
    mSize = mImage->mSize;
#ifdef CMT_SYS_TIMER_MAP
    uint32 time = mImage->mFrames ? mImage->mFrames[ 0 ] : 0;
    if( time == 0 ) time = 0x70000000;
    cmtSetCheckTick( mFrameDisposition, time );
#endif
    if( mContext == 0 )
      mContext = new CmtContext( mImage->mSize, mImage->mFrames + 1 );
    else
      mContext->Setup( mImage->mSize, mImage->mFrames + 1 );
    }
  else {
    if( mContext ) delete mContext;
    mContext = 0;
    }
  }

void
CmtWiPtrImage::OnNextFrame() {
  if( mImage == 0 || mImage->mFrames == 0 ) return;
#ifdef CMT_SYS_TIMER_MAP
  if( cmtCheckTick(mFrameDisposition) ) {
    //Переходим к следующему фрейму
    mFrameIndex++;
    //Проверить заворачивание
    if( mFrameIndex >= mImage->mFrameCount ) mFrameIndex = 0;
    //Установить время диспозиции
    uint32 time = mImage->mFrames ? mImage->mFrames[ mFrameIndex * mImage->mFrameSize ] : 0;
    if( time == 0 ) time = 0x70000000;
    cmtSetCheckTick( mFrameDisposition, time );
    SetDirty();
    }
#else
  //Переходим к следующему фрейму
  mFrameIndex++;
  //Проверить заворачивание
  if( mFrameIndex >= mImage->mFrameCount ) mFrameIndex = 0;
#endif
  if( mContext )
    mContext->Setup( mImage->mSize, mImage->mFrames + mFrameIndex * mImage->mFrameSize + 1 );
  }

void
CmtWiPtrImage::Free() {
  if( mImage && mOwner ) delete mImage;
  mImage = 0;
  mOwner = false;
  }

