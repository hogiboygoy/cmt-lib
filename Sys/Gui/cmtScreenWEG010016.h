//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! ������ ������ ������
#define CMT_WEG010016_ON  1 //�������� �����
#define CMT_WEG010016_OFF 2 //��������� �����

class CmtWEG010016 : public CmtScreen {
public:
  CmtWEG010016() : CmtScreen() {}
  //���������� ���������
  void  WriteC( uint8 val );
  void  WriteD( uint8 val );
  uint8 ReadC();
  uint8 ReadD();

  //! ���������� ������������� ���������� ������
  virtual CmtPoint Size() { return CmtPoint(100,16); }
  
  //! ���������������� �����
  virtual void Init();
  
  //! ���������� ����� ������
  /*! \param mode ����� ����� ������ ������������ ����������� */
  virtual void SetMode( int mode );
  
  //! �������� ���������� ������ �������� ������
  /*! \param color ���� ������� */
  virtual void Clear( uint32 color );
  
  //! ���������� � ����� ������� ��������
  /*! \param src �������� ��������
    \param pos ����� ���������� ���������
    \param size ������ ���������� ��������� ���������
    \param exPos ������ ����������� �������
    \param exSize ������ ����������� ������� */
  virtual void Copy( CmtContext *src, CmtPoint pos, CmtPoint size, CmtPoint exPos, CmtPoint exSize );
  
  //! ���������� � ����� ������� ��������
  /*! ���������� ����������� ����� ���������, ��������������, ��� ������ ��������� ���������
    � �������� ������
    \param src �������� �������� */
  virtual void CopyFull( CmtContext *src );
  };
