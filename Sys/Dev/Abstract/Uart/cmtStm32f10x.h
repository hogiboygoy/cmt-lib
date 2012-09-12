//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! ���������������� ���� ���� UART � ������� �����������������
/*! ����������� ����������
�������� � ����� ������� ��������� ���������. ������� ��������� �������������� ���������.
  */
struct CmtUart : public CmtUartBase {
    USART_TypeDef     *mUart;
    USART_InitTypeDef  USART_InitStructure;

    //! �����������
    /*!
      \param uart ���������, ����������� �� ���������� �������� �����
      \param baudRate �������� ������, ���
      \param parity ��������, ����� ���� ����� �� ��������� ������
               USART_Parity_No    �������� �� �������� �����������
               USART_Parity_Even  �������� �� ��������
               USART_Parity_Odd   �������� �� ����������
      \param stopBits ���������� �������� �����
               USART_StopBits_0_5 0.5
               USART_StopBits_1   1
               USART_StopBits_1_5 1.5
               USART_StopBits_2   2
      \param rx ���������� �������� ��������� (Interrup, Dma)
      \param tx ���������� �������� ����������� (Interrup, Dma)
      */
    CmtUart( USART_TypeDef *uart, uint32 baudRate, uint16 parity, uint16 stopBits, CmtTransfer *rx = 0, CmtTransfer *tx = 0 );

    //! ��������� �����
    /*! ������������� ����� �������� ������
      \param param ��������� �� ��������� ���������� #CmtSerialParam */
    virtual void SetBaudRate( uint32 baudRate );

    //! �������� ��������� �����
    /*! ���������� ��������� �����.
      \return ����, ���� ��� ��������� � ��� ������ � ��������� ������ */
    virtual uint_8 GetState() { return CMTE_OK; }

    //! �������� ���� ������
    /*! ��������� ������� �� ����� ����� ������ � ��������� �����.
      \param dest - ����� ��� �������� ������
      \param count - ���������� ����������� ������
      \param pBreak ������ ����������� ��������
      \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
    virtual uint_8 ReadBlock( pvoid dest, uint32 count, CmtEvent* pBreak );

    //! �������� ���� ������
    /*! ��������� ������� �� �������� ����� ������ �� ���������� ������.
      \param src - ����� ������ ��� ��������
      \param count - ���������� ������������ ������
      \param pBreak ������ ����������� ��������
      \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
    virtual uint_8 WriteBlock( cpvoid src, uint32 count, CmtEvent* pBreak );

  };

