//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#ifdef CMT_DEV_ARBITR //Enable device arbitration

CmtDev::CmtDev() : mRefCount(1), mSemaphore(CMT_DEV_FREE) { //!< Конструктор
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
  if( mSemaphore == CMT_DEV_BUSY ) //Проверить занятость устройства
    cmtWaitSetUInt32( &mSemaphore ); //Ожидать освобождение устройства
  
  if( mSemaphore == CMT_DEV_FREE ) { //Устройство свободно, занять
    mSemaphore--;
    return 1;
    }
  
  //Устройство не доступно
  return 0;
  }
  
void  
CmtDev::UnLock() {
  mSemaphore++; //Освободить устройство
  }
#else

CmtDev::CmtDev() { //!< Конструктор
  }
#endif
  
#if defined(CMT_SYS_TIMER_MAP) && defined(CMT_DEV_ARBITR) //Enable device arbitration
CmtDevPolling *CmtDevPolling::rootDev;  //Корневое устройство опроса
CmtDevPolling *CmtDevPolling::nextPoll; //Следующее устройство опроса (при цикле)

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
    cmtWaitTick( 100 ); //Интервал опроса устройств 0.1 сек
    for( CmtDevPolling *dev = CmtDevPolling::rootDev; dev != 0; dev = CmtDevPolling::nextPoll ) {
      CmtDevPolling::nextPoll = dev->mNext; //Подготовить следующее устройство для опроса
      dev->AddRef(); //Блокировать удаление устройства на время опроса
      dev->Poll();   //Опросить устройство
      dev->DecRef(); //Разблокировать устройство, разрешить удаление
      }
    }
  }
#endif
