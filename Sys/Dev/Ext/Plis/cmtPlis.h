//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
void cmtInitPlis();

//! Базовый загрузчик ПЛИС
struct CmtPlisBase {
  CmtPlisBase() {}
  
  //! Загрузка в PLIS конфигурации
  /*! Производит конфигурирование ПЛИС с текущей файловой позиции. По завершении
    файл НЕ закрывается.
    \param src - файл, содержащий данные конфигурирования
    \param dst - файл-шлюз для загрузки ПЛИС
    \param size - размер данных для загрузки
    \return CMTE_OK если загружено успешно или код ошибки в противном случае */
  virtual uint_8 Config( CmtFile &src, CmtFile &dst, uint32 size ) = 0;
  };

struct CmtPlisAltera : public CmtPlisBase {
    CmtPlisAltera() : CmtPlisBase() {}
    
    //! Выполняет сброс ПЛИС, инициируя процесс загрузки
    virtual void Reset( bool on ) = 0;
    
    //! Проверяет сигнал успешности загрузки
    virtual bool Done() = 0;

    //! Загрузка в PLIS конфигурации
    /*! Производит конфигурирование ПЛИС с текущей файловой позиции. По завершении
      файл НЕ закрывается.
      \param src - файл, содержащий данные конфигурирования
      \param dst - файл-шлюз для загрузки ПЛИС
      \param size - размер данных для загрузки
      \return CMTE_OK если загружено успешно или код ошибки в противном случае */
    virtual uint_8 Config( CmtFile &src, CmtFile &dst, uint32 size );
  };

//Стандартные размеры ПЛИСов
#define CMT_ALTERA_EP3C10 368011
#define CMT_ALTERA_EP3C25 718569
