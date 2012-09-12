//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
class CmtI2cMaster : public CmtI2cBase {
    uint32 mBase;      //!< ������� ����� �����
    //uint8  mSlaveAddr; //!< �����, �� �������� �������������� ����� �� ����
  public:
    CmtI2cMaster( uint32 baseAddr );
    
    //! ���������� ����� I2C ��� �������� ��������
    virtual void SetupMasterAddr( uint8 adr );
    
    //! ���������� ����� I2C ��� �������� �����������
    virtual void SetupSlaveAddr( uint8 index, uint8 addr ) {}

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


