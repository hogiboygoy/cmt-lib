//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
uint_8 
CmtPlisAltera::Config( CmtFile &src, CmtFile &dst, uint32 size ) {
  if( !src.Good() ) return CMTE_FS_NO_FILE;

  //������ �����
  Reset( true );
  //��������
  cmtWaitTick( 2 );
  Reset( false );
  cmtWaitTick( 2 );

  //��������� ����
  cmtParseBinCopy( src, dst, size );
  
  //������ �������������� �����
  CmtFile zero;
  cmtParseBinCopy( zero, dst, 255 );
  
  //���� ������ DONE �����������, �� �������� �������
  return Done() ? CMTE_OK : CMTE_FAIL;
  }

