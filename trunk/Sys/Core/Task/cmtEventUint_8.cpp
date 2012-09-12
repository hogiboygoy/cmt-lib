//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//ru Событие проверки установки целого в не-ноль
uint_8 
cmtSignalSetUInt_8( void *ptr ) {
  return *((uint_8*)ptr) != 0;
  }

//ru Ожидание установки целого в не-ноль
void 
cmtWaitSetUInt_8( uint_8 *val ) {
  cmtWaitEvent( cmtSignalSetUInt_8, val );
  }

//ru Событие проверки установки целого в ноль
uint_8 
cmtSignalClearUInt_8( void *ptr ) {
  return *((uint_8*)ptr) == 0;
  }

//ru Ожидание установки целого в не-ноль
void 
cmtWaitClearUInt_8( uint_8 *val ) {
  cmtWaitEvent( cmtSignalClearUInt_8, val );
  }

//! Структура для сигнала установки бита
struct CmtBitSignal_8 {
  uint_8 *mVal;  //!< Указатель на переменную с битами
  uint_8  mMask; //!< Битовая маска проверяемых битов
  uint32  mTime; //!< Тайм-аут ожидания
  CmtBitSignal_8( uint_8 *val, uint_8 mask ) : mVal(val), mMask(mask) {}
  CmtBitSignal_8( uint_8 *val, uint_8 mask, uint32 out );
  };

//! Сигнал установки бита
uint_8
cmtSignalSetBit_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) & (((CmtBitSignal_8*)ptr)->mMask)) != 0;
  }

//! Сигнал очистки бита
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

//Сигнал равенства значения
uint_8
cmtSignalEq_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) == (((CmtBitSignal_8*)ptr)->mMask));
  }
  
//Сигнал неравенства значения
uint_8
cmtSignalNotEq_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) != (((CmtBitSignal_8*)ptr)->mMask));
  }
  
//Сигнал больше эталона
uint_8
cmtSignalGt_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) > (((CmtBitSignal_8*)ptr)->mMask));
  }
  
//Сигнал меньше эталона
uint_8
cmtSignalLt_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) < (((CmtBitSignal_8*)ptr)->mMask));
  }
  
//Сигнал больше или равно эталона
uint_8
cmtSignalGtEq_8( void *ptr ) {
  return ((*(((CmtBitSignal_8*)ptr)->mVal)) >= (((CmtBitSignal_8*)ptr)->mMask));
  }
  
//Сигнал меньше или равно эталона
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

//ru Регулирование доступа к ресурсам
void 
cmtLock_8( uint_8 *val ) {
  if( *val == 0 )
    //Заблокировано, ожидать освобождения
    cmtWaitEvent( cmtSignalSetUInt_8, val );
  //Свободно, заблокировать
  *val = 0;
  }

void 
cmtUnLock_8( uint_8 *val ) {
  //Освободить устройство
  *val = 1;
  }

