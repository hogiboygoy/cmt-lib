//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#ifndef CMT_CONTEXT_H
#define CMT_CONTEXT_H

//==============================================================================
//-------------------- ����� ---------------------------------------------------
#define ARGB32(a,r,g,b)  ((a<<24)|b|(g<<8)|(r<<16))
#define MRGB32(r,g,b)    ARGB32(0xff,r,g,b)
#define TRANSPARENT32    ARGB32(0,0,0,0)     //���������� ����
#define MC_BLACK32       MRGB32(0,0,0)       //������ ����
#define MC_WHITE32       MRGB32(255,255,255) //����� ����

#define CMT_R(color) (((color) >> 16) & 0xff)
#define CMT_G(color) (((color) >> 8) & 0xff)
#define CMT_B(color) (((color) >> 0) & 0xff)
#define CMT_A(color) (((color) >> 24) & 0xff)

//�������������� ������� �����
uint32    cmtLight( uint32 );
uint32    cmtShadow( uint32 );
uint32    cmtGradyUp( uint32 );

//�������������� ������ ����������
uint32    cmtOneLight( uint32 );
uint32    cmtOneShadow( uint32 );
uint32    cmtOneGradyUp( uint32 );

//! ���� ����������� ����� �����������
#define CMT_CC_COPY        0 //�������� �����������
#define CMT_CC_FOREGROUND  1 //���������� ������ �������� ���
#define CMT_CC_TRANSPARENT 2 //����������

//! ��������� ������� ����������� Image
#define CMT_OPC_COPY        0
#define CMT_OPC_TRANSPARENT 1
#define CMT_OPC_OR          2
#define CMT_OPC_AND         3

//! ������� ������� ��������
#define CMT_FLT_VERT        0 //!< ��������� ������ ����
#define CMT_FLT_VERT_GRADY  8 //!< ������������ �������� ������ ����

#ifdef CMT_FONT
//==============================================================================
//-------------------- ������ --------------------------------------------------
struct CmtFontCharW {
  int mIndex; //!< ������ ������ ������� � ������� �������
  int mX;     //!< ������ ������� �� X
  int mY;     //!< ������ ������� �� Y (16 ������� ��� ������, 16 ������� - �������� ���� ������� ����� (����� ���� �))
  };

struct CmtFontW {
  int           mFontSize;       //!< ������ ������ � ��������
  bool          mBold;           //!< ���� ��������
  bool          mItalic;         //!< ���� �������
  int           mDeltaX;         //!< ���������� ����� ���������
  int           mDeltaY;         //!< ���������� ����� ��������
  int           mSpaceX;         //!< �������� �������
  const uint32 *mImages;         //!< ������ ��������
  CmtFontCharW  mCharTable[256]; //!< ������� ���������� ��������
  };

//���� �������
#define FT_TIMES   0
#define FT_COURIER 1

#endif

//�������� ���������
struct CmtEdge;

struct CmtContextInfo {
    uint32   *mData;     //!< ������� ���������
    CmtPoint  mSize;     //!< ������ ������� ���������
    CmtPoint  mClipSize; //!< ������ ������� ���������
  };

class CmtContext {
    bool      mOwner;    //!< ������, ����� ����������� ������� ���������
  protected:
    uint32   *mData;     //!< ������� ���������
    CmtPoint  mSize;     //!< ������ ������� ���������
    CmtPoint  mClipSize; //!< ������ ������� ���������
    CmtPoint  mPos;      //!< ������� ��������� ����
#ifdef CMT_FONT
    CmtFontW *mFont;     //!< �����
    int32     mFontDir;  //!< ���� �������� ������
#endif
    void     AddEdge( CmtPoint a, CmtPoint b );
  public:
    CmtContext( CmtPoint size, uint32 *src );
    
    CmtContext( CmtContext *src, CmtPoint pos );
    
    ~CmtContext();
    
            void     Setup( CmtPoint size, uint32 *src ) { if( mOwner && mData) delete mData; mSize = size; mClipSize = size; mData = src; mOwner = false; }
            void     SetSize( CmtPoint size );
            
            bool     Push( CmtContextInfo &info, CmtPoint pos, CmtPoint size, CmtContext *src );
            
            void     Pop( CmtContextInfo &info );
    
    //�����
            CmtPoint GetSize() { return mSize; }
            CmtPoint GetClipSize() { return mClipSize; }
            bool     CheckBound( int32 x, int32 y ) { return !(x < 0 || x >= mClipSize.x || y < 0 || y >= mClipSize.y); }

            void     Clip( CmtPoint &p );                  //�������������� ����� � ������ ���������
            int32    ClipCode( CmtPoint p );               //��� ���������
            bool     ClipLine( CmtPoint &a, CmtPoint &b ); //��������� �����
    //����� � �������� �������
    virtual void     LineTo( CmtPoint to, uint32 color, bool set = true ); //����� �� ����� �� �������
    virtual void     ThickLineTo( CmtPoint to, uint32 thickness, uint32 color, bool set = true ); //����� �������� �������
    virtual void     Circle( CmtPoint center, int32 radius, uint32 color, bool set = true );
    virtual void     PixelCircle( CmtPoint center, int32 x, int32 y, uint32 color, bool set = true );
    virtual void     MoveTo( CmtPoint to ) { mPos = to; }
    virtual void     Line( CmtPoint a, CmtPoint b, uint32 color, bool set = true );
    virtual void     ThickLine( CmtPoint a, CmtPoint b, uint32 thickness, uint32 color, bool set = true ); //����� �������� �������
    virtual void     Rect( CmtPoint a, CmtPoint b, uint32 color, bool set = true );

    //������������
//    virtual void     TranspLineAdd( CmtPoint a, CmtPoint b, int32 color );
//    virtual void     TranspLineSet( CmtPoint a, CmtPoint b, uint32 color );

//    virtual void     TranspCircleAdd( CmtPoint center, int32 radius, int32 color );
//    virtual void     TranspCircleSet( CmtPoint center, int32 radius, uint32 color );

//    virtual void     TranspFilledRectAdd( CmtPoint a, CmtPoint b, int32 color );
//    virtual void     TranspFilledRectSet( CmtPoint a, CmtPoint b, uint32 color );
            
    //����������� �����
    virtual void     HorzLine( int32 x1, int32 x2, int32 y, uint32 color, bool set = true );
    virtual void     VertLine( int32 x, int32 y1, int32 y2, uint32 color, bool set = true );
    virtual void     FilledRect( CmtPoint a, CmtPoint b, uint32 color, bool set = true );
    virtual void     FilledCircle( CmtPoint center, int32 radius, uint32 color, bool set = true );
    virtual void     FillPixelCircle( CmtPoint center, int32 x, int32 y, uint32 color, bool set = true );
    virtual void     Clear( uint32 color, bool set = true );
    virtual void     FilledLine( CmtPoint a, CmtPoint size, uint32 *colors, bool horz = true, bool set = true );
#ifdef CMT_GR_SIMPLE_POLYGON
    virtual void     SimpleFill( CmtPoint p1, CmtPoint p2, CmtPoint p3, CmtPoint p4, uint32 color, bool set = true );
    virtual void     SimpleFillCore( CmtEdge *simpleEdge, int32 simpleEdgeCount, uint32 color, bool set = true );
#endif
#ifdef CMT_GR_POLYGON
    virtual void     Polygon( CmtPoint table[], int32 count, uint32 color, bool set = true );
    virtual void     InitPoly();
    virtual void     AddPolyVertex( CmtPoint table[], int32 count );
#endif
            
#ifdef CMT_FONT            
    //�����
            void     SetFont( uint32 size, bool bold = false, bool italic = false, uint32 fontType = FT_TIMES );
            void     SetFontDir( int32 dir = 0 ) { mFontDir = dir; } //���������� ���� �������� ������
    //�����
            void     Text( uint32 color, CmtPoint p, const char *str, uint32 len = 0xffffffff, bool set = true );
            CmtPoint GetTextExtent( const char *str, uint32 len = 0xffffffff );
            CmtPoint GetWrapTextExtent( const char *str, int width, uint32 len = 0xffffffff );
            //! �������� ����� �������� ������ � ������������� � �������������� � ������� a,b (b - �����, � �� ������)
            void     HText( uint32 color, cpchar str, CmtPoint a, CmtPoint b, int32 align, bool set = true );
            
            //! �������� ����� �������� ������ � ������������� � �������������� � � �������������� ��������� �� ������� ����
            void     RText( uint32 color, cpchar str, CmtPoint a, CmtPoint b, int32 align, bool set = true );
#endif

    //�������
    virtual uint32   GetPixel( CmtPoint p ); //�������� ������ � ��������� ������
    virtual void     SetPixel( CmtPoint p, uint32 color, bool set = true ); //�������� ���� ������� � ��������� ������ � � ������ ������������
    virtual void     AddPixel( CmtPoint p, uint32 color, int level ); //�������� ���� � �������� ����� (� 1/256 ��������)

#ifdef CMT_IMAGE            
    //Image
    virtual void     Image( CmtPoint p, CmtImage *image, int32 frame, bool set = true );
    virtual void     HImage( CmtImage *image, int32 frame, CmtPoint a, CmtPoint b, int32 align, CmtPoint *lt = 0, CmtPoint *rb = 0, bool set = true );
    virtual void     ImageBorder( CmtImage *image, int32 frame, CmtPoint pos, CmtPoint size, bool filled = true, CmtPoint center = CmtPoint(-1,-1), bool set = true );
#endif
    const   uint32*  ClipRect( CmtPoint &pos, CmtPoint &size, const uint32 *srcData, int32 srcSizeX );
            
            //! ���������� � ������ �������� ������� ��������
            /*! \param src �������� ��������
              \param pos ����� ���������� ���������
              \param size ������ ���������� ��������� ��������� 
              \param set ��� �����������: true - ������� �����������, false - ���������� */
    virtual void     Copy( CmtContext *src, CmtPoint pos, CmtPoint size, bool set = true );
            
            //! ���������� � ������ �������� ������������� ������� �� �����
            /*! ������������ ����������� �� ����� ��������������� ������������� �������� � 
              ������ �������� � �������� �����. ��� ����������� �
              ��������� ������ ���� ���������� ���������� ����������!
              \param size ������ ������� �����������
              \param srcData ������ ������� ����������� (��� �� ������ �����!)
              \param srcSizeX ������ ����� �� X
              \param dest ����� ���������� ��� �����������
              \param set ��� �����������: true - ������� �����������, false - ���������� */
    virtual void     CopyRect( CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest, bool set = true );
            
            //! ���������� ����������� � ������ �������� ������������� ������� �� �����
            /*! ���������� ����������� ��������� �������� ����������� � ��������� � ��������
              ����������� �����.
              \param size ������ ������� �����������
              \param srcData ������ ������� ����������� (��� �� ������ �����!)
              \param srcSizeX ������ ����� �� X
              \param dest ����� ���������� ��� �����������
              \param set ��� �����������: true - ������� �����������, false - ���������� */
            void     CopyRectSafe( CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest, bool set = true );
            
            //! ���������� � ������ �������� ������������� ������� �� ����� � ������ ����� ������������
            /*! ������������ ����������� �� ����� ��������������� ������������� �������� � 
              ������ �������� � �������� ����� � ������ ����� ������������. ��� ����������� �
              ��������� ������ ���� ���������� ���������� ����������!
              \param transpColor ����, ����������� ����������
              \param size ������ ������� �����������
              \param srcData ������ ������� ����������� (��� �� ������ �����!)
              \param srcSizeX ������ ����� �� X
              \param dest ����� ���������� ��� ����������� */
    virtual void     CopyTranspRect( uint32 transpColor, CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest );
            
            void     CopyTranspRectSafe(uint32 transpColor, CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest);
            
    virtual void     AddRect( int transpFactor, CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest );
            
            void     AddRectSafe(int transpFactor, CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest);
            
   // virtual CmtContext* CreateCopy( CmtPoint pos, CmtPoint size, uint32 *data = 0 );
            
            //! �������� ���������� ���������� ��������
    virtual void     WaitAccel() {}

    virtual uint32  *GetData() { return mData; }
    virtual uint32  *GetData( CmtPoint p );

#ifdef CMT_FONT            
  static    CmtPoint GetTextExtent( CmtFontW *font, const char *str, uint32 len = 0xffffffff );
  static    CmtPoint GetWrapTextExtent( CmtFontW *font, const char *str, int width, uint32 len = 0xffffffff );
  static    int      GetFilledLen( CmtFontW *font, cpchar str, int width );
  static    CmtFontW *GetFont( uint32 size, bool bold, bool italic, uint32 fontType );
#endif
  };

//! ����� ��� ����������� ����������� ����������
/*!
  */
class CmtScreen : public CmtContext {
public:
  CmtScreen( CmtPoint size, uint32 *src ) : CmtContext( size, src ) {}
  
  //! ���������������� �����
  virtual void Init() {}
  
  //! ���������� ����� ������
  /*! \param mode ����� ����� ������ ������������ ����������� */
  virtual void SetMode( int mode ) {}

  //! ��������� ��������� ����� ���������� ����� ������
  virtual void FrameComplete() {}

  //! ������� ���������� ����������� ���������� ������  
  virtual void WaitEndOfFrame() {}
  };



//��������
//�������� ������������
#define MP_CENTER              0
#define MP_LEFT                1
#define MP_RIGHT               2
#define MP_HORZ_MASK           3

#define MP_MIDDLE              0
#define MP_TOP                 4
#define MP_BOTTOM              8
#define MP_VERT_MASK      0x000c

#define MP_ALIGN_HORZ     0x0010
#define MP_ALIGN_VERT     0x0020
#define MP_SIZE_HORZ      0x0040
#define MP_SIZE_VERT      0x0080

//�������� ������������ ������ �� ��������������
#define MP_TEXT                0
#define MP_TLEFT               (MP_LEFT      << MP_TEXT)
#define MP_TCENTER             (MP_CENTER    << MP_TEXT)
#define MP_TRIGHT              (MP_RIGHT     << MP_TEXT)
#define MP_THORZ_MASK          (MP_HORZ_MASK << MP_TEXT)

#define MP_TTOP                (MP_TOP       << MP_TEXT)
#define MP_TMIDDLE             (MP_MIDDLE    << MP_TEXT)
#define MP_TBOTTOM             (MP_BOTTOM    << MP_TEXT)
#define MP_TVERT_MASK          (MP_VERT_MASK << MP_TEXT)

//�������� ����������� �������� �� ��������������
#define MP_PICT                4
#define MP_PLEFT               (MP_LEFT      << MP_PICT)
#define MP_PCENTER             (MP_CENTER    << MP_PICT)
#define MP_PRIGHT              (MP_RIGHT     << MP_PICT)
#define MP_PHORZ_MASK          (MP_HORZ_MASK << MP_PICT)

#define MP_PTOP                (MP_TOP       << MP_PICT)
#define MP_PMIDDLE             (MP_MIDDLE    << MP_PICT)
#define MP_PBOTTOM             (MP_BOTTOM    << MP_PICT)
#define MP_PVERT_MASK          (MP_VERT_MASK << MP_PICT)

#define MP_WRAP_TEXT           0x00000100 //������������� ������������ ����� �� ������� ����
#define MP_OVER_TEXT           0x00000200 //����� ������ ��������

//������� ���������

//! �������� �������� ������ �� �������� ������ � ������� ����� �����
/*! ������ �������� ������ � ����������� ��������, ������ � ���� ������
  \param ctx �������� ������
  \param pos ������� ���� ������
  \param size ������ ���� ������
  \param color ���� �������
  \param offset �������� ������ (� ������ ������)
  \param scale ������� ����������� (� ���������� �������� �� 16 ����� ������)
  \param values ������ ��������
  \param count ���������� �������� */
void CmtDrawLinearGraphic( CmtContext *ctx, CmtPoint pos, CmtPoint size, uint32 color, CmtPoint offset, CmtPoint scale, int32 *values, int count );
//���������� ����������� ���������
#ifdef CMT_TI_OMAP3530
  #include <Sys/Gui/cmtContextTiOmap3530.h>
#endif

#endif
