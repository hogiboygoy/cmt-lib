/*
������ "MeLViN" 32-������ �� C++
���� mvnFsFat32.cpp
��������
  ���������� �� �� ������ FAT32
�������
  1.11.2008 ������
*/

struct CmtFat32 : public CmtFat {
  CmtFat32( CmtBlockDevice *disk, uint32 _fatSize, uint32 lbaSize, uint32 spc, uint32 fatNum, uint32 lbaStart, uint32 rootStart, uint32 numClus ) :
    CmtFat( disk, _fatSize, lbaSize, spc, fatNum, lbaStart, numClus ) {
        //��������� ������ ��������� �������� ��� FAT16
      firstRoot = rootStart;
      //��������� ������ �������� ������
      firstData = ptLBAStart + fatNumber * fatSize;
      }

          void           NeedCluster32( uint32 offset );
  virtual uint32         GetCluster( uint32 cluster );
  virtual void           SetCluster( uint32 cluster, uint32 value );
  virtual FatDirEntry1x* GetNextFileEntry( CmtFatFinder *lpFinder );
  virtual void           DeleteFileEntry( FatDirEntry1x *ptr );
  virtual int32          IncreaseDir( uint32 startDir );
  };

int32
CmtFat32::IncreaseDir( uint32 startDir ) {
  if( startDir == 0 ) startDir = firstRoot;
  return CmtFat::IncreaseDir( startDir );
  }

void
CmtFat32::NeedCluster32( uint32 cluster ) {
  cluster >>= 7; //����� ���������� ������� FAT
  if( cluster != curFatSector ) {
    //��������� �������� ������ �������
    FlushFat();
    //��������� ������ ������
    mDisk->Read( (uint32*)fatSector, cluster + ptLBAStart, 1 );
    //���������� ������� ����� ������� FAT
    curFatSector = cluster;
    }
  }

uint32
CmtFat32::GetCluster( uint32 cluster ) {
  //���������� ���������� ������ �������� � ������ ������� fat
  NeedCluster32( cluster );
  //cluster = ((uint32*)(fatSector))[cluster & 0x7f];
  cluster = *((uint32*)( fatSector + ((cluster & 0x7f) << 2) ));
  if( cluster >= clusterPerPart ) cluster = END_CLUSTER;
  //if( (cluster & 0xfffffff) == 0xfffffff ) cluster = END_CLUSTER;
  return cluster;
  }

void
CmtFat32::SetCluster( uint32 cluster, uint32 value ) {
  //���������� ���������� ������ �������� � ������ ������� fat
  NeedCluster32( cluster );
  if( value == END_CLUSTER ) value = 0xfffffff;
  //�������� �������� �������
  ((uint32*)(fatSector))[cluster & 0x7f] = value;
  dirtyFatSector = 1;
  }


void
CmtFat32::DeleteFileEntry( FatDirEntry1x *ptr ) {
  DeleteFileEntryEx( ptr, ptr->GetFirstCluster32() );
  }

FatDirEntry1x*
CmtFat32::GetNextFileEntry( CmtFatFinder *lpFinder ) {
  if( lpFinder->subDirStart == 0 )
    lpFinder->subDirStart = firstRoot;
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

