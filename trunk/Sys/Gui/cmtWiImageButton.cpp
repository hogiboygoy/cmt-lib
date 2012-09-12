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
      //Показ нажатия завершен, послать код
      if( mParent ) mParent->Event( cetMenu, mCode, CmtPoint() );
      }
    else {
      //Обновить контекст при смене фрейма
      if( mContext ) {
        //Если контекст присутствует, то назначить контекст на текущий фрейм картинки
        mContext->Setup( mPressImage.Size(), mPressImage.Frame( mFrameIndex ) );
        //Обновить картинку
        SetDirty();
        }
      mFrameIndex++;
      }
    }
  else if( mDefImage.FrameCount() > 1 ) {
    //Очередной фрейм при обычной работе
    mFrameIndex = mDefImage.NextFrameIndex( mFrameIndex );
    //Обновить контекст при смене фрейма
    if( mContext ) {
      //Если контекст присутствует, то назначить контекст на текущий фрейм картинки
      mContext->Setup( mDefImage.Size(), mDefImage.Frame( mFrameIndex ) );
      //Обновить картинку
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
