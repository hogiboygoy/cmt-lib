//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

struct CmtSpi : public CmtSpiBase {
  SPI_TypeDef         *mSpi;       //!< Аппаратyный порт spi
    
  
  //! Конструктор
  //! \param spi указатель на аппаратный порт SPI
  //! \param rx указатель на аппаратный канал для приема
  //! \param tx указатель на аппаратный канал для передачи
  CmtSpi( SPI_TypeDef *spi, CmtTransfer *rx = 0, CmtTransfer *tx = 0 ) : CmtSpiBase( rx, tx ), mSpi(spi) {}
  
  //! Операция обмена по SPI
  /*! Выполняет операцию обмена байтом по SPI. Ожидает завершения операции
прихода очередного байта с учетом сигнала прекращения операции
    \param src - указатель исходного буфера для передачи
    \param dest - указатель на место для размещения принятых байтов
    \param pBreak сигнал прекращения операции
    \return нуль, если операция завершена успешно и код ошибки в противном случае */
  virtual uint_8 Transfer( cpvoid src, pvoid dest, uint32 count, CmtEvent* pBreak );

  };

struct CmtSpiPipe : public CmtSpiPipeLinked {
  GPIO_TypeDef *mGpio;
  uint32        mPin;
  
  CmtSpiPipe( CmtSpiBase *spi, GPIO_TypeDef *gpio, uint32 pin );
  
  virtual void Select() { GPIO_ResetBits( mGpio, mPin ); }
  virtual void UnSelect() { GPIO_SetBits( mGpio, mPin ); }
  };


