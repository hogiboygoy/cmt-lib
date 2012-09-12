//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

CmtFileSystemBasePtr cmtLogDrives[CMT_FS];

//=====================================================================================================
//------------------------ ���������� ���������������� ������� � ������ -------------------------------
CmtFileSystemBase cmtNullFileSystem(0);

uint_8
cmtAllocFileSystem( uint_8 baseSlot, CmtFileSystemBase *fs ) {
  for( uint_8 i = baseSlot; i < CMT_FS; i++ )
    if( cmtLogDrives[i] == 0 ) {
      //���� ��������, ������
      cmtLogDrives[i] = fs;
      return CMTE_OK;
      }
  return CMTE_FS_COUNT_OVER;
  }


CmtFileSystemBase*
cmtGetLogDisk( cpchar fname ) {
  CmtFileSystemBase *fs = &cmtNullFileSystem;
  //�������� ������� ����� � �����
  if( fname[1] == ':' ) {
    //���� ������������
    int i = (fname[0] | 0x20) - 'a';
    if( i >= 0 && i < CMT_FS && cmtLogDrives[i] )
      fs = cmtLogDrives[i];
    }
  //��� ���������� ����� ���������� ���� C
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

//! �������� �������� �����
int32
cmtGetFileAttr( cpchar fname, CMT_FILE_ATTR *lpFileAttr ) {
  return cmtGetLogDisk( fname )->GetFileAttr( cmtCutDisk(fname), lpFileAttr );
  }

//! ���������� �������� �����
int32
cmtSetFileAttr( cpchar fname, CMT_FILE_ATTR *lpFileAttr ) {
  return cmtGetLogDisk( fname )->SetFileAttr( cmtCutDisk(fname), lpFileAttr );
  }

//! �������� ����� � �������� ������
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
  CmtFileSystemBase *srcDisk = cmtGetLogDisk(sour);  //�������� ���������� ����
  CmtFileSystemBase *dstDisk = cmtGetLogDisk(dest);  //�������� ���������� ����
  if( srcDisk != dstDisk ) return CMTE_FS_NOT_IDENTIFICAL;
  return srcDisk->FileRename( cmtCutDisk(sour), cmtCutDisk(dest) );
  }

//! ����� ���� �� ������� � ������� ��������� ��� ��������� ������
//! ���� attr ����� ����, �� ������������ ������ ������� ����� � ��������� �� ���������
CmtFinderBase*
cmtFindFirst( cpchar pattern, int32 findFlag, CMT_FILE_ATTR *attr ) {
  return cmtGetLogDisk( pattern )->FindFirst( cmtCutDisk(pattern), findFlag, attr );
  }

//! �������� ����� � �������� ������ ��� ������ ��� ������
CmtFileBase*
cmtFileCreate( cpchar fname, int32 msMode ) {
  return cmtGetLogDisk( fname )->FileCreate( cmtCutDisk(fname), msMode );
  }

//! �������� ������������� ����� (��� ��������)
//! \param fname - ��� �����, ������������� �������� ���� ���������
//! \return ���������� 0 � ������ ������� ����� ��� ��� ������
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
//------------------------ �������� �������� ������ ---------------------------------------------------
//��������� �������� ��
#define ptFlag(base)                       ((uint8*)base)[0]     //���� ������� 80-��������, 0-����������
#define ptSHead(base)                      ((uint8*)base)[1]     //��������� ������� �������
#define ptSSector(base)      cmtRead16   ( ((uint8*)base) + 2 )  //��������� ������ � ������� (6 ������� ������, 10 ������� �������)
#define ptType(base)                       ((uint8*)base)[4]     //��� �������� ������� (06-FAT16)
#define ptEHead(base)                      ((uint8*)base)[5]     //��������� ������� �������
#define ptESector(base)      cmtRead16   ( ((uint8*)base) + 6 )  //��������� ������ � �������
#define ptLBAStart(base)     cmtRead32   ( ((uint8*)base) + 8 )  //����� ���������� ������� ������� � ������ LBA
#define ptLBASize(base)      cmtRead32   ( ((uint8*)base) + 12)  //���������� �������� � �������
#define DOS_PARTITION 16

//Boot-������
#define bsStart(base)                      ((uint8*)base)         //������� ������������ �������� �� ��������� ��������
#define bsName(base)                       (((uint8*)base)+3)     //��� � ������ OEM (������� �� ���� ������������� ��)
#define bsBPS(base)          cmtRead16   ( ((uint8*)base) + 11 )  //���� �� ������
#define bsSPC(base)                        ((uint8*)base)[13]     //�������� �� �������
#define bsReserv(base)       cmtRead16   ( ((uint8*)base) + 14 )  //���������� ����������������� ��������, ������� Boot-������
#define bsNumFat(base)                     ((uint8*)base)[16]     //���������� ������ FAT
#define bsNumRootElem(base)  cmtRead16   ( ((uint8*)base) + 17 )  //���������� ��������� � �������� ��������
#define bsSectorNum16(base)  cmtRead16   ( ((uint8*)base) + 19 )  //���������� �������� �� ���������� ����� (16-������ �������)
#define bsType(base)                       ((uint8*)base)[21]     //��� �������� (F8-HDD, FD-FDD)
#define bsSPF(base)          cmtRead16   ( ((uint8*)base) + 22 )	//���������� �������� �� FAT
#define bsSPT(base)          cmtRead16   ( ((uint8*)base) + 24 )  //���������� �������� �� ����
#define bsHeadNumber(base)   cmtRead16   ( ((uint8*)base) + 26 )  //���������� �������
#define bsHideNumber(base)   cmtRead32   ( ((uint8*)base) + 28 )  //���������� "�������" ��������
#define bsSectorNum32(base)  cmtRead32   ( ((uint8*)base) + 32 )  //���������� �������� LBA
#define bsDiskNum(base)                     ((uint8*)base)[36]    //���������� ����� �����
#define bsSignature(base)                   ((uint8*)base)[38]    //��������� 29h
#define bsSerialNum(base)    cmtRead32   ( ((uint8*)base) + 39 )  //�������� ����� �����
#define bsLabel(base)                       (((uint8*)base)+43)   //����� ����
#define bsFSID(base)                        (((uint8*)base)+54)   //������������� �������� �������
#define BootSector16    62

//FAT32
#define bsSignature32(base)                 ((uint8*)base)[66]    //���������
#define bsSPF32(base)         cmtRead32   ( ((uint8*)base) + 36 ) //���������� �������� �� FAT
#define bsRootCluster32(base) cmtRead32   ( ((uint8*)base) + 44 ) //����� �������� ��������� ����������
#define bsFSID32(base)                      (((uint8*)base) + 82) //������������� �������� �������

#ifdef CMT_FS_FAT
uint_8
cmtCftAddLogDisk( CmtBlockDevice *disk, uint_8 baseSlot, uint32 lbaStart, uint8 sector[] ) {
  cmtDebug2( 2, 1, "cmtCftAddLogDisk start %1", lbaStart );
  //uint8 sector[512];
  //ReadSector( lbaStart, 1, sector );// ������ - ������� BootSector
  // ������������� �������, ��� ��� FAT = FAT16
  if( sector[0] != 0xeb || (bsSignature(sector) != 0x29 && bsSignature32(sector) != 0x29) ) {
    cmtDebug2( 2, 2, "cmtCftAddLogDisk fail signature", 0 );
    return CMTE_FS_BOOT_FAIL;
    }

  uint32 fatSize = bsSPF(sector);         //������ FAT � �������� (��� FAT16)- �������������, �������� ��� ��������������. ��� FAT32 ��� ���� == 0, ������ FAT ��� FAT32 �������� � ������ ����.
  if ( fatSize == 0 )
    fatSize = bsSPF32(sector);  // ����  boot->bsSPF == 0, �� ������ FAT �������� � ������ ����� BootSector'a

  //�������� � �������, �������� ��������� ������� �����
  uint32 ptLBASize = bsSectorNum16(sector);       //���������� �������� � ������� �����
  if( ptLBASize == 0 )
    ptLBASize = bsSectorNum32(sector);     //���������� �������� � ������� ��.�����

  //������������� boot-������
  uint32 sectorPerCluster = bsSPC(sector);        //�������� �� �������, ��� FAT - 512 ��.
  uint32 fatNumber = bsNumFat(sector);            //���������� ������ FAT, ������ 2 ��.
  uint32 ptLBAStart= lbaStart + bsReserv(sector); //��������� ������ ������ FAT
  uint32 numRootElem = bsNumRootElem(sector);

  //���������� ��� FAT:
  uint32 count = (numRootElem) >> 4; // ��������� ���-�� �������� � �������� ����������: ��� FAT16 ������ ���������� 32; ��� FAT32 numRootElem == 0, ������� ���-��� ���� ����� ==0
  uint32 tmp = ptLBASize - (bsReserv(sector) + (bsNumFat(sector) * fatSize) + count); // ��������� ���-�� ��������, ���������� � ������� ��������������� ��� ������
  tmp = tmp / bsSPC(sector); // �� �� �����, �� � ����������� [�������]

  cmtDebug2( 2, 3, "cmtCftAddLogDisk claster count %1", tmp );
  if ( tmp < 4085 ) {
    //FAT12
    #ifdef CMT_FS_FAT12
      //��������� �������� FAT
      if( bsFSID(sector)[0] != 0x46 || bsFSID(sector)[1] != 0x41 || bsFSID(sector)[3] != 0x31 || bsFSID(sector)[4] != 0x32 )
        return CMTE_FS_UNSUPPORTED;
      cmtDebug2( 2, 4, "cmtCftAddLogDisk FAT12 detected", 0 );
      return cmtAllocFileSystem( baseSlot, new CmtFat12( disk, fatSize, ptLBASize, sectorPerCluster, fatNumber, ptLBAStart, numRootElem, tmp ) );
    #else
      return CMTE_FS_UNSUPPORTED; //FAT12 �� ��������������
    #endif
    }
  else if ( tmp < 65525 ) {
    //FAT16
    #ifdef CMT_FS_FAT16
      //��������� �������� FAT
      if( bsFSID(sector)[0] != 0x46 || bsFSID(sector)[1] != 0x41 || bsFSID(sector)[3] != 0x31 || bsFSID(sector)[4] != 0x36 )
        return CMTE_FS_UNSUPPORTED;
      cmtDebug2( 2, 5, "cmtCftAddLogDisk FAT16 detected", 0 );
      return cmtAllocFileSystem( baseSlot, new CmtFat16( disk, fatSize, ptLBASize, sectorPerCluster, fatNumber, ptLBAStart, numRootElem, tmp ) );
    #else
      return CMTE_FS_UNSUPPORTED; //FAT16 �� ��������������
    #endif
    }
  //FAT32
  #ifdef CMT_FS_FAT32
    //��������� �������� FAT
    if( bsFSID32(sector)[0] != 0x46 || bsFSID32(sector)[1] != 0x41 || bsFSID32(sector)[3] != 0x33 || bsFSID32(sector)[4] != 0x32 )
      return CMTE_FS_UNSUPPORTED;
    cmtDebug2( 2, 6, "cmtCftAddLogDisk FAT32 detected", 0 );
    return cmtAllocFileSystem( baseSlot, new CmtFat32( disk, fatSize, ptLBASize, sectorPerCluster, fatNumber, ptLBAStart, bsRootCluster32(sector), tmp ) );
  #else
    return CMTE_FS_UNSUPPORTED; //FAT32 �� ��������������
  #endif
  }

uint_8
cmtCftAddPartition( CmtBlockDevice *disk, uint_8 baseSlot, uint32 start ) {
  uint8 sector[512];
  unsigned count;
  uint8 *ptr;  //��������� �� ������� ��������
  //������� ������ � ��������
  cmtDebug2( 3, 1, "cmtCftAddPartition read sector %1", start );
  if( disk->Read( (uint32*)sector, start, 1 ) != CMTE_OK ) return CMTE_FS_MBR_READ;
  cmtDebug2( 3, 2, "cmtCftAddPartition read OK", 0 );
  if( sector[0] != 0xeb ) { // � Bootsector[0] ������ ������ 0xEB ����� ������ 0xE9
    uint_8 result;
    // ��� - MBR.
    //��������� ��������� "0x55AA" � MBR:
    if( sector[510] != 0x55 || sector[511] != 0xaa ) {
      return CMTE_FS_MBR_FAIL;
      }
    //������� ������ ������� ��������:
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

