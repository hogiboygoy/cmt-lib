//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_STRING_H
#define CMT_STRING_H

class CmtString {
    static char mNullStr[1];
    static int  mNullRef;

    int    *mRef;
    cpchar  mBuf;

    char* Alloc( int count );

    void AddRef() { mRef[0]++; }
    
    static void DecRef( int *ref ) { ref[0]--; if( ref[0] == 0 ) delete ref; }
  public:
    CmtString();
    
    CmtString( cpchar str );
    
    CmtString( const CmtString &str );
    
    ~CmtString();
    
    cpchar    Ascii() const { return mBuf; }

    void      Clear();

    bool      IsEmpty() const { return mBuf[0] == 0; }
    
    bool      operator == ( const CmtString &str ) const;
    
    bool      operator != ( const CmtString &str ) const;
    
    CmtString operator += ( const CmtString &str );
    
    CmtString Copy() const;
    
    //CmtString operator = ( CmtString &str );
    void      Set( const CmtString &str );
    
    void      Set( cpchar str );

    void      Add( const CmtString &str );
    //pchar  GetBuffer() { return mBuffer; }
    //void   CheckExtension( cpchar ext );
    //cpchar GetName() { return FindName(); }
    
    CmtString GetExtension() const;
    //void   CutPath();      //�������� ������ ����
    //void   CutTop();       //�������� ����� ������� �������
    //void   CutName();      //�������� ��� � ����������� (��� ����)
    //void   CutNamePart();  //�������� ��� ��� ����������
    //void   CreateBin( cpchar base, cpchar fname ) { Set( base ); Add( fname ); }
  };

#define CMT_MAX_PATH 256

class CmtPath {
    char mBuffer[CMT_MAX_PATH];
  public:
    CmtPath() { mBuffer[0] = 0; }
    
    CmtPath( cpchar str ) { Set(str); }
    
    CmtPath( const CmtString &str ) { Set( str.Ascii() ); }
    
    CmtPath( const CmtPath &path ) { Set( path.Buf() ); }
    
    cpchar Buf() const { return mBuffer; }
    
    int    Lenght() const { return strlen(mBuffer); }
    
    bool   IsEmpty() const { return mBuffer[0] == 0; }
    
    CmtPath& Add( cpchar str );
    
    CmtPath& Add( const CmtString &str ) { return Add( str.Ascii() ); }
    
    CmtPath& Add( const CmtPath &path ) { return Add( path.Buf() ); }
    
    CmtPath& AddDir( cpchar str );
    
    CmtPath& Set( cpchar str );
    
    int      Compare( cpchar str, bool caseSensivity = true, int len = -1 ) const;

    int      Compare( const CmtString &str, bool caseSensivity = true, int len = -1 ) const { return Compare( str.Ascii(), caseSensivity, len ); }
    
    int      Compare( const CmtPath &path, bool caseSensivity = true, int len = -1 ) const { return Compare( path.Buf(), caseSensivity, len ); }
    
    bool     CmpEq( cpchar str, bool caseSensivity = true, int len = -1 ) const;
    
    bool     CmpEq( const CmtString &str, bool caseSensivity = true, int len = -1 ) const { return CmpEq( str.Ascii(), caseSensivity, len ); }
    
    bool     CmpEq( const CmtPath &path, bool caseSensivity = true, int len = -1 ) const { return CmpEq( path.Buf(), caseSensivity, len ); }
    
    cpchar   GetExtension() const;
    
    cpchar   GetName() const; //���������� ������ ��� (��� ����)
    
    void     SetExtension( cpchar ext );
    
    void     CutExtension(); //�������� ������ ����������
    
    void     CutPath();      //�������� ������ ����
    
    void     CutTop();       //�������� ����� ������� �������
    
    void     CutName();      //�������� ��� � ����������� (��� ����)
    
    void     CutNamePart();  //�������� ��� ��� ����������
    
    CmtPath& Arg( cpchar str );
    
    CmtPath& Arg( const CmtString &str ) { return Arg( str.Ascii() ); }
    
    CmtPath& Arg( const CmtPath &path ) { return Arg( path.Buf() ); }
    
    CmtPath& Arg( char ch );
    
    CmtPath& Arg( uint8 val, uint_8 width = 0, uint_8 base = 10, char padding = ' ' );

    CmtPath& Arg( int8 val, uint_8 width = 0, uint_8 base = 10, char padding = ' ' );

    CmtPath& Arg( uint16 val, uint_8 width = 0, uint_8 base = 10, char padding = ' ' );

    CmtPath& Arg( int16 val, uint_8 width = 0, uint_8 base = 10, char padding = ' ' );

    CmtPath& Arg( uint32 val, uint_8 width = 0, uint_8 base = 10, char padding = ' ' );

    CmtPath& Arg( int32 val, uint_8 width = 0, uint_8 base = 10, char padding = ' ' );
    
//    CmtPath& Arg( uint64 val, uint_8 width = 0, uint_8 base = 10, char padding = ' ' );
    
    void     ToLower();
    
    void     ToUpper();
    
    void     Write( CmtFile &os ) { os.Write( mBuffer, CMT_MAX_PATH ); }
    
    void     Read( CmtFile &is ) { is.Read( mBuffer, CMT_MAX_PATH ); mBuffer[CMT_MAX_PATH-1] = 0; }
    
  private:
    pchar    FindName();
    
    pchar    FindExtension();
    
    void     ReplaceArgIndex( cpchar buf );
    
    static const char digits[];
  };

//������ ���������� �������

//! ������������ 3-� ���������� ��������
/*! � ������ dest ��������� ������ ���� 56.91 ���
  \param dest ������, � ������� ����������� ������
  \param integer �������� ����� �����
  \param portion �������� ������� �����
  \param text ������� ���������� �������� */
void CmtPrintPhisItemHundredth( CmtPath &dest, uint32 integer, uint32 portion, cpchar text );

//! ������������ 3-� ���������� ��������
/*! � ������ dest ��������� ������ ���� 56.91 ���
  \param dest ������, � ������� ����������� ������
  \param val �������� � ����� �����
  \param text ������� ���������� �������� */
void CmtPrintPhisItemHundredth( CmtPath &dest, uint32 val, cpchar text );

//! ������������ 3-� ���������� ��������
/*! � ������ dest ��������� ������ ���� 56.9 ���
  \param dest ������, � ������� ����������� ������
  \param integer �������� ����� �����
  \param portion �������� ������� �����
  \param text ������� ���������� �������� */
void CmtPrintPhisItemTenth( CmtPath &dest, uint32 integer, uint32 portion, cpchar text );

//! ������������ 3-� ���������� ��������
/*! � ������ dest ��������� ������ ���� 56.9 ���
  \param dest ������, � ������� ����������� ������
  \param val �������� � ������� �����
  \param text ������� ���������� �������� */
void CmtPrintPhisItemTenth( CmtPath &dest, uint32 val, cpchar text );

//! ������������ 3-� ���������� ��������
/*! � ������ dest ��������� ������ ���� 56 ���
  \param dest ������, � ������� ����������� ������
  \param integer �������� ����� �����
  \param text ������� ���������� �������� */
void CmtPrintPhisItemOnes( CmtPath &dest, uint32 integer, cpchar text );

//! ������������ 3-� ���������� ��������
/*! � ������ dest ����������� ������ ���� 56.45 ��� ��� 56.4 ��� ��� 56 ���
  \param dest ������, � ������� ����������� ������
  \param val �������� � �������� �����
  \param text ������� ���������� �������� */
void CmtPrintPhisDecadaAdaptive( CmtPath &dest, uint32 val, cpchar text );

//! ������������ ���������� �������� � ������
/*! � ������ dest ����������� ������ ���� 56.45 ��� ��� 56.4 ��� ��� 56 ���
  \param dest ������, � ������� ����������� ������
  \param val �������� � ������� ��������
  \param power3 ������� �������� ������ �� 3 ������� (��)
  \param power6 ������� �������� ������ �� 6 ������� (���)
  \param power9 ������� �������� ������ �� 9 ������� (���)
  \param power12 ������� �������� ������ �� 12 ������� (���) */
void CmtPrintPhis( CmtPath &dest, uint32 val, cpchar power3, cpchar power6, cpchar power9, cpchar power12 );

//! ������������ � ������ ���������� �������� � ��
CmtPath& CmtPrintPhisHz( CmtPath &dest, uint32 val );

//! ������������ � ������ ���������� �������� � ��
CmtPath& CmtPrintPhisNs( CmtPath &dest, uint32 val );

//! ������������ � ������ ���������� �������� � ��
CmtPath& CmtPrintPhisMm( CmtPath &dest, uint32 val );

//�������� �������� �� ������ ���� (��������� ���������� ������)
inline cpchar tr( cpchar msg, cpchar remark = 0 ) { return msg; }

#endif
