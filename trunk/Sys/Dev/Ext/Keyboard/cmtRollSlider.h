//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

class CmtRollSlider {
    uint32       mTime;        //!< Момент смены состояния
    const uint8 *mSequence;    //!< Последовательсность перебора
    uint8        mMask;        //!< Маска задействованных битов
    uint8        mAbsSpeed;    //!< Абсолютное обратное значение скорости
    uint8        mPrevState;   //!< Предыдущее состояние кнопок
    uint8        mSensitivity; //!< Чувствительность к прокручиванию
  public:
    uint8        mSpeed;     //!< Скорость движения (в виде вектора, старший бит - направление, младшие биты - значение скорости)

    CmtRollSlider( const uint8 *seq, uint8 sensitivity );
    
    void Step( uint8 state );
  };

