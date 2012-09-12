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
    //������� ���������� ���������� ����������
    WaitTransfer(0);
    }
  mNext = true;
  
  //��������� �����
  __raw_clearl( MBIT7, DMA4_CCR(mDmaIndex) );
  
  //�������� ���� ���������� ����������
  __raw_writel( 0x1ffe, DMA4_CSR(mDmaIndex) );
  
  //����������������� ����� � ����������
  if( mSetSrc ) {
    //�������� ����� ���������
    __raw_writel( (uint32)adr, DMA4_CSSA(mDmaIndex) );
    }
  else {
    //�������� ����� ���������
    __raw_writel( (uint32)adr, DMA4_CDSA(mDmaIndex) );
    }
  __raw_writel( elemCount, DMA4_CEN(mDmaIndex) );  //���������� ��������� �� ������
  __raw_writel( frameCount, DMA4_CFN(mDmaIndex) ); //���������� �������
  
  //��� ������, ��������� �����
  __raw_setl( MBIT7, DMA4_CCR(mDmaIndex) );
  }

void
CmtDma::StopTransfer() {
  //�������� ���������� ������
  __raw_clearl( MBIT7, DMA4_CCR(mDmaIndex) );
  //������� ���������� �������� ������-������
  while( __raw_readl(DMA4_CCR(mDmaIndex)) & (MBIT10|MBIT9) );
  
  mNext = false;
  }

uint_8 
CmtDma::WaitTransfer( CmtEvent *pBreak ) {
  CmtEvSetBit32( (uint32*)(DMA4_CSR(mDmaIndex)), MBIT7 | MBIT5 ).Wait2( pBreak );
  return (pBreak && pBreak->IsSignaled()) ? CMTE_BREAK : CMTE_OK;
  }
