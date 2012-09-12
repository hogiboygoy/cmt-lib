//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif

CmtTaskProc cmtTaskAddProc;

void cmtTaskAdd32Asm();

void cmtPortSwitchTask();

void cmtJumpProcess();

#ifdef __cplusplus
}
#endif

void
cmtTaskAdd32( CmtTask *task, CmtTaskProc proc, void *ptr, void *stack ) {
  cmtTaskAppend( task );
  cmtNextTask = task;
  task->mStack = (uint32)stack;
  //Записать параметр задачи
  task->mEvent = ptr;
  task->mSignaled = cmtSignaled;
  cmtTaskAddProc = proc;
  cmtTaskAdd32Asm();
  }


void cmtWaitMKS( uint32 mks )
{
  mks *= CMT_CORE_CLK / 4000000;
  while( mks-- ) __asm( "         cmp r0,r0" );
}

