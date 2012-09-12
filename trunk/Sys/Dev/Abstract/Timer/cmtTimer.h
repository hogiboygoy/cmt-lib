//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

class CmtTimer8Base : public CmtDev {
  public:
    CmtSignalVoid *mSignalOver; //!< ������ ������������ �������
    
    CmtTimer8Base() : CmtDev() {}
    
    virtual uint8 GetTimer() = 0;
    
    virtual void  SetTimer( uint8 val ) = 0;
    
    virtual void  StartTimer( bool ) = 0;
    
    virtual void  SetInterval( uint8 val ) = 0;
  };

class CmtTimerBase : public CmtDev {
  public:
    CmtSignalVoid *mSignalInterval; //!< ������ ��������� �������
    
    CmtTimerBase() : CmtDev(), mSignalInterval(0) {}
    
    //! �������� �������� ������� � �������������
    virtual uint32 GetMKS() = 0;
    
    //! ���������� �������� ������� � �������������
    virtual void   SetMKS( uint32 mks ) = 0;

    //! ����-������� �������
    virtual void   StartTimer( bool ) = 0;
    
    //! ��������� ��������� ������������ �������
    virtual void   SetInterval( uint32 mks ) = 0;
  };



class CmtTimer2 : public CmtTimerBase {
    uint32 mResolution
  public:
    CmtTimer2( uint16 resolutionMKS )
  };
