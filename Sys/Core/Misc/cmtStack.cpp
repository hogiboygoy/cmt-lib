//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
CmtStackUint32::CmtStackUint32( uint32 size ) :
  mBuf(0),
  mTop(size),
  mCount(size) {
    mBuf = new uint32[size];
    if( mBuf == 0 ) mCount = mTop = 0;
  }

uint_8
CmtStackUint32::Push( uint32 val ) {
  if( mTop == 0 ) return CMTE_FAIL;
  mBuf[--mTop] = val;
  return CMTE_OK;
  }

void
CmtStackUint32::HardPush( uint32 val ) {
  if( mTop == 0 ) {
    //Продвинуть весь стек исключив один элемент
    for( uint32 i = mCount - 1; i; i-- )
      mBuf[i] = mBuf[i-1];
    mBuf[mTop] = val;
    }
  else mBuf[--mTop] = val;
  }

void   
CmtStackUint32::WaitAndPush( uint32 val ) {
  if( mTop == 0 ) WaitLE( mCount - 1, 0x10000000 );
  Push( val );
  }

uint32 
CmtStackUint32::WaitAndPop() {
  if( mTop == mCount ) WaitGE( 1, 0x10000000 );
  return mBuf[mTop++];
  }

uint32  
CmtStackUint32::Look( uint32 index ) {
  return mBuf[ mTop + index ];
  }

uint32 
CmtStackUint32::Count() const {
  return mCount - mTop;
  }

uint_8 
CmtStackUint32::CanPlace() const {
  return Count() < mCount - 1;
  }

void   
CmtStackUint32::Clear() {
  mTop = mCount;
  }

struct CmtStackSignal {
  CmtStackUint32 *mStack;
  uint32          mCount;
  uint32          mTime;
  };

uint_8
cmtStackLESignal( void *ptr ) {
  CmtStackSignal *signal = (CmtStackSignal*)ptr;
  return signal->mStack->Count() <= signal->mCount || cmtCheckTick( signal->mTime );
  }

uint_8
cmtStackGESignal( void *ptr ) {
  CmtStackSignal *signal = (CmtStackSignal*)ptr;
  return signal->mStack->Count() >= signal->mCount || cmtCheckTick( signal->mTime );
  }

uint_8 
CmtStackUint32::WaitLE( uint32 count, uint32 timeOut ) {
  CmtStackSignal signal;
  cmtSetCheckTick( signal.mTime, timeOut );
  signal.mStack = this;
  signal.mCount = count;
  cmtWaitEvent( cmtStackLESignal, &signal );
  return cmtCheckTick( signal.mTime ) ? CMTE_TIME_OUT : CMTE_OK;
  }

uint_8 
CmtStackUint32::WaitGE( uint32 count, uint32 timeOut ) {
  CmtStackSignal signal;
  cmtSetCheckTick( signal.mTime, timeOut );
  signal.mStack = this;
  signal.mCount = count;
  cmtWaitEvent( cmtStackGESignal, &signal );
  return cmtCheckTick( signal.mTime ) ? CMTE_TIME_OUT : CMTE_OK;
  }

