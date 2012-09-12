//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
class CmtScreenWin32 : public CmtScreen {
    HWND hWnd;
  public:
    CmtScreenWin32( CmtPoint size, HWND wnd, uint32 *buf ) : CmtScreen( size, buf ), hWnd(wnd) {}
    
    //! Рисование завершено можно произвести смену фрейма
    virtual void FrameComplete();

    
    void PaintDC( HDC hdc );
  };

