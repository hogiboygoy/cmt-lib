//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

/*������ ����������� �������. � ������� �� ����������� ����������, ����������� � ����
  ��������, ��� ���������� ��������� � ���� ������� ������� � ����� � �������������
  ����������� "��������� ������������� ��������, ��������� ��������� ��������������
  �� ����� embedded �������������". � ����������, ������ ������ ������� �� ��� ������
  �����, ������� ��������� ������ �� ��� ����� ��������� ������.
  */

#ifndef CMT_BUFFER_H
#define CMT_BUFFER_H

//! ����� ����������� ������� � ���������� uint32
class CmtBuffer32 {
    uint32 *mBuffer; //!< ���������� �����
    uint32  mSize;   //!< ������ ������
  public:
    
    //! ����������� ������
    /*! ������� ����� � �������� ����������� ���������
      \param size ���������� ��������� ���� uint32 */
    CmtBuffer32( uint32 size = 0 );
    
    //! ����������. ����������� ������, ������� �������
    ~CmtBuffer32() { if( mBuffer ) delete mBuffer; }
    
    //! ������ ������ � ������. � ���������� ������ ��������� ����� �������� ����� ������������
    uint32* Buffer() { return mBuffer; }
    
    //! ������� ������������ ������ ������.
    uint32  Size() const { return mSize; }
    
    //! ���������������� ������ ��� �����.
    /*! ���������������� ������ ��� ����� � ����� ��������. ������ �����������������
      ������������ ������ ���� ����� ������ ������ �����������. ���������� �������
      ������ �� ������������.
      \param size ����� ������ ������ � ��������� uint32
      \return ������, � ������ ������, ��� ���� ���� ����������������� ����������� ��������,
      ��� ���� ����� ��������� ������� ������ � ���������� */
    bool    Resize( uint32 size );
  };

//! ����� ����������� ������� � ���������� ���� uint8.
/*! ����� ����������� ������� � ���������� ���� uint8 ���������� � ���� ������
  � ���������� ���� uint32 � ��������������� �����. ��� ������� �� ���� ��������:
  �� ������ ��� ���������� ��������� (��������� �������������� ������ uint32), ��
  ������ ��� ��������� ������� � ������������� ��� ��������� ����������� */
class CmtBuffer8 {
    CmtBuffer32 mBuffer; //!< ���������� �����. ���������� � ���� ������ uint32.
  public:
    //! ����������� ������
    /*! ������� ����� � �������� ����������� ���������
      \param size ���������� ��������� ���� uint8 */
    CmtBuffer8( uint32 size = 0 ) : mBuffer( (sizeof(uint32) - 1 + size) / sizeof(uint32) ) {}
    
    //! ����������. ����������� ������, ������� �������
    ~CmtBuffer8() {}
    
    //! ������ ������ � ������. � ���������� ������ ��������� ����� �������� ����� ������������
    uint8* Buffer() { return (uint8*)(mBuffer.Buffer()); }
    
    //! ���������� ������ � ���� ������
    char*  Str() { return (char*)(mBuffer.Buffer()); }
    
    //! ������� ������������ ������ ������.
    uint32 Size() const { return mBuffer.Size() * sizeof(uint32); }
    
    //! ���������������� ������ ��� �����.
    /*! ���������������� ������ ��� ����� � ����� ��������. ������ �����������������
      ������������ ������ ���� ����� ������ ������ �����������. ���������� �������
      ������ �� ������������.
      \param size ����� ������ ������ � ��������� uint32
      \return ������, � ������ ������, ��� ���� ���� ����������������� ����������� ��������,
      ��� ���� ����� ��������� ������� ������ � ���������� */
    bool   Resize( uint32 size ) { return mBuffer.Resize( (sizeof(uint32) - 1 + size) / sizeof(uint32) ); }
  };
#endif
