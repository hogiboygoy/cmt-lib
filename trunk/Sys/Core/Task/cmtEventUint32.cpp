//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
//ru ������� �������� ��������� ������ � ��-����
uint_8 
cmtSignalSetUInt32( void *ptr ) {
  return *((uint32*)ptr) != 0;
  }

//ru �������� ��������� ������ � ��-����
void 
cmtWaitSetUInt32( uint32 *val ) {
  if( *val == 0 )
    cmtWaitEvent( cmtSignalSetUInt32, val );
  }

//ru ������� �������� ��������� ������ � ����
uint_8 
cmtSignalClearUInt32( void *ptr ) {
  return *((uint32*)ptr) == 0;
  }

//ru �������� ��������� ������ � ��-����
void 
cmtWaitClearUInt32( uint32 *val ) {
  if( *val != 0 )
    cmtWaitEvent( cmtSignalClearUInt32, val );
  }

//! ��������� ��� ������� ��������� ����
struct CmtBitSignal {
  uint32 *mVal;  //!< ��������� �� ���������� � ������
  uint32  mMask; //!< ������� ����� ����������� �����
  uint32  mTime; //!< ����-��� ��������
  CmtBitSignal( uint32 *val, uint32 mask ) : mVal(val), mMask(mask) {}
  CmtBitSignal( uint32 *val, uint32 mask, uint32 out );
  };

//! ������ ��������� ����
uint_8
cmtSignalSetBit( void *ptr ) {
  return ((*(((CmtBitSignal*)ptr)->mVal)) & (((CmtBitSignal*)ptr)->mMask)) != 0;
  }

//! ������ ������� ����
uint_8
cmtSignalClearBit( void *ptr ) {
  return (~(*(((CmtBitSignal*)ptr)->mVal)) & (((CmtBitSignal*)ptr)->mMask)) != 0;
  }

uint32
cmtWaitSetBit( uint32 *val, uint32 mask ) {
  if( (*val & mask) == 0 ) {
    CmtBitSignal signal( val, mask );
    cmtWaitEvent( cmtSignalSetBit, &signal );
    }
  return *val & mask;
  }

uint32
cmtWaitClearBit( uint32 *val, uint32 mask ) {
  if( (~*val & mask) == 0 ) {
    CmtBitSignal signal( val, mask );
    cmtWaitEvent( cmtSignalClearBit, &signal );
    }
  return (~(*val)) & mask;
  }

//������ ��������� ��������
uint_8
cmtSignalEq( void *ptr ) {
  return ((*(((CmtBitSignal*)ptr)->mVal)) == (((CmtBitSignal*)ptr)->mMask));
  }
  
//������ ����������� ��������
uint_8
cmtSignalNotEq( void *ptr ) {
  return ((*(((CmtBitSignal*)ptr)->mVal)) != (((CmtBitSignal*)ptr)->mMask));
  }
  
//������ ������ �������
uint_8
cmtSignalGt( void *ptr ) {
  return ((*(((CmtBitSignal*)ptr)->mVal)) > (((CmtBitSignal*)ptr)->mMask));
  }
  
//������ ������ �������
uint_8
cmtSignalLt( void *ptr ) {
  return ((*(((CmtBitSignal*)ptr)->mVal)) < (((CmtBitSignal*)ptr)->mMask));
  }
  
//������ ������ ��� ����� �������
uint_8
cmtSignalGtEq( void *ptr ) {
  return ((*(((CmtBitSignal*)ptr)->mVal)) >= (((CmtBitSignal*)ptr)->mMask));
  }
  
//������ ������ ��� ����� �������
uint_8
cmtSignalLtEq( void *ptr ) {
  return ((*(((CmtBitSignal*)ptr)->mVal)) <= (((CmtBitSignal*)ptr)->mMask));
  }

void
cmtWaitEq( uint32 *ptr, uint32 val ) {
  if( *ptr != val ) {
    CmtBitSignal signal( ptr, val );
    cmtWaitEvent( cmtSignalEq, &signal );
    }
  }

void
cmtWaitNotEq( uint32 *ptr, uint32 val ) {
  if( *ptr == val ) {
    CmtBitSignal signal( ptr, val );
    cmtWaitEvent( cmtSignalNotEq, &signal );
    }
  }

void
cmtWaitGt( uint32 *ptr, uint32 val ) {
  if( *ptr <= val ) {
    CmtBitSignal signal( ptr, val );
    cmtWaitEvent( cmtSignalGt, &signal );
    }
  }

void
cmtWaitLt( uint32 *ptr, uint32 val ) {
  if( *ptr >= val ) {
    CmtBitSignal signal( ptr, val );
    cmtWaitEvent( cmtSignalLt, &signal );
    }
  }

void
cmtWaitGtEq( uint32 *ptr, uint32 val ) {
  if( *ptr < val ) {
    CmtBitSignal signal( ptr, val );
    cmtWaitEvent( cmtSignalGtEq, &signal );
    }
  }

void
cmtWaitLtEq( uint32 *ptr, uint32 val ) {
  if( *ptr > val ) {
    CmtBitSignal signal( ptr, val );
    cmtWaitEvent( cmtSignalLtEq, &signal );
    }
  }

//ru ������������� ������� � ��������
void 
cmtLock( uint32 *val ) {
  if( *val == 0 )
    //�������������, ������� ������������
    cmtWaitEvent( cmtSignalSetUInt32, val );
  //��������, �������������
  *val = 0;
  }

void 
cmtUnLock( uint32 *val ) {
  //���������� ����������
  *val = 1;
  }

struct CmtBreakBitSignal {
  uint32  mAddr;  //!< ����� ��� ��������
  uint32  mMask;  //!< ����� ����� ��� ��������
  uint32 *mBreak; //!< ���������� �������� ����������� ��������
  
  CmtBreakBitSignal( uint32 addr, uint32 mask, uint32 *brk ) : mAddr(addr), mMask(mask), mBreak(brk) {}
  };

//�������� ������� ����������
uint_8
cmtSetBreakBitSignal( void *ptr ) {
  if( ((CmtBreakBitSignal*)ptr)->mBreak && *((CmtBreakBitSignal*)ptr)->mBreak == 0 ) return 1;
  return ((*(volatile unsigned int *)(((CmtBreakBitSignal*)ptr)->mAddr)) & ((CmtBreakBitSignal*)ptr)->mMask) != 0;
  }

uint_8
cmtClearBreakBitSignal( void *ptr ) {
  if( ((CmtBreakBitSignal*)ptr)->mBreak && *((CmtBreakBitSignal*)ptr)->mBreak == 0 ) return 1;
  return ((~(*(volatile unsigned int *)(((CmtBreakBitSignal*)ptr)->mAddr))) & ((CmtBreakBitSignal*)ptr)->mMask) != 0;
  }


uint32 
cmtWaitBreakSetBit( uint32 adr, uint32 mask, uint32 *pBreak ) {
  CmtBreakBitSignal signal( adr, mask, pBreak );
  if( !cmtSetBreakBitSignal( &signal ) )
    cmtWaitEvent( cmtSetBreakBitSignal, &signal );
  return (*(volatile unsigned int *)(adr)) & mask;
  }

uint32 
cmtWaitBreakClearBit( uint32 adr, uint32 mask, uint32 *pBreak ) {
  CmtBreakBitSignal signal( adr, mask, pBreak );
  if( !cmtClearBreakBitSignal( &signal ) )
    cmtWaitEvent( cmtClearBreakBitSignal, &signal );
  return (~(*(volatile unsigned int *)(adr))) & mask;
  }

