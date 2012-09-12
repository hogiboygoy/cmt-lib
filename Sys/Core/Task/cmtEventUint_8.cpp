//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//ru ������� �������� ��������� ������ � ��-����
uint_8 
cmtSignalSetUInt_8( void *ptr ) {
  return *((uint_8*)ptr) != 0;
  }

//ru �������� ��������� ������ � ��-����
void 
cmtWaitSetUInt_8( uint_8 *val ) {
  cmtWaitEvent( cmtSignalSetUInt_8, val );
  }

//ru ������� �������� ��������� ������ � ����
uint_8 
cmtSignalClearUInt_8( void *ptr ) {
  return *((uint_8*)ptr) == 0;
  }

//ru �������� ��������� ������ � ��-����
void 
cmtWaitClearUInt_8( uint_8 *val ) {
  cmtWaitEvent( cmtSignalClearUInt_8, val );
  }

//! ��������� ��� ������� ��������� ����
struct CmtBitSignal_8 {
  uint_8 *mVal;  //!< ��������� �� ���������� � ������
  uint_8  mMask; //!< ������� ����� ����������� �����
  uint32  mTime; //!< ����-��� ��������
  CmtBitSignal_8( uint_8 *val, uint_8 mask ) : mVal(val), mMask(mask) {}
  CmtBitSignal_8( uint_8 *val, uint_8 mask, uint32 out );
  };

//! ������ ��������� ����
uint_8
cmtSignalSetBit_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) & (((CmtBitSignal_8*)ptr)->mMask)) != 0;
  }

//! ������ ������� ����
uint_8
cmtSignalClearBit_8( void *ptr ) {
  return (~(*(((CmtBitSignal_8*)ptr)->mVal)) & (((CmtBitSignal_8*)ptr)->mMask)) != 0;
  }

uint32
cmtWaitSetBit_8( uint_8 *val, uint_8 mask ) {
  if( (*val & mask) == 0 ) {
    CmtBitSignal_8 signal( val, mask );
    cmtWaitEvent( cmtSignalSetBit_8, &signal );
    }
  return *val & mask;
  }

uint32
cmtWaitClearBit_8( uint_8 *val, uint_8 mask ) {
  if( (~*val & mask) == 0 ) {
    CmtBitSignal_8 signal( val, mask );
    cmtWaitEvent( cmtSignalClearBit_8, &signal );
    }
  return (~(*val)) & mask;
  }

//������ ��������� ��������
uint_8
cmtSignalEq_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) == (((CmtBitSignal_8*)ptr)->mMask));
  }
  
//������ ����������� ��������
uint_8
cmtSignalNotEq_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) != (((CmtBitSignal_8*)ptr)->mMask));
  }
  
//������ ������ �������
uint_8
cmtSignalGt_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) > (((CmtBitSignal_8*)ptr)->mMask));
  }
  
//������ ������ �������
uint_8
cmtSignalLt_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) < (((CmtBitSignal_8*)ptr)->mMask));
  }
  
//������ ������ ��� ����� �������
uint_8
cmtSignalGtEq_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) >= (((CmtBitSignal_8*)ptr)->mMask));
  }
  
//������ ������ ��� ����� �������
uint_8
cmtSignalLtEq_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) <= (((CmtBitSignal_8*)ptr)->mMask));
  }

void
cmtWaitEq_8( uint_8 *ptr, uint_8 val ) {
  if( *ptr != val ) {
    CmtBitSignal_8 signal( ptr, val );
    cmtWaitEvent( cmtSignalEq_8, &signal );
    }
  }

void
cmtWaitNotEq_8( uint_8 *ptr, uint_8 val ) {
  if( *ptr == val ) {
    CmtBitSignal_8 signal( ptr, val );
    cmtWaitEvent( cmtSignalNotEq_8, &signal );
    }
  }

void
cmtWaitGt_8( uint_8 *ptr, uint_8 val ) {
  if( *ptr <= val ) {
    CmtBitSignal_8 signal( ptr, val );
    cmtWaitEvent( cmtSignalGt_8, &signal );
    }
  }

void
cmtWaitLt_8( uint_8 *ptr, uint_8 val ) {
  if( *ptr >= val ) {
    CmtBitSignal_8 signal( ptr, val );
    cmtWaitEvent( cmtSignalLt_8, &signal );
    }
  }

void
cmtWaitGtEq_8( uint_8 *ptr, uint_8 val ) {
  if( *ptr < val ) {
    CmtBitSignal_8 signal( ptr, val );
    cmtWaitEvent( cmtSignalGtEq_8, &signal );
    }
  }

void
cmtWaitLtEq_8( uint_8 *ptr, uint_8 val ) {
  if( *ptr > val ) {
    CmtBitSignal_8 signal( ptr, val );
    cmtWaitEvent( cmtSignalLtEq_8, &signal );
    }
  }

//ru ������������� ������� � ��������
void 
cmtLock_8( uint_8 *val ) {
  if( *val == 0 )
    //�������������, ������� ������������
    cmtWaitEvent( cmtSignalSetUInt_8, val );
  //��������, �������������
  *val = 0;
  }

void 
cmtUnLock_8( uint_8 *val ) {
  //���������� ����������
  *val = 1;
  }

