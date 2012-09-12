//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

void
CmtWiBigListBase::SetSize(CmtPoint size) {
  CmtWiListBase::SetSize( size );
  size.y *= 2;
  if( mCash == 0 ) mCash = new CmtContext( size, 0 );
  else mCash->SetSize( size );
  mCashStart = mMoveCell->StartPos() + 1; //Обеспечить перерисовку кэша
  }

void
CmtWiBigListBase::OnPaint(CmtContext *ctx) {
  if( mCash == 0 || mMoveCell == 0 ) {
    //Закрасить цветом фона
    CmtPoint b( mPos );
    b += mSize;
    ctx->FilledRect( mPos, b, mBackColor );
    return;
    }
  //Проверить, требуется ли обновление кэша (т.е. покрывает ли кэш область окна)
  int cashSize = mCash->GetSize().y;
  if( mMoveCell->StartPos() < mCashStart || (mMoveCell->StartPos() + mSize.y) > (mCashStart + cashSize) ) {
    //Требуется обновление контекста
    
    //Очистить контекст
    mCash->Clear( mBackColor );
    
    mCash->SetFont( mFontSize );
    
    int cellSize = mMoveCell->CellSize();
    
    //Вычислить новое смещение контекста
    mCashStart = (mMoveCell->StartPos() - (cashSize >> 2)); // / mMoveCell->CellSize()) * mMoveCell->CellSize();
    
    //Заполнить контекст
    int y = (mCashStart - mCurCellPos);    //Абсолютное начало виртуального экрана
    int i = y / cellSize - 1; //Индекс элемента, соответствующего виртуальному началу экрана
    y = i * cellSize + mCurCellPos - mCashStart;
    for( ; i < (int)mCellCount; ++i ) {
      if( i >= 0 ) {
        //Выполнить рисование элемента
        PaintCell( i, mCash, y );
        }
      y += cellSize;
      if( y > cashSize ) break;
      }
    }
  //Рисовать кэш в нужной позиции
  ctx->CopyRectSafe( mSize, mCash->GetData( CmtPoint(0,mMoveCell->StartPos() - mCashStart) ), mCash->GetSize().x, mPos );
  //ctx->CopyRectSafe( mSize, mCash->GetData(), mCash->GetSize().x, mPos );
  }

void
CmtWiBigListBase::PaintCell(int cellIndex, CmtContext *ctx, int pos) {
  //По умолчанию рисуем текст
  ctx->Text( MC_BLACK32, CmtPoint(0,pos), GetCellText( cellIndex ) );
  }
