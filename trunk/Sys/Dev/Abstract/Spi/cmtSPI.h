//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
//! SPI порт
struct CmtSpiBase : public CmtSerial {
  //! Конструктор
  CmtSpiBase( CmtTransfer *rx = 0, CmtTransfer *tx = 0 ) : CmtSerial( rx, tx ) {}
  
  //! Операция обмена по SPI
  /*! Выполняет операцию обмена байтом по SPI. Ожидает завершения операции
прихода очередного байта с учетом сигнала прекращения операции
    \param src - указатель исходного буфера для передачи
    \param dest - указатель на место для размещения принятых байтов
    \param pBreak сигнал прекращения операции
    \return нуль, если операция завершена успешно и код ошибки в противном случае */
  virtual uint_8 Transfer( cpvoid src, pvoid dest, uint32 count, CmtEvent* pBreak ) = 0;

  //! Получить блок байтов
  /*! Формирует задание на прием блока байтов в указанный адрес.
    \param dest - буфер для принятых байтов
    \param count - количество принимаемых байтов
    \param pBreak сигнал прекращения операции
    \return нуль когда все хорошо или код ошибки в противном случае */
  virtual uint_8 ReadBlock( pvoid dest, uint32 count, CmtEvent* pBreak ) { return Transfer( dest, dest, count, pBreak ); }

  //! Передать блок байтов
  /*! Формирует задание на передачу блока байтов из указанного адреса.
    \param src - буфер байтов для передачи
    \param count - количество передаваемых байтов
    \param pBreak сигнал прекращения операции
    \return нуль когда все хорошо или код ошибки в противном случае */
  virtual uint_8 WriteBlock( cpvoid src, uint32 count, CmtEvent* pBreak ) { return Transfer( src, 0, count, pBreak ); }
  };

//! SPI канал
struct CmtSpiPipeBase : public CmtSpiBase {
  
  CmtSpiPipeBase( CmtTransfer *rx = 0, CmtTransfer *tx = 0 ) : CmtSpiBase(rx,tx) {}
  
  virtual void Select() = 0;
  virtual void UnSelect() = 0;

  };

//! SPI канал, реализованный через связанный SPI
struct CmtSpiPipeLinked : public CmtSpiPipeBase {
  CmtSpiBase *mSpi;
    
  CmtSpiPipeLinked( CmtSpiBase *spi ) : CmtSpiPipeBase(), mSpi(spi) {}
    
  //! Операция обмена по SPI
  /*! Выполняет операцию обмена байтом по SPI. Ожидает завершения операции
прихода очередного байта с учетом сигнала прекращения операции
    \param src - байт для передачи
    \param dest - указатель на место для размещения принятого байта
    \param pBreak сигнал прекращения операции
    \return нуль, если операция завершена успешно и код ошибки в противном случае */
  virtual uint_8 Transfer( cpvoid src, pvoid dest, uint32 count, CmtEvent* pBreak ) { return mSpi->Transfer( src, dest, count, pBreak ); }

  virtual uint_8 GetState() { return mSpi->GetState(); }
  
  //! Получить блок байтов
  /*! Формирует задание на прием блока байтов в указанный адрес.
    \param dest - буфер для принятых байтов
    \param count - количество принимаемых байтов
    \param pBreak сигнал прекращения операции
    \return нуль когда все хорошо или код ошибки в противном случае */
  virtual uint_8 ReadBlock( pvoid dest, uint32 count, CmtEvent* pBreak ) { return mSpi->ReadBlock( dest, count, pBreak ); }
  
  //! Передать блок байтов
  /*! Формирует задание на передачу блока байтов из указанного адреса.
    \param src - буфер байтов для передачи
    \param count - количество передаваемых байтов
    \param pBreak сигнал прекращения операции
    \return нуль когда все хорошо или код ошибки в противном случае */
  virtual uint_8 WriteBlock( cpvoid src, uint32 count, CmtEvent* pBreak ) { return mSpi->WriteBlock( src, count, pBreak ); }

  };

//Аппаратные реализации
#if defined( CMT_STM32F105VCT6 ) || defined( CMT_STM32F103CBT6 )
  #include <Sys/Dev/Abstract/Spi/cmtSTM32F10x.h>
#elif defined(CMT_TI_OMAP3530)
  #include <Sys/Dev/Abstract/Spi/cmtTiOmap3530.h>
#else
  #error "CMT: SPI not supported for this CPU"
#endif

