//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_ST_MEMS_H
#define CMT_ST_MEMS_H

//! ���������� ST MEMS
struct CmtStMems {
  CmtSpiPipeBase *mSpi;
  
  //! ����������� �� ���������
  CmtStMems( CmtSpiPipeBase *spi ) : mSpi(spi) {}
  
  void WrAdr( uint8 adr );
  
  //! ������ � �������
  /*! ��������� ������ �������� � ������� ���������� �� ��������� ������ ��������
    \param adr - ����� ��������
    \param val - �������� ��������
    \return �������� ���� ������ �� ����� �������� ������ */
  uint8 WrReg8( uint8 adr, uint8 val );

  //! ������ � ���� ���������
  /*! ��������� ������ �������� � ���� ������� ��������� ���������� �� ��������� ������ ��������
    \param adr - ����� ��������
    \param val - �������� �������� */
  void WrReg16( uint8 adr, uint16 val );
  
  //! ������ �� ��������
  /*! ��������� ������ �������� �� �������� ���������� �� ��������� ������ ��������
    \param adr - ����� ��������
    \return �������� ���� �� �������� */
  uint8 RdReg8( uint8 adr );
  
  //! ������ �� ���� ���������
  /*! ��������� ������ �������� �� ���� ������� ��������� ���������� �� ��������� ������ ��������
    \param adr - ����� ��������
    \return �������� �������� �� ��������� */
  uint16 RdReg16( uint8 adr );
  
  };

#endif
