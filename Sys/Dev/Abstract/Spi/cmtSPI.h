//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
//! SPI ����
struct CmtSpiBase : public CmtSerial {
  //! �����������
  CmtSpiBase( CmtTransfer *rx = 0, CmtTransfer *tx = 0 ) : CmtSerial( rx, tx ) {}
  
  //! �������� ������ �� SPI
  /*! ��������� �������� ������ ������ �� SPI. ������� ���������� ��������
������� ���������� ����� � ������ ������� ����������� ��������
    \param src - ��������� ��������� ������ ��� ��������
    \param dest - ��������� �� ����� ��� ���������� �������� ������
    \param pBreak ������ ����������� ��������
    \return ����, ���� �������� ��������� ������� � ��� ������ � ��������� ������ */
  virtual uint_8 Transfer( cpvoid src, pvoid dest, uint32 count, CmtEvent* pBreak ) = 0;

  //! �������� ���� ������
  /*! ��������� ������� �� ����� ����� ������ � ��������� �����.
    \param dest - ����� ��� �������� ������
    \param count - ���������� ����������� ������
    \param pBreak ������ ����������� ��������
    \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
  virtual uint_8 ReadBlock( pvoid dest, uint32 count, CmtEvent* pBreak ) { return Transfer( dest, dest, count, pBreak ); }

  //! �������� ���� ������
  /*! ��������� ������� �� �������� ����� ������ �� ���������� ������.
    \param src - ����� ������ ��� ��������
    \param count - ���������� ������������ ������
    \param pBreak ������ ����������� ��������
    \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
  virtual uint_8 WriteBlock( cpvoid src, uint32 count, CmtEvent* pBreak ) { return Transfer( src, 0, count, pBreak ); }
  };

//! SPI �����
struct CmtSpiPipeBase : public CmtSpiBase {
  
  CmtSpiPipeBase( CmtTransfer *rx = 0, CmtTransfer *tx = 0 ) : CmtSpiBase(rx,tx) {}
  
  virtual void Select() = 0;
  virtual void UnSelect() = 0;

  };

//! SPI �����, ������������� ����� ��������� SPI
struct CmtSpiPipeLinked : public CmtSpiPipeBase {
  CmtSpiBase *mSpi;
    
  CmtSpiPipeLinked( CmtSpiBase *spi ) : CmtSpiPipeBase(), mSpi(spi) {}
    
  //! �������� ������ �� SPI
  /*! ��������� �������� ������ ������ �� SPI. ������� ���������� ��������
������� ���������� ����� � ������ ������� ����������� ��������
    \param src - ���� ��� ��������
    \param dest - ��������� �� ����� ��� ���������� ��������� �����
    \param pBreak ������ ����������� ��������
    \return ����, ���� �������� ��������� ������� � ��� ������ � ��������� ������ */
  virtual uint_8 Transfer( cpvoid src, pvoid dest, uint32 count, CmtEvent* pBreak ) { return mSpi->Transfer( src, dest, count, pBreak ); }

  virtual uint_8 GetState() { return mSpi->GetState(); }
  
  //! �������� ���� ������
  /*! ��������� ������� �� ����� ����� ������ � ��������� �����.
    \param dest - ����� ��� �������� ������
    \param count - ���������� ����������� ������
    \param pBreak ������ ����������� ��������
    \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
  virtual uint_8 ReadBlock( pvoid dest, uint32 count, CmtEvent* pBreak ) { return mSpi->ReadBlock( dest, count, pBreak ); }
  
  //! �������� ���� ������
  /*! ��������� ������� �� �������� ����� ������ �� ���������� ������.
    \param src - ����� ������ ��� ��������
    \param count - ���������� ������������ ������
    \param pBreak ������ ����������� ��������
    \return ���� ����� ��� ������ ��� ��� ������ � ��������� ������ */
  virtual uint_8 WriteBlock( cpvoid src, uint32 count, CmtEvent* pBreak ) { return mSpi->WriteBlock( src, count, pBreak ); }

  };

//���������� ����������
#if defined( CMT_STM32F105VCT6 ) || defined( CMT_STM32F103CBT6 )
  #include <Sys/Dev/Abstract/Spi/cmtSTM32F10x.h>
#elif defined(CMT_TI_OMAP3530)
  #include <Sys/Dev/Abstract/Spi/cmtTiOmap3530.h>
#else
  #error "CMT: SPI not supported for this CPU"
#endif

