//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Core/Misc/cmtString.h>

const char
CmtPath::digits[44] = { '0', '1', '2', '3', '4', '5', '6', '7', 
                        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
                        'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 
                        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                        'W', 'X', 'Y', 'Z', '$', '&', '#', '_',
                        '~', '^', '%', '-' };

CmtPath&
CmtPath::Set( cpchar src ) {
  if( src == 0 ) {
    mBuffer[0] = 0;
    }
  else {
    int len = (int)strlen( src );
    len = cmtIMin( len, CMT_MAX_PATH - 1 );
    memcpy( mBuffer, src, len );
    mBuffer[len] = 0;
    }
  return *this;
  }

CmtPath&
CmtPath::Add( cpchar str ) {
  if( str != 0 ) {
    int len = (int)strlen(mBuffer);
    int alen = (int)strlen(str);
    if( len + alen > CMT_MAX_PATH - 1 )
      alen = CMT_MAX_PATH - 1 - len;
    if( alen > 0 ) {
      memcpy( mBuffer + len, str, alen );
      mBuffer[ len + alen ] = 0;
      }
    }
  return *this;
  }

CmtPath&
CmtPath::AddDir( cpchar str ) {
  Add( str );
  return Add( "\\" );
  }

int
CmtPath::Compare( cpchar str, bool caseSensivity, int len ) const {
  if( len < 0 ) len = Lenght();
  if( len > CMT_MAX_PATH ) len = CMT_MAX_PATH;
  if( caseSensivity ) {
    for( int i = 0; i < len; ++i ) {
      if( (uint8)(mBuffer[i]) < (uint8)(str[i]) ) return -1;
      else if( (uint8)(mBuffer[i]) > (uint8)(str[i]) ) return 1;
      }
    return 0;
    }
  for( int i = 0; i < len; ++i ) {
    uint8 ch1 = cmtCharLower(mBuffer[i]);
    uint8 ch2 = cmtCharLower(str[i]);
    if( ch1 < ch2 ) return -1;
    if( ch1 > ch2 ) return 1;
    }
  return 0;
  }

bool
CmtPath::CmpEq(cpchar str, bool caseSensivity, int len) const {
  if( len < 0 ) len = Lenght();
  if( len > CMT_MAX_PATH ) len = CMT_MAX_PATH;
  if( caseSensivity ) {
    for( int i = 0; i < len; ++i )
      if( mBuffer[i] != str[i] ) return false;
    return true;
    }
  for( int i = 0; i < len; ++i ) {
    if( cmtCharLower( mBuffer[i] ) != cmtCharLower( str[i] ) ) return false;
    }
  return true;
  }

void
CmtPath::SetExtension( cpchar ext ) {
  pchar lpTmp = cmtStrFindBack( mBuffer, '.' );
  if( lpTmp ) *lpTmp = 0;
  int pos = strlen( mBuffer );
  if( pos >= CMT_MAX_PATH - 1 ) return;
  if( ext[0] != '.' ) mBuffer[pos++] = '.';
  for( int i = 0; ext[i] && pos < CMT_MAX_PATH - 1; ++i )
    mBuffer[pos++] = ext[i];
  mBuffer[pos] = 0;
  }

pchar
CmtPath::FindName() {
  pchar ptr;
  for( ptr = mBuffer + strlen(mBuffer); ptr != mBuffer && *ptr != '\\' && *ptr != '/'; --ptr);
  if( *ptr == '\\' ) return ptr + 1;
  return ptr;
  }

cpchar
CmtPath::GetName() const {
  cpchar ptr;
  for( ptr = mBuffer + strlen(mBuffer); ptr != mBuffer && *ptr != '\\' && *ptr != '/'; --ptr);
  if( *ptr == '\\' ) return ptr + 1;
  return ptr;
  }

void
CmtPath::CutPath() {
  pchar ptr = FindName();
  *ptr = 0;
  }

void
CmtPath::CutTop() {
  pchar ptr = mBuffer + strlen(mBuffer);
  if( ptr == mBuffer ) return;
  ptr--;
  if( ptr[0] == '\\' || ptr[0] == '/' ) ptr--;
  if( ptr[0] == ':' ) return; //Корневой каталог
  for( ; ptr != mBuffer && *ptr != '\\' && *ptr != '/'; --ptr);
  if( *ptr == '\\' ) ptr[1] = 0;
  else ptr[0] = 0;
  }

pchar
CmtPath::FindExtension() {
  pchar ptr;
  for( ptr = mBuffer + strlen(mBuffer); ptr != mBuffer && *ptr != '\\' && *ptr != '/' && *ptr != '.'; --ptr);
  if( *ptr == '.' ) return ptr;
  return ptr + strlen(mBuffer);
  }

void
CmtPath::CutExtension() {
  Set( FindExtension() );
  }

void
CmtPath::CutName() {
  Set( FindName() );
  }

void
CmtPath::CutNamePart() {
  *FindExtension() = 0;
  }
  
CmtPath&
CmtPath::Arg(char ch) {
  char tmp[2];
  tmp[0] = ch;
  tmp[1] = 0;
  ReplaceArgIndex( tmp );
  return *this;
  }

CmtPath&
CmtPath::Arg(cpchar str) {
  if( str ) ReplaceArgIndex( str );
  else {
    char tmp[2];
    tmp[0] = 0;
    ReplaceArgIndex( tmp );
    }
  return *this;
  }

CmtPath&
CmtPath::Arg( int32 val, uint_8 width, uint_8 base, char padding ) {
  char tmp[34];
  uint_8 i = 33;
  tmp[i] = 0;
  bool negSign = false;
  if( val < 0 ) {
    negSign = true;
    val = -val;
    }
  while( val ) {
    tmp[--i] = digits[val % base];
    val /= base;
    }
  if( i == 33 ) tmp[--i] = '0';
  if( padding == ' ' && negSign ) tmp[--i] = '-';
  while( 33 - i < width ) tmp[--i] = padding;
  if( padding == '0' && negSign ) tmp[--i] = '-';
  ReplaceArgIndex( tmp + i );
  return *this;
  }

CmtPath&
CmtPath::Arg(int8 val, uint_8 width, uint_8 base, char padding) {
  return Arg( (int32)val, width, base, padding );
  }

CmtPath&
CmtPath::Arg(int16 val, uint_8 width, uint_8 base, char padding) {
  return Arg( (int32)val, width, base, padding );
  }

CmtPath&
CmtPath::Arg( uint32 val, uint_8 width, uint_8 base, char padding ) {
  char tmp[34];
  uint_8 i = 33;
  tmp[i] = 0;
  while( val ) {
    tmp[--i] = digits[val % base];
    val /= base;
    }
  if( i == 33 ) tmp[--i] = '0';
  while( 33 - i < width && i ) tmp[--i] = padding;
  ReplaceArgIndex( tmp + i );
  return *this;
  }

#if 0
CmtPath& 
CmtPath::Arg( uint64 val, uint_8 width, uint_8 base, char padding ) {
  char tmp[66];
  uint_8 i = 65;
  tmp[i] = 0;
  while( val ) {
    tmp[--i] = digits[ val % base ];
    val /= base;
    }
  if( i == 65 ) tmp[--i] = '0';
  while( 65 - i < width && i ) tmp[--i] = padding;
  ReplaceArgIndex( tmp + i );
  return *this;
  }
#endif
  
CmtPath&
CmtPath::Arg( uint8 val, uint_8 width, uint_8 base, char padding ) {
  return Arg( (uint32)val, width, base, padding );
  }

CmtPath&
CmtPath::Arg( uint16 val, uint_8 width, uint_8 base, char padding ) {
  return Arg( (uint32)val, width, base, padding );
  }

void
CmtPath::ReplaceArgIndex( cpchar buf ) {
  char index = 0x7f;
  for( int i = 0; mBuffer[i]; ++i )
    if( mBuffer[i] == '%' && index > mBuffer[i+1] && mBuffer[i+1] >= '0' ) index = mBuffer[i+1];
  if( index == 0x7f ) return;
  char tmp[CMT_MAX_PATH];
  int k = 0;
  for( int i = 0; mBuffer[i] && k < CMT_MAX_PATH - 1; ++i )
    if( mBuffer[i] == '%' && mBuffer[i+1] == index ) {
      //Заменить
      for( int t = 0; buf[t] && k < CMT_MAX_PATH - 1; ++t )
        tmp[k++] = buf[t];
      i++;
      }
    else tmp[k++] = mBuffer[i];
  tmp[k] = 0;
  Set(tmp);
  }

void
CmtPath::ToLower() {
  for( int i = 0; mBuffer[i]; ++i )
    mBuffer[i] = cmtCharLower( mBuffer[i] );
  }

void
CmtPath::ToUpper() {
  for( int i = 0; mBuffer[i]; ++i )
    mBuffer[i] = cmtCharUpper( mBuffer[i] );
  }
