//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! Микросхема драйвера шагового двигателя AMIS-30522
struct CmtAmis30522 {
  uint8           crwd;
  uint8           cr0;
  uint8           cr1;
  uint8           cr2;
  CmtSpiPipeBase *mSpi;
  uint8           mRunCurrent;   //!< Ток рабочего хода
  uint8           mHoldCurrent;  //!< Ток удержания
  
  //! Конструктор по умолчанию
  CmtAmis30522( CmtSpiPipeBase *spi );
  
  //! Команда
  /*! Выполняет передачу команды и байта данных, возвращает байт данных принятый
    \param cmd команда
    \param val значение к команде
    \return принятый байт данных */
  uint8 Cmd( uint8 cmd, uint8 val );
  
  //! Установка (программирование) всех регистров
  void Setup();
  
  void SetRunCurrent();
  void SetHoldCurrent();
  void SetCurrent( uint8 current );
  
  uint8 RdCrwd();
  uint8 RdCr0();
  uint8 RdCr1();
  uint8 RdCr2();
  
  //! Чтение регистра статуса 0
  uint8 Sr0();

  //! Чтение регистра статуса 1
  uint8 Sr1();

  //! Чтение регистра статуса 2
  uint8 Sr2();

  //! Чтение регистра статуса 3
  uint8 Sr3();
  };
