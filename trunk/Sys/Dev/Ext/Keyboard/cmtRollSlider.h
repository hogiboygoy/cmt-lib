//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

class CmtRollSlider {
    uint32       mTime;        //!< ������ ����� ���������
    const uint8 *mSequence;    //!< ������������������� ��������
    uint8        mMask;        //!< ����� ��������������� �����
    uint8        mAbsSpeed;    //!< ���������� �������� �������� ��������
    uint8        mPrevState;   //!< ���������� ��������� ������
    uint8        mSensitivity; //!< ���������������� � �������������
  public:
    uint8        mSpeed;     //!< �������� �������� (� ���� �������, ������� ��� - �����������, ������� ���� - �������� ��������)

    CmtRollSlider( const uint8 *seq, uint8 sensitivity );
    
    void Step( uint8 state );
  };

