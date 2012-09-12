//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Core/Misc/cmtString.h>

void
CmtPrintPhisItemOnes( CmtPath &dest, uint32 integer, cpchar text ) {
  dest = "%1 %2";
  dest.Arg( integer ).Arg( text );
  }

void
CmtPrintPhisItemTenth( CmtPath &dest, uint32 integer, uint32 portion, cpchar text ) {
  dest = "%1.%2 %3";
  dest.Arg( integer ).Arg( portion ).Arg( text );
  }

void 
CmtPrintPhisItemTenth( CmtPath &dest, uint32 val, cpchar text ) {
  CmtPrintPhisItemTenth( dest, val / 10, val % 10, text );
  }

void
CmtPrintPhisItemHundredth( CmtPath &dest, uint32 integer, uint32 portion, cpchar text ) {
  dest = "%1.%2 %3";
  dest.Arg( integer ).Arg( portion, 2, 10, '0' ).Arg( text );
  }

void 
CmtPrintPhisItemHundredth( CmtPath &dest, uint32 val, cpchar text ) {
  CmtPrintPhisItemHundredth( dest, val / 100, val % 100, text );
  }

void
CmtPrintPhisDecadaAdaptive( CmtPath &dest, uint32 val, cpchar text ) {
  uint32 portion = val % 1000;
  if( portion < 10 ) CmtPrintPhisItemOnes( dest, val / 1000, text );
  else {
    portion /= 10;
    if( portion % 10 ) CmtPrintPhisItemHundredth( dest, val / 1000, portion, text );
    else CmtPrintPhisItemTenth( dest, val / 100, text );
    }  
  }

void
CmtPrintPhis( CmtPath &dest, uint32 val, cpchar power3, cpchar power6, cpchar power9, cpchar power12 ) {
  if( val < 1000 ) CmtPrintPhisItemOnes( dest, val, power3 );
  else if( val < 1000000 ) CmtPrintPhisDecadaAdaptive( dest, val, power6 );
  else if( val < 1000000000 || power12 == 0 ) CmtPrintPhisDecadaAdaptive( dest, val / 1000, power9 );
  else CmtPrintPhisDecadaAdaptive( dest, val / 1000000, power12 );
  }

CmtPath& 
CmtPrintPhisHz( CmtPath &dest, uint32 val ) {
  CmtPrintPhis( dest, val, tr("Ãö"), tr("êÃö"), tr("ÌÃö"), tr("ÃÃö") );
  return dest;
  }

CmtPath&
CmtPrintPhisNs( CmtPath &dest, uint32 val ) {
  CmtPrintPhis( dest, val, tr("íñ"), tr("ìêñ"), tr("ìñ"), tr("ñ") );
  return dest;
  }

CmtPath& 
CmtPrintPhisMm( CmtPath &dest, uint32 val ) {
  CmtPrintPhis( dest, val, tr("ìì"), tr("ì"), tr("êì"), 0 );
  return dest;
  }
