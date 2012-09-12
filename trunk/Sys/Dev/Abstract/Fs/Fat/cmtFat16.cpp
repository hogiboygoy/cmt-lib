/*
Проект "MeLViN" 32-битная ОС C++
Файл mvnFsFat16.cpp
Описание
  Реализация ФС на основе FAT16
История
  18.09.2008 создан
*/
struct CmtFat16 : public CmtFat12 {
  CmtFat16( CmtBlockDevice *disk, uint32 _fatSize, uint32 lbaSize, uint32 spc, uint32 fatNum, uint32 lbaStart, uint32 numRoot, uint32 numClus ) :
    CmtFat12(  disk, _fatSize, lbaSize, spc, fatNum, lbaStart, numRoot, numClus ) {}

          void           NeedCluster16( uint32 cluster );
  virtual uint32         GetCluster( uint32 cluster );
  virtual void           SetCluster( uint32 cluster, uint32 value );
  };

void
CmtFat16::NeedCluster16( uint32 cluster ) {
  cluster >>= 8; //Номер требуемого сектора FAT
  if( cluster != curFatSector ) {
    //Требуется загрузка нового сектора
    FlushFat();
    //Прочитать нужный сектор
    mDisk->Read( (uint32*)fatSector, cluster + ptLBAStart, 1 );
    //Установить текущий номер сектора FAT
    curFatSector = cluster;
    }
  }

#define END_CLUSTER16 0xffff
uint32
CmtFat16::GetCluster( uint32 cluster ) {
  //Обеспечить нахождение номера кластера в буфере сектора fat
  NeedCluster16( cluster );
  cluster = ((uint16*)(fatSector))[cluster & 0xff];
  if( cluster == END_CLUSTER16 ) cluster = END_CLUSTER;
  return cluster;
  }

void
CmtFat16::SetCluster( uint32 cluster, uint32 value ) {
  //Обеспечить нахождение номера кластера в буфере сектора fat
  NeedCluster16( cluster );
  //Получить значение индекса
  ((uint16*)(fatSector))[cluster & 0xff] = (uint16)value;
  dirtyFatSector = 1;
  }






