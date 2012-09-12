//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Core/Misc/cmtQueue.h>

uint32
CmtQueueEngine::Put() {
  uint32 res = mHead;
  if( !Full() ) mHead++;
  if( mHead >= mCount ) mHead = 0;
  return res;
  }

uint32
CmtQueueEngine::Get() {
  uint32 res = mTail;
  if( !Empty() ) mTail++;
  if( mTail >= mCount ) mTail = 0;
  return res;
  }


void
CmtQueueFile::Read(pvoid dest, uint32 size) {
  if( dest == 0 || size == 0 ) return;
  uint32 c;
  uint8 *dst = (uint8*)dest;
  while(1) {
    c = cmtUMin( size, mQueue.Count() );
    if( c ) {
      for( uint32 i = 0; i < c; i++ )
        *dst++ = Get();
      size -= c;
      if( size == 0 ) return;
      }
    //ќжидать некоторого заполнени€ буфера
//    CmtEvQueueFill( &mQueue, cmtUMin(mQueue.Size()/2,size) ).Wait2( mBreak );
    CmtEvQueueFill( &mQueue, 1 ).Wait2( mBreak );
    if( mBreak && mBreak->IsSignaled() ) return;
    }
  }

void
CmtQueueFile::Write(cpvoid sour, uint32 size) {
  if( sour == 0 || size == 0 ) return;
  uint32 c;
  const uint8 *src = (const uint8*)sour;
  while(1) {
    c = cmtUMin( size, mQueue.Free() );
    if( c ) {
      for( uint32 i = 0; i < c; i++ )
        Put( *src++ );
      size -= c;
      if( size == 0 ) return;
      }
    //ќжидать освобождени€ в буфере некоторого места
    CmtEvQueueFree( &mQueue, 1 ).Wait2( mBreak );
    if( mBreak && mBreak->IsSignaled() ) return;
    }
  }
