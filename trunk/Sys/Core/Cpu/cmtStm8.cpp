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
  task->mStack = (CmtPortStackPtr)stack;
  //Записать параметр задачи
  task->mEvent = ptr;
  task->mSignaled = cmtSignaled;
  cmtTaskAddProc = proc;
  cmtTaskAdd32Asm();
  }


void 
cmtWaitMKS( uint32 mks ) {
  mks *= CMT_CORE_CLK / 4000000;
  while( mks-- ) __no_operation();
  }

#if CMT_CPU_CLK_MAP == 0
void
cmtInitSystem() {
  CLK->CKDIVR = 0x00; // Main freq divided by 1 = Full Freq
  }
#else
  #error "CMT: map not defined for this CPU"
#endif