//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

class CmtScreenTiOmap3530 : public CmtContextTiOmap3530 {
  protected:
    uint32 *mDublBuf;

    void    ResetDisplayController();
    void    EnableLcdPower();
    void    ConfigureDss( uint32 frameBuffer );
    void    LdcInitialise();
    void    DisplayLcdImage();
public:
  CmtScreenTiOmap3530( CmtPoint size, uint32 *src, uint32 *dublBuf = 0 );

  //! Установить режим экрана
  /*! \param mode новый режим экрана определяется аппаратурой */
  virtual void SetMode( int mode );
  
  //! Рисование завершено можно произвести смену фрейма
  virtual void FrameComplete();

  //! Ожидать завершения отображения очередного фрейма  
  virtual void WaitEndOfFrame();
  
  uint32* Display() { return mDublBuf == 0 ? GetData() : mDublBuf; }
  };

