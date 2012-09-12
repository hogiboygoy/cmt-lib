//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
uint_8
CmtMemoryBase::WriteBlock(const uint8 *src, uint32 addr, uint32 count) {
  uint_8 res;
  while( count-- ) {
    res = WriteByte( addr++, *src++ );
    if( res ) return res;
    }
  return CMTE_OK;
  }

uint_8
CmtMemoryBase::ReadBlock(uint8 *dst, uint32 addr, uint32 count) {
  uint_8 res;
  while( count-- ) {
    *dst++ = ReadByte( addr++, &res );
    if( res ) return res;
    }
  return CMTE_OK;
  }

