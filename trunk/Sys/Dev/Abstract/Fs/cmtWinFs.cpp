//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

class CmtWinFile : public CmtFileBase {
    HANDLE hBase;
  public:
    CmtWinFile( HANDLE h ) : CmtFileBase(), hBase(h) {}
    
    virtual ~CmtWinFile() { CloseHandle(hBase); }
  
    //! Чтение из файла
    //! \param buffer - буфер, в который производится чтение данных
    //! \param size - количество читаемых байтов
    //! \return возвращает CMTE_OK при отсутствии ошибки или код ошибки
    virtual uint32 Read( pvoid buffer, uint32 size );
  
    //! Запись в файл
    //! \param buffer - буфер, из которого производится запись данных
    //! \param size - количество записываемых байтов
    //! \return возвращает CMTE_OK при отсутствии ошибки или код ошибки
    virtual uint32 Write( cpvoid buffer, uint32 size );
    
    //! Сдвинуть указатель файла
    //! \param pos - расстояние, на которое нужно сдвинуть указатель файла
    //! \param from - от какого места производить вычисление расстояния 
    //! \return возвращает CMTE_OK при отсутствии ошибки или код ошибки
    virtual uint32 Seek( int32 pos, uint32 from );
    
    //! Сбросить содержимое буферов файла в файловую систему
    //! \return возвращает CMTE_OK при отсутствии ошибки или код ошибки
    virtual uint32 Flush() { return FlushFileBuffers( hBase ) ? CMTE_OK : CMTE_FS_FILE_ACCESS; }
  
    //! Возвращает текущее положение указателя файла
    //! \return возвращает текущее положение указателя файла
    virtual uint32 Position() { return SetFilePointer( hBase, 0, 0, FILE_CURRENT ); }
  
    //! Возвращает размер файла
    //! \return возвращает размер файла
    virtual uint32 Size() { return GetFileSize( hBase, 0 ); }
    
    //! Возвращает символ по указателю файла, если указатель в конце файла, то возвращает 0xff
    //! \return возвращает символ по указателю файла
    virtual char   Peek();
    
    //! Обнуляет размер файла и устанавливает указатель файла на нуль, если файл открыт для записи
    virtual void   Clear();
    
    //! Возвращает состояние файла
    //! \return возвращает состояние файла. 0 - нет ошибок или код ошибки в противном случае
    virtual int32  State() { return hBase != 0 ? CMTE_OK : CMTE_FS_NO_FILE; }
  };

uint32 
CmtWinFile::Read( pvoid buffer, uint32 size ) {
  DWORD dwSize;
  ReadFile( hBase, buffer, size, &dwSize, 0 );
  return size == dwSize ? CMTE_OK : CMTE_FS_FILE_ACCESS; 
  }

uint32 
CmtWinFile::Write( cpvoid buffer, uint32 size ) {
  DWORD dwSize;
  WriteFile( hBase, buffer, size, &dwSize, 0 );
  return size == dwSize ? CMTE_OK : CMTE_FS_FILE_ACCESS; 
  }

#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER 0xffffffff
#endif
uint32
CmtWinFile::Seek( int32 pos, uint32 from ) {
  switch( from ) {
    case CMT_SEEK_BEGIN   : from = FILE_BEGIN;   break;
    case CMT_SEEK_CURRENT : from = FILE_CURRENT; break;
    case CMT_SEEK_END     : from = FILE_END;     break;
    }
  return SetFilePointer( hBase, pos, 0, from ) == INVALID_SET_FILE_POINTER ? CMTE_FS_FILE_ACCESS : CMTE_OK;
  }

char
CmtWinFile::Peek() {
  char ch = 0;
  Read( &ch, 1 );
  Seek( -1, CMT_SEEK_CURRENT );
  return ch;
  }

void
CmtWinFile::Clear() {
  if( SetFilePointer( hBase, 0, 0, FILE_BEGIN ) != 0 ) return;
  SetEndOfFile( hBase );
  }

CmtFileBase*
cmtFileCreate( cpchar fname, int32 mode ) {
  CmtFileBase *mBase = 0;
  if( fname ) {
    uint32 access = (mode & CMT_CREATE_READ ? GENERIC_READ : 0) | (mode & CMT_CREATE_WRITE ? GENERIC_WRITE : 0);
    uint32 share = (mode & CMT_SHARE_READ ? FILE_SHARE_READ : 0) | (mode & CMT_SHARE_WRITE ? FILE_SHARE_WRITE : 0);
    uint32 create;
    if( access == GENERIC_READ )       create = OPEN_EXISTING;
    else if( access == GENERIC_WRITE ) create = CREATE_ALWAYS;
    else                               create = OPEN_ALWAYS;
    HANDLE hBase = CreateFileA( fname, access, share, 0, create, FILE_ATTRIBUTE_NORMAL, 0 );
    if( hBase == INVALID_HANDLE_VALUE ) hBase = 0;
    if( hBase ) mBase = new CmtWinFile( hBase );
    }
  return mBase;
  }

int32    
cmtTestFileExist( cpchar fname ) {
  return GetFileAttributesA( fname ) == 0xffffffff ? CMTE_FS_NO_FILE : CMTE_OK;
/*  WIN32_FIND_DATAA mData;
  HANDLE          hFinder = FindFirstFileA( fname, &mData );
  if( hFinder == INVALID_HANDLE_VALUE ) return CMTE_FS_NO_FILE;
  FindClose( hFinder );
  return CMTE_OK; */
  }


class CmtWinFileFinder : public CmtFinderBase {
    HANDLE          hFinder;
    uint32          mAttr;
  public:
    CmtWinFileFinder( HANDLE h, uint32 attr ) :
      hFinder(h),
      mAttr(attr) { }
    virtual uint_8 FindNext( CMT_FILE_ATTR *attr );
  };

bool
mvnTestAttr( _WIN32_FIND_DATAA *mData, int32 findFlag ) {
  if( findFlag == 0 ) return true;
  if( findFlag & FA_READ_ONLY && mData->dwFileAttributes & FILE_ATTRIBUTE_READONLY ) return true;
  if( findFlag & FA_HIDDEN    && mData->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) return true;
  if( findFlag & FA_SYSTEM    && mData->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ) return true;
  if( findFlag & FA_DIRECTORY && mData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) return true;
  if( findFlag & FA_ARHIV     && mData->dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE ) return true;
  return false;
  }

void
mvnCopyAttr( _WIN32_FIND_DATAA *mData, CMT_FILE_ATTR *attr ) {
  attr->mCreationTime = 0;
  attr->mCreationDate = 0;
  attr->mUpdateTime = 0;
  attr->mUpdateDate = 0;
  attr->nFileSizeLow = mData->nFileSizeLow;
  attr->nFileSizeHigh = mData->nFileSizeHigh;
  memcpy( attr->cFileName, mData->cFileName, CMT_MAX_PATH );
  if( mData->cAlternateFileName[0] == 0 ) {
    memcpy( attr->cAlternateFileName, mData->cFileName, 14 );
    }
  else memcpy( attr->cAlternateFileName, mData->cAlternateFileName, 14 );
  attr->fileAttributes = mData->dwFileAttributes & 0xff;
  }

uint_8
CmtWinFileFinder::FindNext( CMT_FILE_ATTR *attr ) {
  _WIN32_FIND_DATAA mData;
  ZeroMemory( &mData, sizeof(_WIN32_FIND_DATAA) );
  do {
    if( !::FindNextFileA( hFinder, &mData ) ) return CMTE_FS_NO_FILE;
    }
  while( !mvnTestAttr( &mData, mAttr ) );
  //Скопировать атрибуты
  mvnCopyAttr( &mData, attr );
  return CMTE_OK;
  }

CmtFinderBase*
cmtFindFirst( cpchar pattern, int32 findFlag, CMT_FILE_ATTR *attr ) {
  _WIN32_FIND_DATAA mData;
  HANDLE            hFinder = FindFirstFileA( pattern, &mData );
  if( hFinder == INVALID_HANDLE_VALUE ) return 0;
  //Проверить на соответствие флагу
  while( !mvnTestAttr( &mData, findFlag ) ) {
    if( !FindNextFileA( hFinder, &mData ) ) {
      //Поиск безуспешный
      FindClose( hFinder );
      return 0;
      }
    }
  if( attr ) {
    //Скопировать свойства
    mvnCopyAttr( &mData, attr );
    return new CmtWinFileFinder( hFinder, findFlag );
    }
  //Требуется только узнать о наличии
  return (CmtFinderBase*)1;
  }
  
int32
cmtFileDelete( cpchar fname ) {
  return DeleteFileA( fname ) ? CMTE_OK : CMTE_FS_NO_FILE;
  }
