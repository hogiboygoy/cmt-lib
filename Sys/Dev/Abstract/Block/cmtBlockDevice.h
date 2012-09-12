//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! ќбщее устройство с блоковым доступом (диски, флешки, CD и т.п.)
struct CmtBlockDevice : public CmtDev {
  uint32               mBlockSize;   //!< –азмер блока (сектора)
  uint32               mBlockCount;  //!<  оличество блоков (секторов)
  uint_8               mStatus;      //!< —осто€ние устройства
  CmtEvent            *mBreak;       //!< ¬нешний сигнал прекращающий обмен с блоковым устройством
 
  //!  онструктор
  //! \param blockSize размер блока данных в байтах
  //! \param blockCount количество блоков в устройстве
  CmtBlockDevice( uint32 blockSize, uint32 blockCount ); 
  
  //! „тение блоков
  //! \param dest - указатель на буфер дл€ чтени€ данных
  //! \param block - номер начального блока данных
  //! \param count - количество блоков данных размером данного устройства
  //! \return 0-нет ошибки, иначе код ошибки
  virtual uint_8 Read( uint32 *dest, uint32 block, uint32 count ) = 0;

  //! «апись блоков
  //! \param src - указатель на буфер данных дл€ записи
  //! \param block - номер начального блока данных
  //! \param count - количество блоков данных размером данного устройства
  //! \return 0-нет ошибки, иначе код ошибки
  virtual uint_8 Write( const uint32 *src, uint32 block, uint32 count ) = 0;
  };
