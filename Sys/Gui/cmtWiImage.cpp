//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiImage::CmtWiImage( const void *src) :
  CmtWidjet(0),
  mImage( (uint32*)src ),
  mFrameIndex(0) {
    SetFrame( 0 );
    }


void
CmtWiImage::OnNextFrame() {
#ifdef CMT_SYS_TIMER_MAP
  if( cmtCheckTick(mFrameDisposition) ) {
    //��������� � ���������� ������
    SetFrame( mImage.NextFrameIndex( mFrameIndex ) );
    }
#else
  //��������� � ���������� ������
  mFrameIndex = mImage.NextFrameIndex( mFrameIndex );
  //�������� �������� ��� ����� ������
  SetupContextOnFrame();
  //�������� ��������
  if( mImage.FrameCount() > 1 ) SetDirty();
#endif
  }

void     
CmtWiImage::SetupContextOnFrame() {
  if( !mImage.Empty() ) {
    //���� �������� ������������
    
    //���������� ������ ������� � ������������ � ���������
    mSize = mImage.Size();
    
    //���������� ��������������� ��������
    if( mContext == 0 )
      //���� ��������� ��� ���, �� ������� ����� ��������
      mContext = new CmtContext( mImage.Size(), mImage.Frame( mFrameIndex ) );
    else
      //���� �������� ������������, �� ��������� �������� �� ������� ����� ��������
      mContext->Setup( mImage.Size(), mImage.Frame( mFrameIndex ) );
    }
  else {
    //�������� �����������, ������� ��������
    if( mContext ) delete mContext;
    mContext = 0;
    }
  }

void     
CmtWiImage::SetFrame( uint32 frameIndex ) {
  //���������� ������� �����
  mFrameIndex = frameIndex >= mImage.FrameCount() ? 0 : frameIndex;
#ifdef CMT_SYS_TIMER_MAP
  //���������� ����� ����������
  mFrameDisposition = mImage.ExpositionTickTime( mFrameIndex );
#endif
  //���������� �������� �� ��������
  SetupContextOnFrame();
  //������������
  SetDirty();
  }


void     
CmtWiImage::SetupImage( uint32 *src ) {
  mImage.Setup( src );
  SetFrame( 0 );
  }

void     
CmtWiImage::LoadImage( cpchar fname ) {
  mImage.Load( fname );
  SetFrame( 0 );
  }

void     
CmtWiImage::DefLoadImage( cpchar fname ) {
  mImage.DefLoad( fname );
  SetFrame( 0 );
  }
