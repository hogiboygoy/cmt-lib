//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_CONSOLE_H
#define CMT_CONSOLE_H

class CmtConsole {
    CmtContext *mScreen;  //!< Экран
    char       *mConsole; //!< Массив памяти для хранения символов
    CmtPoint    mSize;    //!< Размер экрана в символах
  public:
    
    CmtConsole( CmtContext *screen, CmtPoint sizeInSymbols );
    
    //! Печать только одной строки
    void PrintS_W( cpchar str );
    
    //! Печать нескольких строк
    void PrintS( cpchar str );
    
    //! Прокрутить экран
    void Scroll();
    
    //! Обновить экран
    void Update();
    
    void PrintAndUpdate( cpchar str );
  };

#endif
