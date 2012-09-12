//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
//! Флаги для fmode при открытии файла
#define CMT_FMODE
#define CMT_CREATE_WRITE        1  //!< Открыть для записи
#define CMT_CREATE_READ         2  //!< Открыть для чтения
#define CMT_SHARE_WRITE         4  //!< Разрешить другим запись
#define CMT_SHARE_READ          8  //!< Разрешить другим чтение
#define CMT_FILE_RESET       0x10  //!< При открытии файла стирает предыдущее содержимое

//! Флаги для from при перемещении указателя файла
#define CMT_SEEK
#define CMT_SEEK_BEGIN          0  //! Относительно начала файла
#define CMT_SEEK_CURRENT        1  //! Относительно текущей позиции
#define CMT_SEEK_END            2  //! Относительно конца файла

//! Атрибуты файлов
#define FA
#define FA_READ_ONLY            1  //! Файл только для чтения
#define FA_HIDDEN               2  //! Файл скрытый
#define FA_SYSTEM               4  //! Файл системный
#define FA_LABEL                8  //! Метка тома
#define FA_DIRECTORY           16  //! Директорий
#define FA_ARHIV               32  //! Архивный файл

//! Максимальный размер строки для имени файла
#define CMT_MAX_PATH          256

//! 32-битное значение времени-даты файла в формате DOS
//! - 0-4   секунды деленные на 2
//! - 5-10  минуты (0-59)
//! - 11-15 часы (0-23)
//! - 16-20 день месяца (1-31)
//! - 21-24 месяц (1-январь, 2-февраль и т.д.)
//! - 25-31 год с 1980 года
typedef uint32 CmtFileDateTime;

//! 16-битное значение времени файла в формате DOS
//! - 0-4   секунды деленные на 2
//! - 5-10  минуты (0-59)
//! - 11-15 часы (0-23)
typedef uint16 CmtFileTime;

//! 16-битное значение даты файла в формате DOS
//! - 0-4 день месяца (1-31)
//! - 5-8 месяц (1-январь, 2-февраль и т.д.)
//! - 9-15 год с 1980 года
typedef uint16 CmtFileDate;

//! Возвращает код времени из 32-битного значения времени-даты файла
//! \param fdt - 32-битное значение времени-даты файла в формате DOS ::CmtFileDateTime
//! \return 16-битное значение времени файла в формате DOS ::CmtFileTime
#define cmtFDTgetTime( fdt )  (fdt & 0xffff)

//! Возвращает код даты из 32-битного значения времени-даты файла
//! \param fdt - 32-битное значение времени-даты файла в формате DOS ::CmtFileDateTime
//! \return 16-битное значение даты файла в формате DOS ::CmtFileDate
#define cmtFDTgetDate( fdt )  ((fdt >> 16) & 0xffff)

//! Возвращает секунды из 32-битного значения времени-даты файла
//! \param fdt - 32-битное значение времени-даты файла в формате DOS ::CmtFileDateTime
#define cmtFDTgetSecond( ft ) ((ft & 0x1f) << 1)

//! Возвращает минуты из 32-битного значения времени-даты файла
//! \param fdt - 32-битное значение времени-даты файла в формате DOS ::CmtFileDateTime
#define cmtFDTgetMinute( ft ) ((ft >> 5) & 0x3f)

//! Возвращает часы из 32-битного значения времени-даты файла
//! \param fdt - 32-битное значение времени-даты файла в формате DOS ::CmtFileDateTime
#define cmtFDTgetHour( ft )   ((ft >> 11) & 0x1f)

//! Возвращает день месяца (1-31) из 32-битного значения времени-даты файла
//! \param fdt - 32-битное значение времени-даты файла в формате DOS ::CmtFileDateTime
#define cmtFDTgetDay( fd )    ((fd >> 16) & 0x1f)

//! Возвращает номер месяца (1-январь, 2-февраль и т.д.) из 32-битного значения времени-даты файла
//! \param fdt - 32-битное значение времени-даты файла в формате DOS ::CmtFileDateTime
#define cmtFDTgetMonth( fd )  ((fd >> 21) & 0xf)

//! Возвращает год из 32-битного значения времени-даты файла
//! \param fdt - 32-битное значение времени-даты файла в формате DOS ::CmtFileDateTime
#define cmtFDTgetYear( fd )   (((fd >> 25) & 0x7f) + 1980)

//! Возвращает секунды из 16-битного значения времени файла
//! \param fdt - 16-битное значение времени файла в формате DOS ::CmtFileTime
#define mvnFTgetSecond( wTime ) ((wTime & 0x1f) << 1)

//! Возвращает минуты из 16-битного значения времени файла
//! \param fdt - 16-битное значение времени файла в формате DOS ::CmtFileTime
#define mvnFTgetMinute( wTime ) ((wTime >> 5) & 0x3f)

//! Возвращает часы из 16-битного значения времени файла
//! \param fdt - 16-битное значение времени файла в формате DOS ::CmtFileTime
#define mvnFTgetHour( wTime )   ((wTime >> 11) & 0x1f)

//! Возвращает день месяца (1-31) из 16-битного значения даты файла
//! \param fdt - 16-битное значение даты файла в формате DOS ::CmtFileDate
#define mvnFDgetDay( wDate )    ((wDate & 0x1f))

//! Возвращает номер месяца (1-январь, 2-февраль и т.д.) из 16-битного значения даты файла
//! \param fdt - 16-битное значение даты файла в формате DOS ::CmtFileDate
#define mvnFDgetMonth( wDate )  ((wDate >> 5) & 0xf)

//! Возвращает год из 16-битного значения даты файла
//! \param fdt - 16-битное значение даты файла в формате DOS ::CmtFileDate
#define mvnFDgetYear( wDate )   (((wDate >> 9) & 0x7f) + 1980)

//! Создает 32-битное значение времени-даты файла из элементов
//! \param day - день месяца (1-31)
//! \param month - месяц (1-январь, 2-февраль и т.д.)
//! \param year - год
//! \param second - секунды (0-59)
//! \param minute - минуты (0-59)
//! \param hour - часы (0-23)
//! \return 32-битное значение времени-даты файла ::CmtFileDateTime
CmtFileDateTime cmtFileDateTimeCreate( uint8 day, uint8 month, uint16 year, uint8 second, uint8 minute, uint8 hour );

//! Часы и календарь в формате файла
uint32 cmtGetSystemFileTime();

//! Атрибуты файла.
struct CMT_FILE_ATTR {
  uint16   mCreationTime;     //!< Время создания файла ::CmtFileTime
  uint16   mCreationDate;     //!< Дата создания файла ::CmtFileDate
  uint16   mUpdateTime;       //!< Время создания/обновления файла ::CmtFileTime
  uint16   mUpdateDate;       //!< Дата создания/обновления файла ::CmtFileDate
  uint32   nFileSizeLow;      //!< Размер файла
  uint32   nFileSizeHigh;
  uint8    cFileName[ CMT_MAX_PATH ]; //!< Полное имя файла (без пути)
  uint8    cAlternateFileName[ 14 ];  //!< Альтернативное (сокращенное) имя файла
  uint8    fileAttributes;            //!< Блок атрибутов файла ::FA
  };

struct CmtFinderBase;
struct CmtFileBase;

#ifndef CMT_FS_WIN
//! Базовая файловая система. Экземпляр этого класса будет представлять нулевую файловую систему
struct CmtFileSystemBase : public CmtDev {
  CmtBlockDevice *mDisk; //!< Блоковое устройство, на котором работает файловая система
  
  //! Конструктор
  /*! Конструктор
    \param disk блоковое устройство, ассоциированное с данной файловой системой */
  CmtFileSystemBase(CmtBlockDevice *disk) : CmtDev(), mDisk(disk) {}
  ~CmtFileSystemBase() { if( mDisk ) CDECREF(mDisk); }

  //! Поиск первого подходящего файла файла.
  //! Ищет первый файл подходящий под передаваемый шаблон. Если файл есть, то заполняется
  //! структура атрибутов и возвращается объект поиска для следующих подходящих файлов.
  //! \param pattern - шаблон для поиска файлов (текстовая строка)
  //! \param findFlag - битовое поле для уточнения параметров поиска
  //! \param attr - указатель на структуру аттрибутов файла ::CMT_FILE_ATTR, заполняется при нахождении файла 
  //! \result возвращает объект поиска последующих файлов ::CmtFinderBase или 0 в случае если не нашел
  virtual CmtFinderBase* FindFirst( cpchar pattern, int32 findFlag, CMT_FILE_ATTR *attr ) { return 0; }
  
  //! Создание файла.
  //! \param fname имя файла относительно данной файловой системы
  //! \param fmode режим открытия файлов ::CMT_FMODE
  //! \return возвращает созданный файл или 0 в случае ошибки
  virtual CmtFileBase*   FileCreate( cpchar fname, int32 fmode ) { return 0; }
  
  //! Создание директория
  //! \param dirName - имя директория относительно данной файловой системы
  //! \return возвращает 0 в случае успеха или код ошибки
  virtual int32          CreateDirectory( cpchar dirName ) { return CMTE_NOT_RELEASED; }
  
  //! Переименование файла (шаблоны не поддерживаются)
  //! \param sour - имя исходного файла
  //! \param dest - имя файла присваиваемое
  //! \return возвращает 0 в случае успеха или код ошибки
  virtual int32          FileRename( cpchar sour, cpchar dest ) { return CMTE_NOT_RELEASED; }
  
  //! Удаление файла (шаблоны не поддерживаются)
  //! \param fname - имя удаляемого файла
  //! \return возвращает 0 в случае успеха или код ошибки
  virtual int32          FileDelete( cpchar fname ) { return CMTE_NOT_RELEASED; }
  
  //! Получение атрибутов файла
  //! \param fname - имя файла
  //! \param attr - указатель на структуру аттрибутов файла ::CMT_FILE_ATTR, заполняется при нахождении файла 
  //! \return возвращает 0 в случае успеха или код ошибки
  virtual int32          GetFileAttr( cpchar fname, CMT_FILE_ATTR *attr ) { return CMTE_NOT_RELEASED; }

  //! Установка атрибутов файла
  //! \param fname - имя файла
  //! \param attr - указатель на структуру аттрибутов файла ::CMT_FILE_ATTR, атрибуты файла устанавливаются в соответствии с полями структуры
  //! \return возвращает 0 в случае успеха или код ошибки
  virtual int32          SetFileAttr( cpchar fname, CMT_FILE_ATTR *attr ) { return CMTE_NOT_RELEASED; }
  
  static uint_8 CreateFileSystem( CmtBlockDevice *disk, uint_8 baseSlot );
  };

typedef CmtFileSystemBase *CmtFileSystemBasePtr;

extern CmtFileSystemBasePtr cmtLogDrives[CMT_FS];
#endif

//! Поиск файлов - базовый поисковик
struct CmtFinderBase {
  virtual ~CmtFinderBase() {}

  //! Поиск следующего подходящего файла файла.
  //! Ищет следующий файл подходящий под шаблон переданный при создании поисковика. Если файл есть, то заполняется
  //! структура атрибутов.
  //! \param attr - указатель на структуру аттрибутов файла ::CMT_FILE_ATTR, заполняется при нахождении файла 
  //! \return возвращает 0 в случае успеха или код ошибки
  virtual uint_8 FindNext( CMT_FILE_ATTR *attr ) = 0;
  };

//Файл - базовый класс
struct CmtFileBase {
  virtual ~CmtFileBase() {}

  //! Чтение из файла
  //! \param buffer - буфер, в который производится чтение данных
  //! \param size - количество читаемых байтов
  //! \return возвращает CMTE_OK при отсутствии ошибки или код ошибки
  virtual uint32 Read( pvoid buffer, uint32 size ) = 0;

  //! Запись в файл
  //! \param buffer - буфер, из которого производится запись данных
  //! \param size - количество записываемых байтов
  //! \return возвращает CMTE_OK при отсутствии ошибки или код ошибки
  virtual uint32 Write( cpvoid buffer, uint32 size ) = 0;
  
  //! Сдвинуть указатель файла
  //! \param pos - расстояние, на которое нужно сдвинуть указатель файла
  //! \param from - от какого места производить вычисление расстояния 
  //! \return возвращает CMTE_OK при отсутствии ошибки или код ошибки
  virtual uint32 Seek( int32 pos, uint32 from ) = 0;
  
  //! Сбросить содержимое буферов файла в файловую систему
  //! \return возвращает CMTE_OK при отсутствии ошибки или код ошибки
  virtual uint32 Flush() = 0;

  //! Возвращает текущее положение указателя файла
  //! \return возвращает текущее положение указателя файла
  virtual uint32 Position() = 0;

  //! Возвращает размер файла
  //! \return возвращает размер файла
  virtual uint32 Size() = 0;
  
  //! Возвращает символ по указателю файла, если указатель в конце файла, то возвращает 0xff
  //! \return возвращает символ по указателю файла
  virtual char   Peek() = 0;
  
  //! Обнуляет размер файла и устанавливает указатель файла на нуль, если файл открыт для записи
  virtual void   Clear() = 0;
  
  //! Возвращает состояние файла
  //! \return возвращает состояние файла. 0 - нет ошибок или код ошибки в противном случае
  virtual int32  State() = 0;
  };

/**
 * \defgroup fileSystem Интерфейс к файловым системам
 * @{
 */

//! Поиск первого подходящего файла файла.
//! Ищет первый файл подходящий под передаваемый шаблон. Если файл есть, то заполняется
//! структура атрибутов и возвращается объект поиска для следующих подходящих файлов.
//! \param pattern - шаблон для поиска файлов (текстовая строка)
//! \param findFlag - битовое поле для уточнения параметров поиска
//! \param attr - указатель на структуру аттрибутов файла ::CMT_FILE_ATTR, заполняется при нахождении файла 
//! \result возвращает объект поиска последующих файлов ::CmtFinderBase или 0 в случае если не нашел
CmtFinderBase* cmtFindFirst( cpchar pattern, int32 findFlag, CMT_FILE_ATTR *attr );

//! Создание файла.
//! \param fname имя файла
//! \param fmode режим открытия файлов ::CMT_FMODE
//! \return возвращает созданный файл или 0 в случае ошибки
CmtFileBase* cmtFileCreate( cpchar fname, int32 msMode );

//! Логический диск
//! Проверяет наличие логического диска, в т.ч. сетевого
//! \param disk номер диска (0-A, 1-B, 2-C и т.д.)
//! \result возвращает 0 если диск есть и код ошибки в противном случае
int32    cmtDiskPresent( uint8 disk );

//! Получение атрибутов файла
//! \param fname - имя файла
//! \param attr - указатель на структуру аттрибутов файла ::CMT_FILE_ATTR, заполняется при нахождении файла 
//! \return возвращает 0 в случае успеха или код ошибки
int32    cmtGetFileAttr( cpchar fname, CMT_FILE_ATTR *lpFileAttr );

//! Установка атрибутов файла
//! \param fname - имя файла
//! \param attr - указатель на структуру аттрибутов файла ::CMT_FILE_ATTR, атрибуты файла устанавливаются в соответствии с полями структуры
//! \return возвращает 0 в случае успеха или код ошибки
int32    cmtSetFileAttr( cpchar fname, CMT_FILE_ATTR *lpFileAttr );

//! Создание директория
//! \param dirName - имя директория
//! \return возвращает 0 в случае успеха или код ошибки
int32    cmtCreateDirectory( cpchar fname );

//! Переименование файла (шаблоны не поддерживаются)
//! \param sour - имя исходного файла
//! \param dest - имя файла присваиваемое
//! \return возвращает 0 в случае успеха или код ошибки
int32    cmtFileRename( cpchar sour, cpchar dest );

//! Удаление файла (шаблоны не поддерживаются)
//! \param fname - имя удаляемого файла
//! \return возвращает 0 в случае успеха или код ошибки
int32    cmtFileDelete( cpchar fname );

//! Проверка существования файла (без шаблонов)
//! \param fname - имя файла, существование которого надо проверить
//! \return возвращает 0 в случае наличия файла или код ошибки
int32    cmtTestFileExist( cpchar fname );

#ifndef CMT_FS_WIN
//! Разместить файловую систему в списке систем
/*! Размещает файловую систему в списке логических устройств.
  \param baseSlot предпочтительный номер слота для файловой системы (с него начинается поиск свободного)
  \param fs размещаемая файловая система
  \return CMTE_OK в случае успешного размещения или код ошибки */
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

    //Блочный ввод-вывод
    virtual void  Read( pvoid dest, uint32 size )  { if( mState == 0 ) mState |= mFile->Read( dest, size ); }
    virtual void  Write( cpvoid src, uint32 size ) { if( mState == 0 ) mState |= mFile->Write( src, size ); }

    //Проверка
    virtual bool Good() { return mState == CMTE_OK && mFile->State() == CMTE_OK; }

    //Позиционирование
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

