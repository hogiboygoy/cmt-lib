//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#ifndef CMT_WIDJET_SYSTEM_H
#define CMT_WIDJET_SYSTEM_H


class CmtWidjet;

enum CmtEventType {
  cetNone = 0,
  cetPress,
  cetRelease,
  cetClick,
  cetLongPress,
  cetDublClick,
  cetChar,
  cetScroll,
  cetMenu,
  cetCode
  };

enum CmtVirtualKeyCode {
  //Scroll-���� ������ ��������������, �������� - ������������
  cvkScroll0  = 0, //��������������
  cvkScroll1,      //������������
  cvkScroll2,      //��������������
  cvkScroll3,      //������������
  cvkEnter,
  cvkEsc,
  cvkLeft,
  cvkRight,
  cvkUp,
  cvkDown,
  cvkHome,
  cvkEnd,
  cvkInsert,
  cvkDelete,
  cvkPgUp,
  cvkPgDown,
  cvkF0,
  cvkF1,
  cvkF2,
  cvkF3,
  cvkF4,
  cvkF5,
  cvkF6,
  cvkF7,
  cvkF8,
  cvkF9,
  cvkF10,
  cvkF11,
  cvkF12,
  cvkLeftCtrl,
  cvkRightCtrl,
  cvkLeftShift,
  cvkRightShift,
  cvkLeftAlt,
  cvkRightAlt,
  cvkSelect,
  cvkTitle,
  cvkMenu,
  cvkNext,
  cvkPrev,
  cvkPower,
  cvkPowerOff,
  
  cvkLast
  };

class CmtInputHandler : public CmtObject {
  public:
    
    //! ������� ������� (����������� ������������)
    /*! ��������� ������� �� ������� ������������.
      \param evType ��� ������� ��.CmtEventType
      \param evCode ��� ������� (��� ���������� ��.CmtVirtualKeyCode)
      \param pos ����� ������� � �������� ������������� �������
      \return ������, ����� ������� ����������, ���� ����� ������� �� ���������� � ������
      �������������� ����������� ������������.  */
    virtual bool Event( CmtEventType evType, int32 evCode, CmtPoint pos ) { return false; }
    
  };

//! ������������ �������� (�������)
class CmtAnimation : public CmtInputHandler {
  public:
    CmtAnimation() : CmtInputHandler() {}
    virtual ~CmtAnimation() {}
    
    //! ���� ��� �������� (�����)
    /*! ��������� ���� ��� ��������
      \param widjet ������, ��� ������� ������������ ��������, ����� ���� 0, ���� �������� �� ��������� � ������� */
    virtual void OnNextFrame( CmtWidjet *widjet ) = 0;
    
    //! �������������� ������� � �������������� ������������� ���������
    /*! ��������� �������������� ������� � �������������� ��������
      \param widjet ������, ������� ��������������
      \param dest ��������, � ������� ������������ ���������
      \param src �������� �������� */
    virtual void OnPaint( CmtWidjet *widjet, CmtContext *dest, CmtContext *src );
    
    //! ��������� ��������� ������������� ��������� ��� ������������� � �������
    /*! ���������� ��������� ��������� ������������� ��������� ��� ������������� � �������
      \param widjet ������, � �������� ������������ ����������� */
    virtual void OnSetup( CmtWidjet *widjet ) {}
    
  };

#define CMT_WF_DIRTY   MBIT0 //���� ���������� ������������� ����������� ����������� �������
#define CMT_WF_FOCUS   MBIT1 //���� ���������� ����������� ������� ��������� ����� �����
#define CMT_WF_VISIBLE MBIT2 //���� ���������� ��������� �������

class CmtWidjet : public CmtInputHandler {
  protected:
    CmtWidjet     *mNext;       //!< ��������� widjet � ������������ ������ �������� widjet-��
    CmtWidjet     *mParent;     //!< ������������ widjet
    CmtWidjet     *mChild;      //!< ������ �������� widjet-��
    CmtWidjet     *mCalling;    //!< ��������� ������
    CmtWidjet     *mChildFocus; //!< �������� �������, ������� ����� �����
    
    CmtContext    *mContext;    //!< �������� �������� ��� ���������

    CmtPoint       mPos;        //!< ������� widjet-� ������ ������������� Layout
    CmtPoint       mSize;       //!< ������ widjet-a
    
    CmtAnimation  *mAnimation;  //!< �������� ����
    uint32         mFlags;      //!< ����� ������

    //! �������� �������� ������� � ������
    CmtWidjet*     ChildAdd( CmtWidjet *child );
    
    //! ������� �������� ������� �� ������
    void           ChildRemove( CmtWidjet *child );
    
  public:
    CmtWidjet( CmtWidjet *parent = 0 ) :
      mNext(0),
      mParent(0),
      mChild(0),
      mCalling(0),
      mChildFocus(0),
      mContext(0),
      mPos(),
      mSize(),
      mAnimation(0),
      mFlags(CMT_WF_DIRTY|CMT_WF_VISIBLE) {
        SetParent(parent);
        }
    
    virtual ~CmtWidjet();
    
    //! ���������� ����� ������� widjet-�
    /*! \param pos ����� ������� � ����������� �������� */
    virtual void       SetPos(CmtPoint pos);

            CmtPoint   Pos() const { return mPos; }

    //! ���������� ������� � ������ ������� � ������������� ��������
    /*! ������������� ������� � ������ ������� � ������������� �������� (������)
      \param xDiv ���������� ������ � ������������ ����
      \param xPos ������� ������� � ������
      \param xSize ������ ������� � ������
      \param yDiv ���������� ������ � ������������ ����
      \param yPos ������� ������� � ������
      \param ySize ������ ������� � ������
      \param align ����� ������, ������������ ������������
         MP_CENTER - ������������ �� ������ �� �����������
         MP_LEFT   - �� ������ ����
         MP_RIGHT  - �� ������� ����
         MP_MIDDLE - �� ������ �� ���������
         MP_TOP    - �� �����
         MP_BOTTOM - �� ����
         MP_ALIGN_HORZ - ����������� �� ����������� (���� ���� �� ����������, �� ������������ �� ����������� �� ����������)
         MP_ALIGN_VERT - ����������� �� ��������� (���� ���� �� ����������, �� ������������ �� ��������� �� ����������)
         MP_SIZE_HORZ  - ����������� ���������� ������� �� �����������
         MP_SIZE_VERT  - ����������� ���������� ������� �� ���������
      \param xGap ����� ����� �������� � �������� �� x
      \param yGap ����� ����� �������� � �������� �� y */
            void       SetIndirect( uint8 xDiv, uint8 xPos, uint8 xSize, uint8 yDiv, uint8 yPos, uint8 ySize, int32 align = MP_ALIGN_HORZ | MP_ALIGN_VERT, int32 xGap = 3, int32 yGap = 3 );
    
    //! ���������� ����� ������ widjet-�
    /*! \param size ����� ������ */
    virtual void       SetSize(CmtPoint size);
    
    //! �������� ������ �������
            CmtPoint   Size() const { return mSize; }

    virtual CmtPoint   MinSize() { return mSize; }

    //! ���������� ������������ ������ ��� widjet-�
    /*! ��������� widjet �� �������� �������� (���� ���), � ����������� � �����
      (���� ������)
      \param parent ����� ������������ ������ ��� widjet-� (����� ���� 0) */
            void       SetParent( CmtWidjet *parent );
    
    //! �������� ������������ ������
            CmtWidjet* Parent() { return mParent; }
            
            void       SetCalling( CmtWidjet *widjet ) { mCalling = widjet; }
            
            CmtWidjet* Calling() { return mCalling; }
    
    //! �������� ��������� ������
    /*! ������� ��������� ������ (�� ������) � ������ �������� �������� ������ ��������
      \param round ���� �������������, ���� ������, �� ��� ���������� ����� ������������ ��������� ������
      ���� ����, �� ��� ���������� ����� ������������ ������� ������
      \return ��������� ������ */
            CmtWidjet* Next( bool round = true );
    
            CmtWidjet* Prev( bool round = true );

            CmtWidjet* Child() { return mChild; }

    //! ��������� � ������������� ���������� (�� ��������� ���������� ������ ������������� Widjet)
    virtual void       SetDirty();
    
            uint32     Flags() const { return mFlags; }
    
    //! ����������� ���������� ������ (������������� ���������� �����������)
    virtual void       OnNextFrame() {}
    
            void       NextFrame();
    
    //! ������� ��������� ������, ���������� ��� ������������� �����������
    /*! ������� ���������� �������� ��� ������������� ��������� widjet-�. �������� ����������� ����� �������,
      ��� �������� � ���������������� ����������� � ���������������� ����������. ������� widjet ���������� ������
      �������� ���������.
      \param ctx ��������, � ������� ������������ ��������� (��������) */
    virtual void       OnPaint( CmtContext *ctx ) {}

            void       Paint(CmtContext *ctx);
    
    //! ��������� � ������� �������� ��������
            void       SetContext();
    
            void       SetAnimation( CmtAnimation *ani );
            
    //! ���������� ����� ����� �� ��������� �������� ������
            void       SetChildFocus( CmtWidjet *widjet );
    
    //! ������ ����� ����� � ��������� �������
            void       KillChildFocus();
    
    //! ��������� ����� �� ��������� ������
            void       NextChildFocus( bool round );
            void       PrevChildFocus( bool round );
            
    //! ������� ������� (����������� ������������)
    /*! ��������� ������� �� ������� ������������.
       \param evType ��� ������� ��.CmtEventType
       \param evCode ��� ������� (��� ���������� ��.CmtVirtualKeyCode)
       \param pos ����� ������� � �������� ������������� �������
       \return ������, ����� ������� ����������, ���� ����� ������� �� ���������� � ������
              �������������� ����������� ������������.  */
    virtual bool       Event( CmtEventType evType, int32 evCode, CmtPoint pos );

            void       Hide() { mFlags &= ~CMT_WF_VISIBLE; SetDirty(); }
            
            void       Show() { mFlags |= CMT_WF_VISIBLE; SetDirty(); }
    
  };

typedef CmtWidjet *CmtWidjetPtr;


//! ������, ���������� ������ ������ � ���� ��������
class CmtWiBack : public CmtWidjet {
  protected:
    CmtWidjet *mBack; //! ������� ������
  public:
    //! �����������
    /*! ������� ������ � �����
      \param back ������, ������������ � ���� �������� */
    CmtWiBack( CmtWidjet *back = 0 ) : CmtWidjet(), mBack(back) {}
    
    //! ����������
    ~CmtWiBack() { if( mBack ) delete mBack; }
    
    virtual void OnPaint(CmtContext *ctx) { if( mBack ) mBack->Paint( ctx ); }
    
    virtual void SetSize(CmtPoint size) { CmtWidjet::SetSize(size); if( mBack ) mBack->SetSize(size); }
    
    void SetBack( CmtWidjet *back );
    
    CmtWidjet* Back() { return mBack; }
  };


class CmtWiConsole : CmtWidjet {
    uint32   mFontColor; //!< ���� ������
    uint32   mBackColor; //!< ���� ����
    uint8   *mCon;       //!< ������ ��������
    int      mFontSize;  //!< ������ ������
    CmtPoint mSymSize;   //!< ������ ������� � ��������
    
    void ScrollStrs();
  public:
    CmtWiConsole( int fontSize, CmtWidjet *parent );
    ~CmtWiConsole();
    
    //! ������� �� ����� ��������� ������� (��� ������������� ������������ ����������� �� �����)
    void PrintS_W( cpchar str );
    
    //! ������� �� ����� ���� ��� ��������� ����� (������������ ������� �����)
    void PrintS( cpchar str );

    virtual void       OnPaint(CmtContext *ctx);

    virtual void       SetSize(CmtPoint size);
    
            uint32     FontColor() const { return mFontColor; }
            
            void       SetFontColor( uint32 color ) { mFontColor = color; SetDirty(); }
    
            uint32     BackColor() const { return mBackColor; }
            
            void       SetBackColor( uint32 color ) { mBackColor = color; SetDirty(); }
  };

//! �������� �������� � ��������� ���������� � �������� �����������
class CmtWaMove : public CmtAnimation {
  protected:
    uint32 mSpeed;      //!< ������� �������� (� 256 ����� �������)
    bool   mRun;        //!< ���� ���������� ��������� ��� ����������
  public:
    uint32 mAccel;      /*!< ��������� ���������� � 256 ����� �������� �� ���� �����, 
                             �.�. ��������� 256 ����� ��������������� ������� �������� �� ���� �����,
                             512 ����� ��������������� ���� �������� �������� �� ���� �����,
                             128 ����� ��������������� ������� �������� �� ��� ������ � �.�.
                           */
    uint32 mStartSpeed; //!< ��������� �������� (�������� �� �����)
    uint32 mRunSpeed;   //!< �������� �������� (�������� �� �����)

    //! �����������
    CmtWaMove();
    
    //! ���������� ������� �������� ��������
    uint32 Speed() const { return mSpeed >> 8; }
    
    //! ���������� ������ (�������� ��� �������)
    bool   Run() const { return mRun; }
    
    //! ������ ��������
    /*! ���������� ������ ��������
      \param accel ������, ����� ����� ������������ ���������, ����, ���� ����� ������� �������� �������� */
    void   Start( bool accel );
    
    /*! ���������� ��������� ��������
      \param accel ������, ����� ����� ������������ ���������, ����, ���� ����� ���������� */
    void   Stop( bool accel );
    
    //! �������� ����� ��������� � ������ ������������� ��������
    uint32 AccelLen();
    
    //! ����� ������������ ������ ��������
    virtual void OnNextFrame( CmtWidjet *widjet );

    //! ���������� ������� ���������
    virtual bool IsMove() const { return mSpeed != 0; }
    
  };

//! �������� �������� ����������� �� ���������� ��������
class CmtWaMoveCell : public CmtWaMove {
  protected:
    uint32 mCellSize;  //!< ������ ������ (� ������ ������), ��� ���������� ��� ���������� �������
    uint32 mCellCount; //!< ���������� �����
    int32  mCellLast;  //!< ������ ��������� ������
    int32  mStartPos;  //!< ��������� ������� (������ ��� ������������ �� 0 �� FullSize)
    int32  mTargetPos; //!< ����� ���������� �����������
    int32  mAccelLen;  //!< ����� ���� ���������
    bool   mDir;       //!< ����������� ����������� (true �� ����������, false �� ����������)
    
  public:
    //! �����������
    /*! \param cellSize ������ ������ (���������� �������)
      \param cellCount ���������� ����� (���������� �������) */
    CmtWaMoveCell();
    
            void SetCellParam( uint32 cellSize, uint32 cellCount );
    
    virtual bool Event( CmtEventType evType, int32 evCode, CmtPoint pos );
    
    virtual void OnNextFrame( CmtWidjet *widjet );

    //! ���������� ������� ���������
    virtual bool IsMove() const { return mStartPos != mTargetPos || CmtWaMove::IsMove(); }
            
    //! ���������� ��������� ������� (������ ��� ������������!)
    int32  StartPos() const { return mStartPos; }
    
    void   SetStartPos( int32 pos ) { mStartPos = mTargetPos = pos; }
    
    //! ���������� ������ ������
    uint32 CellSize() const { return mCellSize; }
    
    //! ���������� ���������� �����
    uint32 CellCount() const { return mCellCount; }
  };

class CmtWaExhibition : public CmtAnimation {
  protected:
    int  mLevel; //!< ������� ������������ �������
    int  mStep;
    bool mDir;
  public:
    CmtWaExhibition() : CmtAnimation(), mLevel(0), mStep(4), mDir(false) {}
    
    void Show( bool smoothly );
    
    void Hide( bool smoothly );
    
    int  Level() const { return mLevel; }
    
    //! ���� ��� �������� (�����)
    /*! ��������� ���� ��� ��������
      \param widjet ������, ��� ������� ������������ ��������, ����� ���� 0, ���� �������� �� ��������� � ������� */
    virtual void OnNextFrame( CmtWidjet *widjet );
    
    //! �������������� ������� � �������������� ������������� ���������
    /*! ��������� �������������� ������� � �������������� ��������
      \param widjet ������, ������� ��������������
      \param dest ��������, � ������� ������������ ���������
      \param src �������� �������� */
    virtual void OnPaint( CmtWidjet *widjet, CmtContext *dest, CmtContext *src );
    
    
  };

//#ifdef CMT_WI_FLAT
class CmtWiFlat : public CmtWidjet {
  protected:
    uint32 mColor;
  public:
    CmtWiFlat( uint32 color ) :
      CmtWidjet( 0 ),
      mColor(color) {}
    
    CmtWiFlat( uint32 color, CmtPoint pos, CmtPoint size, CmtWidjet *parent ) :
      CmtWidjet( parent ),
      mColor(color) {
        SetPos( pos );
        SetSize( size );
        }

    virtual void OnPaint( CmtContext *ctx ) {
      ctx->FilledRect( CmtPoint(), mSize, mColor, true );
/*      if( mContext ) {
        ctx->FilledRect( CmtPoint(), mSize, mColor, true );
        }
      else {
        CmtPoint b(mPos);
        b += mSize;
        ctx->FilledRect( mPos, b, mColor, true );
        } */
      }
    
    void SetColor( uint32 color ) { mColor = color; SetDirty(); }
    
    uint32 Color() const { return mColor; }
  };
//#endif

//! ��������, � ��� ����� �������������
class CmtWiImage : public CmtWidjet {
  protected:
    CmtImage mImage;            //!< ��������
    uint32   mFrameIndex;       //!< ������ �������� ������
    uint32   mFrameDisposition; //!< ���������� ����� ����������� ������
    
    void     SetupContextOnFrame();
    
    void     InitImage();
  public:
    CmtWiImage( const void *src = 0 );
    
    ~CmtWiImage() {}
    
    //CmtImage& Image() { return mImage; }
    virtual CmtPoint MinSize() { return mImage.Size(); }

    virtual void     OnNextFrame();
    
            void     SetupImage( uint32 *src );
            
            void     LoadImage( cpchar fname );
            
            void     DefLoadImage( cpchar fname );
            
            void     SetFrame( uint32 frameIndex );
  };

typedef CmtWiImage *CmtWiImagePtr;

//! ���������� ��������, � ��� ����� �������������
class CmtWiTranspImage : public CmtWidjet {
  protected:
    CmtImage mImage;            //!< ��������
    uint32   mFrameIndex;       //!< ������ �������� ������
  public:
    CmtWiTranspImage( const void *src = 0 );

    ~CmtWiTranspImage() {}

    //CmtImage& Image() { return mImage; }
    virtual CmtPoint MinSize() { return mImage.Size(); }

    virtual void     OnNextFrame();

    virtual void     OnPaint( CmtContext *ctx );

            void     SetupImage( uint32 *src );

            void     LoadImage( cpchar fname );

            void     DefLoadImage( cpchar fname );

            void     SetFrame( uint32 frameIndex );
  };

typedef CmtWiTranspImage *CmtWiTranspImagePtr;

class CmtWiImageButton : public CmtWidjet {
  protected:
    CmtImage mDefImage;
    CmtImage mPressImage;
    uint32   mFrameIndex;       //!< ������ �������� ������
    uint8    mCode;             //!< ���, ������������ ��� ������� �� ������
    bool     mPress;
  public:
    CmtWiImageButton( CmtWidjet *parent, cpchar def, cpchar press, uint8 code );

    ~CmtWiImageButton() {}

    virtual void     OnNextFrame();

            void     Press();
  };


//typedef CmtWiImage *CmtWiImagePtr;

class CmtWiPtrImage : public CmtWidjet {
  protected:
    CmtImage *mImage;            //!< ��������
    bool      mOwner;            //!< ������, ����� ����� �������� ������������� �������� � ���� � ��������� ������
    uint32    mFrameIndex;       //!< ������ �������� ������
    uint32    mFrameDisposition; //!< ���������� ����� ����������� ������
    
    void Free();
  public:
    CmtWiPtrImage( CmtWidjet *parent );
    
    CmtWiPtrImage( CmtWidjet *parent, CmtImage *image, bool owner );
    
    ~CmtWiPtrImage() { Free(); }
    
            CmtImage* Image() { return mImage; }
    
            void      SetImage( CmtImage *image, bool owner );
    
    virtual void      OnNextFrame();

  };

typedef CmtWiPtrImage *CmtWiPtrImagePtr;

//! ���������� ���������� �������
/*!
  ��� ��������� ������������ cetClick �� ���������� ������:
  cvkUp, cvkDown, cvkPgUp, cvkPgDown
  */
class CmtWiListEngine : public CmtWidjet {
  protected:
    int32 mCount; //!< ���������� ��������� � ������
    int32 mStart; //!< ��������� ������� �����������
    int32 mPage;  //!< �������� ������������ ���������
    int32 mView;  //!< ���������� ���������, ������������ ������������ �� ������
    int32 mCur;   //!< ������ �������� ��������
  public:
    CmtWiListEngine( CmtWidjet *parent );

    int32 Cur() const { return mCur; }

    virtual void PaintItem( CmtContext *ctx, int32 pos, int32 index ) = 0;

    virtual void PaintScroll( CmtContext *ctx ) = 0;

    virtual void PaintContens( CmtContext *ctx );

    virtual void OnPaint( CmtContext *ctx ) { PaintContens(ctx); }

    virtual bool Event( CmtEventType evType, int32 evCode, CmtPoint pos );
  };

class CmtWiListVEngine : public CmtWiListEngine {
  protected:
    //��������
    CmtImage mScrollImage; //!< �������� �������
    int32    mScrollPos;   //!< ���������� ������� ������-����
    int32    mScrollStart; //!< ������ �
    int32    mScrollStop;  //!< ����� ������-����

    //����������
    CmtPoint mContensPos;  //!< ��������� �����������
    CmtPoint mCellSize;    //!< ������ ������
    CmtPoint mCellGap;     //!< ���������� ����� ��������
  public:
    CmtWiListVEngine( CmtWidjet *parent, uint32 *scroll );

    virtual void PaintScroll( CmtContext *ctx );

    virtual void PaintCell( CmtContext *ctx, CmtPoint pos, int32 index ) = 0;

    virtual void PaintItem( CmtContext *ctx, int32 pos, int32 index );
  };

//! ������� ������ � ������������ ����������� �� ��������� � ������� ��������� ����������
class CmtWiListBase : public CmtWidjet {
  protected:
    CmtWaMoveCell *mMoveCell;
    CmtPoint       mCellSize;
    uint32         mGap;
    uint32         mCurCellPos;
    int32          mPrevPos;
    uint32         mCellCount;
    
            void     Setup( int32 cellSize, uint32 gap, uint32 cellCount, uint32 curCellPos );
  public:
    CmtWiListBase();
    
    ~CmtWiListBase();

    virtual void     OnNextFrame();
    
    virtual void     OnPaint( CmtContext *ctx );    

    virtual bool     Event( CmtEventType evType, int32 evCode, CmtPoint pos );
    
    virtual void     PaintCell( int cellIndex, CmtContext *ctx, int pos ) = 0;
    
    virtual void     SetCellParam( CmtPoint size, uint32 gap, uint32 cellCount, uint32 curCellPos );
    
    virtual CmtPoint CellPos( int cellIndex );
    
            int32    CellPosBase( int cellIndex );
            
            uint32   CurrentCellIndex();
            
            void     SetCurrentCellIndex( uint32 index );
            
            bool     IsMove() const { return mMoveCell->IsMove(); }
  };


class CmtWiListHorz : public CmtWiListBase {
  public:
    virtual void     SetCellParam( CmtPoint size, uint32 gap, uint32 cellCount, uint32 curCellPos );
    
    virtual CmtPoint CellPos( int cellIndex );

    //�������������� ������ LEFT, RIGHT � UP DOWN    
    virtual bool     Event( CmtEventType evType, int32 evCode, CmtPoint pos );
  };

class CmtWiListVMenu : public CmtWiListBase {
    CmtImagePtr *mDefault;
    CmtImagePtr *mActive;
  public:
    CmtWiListVMenu( CmtImagePtr *def, CmtImagePtr *act, uint32 gap, uint32 cellCount, uint32 cellPos );
    
    virtual void     PaintCell( int cellIndex, CmtContext *ctx, int pos );
  };

//! ��������� ������� �������
/*! �������� ���� � ��������� ������� ������� ���������:
  ������� ��������, � ������� ���������� ���������� ������������ ���������� ������� ������������
  ������������ �� ������. ��� ������� ��������� �������������� ������ ��������. �����������
  �������������� ������ ��� ������ ��������� �� ������� �����������.
  */
class CmtWiBigListBase : public CmtWiListBase {
  protected:
    CmtContext *mCash;          //!< ��� ������� �������
    uint32      mBackColor;     //!< ���� ���� ��� ������ (��������� ��������� ���������� �����������)
    int         mCashStart;     //!< ������ ����������� ����
    uint32      mFontSize;      //!< ������ ������ ��� ���� �� ���������
  public:
    CmtWiBigListBase( CmtContext *cash, uint32 color = MC_WHITE32 ) : CmtWiListBase(), mCash(cash), mBackColor(color), mFontSize(10) {}
    
    uint32 FontSize() const { return mFontSize; }
    
    void   SetFontSize( uint32 fontSize ) { mFontSize = fontSize; }
    
    virtual void       OnPaint( CmtContext *ctx );    

    virtual void       PaintCell( int cellIndex, CmtContext *ctx, int pos );

    virtual void       SetSize(CmtPoint size);
    
    virtual cpchar     GetCellText( int cellIndex ) = 0;

  };

class CmtWiTextListBase : public CmtWiBigListBase {
  protected:
    cpchar *mTextPtrList;
    bool    mOwner;
    uint32  mTextPtrMax;
    
    void    Free();
    void    Alloc( uint32 textPtrMax );
  public:
    //CmtWiTextListBase( CmtContext *cash, uint32 color = MC_WHITE32, uint32 textPtrMax = 0 );
    
    CmtWiTextListBase( CmtContext *cash, uint32 color = MC_WHITE32 );
    
    ~CmtWiTextListBase() { Free(); }
    
    virtual cpchar     GetCellText( int cellIndex ) { return cellIndex >= 0 && cellIndex < (int)mTextPtrMax ? mTextPtrList[cellIndex] : 0; }
    
            void       SetCellText( int cellIndex, cpchar str ) { if(cellIndex >= 0 && cellIndex < (int)mTextPtrMax) mTextPtrList[cellIndex] = str; }

            void       SetupTextPtrList( cpchar *src );
            
            void       SetupTextPtrParam( CmtPoint cellSize, uint32 gap, uint32 curCellPos  );
            
            cpchar     GetCurrentCellText() { return GetCellText( CurrentCellIndex() ); }
            
            void       Sort( bool up );
  };

//! ������ ��������� �����
/*! ������ ��������� ����� ����� �������� ����������. ������ �������� ����� ��� ���� ����������� �����.
  �������� ��������� ������ CmtWiTextListBase, ������ ������ ���� ������� ��������� ����� ������
  Add, � ����� ������� SetupTextListParam ��� �������������� ������ � ������ � ����������� �� ������.
  ����� ������ SetupTextListParam ���������� ������ �����������. */
class CmtWiTextList : public CmtWiTextListBase {
  protected:
    CmtBuffer8 mBuffer;      //!< ����� � ����������� �������
    uint32     mDelta;       //!< ������ ���������� ������ ������ ��� ��������������� ������� ������ ��� ���������� ������
    uint32     mPos;         //!< ������� ������� � ������ ������
    uint32     mStringCount; //!< ���������� ����� � ������
    bool       mAddEnable;   //!< �������� ���������� ���������
  public:
    //! �����������
    CmtWiTextList( CmtContext *cash, uint32 color = MC_WHITE32 );
    
    //! �������� ������ � �����
    /*! ��������� ��������� ������ � ����� �������� �����.
      \param str ����������� ������ */
    void Add( cpchar str );
    
    //! ������ ��������� ����������� ������ �����
    /*! ������ ��������� ����������� ������ ����� � ��������� ����������� ��������
      ���������� � ��������� ����������.
      \param cellSize ������ ������ ������������ ���� ������ � ��������
      \param gap ������ ���������� ����� ��������
      \param curCellPos ������� �������� �������� ������ � ��������. �.�. ������� �������
      ����� ���� �� ����� ������� � ������ (�� ������ ������), � ����� ���� � ����� �������
      ������  */
    void SetupTextListParam( CmtPoint cellSize, uint32 gap, uint32 curCellPos );
   
  };

class CmtWiFileInfo : public CmtWidjet {
  public:
    CmtWiFileInfo( CmtWidjet *parent = 0 ) : CmtWidjet( parent ) {}
    
    virtual void SetFile( CmtFile &is ) = 0;
  };

class CmtWiFileInfoText : public CmtWiFileInfo {
  public:
    CmtWiFileInfoText() : CmtWiFileInfo( 0 ) {}
    
    virtual void SetFile( CmtFile &is );
  };

class CmtWiFileNameList : public CmtWiBack {
  protected:
    CmtWiTextList   *mList;           //!< ������ ������
    CmtWiFileInfo   *mInfo;           //!< �������������� ������ (�� ������ ������� ����������� �����)
    CmtWaExhibition *mInfoExhibition; //!< �������� ���������� ��� �������������� ������
    CmtPath         *mResult;         //!< ���� ���������� ���� ����������
    CmtPath          mPath;           //!< ����� ���������� ������ (����������) - ����������� �� fnamePat
  public:
    CmtWiFileNameList( CmtWiTextList *list, CmtWiFileInfo *info, cpchar fnamePat, CmtPath *res, CmtPoint cellSize, uint32 gap, uint32 curCellPos );
    
    //! ��������
    virtual void     OnNextFrame();

    //! ������� �� �������
    virtual bool     Event( CmtEventType evType, int32 evCode, CmtPoint pos );

    virtual void     Select( cpchar str ) {}
  };

struct CmtWiListMenuItem {
    cpchar mDefImageFName;    //!< ��� ����� ������� ������
    cpchar mActiveImageFName; //!< ��� ����� �������� (�������) ������
    cpchar mHelpImageFName;   //!< ��� ����� ������
  };

class CmtWiEditorA : public CmtWiBack {
  protected:
    CmtWiListVMenu  *mMenu;
    CmtWiPtrImage   *mHelp;
    
    CmtImagePtr     *mDefaultImgs;
    CmtImagePtr     *mActiveImgs;
    CmtImagePtr     *mHelpImgs;
    
    uint32           mItemCount;
    
    uint32           mCurIndex;
  public:
    CmtWiEditorA( CmtWidjet *back, CmtWiListMenuItem *items, CmtPoint menuPos, uint32 menuCurPos, CmtPoint helpPos, uint32 menuGap = 10, uint32 sel = 0 );
    
    ~CmtWiEditorA();
    
    virtual void     OnSelect( uint32 index );
    
    virtual void     OnNextFrame();

    virtual bool     Event( CmtEventType evType, int32 evCode, CmtPoint pos );
    
    virtual void     Select( uint32 index ) {}
  };


class CmtWiMenuWListHelp : public CmtWiEditorA {
  protected:
    CmtWaExhibition *mHelpExhibition;
  public:
    CmtWiMenuWListHelp( CmtWidjet *back, CmtWiListMenuItem *items, CmtPoint menuPos, uint32 menuCurPos, CmtPoint helpPos, uint32 menuGap = 10, uint32 sel = 0 );
    
    ~CmtWiMenuWListHelp();
    
    virtual void     OnNextFrame();

  };

struct CmtWidjetSystem {
  CmtScreen    *mScreen; //!< ����� �������
  CmtWidjet    *mWidjet; //!< ������� ������� ������
  CmtWidjet    *mDelete; //!< ������� ������ �� ��������
  
  void SetWidjet( CmtWidjet *widjet, bool prevDelete = true );
  
  void Call( CmtWidjet *widjet ) { SetWidjet( widjet, false ); }
  
  void Return() { SetWidjet( mWidjet->Calling(), true ); }
  
  void Loop();

  void Event( CmtEventType evType, int32 evCode, CmtPoint pos ) {
    if( mWidjet ) mWidjet->Event( evType, evCode, pos );
    if( mDelete ) delete mDelete;
    mDelete = 0;
    }
  };


#endif
