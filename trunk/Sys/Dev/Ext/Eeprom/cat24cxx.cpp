//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

uint_8
CmtCat24Cxx::WriteByte(uint32 addr, uint8 value) {
  uint8 tmp[2];
  uint_8 res;
  //Выполнить цикл обмена
  CLOCK(mI2c);
  mI2c->SetupMasterAddr( 0x50 | ((addr >> 8) & 0x7) );
  tmp[0] = addr & 0xff;
  tmp[1] = value;
  res = mI2c->WriteBlock( tmp, 2, 0 );
  CUNLOCK(mI2c);
  return res;
  }

uint8
CmtCat24Cxx::ReadByte(uint32 addr, uint_8 *state) {
  uint_8 res;
  //Выполнить цикл обмена
  CLOCK(mI2c);
  mI2c->SetupMasterAddr( 0x50 | ((addr >> 8) & 0x7) );
  addr &= 0xff;
  //Выполнить dummi Write операцию для установки текущего адреса
  mI2c->WriteBlock( &addr, 1, 0 );
  //Выполнить операцию чтения
  res = mI2c->ReadBlock( &addr, 1, 0 );
  CUNLOCK(mI2c);
  if( state ) *state = res;
  return addr & 0xff;
  }

uint_8
CmtCat24Cxx::WriteBlock(const uint8 *src, uint32 addr, uint32 count) {
  uint_8 res; //Результат операции
  //Первый блок
  int32 len = cmtIMin( 16 - (addr & 0xf), (int32)count );
  if( len == count ) {
    len++;
    uint8 tmp[17];
    tmp[0] = addr & 0xff;
    for( int32 i = 1; i < len; i++ )
      tmp[i] = *src++;
    //Он-же - единственный
    CLOCK(mI2c);
    mI2c->SetupMasterAddr( 0x50 | ((addr >> 8) & 0x7) );
    res = mI2c->WriteBlock( tmp, len, 0 );
    CUNLOCK(mI2c);
    cmtWaitTick(6);
    }
  else {
    //Выполнить блок
    do {
      res = WriteBlock( src, addr, len );
      src += len;
      addr += len;
      count -= len;
      len = cmtIMin( 16 - (addr & 0xf), (int32)count );
      }
    while( res == CMTE_OK && count );
    }
  return res;
  }

uint_8
CmtCat24Cxx::ReadBlock(uint8 *dst, uint32 addr, uint32 count) {
  //Выполнить цикл обмена
  CLOCK(mI2c);
  mI2c->SetupMasterAddr( 0x50 | ((addr >> 8) & 0x7) );
  addr &= 0xff;
  //Выполнить dummi Write операцию для установки текущего адреса
  mI2c->WriteBlock( &addr, 1, 0 );
  //Выполнить операцию чтения
  uint_8 res = mI2c->ReadBlock( dst, count, 0 );
  CUNLOCK(mI2c);
  return res;
  }
