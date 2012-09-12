//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
  
void 
CmtStMems::WrAdr( uint8 adr ) {
  mSpi->Select();
  mSpi->Transfer( adr, &adr, 0 );
  }
  
uint8 
CmtStMems::WrReg8( uint8 adr, uint8 val ) {
  WrAdr( adr );
  mSpi->Transfer( val, &val, 0 );
  mSpi->UnSelect();
  return val;
  }

void 
CmtStMems::WrReg16( uint8 adr, uint16 val ) {
  WrAdr( adr | 0x40 );
  mSpi->Transfer( val, &adr, 0 );
  mSpi->Transfer( val >> 8, &adr, 0 );
  mSpi->UnSelect();
  }
  
uint8 
CmtStMems::RdReg8( uint8 adr ) {
  uint8 val;
  WrAdr( adr | 0x80 );
  mSpi->Transfer( 0, &val, 0 );
  mSpi->UnSelect();
  return val;
  }
  
uint16 
CmtStMems::RdReg16( uint8 adr ) {
  uint8 val0, val1;
  WrAdr( adr | 0x80 | 0x40 );
  mSpi->Transfer( 0, &val0, 0 );
  mSpi->Transfer( 0, &val1, 0 );
  mSpi->UnSelect();
  return val0 | (val1 << 8);
  }
