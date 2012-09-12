//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
typedef uint32    Elf32_Addr; //      4       4       Unsigned program address
typedef uint16    Elf32_Half; //      2       2       Unsigned medium integer
typedef uint32    Elf32_Off;  //      4       4       Unsigned file offset
typedef int32     Elf32_Sword;//      4       4       Signed large integer
typedef uint32    Elf32_Word; //      4       4       Unsigned large integer 

//Заголовок файла
typedef struct {
  unsigned char       e_ident[16];
  Elf32_Half          e_type;               //Тип файла 3-разделяемый объект
  Elf32_Half          e_machine;            //Тип машины 3-Intel386
  Elf32_Word          e_version;            //Версия 1-текущая
  Elf32_Addr          e_entry;              //Виртуальный адрес точки входа
  Elf32_Off           e_phoff;              //Смещение в файле таблицы заголовков программы
  Elf32_Off           e_shoff;              //Смещение в файле таблицы заголовков секций
  Elf32_Word          e_flags;              //Флаги
  Elf32_Half          e_ehsize;             //Размер заголовка
  Elf32_Half          e_phentsize;          //Размер заголовка программы
  Elf32_Half          e_phnum;              //Количество записей в таблице заголовков программы
  Elf32_Half          e_shentsize;          //Размер заголовка секции
  Elf32_Half          e_shnum;              //Количество записей в таблице секций
  Elf32_Half          e_shstrndx;           //Номер секции содержащей таблицу строк
  } Elf32_Ehdr;

//Заголовок секции
typedef struct {
  Elf32_Word  sh_name;         //Индекс имени секции в таблице строк
  Elf32_Word  sh_type;         //
  Elf32_Word  sh_flags;
  Elf32_Addr  sh_addr;         //Адрес начала секции в байтах
  Elf32_Off   sh_offset;       //Смещение секции от начала файла
  Elf32_Word  sh_size;         //Размер секции в файле, если type=SHT_NOBITS-то в памяти
  Elf32_Word  sh_link;         //Индекс связанной секции
  Elf32_Word  sh_info;         //
  Elf32_Word  sh_addralign;    //Выравнивание
  Elf32_Word  sh_entsize;      //Размер записи для секций содержащих таблицы
  } Elf32_Shdr;


//Заголовок программы
typedef struct {
  Elf32_Word  p_type;
  Elf32_Off   p_offset;
  Elf32_Addr  p_vaddr;
  Elf32_Addr  p_paddr;
  Elf32_Word  p_filesz;
  Elf32_Word  p_memsz;
  Elf32_Word  p_flags;
  Elf32_Word  p_align;
  } Elf32_Phdr;
 
//    uint32 mStartAddr;
#define   ELFMAG0      0x7f      /* EI_MAG */
#define   ELFMAG1      'E'
#define   ELFMAG2      'L'
#define   ELFMAG3      'F'

uint_8 
cmtProbeElf( CmtFile &src ) {
  if( src.Good() ) {
    char buf[4];
    src.Seek(0);
    src.Read( buf, 4 );
    cmtDebug3( 1, 1, "Probe res = %1", *((uint32*)buf) );
    return buf[0] == ELFMAG0 && buf[1] == ELFMAG1 && buf[2] == ELFMAG2 && buf[3] == ELFMAG3 ? CMTE_OK : CMTE_FAIL; 
    }
  return CMTE_FAIL;
  }

uint_8 
cmtParseElf( CmtFile &src, CmtFile &dst, uint32 *start ) {
  if( cmtProbeElf( src ) != CMTE_OK ) return CMTE_FAIL;
  cmtDebug3( 2, 1, "Probe ok", 0 );

  Elf32_Ehdr ehdr;
  src.Seek(0);
  src.Read( &ehdr, sizeof(ehdr) );

  //Инициализировать стартовый адрес
  if( start )
    *start = ehdr.e_entry;
  
  cmtDebug3( 2, 2, "Header ok, start addr=%1", ehdr.e_entry );
  
  //Начинаем загрузку
  Elf32_Phdr seg;
  
  //Прочитать сегменты
  for( int i = 0; i < ehdr.e_phnum; ++i ) {
    //Позиционировать на начало записи
    src.Seek( ehdr.e_phoff + i * ehdr.e_phentsize );
    cmtDebug3( 2, 3, "Segment %1", i );
    //Прочитать запись
    src.Read( &seg, sizeof(seg) );
    if( !src.Good() || !dst.Good() ) return CMTE_FAIL;
    //Определить необходимость загрузки
    if( seg.p_type != 1 ) continue;
    if( seg.p_filesz ) {
      //Seek to begin data
      src.Seek( seg.p_offset );
      //Load
      dst.Seek( seg.p_vaddr );
      if( cmtParseBinCopy( src, dst, seg.p_filesz ) != CMTE_OK ) return CMTE_FAIL;
      cmtDebug3( 2, 5, "Load ok at %1", seg.p_vaddr );
      
      //correct zeroing sizes
      seg.p_vaddr += seg.p_filesz;
      seg.p_memsz -= seg.p_filesz;
      }
    //Zero memory
    if( seg.p_memsz ) {
      memset( (void*)(seg.p_vaddr), 0, seg.p_memsz );
      cmtDebug3( 2,7, "Zeroing at %1", seg.p_vaddr ); 
      }
    cmtDebug3( 2, 6, "Segment %1 ok", i );
    }
  return CMTE_OK;
  }

