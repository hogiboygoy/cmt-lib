//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWaMove::CmtWaMove() :
  CmtAnimation(),
  mSpeed(0),
  mRun(false),
  mAccel(256),
  mStartSpeed(0),
  mRunSpeed(10)
  {}

void
CmtWaMove::Start(bool accel) {
  mRun = true;
  if( accel ) {
    if( mSpeed == 0 ) {
      mSpeed = mStartSpeed;
      mSpeed <<= 8;
      }
    }
  else {
    mSpeed = mRunSpeed;
    mSpeed <<= 8;
    }
  }

void
CmtWaMove::Stop(bool accel) {
  mRun = false;
  if( !accel ) mSpeed = 0;
  }

void
CmtWaMove::OnNextFrame( CmtWidjet* ) {
  if( mAccel == 0 ) mAccel = 1;
  if( mRun ) {
    //Производим ускорение
    if( mSpeed < (mRunSpeed << 8) ) mSpeed += mAccel;
    }
  else {
    //Производим замедление
    if( mSpeed > (mStartSpeed << 8) ) {
      if( mSpeed > mAccel ) mSpeed -= mAccel;
      else mSpeed = 0;
      }
    }
  }

uint32
CmtWaMove::AccelLen() {
  uint32 len = 0;
  uint32 sp;
  for( sp = mStartSpeed << 8; sp < (mRunSpeed << 8); sp += mAccel );
  while( sp > (mStartSpeed << 8) ) {
    if( sp > mAccel ) sp -= mAccel;
    else sp = 0;
    len += sp >> 8;
    }
  return len;
  }

