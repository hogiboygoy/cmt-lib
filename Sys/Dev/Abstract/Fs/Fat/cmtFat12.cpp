/*
������ "MeLViN" 32-������ �� C++
���� mvnFsFat12.cpp
��������
  ���������� �� �� ������ FAT12
�������
  18.09.2008 ������
*/
struct CmtFat12 : public CmtFat {
  uint32         numRootElem;      //���������� ��������� � �������� ���������� FAT12,16


  CmtFat12( CmtBlockDevice *disk, uint32 _fatSize, uint32 lbaSize, uint32 spc, uint32 fatNum, uint32 lbaStart, uint32 numRoot, uint32 numClus ) :
    CmtFat( disk, _fatSize, lbaSize, spc, fatNum, lbaStart, numClus ),
    numRootElem( numRoot ) {
        //��������� ������ ��������� �������� ��� FAT16
      firstRoot = ptLBAStart + fatNumber * fatSize;
      //��������� ������ �������� ������
      firstData = firstRoot + (numRootElem >> 4);
      }

          void           NeedCluster12( uint32 cluster );
  virtual uint32         GetCluster( uint32 cluster );
  virtual void           SetCluster( uint32 cluster, uint32 value );
  virtual FatDirEntry1x* GetNextFileEntry( CmtFatFinder *lpFinder );
  virtual int32          IncreaseDir( uint32 startDir );
  };
  
int32
CmtFat12::IncreaseDir( uint32 startDir ) {
  if( startDir == 0 || startDir == END_CLUSTER ) return CMTE_FS_DISK_FULL;
  return CmtFat::IncreaseDir( startDir );
  }

void
CmtFat12::NeedCluster12( uint32 cluster ) {
  cluster /= 341; //����� ���������� ������� FAT
  if( cluster != curFatSector ) {
    //��������� �������� ������ �������
    FlushFat();
    //��������� ������ ������
    mDisk->Read( (uint32*)fatSector, cluster + ptLBAStart, 1 );
    //���������� ������� ����� ������� FAT
    curFatSector = cluster;
    }
  }

#define END_CLUSTER12 0xfff
uint32
CmtFat12::GetCluster( uint32 cluster ) {
  //���������� ���������� ������ �������� � ������ ������� fat
  NeedCluster12( cluster );
  cluster %= 341; //����� �������� ������ �������
  //�������� � �����
  uint32 bits = cluster * 12;
  //�������� � ������
  uint32 offset = bits >> 3;
  cluster = fatSector[ offset & 0x1ff ];
  offset++;
  cluster |= fatSector[ offset & 0x1ff ] << 8;
  cluster >>= bits & 0x4;
  cluster &= 0xfff;
  if( cluster == END_CLUSTER12 ) cluster = END_CLUSTER;
  return cluster;
  }

void
CmtFat12::SetCluster( uint32 cluster, uint32 value ) {
  //���������� ���������� ������ �������� � ������ ������� fat
  NeedCluster12( cluster );
  cluster %= 341; //����� �������� ������ �������
  //�������� � �����
  uint32 bits = cluster * 12;
  //�������� � ������
  uint32 offset = bits >> 3;
  value &= 0xfff;
  value <<= bits & 0x4;
  if( bits & 0x4 ) {
    fatSector[ offset & 0x1ff ] &= 0xf;
    fatSector[ offset & 0x1ff ] |= (uint8)value;
    }
  else {
    fatSector[ offset & 0x1ff ] = (uint8)value;
    }

  offset++;
  value >>= 8;
  if( bits & 0x4 ) {
    fatSector[ offset & 0x1ff ] = (uint8)(value >> 8);
    }
  else {
    fatSector[ offset & 0x1ff ] &= 0xf0;
    fatSector[ offset & 0x1ff ] |= (uint8)value;
    }
  dirtyFatSector = 1;
  }

FatDirEntry1x*
CmtFat12::GetNextFileEntry( CmtFatFinder *lpFinder ) {
  if( lpFinder->subDirStart == 0 ) {
    uint32 sector = lpFinder->rootCount >> 4;
    //�������� �� ��������� ����������
    if( lpFinder->rootCount >= numRootElem ) return 0;
    //�������� ��������� ������
    return GetDirRecord( firstRoot + sector, lpFinder->rootCount++ );
    }
  //�������� �� �������������
  if( lpFinder->recCount >= 16 ) {
    //��������� �� ��������� ������ � �������
    lpFinder->recCount = 0;
    lpFinder->sector++;
    if( lpFinder->sector >= sectorPerCluster ) {
      //��������� �� ���������� ������� � ��������
      //��������� � ���������� ��������
      lpFinder->subDirStart = GetCluster( lpFinder->subDirStart );
      lpFinder->sector = 0;
      if( lpFinder->subDirStart == END_CLUSTER ||
          lpFinder->subDirStart == 0 ) return 0; //�� �����
      }
    }
  //�������� ��������� ������
  return GetDirRecord( SectorFromCluster(lpFinder->subDirStart) + lpFinder->sector, lpFinder->recCount++ );
  }


