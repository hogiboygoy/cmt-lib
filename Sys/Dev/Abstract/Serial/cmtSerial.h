//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! Устройство с последовательным доступом
/*! Базовое устройство с последовательным доступом. Может представлять собой комбинацию
  из устройств с работой по опросу и по прерываниям. Конкретный режим программирования
  определяется конкретной системой. */
struct CmtSerial : public CmtDev {
  CmtTransfer *mRxTransfer; //!< Приемник (DMA или Interrupt)
  CmtTransfer *mTxTransfer; //!< Передатчик (DMA или Interrupt)
  
  //! Конструктор
  CmtSerial( CmtTransfer *rx = 0, CmtTransfer *tx = 0 ) : CmtDev(), mRxTransfer(rx), mTxTransfer(tx) {}
  
  //! Настроить канал
  /*! Устанавливает новую скорость канала
    \param param указатель на структуру параметров #CmtSerialParam */
  virtual void SetBaudRate( uint32 baudRate ) {}
  
  //! Получить состояние порта
  /*! Возвращает состояние порта.
    \return нуль, если все нормально и код ошибки в противном случае */
  virtual uint_8 GetState() { return CMTE_OK; }
  
  //! Получить блок байтов
  /*! Формирует задание на прием блока байтов в указанный адрес.
    \param dest - буфер для принятых байтов
    \param count - количество принимаемых байтов
    \param pBreak сигнал прекращения операции
    \return нуль когда все хорошо или код ошибки в противном случае */
  virtual uint_8 ReadBlock( pvoid dest, uint32 count, CmtEvent* pBreak ) = 0;
  
  //! Передать блок байтов
  /*! Формирует задание на передачу блока байтов из указанного адреса.
    \param src - буфер байтов для передачи
    \param count - количество передаваемых байтов
    \param pBreak сигнал прекращения операции
    \return нуль когда все хорошо или код ошибки в противном случае */
  virtual uint_8 WriteBlock( cpvoid src, uint32 count, CmtEvent* pBreak ) = 0;
  
};

//! Последовательный приемник в статический буфер (заготовка для прерывания)
struct CmtSerialReceiver : public CmtTransfer {
    uint8       *mDst;
    uint32       mDstCount;
    
    CmtSerialReceiver() : CmtTransfer(), mDst(0), mDstCount(0) {}
    
    void ReceivByte( uint8 val ) { if( mDstCount ) { *mDst++ = val; mDstCount--; } }

    void Setup( uint8 *dst, uint32 count ) { mDst = dst; mDstCount = count; }

    //! Инициировать обмен
    /*! Настраивает обмен в соответствии с заданными параметрами и разрешает
      работу приемо-передатчика (приемо-передатчик входит в режим ожидания запроса)
      \param adr адрес обмена (источник или приемник, в зависимости от конфигурации канала)
      \param elemCount количество элементов во фрейме
      \param frameCount количество фреймов */
    virtual void   InitTransfer( void *adr, uint32 elemCount, uint32 frameCount ) = 0;

    //! Ожидает завершения обмена
    /*! Ожидает завершения обмена с возможностью прекращения ожидания.
      \param pBreak указатель на событие, обеспечивающее прекращение ожидания. Возможен нулевой указатель,
      если прекращение ожидания не требуется.
      \return код ошибки, отражающий состояние завершенной операции */
    virtual uint_8 WaitTransfer( CmtEvent *pBreak = 0 ) = 0;

    //! Прекращение обмена
    /*! Прекращает операцию. После этого вызова возможно повторное инициирование операции.
        *** В частном случае запрещает прерывания от приемника */
    virtual void   StopTransfer() = 0;

  };

//! Последовательный передатчик из статического буфера (заготовка для прерывания)
struct CmtSerialTransmitter : public CmtTransfer {
    const uint8 *mSrc;
    uint32       mSrcCount;

    CmtSerialTransmitter() : CmtTransfer(), mSrc(0), mSrcCount(0) {}
    
    void TransmitByte( uint8 *val ) { if( mSrcCount ) { *val = *mSrc++; mSrcCount--; } }
    
    void Setup( const uint8 *src, uint32 count ) { mSrc = src; mSrcCount = count; }

    //! Инициировать обмен
    /*! Настраивает обмен в соответствии с заданными параметрами и разрешает
      работу приемо-передатчика (приемо-передатчик входит в режим ожидания запроса)
      \param adr адрес обмена (источник или приемник, в зависимости от конфигурации канала)
      \param elemCount количество элементов во фрейме
      \param frameCount количество фреймов */
    virtual void   InitTransfer( void *adr, uint32 elemCount, uint32 frameCount ) = 0;

    //! Ожидает завершения обмена
    /*! Ожидает завершения обмена с возможностью прекращения ожидания.
      \param pBreak указатель на событие, обеспечивающее прекращение ожидания. Возможен нулевой указатель,
      если прекращение ожидания не требуется.
      \return код ошибки, отражающий состояние завершенной операции */
    virtual uint_8 WaitTransfer( CmtEvent *pBreak = 0 ) = 0;

    //! Прекращение обмена
    /*! Прекращает операцию. После этого вызова возможно повторное инициирование операции.*/
    virtual void   StopTransfer() = 0;
  };

class CmtFileSerial : public CmtFile {
    CmtSerial *mSerial;
  public:
    CmtFileSerial( CmtSerial *serial ) : CmtFile(), mSerial(serial) {}

    //Блочный ввод-вывод
    virtual void  Read( pvoid dest, uint32 size )  { mSerial->ReadBlock( dest, size, mBreak ); }
    virtual void  Write( cpvoid src, uint32 size ) { mSerial->WriteBlock( src, size, mBreak ); }
    
    //Проверка
    virtual bool Good() { return mSerial != 0 && mSerial->GetState() == CMTE_OK; }
    
    //Позиционирование не поддерживается
  };
