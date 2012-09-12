//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiTranspImage::CmtWiTranspImage( const void *src) :
  CmtWidjet(0),
  mImage( (uint32*)src ),
  mFrameIndex(0) {
    SetFrame( 0 );
    }


void
CmtWiTranspImage::OnNextFrame() {
  //Переходим к следующему фрейму
  mFrameIndex = mImage.NextFrameIndex( mFrameIndex );
  //Обновить картинку
  if( mImage.FrameCount() > 1 ) SetDirty();
  }

void
CmtWiTranspImage::OnPaint( CmtContext *ctx ) {
  if( !mImage.Empty() ) {
    uint32 *frame = mImage.Frame( mFrameIndex );
    ctx->CopyTranspRectSafe( frame[0], mImage.Size(), frame, mImage.Size().x, CmtPoint() );
    }
  }

void     
CmtWiTranspImage::SetFrame( uint32 frameIndex ) {
  //Установить текущий фрейм
  mFrameIndex = frameIndex >= mImage.FrameCount() ? 0 : frameIndex;
  //Перерисовать
  SetDirty();
  }


void     
CmtWiTranspImage::SetupImage( uint32 *src ) {
  mImage.Setup( src );
  mSize = mImage.Size();
  SetFrame( 0 );
  }

void     
CmtWiTranspImage::LoadImage( cpchar fname ) {
  mImage.Load( fname );
  mSize = mImage.Size();
  SetFrame( 0 );
  }

void     
CmtWiTranspImage::DefLoadImage( cpchar fname ) {
  mImage.DefLoad( fname );
  mSize = mImage.Size();
  SetFrame( 0 );
  }
