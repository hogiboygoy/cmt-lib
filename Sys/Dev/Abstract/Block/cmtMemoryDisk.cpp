//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

CmtMemoryDisk::CmtMemoryDisk( uint32 blockSize, uint32 blockCount, const uint8 *srcImage, uint32 srcImageSize ) :
  CmtBlockDevice( blockSize, blockCount ) {
  //Размер памяти для диска
  uint32 size = blockSize * blockCount;
  //Выделить память для диска
  mDiskImage = new uint32[size>>2];
  if( mDiskImage ) {
    //Скопировать исходную информацию, если есть
    if( srcImage && srcImageSize <= size ) 
      memcpy( mDiskImage, srcImage, srcImageSize );
    mStatus = CMTE_OK;
    }
  }

uint_8
CmtMemoryDisk::Read( uint32 *dest, uint32 block, uint32 count ) {
  if( block + count <= mBlockCount ) {
    memcpy( dest, mDiskImage + ((block * mBlockSize) >> 2), (mBlockSize * count) );
    return CMTE_OK;
    }
  return CMTE_FAIL;
  }

uint_8
CmtMemoryDisk::Write( const uint32 *src, uint32 block, uint32 count ) {
  if( block + count <= mBlockCount ) {
    memcpy( mDiskImage + ((block * mBlockSize) >> 2), src, (mBlockSize * count) );
    return CMTE_OK;
    }
  return CMTE_FAIL;
  }
