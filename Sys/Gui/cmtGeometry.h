//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! Точка на плоскости
struct CmtPoint {
    int32 x; //!< Координата X
    int32 y; //!< Координата Y
  public:
    
    //! Конструктор точки с нулевыми координатами
    CmtPoint() : x(0), y(0) {}
    
    //! Конструктор точки с заданными координатами
    /*! \param sx x-координата точки
      \param sy y-координата точки */
    CmtPoint( int32 sx, int32 sy ) : x(sx), y(sy) {}

    //! Установить новые значения координат точки
    /*! \param sx x-координата точки
      \param sy y-координата точки */
    void     Set( int32 sx, int32 sy ) { x = sx; y = sy; }

    //! Поворот точки вокруг заданного центра
    /*! Возвращает повернутую точку вокруг заданного центра на заданный угол.
      Исходная точка НЕ трогается.
      \param center точка центра поворота
      \param angle угол поворота в тысяных долях градуса
      \return повернутая точка */
    CmtPoint Rotate( CmtPoint center, double angle );
    
    //! Поворот точки с заданными коэффициентами
    /*! Возвращает повернутую точку вокруг заданного центра и заполняет веса промежуточных позиций
      \param center точка центра поворота
      \param sin значение синуса угла поворота
      \param cos значение косинуса угла поворота
      \param levelTable указатель на массив из четырех элементов, которые получают вес текущей и трех соседних точек (справа, справа внизу, внизу)
      \return повернутая точка */
    CmtPoint RotateL( CmtPoint center, double sin, double cos, int32 *levelTable );
    
    //! Сравнение точек
    bool     operator == ( CmtPoint p )   { return x == p.x && y == p.y; }

    //! Сравнение точек
    bool     operator != ( CmtPoint p )   { return x != p.x || y != p.y; }

    //! Смещение точек (изменяет текущую точку)
    CmtPoint operator += ( int32 offset ) { x += offset; y += offset; return *this; }

    //! Смещение точек (изменяет текущую точку)
    CmtPoint operator += ( CmtPoint p )   { x += p.x; y += p.y; return *this; }

    //! Смещение точек (изменяет текущую точку)
    CmtPoint operator -= ( int32 offset ) { x -= offset; y -= offset; return *this; }

    //! Смещение точек (изменяет текущую точку)
    CmtPoint operator -= ( CmtPoint p )   { x -= p.x; y -= p.y; return *this; }

    //! Масштабирование точек (изменяет текущую точку)
    CmtPoint operator /= ( int32 div )    { x /= div; y /= div; return *this; }

    //! Масштабирование точек (изменяет текущую точку)
    CmtPoint operator *= ( int32 mul )    { x *= mul; y *= mul; return *this; }

    //! Операция подстройки точки
    void     Ajust( CmtPoint origin, CmtPoint ajust, CmtPoint delta );
    
    //! Операция получения точки с максимальными значениями
    void     Max( CmtPoint test );
    
    //! Операция получения точки с минимальными значениями
    void     Min( CmtPoint test );
    
    //! Расстояние между данной точкой и заданной
    double   Distance( CmtPoint p );
    
    //! Угол поворота данной точки относительно ортогональной системы координат с центром в заданной точке
    double   GetAngle( CmtPoint center );
    
    static void GetSinCos( int angle, double *sinVal, double *cosVal ); //cmtGBase.cpp
    static bool IsOverRect( CmtPoint testPos, CmtPoint testSize, CmtPoint overPos, CmtPoint overSize );

  };


//! Прямоугольник на плоскости (ортогональный по двум противоположным углам)
struct CmtRect {
    CmtPoint a; //!< Координата верхего левого угла
    CmtPoint b; //!< Координата нижнего правого угла
  public:
    
    //! Конструктор по умолчанию с углами в точке 0,0
    CmtRect() : a(), b() {}
    
    //! Конструктор прямоугольника с углами в заданных координатах
    CmtRect( int32 ax, int32 ay, int32 bx, int32 by ) : a(ax,ay), b(bx,by) {}

    //! Конструктор прямоугольника с углами в заданных точках
    CmtRect( CmtPoint a, CmtPoint b ) : a(a), b(b) {}
    
    //! Конструктор копирования
    CmtRect( CmtRect &r ) : a(r.a), b(r.b) {}

    //! Ширина прямоугольника
    int    Width() const { return b.x - a.x; }
    
    //! Высота прямоугольника
    int    Height() const { return b.y - b.y; }
    
    //! Определяет попадает ли точка внутрь прямоугольника
    bool   IsPointInside( CmtPoint p );
    
    //! Определяет пустой прямоугольник (с совпадающими углами)
    bool   IsEmpty();
    
    //! Сравнение прямоугольников
    bool   IsEqual( CmtRect &rect );
    
    //! Определяет пересекаются ли прямоугольники
    bool   IsIntersect( const CmtRect &rect );
    
    //! Подстройка
    void   Ajust( CmtRect &origin, CmtRect &ajust, CmtPoint delta );
    
    //! Установить новые координаты углов
    void   Set( CmtPoint sa, CmtPoint sb ) { a = sa; b = sb; }

    //! Возвращает угол прямоугольника
    CmtPoint LeftTop() const { return a; }

    //! Возвращает угол прямоугольника
    CmtPoint LeftBottom() const { return CmtPoint( a.x, b.y ); }

    //! Возвращает угол прямоугольника
    CmtPoint RightTop() const { return CmtPoint( b.x, a.y ); }

    //! Возвращает угол прямоугольника
    CmtPoint RightBottom() const { return b; }
  };

