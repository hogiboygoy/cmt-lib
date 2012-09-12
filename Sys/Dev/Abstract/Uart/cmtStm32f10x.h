//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! ѕоследовательный порт типа UART с базовой функциональностью
/*! ќсобенности реализации
—одержит в своем составе структуру настройки. ѕоэтому требуетс€ дополнительна€ настройка.
  */
struct CmtUart : public CmtUartBase {
    USART_TypeDef     *mUart;
    USART_InitTypeDef  USART_InitStructure;

    //!  онструктор
    /*!
      \param uart структура, указывающа€ на аппаратные регистры порта
      \param baudRate скорость обмена, бод
      \param parity четность, может быть одним из следующих флагов
               USART_Parity_No    контроль по четности отсутствует
               USART_Parity_Even  контроль по четности
               USART_Parity_Odd   контроль по нечетности
      \param stopBits количество стоповых битов
               USART_StopBits_0_5 0.5
               USART_StopBits_1   1
               USART_StopBits_1_5 1.5
               USART_StopBits_2   2
      \param rx аппаратный механизм приемника (Interrup, Dma)
      \param tx аппаратный механизм передатчика (Interrup, Dma)
      */
    CmtUart( USART_TypeDef *uart, uint32 baudRate, uint16 parity, uint16 stopBits, CmtTransfer *rx = 0, CmtTransfer *tx = 0 );

    //! Ќастроить канал
    /*! ”станавливает новую скорость канала
      \param param указатель на структуру параметров #CmtSerialParam */
    virtual void SetBaudRate( uint32 baudRate );

    //! ѕолучить состо€ние порта
    /*! ¬озвращает состо€ние порта.
      \return нуль, если все нормально и код ошибки в противном случае */
    virtual uint_8 GetState() { return CMTE_OK; }

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

