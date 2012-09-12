//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

uint_8 cmtEventSignal1( void *ptr ) {
  return ((CmtEvent*)ptr)->IsSignaled();
  }

uint_8 cmtEventSignal2(void *ptr) {
  return ((CmtEvent*)ptr)->IsSignaled() || ((CmtEvent*)ptr)->mNext->IsSignaled();
  }

uint_8 cmtEventSignal3(void *ptr) {
  return ((CmtEvent*)ptr)->IsSignaled() || ((CmtEvent*)ptr)->mNext->IsSignaled() || ((CmtEvent*)ptr)->mNext->mNext->IsSignaled();
  }

void
CmtEvent::Wait() {
  cmtWaitEvent( cmtEventSignal1, this );
  }

void
CmtEvent::Wait2(CmtEvent *ev) {
  if( ev == 0 )
    cmtWaitEvent( cmtEventSignal1, this );
  else {
    mNext = ev;
    cmtWaitEvent( cmtEventSignal2, this );
    }
  }

void
CmtEvent::Wait3( CmtEvent *ev1, CmtEvent *ev2 ) {
  if( ev1 == 0 ) Wait2( ev2 );
  else if( ev2 == 0 ) Wait2( ev1 );
  else {
    mNext = ev1;
    ev1->mNext = ev2;
    cmtWaitEvent( cmtEventSignal2, this );
    }
  }
