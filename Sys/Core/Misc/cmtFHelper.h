//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

/**
 * \defgroup helperFuncs ������� ������ ����������
 * @{
 */
void cmtOneToHexUpper( uint_8 dig, pchar dest );

void cmtMultiToHexUpper( uint32 dig, pchar dest, uint_8 num );

pchar cmtSPrintStr( pchar dest, cpchar src );

pchar cmtSPrintHexUpper( pchar dest, uint32 value, uint_8 num );

pchar cmtSPrintUInt( pchar dest, uint32 value );

//! �������������� hex ������� � �����
//! \param ch - hex-������ (���������, ������� ����� - ��� �������)
//! \return �����, ��������������� �������
int cmtTransHex( char ch );

//! �������������� ������ hex �������� � �����
//! \param str - ������ hex-�������� (���������, ������� ����� - ��� �������), ����������� �����
//! \return �����, ��������������� ������
int atoh( cpchar str );

//! �������������� ���� ���������� �������� � �����
//! \param str ������ ���� ��������
//! \return �����, ��������������� ������
uint_8 cmtTwoDigitToInt( cpchar str );

//! �������������� ������� � ������� �������� (��������� �����).
//! ������� ������� �������.
//! \param c ������� ������
//! \return ������ � ������ ��������
uint8 cmtCharLower( uint8 c );

//! �������������� ������� � �������� �������� (������� �����).
//! ������� ������� �������.
//! \param c ������� ������
//! \return ������ � ������� ��������
uint8 cmtCharUpper( uint8 c );

//! ��������� �������� ��� ����� ��������
uint_8 cmtCompareChar( uint8 c1, uint8 c2 );

//! ��������� ����� ��� ����� �������� (������ �����, ����� ����� ��� ����� � ��������� �����)
//! \param s1 ������ 1
//! \param s2 ������ 2
//! \return -1 ������ 1 ������ ������ 2, 0 ������ �����, 1 ������ 1 ������ ������ 2
int cmtCompareStrNoReg( cpchar s1, cpchar s2 );

//! ��������� ������ � �������� ��� ����� �������� (������� ����� ���� ������ ������)
//! \param pat �������
//! \param str ������
//! \return true ����� ���� ���������� � ��������, false � ��������� ������
bool cmtComparePatNoReg( cpchar pat, cpchar str );

//! ��������� ����� � ������������ �� �����
//! \param src1 ������ 1
//! \param src2 ������ 2
//! \param len ������������ ���������� �������� ��� ���������
//! \return 0 ������ �����, 1 ����� �� �����
uint_8 cmtStrNCmp( const uint8 *src1, fcpchar src2, uint_8 len );

//! ����������� ����� � ������������ �� �����
void strmcpy( pchar dest, cpchar sour, uint32 maximum );

//! �������������� ������ � ������� ��������.
//! ������� ������� �������. �������������� ������������ �� ����� ������� ��������.
//! \param str ������ ��� ��������������
//! \return ��������� �������� ������� ������
void cmtStrLower( pchar str );

//! �������������� ������ � �������� ��������.
//! ������� ������� �������. �������������� ������������ �� ����� ������� ��������.
//! \param str ������ ��� ��������������
//! \return ��������� �������� ������� ������
void cmtStrUpper( pchar str );

//! ����� ������� � ������ � ����� ������
//! \param sour ������ ��� ������
//! \param ch ������� ������
//! \return ��������� �� ��������� ������ ������ � ����� ��� ����, ���� �������� �� �������
pchar cmtStrFindBack( pchar sour, char ch );

//! ����� ������� � ������ � ����� ������
//! \param sour ������ ��� ������
//! \param ch ������� ������
//! \return ��������� �� ��������� ������ ������ � ����� ��� ����, ���� �������� �� �������
cpchar cmtStrFindBack( cpchar sour, char ch );

//! ����� ��������� � ������ ��� ����� ��������
/*! \param str ������, � ������� ������������ �����
  \param subStr ������� ���������
  \return ��������� �� ��������� � ������ ��� ����, ���� ��������� �� ������� */
cpchar cmtStrStrNoReg( cpchar str, cpchar subStr );

#ifdef __cplusplus
inline int cmtIMin( int i1, int i2 ) { return i1 < i2 ? i1 : i2; }
inline int cmtIMax( int i1, int i2 ) { return i1 > i2 ? i1 : i2; }
inline int cmtAbs( int i1 ) { return i1 < 0 ? -i1 : i1; }

inline uint32 cmtUMin( uint32 i1, uint32 i2 ) { return i1 < i2 ? i1 : i2; }
inline uint32 cmtUMax( uint32 i1, uint32 i2 ) { return i1 > i2 ? i1 : i2; }

//! ����������� (������������, �.�. �������� limMin � limMax - ���������)
inline int32 cmtILimit( int32 val, int32 limMin, int32 limMax ) { if( val < limMin ) return limMin; if( val > limMax ) return limMax; return val; }

//! �������������, �������� limMin - ���������, ������� �������� �������������� �� limMax - 1, ����� ������� limMax �� ������ � ��������
inline int32 cmtIWrap( int32 val, int32 limMin, int32 limMax ) { if( val < limMin ) return limMax - 1; if( val >= limMax ) return limMin; return val; }
#else
#define cmtMin( i1, i2 ) ((i1) < (i2) ? (i1) : (i2))
#define cmtMax( i1, i2 ) ((i1) > (i2) ? (i1) : (i2))
#define cmtAbs( i1 )     ((i1) < 0 ? -(i1) : (i1))
#endif

  /**
 * @}
 */
