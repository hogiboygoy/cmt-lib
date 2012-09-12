//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#include <Sys/Core/Misc/cmtBuffer.h>

CmtBuffer32::CmtBuffer32(uint32 size) :
  mBuffer(0),
  mSize(0) {
  if( size ) {
    mBuffer = new uint32[size];
    if( mBuffer ) mSize = size;
    }
  }

bool
CmtBuffer32::Resize(uint32 size) {
  if( size > mSize ) {
    uint32 *tmp;
    tmp = new uint32[size];
    if( tmp ) {
      //Копировать старое содержимое
      if( mSize ) memcpy( tmp, mBuffer, mSize * sizeof(uint32) );
      if( mBuffer ) delete mBuffer;
      mBuffer = tmp;
      mSize = size;
      return true;
      }
    return false;
    }
  return true;
  }

