//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
/** \defgroup core ������� � ������ ���������� �������� � ���������
 *  @{
 */

#ifndef CMT_CORE_H
#define CMT_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

//! ��������������� ���������� ������
typedef struct CmtTask CmtTask;

extern CmtTask *cmtCurTask;   //!< ��������� �� ������� ������
extern CmtTask *cmtNextTask;  //!< ��������� �� ��������� ������

#ifdef __cplusplus
 }
#endif

//! ������ �������
struct CmtEvent {
    CmtEvent *mNext; //!< ��������� ������� � ������ �������������� �������

    //! �����������
    CmtEvent() : mNext(0) {}

    //! ���������� ��������� �������
    virtual bool IsSignaled() { return false; }

    //! �������� ����������� �������
    void Wait();

    //! �������� ����������� ������ �� ���� �������
    void Wait2( CmtEvent *ev );

    //! �������� ����������� ������ �� ���� �������
    void Wait3( CmtEvent *ev1, CmtEvent *ev2 );
  };

//! �������� ������� ����������� ������������ �������

//! \param ptr ������������ �������� �������, ������������ ��� ������ ::cmtWaitEvent
//! \result ���������� ���� ��� ���������� ������� � �� ���� - ��� �������
typedef uint_8 (*CmtSignaled)( void *ptr );

//! �������� ������� ������
typedef void (*CmtTaskProc)();

//! ������������ �����
void cmtSwitchTask();

//! ��������� ������
struct CmtTask {
  CmtPortStackPtr mStack;     //!< ��������� ����� ������
  CmtTask        *mNext;      //!< ��������� ������ � ������
  void           *mEvent;     //!< ������������ ������ ��� ����������� ������������
  CmtSignaled     mSignaled;  //!< ��������� �� ������� ����������� ������������
  };

#define cmtTaskParam() (cmtCurTask->mEvent)


//! �������� ������ � ������ �����

//! \param task - ������
void cmtTaskAppend( CmtTask *task );



//! ������� ������ �� ������ �����

//! \param task - ������ ��� �������� (������� ������ �� ����� ���� �������)
void cmtTaskRemove( CmtTask *task );


//! ��������� ������

//! \param task - ��������� �� ��������� ������
//! \param proc - ������� ������
//! \param ptr  - �������� ������ (��������� ��� ������ ������� ������ ����������� ��������)
//! \param dataStack - �������� ����� ������ (����������� AVR)
//! \param callStack - �������� ����� ������� (����������� AVR)
void cmtTaskAdd( CmtTask *task, CmtTaskProc proc, void *ptr, int dataStack, int callStack );


//! ��������� ������

//! \param task - ��������� �� ��������� ������
//! \param proc - ������� ������
//! \param param - �������� ������ (��������� ��� ������ ������� ������ ����������� ��������)
//! \param stack - ��������� �� ���� (��������� ������ ��� �������, ��� ���� ������ ����)
void cmtTaskAdd32( CmtTask *task, CmtTaskProc proc, void *param, void *stack );


//! ��������� ������� ������

//! \param task - ��������� �� ��������� ������
//! \param oneStep - ������� ������� ������ ����
void cmtTaskAddStep( CmtTask *task, CmtSignaled oneStep );



//! �������� ������

//! ��������� ��������� ������, ���� ��������� ������� � ��������� ������.
//! \param name - ������� ������
//! \param param - �������� ������ (��������� ��� ������ ������� ������ ����������� ��������)
//! \param stackSize - ������ �����, ���������� � ������
#define CMT_TASK_ADD( name, param, stackSize ) \
  static CmtTask name##Task; \
  static uint32  name##Stack[stackSize]; \
  cmtTaskAdd32( &name##Task, name, param, name##Stack+stackSize-1 );
 


//! �������� ����������� �������

//! \param signaled - ������� ����������� ����������� ������� ::CmtSignaled
//! \param ev       - �������� ��� �������
void cmtWaitEvent( CmtSignaled signaled, void *ev );


//! ����������� �������, ������� ������ ���������

//! \param ptr ��������� �� ������ ������� (�� ������������)
//! \result ���������� 1 ������������, ��� ������� ���������
uint_8 cmtSignaled( void *ptr );

//! \param ptr ��������� �� ������ ������� (�� ������������)
//! \result ���������� 0 ������������, ��� ������� �� ���������
uint_8 cmtUnSignaled( void *ptr );

//! ����������� inline ������� ��� ��������� ������
inline void cmtWaitSignaled() { cmtWaitEvent( cmtSignaled, 0 ); }
inline void cmtWaitUnSignaled() { cmtWaitEvent( cmtUnSignaled, 0 ); }


//==================================================================================
//                                  �������

//! ��������� ���� � 8-��������� ��������
struct CmtEvSetBit8 : public CmtEvent {
    uint8 *mReg;  //!< ����� ����������� ��������
    uint8  mMask; //!< ����� ����������� �����

    CmtEvSetBit8( uint8 *reg, uint8 mask ) : CmtEvent(), mReg(reg), mMask(mask) {}

    virtual bool IsSignaled() { return (mReg[0] & mMask) != 0; }

    uint8 Mask() const { return mReg[0] & mMask; }

    void WaitBit( uint8 mask ) { mMask = mask; Wait(); }
    
    void WaitBit2( uint8 mask, CmtEvent *ev ) { mMask = mask; Wait2( ev ); }
  };

//! ����� ���� � 8-��������� ��������
struct CmtEvClrBit8 : public CmtEvSetBit8 {
    CmtEvClrBit8( uint8 *reg, uint8 mask ) : CmtEvSetBit8( reg, mask ) {}

    virtual bool IsSignaled() { return ((~mReg[0]) & mMask) != 0; }
  };

//! ��������� ���� � 16-��������� ��������
struct CmtEvSetBit16 : public CmtEvent {
    uint16 *mReg;  //!< ����� ����������� ��������
    uint16  mMask; //!< ����� ����������� �����

    CmtEvSetBit16( uint16 *reg, uint16 mask ) : CmtEvent(), mReg(reg), mMask(mask) {}

    virtual bool IsSignaled() { return (mReg[0] & mMask) != 0; }

    uint16 Mask() const { return mReg[0] & mMask; }

    void WaitBit( uint16 mask ) { mMask = mask; Wait(); }
    
    void WaitBit2( uint16 mask, CmtEvent *ev ) { mMask = mask; Wait2( ev ); }
    
  };

//! ����� ���� � 16-��������� ��������
struct CmtEvClrBit16 : public CmtEvSetBit16 {
    CmtEvClrBit16( uint16 *reg, uint16 mask ) : CmtEvSetBit16( reg, mask ) {}

    virtual bool IsSignaled() { return ((~mReg[0]) & mMask) != 0; }
  };

//! ��������� ���� � 32-��������� ��������
struct CmtEvSetBit32 : public CmtEvent {
    uint32 *mReg;  //!< ����� ����������� ��������
    uint32  mMask; //!< ����� ����������� �����

    CmtEvSetBit32( uint32 *reg, uint32 mask ) : CmtEvent(), mReg(reg), mMask(mask) {}

    virtual bool IsSignaled() { return (mReg[0] & mMask) != 0; }

    uint32 Mask() const { return mReg[0] & mMask; }
    
    void WaitBit( uint32 mask ) { mMask = mask; Wait(); }
    
    void WaitBit2( uint32 mask, CmtEvent *ev ) { mMask = mask; Wait2( ev ); }

  };

//! ����� ���� � 32-��������� ��������
struct CmtEvClrBit32 : public CmtEvSetBit32 {
    CmtEvClrBit32( uint32 *reg, uint32 mask ) : CmtEvSetBit32( reg, mask ) {}

    virtual bool IsSignaled() { return ((~mReg[0]) & mMask) != 0; }
  };

//! ��������� ����������
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

//! ������������� cmt (� ����� ����������)
void cmtInitTask();

//! ������� ���� ��� ���������� ������ ���������
typedef void (*CmtStepProc)();



/**@}*/

#endif
