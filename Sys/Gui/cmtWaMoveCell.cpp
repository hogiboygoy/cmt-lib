//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWaMoveCell::CmtWaMoveCell() :
  CmtWaMove(),
  mCellSize(10),
  mCellCount(1),
  mStartPos(0),
  mTargetPos(0) {
  mCellLast = (mCellCount - 1) * mCellSize;
  mAccelLen = AccelLen();
  }
  
void
CmtWaMoveCell::SetCellParam( uint32 cellSize, uint32 cellCount ) {
  mCellSize = cellSize;
  mCellCount = cellCount;
  mStartPos = mTargetPos = 0;
  mCellLast = (mCellCount - 1) * mCellSize;
  mAccelLen = AccelLen();
  }

void
CmtWaMoveCell::OnNextFrame(CmtWidjet *widjet) {
  //���������� ������ ��������
  CmtWaMove::OnNextFrame(widjet);
  if( CmtWaMove::IsMove() ) {
    //��������
    if( mDir ) {
      //� ������ �����������
      if( mStartPos + mAccelLen >= mTargetPos ) Stop( true );
      mStartPos += Speed();
      }
    else {
      //� �������� �����������
      if( mStartPos - mAccelLen <= mTargetPos ) Stop( true );
      mStartPos -= Speed();
      }
    }
  else {
    //����������, ������� ���������� �� ������ ������
    if( mStartPos < mTargetPos ) 
      mStartPos++;
    if( mStartPos > mTargetPos ) 
      mStartPos--;
    }
  }

bool
CmtWaMoveCell::Event( CmtEventType evType, int32 evCode, CmtPoint pos ) {
  if( evType != cetClick ) return false;
  //����������, ������ ��������
  mAccelLen = AccelLen();
  
  //���� ���� ���, �� �������� �� ��������� ������
  if( (evCode == cvkUp && mTargetPos < mCellLast) || (evCode == cvkDown && mTargetPos > 0) ) {
    mTargetPos += mCellSize * (evCode == cvkUp ? 1 : -1);
    if( mTargetPos > mCellLast ) 
      mTargetPos = mCellLast;
    if( mTargetPos < 0 )
      mTargetPos = 0;
    int32 path;
    if( mTargetPos > mStartPos ) {
      //�������� ������
      path = mTargetPos - mStartPos;
      mDir = true;
      }
    else {
      //�������� �����
      path = mStartPos - mTargetPos;
      mDir = false;
      }
    //��������������� ���� ���������
    if( path < 2 * mAccelLen )
      mAccelLen = path >> 1;
    Start(true);
    }
  return true;
  }
