//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! ����� ���������� � �������� �������� (�����, ������, CD � �.�.)
struct CmtBlockDevice : public CmtDev {
  uint32               mBlockSize;   //!< ������ ����� (�������)
  uint32               mBlockCount;  //!< ���������� ������ (��������)
  uint_8               mStatus;      //!< ��������� ����������
  CmtEvent            *mBreak;       //!< ������� ������ ������������ ����� � �������� �����������
 
  //! �����������
  //! \param blockSize ������ ����� ������ � ������
  //! \param blockCount ���������� ������ � ����������
  CmtBlockDevice( uint32 blockSize, uint32 blockCount ); 
  
  //! ������ ������
  //! \param dest - ��������� �� ����� ��� ������ ������
  //! \param block - ����� ���������� ����� ������
  //! \param count - ���������� ������ ������ �������� ������� ����������
  //! \return 0-��� ������, ����� ��� ������
  virtual uint_8 Read( uint32 *dest, uint32 block, uint32 count ) = 0;

  //! ������ ������
  //! \param src - ��������� �� ����� ������ ��� ������
  //! \param block - ����� ���������� ����� ������
  //! \param count - ���������� ������ ������ �������� ������� ����������
  //! \return 0-��� ������, ����� ��� ������
  virtual uint_8 Write( const uint32 *src, uint32 block, uint32 count ) = 0;
  };
