//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
void cmtInitPlis();

//! ������� ��������� ����
struct CmtPlisBase {
  CmtPlisBase() {}
  
  //! �������� � PLIS ������������
  /*! ���������� ���������������� ���� � ������� �������� �������. �� ����������
    ���� �� �����������.
    \param src - ����, ���������� ������ ����������������
    \param dst - ����-���� ��� �������� ����
    \param size - ������ ������ ��� ��������
    \return CMTE_OK ���� ��������� ������� ��� ��� ������ � ��������� ������ */
  virtual uint_8 Config( CmtFile &src, CmtFile &dst, uint32 size ) = 0;
  };

struct CmtPlisAltera : public CmtPlisBase {
    CmtPlisAltera() : CmtPlisBase() {}
    
    //! ��������� ����� ����, ��������� ������� ��������
    virtual void Reset( bool on ) = 0;
    
    //! ��������� ������ ���������� ��������
    virtual bool Done() = 0;

    //! �������� � PLIS ������������
    /*! ���������� ���������������� ���� � ������� �������� �������. �� ����������
      ���� �� �����������.
      \param src - ����, ���������� ������ ����������������
      \param dst - ����-���� ��� �������� ����
      \param size - ������ ������ ��� ��������
      \return CMTE_OK ���� ��������� ������� ��� ��� ������ � ��������� ������ */
    virtual uint_8 Config( CmtFile &src, CmtFile &dst, uint32 size );
  };

//����������� ������� ������
#define CMT_ALTERA_EP3C10 368011
#define CMT_ALTERA_EP3C25 718569
