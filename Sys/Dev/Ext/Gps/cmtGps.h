//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#if 0
struct CmtGeoPoint : public CmtPoint {

    //! ������� � ����� ����� ����� (������������� - E ������, ������������� - W �����)
    int32 Longi() const { return x; }

    //! ������ � ����� ����� ����� (������������� - S ��, ������������� - N �����)
    int32 Latti() const { return y; }

    void  GeoSet( int32 latti, int32 longi ) { y = latti; x = longi; }
  };
#endif

//! ��������� ��������� GPRMC
struct CmtGpsGprmc {
  CmtSystemTime mDateTime;  //!< ���� � �����
  int32         mLongitude; //!< ������� � ����� ����� ����� (������������� - W �����, ������������� - E ������)
  int32         mLattitude; //!< ������ � ����� ����� ����� (������������� - S ��, ������������� - N �����)
  uint32        mSpeed;     //!< �������� � ����������� ����� � 10�/�
  uint8         mCourse;    //!< ����
  uint8         mActive;    //!< ���������� 0-�������� �� �����, 1-�������� �����
  uint8         mReal;      //!< ������������� ������, 0-������ �� ����������, 1-������ ����������
  };

//��������� �������� �� ��������� mLxx / 6000
inline int32 cmtGeoGrad( int32 coord ) { return coord < 0 ? -coord / 6000 : coord / 6000; }

//��������� �������������� ����� ������� (mLxx % 6000) * 10000 / 6000
inline int32 cmtGeoTtpg( int32 coord ) { return coord < 0 ? (-coord % 6000) * 10000 / 6000 : (coord % 6000) * 10000 / 6000; }

//! ��������� �������� ��������
struct CmtGpsSatellitInfo {
  uint8  mNumber;     //!< ����� ��������
  uint8  mElevation;  //!< ������ ��� ����������, � ��������
  uint8  mAzimuth;    //!< ������, � �������� (0 �������� - �����, ����-�� ������� �������)
  uint8  mSNR;        //!< ��������� ������/��� � dB, 0 - ����� �� �����
  };

//! ��������� ��������� GPGSV
struct CmtGpsGpgsv {
  uint8              mSatellitesInView; //!< ���������� ������� ���������
  CmtGpsSatellitInfo mInfo[4];          //!< �������� ���������
  uint8              mReal;             //!< ������������� ������, 0-������ �� ����������, 1-������ ����������
  };

//! �������� ������� NMEA
/*! ������� ������� ��������� ������� NMEA � �������� �� � ����� �
  ������������ �������� ������.
  \param serial ���������������� ����, � �������� ��������� �������
  \param dest �������� �����
  \param destSize ������ ��������� ������ � ������ ������������ �������� �������
  \param pBreak ������� ������� ����������� �������� �������, ����� ���� ����
  \return CMTE_OK ����� ������� �������, CMTE_BREAK ���� ����-��� */
uint_8 cmtGpsGetNMEA( CmtFile *serial, uint8 *dest, uint32 destSize, CmtEvent *pBreak );

//! ������������ ��������� RMC
/*! ���������� ��������� RMC � ��������� ��������������� ���� ���������.
  \param buf ����� � ����������
  \param rmc ��������� �� ���������, ������� ����������� ��� �������
  \return CMTE_OK ����� ������ �������� ��� ��� ������ � ��������� ������ */
uint_8 cmtGpsDecodeGprmc( uint8 *buf, CmtGpsGprmc *rmc );

//! ������������ ��������� GSV
/*! ���������� ��������� GSV � ��������� ��������������� ���� ���������.
  \param buf ����� � ����������
  \param gsv ��������� �� ���������, ������� ����������� ��� �������
  \return CMTE_OK ����� ������ �������� ��� ��� ������ � ��������� ������ */
uint_8 cmtGpsDecodeGpgsv( uint8 *buf, CmtGpsGpgsv *gsv );

