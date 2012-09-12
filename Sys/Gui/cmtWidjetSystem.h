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
  //Scroll-бары четные горизонтальные, нечетные - вертикальные
  cvkScroll0  = 0, //Горизонтальный
  cvkScroll1,      //Вертикальный
  cvkScroll2,      //Горизонтальный
  cvkScroll3,      //Вертикальный
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
    
    //! Внешнее событие (воздействие пользователя)
    /*! Выполняет реакцию на внешние раздражители.
      \param evType тип события см.CmtEventType
      \param evCode код события (для клавиатуры см.CmtVirtualKeyCode)
      \param pos точка события в пикселях родительского виджета
      \return истина, когда событие обработано, ложь когда событие не обработано и должно
      обрабатываться стандартным обработчиком.  */
    virtual bool Event( CmtEventType evType, int32 evCode, CmtPoint pos ) { return false; }
    
  };

//! Анимационный механизм (базовый)
class CmtAnimation : public CmtInputHandler {
  public:
    CmtAnimation() : CmtInputHandler() {}
    virtual ~CmtAnimation() {}
    
    //! Один шаг анимации (фрейм)
    /*! Выполняет один шаг анимации
      \param widjet виджет, над которым производится анимация, может быть 0, если анимация не привязана к виджету */
    virtual void OnNextFrame( CmtWidjet *widjet ) = 0;
    
    //! Преобразование рисунка с использованием анимационного алгоритма
    /*! Выполняет преобразование рисунка с использованием анимации
      \param widjet виджет, который отрисовывается
      \param dest контекст, в который производится рисование
      \param src исходный контекст */
    virtual void OnPaint( CmtWidjet *widjet, CmtContext *dest, CmtContext *src );
    
    //! Начальные установки анимационного алгоритма при присоединении к виджету
    /*! Производит начальные установки анимационного алгоритма при присоединении к виджету
      \param widjet виджет, к которому производится подключение */
    virtual void OnSetup( CmtWidjet *widjet ) {}
    
  };

#define CMT_WF_DIRTY   MBIT0 //Флаг определяет необходимость перерисовки изображения виджета
#define CMT_WF_FOCUS   MBIT1 //Флаг определяет способность виджета принимать фокус ввода
#define CMT_WF_VISIBLE MBIT2 //Флаг определяет видимость виджета

class CmtWidjet : public CmtInputHandler {
  protected:
    CmtWidjet     *mNext;       //!< Следующий widjet в родительском списке дочерних widjet-ов
    CmtWidjet     *mParent;     //!< Родительский widjet
    CmtWidjet     *mChild;      //!< Список дочерних widjet-ов
    CmtWidjet     *mCalling;    //!< Вызвавший виджет
    CmtWidjet     *mChildFocus; //!< Дочерний элемент, имеющий фокус ввода
    
    CmtContext    *mContext;    //!< Буферный контекст для рисования

    CmtPoint       mPos;        //!< Позиция widjet-а внутри родительского Layout
    CmtPoint       mSize;       //!< Размер widjet-a
    
    CmtAnimation  *mAnimation;  //!< Анимация окна
    uint32         mFlags;      //!< Набор флагов

    //! Добавить дочерний элемент к списку
    CmtWidjet*     ChildAdd( CmtWidjet *child );
    
    //! Удалить дочерний элемент из списка
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
    
    //! Установить новую позицию widjet-а
    /*! \param pos новая позиция в координатах родителя */
    virtual void       SetPos(CmtPoint pos);

            CmtPoint   Pos() const { return mPos; }

    //! Установить позицию и размер виджета в относительных единицах
    /*! Устанавливает позицию и размер виджета в относительных единицах (частях)
      \param xDiv количество частей в родительском окне
      \param xPos позиция виджета в частях
      \param xSize размер виджета в частях
      \param yDiv количество частей в родительском окне
      \param yPos позиция виджета в частях
      \param ySize размер виджета в частях
      \param align набор флагов, определяющих выравнивание
         MP_CENTER - выравнивание по центру по горизонтали
         MP_LEFT   - по левому краю
         MP_RIGHT  - по правому краю
         MP_MIDDLE - по центру по вертикали
         MP_TOP    - по верху
         MP_BOTTOM - по низу
         MP_ALIGN_HORZ - выравнивать по горизонтали (если флаг не установлен, то выравнивания по горизонтали не происходит)
         MP_ALIGN_VERT - выравнивать по вертикали (если флаг не установлен, то выравнивания по вертикали не происходит)
         MP_SIZE_HORZ  - производить подстройку размера по горизонтали
         MP_SIZE_VERT  - производить подстройку размера по вертикали
      \param xGap зазор между областью и виджетом по x
      \param yGap зазор между областью и виджетом по y */
            void       SetIndirect( uint8 xDiv, uint8 xPos, uint8 xSize, uint8 yDiv, uint8 yPos, uint8 ySize, int32 align = MP_ALIGN_HORZ | MP_ALIGN_VERT, int32 xGap = 3, int32 yGap = 3 );
    
    //! Установить новый размер widjet-а
    /*! \param size новый размер */
    virtual void       SetSize(CmtPoint size);
    
    //! Получить размер виджета
            CmtPoint   Size() const { return mSize; }

    virtual CmtPoint   MinSize() { return mSize; }

    //! Установить родительский виджет для widjet-а
    /*! Исключает widjet из прежнего родителя (если был), и прописывает в новом
      (если указан)
      \param parent новый родительский виджет для widjet-а (может быть 0) */
            void       SetParent( CmtWidjet *parent );
    
    //! Получить родительский виджет
            CmtWidjet* Parent() { return mParent; }
            
            void       SetCalling( CmtWidjet *widjet ) { mCalling = widjet; }
            
            CmtWidjet* Calling() { return mCalling; }
    
    //! Получить следующий виджет
    /*! Смотрит следующий виджет (за данным) в списке дочерних виджетов своего родителя
      \param round флаг заворачивания, если истина, то при достижении конца возвращается начальный виджет
      если ложь, то при достижении конца возвращается текущий виджет
      \return следующий виджет */
            CmtWidjet* Next( bool round = true );
    
            CmtWidjet* Prev( bool round = true );

            CmtWidjet* Child() { return mChild; }

    //! Извещение о необходимости обновления (по умолчанию отправляет сигнал родительскому Widjet)
    virtual void       SetDirty();
    
            uint32     Flags() const { return mFlags; }
    
    //! Отображение очередного фрейма (синхронизация устройства отображения)
    virtual void       OnNextFrame() {}
    
            void       NextFrame();
    
    //! Функция обратного вызова, вызывается при необходимости перерисовки
    /*! Функция вызывается системой при необходимости отрисовки widjet-а. Контекст формируется таким образом,
      что работает в пользовательских координатах с пользовательским отсечением. Поэтому widjet производит только
      операцию рисования.
      \param ctx контекст, в котором производится рисование (контекст) */
    virtual void       OnPaint( CmtContext *ctx ) {}

            void       Paint(CmtContext *ctx);
    
    //! Добавляет к виджету буферный контекст
            void       SetContext();
    
            void       SetAnimation( CmtAnimation *ani );
            
    //! Установить фокус ввода на указанный дочерний виджет
            void       SetChildFocus( CmtWidjet *widjet );
    
    //! Убрать фокус ввода с дочернего виджета
            void       KillChildFocus();
    
    //! Перевести фокус на следующий виджет
            void       NextChildFocus( bool round );
            void       PrevChildFocus( bool round );
            
    //! Внешнее событие (воздействие пользователя)
    /*! Выполняет реакцию на внешние раздражители.
       \param evType тип события см.CmtEventType
       \param evCode код события (для клавиатуры см.CmtVirtualKeyCode)
       \param pos точка события в пикселях родительского виджета
       \return истина, когда событие обработано, ложь когда событие не обработано и должно
              обрабатываться стандартным обработчиком.  */
    virtual bool       Event( CmtEventType evType, int32 evCode, CmtPoint pos );

            void       Hide() { mFlags &= ~CMT_WF_VISIBLE; SetDirty(); }
            
            void       Show() { mFlags |= CMT_WF_VISIBLE; SetDirty(); }
    
  };

typedef CmtWidjet *CmtWidjetPtr;


//! Виджет, содержащий другой виджет в виде фонового
class CmtWiBack : public CmtWidjet {
  protected:
    CmtWidjet *mBack; //! Фоновый виджет
  public:
    //! Конструктор
    /*! Создает виджет с фоном
      \param back виджет, используемый в виде фонового */
    CmtWiBack( CmtWidjet *back = 0 ) : CmtWidjet(), mBack(back) {}
    
    //! Деструктор
    ~CmtWiBack() { if( mBack ) delete mBack; }
    
    virtual void OnPaint(CmtContext *ctx) { if( mBack ) mBack->Paint( ctx ); }
    
    virtual void SetSize(CmtPoint size) { CmtWidjet::SetSize(size); if( mBack ) mBack->SetSize(size); }
    
    void SetBack( CmtWidjet *back );
    
    CmtWidjet* Back() { return mBack; }
  };


class CmtWiConsole : CmtWidjet {
    uint32   mFontColor; //!< Цвет текста
    uint32   mBackColor; //!< Цвет фона
    uint8   *mCon;       //!< Массив символов
    int      mFontSize;  //!< Размер шрифта
    CmtPoint mSymSize;   //!< Размер консоли в символах
    
    void ScrollStrs();
  public:
    CmtWiConsole( int fontSize, CmtWidjet *parent );
    ~CmtWiConsole();
    
    //! Выводит на экран очередную строчку (при необходимости производится ограничение по длине)
    void PrintS_W( cpchar str );
    
    //! Выводит на экран одну или несколько строк (осуществляет перенос строк)
    void PrintS( cpchar str );

    virtual void       OnPaint(CmtContext *ctx);

    virtual void       SetSize(CmtPoint size);
    
            uint32     FontColor() const { return mFontColor; }
            
            void       SetFontColor( uint32 color ) { mFontColor = color; SetDirty(); }
    
            uint32     BackColor() const { return mBackColor; }
            
            void       SetBackColor( uint32 color ) { mBackColor = color; SetDirty(); }
  };

//! Анимация движения с начальным ускорением и конечным замедлением
class CmtWaMove : public CmtAnimation {
  protected:
    uint32 mSpeed;      //!< Текущая скорость (в 256 долях пиксела)
    bool   mRun;        //!< Флаг определяет ускорение или замедление
  public:
    uint32 mAccel;      /*!< Ускорение измеряется в 256 долях скорости на один фрейм, 
                             т.е. ускорение 256 будет соответствовать единице скорости на один фрейм,
                             512 будет соответствовать двум единицам скорости на один фрейм,
                             128 будет соответствовать единице скорости на два фрейма и т.д.
                           */
    uint32 mStartSpeed; //!< Начальная скорость (пикселов на фрейм)
    uint32 mRunSpeed;   //!< Скорость движения (пикселов на фрейм)

    //! Конструктор
    CmtWaMove();
    
    //! Возвращает текущую скорость движения
    uint32 Speed() const { return mSpeed >> 8; }
    
    //! Показывает статус (движение или останов)
    bool   Run() const { return mRun; }
    
    //! Начать движение
    /*! Инициирует начало движения
      \param accel истина, когда нужно использовать ускорение, ложь, если сразу ставить скорость движения */
    void   Start( bool accel );
    
    /*! Инициирует окончание движения
      \param accel истина, когда нужно использовать ускорение, ложь, если сразу остановить */
    void   Stop( bool accel );
    
    //! Получить длину ускорения с учетом установленных значений
    uint32 AccelLen();
    
    //! Здесь производится расчет скорости
    virtual void OnNextFrame( CmtWidjet *widjet );

    //! Показывает текущее состояние
    virtual bool IsMove() const { return mSpeed != 0; }
    
  };

//! Механизм плавного перемещения по дискретным позициям
class CmtWaMoveCell : public CmtWaMove {
  protected:
    uint32 mCellSize;  //!< Размер ячейки (с учетом зазора), это собственно шаг дискретной позиции
    uint32 mCellCount; //!< Количество ячеек
    int32  mCellLast;  //!< Начало последней ячейки
    int32  mStartPos;  //!< Начальная позиция (именно она перемещается от 0 до FullSize)
    int32  mTargetPos; //!< Точка назначения перемещения
    int32  mAccelLen;  //!< Длина пути ускорения
    bool   mDir;       //!< Направление перемещения (true на увеличение, false на уменьшение)
    
  public:
    //! Конструктор
    /*! \param cellSize размер ячейки (дискретной позиции)
      \param cellCount количество ячеек (дискретных позиций) */
    CmtWaMoveCell();
    
            void SetCellParam( uint32 cellSize, uint32 cellCount );
    
    virtual bool Event( CmtEventType evType, int32 evCode, CmtPoint pos );
    
    virtual void OnNextFrame( CmtWidjet *widjet );

    //! Показывает текущее состояние
    virtual bool IsMove() const { return mStartPos != mTargetPos || CmtWaMove::IsMove(); }
            
    //! Возвращает начальную позицию (именно она перемещается!)
    int32  StartPos() const { return mStartPos; }
    
    void   SetStartPos( int32 pos ) { mStartPos = mTargetPos = pos; }
    
    //! Возвращает размер ячейки
    uint32 CellSize() const { return mCellSize; }
    
    //! Вовзращает количество ячеек
    uint32 CellCount() const { return mCellCount; }
  };

class CmtWaExhibition : public CmtAnimation {
  protected:
    int  mLevel; //!< Уровень прозрачности объекта
    int  mStep;
    bool mDir;
  public:
    CmtWaExhibition() : CmtAnimation(), mLevel(0), mStep(4), mDir(false) {}
    
    void Show( bool smoothly );
    
    void Hide( bool smoothly );
    
    int  Level() const { return mLevel; }
    
    //! Один шаг анимации (фрейм)
    /*! Выполняет один шаг анимации
      \param widjet виджет, над которым производится анимация, может быть 0, если анимация не привязана к виджету */
    virtual void OnNextFrame( CmtWidjet *widjet );
    
    //! Преобразование рисунка с использованием анимационного алгоритма
    /*! Выполняет преобразование рисунка с использованием анимации
      \param widjet виджет, который отрисовывается
      \param dest контекст, в который производится рисование
      \param src исходный контекст */
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

//! Картинка, в том числе анимированная
class CmtWiImage : public CmtWidjet {
  protected:
    CmtImage mImage;            //!< Картинка
    uint32   mFrameIndex;       //!< Индекс текущего фрейма
    uint32   mFrameDisposition; //!< Оставшееся время отображения фрейма
    
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

//! Прозрачная картинка, в том числе анимированная
class CmtWiTranspImage : public CmtWidjet {
  protected:
    CmtImage mImage;            //!< Картинка
    uint32   mFrameIndex;       //!< Индекс текущего фрейма
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
    uint32   mFrameIndex;       //!< Индекс текущего фрейма
    uint8    mCode;             //!< Код, генерируемый при нажатии на кнопку
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
    CmtImage *mImage;            //!< Картинка
    bool      mOwner;            //!< Истина, когда класс является собственником картинки и ложь в противном случае
    uint32    mFrameIndex;       //!< Индекс текущего фрейма
    uint32    mFrameDisposition; //!< Оставшееся время отображения фрейма
    
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

//! Управление визуальным списком
/*!
  Для прокрутки используется cetClick со следующими кодами:
  cvkUp, cvkDown, cvkPgUp, cvkPgDown
  */
class CmtWiListEngine : public CmtWidjet {
  protected:
    int32 mCount; //!< Количество элементов в списке
    int32 mStart; //!< Начальный элемент отображения
    int32 mPage;  //!< Величина постраничной прокрутки
    int32 mView;  //!< Количество элементов, отображаемых одновременно на экране
    int32 mCur;   //!< Индекс текущего элемента
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
    //Скроллер
    CmtImage mScrollImage; //!< Картинка скролла
    int32    mScrollPos;   //!< Постоянная позиция скролл-бара
    int32    mScrollStart; //!< Начало и
    int32    mScrollStop;  //!< конец скролл-бара

    //Содержимое
    CmtPoint mContensPos;  //!< Положение содержимого
    CmtPoint mCellSize;    //!< Размер ячейки
    CmtPoint mCellGap;     //!< Промежуток между ячейками
  public:
    CmtWiListVEngine( CmtWidjet *parent, uint32 *scroll );

    virtual void PaintScroll( CmtContext *ctx );

    virtual void PaintCell( CmtContext *ctx, CmtPoint pos, int32 index ) = 0;

    virtual void PaintItem( CmtContext *ctx, int32 pos, int32 index );
  };

//! Базовый список с вертикальной ориентацией по умолчанию с текущим элементом посередине
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

    //Преобразование кнопок LEFT, RIGHT в UP DOWN    
    virtual bool     Event( CmtEventType evType, int32 evCode, CmtPoint pos );
  };

class CmtWiListVMenu : public CmtWiListBase {
    CmtImagePtr *mDefault;
    CmtImagePtr *mActive;
  public:
    CmtWiListVMenu( CmtImagePtr *def, CmtImagePtr *act, uint32 gap, uint32 cellCount, uint32 cellPos );
    
    virtual void     PaintCell( int cellIndex, CmtContext *ctx, int pos );
  };

//! Поддержка больших списков
/*! Основная идея в поддержке больших списков следующая:
  имеется контекст, в котором отрисовано содержимое увеличенного количества пунктов относительно
  помещающихся на экране. При обычной прокрутке прокручивается только контекст. Перерисовка
  осуществляется только при выходе контекста за границы отображения.
  */
class CmtWiBigListBase : public CmtWiListBase {
  protected:
    CmtContext *mCash;          //!< Кэш видимых пунктов
    uint32      mBackColor;     //!< Цвет фона для списка (позволяет выполнить прозрачное копирование)
    int         mCashStart;     //!< Начало отображения кэша
    uint32      mFontSize;      //!< Размер шрифта для кэша по умолчанию
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

//! Список текстовых строк
/*! Список текстовых строк имеет странную реализацию. Список содержит буфер для всех добавляемых строк.
  Учитывая специфику работы CmtWiTextListBase, строки должны быть сначала добавлены путем вызова
  Add, а затем вызвана SetupTextListParam для отождествления текста в буфере и изображения на экране.
  После вызова SetupTextListParam добавление текста блокируется. */
class CmtWiTextList : public CmtWiTextListBase {
  protected:
    CmtBuffer8 mBuffer;      //!< Буфер с добавленным текстом
    uint32     mDelta;       //!< Размер приращения буфера текста при недостаточности размера буфера для добавления строки
    uint32     mPos;         //!< Текущая позиция в буфере текста
    uint32     mStringCount; //!< Количество строк в буфере
    bool       mAddEnable;   //!< Операция добавления разрешена
  public:
    //! Конструктор
    CmtWiTextList( CmtContext *cash, uint32 color = MC_WHITE32 );
    
    //! Добавить строку в буфер
    /*! Добавляет очередную строку в буфер хранения строк.
      \param str добавляемая строка */
    void Add( cpchar str );
    
    //! Задает параметры отображения списка строк
    /*! Задает параметры отображения списка строк и блокирует последующие операции
      добавления и установки параметров.
      \param cellSize размер ячейки отображающей одну строку в пикселах
      \param gap размер промежутка между ячейками
      \param curCellPos позиция текущего элемента списка в пикселах. Т.е. текущий элемент
      может быть не самым верхним в списке (не вверху экрана), а может быть в любой позиции
      экрана  */
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
    CmtWiTextList   *mList;           //!< Список файлов
    CmtWiFileInfo   *mInfo;           //!< Информационная панель (на основе анализа содержимого файла)
    CmtWaExhibition *mInfoExhibition; //!< Алгоритм проявления для информационной панели
    CmtPath         *mResult;         //!< Сюда помещается файл результата
    CmtPath          mPath;           //!< Место размещения файлов (директорий) - извлекается из fnamePat
  public:
    CmtWiFileNameList( CmtWiTextList *list, CmtWiFileInfo *info, cpchar fnamePat, CmtPath *res, CmtPoint cellSize, uint32 gap, uint32 curCellPos );
    
    //! Анимация
    virtual void     OnNextFrame();

    //! Реакция на событие
    virtual bool     Event( CmtEventType evType, int32 evCode, CmtPoint pos );

    virtual void     Select( cpchar str ) {}
  };

struct CmtWiListMenuItem {
    cpchar mDefImageFName;    //!< Имя файла обычной кнопки
    cpchar mActiveImageFName; //!< Имя файла активной (текущей) кнопки
    cpchar mHelpImageFName;   //!< Имя файла помощи
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
  CmtScreen    *mScreen; //!< Экран системы
  CmtWidjet    *mWidjet; //!< Главный текущий виджет
  CmtWidjet    *mDelete; //!< Главный виджет на удаление
  
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
