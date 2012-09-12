//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMTMEMORYDISK_H
#define CMTMEMORYDISK_H

struct CmtMemoryDisk : public CmtBlockDevice {
  uint32 *mDiskImage; //!< Ѕуфер с образом диска
  
  //!  онструктор
  //! \param blockSize размер блока данных в байтах
  //! \param blockCount количество блоков в устройстве
  CmtMemoryDisk( uint32 blockSize, uint32 blockCount, const uint8 *srcImage, uint32 srcImageSize ); 
  
  //! „тение блоков
  //! \param dest - указатель на буфер дл€ чтени€ данных
  //! \param block - номер начального блока данных
  //! \param count - количество блоков данных размером данного устройства
  //! \return 0-нет ошибки, иначе код ошибки
  virtual uint_8 Read( uint32 *dest, uint32 block, uint32 count );

  //! «апись блоков
  //! \param src - указатель на буфер данных дл€ записи
  //! \param block - номер начального блока данных
  //! \param count - количество блоков данных размером данного устройства
  //! \return 0-нет ошибки, иначе код ошибки
  virtual uint_8 Write( const uint32 *src, uint32 block, uint32 count );
  
  };

#endif // CMTMEMORYDISK_H
