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
    //Переходим к следующему фрейму
    SetFrame( mImage.NextFrameIndex( mFrameIndex ) );
    }
#else
  //Переходим к следующему фрейму
  mFrameIndex = mImage.NextFrameIndex( mFrameIndex );
  //Обновить контекст при смене фрейма
  SetupContextOnFrame();
  //Обновить картинку
  if( mImage.FrameCount() > 1 ) SetDirty();
#endif
  }

void     
CmtWiImage::SetupContextOnFrame() {
  if( !mImage.Empty() ) {
    //Если картинка присутствует
    
    //Установить размер виджета в соответствии с картинкой
    mSize = mImage.Size();
    
    //Установить соответствующий контекст
    if( mContext == 0 )
      //Если контекста еще нет, то создать новый контекст
      mContext = new CmtContext( mImage.Size(), mImage.Frame( mFrameIndex ) );
    else
      //Если контекст присутствует, то назначить контекст на текущий фрейм картинки
      mContext->Setup( mImage.Size(), mImage.Frame( mFrameIndex ) );
    }
  else {
    //Картинка отсутствует, удалить контекст
    if( mContext ) delete mContext;
    mContext = 0;
    }
  }

void     
CmtWiImage::SetFrame( uint32 frameIndex ) {
  //Установить текущий фрейм
  mFrameIndex = frameIndex >= mImage.FrameCount() ? 0 : frameIndex;
#ifdef CMT_SYS_TIMER_MAP
  //Установить время диспозиции
  mFrameDisposition = mImage.ExpositionTickTime( mFrameIndex );
#endif
  //Установить контекст на картинку
  SetupContextOnFrame();
  //Перерисовать
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
