//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

#if 0
struct CmtGeoPoint : public CmtPoint {

    //! Долгота в сотых долях минут (отрицательные - E восток, положительные - W запад)
    int32 Longi() const { return x; }

    //! Широта в сотых долях минут (отрицательные - S юг, положительные - N север)
    int32 Latti() const { return y; }

    void  GeoSet( int32 latti, int32 longi ) { y = latti; x = longi; }
  };
#endif

//! Структура сообщения GPRMC
struct CmtGpsGprmc {
  CmtSystemTime mDateTime;  //!< Дата и время
  int32         mLongitude; //!< Долгота в сотых долях минут (отрицательные - W запад, положительные - E восток)
  int32         mLattitude; //!< Широта в сотых долях минут (отрицательные - S юг, положительные - N север)
  uint32        mSpeed;     //!< Скорость у поверхности земли в 10м/с
  uint8         mCourse;    //!< Курс
  uint8         mActive;    //!< Активность 0-спутники не видны, 1-спутники видны
  uint8         mReal;      //!< Достоверность данных, 0-данные не достоверны, 1-данные достоверны
  };

//Получение градусов из координат mLxx / 6000
inline int32 cmtGeoGrad( int32 coord ) { return coord < 0 ? -coord / 6000 : coord / 6000; }

//Получение десятитысячных долей градуса (mLxx % 6000) * 10000 / 6000
inline int32 cmtGeoTtpg( int32 coord ) { return coord < 0 ? (-coord % 6000) * 10000 / 6000 : (coord % 6000) * 10000 / 6000; }

//! Структура описания спутника
struct CmtGpsSatellitInfo {
  uint8  mNumber;     //!< Номер спутника
  uint8  mElevation;  //!< Подъем над горизонтом, в градусах
  uint8  mAzimuth;    //!< Азимут, в градусах (0 градусов - север, плюс-по часовой стрелке)
  uint8  mSNR;        //!< Отношение сигнал/шум в dB, 0 - когда не виден
  };

//! Структура сообщения GPGSV
struct CmtGpsGpgsv {
  uint8              mSatellitesInView; //!< Количество видимых спутников
  CmtGpsSatellitInfo mInfo[4];          //!< Описание спутников
  uint8              mReal;             //!< Достоверность данных, 0-данные не достоверны, 1-данные достоверны
  };

//! Получить команду NMEA
/*! Ожидает прихода очередной команды NMEA и копирует ее в буфер с
  максимальной заданной длиной.
  \param serial последовательный порт, с которого ожидаются команды
  \param dest приемный буфер
  \param destSize размер приемного буфера с учетом завершающего нулевого символа
  \param pBreak внешнее событие прекращения ожидания команды, может быть нуль
  \return CMTE_OK когда команда принята, CMTE_BREAK если тайм-аут */
uint_8 cmtGpsGetNMEA( CmtFile *serial, uint8 *dest, uint32 destSize, CmtEvent *pBreak );

//! Декодировать сообщение RMC
/*! Декодирует сообщение RMC и заполняет соответствующие поля структуры.
  \param buf буфер с сообщением
  \param rmc указатель на структуру, которая заполняется при разборе
  \return CMTE_OK когда разбор успешный или код ошибки в противном случае */
uint_8 cmtGpsDecodeGprmc( uint8 *buf, CmtGpsGprmc *rmc );

//! Декодировать сообщение GSV
/*! Декодирует сообщение GSV и заполняет соответствующие поля структуры.
  \param buf буфер с сообщением
  \param gsv указатель на структуру, которая заполняется при разборе
  \return CMTE_OK когда разбор успешный или код ошибки в противном случае */
uint_8 cmtGpsDecodeGpgsv( uint8 *buf, CmtGpsGpgsv *gsv );

