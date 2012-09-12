//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

class CmtI2c : public CmtI2cBase {
  protected:
    I2C_TypeDef *mPort;
  public:
    CmtI2c( I2C_TypeDef *port ) : mPort(port), CmtI2cBase() {}
    
    //! �������� �������� ����.
    /*! ���������� ���� �� ��������� ������. 
        \return ���� �� �������� ������ */  
    virtual uint8 GetByte() { return mPort->DR; }
    
    //! ���������� ���� � ����� �����������
    /*! �������� ������������ ���� � ������ �����������.
      \param byte ���� ��� �������� */
    virtual void PutByte( uint8 byte ) { mPort->DR = byte; }
    
    //! ���������� ����� I2C ��� �������� ��������
    virtual void SetupMasterAddr( uint8 adr );
    
    //! ���������� ����� I2C ��� �������� �����������
    virtual void SetupSlaveAddr( uint8 index, uint8 addr );
  };

class CmtI2cMaster : public CmtI2c {
  public:
    CmtI2cMaster( I2C_TypeDef *port ) : CmtI2c( port ) {}
    
    //! ���������� ����� I2C 
    virtual void SetupAddr( uint8 adr );

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

class CmtI2cSlave : public CmtI2c {
    uint_8 mIndex;
  public:
    CmtI2cSlave( uint_8 index, I2C_TypeDef *port ) : CmtI2c( port ), mIndex(index) {}

    //! ���������� ����� I2C 
    void SetupAddr( uint8 addr ) { SetupSlaveAddr( mIndex, addr ); }

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
    
    //������� ������ �� �������
    //���������� 0 - ��� ������, 1 - ��� ������
    uint_8 WaitQuery();
  };

//void CmtI2cSetup()
