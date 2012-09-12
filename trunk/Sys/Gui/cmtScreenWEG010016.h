//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! Режимы работы экрана
#define CMT_WEG010016_ON  1 //Включить экран
#define CMT_WEG010016_OFF 2 //Выключить экран

class CmtWEG010016 : public CmtScreen {
public:
  CmtWEG010016() : CmtScreen() {}
  //Аппаратный интерфейс
  void  WriteC( uint8 val );
  void  WriteD( uint8 val );
  uint8 ReadC();
  uint8 ReadD();

  //! Возвращает установленное разрешение экрана
  virtual CmtPoint Size() { return CmtPoint(100,16); }
  
  //! Инициализировать экран
  virtual void Init();
  
  //! Установить режим экрана
  /*! \param mode новый режим экрана определяется аппаратурой */
  virtual void SetMode( int mode );
  
  //! Очистить содержимое экрана заданным цветом
  /*! \param color цвет очистки */
  virtual void Clear( uint32 color );
  
  //! Копировать в экран внешний контекст
  /*! \param src исходный контекст
    \param pos место размещения контекста
    \param size размер размещения исходного контекста
    \param exPos начало исключаемой области
    \param exSize размер исключаемой области */
  virtual void Copy( CmtContext *src, CmtPoint pos, CmtPoint size, CmtPoint exPos, CmtPoint exSize );
  
  //! Копировать в экран внешний контекст
  /*! Производит копирование всего контекста, предполагается, что размер контекста совпадает
    с размером экрана
    \param src исходный контекст */
  virtual void CopyFull( CmtContext *src );
  };
