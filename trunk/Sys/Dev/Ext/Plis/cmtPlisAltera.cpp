//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
uint_8 
CmtPlisAltera::Config( CmtFile &src, CmtFile &dst, uint32 size ) {
  if( !src.Good() ) return CMTE_FS_NO_FILE;

  //Подать сброс
  Reset( true );
  //Ожидание
  cmtWaitTick( 2 );
  Reset( false );
  cmtWaitTick( 2 );

  //Загружаем ПЛИС
  cmtParseBinCopy( src, dst, size );
  
  //Подать дополнительные такты
  CmtFile zero;
  cmtParseBinCopy( zero, dst, 255 );
  
  //Если сигнал DONE установился, то загрузка успешна
  return Done() ? CMTE_OK : CMTE_FAIL;
  }

