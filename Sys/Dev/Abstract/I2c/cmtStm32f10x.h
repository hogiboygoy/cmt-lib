//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
class CmtI2cMaster : public CmtI2cBase {
    I2C_TypeDef *mI2c;        //!< ������� ����� �����
    uint8        mMasterAddr; //!< ����� ��� �������� ��������
  public:
    CmtI2cMaster( I2C_TypeDef *i2c, uint8 addr = 0 ) :
      CmtI2cBase(),
      mI2c(i2c),
      mMasterAddr(addr) {}
    
    //! ���������� ����� I2C ��� �������� ��������
    virtual void SetupMasterAddr( uint8 adr ) { mMasterAddr = adr; }
    
    //! ���������� ����� I2C ��� �������� �����������
    virtual void SetupSlaveAddr( uint8 index, uint8 addr ) {}

    //! �������� �������� ����.
    /*! ���������� ���� �� ��������� ������. 
        \return ���� �� �������� ������ */  
    virtual uint8 GetByte() { return I2C_ReceiveData(mI2c) & 0xff; }
    
    //! ���������� ���� � ����� �����������
    /*! �������� ������������ ���� � ������ �����������.
      \param byte ���� ��� �������� */
    virtual void PutByte( uint8 byte ) { I2C_SendData( mI2c, byte ); }
    
    
    //! �������� ���� ������
    /*! ��������� ������� �� ����� ����� ������ � ��������� �����.
      \param dest - ����� ��� �������� ������
      \param count - ���������� ����������� ������
      \param pBreak ������ ����������� ��������
      \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
    virtual uint_8 ReadBlock( pvoid dest, uint32 count, uint32* pBreak );
    
    //! �������� ���� ������
    /*! ��������� ������� �� �������� ����� ������ �� ���������� ������.
      \param src - ����� ������ ��� ��������
      \param count - ���������� ������������ ������
      \param pBreak ������ ����������� ��������
      \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
    virtual uint_8 WriteBlock( cpvoid src, uint32 count, uint32* pBreak );
  };

