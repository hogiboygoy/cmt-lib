//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

class CmtContextTiOmap3530 : public CmtScreen {
    static uint32 dmaIndex;
    static bool   dmaActive;
    //void SetupSrcDma( uint32 *base, CmtPoint pos, CmtPoint size );
    //void SetupDstDma( C)
  public:
    CmtContextTiOmap3530( CmtPoint size, uint32 *src ) : CmtScreen( size, src ) {}

    //! Рисование завершено можно произвести смену фрейма
    virtual void FrameComplete();
    
    //Заполненные формы
    virtual void     Clear( uint32 color, bool set = true );
    virtual void     FilledRect( CmtPoint a, CmtPoint b, uint32 color, bool set = true );
    virtual void     HorzLine( int32 x1, int32 x2, int32 y, uint32 color, bool set = true );
    
    //Ускоренные операции копирования
    void CopyLine( uint32 strLen, uint32 strCount, uint32 *srcData, uint32 srcSizeX, CmtPoint dest );
    void CopyColon( uint32 colonHigh, uint32 colonCount, uint32 *srcData, uint32 srcSizeX, CmtPoint dest );
    virtual void     CopyRect( CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest, bool set = true );
    virtual void     CopyTranspRect( uint32 transpColor, CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest );
    virtual void     WaitAccel();
    
    static void SetDmaChannel( uint32 index );
  };

