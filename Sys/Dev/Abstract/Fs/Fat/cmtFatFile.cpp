//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
CmtFatFile::CmtFatFile( CmtFat *fs, FatDirEntry1x *ptr, uint32 mode ) {
  msMode = mode;
  mFileSize = ptr->mFileSize;        //Размер файла
  mDirSector = fs->curDirSector;     //Сектор директория
  mFilePosition = 0;  //Текущая позиция указателя файла
  mFirstCluster =
  mFileCluster = ptr->GetFirstCluster32();    //Текущий номер кластера файла
  mDirOffset = ((uint8*)ptr) - fs->dirSector; //Смещение в секторе директория позиции директория
  mFileSector = 0;    //Номер сектора в кластере
  mFileOffset = msMode & CMT_CREATE_READ ? 512 : 0;  //Смещение указателя внутри буфера сектора (кэша)
  mFat = fs;
#ifndef CMT_FS_NO_WRITE
  //Специальный случай дописывания
  if( (msMode & CMT_CREATE_WRITE) && mFileSize ) {
    //Нужно обеспечить в кэш последний сектор
    uint32 sectorCount = mFileSize >> 9;
    mFileOffset = mFileSize & 0x1ff;
    //Добраться до нужного кластера
    while( sectorCount > mFat->sectorPerCluster ) {
      mFileCluster = mFat->GetCluster( mFileCluster );
      sectorCount -= mFat->sectorPerCluster;
      }
    if( (sectorCount == mFat->sectorPerCluster) && mFileOffset ) {
      //Еще один кластер
      mFileCluster = mFat->GetCluster( mFileCluster );
      sectorCount -= mFat->sectorPerCluster;
      }
    //Добраться до нужного сектора
    mFileSector = sectorCount;
    //Считать содержимое сектора
    if( mFileOffset ) {
      mFat->mDisk->Read( (uint32*)buffer, mFileSector + mFat->SectorFromCluster( mFileCluster ), 1 );
      }
    mFilePosition = mFileSize;
    }
#endif
  CADDREF(mFat);
  }

CmtFatFile::~CmtFatFile() {
  //Сбросить буфер на диск
  Flush();
  //Освободить файловую систему
  CDECREF(mFat);
  }

uint32
CmtFatFile::ReadFileSectorInc() {
  //Проверить необходимость чтения нового кластера
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
    //Дошли до последнего сектора кластера, выделить следующий кластер
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
CmtFatFile::Seek( int32 offset, uint32 msFrom ) { //Изменение положения указателя файла
  uint32 target;
  uint32 bpc = mFat->sectorPerCluster * 512;
  if( msMode != CMT_CREATE_READ ) return CMTE_NOT_RELEASED;
  if( msFrom == CMT_SEEK_BEGIN ) {
    //От начала
    target = (uint32)offset;
    }
  else if( msFrom == CMT_SEEK_CURRENT ) {
    //От текущей позиции
    target = mFilePosition + offset;
    }
  else if( msFrom == CMT_SEEK_END ) {
    //От конечной позиции
    target = mFileSize + offset;
    }
  else return CMTE_BAD_PARAM;
  //Проверить границы
  if( target > mFileSize ) target = mFileSize;
  //Проверить ненужность движения
  if( target == mFilePosition ) return CMTE_OK;
  //Вычислить сектор
  mFileSector = (target >> 9) % (mFat->sectorPerCluster);
  //Проверить необходимость движения
  if( (target >= ((mFilePosition / bpc) * bpc)) && ( target < ((mFilePosition / bpc + 1) * bpc )) ) {
    //В текущем кластере
    }
  else {
    //В более позднем секторе, отыскать
    uint32 cluster = mFileCluster;
    uint32 c = mFilePosition / bpc;
    if( (target < ((mFilePosition / bpc) * bpc)) ) {
      //В более раннем секторе, отыскать
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
  //Прочитать текущий сектор в буфер
  uint32 res = ReadFileSectorInc();
  return res;
  }

uint32
CmtFatFile::Read( pvoid buf, uint32 size ) {
  if( size == 0 ) return CMTE_OK;
  uint8 *lpBuffer = (uint8*)buf;
  uint32 res = CMTE_OK;
  //Проверить возможность чтения файла
  if( size > mFileSize - mFilePosition )
    size = mFileSize - mFilePosition;
  //Выгребаем из кэш-буфера
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
  //Полные сектора
  CLOCK(mFat);
  while( size && res == CMTE_OK ) {
    //Когда размер превышает сектор и буфер выровнен по 32 разрядам
    if( size >= 512 && ((((uint32)lpBuffer) & 0x3) == 0) ) {
      //Полные сектора
      uint32 count = size >> 9; //Количество целых секторов
      if( mFileSector == mFat->sectorPerCluster ) {
        //Требуется взять очередной кластер
        mFileSector = 0;
        mFileCluster = mFat->GetCluster( mFileCluster );
        if( mFileCluster == 0 || mFileCluster == END_CLUSTER ) {
          res = CMTE_FS_FILE_STRUCT;
          break;
          }
        }
      if( count > mFat->sectorPerCluster - mFileSector )
        count = mFat->sectorPerCluster - mFileSector;
      //Прочитать сектора
      res = mFat->mDisk->Read( (uint32*)lpBuffer, mFileSector + mFat->SectorFromCluster( mFileCluster ), count );
      mFileSector += count;
      count <<= 9; //Количество секторов переведем в байты
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
    //Новая запись целиком влезает в кэш
    memcpy( buffer + mFileOffset, lpBuffer, nNumber );
    mFileOffset   += nNumber;
    mFileSize     += nNumber;
    mFilePosition += nNumber;
    }
  else {
    //Целиком в кэш не влезает
    //Проверим кэш на пустоту
    if( mFileOffset ) {
      //Что-то есть, дописать до целого
      memcpy( buffer + mFileOffset, lpBuffer, min );
      mFileSize     += min;
      mFilePosition += min;
      nNumber -= min;
      lpBuffer += min;
      //Записать в файл
      res = WriteFileSectorInc();
      mFileOffset = 0;
      }
    //Записать целые сектора
    while( nNumber && res == CMTE_OK ) {
      if( nNumber >= 512 && ((((uint32)lpBuffer) & 0x3) == 0) ) {
        //Полные сектора
        uint32 count = nNumber >> 9; //Количество целых секторов
        res = CheckWriteSector();
        if( res != CMTE_OK ) break;
        if( count > mFat->sectorPerCluster - mFileSector )
          count = mFat->sectorPerCluster - mFileSector;
        res = mFat->mDisk->Write( (uint32*)lpBuffer, mFileSector + mFat->SectorFromCluster( mFileCluster ), count );
        mFileSector += count;
        count <<= 9; //Количество секторов переведем в байты
        lpBuffer      += count;
        mFilePosition += count;
        mFileSize     += count;
        nNumber       -= count;
        }
      else {
        uint32 count = nNumber > 512 ? 512 : nNumber;
        //Заполнить остаток в кэш
        memcpy( buffer, lpBuffer, count );
        mFileOffset   = count;
        mFileSize     += count;
        mFilePosition += count;
        nNumber -= count;
        if( mFileOffset == 512 ) {
          //Записать в файл
          res = WriteFileSectorInc();
          mFileOffset = 0;
          }
        }
      }
    }
  //Обновить запись в директории
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
    //Сбросить буфер на диск
    if( mFileOffset ) {
      WriteFileSectorInc();
      mFileSector --;
      }
    //Сбросить на диск FAT и директорий
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
    //Освободить цепочку файла
    mFat->NeedDirSector( mDirSector );
    FatDirEntry1x *ptr = (FatDirEntry1x*)(mFat->dirSector + mDirOffset);
    ptr->mFileSize = 0;
    CmtSystemTime ft;
    ft.GetSystem();
    ptr->mUpdateTime = ft.FFTime();
    ptr->mUpdateDate = ft.FFDate();
    mFat->dirtyDirSector = 1;

    //Удалить цепочку кластеров
    mFat->FreeClusters( ptr->GetFirstCluster32() );

    ptr->SetFirstCluster32( mFat->GetFreeCluster() );
    if( ptr->GetFirstCluster32() > 0 )
      mFat->SetCluster( ptr->GetFirstCluster32(), END_CLUSTER );
    mFat->dirtyDirSector = 1;

    mFileSize = 0;        //Размер файла
    mFilePosition = 0;  //Текущая позиция указателя файла
    mFirstCluster =
    mFileCluster = ptr->GetFirstCluster32();   //Текущий номер кластера файла
    mFileSector = 0;    //Номер сектора в кластере
    mFileOffset = 0;    //Смещение указателя внутри буфера сектора (кэша)

    //Сбросить на диск FAT и директорий
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
    //Прочитать в кэш
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
