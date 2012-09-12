//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================


CmtScreenTiOmap3530::CmtScreenTiOmap3530(CmtPoint size, uint32 *src, uint32 *dublBuf) :
  CmtContextTiOmap3530( size, src ),
  mDublBuf(dublBuf) {}

void
CmtScreenTiOmap3530::FrameComplete() {
  if( mDublBuf ) {
    //���������� ����� ������
    //���������� ��� ����������� ������ ���������
    __raw_writel( (uint32)(GetData()), DISPC_GFX_BA0 );
    
    //��������� ��������� (��������� ������ ������ �����������)
    __raw_setl( DISPC_CONTROL_GOLCD, DISPC_CONTROL );
    
    uint32 *tmp = mData;
    mData = mDublBuf;
    mDublBuf = tmp;
    }
  }

void
CmtScreenTiOmap3530::SetMode( int mode ) {
  }

void 
CmtScreenTiOmap3530::WaitEndOfFrame() {
  //�������� ��� ����� ��������� ������������ ������
  __raw_writel( MBIT7, DISPC_IRQSTATUS );
  //������� ��������� ���� ��������� ������������ ������
  cmtWaitBreakSetBit( DISPC_IRQSTATUS, MBIT7, 0 );
  }
