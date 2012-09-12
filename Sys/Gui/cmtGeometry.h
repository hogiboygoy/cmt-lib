//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! ����� �� ���������
struct CmtPoint {
    int32 x; //!< ���������� X
    int32 y; //!< ���������� Y
  public:
    
    //! ����������� ����� � �������� ������������
    CmtPoint() : x(0), y(0) {}
    
    //! ����������� ����� � ��������� ������������
    /*! \param sx x-���������� �����
      \param sy y-���������� ����� */
    CmtPoint( int32 sx, int32 sy ) : x(sx), y(sy) {}

    //! ���������� ����� �������� ��������� �����
    /*! \param sx x-���������� �����
      \param sy y-���������� ����� */
    void     Set( int32 sx, int32 sy ) { x = sx; y = sy; }

    //! ������� ����� ������ ��������� ������
    /*! ���������� ���������� ����� ������ ��������� ������ �� �������� ����.
      �������� ����� �� ���������.
      \param center ����� ������ ��������
      \param angle ���� �������� � ������� ����� �������
      \return ���������� ����� */
    CmtPoint Rotate( CmtPoint center, double angle );
    
    //! ������� ����� � ��������� ��������������
    /*! ���������� ���������� ����� ������ ��������� ������ � ��������� ���� ������������� �������
      \param center ����� ������ ��������
      \param sin �������� ������ ���� ��������
      \param cos �������� �������� ���� ��������
      \param levelTable ��������� �� ������ �� ������� ���������, ������� �������� ��� ������� � ���� �������� ����� (������, ������ �����, �����)
      \return ���������� ����� */
    CmtPoint RotateL( CmtPoint center, double sin, double cos, int32 *levelTable );
    
    //! ��������� �����
    bool     operator == ( CmtPoint p )   { return x == p.x && y == p.y; }

    //! ��������� �����
    bool     operator != ( CmtPoint p )   { return x != p.x || y != p.y; }

    //! �������� ����� (�������� ������� �����)
    CmtPoint operator += ( int32 offset ) { x += offset; y += offset; return *this; }

    //! �������� ����� (�������� ������� �����)
    CmtPoint operator += ( CmtPoint p )   { x += p.x; y += p.y; return *this; }

    //! �������� ����� (�������� ������� �����)
    CmtPoint operator -= ( int32 offset ) { x -= offset; y -= offset; return *this; }

    //! �������� ����� (�������� ������� �����)
    CmtPoint operator -= ( CmtPoint p )   { x -= p.x; y -= p.y; return *this; }

    //! ��������������� ����� (�������� ������� �����)
    CmtPoint operator /= ( int32 div )    { x /= div; y /= div; return *this; }

    //! ��������������� ����� (�������� ������� �����)
    CmtPoint operator *= ( int32 mul )    { x *= mul; y *= mul; return *this; }

    //! �������� ���������� �����
    void     Ajust( CmtPoint origin, CmtPoint ajust, CmtPoint delta );
    
    //! �������� ��������� ����� � ������������� ����������
    void     Max( CmtPoint test );
    
    //! �������� ��������� ����� � ������������ ����������
    void     Min( CmtPoint test );
    
    //! ���������� ����� ������ ������ � ��������
    double   Distance( CmtPoint p );
    
    //! ���� �������� ������ ����� ������������ ������������� ������� ��������� � ������� � �������� �����
    double   GetAngle( CmtPoint center );
    
    static void GetSinCos( int angle, double *sinVal, double *cosVal ); //cmtGBase.cpp
    static bool IsOverRect( CmtPoint testPos, CmtPoint testSize, CmtPoint overPos, CmtPoint overSize );

  };


//! ������������� �� ��������� (������������� �� ���� ��������������� �����)
struct CmtRect {
    CmtPoint a; //!< ���������� ������� ������ ����
    CmtPoint b; //!< ���������� ������� ������� ����
  public:
    
    //! ����������� �� ��������� � ������ � ����� 0,0
    CmtRect() : a(), b() {}
    
    //! ����������� �������������� � ������ � �������� �����������
    CmtRect( int32 ax, int32 ay, int32 bx, int32 by ) : a(ax,ay), b(bx,by) {}

    //! ����������� �������������� � ������ � �������� ������
    CmtRect( CmtPoint a, CmtPoint b ) : a(a), b(b) {}
    
    //! ����������� �����������
    CmtRect( CmtRect &r ) : a(r.a), b(r.b) {}

    //! ������ ��������������
    int    Width() const { return b.x - a.x; }
    
    //! ������ ��������������
    int    Height() const { return b.y - b.y; }
    
    //! ���������� �������� �� ����� ������ ��������������
    bool   IsPointInside( CmtPoint p );
    
    //! ���������� ������ ������������� (� ������������ ������)
    bool   IsEmpty();
    
    //! ��������� ���������������
    bool   IsEqual( CmtRect &rect );
    
    //! ���������� ������������ �� ��������������
    bool   IsIntersect( const CmtRect &rect );
    
    //! ����������
    void   Ajust( CmtRect &origin, CmtRect &ajust, CmtPoint delta );
    
    //! ���������� ����� ���������� �����
    void   Set( CmtPoint sa, CmtPoint sb ) { a = sa; b = sb; }

    //! ���������� ���� ��������������
    CmtPoint LeftTop() const { return a; }

    //! ���������� ���� ��������������
    CmtPoint LeftBottom() const { return CmtPoint( a.x, b.y ); }

    //! ���������� ���� ��������������
    CmtPoint RightTop() const { return CmtPoint( b.x, a.y ); }

    //! ���������� ���� ��������������
    CmtPoint RightBottom() const { return b; }
  };

