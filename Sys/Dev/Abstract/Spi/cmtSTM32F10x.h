//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

struct CmtSpi : public CmtSpiBase {
  SPI_TypeDef         *mSpi;       //!< �������y��� ���� spi
    
  
  //! �����������
  //! \param spi ��������� �� ���������� ���� SPI
  //! \param rx ��������� �� ���������� ����� ��� ������
  //! \param tx ��������� �� ���������� ����� ��� ��������
  CmtSpi( SPI_TypeDef *spi, CmtTransfer *rx = 0, CmtTransfer *tx = 0 ) : CmtSpiBase( rx, tx ), mSpi(spi) {}
  
  //! �������� ������ �� SPI
  /*! ��������� �������� ������ ������ �� SPI. ������� ���������� ��������
������� ���������� ����� � ������ ������� ����������� ��������
    \param src - ��������� ��������� ������ ��� ��������
    \param dest - ��������� �� ����� ��� ���������� �������� ������
    \param pBreak ������ ����������� ��������
    \return ����, ���� �������� ��������� ������� � ��� ������ � ��������� ������ */
  virtual uint_8 Transfer( cpvoid src, pvoid dest, uint32 count, CmtEvent* pBreak );

  };

struct CmtSpiPipe : public CmtSpiPipeLinked {
  GPIO_TypeDef *mGpio;
  uint32        mPin;
  
  CmtSpiPipe( CmtSpiBase *spi, GPIO_TypeDef *gpio, uint32 pin );
  
  virtual void Select() { GPIO_ResetBits( mGpio, mPin ); }
  virtual void UnSelect() { GPIO_SetBits( mGpio, mPin ); }
  };


