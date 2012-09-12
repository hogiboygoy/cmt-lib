//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
CmtTask *cmtCurTask;   //ru ”казатель на текущую задачу
CmtTask *cmtNextTask;  //ru ”казатель на следующую задачу

void
cmtInitTask() {
  static CmtTask rootTask;
  cmtCurTask = &rootTask;
  rootTask.mNext = cmtCurTask;
  rootTask.mSignaled = cmtUnSignaled;
  }

void
cmtTaskAppend( CmtTask *task ) {
  task->mNext = cmtCurTask->mNext;
  cmtCurTask->mNext = task;
  }

void
cmtTaskRemove( CmtTask *task ) {
  if( task->mNext == 0 || cmtCurTask == task ) return;
  CmtTask *ptr = task->mNext;
  while( ptr->mNext != task ) ptr = ptr->mNext;
  ptr->mNext = task->mNext;
  task->mNext = 0;
  }

void
cmtTaskAddStep( CmtTask *task, CmtSignaled oneStep ) {
  cmtTaskAppend( task );
  task->mSignaled = oneStep;
  }

//#pragma optimize=z none
void
cmtWaitEvent( CmtSignaled signaled, void *ev ) {
  cmtCurTask->mEvent = ev;
  cmtCurTask->mSignaled = signaled;
  cmtNextTask = cmtCurTask->mNext;
  while( !cmtNextTask->mSignaled( cmtNextTask->mEvent ) )
    cmtNextTask = cmtNextTask->mNext;
  if( cmtNextTask != cmtCurTask )
    cmtPortSwitchTask();
  }

uint_8
cmtSignaled( void *ptr ) {
  return 1;
  }

uint_8
cmtUnSignaled( void *ptr ) {
  return 1;
  }
