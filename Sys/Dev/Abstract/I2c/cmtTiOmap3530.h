//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
class CmtI2cMaster : public CmtI2cBase {
    uint32 mBase;      //!< Ѕазовый адрес порта
    //uint8  mSlaveAddr; //!< јдрес, по которому осуществл€етс€ обмен по шине
  public:
    CmtI2cMaster( uint32 baseAddr );
    
    //! ”становить адрес I2C дл€ передачи мастером
    virtual void SetupMasterAddr( uint8 adr );
    
    //! ”становить адрес I2C дл€ ожидани€ подчиненным
    virtual void SetupSlaveAddr( uint8 index, uint8 addr ) {}

    //! ѕолучить блок байтов
    /*! ‘ормирует задание на прием блока байтов в указанный адрес.
      \param dest - буфер дл€ прин€тых байтов
      \param count - количество принимаемых байтов
      \param pBreak сигнал прекращени€ операции
      \return нуль когда все хорошо или код ошибки в противном случае */
    virtual uint_8 ReadBlock( pvoid dest, uint32 count, CmtEvent* pBreak );

    //! ѕередать блок байтов
    /*! ‘ормирует задание на передачу блока байтов из указанного адреса.
      \param src - буфер байтов дл€ передачи
      \param count - количество передаваемых байтов
      \param pBreak сигнал прекращени€ операции
      \return нуль когда все хорошо или код ошибки в противном случае */
    virtual uint_8 WriteBlock( cpvoid src, uint32 count, CmtEvent* pBreak );
  };


