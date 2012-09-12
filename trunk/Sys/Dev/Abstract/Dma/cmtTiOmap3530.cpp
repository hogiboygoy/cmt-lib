//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

CmtDma::CmtDma( uint32 index, bool setSrc ) :
  CmtDmaBase(),
  mDmaIndex(index),
  mSetSrc(setSrc),
  mNext(false) {
  
  }

void   
CmtDma::InitTransfer( void *adr, uint32 elemCount, uint32 frameCount ) {
  if( mNext ) {
    //Ожидать завершения предыдущей транзакции
    WaitTransfer(0);
    }
  mNext = true;
  
  //Запретить канал
  __raw_clearl( MBIT7, DMA4_CCR(mDmaIndex) );
  
  //Сбросить флаг завершения транзакции
  __raw_writel( 0x1ffe, DMA4_CSR(mDmaIndex) );
  
  //Запрограммировать адрес и количество
  if( mSetSrc ) {
    //Задается адрес источника
    __raw_writel( (uint32)adr, DMA4_CSSA(mDmaIndex) );
    }
  else {
    //Задается адрес приемника
    __raw_writel( (uint32)adr, DMA4_CDSA(mDmaIndex) );
    }
  __raw_writel( elemCount, DMA4_CEN(mDmaIndex) );  //Количество элементов во фрейме
  __raw_writel( frameCount, DMA4_CFN(mDmaIndex) ); //Количество фреймов
  
  //Все готово, разрешить обмен
  __raw_setl( MBIT7, DMA4_CCR(mDmaIndex) );
  }

void
CmtDma::StopTransfer() {
  //Сбросить разрешение канала
  __raw_clearl( MBIT7, DMA4_CCR(mDmaIndex) );
  //Ожидать завершения операций чтения-записи
  while( __raw_readl(DMA4_CCR(mDmaIndex)) & (MBIT10|MBIT9) );
  
  mNext = false;
  }

uint_8 
CmtDma::WaitTransfer( CmtEvent *pBreak ) {
  CmtEvSetBit32( (uint32*)(DMA4_CSR(mDmaIndex)), MBIT7 | MBIT5 ).Wait2( pBreak );
  return (pBreak && pBreak->IsSignaled()) ? CMTE_BREAK : CMTE_OK;
  }
