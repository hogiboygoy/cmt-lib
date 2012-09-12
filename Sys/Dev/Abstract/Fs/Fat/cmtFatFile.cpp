//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
CmtFatFile::CmtFatFile( CmtFat *fs, FatDirEntry1x *ptr, uint32 mode ) {
  msMode = mode;
  mFileSize = ptr->mFileSize;        //������ �����
  mDirSector = fs->curDirSector;     //������ ����������
  mFilePosition = 0;  //������� ������� ��������� �����
  mFirstCluster =
  mFileCluster = ptr->GetFirstCluster32();    //������� ����� �������� �����
  mDirOffset = ((uint8*)ptr) - fs->dirSector; //�������� � ������� ���������� ������� ����������
  mFileSector = 0;    //����� ������� � ��������
  mFileOffset = msMode & CMT_CREATE_READ ? 512 : 0;  //�������� ��������� ������ ������ ������� (����)
  mFat = fs;
#ifndef CMT_FS_NO_WRITE
  //����������� ������ �����������
  if( (msMode & CMT_CREATE_WRITE) && mFileSize ) {
    //����� ���������� � ��� ��������� ������
    uint32 sectorCount = mFileSize >> 9;
    mFileOffset = mFileSize & 0x1ff;
    //��������� �� ������� ��������
    while( sectorCount > mFat->sectorPerCluster ) {
      mFileCluster = mFat->GetCluster( mFileCluster );
      sectorCount -= mFat->sectorPerCluster;
      }
    if( (sectorCount == mFat->sectorPerCluster) && mFileOffset ) {
      //��� ���� �������
      mFileCluster = mFat->GetCluster( mFileCluster );
      sectorCount -= mFat->sectorPerCluster;
      }
    //��������� �� ������� �������
    mFileSector = sectorCount;
    //������� ���������� �������
    if( mFileOffset ) {
      mFat->mDisk->Read( (uint32*)buffer, mFileSector + mFat->SectorFromCluster( mFileCluster ), 1 );
      }
    mFilePosition = mFileSize;
    }
#endif
  CADDREF(mFat);
  }

CmtFatFile::~CmtFatFile() {
  //�������� ����� �� ����
  Flush();
  //���������� �������� �������
  CDECREF(mFat);
  }

uint32
CmtFatFile::ReadFileSectorInc() {
  //��������� ������������� ������ ������ ��������
  if( mFileSector >= mFat->sectorPerCluster ) {
    mFileCluster = mFat->GetCluster( mFileCluster );
    if( mFileCluster == 0 || mFileCluster == END_CLUSTER )
      return CMTE_FS_STRUCT;
    mFileSector = 0;
    }
  uint32 res = mFat->mDisk->Read( (uint32*)buffer, mFileSector + mFat->SectorFromCluster( mFileCluster ), 1 );
  mFileSector ++;
  return res;
  }

uint32
CmtFatFile::CheckWriteSector() {
#ifndef CMT_FS_NO_WRITE
  if( mFileSector >= mFat->sectorPerCluster ) {
    //����� �� ���������� ������� ��������, �������� ��������� �������
    uint32 cluster = mFileCluster;
    mFileCluster = mFat->GetFreeCluster();
    if( mFileCluster > 0 && mFileCluster != END_CLUSTER ) {
      mFat->SetCluster( mFileCluster, END_CLUSTER );
      mFat->SetCluster( cluster, mFileCluster );
      mFileSector = 0;
      }
    else return CMTE_FS_DISK_FULL;
    }
#endif
  return CMTE_OK;
  }


uint32
CmtFatFile::WriteFileSectorInc() {
  uint32 res = CheckWriteSector();
#ifndef CMT_FS_NO_WRITE
  if( res != CMTE_OK ) return res;
  res = mFat->mDisk->Write( (uint32*)buffer, mFileSector + mFat->SectorFromCluster( mFileCluster ), 1 );
  mFileSector ++;
#endif
  return res;
  }

uint32
CmtFatFile::Seek( int32 offset, uint32 msFrom ) { //��������� ��������� ��������� �����
  uint32 target;
  uint32 bpc = mFat->sectorPerCluster * 512;
  if( msMode != CMT_CREATE_READ ) return CMTE_NOT_RELEASED;
  if( msFrom == CMT_SEEK_BEGIN ) {
    //�� ������
    target = (uint32)offset;
    }
  else if( msFrom == CMT_SEEK_CURRENT ) {
    //�� ������� �������
    target = mFilePosition + offset;
    }
  else if( msFrom == CMT_SEEK_END ) {
    //�� �������� �������
    target = mFileSize + offset;
    }
  else return CMTE_BAD_PARAM;
  //��������� �������
  if( target > mFileSize ) target = mFileSize;
  //��������� ���������� ��������
  if( target == mFilePosition ) return CMTE_OK;
  //��������� ������
  mFileSector = (target >> 9) % (mFat->sectorPerCluster);
  //��������� ������������� ��������
  if( (target >= ((mFilePosition / bpc) * bpc)) && ( target < ((mFilePosition / bpc + 1) * bpc )) ) {
    //� ������� ��������
    }
  else {
    //� ����� ������� �������, ��������
    uint32 cluster = mFileCluster;
    uint32 c = mFilePosition / bpc;
    if( (target < ((mFilePosition / bpc) * bpc)) ) {
      //� ����� ������ �������, ��������
      cluster = mFirstCluster;
      c = 0;
      }
    while( target > (c + 1) * bpc && cluster != END_CLUSTER ) {
      cluster = mFat->GetCluster( cluster );
      c++;
      }
    if( cluster == END_CLUSTER ) return CMTE_FS_FILE_STRUCT;
    mFileCluster = cluster;
    }
  mFilePosition = target;
  mFileOffset = target & 0x1ff;
  //��������� ������� ������ � �����
  uint32 res = ReadFileSectorInc();
  return res;
  }

uint32
CmtFatFile::Read( pvoid buf, uint32 size ) {
  if( size == 0 ) return CMTE_OK;
  uint8 *lpBuffer = (uint8*)buf;
  uint32 res = CMTE_OK;
  //��������� ����������� ������ �����
  if( size > mFileSize - mFilePosition )
    size = mFileSize - mFilePosition;
  //��������� �� ���-������
  uint32 len = size;
  if( len > 512 - mFileOffset )
    len = 512 - mFileOffset;
  if( len )
    memcpy( lpBuffer, buffer + mFileOffset, len );
  lpBuffer      += len;
  size          -= len;
  mFileOffset   += len;
  mFilePosition += len;
  if( size == 0 ) return CMTE_OK;
  //������ �������
  CLOCK(mFat);
  while( size && res == CMTE_OK ) {
    //����� ������ ��������� ������ � ����� �������� �� 32 ��������
    if( size >= 512 && ((((uint32)lpBuffer) & 0x3) == 0) ) {
      //������ �������
      uint32 count = size >> 9; //���������� ����� ��������
      if( mFileSector == mFat->sectorPerCluster ) {
        //��������� ����� ��������� �������
        mFileSector = 0;
        mFileCluster = mFat->GetCluster( mFileCluster );
        if( mFileCluster == 0 || mFileCluster == END_CLUSTER ) {
          res = CMTE_FS_FILE_STRUCT;
          break;
          }
        }
      if( count > mFat->sectorPerCluster - mFileSector )
        count = mFat->sectorPerCluster - mFileSector;
      //��������� �������
      res = mFat->mDisk->Read( (uint32*)lpBuffer, mFileSector + mFat->SectorFromCluster( mFileCluster ), count );
      mFileSector += count;
      count <<= 9; //���������� �������� ��������� � �����
      lpBuffer      += count;
      mFilePosition += count;
      size          -= count;
      }
    else {
      uint32 count = size > 512 ? 512 : size;
      res = ReadFileSectorInc();
      if( res != CMTE_OK ) break;
      mFileOffset    = count;
      memcpy( lpBuffer, buffer, count );
      lpBuffer      += count;
      mFilePosition += count;
      size          -= count;
      }
    }
  CUNLOCK(mFat);
  return res;
  }

uint32
CmtFatFile::Write( cpvoid buf, uint32 nNumber ) {
#ifndef CMT_FS_NO_WRITE
  FatDirEntry1x *ptr;
  uint8*  lpBuffer = (uint8*)buf;
  uint32  min = 512 - mFileOffset;
  uint32  res = CMTE_OK;
  if( mFileCluster == 0 || mFileCluster == END_CLUSTER )
    return CMTE_FS_DISK_FULL;
  CLOCK(mFat);
  if( min > nNumber ) {
    //����� ������ ������� ������� � ���
    memcpy( buffer + mFileOffset, lpBuffer, nNumber );
    mFileOffset   += nNumber;
    mFileSize     += nNumber;
    mFilePosition += nNumber;
    }
  else {
    //������� � ��� �� �������
    //�������� ��� �� �������
    if( mFileOffset ) {
      //���-�� ����, �������� �� ������
      memcpy( buffer + mFileOffset, lpBuffer, min );
      mFileSize     += min;
      mFilePosition += min;
      nNumber -= min;
      lpBuffer += min;
      //�������� � ����
      res = WriteFileSectorInc();
      mFileOffset = 0;
      }
    //�������� ����� �������
    while( nNumber && res == CMTE_OK ) {
      if( nNumber >= 512 && ((((uint32)lpBuffer) & 0x3) == 0) ) {
        //������ �������
        uint32 count = nNumber >> 9; //���������� ����� ��������
        res = CheckWriteSector();
        if( res != CMTE_OK ) break;
        if( count > mFat->sectorPerCluster - mFileSector )
          count = mFat->sectorPerCluster - mFileSector;
        res = mFat->mDisk->Write( (uint32*)lpBuffer, mFileSector + mFat->SectorFromCluster( mFileCluster ), count );
        mFileSector += count;
        count <<= 9; //���������� �������� ��������� � �����
        lpBuffer      += count;
        mFilePosition += count;
        mFileSize     += count;
        nNumber       -= count;
        }
      else {
        uint32 count = nNumber > 512 ? 512 : nNumber;
        //��������� ������� � ���
        memcpy( buffer, lpBuffer, count );
        mFileOffset   = count;
        mFileSize     += count;
        mFilePosition += count;
        nNumber -= count;
        if( mFileOffset == 512 ) {
          //�������� � ����
          res = WriteFileSectorInc();
          mFileOffset = 0;
          }
        }
      }
    }
  //�������� ������ � ����������
  mFat->NeedDirSector( mDirSector );
  ptr = (FatDirEntry1x*)(mFat->dirSector + mDirOffset);
  ptr->mFileSize = mFileSize;
  CmtSystemTime ft;
  ft.GetSystem();
  ptr->mUpdateTime = ft.FFTime();
  ptr->mUpdateDate = ft.FFDate();
  mFat->dirtyDirSector = 1;
  CUNLOCK(mFat);
  return res;
#else
  return CMTE_NOT_RELEASED;
#endif
  }

uint32
CmtFatFile::Flush() {
#ifndef CMT_FS_NO_WRITE
  if( msMode & CMT_CREATE_WRITE ) {
    CLOCK(mFat);
    //�������� ����� �� ����
    if( mFileOffset ) {
      WriteFileSectorInc();
      mFileSector --;
      }
    //�������� �� ���� FAT � ����������
    mFat->FlushFat();
    mFat->FlushDir();
    CUNLOCK(mFat);
    }
#endif
  return CMTE_OK;
  }

void
CmtFatFile::Clear() {
#ifndef CMT_FS_NO_WRITE
  if( msMode & CMT_CREATE_WRITE ) {
    CLOCK(mFat);
    //���������� ������� �����
    mFat->NeedDirSector( mDirSector );
    FatDirEntry1x *ptr = (FatDirEntry1x*)(mFat->dirSector + mDirOffset);
    ptr->mFileSize = 0;
    CmtSystemTime ft;
    ft.GetSystem();
    ptr->mUpdateTime = ft.FFTime();
    ptr->mUpdateDate = ft.FFDate();
    mFat->dirtyDirSector = 1;

    //������� ������� ���������
    mFat->FreeClusters( ptr->GetFirstCluster32() );

    ptr->SetFirstCluster32( mFat->GetFreeCluster() );
    if( ptr->GetFirstCluster32() > 0 )
      mFat->SetCluster( ptr->GetFirstCluster32(), END_CLUSTER );
    mFat->dirtyDirSector = 1;

    mFileSize = 0;        //������ �����
    mFilePosition = 0;  //������� ������� ��������� �����
    mFirstCluster =
    mFileCluster = ptr->GetFirstCluster32();   //������� ����� �������� �����
    mFileSector = 0;    //����� ������� � ��������
    mFileOffset = 0;    //�������� ��������� ������ ������ ������� (����)

    //�������� �� ���� FAT � ����������
    mFat->FlushFat();
    mFat->FlushDir();
    CUNLOCK(mFat);
    }
#endif
  }

char
CmtFatFile::Peek() {
  if( mFileSize == mFilePosition ) return 0xff;
  if( mFileOffset == 512 ) {
    //��������� � ���
    CLOCK(mFat);
    uint_8 res = ReadFileSectorInc();
    CUNLOCK(mFat);
    if( res != CMTE_OK ) return 0xff;
    }
  return buffer[mFileOffset];
  }

int32  
CmtFatFile::State() {
  if( (msMode & CMT_CREATE_READ) && (mFilePosition == mFileSize) ) 
    return CMTE_EOF;
  return CMTE_OK;
  }
