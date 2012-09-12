//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! Преобразователь битовых полей. Позволяет преобразовывать порядок следования битов
struct CmtBitMap8 {
    const uint8 mMap[8];
    
    uint8 Map( uint8 src ) const;
  };

uint8
CmtBitMap8::Map(uint8 src) const {
  uint8 res = 0;
  uint_8 i = 0;
  while( src ) {
    if( src & 1 ) res |= mMap[i];
    src >>= 1;
    i++;
    }
  return res;
  }

struct CmtBitMap16 {
    const uint16 mMap[16];
    
    uint16 Map( uint16 src ) const;
  };

uint16
CmtBitMap16::Map(uint16 src) const {
  uint16 res = 0;
  uint_8 i = 0;
  while( src ) {
    if( src & 1 ) res |= mMap[i];
    src >>= 1;
    i++;
    }
  return res;
  }

struct CmtBitMap32 {
    const uint32 mMap[32];
    
    uint32 Map( uint32 src ) const;
  };

uint32
CmtBitMap32::Map(uint32 src) const {
  uint32 res = 0;
  uint_8 i = 0;
  while( src ) {
    if( src & 1 ) res |= mMap[i];
    src >>= 1;
    i++;
    }
  return res;
  }

//! Один или несколько выводов, действующих как одно целое
class CmtPin {
  public:
    virtual void SetDirIn( bool in = true ) = 0;
    
    virtual bool DirIn() const = 0;
    
    virtual bool In() const = 0;
    
    virtual void Set() = 0;
    
    virtual void Reset() = 0;
  };

//! Группа из восьми выводов (восьмибитная шина)
class CmtPinGroup8 {
  public:
    
    virtual void  SetDirIn( uint8 dirIn = 0xff ) = 0;
    
    virtual uint8 DirIn() const = 0;
    
    virtual uint8 In() const = 0;
    
    virtual void  Set( uint8 mask ) = 0;
    
    virtual void  Reset( uint8 mask ) = 0;
    
    virtual void  Out( uint8 value ) = 0;
  };

//! Группа из шестнадцати выводов (шестнадцатибитная шина)
class CmtPinGroup16 {
  public:
    
    virtual void   SetDirIn( uint16 dirIn = 0xff ) = 0;
    
    virtual uint16 DirIn() const = 0;
    
    virtual uint16 In() const = 0;
    
    virtual void   Set( uint16 mask ) = 0;
    
    virtual void   Reset( uint16 mask ) = 0;
    
    virtual void   Out( uint16 value ) = 0;
  };

//! Группа из тридцати двух выводов (тридцатидвухбитная шина)
class CmtPinGroup32 {
  public:
    
    virtual void   SetDirIn( uint32 dirIn = 0xff ) = 0;
    
    virtual uint32 DirIn() const = 0;
    
    virtual uint32 In() const = 0;
    
    virtual void   Set( uint32 mask ) = 0;
    
    virtual void   Reset( uint32 mask ) = 0;
    
    virtual void   Out( uint32 value ) = 0;
  };

