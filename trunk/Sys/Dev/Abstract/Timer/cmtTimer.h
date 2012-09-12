//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

class CmtTimer8Base : public CmtDev {
  public:
    CmtSignalVoid *mSignalOver; //!< Сигнал переполнения таймера
    
    CmtTimer8Base() : CmtDev() {}
    
    virtual uint8 GetTimer() = 0;
    
    virtual void  SetTimer( uint8 val ) = 0;
    
    virtual void  StartTimer( bool ) = 0;
    
    virtual void  SetInterval( uint8 val ) = 0;
  };

class CmtTimerBase : public CmtDev {
  public:
    CmtSignalVoid *mSignalInterval; //!< Сигнал интервала таймера
    
    CmtTimerBase() : CmtDev(), mSignalInterval(0) {}
    
    //! Получить значение таймера в микросекундах
    virtual uint32 GetMKS() = 0;
    
    //! Установить значение таймера в микросекундах
    virtual void   SetMKS( uint32 mks ) = 0;

    //! Пуск-останов таймера
    virtual void   StartTimer( bool ) = 0;
    
    //! Установка интервала срабатывания таймера
    virtual void   SetInterval( uint32 mks ) = 0;
  };



class CmtTimer2 : public CmtTimerBase {
    uint32 mResolution
  public:
    CmtTimer2( uint16 resolutionMKS )
  };
