//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_MEMORY_H
#define CMT_MEMORY_H

//!  ласс абстрактной пам€ти, позвол€ющей манипул€ции со страницами
class CmtMemoryPage : public CmtDev {
  public:
    virtual uint32 PageSize( uint32 pageIndex ) = 0;
    
    virtual uint32 PageCount() = 0;
    
    virtual uint_8 WriteBlock( cpvoid src, uint32 startPage, uint32 pageCount = 1 ) { return CMTE_NOT_RELEASED; }
    
    virtual uint_8 ReadBlock( pvoid dst, uint32 startPage, uint32 pageCount = 1 ) { return CMTE_NOT_RELEASED; }
  };

//!  ласс абстрактной пам€ти, позвол€ющей манипул€ции с байтами
class CmtMemoryBase : public CmtDev {
  public:
    virtual uint_8 WriteByte( uint32 addr, uint8 value ) = 0;
    virtual uint8  ReadByte( uint32 addr, uint_8 *state = 0 ) = 0;
    
    virtual uint_8 WriteBlock( const uint8 *src, uint32 addr, uint32 count );
    virtual uint_8 ReadBlock( uint8 *dst, uint32 addr, uint32 count );
  };

#endif
