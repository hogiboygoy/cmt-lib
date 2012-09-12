//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_CAT24Cxx_H
#define CMT_CAT24Cxx_H

class CmtCat24Cxx : public CmtMemoryBase {
    CmtI2cBase *mI2c;
  public:
    CmtCat24Cxx( CmtI2cBase *i2c ) : CmtMemoryBase(), mI2c(i2c) {}
    
    virtual uint_8 WriteByte( uint32 addr, uint8 value );
    
    virtual uint8  ReadByte( uint32 addr, uint_8 *state = 0 );
    
    virtual uint_8 WriteBlock( const uint8 *src, uint32 addr, uint32 count );
    virtual uint_8 ReadBlock( uint8 *dst, uint32 addr, uint32 count );
    
  };


#endif
