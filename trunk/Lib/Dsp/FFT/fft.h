/*
    Fast Fourier Transformation
    ====================================================
    Coded by Miroslav Voinarovsky, 2002
    This source is freeware.
*/

#ifndef FFT_H_
#define FFT_H_

struct Complex;
struct ShortComplex;

struct ShortComplex {
  double re, im;
  inline void operator=(const Complex &y);
  };

struct Complex {
  long double re, im;
  inline void operator= (const Complex &y);
  inline void operator= (const ShortComplex &y);
  };


inline void ShortComplex::operator=(const Complex &y)    { re = (double)y.re; im = (double)y.im;  }
inline void Complex::operator= (const Complex &y)       { re = y.re; im = y.im; }
inline void Complex::operator= (const ShortComplex &y)  { re = y.re; im = y.im; }

class CmtSpectrometr {
    int           T;
    ShortComplex *Wstore;
  public:
    ShortComplex *mArr;    //��� ����������� �����, ��������� ������� ����������� �������������� �����
                           //ShortComplex - ��� ��������� � ������: double re, im;
    double       *mAmp;    //������ ��������, ���������� � ���������� �������

    //! pow ������� ������ ��� ���������� �������
    CmtSpectrometr( int pow );
    ~CmtSpectrometr();

    //! ���������� �������� �������������
    void Analis();

    //! Fast Fourier Transformation
    void Fft( bool complement = false );

    //! ���������� ���������
    int  Count() const { return 1 << T; }
  };

#endif
