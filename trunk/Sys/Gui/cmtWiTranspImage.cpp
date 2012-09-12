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
  //��������� � ���������� ������
  mFrameIndex = mImage.NextFrameIndex( mFrameIndex );
  //�������� ��������
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
  //���������� ������� �����
  mFrameIndex = frameIndex >= mImage.FrameCount() ? 0 : frameIndex;
  //������������
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
