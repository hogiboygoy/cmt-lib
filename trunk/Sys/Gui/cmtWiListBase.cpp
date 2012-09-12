//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiListBase::CmtWiListBase() :
  CmtWidjet(0),
  mCellSize(100,30),
  mGap(10),
  mCurCellPos(0),
  mPrevPos(0),
  mCellCount(0) {
  mMoveCell = new CmtWaMoveCell();
  }

CmtWiListBase::~CmtWiListBase() {
  delete mMoveCell;
  }

void
CmtWiListBase::OnPaint(CmtContext *ctx) {
  //Рисуем пункты
  for( uint32 i = 0; i < mCellCount; i++ )
    PaintCell( i, ctx, CellPosBase(i) );
  }

bool
CmtWiListBase::Event( CmtEventType evType, int32 evCode, CmtPoint pos ) {
  return mMoveCell->Event( evType, evCode, pos );
  }

void
CmtWiListBase::Setup(int32 cellSize, uint32 gap, uint32 cellCount, uint32 curCellPos) {
  mGap = gap;
  mCurCellPos = curCellPos;
  mCellCount = cellCount;
 // uint32 extraCell = (curCellPos + cellSize - 1 - mGap) / (cellSize + mGap);
  mMoveCell->SetCellParam( cellSize + mGap, cellCount );
  }

void
CmtWiListBase::OnNextFrame() {
  if( mMoveCell == 0 ) return;
  mMoveCell->OnNextFrame( this );
  if( mPrevPos != mMoveCell->StartPos() ) {
    mPrevPos = mMoveCell->StartPos();
    SetDirty();
    }
  }

uint32
CmtWiListBase::CurrentCellIndex() {
  if( mMoveCell == 0 ) return 0;
  return (mMoveCell->StartPos() + mMoveCell->CellSize() / 2) / mMoveCell->CellSize();
  }

void
CmtWiListBase::SetCurrentCellIndex( uint32 index ) {
  if( mMoveCell == 0 ) return;
  mMoveCell->SetStartPos( index * mMoveCell->CellSize() );
  SetDirty();
  }

int32
CmtWiListBase::CellPosBase(int cellIndex) {
  if( mMoveCell == 0 ) return 0;
  return cellIndex * mMoveCell->CellSize() + mCurCellPos - mMoveCell->StartPos();
  }

CmtPoint
CmtWiListBase::CellPos(int cellIndex) {
  CmtPoint p(mGap,mGap);
  if( mContext == 0 ) p += mPos;
  p.y += CellPosBase(cellIndex);
  return p;
  }

void
CmtWiListBase::SetCellParam(CmtPoint size, uint32 gap, uint32 cellCount, uint32 curCellPos) {
  mCellSize = size;
  Setup( size.y, gap, cellCount, curCellPos );
  }
