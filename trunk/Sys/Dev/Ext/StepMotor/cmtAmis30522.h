//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! ���������� �������� �������� ��������� AMIS-30522
struct CmtAmis30522 {
  uint8           crwd;
  uint8           cr0;
  uint8           cr1;
  uint8           cr2;
  CmtSpiPipeBase *mSpi;
  uint8           mRunCurrent;   //!< ��� �������� ����
  uint8           mHoldCurrent;  //!< ��� ���������
  
  //! ����������� �� ���������
  CmtAmis30522( CmtSpiPipeBase *spi );
  
  //! �������
  /*! ��������� �������� ������� � ����� ������, ���������� ���� ������ ��������
    \param cmd �������
    \param val �������� � �������
    \return �������� ���� ������ */
  uint8 Cmd( uint8 cmd, uint8 val );
  
  //! ��������� (����������������) ���� ���������
  void Setup();
  
  void SetRunCurrent();
  void SetHoldCurrent();
  void SetCurrent( uint8 current );
  
  uint8 RdCrwd();
  uint8 RdCr0();
  uint8 RdCr1();
  uint8 RdCr2();
  
  //! ������ �������� ������� 0
  uint8 Sr0();

  //! ������ �������� ������� 1
  uint8 Sr1();

  //! ������ �������� ������� 2
  uint8 Sr2();

  //! ������ �������� ������� 3
  uint8 Sr3();
  };
