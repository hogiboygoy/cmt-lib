//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
pchar 
cmtSPrintStr( pchar dest, cpchar src ) {
  if( src == 0 ) return dest;
  while( *src ) *dest++ = *src++;
  //Close string
  *dest = 0;
  return dest;
  }
  
pchar 
cmtSPrintHexUpper( pchar dest, uint32 value, uint_8 num ) {
  cmtMultiToHexUpper( value, dest, num );
  //Close string
  dest[num] = 0;
  return dest + num;
  }

pchar 
cmtSPrintUInt( pchar dest, uint32 value ) {
  uint32 dv = 1000000000;
  bool next = false;
  while( dv ) {
    if( value > dv || next ) {
      *dest++ = '0' + value / dv;
      value %= dv;
      next = true;
      }
    dv /= 10;
    if( dv == 1 ) next = true;
    }
  //Close string
  *dest = 0;
  return dest;
  }

void 
cmtOneToHexUpper( uint_8 dig, pchar dest ) {
  dig &= 0xf;
  if( dig > 9 ) dest[0] = 'A' - 10 + dig;
  else dest[0] = '0' + dig;
  }
  
void
cmtMultiToHexUpper( uint32 dig, pchar dest, uint_8 num ) {
  while( num ) {
    cmtOneToHexUpper( dig >> ((num - 1) * 4), dest++ );
    num--;
    }
  } 

int
cmtTransHex( char ch ) {
  if( ch > 0x60 ) ch &= 0xdf;
  ch -= '0';
  if( ch > 9 ) ch += (char)('0' - 'A' + 10);
  if( ch > 15 ) ch = 0;
  return ch;
  }

int
atoh( cpchar str ) {
  int res = 0;
  while( *str ) {
    res <<= 4;
    res += cmtTransHex( *str );
    str ++;
    }
  return res;
  }

uint8
cmtCharLower( uint8 c ) {
  if( c >= 'A' && c <= 'Z' ) return (uint8)(c - 'A' + 'a');
  if( c >= (uint8)'ј' && c <= (uint8)'я' ) return (uint8)(c - 'ј' + 'а');
  return c;
  }

uint8
cmtCharUpper( uint8 c ) {
  if( c >= 'a' && c <= 'z' ) return (uint8)(c - 'a' + 'A');
  if( c >= ((uint8)('а')) ) return (uint8)(c - 'а' + 'ј');
  return c;
  }

//—равнение символов без учета регистра
uint_8
cmtCompareChar( uint8 c1, uint8 c2 ) {
  return cmtCharLower( c1 ) == cmtCharLower( c2 );
  }

//—равнение строк без учета регистра
int    
cmtCompareStrNoReg( cpchar s1, cpchar s2 ) {
  if( s1 == 0 || s2 == 0 ) return 1;
  char c1,c2;
  while( *s1 != 0 || *s2 != 0 ) {
    c1 = cmtCharLower( *s1++ );
    c2 = cmtCharLower( *s2++ );
    if( c1 < c2 ) return -1;
    if( c1 > c2 ) return 1;
    }
  return 0;
  }

//! —равнение строки с образцом без учета регистра (образец может быть короче строки)
//! \param pat образец
//! \param str строка
//! \return true когда есть совпадение с образцом, false в противном случае
bool 
cmtComparePatNoReg( cpchar pat, cpchar src ) {
  while( *pat ) {
    if( !cmtCompareChar( *pat, *src ) ) return false;
    pat++;
    src++;
    }
  return true;
  }

//! ѕоиск подстроки в строке без учета регистра
/*! \param str строка, в которой производитс€ поиск
  \param subStr искома€ подстрока
  \return указатель на подстроку в строке или нуль, если подстроки не найдено */
cpchar 
cmtStrStrNoReg( cpchar src, cpchar subStr ) {
  while( *src ) {
    if( cmtComparePatNoReg( subStr, src ) ) return src;
    src++;
    }
  return 0;
  }

uint_8 
cmtStrNCmp( const uint8 *src1, fcpchar src2, uint_8 len ) {
  while( len-- ) {
    if( src1[0] != src2[0] ) return 1;
    src1++;
    src2++;
    }
  return 0;
  }

void
strmcpy( pchar dest, cpchar sour, uint32 maximum ) {
  maximum = cmtUMin( strlen(sour), maximum - 1 );
  strncpy( dest, sour, maximum );
  dest[maximum] = 0;
  }

void
cmtStrLower( pchar str ) {
  while( *str ) {
    *str = cmtCharLower( *str );
    str++;
    }
  }

void
cmtStrUpper( pchar str ) {
  while( *str ) {
    *str = cmtCharUpper( *str );
    str++;
    }
  }

pchar
cmtStrFindBack( pchar sour, char ch ) {
  if( !sour ) return 0;
  pchar p = sour + strlen(sour);
  while( p != sour )
    if( *p == ch ) return p;
    else p--;
  return 0;
  }

cpchar
cmtStrFindBack( cpchar sour, char ch ) {
  if( !sour ) return 0;
  cpchar p = sour + strlen(sour);
  while( p != sour )
    if( *p == ch ) return p;
    else p--;
  return 0;
  }
