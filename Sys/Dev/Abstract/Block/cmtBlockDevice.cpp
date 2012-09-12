//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
CmtBlockDevice::CmtBlockDevice( uint32 blockSize, uint32 blockCount ) :
  CmtDev(),
  mBlockSize(blockSize),
  mBlockCount(blockCount),
  mStatus(CMTE_NOT_INIT),
  mBreak(0){}

