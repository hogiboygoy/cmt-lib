//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// ��������
//   ���� �������� �������� ������������ ����������
//==============================================================================

//! ����������� �������� ������ �� ��������� � �������� � �������� ��������
uint_8 cmtParseBinCopy( CmtFile &src, CmtFile &dst, uint32 size );

uint_8 cmtParseBinCopyBuf( CmtFile &src, CmtFile &dst, uint32 size );

//! ELF ��������� (���������)
uint_8 cmtParseElf( CmtFile &src, CmtFile &dst, uint32 *start );

//! ELF �����
uint_8 cmtProbeElf( CmtFile &src );

