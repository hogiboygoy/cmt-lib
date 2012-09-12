//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

//CmtWiTextListBase::CmtWiTextListBase( CmtContext *cash, uint32 color, uint32 textPtrMax ) :
//  CmtWiBigListBase( cash, color ), 
//  mTextPtrList(0), 
//  mOwner(false), 
//  mTextPtrMax(0) { 
//    Alloc( textPtrMax ); 
//    }

CmtWiTextListBase::CmtWiTextListBase(CmtContext *cash, uint32 color ) :
  CmtWiBigListBase( cash, color ), 
  mTextPtrList(0), 
  mOwner(false), 
  mTextPtrMax(0) { 
    }

void
CmtWiTextListBase::Free() {
  if( mTextPtrList && mOwner ) delete mTextPtrList; 
  mTextPtrList = 0; 
  mOwner = false;
  }

void
CmtWiTextListBase::Alloc(uint32 textPtrMax) {
  Free();
  if( textPtrMax ) {
    mTextPtrList = new cpchar[textPtrMax]; 
    mTextPtrMax = textPtrMax; 
    mOwner = true;
    }
  else {
    mTextPtrList = 0;
    mTextPtrMax = 0;
    mOwner = false;
    }
  }

void
CmtWiTextListBase::SetupTextPtrList( cpchar *src ) {
  if( src == 0 ) {
    Free();
    }
  else {
    mOwner = false;
    mTextPtrList = src;
    }
  }

void
CmtWiTextListBase::SetupTextPtrParam(CmtPoint cellSize, uint32 gap, uint32 curCellPos) {
  if( mTextPtrList == 0 ) return;
  //Подсчитать количество строк
  for( mTextPtrMax = 0; mTextPtrList[mTextPtrMax]; mTextPtrMax++ );
  SetCellParam( cellSize, gap, mTextPtrMax, curCellPos );
  }

extern "C" {
int
cpcharCompareUp( const void *ptr1, const void *ptr2 ) {
  return cmtCompareStrNoReg( ((cpchar*)ptr1)[0], ((cpchar*)ptr2)[0] );
  }

int
cpcharCompareDown( const void *ptr1, const void *ptr2 ) {
  return -cmtCompareStrNoReg( ((cpchar*)ptr1)[0], ((cpchar*)ptr2)[0] );
  }
}

void
CmtWiTextListBase::Sort(bool up) {
  qsort( mTextPtrList, mTextPtrMax, sizeof(cpchar), up ? cpcharCompareUp : cpcharCompareDown );
  }

