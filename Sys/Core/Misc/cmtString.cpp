//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Core/Misc/cmtString.h>

char CmtString::mNullStr[1] = { 0 };
int  CmtString::mNullRef = 2;

CmtString 
CmtString::Copy() const {
  CmtString str;
  DecRef( str.mRef );
  char *ptr = str.Alloc( strlen(mBuf) );
  strcpy( ptr, mBuf );
  return str;
  }

char*
CmtString::Alloc(int count) {
  count = (count + 1 + (sizeof(int) - 1)) / sizeof(int);
  mRef = new int[count + 1];
  mBuf = (cpchar)(&mRef[1]);
  mRef[0] = 1;
  return (char*)(&mRef[1]);
  }

CmtString::CmtString() {
  mRef = &mNullRef;
  mBuf = mNullStr;
  AddRef();
  }

CmtString::CmtString(cpchar str) {
  mRef = &mNullRef;
  if( str ) mBuf = str;
  else      mBuf = mNullStr;
  AddRef();
  }

void      
CmtString::Set( cpchar str ) {
  DecRef( mRef );
  mRef = &mNullRef;
  if( str ) mBuf = str;
  else      mBuf = mNullStr;
  AddRef();
  }

CmtString::CmtString( const CmtString &str) {
  mBuf = str.mBuf;
  mRef = str.mRef;
  AddRef();
  }

void 
CmtString::Set( const CmtString &str ) {
  DecRef( mRef );
  mBuf = str.mBuf;
  mRef = str.mRef;
  AddRef();
  }

CmtString::~CmtString() {
  DecRef( mRef );
  }

void
CmtString::Clear() {
  DecRef( mRef );
  mRef = &mNullRef;
  mBuf = mNullStr;
  AddRef();
  }

bool
CmtString::operator ==( const CmtString &str ) const {
  return mBuf == str.mBuf || strcmp(mBuf, str.mBuf) == 0;
  }

bool
CmtString::operator !=( const CmtString &str ) const {
  return mBuf != str.mBuf && strcmp( mBuf, str.mBuf ) != 0;
  }

CmtString
CmtString::operator +=( const CmtString &str ) {
  Add( str );
  return *this;
  }

void
CmtString::Add( const CmtString &str) {
  if( IsEmpty() ) {
    DecRef( mRef );
    mBuf = str.mBuf;
    mRef = str.mRef;
    AddRef();
    }
  else if( !str.IsEmpty() ) {
    //Произвести конкатенацию
    int *ref = mRef;
    cpchar  buf = mBuf;
    char *dst = Alloc( strlen(buf) + strlen(str.mBuf) );
    strcpy( dst, buf );
    strcat( dst, str.mBuf );
    DecRef( ref );
    }
  }

CmtString
CmtString::GetExtension() const {
  CmtString str( *this );
  str.mBuf = cmtStrFindBack( str.mBuf, '.' );
  return str;
  }

