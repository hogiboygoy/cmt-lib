//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_QUEUE_H
#define CMT_QUEUE_H

class CmtQueueEngine {
  protected:
    uint32  mHead;  //!< Голова очереди (вставка)
    uint32  mTail;  //!< Хвост очереди (извлечение)
    uint32  mCount; //!< Максимальное количество элементов в очереди
  public:
    //! Конструктор
    /*! Конструктор очереди
      \param size максимальное количество элементов в очереди */
    CmtQueueEngine( uint32 size ) : mHead(0), mTail(0), mCount(size) {}
    
    //! Возвращает состояние свободности (когда истина - очередь полностью и извлекать нельзя)
    bool   Empty() const { return mHead == mTail; }
    
    //! Возвращает состояние заполненности (когда истина - очередь заполнена и добавлять нельзя)
    bool   Full() const { return cmtIWrap( mHead + 1, 0, mCount ) == (int32)mTail; }
    
    //! Возвращает количество элементов в очереди
    /*! Возвращает количество элементов в очереди.
      \return количество элементов в очереди. */
    uint32 Count() const { return mHead >= mTail ? mHead - mTail : mHead + mCount - mTail; }

    //! Возвращает количество свободных ячеек в очереди
    uint32 Free() const { return (mTail > mHead ? mTail - mHead : mTail + mCount - mHead) - 1; }

    //! Возвращает максимальное количество элементов в очереди
    uint32 Size() const { return mCount - 1; }
    
    //! Возвращает индекс головы (вставка)
    uint32 Head() const { return mHead; }
    
    //! Разместить в очереди
    uint32 Put();
    
    //! Возвращает индекс хвоста (извлечение)
    uint32 Tail() const { return mTail; }
    
    //! Получить из очереди
    uint32 Get();
    
    //! Индекс в очереди элемента с индексом относительно хвоста
    uint32 Look( uint32 index ) { return cmtIWrap( mTail + index, 0, mCount ); }
    
    //! Удалить элементы из очереди
    /*! Удаляет из очереди заданное количество элементов.
      \param count количество удаляемых элементов */
    void   Remove( uint32 count ) { mTail = cmtIWrap( mTail + cmtUMin(count, Free()), 0, mCount ); }
    
    //! Опустошает очередь путем приравнивания указателей головы и хвоста
    /*! Теоретически одна операция не должна прерываться в середине. */
    void   Clear() { mHead = mTail; }

  };

//! Сигнал для работы с очередью. Уровень заполненности
class CmtEvQueueFill : public CmtEvent {
    CmtQueueEngine *mQueue;
    uint32          mCompare;
  public:
    CmtEvQueueFill( CmtQueueEngine *queue, uint32 comp = 1 ) : mQueue(queue), mCompare(comp) {}

    virtual bool IsSignaled() { return mQueue->Count() >= mCompare; }
  };

//! Сигнал для работы с очередью. Уровень свободности
class CmtEvQueueFree : public CmtEvent {
    CmtQueueEngine *mQueue;
    uint32          mCompare;
  public:
    CmtEvQueueFree( CmtQueueEngine *queue, uint32 comp = 1 ) : mQueue(queue), mCompare(comp) {}

    virtual bool IsSignaled() { return mQueue->Free() >= mCompare; }
  };

//! Файл на базе очереди
class CmtQueueFile : public CmtFile {
    uint8          *mBuf;   //! Буфер, содержащий данные файла
    CmtQueueEngine  mQueue; //! Очередь, обеспечивающая механизм файла
  public:
    CmtQueueFile( uint32 size, uint8 *buf = 0 ) : mBuf(buf), mQueue(size) { if( mBuf == 0 ) mBuf = new uint8[size]; }

    CmtQueueEngine* Queue() { return &mQueue; }

    //Побайтный ввод-вывод без проверки очереди
    //! Читает один байт из очереди без проверки наличия
    uint8 Get() { return mBuf[mQueue.Get()]; }

    //! Записывает один байт в очередь без проверки места
    void  Put( uint8 val ) { mBuf[mQueue.Put()] = val; }

    //Блочный ввод-вывод
    //! Читает блок данных заданного размера в буфер
    /*! Выполняет чтение блока данных из файла по текущему указателю
      при этом указатель сдвигается на размер прочитанных байтов.
      \param dest указатель на буфер для чтения данных
      \param size размер блока данных */
    virtual void  Read( pvoid dest, uint32 size );

    //! Пишет блок данных заданного размера из буфера в файл
    /*! Выполняет запись блока данных в файл по текущему указателю
      при этом указатель сдвигается на размер записанных данных.
      \param src указатель на записываемые данные
      \param size размер блока данных */
    virtual void  Write( cpvoid src, uint32 size );

  };


#endif
