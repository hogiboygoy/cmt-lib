//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// ��������
//   Parser �����������
//==============================================================================
uint_8 
cmtParseBinCopyBuf( CmtFile &src, CmtFile &dst, uint32 size ) {
  uint8 buf[CMT_PARSE_BIN_COPY_BUF_SIZE];
  uint32 len;
  while( size ) {
    if( !src.Good() || !dst.Good() ) return CMTE_FAIL;
    len = size > CMT_PARSE_BIN_COPY_BUF_SIZE ? CMT_PARSE_BIN_COPY_BUF_SIZE : size;
    src.Read( buf, len );
    dst.Write( buf, len );
    size -= len;
    }
  return CMTE_OK;
  }


uint_8 
cmtParseBinCopy( CmtFile &src, CmtFile &dst, uint32 size ) {
  const void *binSour = src.GetRead( size );
  void *binDest = dst.GetWrite( size );
  if( binSour == 0 ) {
    if( binDest == 0 ) //�������� ������ ����������� ����� �����
      return cmtParseBinCopyBuf( src, dst, size );
    //�������� ������ ������ � �����
    src.Read( binDest, size );
    }
  else if( binDest == 0 ) {
    //�������� ������ ���������� �� ������
    dst.Write( binSour, size );
    }
  else //�������� ������ �����������
    memcpy( binDest, binSour, size );
  return CMTE_OK;
  }
