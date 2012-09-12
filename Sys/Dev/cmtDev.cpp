//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#ifdef CMT_DEV_ARBITR //Enable device arbitration

CmtDev::CmtDev() : mRefCount(1), mSemaphore(CMT_DEV_FREE) { //!< �����������
}

void
CmtDev::AddRef() {
  mRefCount++;
  }

void
CmtDev::DecRef() {
  mRefCount--;
  if( mRefCount <= 0 ) delete this;
  }

uint_8
CmtDev::Lock() {
  if( mSemaphore == CMT_DEV_BUSY ) //��������� ��������� ����������
    cmtWaitSetUInt32( &mSemaphore ); //������� ������������ ����������
  
  if( mSemaphore == CMT_DEV_FREE ) { //���������� ��������, ������
    mSemaphore--;
    return 1;
    }
  
  //���������� �� ��������
  return 0;
  }
  
void  
CmtDev::UnLock() {
  mSemaphore++; //���������� ����������
  }
#else

CmtDev::CmtDev() { //!< �����������
  }
#endif
  
#if defined(CMT_SYS_TIMER_MAP) && defined(CMT_DEV_ARBITR) //Enable device arbitration
CmtDevPolling *CmtDevPolling::rootDev;  //�������� ���������� ������
CmtDevPolling *CmtDevPolling::nextPoll; //��������� ���������� ������ (��� �����)

void
CmtDevPolling::AddPoll() {
  mNext = rootDev;
  rootDev = this;
  }

void
CmtDevPolling::RemovePoll() {
  if( nextPoll == this ) nextPoll = mNext;
  for( CmtDevPollingPtr *ptr = &rootDev; *ptr; ptr = &((*ptr)->mNext) )
    if( *ptr == this ) {
      *ptr = mNext;
      break;
      }
  }

void
cmtPollingDeviceTask() {
  while(1) {
    cmtWaitTick( 100 ); //�������� ������ ��������� 0.1 ���
    for( CmtDevPolling *dev = CmtDevPolling::rootDev; dev != 0; dev = CmtDevPolling::nextPoll ) {
      CmtDevPolling::nextPoll = dev->mNext; //����������� ��������� ���������� ��� ������
      dev->AddRef(); //����������� �������� ���������� �� ����� ������
      dev->Poll();   //�������� ����������
      dev->DecRef(); //�������������� ����������, ��������� ��������
      }
    }
  }
#endif
