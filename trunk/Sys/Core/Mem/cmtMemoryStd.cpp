//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <string.h>
#include <stdlib.h>

void
cmtInitHeap() {
  }

pvoid
cmtMalloc( uint32 size ) {
  return malloc( size );
  }

pvoid
cmtCalloc( uint32 count, uint32 size ) {
  size *= count;
  pvoid ptr = cmtMalloc( size );
  if( ptr ) memset( ptr, 0, size );
  return ptr;
  }

void
cmtFree( pvoid ptr ) {
  free( ptr );
  }

 
