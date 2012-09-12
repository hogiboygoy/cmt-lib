//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

class CmtI2c : public CmtI2cBase {
  protected:
    I2C_TypeDef *mPort;
  public:
    CmtI2c( I2C_TypeDef *port ) : mPort(port), CmtI2cBase() {}
    
    //! ѕолучить прин€тый байт.
    /*! ¬озвращает байт из приемного буфера. 
        \return байт из премного буфера */  
    virtual uint8 GetByte() { return mPort->DR; }
    
    //! –азместить байт в буфер передатчика
    /*! ѕомещает передаваемый байт в буфере передатчика.
      \param byte байт дл€ передачи */
    virtual void PutByte( uint8 byte ) { mPort->DR = byte; }
    
    //! ”становить адрес I2C дл€ передачи мастером
    virtual void SetupMasterAddr( uint8 adr );
    
    //! ”становить адрес I2C дл€ ожидани€ подчиненным
    virtual void SetupSlaveAddr( uint8 index, uint8 addr );
  };

class CmtI2cMaster : public CmtI2c {
  public:
    CmtI2cMaster( I2C_TypeDef *port ) : CmtI2c( port ) {}
    
    //! ”становить адрес I2C 
    virtual void SetupAddr( uint8 adr );

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

class CmtI2cSlave : public CmtI2c {
    uint_8 mIndex;
  public:
    CmtI2cSlave( uint_8 index, I2C_TypeDef *port ) : CmtI2c( port ), mIndex(index) {}

    //! ”становить адрес I2C 
    void SetupAddr( uint8 addr ) { SetupSlaveAddr( mIndex, addr ); }

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
    
    //ќжидать запрос от мастера
    //¬озвращает 0 - дл€ чтени€, 1 - дл€ записи
    uint_8 WaitQuery();
  };

//void CmtI2cSetup()
