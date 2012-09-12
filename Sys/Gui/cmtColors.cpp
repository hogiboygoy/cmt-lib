//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
uint32
cmtOneLight( uint32 sour ) {
  if( sour + 70 > 255 ) return 255;
  return sour + 70;
  }

uint32
cmtOneShadow( uint32 sour ) {
  if( sour < 70 ) return 0;
  return sour - 70;
  }


uint32
cmtLight( uint32 color ) {
  return MRGB32( cmtOneLight(MVN_R(color)), cmtOneLight(MVN_G(color)), cmtOneLight(MVN_B(color)) );
  }

uint32
cmtShadow( uint32 color ) {
  return MRGB32( cmtOneShadow(MVN_R(color)), cmtOneShadow(MVN_G(color)), cmtOneShadow(MVN_B(color)) );
  }

uint32
cmtOneGradyUp( uint32 sour ) {
  if( sour < 255 ) return sour + 1;
  return sour;
  }

uint32
cmtGradyUp( uint32 color ) {
  return MRGB32( cmtOneGradyUp(MVN_R(color)), cmtOneGradyUp(MVN_G(color)), cmtOneGradyUp(MVN_B(color)) );
  }
