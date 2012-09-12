//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! ���������� ���� SPI. 
/*! ����������� ����� � ���, ��� �� ������������� ��������� ������� CS.
 CS ��������������� �������� (������ ��� ������� ������� � ����������� ��
 ��������) ������� �� ������ �������� �����. ����� ����� CS ������� �������
 �������� � ������� �������� �����. ��� ���� ����������� ����� ������������
 CmtSpiSoft � pipe ������������� � ���� �� ������� CmtSpiPipeHard */
struct CmtSpiPipeHard : public CmtSpiPipeBase {
  uint32 mCs;   //!< ������ ������
  uint32 mBase; //!< ������� ����� �����
  
  CmtSpiPipeHard(uint32 base, uint32 cs, CmtTransfer *rx = 0, CmtTransfer *tx = 0 );
  
  virtual void Select();
  virtual void UnSelect();

  //! �������� ������ �� SPI
  /*! ��������� �������� ������ ������ �� SPI. ������� ���������� ��������
������� ���������� ����� � ������ ������� ����������� ��������
    \param src - ��������� ��������� ������ ��� ��������
    \param dest - ��������� �� ����� ��� ���������� �������� ������
    \param pBreak ������ ����������� ��������
    \return ����, ���� �������� ��������� ������� � ��� ������ � ��������� ������ */
  virtual uint_8 Transfer( cpvoid src, pvoid dest, uint32 count, CmtEvent* pBreak );

  };


class CmtSpiPipeSoft : public CmtSpiPipeLinked {
    uint32 mCsPin; //!< ����� ������ CS
  public:
    CmtSpiPipeSoft( CmtSpiPipeHard *pipe, uint32 pin );

    virtual void Select();
    
    virtual void UnSelect();
  };
