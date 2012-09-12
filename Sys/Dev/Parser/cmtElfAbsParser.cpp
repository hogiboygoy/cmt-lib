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

//��������� �����
typedef struct {
  unsigned char       e_ident[16];
  Elf32_Half          e_type;               //��� ����� 3-����������� ������
  Elf32_Half          e_machine;            //��� ������ 3-Intel386
  Elf32_Word          e_version;            //������ 1-�������
  Elf32_Addr          e_entry;              //����������� ����� ����� �����
  Elf32_Off           e_phoff;              //�������� � ����� ������� ���������� ���������
  Elf32_Off           e_shoff;              //�������� � ����� ������� ���������� ������
  Elf32_Word          e_flags;              //�����
  Elf32_Half          e_ehsize;             //������ ���������
  Elf32_Half          e_phentsize;          //������ ��������� ���������
  Elf32_Half          e_phnum;              //���������� ������� � ������� ���������� ���������
  Elf32_Half          e_shentsize;          //������ ��������� ������
  Elf32_Half          e_shnum;              //���������� ������� � ������� ������
  Elf32_Half          e_shstrndx;           //����� ������ ���������� ������� �����
  } Elf32_Ehdr;

//��������� ������
typedef struct {
  Elf32_Word  sh_name;         //������ ����� ������ � ������� �����
  Elf32_Word  sh_type;         //
  Elf32_Word  sh_flags;
  Elf32_Addr  sh_addr;         //����� ������ ������ � ������
  Elf32_Off   sh_offset;       //�������� ������ �� ������ �����
  Elf32_Word  sh_size;         //������ ������ � �����, ���� type=SHT_NOBITS-�� � ������
  Elf32_Word  sh_link;         //������ ��������� ������
  Elf32_Word  sh_info;         //
  Elf32_Word  sh_addralign;    //������������
  Elf32_Word  sh_entsize;      //������ ������ ��� ������ ���������� �������
  } Elf32_Shdr;


//��������� ���������
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

  //���������������� ��������� �����
  if( start )
    *start = ehdr.e_entry;
  
  cmtDebug3( 2, 2, "Header ok, start addr=%1", ehdr.e_entry );
  
  //�������� ��������
  Elf32_Phdr seg;
  
  //��������� ��������
  for( int i = 0; i < ehdr.e_phnum; ++i ) {
    //��������������� �� ������ ������
    src.Seek( ehdr.e_phoff + i * ehdr.e_phentsize );
    cmtDebug3( 2, 3, "Segment %1", i );
    //��������� ������
    src.Read( &seg, sizeof(seg) );
    if( !src.Good() || !dst.Good() ) return CMTE_FAIL;
    //���������� ������������� ��������
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

