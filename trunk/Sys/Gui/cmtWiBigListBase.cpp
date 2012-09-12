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
  mCashStart = mMoveCell->StartPos() + 1; //���������� ����������� ����
  }

void
CmtWiBigListBase::OnPaint(CmtContext *ctx) {
  if( mCash == 0 || mMoveCell == 0 ) {
    //��������� ������ ����
    CmtPoint b( mPos );
    b += mSize;
    ctx->FilledRect( mPos, b, mBackColor );
    return;
    }
  //���������, ��������� �� ���������� ���� (�.�. ��������� �� ��� ������� ����)
  int cashSize = mCash->GetSize().y;
  if( mMoveCell->StartPos() < mCashStart || (mMoveCell->StartPos() + mSize.y) > (mCashStart + cashSize) ) {
    //��������� ���������� ���������
    
    //�������� ��������
    mCash->Clear( mBackColor );
    
    mCash->SetFont( mFontSize );
    
    int cellSize = mMoveCell->CellSize();
    
    //��������� ����� �������� ���������
    mCashStart = (mMoveCell->StartPos() - (cashSize >> 2)); // / mMoveCell->CellSize()) * mMoveCell->CellSize();
    
    //��������� ��������
    int y = (mCashStart - mCurCellPos);    //���������� ������ ������������ ������
    int i = y / cellSize - 1; //������ ��������, ���������������� ������������ ������ ������
    y = i * cellSize + mCurCellPos - mCashStart;
    for( ; i < (int)mCellCount; ++i ) {
      if( i >= 0 ) {
        //��������� ��������� ��������
        PaintCell( i, mCash, y );
        }
      y += cellSize;
      if( y > cashSize ) break;
      }
    }
  //�������� ��� � ������ �������
  ctx->CopyRectSafe( mSize, mCash->GetData( CmtPoint(0,mMoveCell->StartPos() - mCashStart) ), mCash->GetSize().x, mPos );
  //ctx->CopyRectSafe( mSize, mCash->GetData(), mCash->GetSize().x, mPos );
  }

void
CmtWiBigListBase::PaintCell(int cellIndex, CmtContext *ctx, int pos) {
  //�� ��������� ������ �����
  ctx->Text( MC_BLACK32, CmtPoint(0,pos), GetCellText( cellIndex ) );
  }
