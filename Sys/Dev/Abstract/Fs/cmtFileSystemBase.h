//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
//! ����� ��� fmode ��� �������� �����
#define CMT_FMODE
#define CMT_CREATE_WRITE        1  //!< ������� ��� ������
#define CMT_CREATE_READ         2  //!< ������� ��� ������
#define CMT_SHARE_WRITE         4  //!< ��������� ������ ������
#define CMT_SHARE_READ          8  //!< ��������� ������ ������
#define CMT_FILE_RESET       0x10  //!< ��� �������� ����� ������� ���������� ����������

//! ����� ��� from ��� ����������� ��������� �����
#define CMT_SEEK
#define CMT_SEEK_BEGIN          0  //! ������������ ������ �����
#define CMT_SEEK_CURRENT        1  //! ������������ ������� �������
#define CMT_SEEK_END            2  //! ������������ ����� �����

//! �������� ������
#define FA
#define FA_READ_ONLY            1  //! ���� ������ ��� ������
#define FA_HIDDEN               2  //! ���� �������
#define FA_SYSTEM               4  //! ���� ���������
#define FA_LABEL                8  //! ����� ����
#define FA_DIRECTORY           16  //! ����������
#define FA_ARHIV               32  //! �������� ����

//! ������������ ������ ������ ��� ����� �����
#define CMT_MAX_PATH          256

//! 32-������ �������� �������-���� ����� � ������� DOS
//! - 0-4   ������� �������� �� 2
//! - 5-10  ������ (0-59)
//! - 11-15 ���� (0-23)
//! - 16-20 ���� ������ (1-31)
//! - 21-24 ����� (1-������, 2-������� � �.�.)
//! - 25-31 ��� � 1980 ����
typedef uint32 CmtFileDateTime;

//! 16-������ �������� ������� ����� � ������� DOS
//! - 0-4   ������� �������� �� 2
//! - 5-10  ������ (0-59)
//! - 11-15 ���� (0-23)
typedef uint16 CmtFileTime;

//! 16-������ �������� ���� ����� � ������� DOS
//! - 0-4 ���� ������ (1-31)
//! - 5-8 ����� (1-������, 2-������� � �.�.)
//! - 9-15 ��� � 1980 ����
typedef uint16 CmtFileDate;

//! ���������� ��� ������� �� 32-������� �������� �������-���� �����
//! \param fdt - 32-������ �������� �������-���� ����� � ������� DOS ::CmtFileDateTime
//! \return 16-������ �������� ������� ����� � ������� DOS ::CmtFileTime
#define cmtFDTgetTime( fdt )  (fdt & 0xffff)

//! ���������� ��� ���� �� 32-������� �������� �������-���� �����
//! \param fdt - 32-������ �������� �������-���� ����� � ������� DOS ::CmtFileDateTime
//! \return 16-������ �������� ���� ����� � ������� DOS ::CmtFileDate
#define cmtFDTgetDate( fdt )  ((fdt >> 16) & 0xffff)

//! ���������� ������� �� 32-������� �������� �������-���� �����
//! \param fdt - 32-������ �������� �������-���� ����� � ������� DOS ::CmtFileDateTime
#define cmtFDTgetSecond( ft ) ((ft & 0x1f) << 1)

//! ���������� ������ �� 32-������� �������� �������-���� �����
//! \param fdt - 32-������ �������� �������-���� ����� � ������� DOS ::CmtFileDateTime
#define cmtFDTgetMinute( ft ) ((ft >> 5) & 0x3f)

//! ���������� ���� �� 32-������� �������� �������-���� �����
//! \param fdt - 32-������ �������� �������-���� ����� � ������� DOS ::CmtFileDateTime
#define cmtFDTgetHour( ft )   ((ft >> 11) & 0x1f)

//! ���������� ���� ������ (1-31) �� 32-������� �������� �������-���� �����
//! \param fdt - 32-������ �������� �������-���� ����� � ������� DOS ::CmtFileDateTime
#define cmtFDTgetDay( fd )    ((fd >> 16) & 0x1f)

//! ���������� ����� ������ (1-������, 2-������� � �.�.) �� 32-������� �������� �������-���� �����
//! \param fdt - 32-������ �������� �������-���� ����� � ������� DOS ::CmtFileDateTime
#define cmtFDTgetMonth( fd )  ((fd >> 21) & 0xf)

//! ���������� ��� �� 32-������� �������� �������-���� �����
//! \param fdt - 32-������ �������� �������-���� ����� � ������� DOS ::CmtFileDateTime
#define cmtFDTgetYear( fd )   (((fd >> 25) & 0x7f) + 1980)

//! ���������� ������� �� 16-������� �������� ������� �����
//! \param fdt - 16-������ �������� ������� ����� � ������� DOS ::CmtFileTime
#define mvnFTgetSecond( wTime ) ((wTime & 0x1f) << 1)

//! ���������� ������ �� 16-������� �������� ������� �����
//! \param fdt - 16-������ �������� ������� ����� � ������� DOS ::CmtFileTime
#define mvnFTgetMinute( wTime ) ((wTime >> 5) & 0x3f)

//! ���������� ���� �� 16-������� �������� ������� �����
//! \param fdt - 16-������ �������� ������� ����� � ������� DOS ::CmtFileTime
#define mvnFTgetHour( wTime )   ((wTime >> 11) & 0x1f)

//! ���������� ���� ������ (1-31) �� 16-������� �������� ���� �����
//! \param fdt - 16-������ �������� ���� ����� � ������� DOS ::CmtFileDate
#define mvnFDgetDay( wDate )    ((wDate & 0x1f))

//! ���������� ����� ������ (1-������, 2-������� � �.�.) �� 16-������� �������� ���� �����
//! \param fdt - 16-������ �������� ���� ����� � ������� DOS ::CmtFileDate
#define mvnFDgetMonth( wDate )  ((wDate >> 5) & 0xf)

//! ���������� ��� �� 16-������� �������� ���� �����
//! \param fdt - 16-������ �������� ���� ����� � ������� DOS ::CmtFileDate
#define mvnFDgetYear( wDate )   (((wDate >> 9) & 0x7f) + 1980)

//! ������� 32-������ �������� �������-���� ����� �� ���������
//! \param day - ���� ������ (1-31)
//! \param month - ����� (1-������, 2-������� � �.�.)
//! \param year - ���
//! \param second - ������� (0-59)
//! \param minute - ������ (0-59)
//! \param hour - ���� (0-23)
//! \return 32-������ �������� �������-���� ����� ::CmtFileDateTime
CmtFileDateTime cmtFileDateTimeCreate( uint8 day, uint8 month, uint16 year, uint8 second, uint8 minute, uint8 hour );

//! ���� � ��������� � ������� �����
uint32 cmtGetSystemFileTime();

//! �������� �����.
struct CMT_FILE_ATTR {
  uint16   mCreationTime;     //!< ����� �������� ����� ::CmtFileTime
  uint16   mCreationDate;     //!< ���� �������� ����� ::CmtFileDate
  uint16   mUpdateTime;       //!< ����� ��������/���������� ����� ::CmtFileTime
  uint16   mUpdateDate;       //!< ���� ��������/���������� ����� ::CmtFileDate
  uint32   nFileSizeLow;      //!< ������ �����
  uint32   nFileSizeHigh;
  uint8    cFileName[ CMT_MAX_PATH ]; //!< ������ ��� ����� (��� ����)
  uint8    cAlternateFileName[ 14 ];  //!< �������������� (�����������) ��� �����
  uint8    fileAttributes;            //!< ���� ��������� ����� ::FA
  };

struct CmtFinderBase;
struct CmtFileBase;

#ifndef CMT_FS_WIN
//! ������� �������� �������. ��������� ����� ������ ����� ������������ ������� �������� �������
struct CmtFileSystemBase : public CmtDev {
  CmtBlockDevice *mDisk; //!< �������� ����������, �� ������� �������� �������� �������
  
  //! �����������
  /*! �����������
    \param disk �������� ����������, ��������������� � ������ �������� �������� */
  CmtFileSystemBase(CmtBlockDevice *disk) : CmtDev(), mDisk(disk) {}
  ~CmtFileSystemBase() { if( mDisk ) CDECREF(mDisk); }

  //! ����� ������� ����������� ����� �����.
  //! ���� ������ ���� ���������� ��� ������������ ������. ���� ���� ����, �� �����������
  //! ��������� ��������� � ������������ ������ ������ ��� ��������� ���������� ������.
  //! \param pattern - ������ ��� ������ ������ (��������� ������)
  //! \param findFlag - ������� ���� ��� ��������� ���������� ������
  //! \param attr - ��������� �� ��������� ���������� ����� ::CMT_FILE_ATTR, ����������� ��� ���������� ����� 
  //! \result ���������� ������ ������ ����������� ������ ::CmtFinderBase ��� 0 � ������ ���� �� �����
  virtual CmtFinderBase* FindFirst( cpchar pattern, int32 findFlag, CMT_FILE_ATTR *attr ) { return 0; }
  
  //! �������� �����.
  //! \param fname ��� ����� ������������ ������ �������� �������
  //! \param fmode ����� �������� ������ ::CMT_FMODE
  //! \return ���������� ��������� ���� ��� 0 � ������ ������
  virtual CmtFileBase*   FileCreate( cpchar fname, int32 fmode ) { return 0; }
  
  //! �������� ����������
  //! \param dirName - ��� ���������� ������������ ������ �������� �������
  //! \return ���������� 0 � ������ ������ ��� ��� ������
  virtual int32          CreateDirectory( cpchar dirName ) { return CMTE_NOT_RELEASED; }
  
  //! �������������� ����� (������� �� ��������������)
  //! \param sour - ��� ��������� �����
  //! \param dest - ��� ����� �������������
  //! \return ���������� 0 � ������ ������ ��� ��� ������
  virtual int32          FileRename( cpchar sour, cpchar dest ) { return CMTE_NOT_RELEASED; }
  
  //! �������� ����� (������� �� ��������������)
  //! \param fname - ��� ���������� �����
  //! \return ���������� 0 � ������ ������ ��� ��� ������
  virtual int32          FileDelete( cpchar fname ) { return CMTE_NOT_RELEASED; }
  
  //! ��������� ��������� �����
  //! \param fname - ��� �����
  //! \param attr - ��������� �� ��������� ���������� ����� ::CMT_FILE_ATTR, ����������� ��� ���������� ����� 
  //! \return ���������� 0 � ������ ������ ��� ��� ������
  virtual int32          GetFileAttr( cpchar fname, CMT_FILE_ATTR *attr ) { return CMTE_NOT_RELEASED; }

  //! ��������� ��������� �����
  //! \param fname - ��� �����
  //! \param attr - ��������� �� ��������� ���������� ����� ::CMT_FILE_ATTR, �������� ����� ��������������� � ������������ � ������ ���������
  //! \return ���������� 0 � ������ ������ ��� ��� ������
  virtual int32          SetFileAttr( cpchar fname, CMT_FILE_ATTR *attr ) { return CMTE_NOT_RELEASED; }
  
  static uint_8 CreateFileSystem( CmtBlockDevice *disk, uint_8 baseSlot );
  };

typedef CmtFileSystemBase *CmtFileSystemBasePtr;

extern CmtFileSystemBasePtr cmtLogDrives[CMT_FS];
#endif

//! ����� ������ - ������� ���������
struct CmtFinderBase {
  virtual ~CmtFinderBase() {}

  //! ����� ���������� ����������� ����� �����.
  //! ���� ��������� ���� ���������� ��� ������ ���������� ��� �������� ����������. ���� ���� ����, �� �����������
  //! ��������� ���������.
  //! \param attr - ��������� �� ��������� ���������� ����� ::CMT_FILE_ATTR, ����������� ��� ���������� ����� 
  //! \return ���������� 0 � ������ ������ ��� ��� ������
  virtual uint_8 FindNext( CMT_FILE_ATTR *attr ) = 0;
  };

//���� - ������� �����
struct CmtFileBase {
  virtual ~CmtFileBase() {}

  //! ������ �� �����
  //! \param buffer - �����, � ������� ������������ ������ ������
  //! \param size - ���������� �������� ������
  //! \return ���������� CMTE_OK ��� ���������� ������ ��� ��� ������
  virtual uint32 Read( pvoid buffer, uint32 size ) = 0;

  //! ������ � ����
  //! \param buffer - �����, �� �������� ������������ ������ ������
  //! \param size - ���������� ������������ ������
  //! \return ���������� CMTE_OK ��� ���������� ������ ��� ��� ������
  virtual uint32 Write( cpvoid buffer, uint32 size ) = 0;
  
  //! �������� ��������� �����
  //! \param pos - ����������, �� ������� ����� �������� ��������� �����
  //! \param from - �� ������ ����� ����������� ���������� ���������� 
  //! \return ���������� CMTE_OK ��� ���������� ������ ��� ��� ������
  virtual uint32 Seek( int32 pos, uint32 from ) = 0;
  
  //! �������� ���������� ������� ����� � �������� �������
  //! \return ���������� CMTE_OK ��� ���������� ������ ��� ��� ������
  virtual uint32 Flush() = 0;

  //! ���������� ������� ��������� ��������� �����
  //! \return ���������� ������� ��������� ��������� �����
  virtual uint32 Position() = 0;

  //! ���������� ������ �����
  //! \return ���������� ������ �����
  virtual uint32 Size() = 0;
  
  //! ���������� ������ �� ��������� �����, ���� ��������� � ����� �����, �� ���������� 0xff
  //! \return ���������� ������ �� ��������� �����
  virtual char   Peek() = 0;
  
  //! �������� ������ ����� � ������������� ��������� ����� �� ����, ���� ���� ������ ��� ������
  virtual void   Clear() = 0;
  
  //! ���������� ��������� �����
  //! \return ���������� ��������� �����. 0 - ��� ������ ��� ��� ������ � ��������� ������
  virtual int32  State() = 0;
  };

/**
 * \defgroup fileSystem ��������� � �������� ��������
 * @{
 */

//! ����� ������� ����������� ����� �����.
//! ���� ������ ���� ���������� ��� ������������ ������. ���� ���� ����, �� �����������
//! ��������� ��������� � ������������ ������ ������ ��� ��������� ���������� ������.
//! \param pattern - ������ ��� ������ ������ (��������� ������)
//! \param findFlag - ������� ���� ��� ��������� ���������� ������
//! \param attr - ��������� �� ��������� ���������� ����� ::CMT_FILE_ATTR, ����������� ��� ���������� ����� 
//! \result ���������� ������ ������ ����������� ������ ::CmtFinderBase ��� 0 � ������ ���� �� �����
CmtFinderBase* cmtFindFirst( cpchar pattern, int32 findFlag, CMT_FILE_ATTR *attr );

//! �������� �����.
//! \param fname ��� �����
//! \param fmode ����� �������� ������ ::CMT_FMODE
//! \return ���������� ��������� ���� ��� 0 � ������ ������
CmtFileBase* cmtFileCreate( cpchar fname, int32 msMode );

//! ���������� ����
//! ��������� ������� ����������� �����, � �.�. ��������
//! \param disk ����� ����� (0-A, 1-B, 2-C � �.�.)
//! \result ���������� 0 ���� ���� ���� � ��� ������ � ��������� ������
int32    cmtDiskPresent( uint8 disk );

//! ��������� ��������� �����
//! \param fname - ��� �����
//! \param attr - ��������� �� ��������� ���������� ����� ::CMT_FILE_ATTR, ����������� ��� ���������� ����� 
//! \return ���������� 0 � ������ ������ ��� ��� ������
int32    cmtGetFileAttr( cpchar fname, CMT_FILE_ATTR *lpFileAttr );

//! ��������� ��������� �����
//! \param fname - ��� �����
//! \param attr - ��������� �� ��������� ���������� ����� ::CMT_FILE_ATTR, �������� ����� ��������������� � ������������ � ������ ���������
//! \return ���������� 0 � ������ ������ ��� ��� ������
int32    cmtSetFileAttr( cpchar fname, CMT_FILE_ATTR *lpFileAttr );

//! �������� ����������
//! \param dirName - ��� ����������
//! \return ���������� 0 � ������ ������ ��� ��� ������
int32    cmtCreateDirectory( cpchar fname );

//! �������������� ����� (������� �� ��������������)
//! \param sour - ��� ��������� �����
//! \param dest - ��� ����� �������������
//! \return ���������� 0 � ������ ������ ��� ��� ������
int32    cmtFileRename( cpchar sour, cpchar dest );

//! �������� ����� (������� �� ��������������)
//! \param fname - ��� ���������� �����
//! \return ���������� 0 � ������ ������ ��� ��� ������
int32    cmtFileDelete( cpchar fname );

//! �������� ������������� ����� (��� ��������)
//! \param fname - ��� �����, ������������� �������� ���� ���������
//! \return ���������� 0 � ������ ������� ����� ��� ��� ������
int32    cmtTestFileExist( cpchar fname );

#ifndef CMT_FS_WIN
//! ���������� �������� ������� � ������ ������
/*! ��������� �������� ������� � ������ ���������� ���������.
  \param baseSlot ���������������� ����� ����� ��� �������� ������� (� ���� ���������� ����� ����������)
  \param fs ����������� �������� �������
  \return CMTE_OK � ������ ��������� ���������� ��� ��� ������ */
uint_8   cmtAllocFileSystem( uint_8 baseSlot, CmtFileSystemBase *fs );
#endif
/**
 * @}
 */

class CmtFileFs : public CmtFile {
  protected:
    CmtFileBase *mFile;
    uint_8       mState;
  public:
    CmtFileFs( CmtFileBase *file ) : CmtFile(), mFile(file), mState( file == 0 ? CMTE_FAIL : file->State() ) {}

    //������� ����-�����
    virtual void  Read( pvoid dest, uint32 size )  { if( mState == 0 ) mState |= mFile->Read( dest, size ); }
    virtual void  Write( cpvoid src, uint32 size ) { if( mState == 0 ) mState |= mFile->Write( src, size ); }

    //��������
    virtual bool Good() { return mState == CMTE_OK && mFile->State() == CMTE_OK; }

    //����������������
    virtual void Seek( uint32 absPos ) { if( mState == 0 ) mState |= mFile->Seek( (int32)absPos, CMT_SEEK_BEGIN ); }

    virtual uint32 GetPos() { if( mState ) return 0; return mFile->Position(); }

    virtual uint32 Size() { if( mState ) return 0; return mFile->Size(); }
  };

class CmtFileRead : public CmtFileFs {
  protected:
    uint32 mSize;
  public:
    CmtFileRead( cpchar fname ) : CmtFileFs( cmtFileCreate( fname, CMT_CREATE_READ ) ), mSize(0) { if(mFile) mSize = mFile->Size(); }
    ~CmtFileRead() { if( mFile ) delete mFile; mFile = 0; }

    virtual bool Good() { return mState == CMTE_OK && mFile->Position() < mSize; }
  };

class CmtFileWrite : public CmtFileFs {
  public:
    CmtFileWrite( cpchar fname, bool reset = true ) : CmtFileFs( cmtFileCreate( fname, CMT_CREATE_WRITE | (reset ? CMT_FILE_RESET : 0) ) ) {}
    ~CmtFileWrite() { if( mFile ) delete mFile; mFile = 0; }
  };

class CmtFileFinder {
    CmtFinderBase *mFinder;
  public:
    CMT_FILE_ATTR  mAttr;

    CmtFileFinder( cpchar pat, int32 findFlag = 0 ) :
      mFinder(0) {
      mFinder = cmtFindFirst( pat, findFlag, &mAttr );
      }

    ~CmtFileFinder() { if( mFinder ) delete mFinder; }

    bool IsGood() const { return mFinder != 0; }

    void Next() { if( mFinder && mFinder->FindNext( &mAttr ) ) { delete mFinder; mFinder = 0; } }
  };

