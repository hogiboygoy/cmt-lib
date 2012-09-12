//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! јппаратный порт SPI. 
/*! ќсобенность порта в том, что он автоматически управл€ет выводом CS.
 CS устанавливаетс€ активным (низким или высоким уровнем в зависимости от
 настроек) уровнем на момент передачи слова. ќчень часто CS следует держать
 активным в течение передачи блока. ƒл€ этой возможности нужно использовать
 CmtSpiSoft с pipe установленным в один из каналов CmtSpiPipeHard */
struct CmtSpiPipeHard : public CmtSpiPipeBase {
  uint32 mCs;   //!< »ндекс канала
  uint32 mBase; //!< Ѕазовый адрес порта
  
  CmtSpiPipeHard(uint32 base, uint32 cs, CmtTransfer *rx = 0, CmtTransfer *tx = 0 );
  
  virtual void Select();
  virtual void UnSelect();

  //! ќпераци€ обмена по SPI
  /*! ¬ыполн€ет операцию обмена байтом по SPI. ќжидает завершени€ операции
прихода очередного байта с учетом сигнала прекращени€ операции
    \param src - указатель исходного буфера дл€ передачи
    \param dest - указатель на место дл€ размещени€ прин€тых байтов
    \param pBreak сигнал прекращени€ операции
    \return нуль, если операци€ завершена успешно и код ошибки в противном случае */
  virtual uint_8 Transfer( cpvoid src, pvoid dest, uint32 count, CmtEvent* pBreak );

  };


class CmtSpiPipeSoft : public CmtSpiPipeLinked {
    uint32 mCsPin; //!< Ќомер вывода CS
  public:
    CmtSpiPipeSoft( CmtSpiPipeHard *pipe, uint32 pin );

    virtual void Select();
    
    virtual void UnSelect();
  };
