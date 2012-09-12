//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
void     cmtInitHeap();
//MRESULT  mdsTestMemory();
//MRESULT  mdsTestMemoryEx( HEAPT *adr, HEAPT *val, HEAPT *pat );
pvoid    cmtMalloc( uint32 size );
pvoid    cmtCalloc( uint32 count, uint32 size );
void     cmtFree( pvoid ptr );
