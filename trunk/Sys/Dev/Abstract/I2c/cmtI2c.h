//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================


class CmtI2cBase : public CmtSerial {
  public:
    CmtI2cBase() : CmtSerial() {}

    //! ���������� ����� I2C ��� �������� ��������
    virtual void SetupMasterAddr( uint8 adr ) = 0;
    
    //! ���������� ����� I2C ��� �������� �����������
    virtual void SetupSlaveAddr( uint8 index, uint8 addr ) = 0;
  };

//���������� ����������
#if defined( CMT_STM32F105VCT6 ) || defined( CMT_STM32F103CBT6 ) || defined( CMT_STM32F100C8T )
  #include <Sys/Dev/Abstract/I2c/cmtSTM32F10x.h>
#elif defined(CMT_TI_OMAP3530)
  #include <Sys/Dev/Abstract/I2c/cmtTiOmap3530.h>
#elif defined(CMT_STM8L151K4)
  #include <Sys/Dev/Abstract/I2c/cmtStm8l15x.h>
#else
  #error "CMT: I2C not supported for this CPU"
#endif

