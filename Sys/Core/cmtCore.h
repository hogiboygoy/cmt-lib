//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
/** \defgroup core Функции и классы управления задачами и событиями
 *  @{
 */

#ifndef CMT_CORE_H
#define CMT_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

//! Предварительное объявление задачи
typedef struct CmtTask CmtTask;

extern CmtTask *cmtCurTask;   //!< Указатель на текущую задачу
extern CmtTask *cmtNextTask;  //!< Указатель на следующую задачу

#ifdef __cplusplus
 }
#endif

//! Сигнал события
struct CmtEvent {
    CmtEvent *mNext; //!< Следующие событие в списке контролируемых событий

    //! Конструктор
    CmtEvent() : mNext(0) {}

    //! Возвращает состояние события
    virtual bool IsSignaled() { return false; }

    //! Ожидание наступления события
    void Wait();

    //! Ожидание наступления одного из двух событий
    void Wait2( CmtEvent *ev );

    //! Ожидание наступления одного из трех событий
    void Wait3( CmtEvent *ev1, CmtEvent *ev2 );
  };

//! Прототип функции определения сигнальности события

//! \param ptr произвольный параметр сигнала, передаваемый при вызове ::cmtWaitEvent
//! \result возвращает нуль при отсутствии сигнала и не нуль - при наличии
typedef uint_8 (*CmtSignaled)( void *ptr );

//! Прототип функции задачи
typedef void (*CmtTaskProc)();

//! Переключение задач
void cmtSwitchTask();

//! Структура задачи
struct CmtTask {
  CmtPortStackPtr mStack;     //!< Указатель стека задачи
  CmtTask        *mNext;      //!< Следующая задача в списке
  void           *mEvent;     //!< Произвольные данные для определения сигнальности
  CmtSignaled     mSignaled;  //!< Указатель на функцию определения сигнальности
  };

#define cmtTaskParam() (cmtCurTask->mEvent)


//! Добавить задачу к списку задач

//! \param task - задача
void cmtTaskAppend( CmtTask *task );



//! Удалить задачу из списка задач

//! \param task - задача для удаления (текущая задача не может быть удалена)
void cmtTaskRemove( CmtTask *task );


//! Запустить задачу

//! \param task - указатель на структуру задачи
//! \param proc - функция задачи
//! \param ptr  - параметр задачи (достается при старте функции задачи специальным макросом)
//! \param dataStack - смещение стека данных (архитектура AVR)
//! \param callStack - смещение стека вызовов (архитектура AVR)
void cmtTaskAdd( CmtTask *task, CmtTaskProc proc, void *ptr, int dataStack, int callStack );


//! Запустить задачу

//! \param task - указатель на структуру задачи
//! \param proc - функция задачи
//! \param param - параметр задачи (достается при старте функции задачи специальным макросом)
//! \param stack - указатель на стек (последнюю ячейку при условии, что стек растет вниз)
void cmtTaskAdd32( CmtTask *task, CmtTaskProc proc, void *param, void *stack );


//! Запустить шаговую задачу

//! \param task - указатель на структуру задачи
//! \param oneStep - функция прохода одного шага
void cmtTaskAddStep( CmtTask *task, CmtSignaled oneStep );



//! Добавить задачу

//! Объявляет структуру задачи, стек заданного размера и добавляет задачу.
//! \param name - функция задачи
//! \param param - параметр задачи (достается при старте функции задачи специальным макросом)
//! \param stackSize - размер стека, выраженный в словах
#define CMT_TASK_ADD( name, param, stackSize ) \
  static CmtTask name##Task; \
  static uint32  name##Stack[stackSize]; \
  cmtTaskAdd32( &name##Task, name, param, name##Stack+stackSize-1 );
 


//! Ожидание наступления события

//! \param signaled - функция определения наступления события ::CmtSignaled
//! \param ev       - параметр для функции
void cmtWaitEvent( CmtSignaled signaled, void *ev );


//! Специальное событие, которое всегда наступило

//! \param ptr указатель на данные события (не используется)
//! \result возвращает 1 сигнализируя, что событие наступило
uint_8 cmtSignaled( void *ptr );

//! \param ptr указатель на данные события (не используется)
//! \result возвращает 0 сигнализируя, что событие не наступило
uint_8 cmtUnSignaled( void *ptr );

//! Специальные inline функции для упрощения записи
inline void cmtWaitSignaled() { cmtWaitEvent( cmtSignaled, 0 ); }
inline void cmtWaitUnSignaled() { cmtWaitEvent( cmtUnSignaled, 0 ); }


//==================================================================================
//                                  События

//! Установка бита в 8-разрядном регистре
struct CmtEvSetBit8 : public CmtEvent {
    uint8 *mReg;  //!< Адрес проверямого регистра
    uint8  mMask; //!< Маска проверяемых битов

    CmtEvSetBit8( uint8 *reg, uint8 mask ) : CmtEvent(), mReg(reg), mMask(mask) {}

    virtual bool IsSignaled() { return (mReg[0] & mMask) != 0; }

    uint8 Mask() const { return mReg[0] & mMask; }

    void WaitBit( uint8 mask ) { mMask = mask; Wait(); }
    
    void WaitBit2( uint8 mask, CmtEvent *ev ) { mMask = mask; Wait2( ev ); }
  };

//! Сброс бита в 8-разрядном регистре
struct CmtEvClrBit8 : public CmtEvSetBit8 {
    CmtEvClrBit8( uint8 *reg, uint8 mask ) : CmtEvSetBit8( reg, mask ) {}

    virtual bool IsSignaled() { return ((~mReg[0]) & mMask) != 0; }
  };

//! Установка бита в 16-разрядном регистре
struct CmtEvSetBit16 : public CmtEvent {
    uint16 *mReg;  //!< Адрес проверямого регистра
    uint16  mMask; //!< Маска проверяемых битов

    CmtEvSetBit16( uint16 *reg, uint16 mask ) : CmtEvent(), mReg(reg), mMask(mask) {}

    virtual bool IsSignaled() { return (mReg[0] & mMask) != 0; }

    uint16 Mask() const { return mReg[0] & mMask; }

    void WaitBit( uint16 mask ) { mMask = mask; Wait(); }
    
    void WaitBit2( uint16 mask, CmtEvent *ev ) { mMask = mask; Wait2( ev ); }
    
  };

//! Сброс бита в 16-разрядном регистре
struct CmtEvClrBit16 : public CmtEvSetBit16 {
    CmtEvClrBit16( uint16 *reg, uint16 mask ) : CmtEvSetBit16( reg, mask ) {}

    virtual bool IsSignaled() { return ((~mReg[0]) & mMask) != 0; }
  };

//! Установка бита в 32-разрядном регистре
struct CmtEvSetBit32 : public CmtEvent {
    uint32 *mReg;  //!< Адрес проверямого регистра
    uint32  mMask; //!< Маска проверяемых битов

    CmtEvSetBit32( uint32 *reg, uint32 mask ) : CmtEvent(), mReg(reg), mMask(mask) {}

    virtual bool IsSignaled() { return (mReg[0] & mMask) != 0; }

    uint32 Mask() const { return mReg[0] & mMask; }
    
    void WaitBit( uint32 mask ) { mMask = mask; Wait(); }
    
    void WaitBit2( uint32 mask, CmtEvent *ev ) { mMask = mask; Wait2( ev ); }

  };

//! Сброс бита в 32-разрядном регистре
struct CmtEvClrBit32 : public CmtEvSetBit32 {
    CmtEvClrBit32( uint32 *reg, uint32 mask ) : CmtEvSetBit32( reg, mask ) {}

    virtual bool IsSignaled() { return ((~mReg[0]) & mMask) != 0; }
  };

//! Сравнение переменных
struct CmtEvEq32 : public CmtEvent {
    uint32  mValue;
    uint32 *mTest;

    CmtEvEq32( uint32 val, uint32 *test ) : CmtEvent(), mValue(val), mTest(test) {}

    virtual bool IsSignaled() { return mValue == *mTest; }

  };

struct CmtEvNEq32 : public CmtEvEq32 {
    CmtEvNEq32( uint32 val, uint32 *test ) : CmtEvEq32( val, test ) {}

    virtual bool IsSignaled() { return mValue != *mTest; }

  };

//! Инициализация cmt (в части аппаратуры)
void cmtInitTask();

//! Функция шага для реализации машины состояний
typedef void (*CmtStepProc)();



/**@}*/

#endif
