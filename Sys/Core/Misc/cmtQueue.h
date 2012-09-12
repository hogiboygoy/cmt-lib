//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_QUEUE_H
#define CMT_QUEUE_H

class CmtQueueEngine {
  protected:
    uint32  mHead;  //!< ������ ������� (�������)
    uint32  mTail;  //!< ����� ������� (����������)
    uint32  mCount; //!< ������������ ���������� ��������� � �������
  public:
    //! �����������
    /*! ����������� �������
      \param size ������������ ���������� ��������� � ������� */
    CmtQueueEngine( uint32 size ) : mHead(0), mTail(0), mCount(size) {}
    
    //! ���������� ��������� ����������� (����� ������ - ������� ��������� � ��������� ������)
    bool   Empty() const { return mHead == mTail; }
    
    //! ���������� ��������� ������������� (����� ������ - ������� ��������� � ��������� ������)
    bool   Full() const { return cmtIWrap( mHead + 1, 0, mCount ) == (int32)mTail; }
    
    //! ���������� ���������� ��������� � �������
    /*! ���������� ���������� ��������� � �������.
      \return ���������� ��������� � �������. */
    uint32 Count() const { return mHead >= mTail ? mHead - mTail : mHead + mCount - mTail; }

    //! ���������� ���������� ��������� ����� � �������
    uint32 Free() const { return (mTail > mHead ? mTail - mHead : mTail + mCount - mHead) - 1; }

    //! ���������� ������������ ���������� ��������� � �������
    uint32 Size() const { return mCount - 1; }
    
    //! ���������� ������ ������ (�������)
    uint32 Head() const { return mHead; }
    
    //! ���������� � �������
    uint32 Put();
    
    //! ���������� ������ ������ (����������)
    uint32 Tail() const { return mTail; }
    
    //! �������� �� �������
    uint32 Get();
    
    //! ������ � ������� �������� � �������� ������������ ������
    uint32 Look( uint32 index ) { return cmtIWrap( mTail + index, 0, mCount ); }
    
    //! ������� �������� �� �������
    /*! ������� �� ������� �������� ���������� ���������.
      \param count ���������� ��������� ��������� */
    void   Remove( uint32 count ) { mTail = cmtIWrap( mTail + cmtUMin(count, Free()), 0, mCount ); }
    
    //! ���������� ������� ����� ������������� ���������� ������ � ������
    /*! ������������ ���� �������� �� ������ ����������� � ��������. */
    void   Clear() { mHead = mTail; }

  };

//! ������ ��� ������ � ��������. ������� �������������
class CmtEvQueueFill : public CmtEvent {
    CmtQueueEngine *mQueue;
    uint32          mCompare;
  public:
    CmtEvQueueFill( CmtQueueEngine *queue, uint32 comp = 1 ) : mQueue(queue), mCompare(comp) {}

    virtual bool IsSignaled() { return mQueue->Count() >= mCompare; }
  };

//! ������ ��� ������ � ��������. ������� �����������
class CmtEvQueueFree : public CmtEvent {
    CmtQueueEngine *mQueue;
    uint32          mCompare;
  public:
    CmtEvQueueFree( CmtQueueEngine *queue, uint32 comp = 1 ) : mQueue(queue), mCompare(comp) {}

    virtual bool IsSignaled() { return mQueue->Free() >= mCompare; }
  };

//! ���� �� ���� �������
class CmtQueueFile : public CmtFile {
    uint8          *mBuf;   //! �����, ���������� ������ �����
    CmtQueueEngine  mQueue; //! �������, �������������� �������� �����
  public:
    CmtQueueFile( uint32 size, uint8 *buf = 0 ) : mBuf(buf), mQueue(size) { if( mBuf == 0 ) mBuf = new uint8[size]; }

    CmtQueueEngine* Queue() { return &mQueue; }

    //��������� ����-����� ��� �������� �������
    //! ������ ���� ���� �� ������� ��� �������� �������
    uint8 Get() { return mBuf[mQueue.Get()]; }

    //! ���������� ���� ���� � ������� ��� �������� �����
    void  Put( uint8 val ) { mBuf[mQueue.Put()] = val; }

    //������� ����-�����
    //! ������ ���� ������ ��������� ������� � �����
    /*! ��������� ������ ����� ������ �� ����� �� �������� ���������
      ��� ���� ��������� ���������� �� ������ ����������� ������.
      \param dest ��������� �� ����� ��� ������ ������
      \param size ������ ����� ������ */
    virtual void  Read( pvoid dest, uint32 size );

    //! ����� ���� ������ ��������� ������� �� ������ � ����
    /*! ��������� ������ ����� ������ � ���� �� �������� ���������
      ��� ���� ��������� ���������� �� ������ ���������� ������.
      \param src ��������� �� ������������ ������
      \param size ������ ����� ������ */
    virtual void  Write( cpvoid src, uint32 size );

  };


#endif
