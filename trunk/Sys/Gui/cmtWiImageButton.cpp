//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiImageButton::CmtWiImageButton( CmtWidjet *parent, cpchar def, cpchar press, uint8 code ) :
  CmtWidjet( parent ),
  mDefImage( CIS(def) ),
  mPressImage( CIS(press) ),
  mFrameIndex(0),
  mCode(code),
  mPress(false) {
  if( !mDefImage.Empty() ) {
    mSize = mDefImage.Size();
    mContext = new CmtContext( mSize, mDefImage.Frame( mFrameIndex ) );
    }
  }

void
CmtWiImageButton::OnNextFrame() {
  if( mPress ) {
    if( mFrameIndex == mPressImage.FrameCount() ) {
      //����� ������� ��������, ������� ���
      if( mParent ) mParent->Event( cetMenu, mCode, CmtPoint() );
      }
    else {
      //�������� �������� ��� ����� ������
      if( mContext ) {
        //���� �������� ������������, �� ��������� �������� �� ������� ����� ��������
        mContext->Setup( mPressImage.Size(), mPressImage.Frame( mFrameIndex ) );
        //�������� ��������
        SetDirty();
        }
      mFrameIndex++;
      }
    }
  else if( mDefImage.FrameCount() > 1 ) {
    //��������� ����� ��� ������� ������
    mFrameIndex = mDefImage.NextFrameIndex( mFrameIndex );
    //�������� �������� ��� ����� ������
    if( mContext ) {
      //���� �������� ������������, �� ��������� �������� �� ������� ����� ��������
      mContext->Setup( mDefImage.Size(), mDefImage.Frame( mFrameIndex ) );
      //�������� ��������
      SetDirty();
      }
    }
  }

void
CmtWiImageButton::Press() {
  mPress = true;
  mFrameIndex = 0;
  SetDirty();
  }
