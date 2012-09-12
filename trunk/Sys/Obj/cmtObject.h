//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

struct CmtObject {
  CmtObject() {}
  };

typedef void (CmtObject::*CmtObjectVoidPtr)(void); 

typedef void (CmtObject::*CmtObjectUInt8Ptr)( uint8 val );

typedef void (CmtObject::*CmtObjectPUInt8Ptr)( uint8 *ptr );

typedef void (CmtObject::*CmtObjectUInt32Ptr)( uint32 val );

//! �������� ����������.
/*! ����������� ��� ����������. ������������ ���������� ����� ����� ��������� �����������
  �������� � ������ �������. */
class CmtSignalVoid {
    CmtObjectVoidPtr  mVoidPtr; //!< ��������� �� �������-���� ���������� �������
    CmtObject        *mObject;  //!< ��������� �� ������ ���������� �������
    CmtSignalVoid    *mNext;    //!< ��������� ������ � ������
  public:
    CmtSignalVoid( CmtObject *obj, CmtObjectVoidPtr objVoidPtr, CmtSignalVoid *next = 0 ) : mVoidPtr(objVoidPtr), mObject(obj), mNext(next) {}

    void Signal() { (mObject->*mVoidPtr)(); if( mNext ) mNext->Signal(); }
  };

//! �������� ����������.
/*! ����������� c ���������� uint8. ������������ ���������� ����� ����� ��������� �����������
  �������� � ������ ������� � ������������ �������� ����� ������ ������ ��������� ���� uint8. */
class CmtSignalUInt8 {
    CmtObjectUInt8Ptr  mUInt8Ptr; //!< ��������� �� �������-���� ���������� �������
    CmtObject         *mObject;   //!< ��������� �� ������ ���������� �������
    CmtSignalUInt8    *mNext;     //!< ��������� ������ � ������
  public:
    CmtSignalUInt8( CmtObject *obj, CmtObjectUInt8Ptr objUint8Ptr, CmtSignalUInt8 *next ) : mUInt8Ptr(objUint8Ptr), mObject(obj), mNext(next) {}

    void Signal( uint8 val ) { (mObject->*mUInt8Ptr)( val ); if( mNext ) mNext->Signal( val ); }
  };

//! �������� ����������.
/*! ����������� c ���������� ��������� �� uint8. ������������ ���������� ����� ����� ��������� �����������
  �������� � ������ ������� � ������������ ��������� ����� ������ ������ ��������� ���� uint8. */
class CmtSignalPUInt8 {
    CmtObjectPUInt8Ptr  mPUInt8Ptr; //!< ��������� �� �������-���� ���������� �������
    CmtObject          *mObject;    //!< ��������� �� ������ ���������� �������
    CmtSignalPUInt8    *mNext;      //!< ��������� ������ � ������
  public:
    CmtSignalPUInt8( CmtObject *obj, CmtObjectPUInt8Ptr objPUint8Ptr, CmtSignalPUInt8 *next = 0 ) :
      mPUInt8Ptr(objPUint8Ptr), mObject(obj), mNext(next) {}
    
    void Signal( uint8 *ptr ) { (mObject->*mPUInt8Ptr)( ptr ); if( mNext ) mNext->Signal( ptr ); }
  };

//! �������� ����������.
/*! ����������� c ���������� uint32. ������������ ���������� ����� ����� ��������� �����������
  �������� � ������ ������� � ������������ �������� ����� ������ ������ ��������� ���� uint32. */
class CmtSignalUInt32 {
    CmtObjectUInt32Ptr  mUInt32Ptr;
    CmtObject          *mObject;
  public:
    CmtSignalUInt32() : mUInt32Ptr(0), mObject(0) {}
    void Signal( uint32 val ) { if( mObject ) (mObject->*mUInt32Ptr)( val ); }
    void Connect( CmtObject *obj, CmtObjectUInt32Ptr objUint32Ptr ) { mObject = obj; mUInt32Ptr = objUint32Ptr; }
  };
