//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
class CmtI2cMaster : public CmtI2cBase {
    I2C_TypeDef *mI2c;        //!< Базовый адрес порта
    uint8        mMasterAddr; //!< Адрес для передачи мастером
  public:
    CmtI2cMaster( I2C_TypeDef *i2c, uint8 addr = 0 ) :
      CmtI2cBase(),
      mI2c(i2c),
      mMasterAddr(addr) {}
    
    //! Установить адрес I2C для передачи мастером
    virtual void SetupMasterAddr( uint8 adr ) { mMasterAddr = adr; }
    
    //! Установить адрес I2C для ожидания подчиненным
    virtual void SetupSlaveAddr( uint8 index, uint8 addr ) {}

    //! Получить принятый байт.
    /*! Возвращает байт из приемного буфера. 
        \return байт из премного буфера */  
    virtual uint8 GetByte() { return I2C_ReceiveData(mI2c) & 0xff; }
    
    //! Разместить байт в буфер передатчика
    /*! Помещает передаваемый байт в буфере передатчика.
      \param byte байт для передачи */
    virtual void PutByte( uint8 byte ) { I2C_SendData( mI2c, byte ); }
    
    
    //! Получить блок байтов
    /*! Формирует задание на прием блока байтов в указанный адрес.
      \param dest - буфер для принятых байтов
      \param count - количество принимаемых байтов
      \param pBreak сигнал прекращения операции
      \return нуль когда все хорошо или код ошибки в противном случае */
    virtual uint_8 ReadBlock( pvoid dest, uint32 count, uint32* pBreak );
    
    //! Передать блок байтов
    /*! Формирует задание на передачу блока байтов из указанного адреса.
      \param src - буфер байтов для передачи
      \param count - количество передаваемых байтов
      \param pBreak сигнал прекращения операции
      \return нуль когда все хорошо или код ошибки в противном случае */
    virtual uint_8 WriteBlock( cpvoid src, uint32 count, uint32* pBreak );
  };

