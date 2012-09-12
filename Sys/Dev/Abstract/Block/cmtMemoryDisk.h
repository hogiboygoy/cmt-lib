//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMTMEMORYDISK_H
#define CMTMEMORYDISK_H

struct CmtMemoryDisk : public CmtBlockDevice {
  uint32 *mDiskImage; //!< ����� � ������� �����
  
  //! �����������
  //! \param blockSize ������ ����� ������ � ������
  //! \param blockCount ���������� ������ � ����������
  CmtMemoryDisk( uint32 blockSize, uint32 blockCount, const uint8 *srcImage, uint32 srcImageSize ); 
  
  //! ������ ������
  //! \param dest - ��������� �� ����� ��� ������ ������
  //! \param block - ����� ���������� ����� ������
  //! \param count - ���������� ������ ������ �������� ������� ����������
  //! \return 0-��� ������, ����� ��� ������
  virtual uint_8 Read( uint32 *dest, uint32 block, uint32 count );

  //! ������ ������
  //! \param src - ��������� �� ����� ������ ��� ������
  //! \param block - ����� ���������� ����� ������
  //! \param count - ���������� ������ ������ �������� ������� ����������
  //! \return 0-��� ������, ����� ��� ������
  virtual uint_8 Write( const uint32 *src, uint32 block, uint32 count );
  
  };

#endif // CMTMEMORYDISK_H
