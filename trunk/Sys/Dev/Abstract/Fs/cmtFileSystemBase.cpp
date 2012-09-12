//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

CmtFileSystemBasePtr cmtLogDrives[CMT_FS];

//=====================================================================================================
//------------------------ Реализация высокоуровневого доступа к файлам -------------------------------
CmtFileSystemBase cmtNullFileSystem(0);

uint_8
cmtAllocFileSystem( uint_8 baseSlot, CmtFileSystemBase *fs ) {
  for( uint_8 i = baseSlot; i < CMT_FS; i++ )
    if( cmtLogDrives[i] == 0 ) {
      //Слот свободен, занять
      cmtLogDrives[i] = fs;
      return CMTE_OK;
      }
  return CMTE_FS_COUNT_OVER;
  }


CmtFileSystemBase*
cmtGetLogDisk( cpchar fname ) {
  CmtFileSystemBase *fs = &cmtNullFileSystem;
  //Проверим наличие диска в имени
  if( fname[1] == ':' ) {
    //Диск присутствует
    int i = (fname[0] | 0x20) - 'a';
    if( i >= 0 && i < CMT_FS && cmtLogDrives[i] )
      fs = cmtLogDrives[i];
    }
  //При отсутствии диска возвратить диск C
  else if( cmtLogDrives[2] ) fs = cmtLogDrives[2];
  return fs;
  }

cpchar
cmtCutDisk( cpchar fname ) {
  if( fname[1] == ':' ) fname += 2;
  if( fname[0] == '\\' ) fname++;
  else if( fname[0] == '/' ) fname++;
  return fname;
  }

//! Получить атрибуты файла
int32
cmtGetFileAttr( cpchar fname, CMT_FILE_ATTR *lpFileAttr ) {
  return cmtGetLogDisk( fname )->GetFileAttr( cmtCutDisk(fname), lpFileAttr );
  }

//! Установить атрибуты файла
int32
cmtSetFileAttr( cpchar fname, CMT_FILE_ATTR *lpFileAttr ) {
  return cmtGetLogDisk( fname )->SetFileAttr( cmtCutDisk(fname), lpFileAttr );
  }

//! Удаление файла с заданным именем
int32
cmtFileDelete( cpchar fname ) {
  return cmtGetLogDisk( fname )->FileDelete( cmtCutDisk(fname) );
  }

int32
cmtCreateDirectory( cpchar fname ) {
  return cmtGetLogDisk( fname )->CreateDirectory( cmtCutDisk(fname) );
  }

int32
cmtFileRename( cpchar sour, cpchar dest ) {
  CmtFileSystemBase *srcDisk = cmtGetLogDisk(sour);  //Получить логический диск
  CmtFileSystemBase *dstDisk = cmtGetLogDisk(dest);  //Получить логический диск
  if( srcDisk != dstDisk ) return CMTE_FS_NOT_IDENTIFICAL;
  return srcDisk->FileRename( cmtCutDisk(sour), cmtCutDisk(dest) );
  }

//! Найти файл по шаблону и создать поисковик для следующих файлов
//! Если attr равен нулю, то определяется только наличие файла и поисковик не создается
CmtFinderBase*
cmtFindFirst( cpchar pattern, int32 findFlag, CMT_FILE_ATTR *attr ) {
  return cmtGetLogDisk( pattern )->FindFirst( cmtCutDisk(pattern), findFlag, attr );
  }

//! Создание файла с заданным именем для чтения или записи
CmtFileBase*
cmtFileCreate( cpchar fname, int32 msMode ) {
  return cmtGetLogDisk( fname )->FileCreate( cmtCutDisk(fname), msMode );
  }

//! Проверка существования файла (без шаблонов)
//! \param fname - имя файла, существование которого надо проверить
//! \return возвращает 0 в случае наличия файла или код ошибки
int32    
cmtTestFileExist( cpchar fname ) {
  return cmtFindFirst( fname, 0, 0 ) ? CMTE_OK : CMTE_FS_NO_FILE;
  }


int32    
cmtDiskPresent( uint8 disk ) {
  if( disk < CMT_FS && cmtLogDrives[disk] )
    return CMTE_OK;
  return CMTE_BD_NO_DEV;
  }

CmtFileTime
FileDateTimeCreate( uint8 day, uint8 month, uint16 year, uint8 second, uint8 minute, uint8 hour ) {
  CmtFileTime tm = year - 1980;
  tm &= 0x7f;
  tm <<= 4;
  tm |= month;
  tm <<= 5;
  tm |= day;
  tm <<= 5;
  tm |= hour;
  tm <<= 6;
  tm |= minute;
  tm <<= 5;
  tm |= second >> 1;
  return tm;
  }

//=====================================================================================================
//------------------------ Создание файловых систем ---------------------------------------------------
//Структура разделов ЖД
#define ptFlag(base)                       ((uint8*)base)[0]     //Флаг раздела 80-активный, 0-неактивный
#define ptSHead(base)                      ((uint8*)base)[1]     //Начальная головка раздела
#define ptSSector(base)      cmtRead16   ( ((uint8*)base) + 2 )  //Начальный сектор и цилиндр (6 младших сектор, 10 старших цилиндр)
#define ptType(base)                       ((uint8*)base)[4]     //Тип файловой системы (06-FAT16)
#define ptEHead(base)                      ((uint8*)base)[5]     //Последняя головка раздела
#define ptESector(base)      cmtRead16   ( ((uint8*)base) + 6 )  //Последний сектор и цилиндр
#define ptLBAStart(base)     cmtRead32   ( ((uint8*)base) + 8 )  //Номер начального сектора раздела в режиме LBA
#define ptLBASize(base)      cmtRead32   ( ((uint8*)base) + 12)  //Количество секторов в разделе
#define DOS_PARTITION 16

//Boot-сектор
#define bsStart(base)                      ((uint8*)base)         //Команда безусловного перехода на программу загрузки
#define bsName(base)                       (((uint8*)base)+3)     //Имя и версия OEM (зависит от типа установленной ОС)
#define bsBPS(base)          cmtRead16   ( ((uint8*)base) + 11 )  //Байт на сектор
#define bsSPC(base)                        ((uint8*)base)[13]     //Секторов на кластер
#define bsReserv(base)       cmtRead16   ( ((uint8*)base) + 14 )  //Количество зарезервированных секторов, включая Boot-сектор
#define bsNumFat(base)                     ((uint8*)base)[16]     //Количество таблиц FAT
#define bsNumRootElem(base)  cmtRead16   ( ((uint8*)base) + 17 )  //Количество элементов в корневом каталоге
#define bsSectorNum16(base)  cmtRead16   ( ((uint8*)base) + 19 )  //Количество секторов на логическом диске (16-битный вариант)
#define bsType(base)                       ((uint8*)base)[21]     //Тип носителя (F8-HDD, FD-FDD)
#define bsSPF(base)          cmtRead16   ( ((uint8*)base) + 22 )	//Количество секторов на FAT
#define bsSPT(base)          cmtRead16   ( ((uint8*)base) + 24 )  //Количество секторов на трек
#define bsHeadNumber(base)   cmtRead16   ( ((uint8*)base) + 26 )  //Количество головок
#define bsHideNumber(base)   cmtRead32   ( ((uint8*)base) + 28 )  //Количество "скрытых" секторов
#define bsSectorNum32(base)  cmtRead32   ( ((uint8*)base) + 32 )  //Количество секторов LBA
#define bsDiskNum(base)                     ((uint8*)base)[36]    //Физический номер диска
#define bsSignature(base)                   ((uint8*)base)[38]    //Сигнатура 29h
#define bsSerialNum(base)    cmtRead32   ( ((uint8*)base) + 39 )  //Серийный номер диска
#define bsLabel(base)                       (((uint8*)base)+43)   //Метка тома
#define bsFSID(base)                        (((uint8*)base)+54)   //Идентификатор файловой системы
#define BootSector16    62

//FAT32
#define bsSignature32(base)                 ((uint8*)base)[66]    //Сигнатура
#define bsSPF32(base)         cmtRead32   ( ((uint8*)base) + 36 ) //Количество секторов на FAT
#define bsRootCluster32(base) cmtRead32   ( ((uint8*)base) + 44 ) //Номер кластера корневого директория
#define bsFSID32(base)                      (((uint8*)base) + 82) //Идентификатор файловой системы

#ifdef CMT_FS_FAT
uint_8
cmtCftAddLogDisk( CmtBlockDevice *disk, uint_8 baseSlot, uint32 lbaStart, uint8 sector[] ) {
  cmtDebug2( 2, 1, "cmtCftAddLogDisk start %1", lbaStart );
  //uint8 sector[512];
  //ReadSector( lbaStart, 1, sector );// теперь - считать BootSector
  // Первоначально считаем, что тип FAT = FAT16
  if( sector[0] != 0xeb || (bsSignature(sector) != 0x29 && bsSignature32(sector) != 0x29) ) {
    cmtDebug2( 2, 2, "cmtCftAddLogDisk fail signature", 0 );
    return CMTE_FS_BOOT_FAIL;
    }

  uint32 fatSize = bsSPF(sector);         //Размер FAT в секторах (для FAT16)- фиксированный, задается при форматировании. Для FAT32 это поле == 0, размер FAT для FAT32 задается в другом поле.
  if ( fatSize == 0 )
    fatSize = bsSPF32(sector);  // если  boot->bsSPF == 0, то размер FAT задается в другом месте BootSector'a

  //Секторов в разделе, пытаемся прочитать вариант слово
  uint32 ptLBASize = bsSectorNum16(sector);       //Количество секторов в разделе слово
  if( ptLBASize == 0 )
    ptLBASize = bsSectorNum32(sector);     //Количество секторов в разделе дв.слово

  //Анализировать boot-сектор
  uint32 sectorPerCluster = bsSPC(sector);        //Секторов на кластер, для FAT - 512 шт.
  uint32 fatNumber = bsNumFat(sector);            //Количество таблиц FAT, обычно 2 шт.
  uint32 ptLBAStart= lbaStart + bsReserv(sector); //Вычислить начало таблиц FAT
  uint32 numRootElem = bsNumRootElem(sector);

  //Определить тип FAT:
  uint32 count = (numRootElem) >> 4; // вычислить кол-во секторов в корневой директории: для FAT16 должно получиться 32; для FAT32 numRootElem == 0, поэтому рез-тат тоже будет ==0
  uint32 tmp = ptLBASize - (bsReserv(sector) + (bsNumFat(sector) * fatSize) + count); // вычислить кол-во секторов, отведенное в разделе непосредственно для данных
  tmp = tmp / bsSPC(sector); // то же самое, но в размерности [кластер]

  cmtDebug2( 2, 3, "cmtCftAddLogDisk claster count %1", tmp );
  if ( tmp < 4085 ) {
    //FAT12
    #ifdef CMT_FS_FAT12
      //Проверить название FAT
      if( bsFSID(sector)[0] != 0x46 || bsFSID(sector)[1] != 0x41 || bsFSID(sector)[3] != 0x31 || bsFSID(sector)[4] != 0x32 )
        return CMTE_FS_UNSUPPORTED;
      cmtDebug2( 2, 4, "cmtCftAddLogDisk FAT12 detected", 0 );
      return cmtAllocFileSystem( baseSlot, new CmtFat12( disk, fatSize, ptLBASize, sectorPerCluster, fatNumber, ptLBAStart, numRootElem, tmp ) );
    #else
      return CMTE_FS_UNSUPPORTED; //FAT12 не поддерживается
    #endif
    }
  else if ( tmp < 65525 ) {
    //FAT16
    #ifdef CMT_FS_FAT16
      //Проверить название FAT
      if( bsFSID(sector)[0] != 0x46 || bsFSID(sector)[1] != 0x41 || bsFSID(sector)[3] != 0x31 || bsFSID(sector)[4] != 0x36 )
        return CMTE_FS_UNSUPPORTED;
      cmtDebug2( 2, 5, "cmtCftAddLogDisk FAT16 detected", 0 );
      return cmtAllocFileSystem( baseSlot, new CmtFat16( disk, fatSize, ptLBASize, sectorPerCluster, fatNumber, ptLBAStart, numRootElem, tmp ) );
    #else
      return CMTE_FS_UNSUPPORTED; //FAT16 не поддерживается
    #endif
    }
  //FAT32
  #ifdef CMT_FS_FAT32
    //Проверить название FAT
    if( bsFSID32(sector)[0] != 0x46 || bsFSID32(sector)[1] != 0x41 || bsFSID32(sector)[3] != 0x33 || bsFSID32(sector)[4] != 0x32 )
      return CMTE_FS_UNSUPPORTED;
    cmtDebug2( 2, 6, "cmtCftAddLogDisk FAT32 detected", 0 );
    return cmtAllocFileSystem( baseSlot, new CmtFat32( disk, fatSize, ptLBASize, sectorPerCluster, fatNumber, ptLBAStart, bsRootCluster32(sector), tmp ) );
  #else
    return CMTE_FS_UNSUPPORTED; //FAT32 не поддерживается
  #endif
  }

uint_8
cmtCftAddPartition( CmtBlockDevice *disk, uint_8 baseSlot, uint32 start ) {
  uint8 sector[512];
  unsigned count;
  uint8 *ptr;  //Указатель на таблицу разделов
  //Считать сектор с разделом
  cmtDebug2( 3, 1, "cmtCftAddPartition read sector %1", start );
  if( disk->Read( (uint32*)sector, start, 1 ) != CMTE_OK ) return CMTE_FS_MBR_READ;
  cmtDebug2( 3, 2, "cmtCftAddPartition read OK", 0 );
  if( sector[0] != 0xeb ) { // в Bootsector[0] иногда вместо 0xEB может лежать 0xE9
    uint_8 result;
    // Это - MBR.
    //Проверить сигнатуру "0x55AA" в MBR:
    if( sector[510] != 0x55 || sector[511] != 0xaa ) {
      return CMTE_FS_MBR_FAIL;
      }
    //Считать данные таблицы разделов:
    ptr = (uint8*)(sector + 0x1be);
    for( count = 0; count < 4 && ptLBAStart(ptr); count++ ) {
      result = cmtCftAddPartition( disk, baseSlot, start + ptLBAStart(ptr) );  //AddLogDisk( baseSlot, ptLBAStart(ptr) );
      if( result != CMTE_OK ) break;
      ptr += DOS_PARTITION;
      }
    return count > 0 ? CMTE_OK : result;
    }
  cmtDebug2( 3, 3, "cmtCftAddPartition no partition", 0 );
  return cmtCftAddLogDisk( disk, baseSlot, start, sector );
  }

#endif

uint_8 
CmtFileSystemBase::CreateFileSystem( CmtBlockDevice *disk, uint_8 baseSlot ) {
  if( disk->mStatus != CMTE_OK ) return disk->mStatus;
#ifdef CMT_FS_FAT
  if( disk->mBlockSize == 512 ) return cmtCftAddPartition( disk, baseSlot, 0 );
#endif
#ifdef CMT_FS_CD
  if( disk->mBlockSize == 2048 ) return cmtCreateFsCD( disk, baseSlot );
#endif
  return CMTE_FS_UNSUPPORTED;
  }

