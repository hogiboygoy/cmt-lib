/*
Проект "MeLViN" 32-битная ОС C++
Файл mvnFsFat32.cpp
Описание
  Реализация ФС на основе FAT32
История
  1.11.2008 создан
*/

struct CmtFat32 : public CmtFat {
  CmtFat32( CmtBlockDevice *disk, uint32 _fatSize, uint32 lbaSize, uint32 spc, uint32 fatNum, uint32 lbaStart, uint32 rootStart, uint32 numClus ) :
    CmtFat( disk, _fatSize, lbaSize, spc, fatNum, lbaStart, numClus ) {
        //Вычислить начало корневого каталога для FAT16
      firstRoot = rootStart;
      //Вычислить начало секторов данных
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
  cluster >>= 7; //Номер требуемого сектора FAT
  if( cluster != curFatSector ) {
    //Требуется загрузка нового сектора
    FlushFat();
    //Прочитать нужный сектор
    mDisk->Read( (uint32*)fatSector, cluster + ptLBAStart, 1 );
    //Установить текущий номер сектора FAT
    curFatSector = cluster;
    }
  }

uint32
CmtFat32::GetCluster( uint32 cluster ) {
  //Обеспечить нахождение номера кластера в буфере сектора fat
  NeedCluster32( cluster );
  //cluster = ((uint32*)(fatSector))[cluster & 0x7f];
  cluster = *((uint32*)( fatSector + ((cluster & 0x7f) << 2) ));
  if( cluster >= clusterPerPart ) cluster = END_CLUSTER;
  //if( (cluster & 0xfffffff) == 0xfffffff ) cluster = END_CLUSTER;
  return cluster;
  }

void
CmtFat32::SetCluster( uint32 cluster, uint32 value ) {
  //Обеспечить нахождение номера кластера в буфере сектора fat
  NeedCluster32( cluster );
  if( value == END_CLUSTER ) value = 0xfffffff;
  //Получить значение индекса
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
  //Выбираем из поддиректория
  if( lpFinder->recCount >= 16 ) {
    //Добрались до последней записи в секторе
    lpFinder->recCount = 0;
    lpFinder->sector++;
    if( lpFinder->sector >= sectorPerCluster ) {
      //Добрались до последнего сектора в кластере
      //Переходим к следующему кластеру
      lpFinder->subDirStart = GetCluster( lpFinder->subDirStart );
      lpFinder->sector = 0;
      if( lpFinder->subDirStart == END_CLUSTER ||
          lpFinder->subDirStart == 0 ) return 0; //Не нашли
      }
    }
  //Получить очередную запись
  return GetDirRecord( SectorFromCluster(lpFinder->subDirStart) + lpFinder->sector, lpFinder->recCount++ );
  }

