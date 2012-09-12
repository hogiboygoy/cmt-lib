//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

/**
 * \defgroup helperFuncs Функции общего назначения
 * @{
 */
void cmtOneToHexUpper( uint_8 dig, pchar dest );

void cmtMultiToHexUpper( uint32 dig, pchar dest, uint_8 num );

pchar cmtSPrintStr( pchar dest, cpchar src );

pchar cmtSPrintHexUpper( pchar dest, uint32 value, uint_8 num );

pchar cmtSPrintUInt( pchar dest, uint32 value );

//! Преобразование hex символа в число
//! \param ch - hex-символ (маленькие, большие буквы - без разницы)
//! \return число, соответствующее символу
int cmtTransHex( char ch );

//! Преобразование строки hex символов в число
//! \param str - строка hex-символов (маленькие, большие буквы - без разницы), завершается нулем
//! \return число, соответствующее строке
int atoh( cpchar str );

//! Преобразование двух десятичных символов к числу
//! \param str строка двух символов
//! \return число, соответствующее строке
uint_8 cmtTwoDigitToInt( cpchar str );

//! Преобразование символа к нижнему регистру (маленькие буквы).
//! Включая русский алфавит.
//! \param c входной символ
//! \return символ в нижнем регистре
uint8 cmtCharLower( uint8 c );

//! Преобразование символа к верхнему регистру (большие буквы).
//! Включая русский алфавит.
//! \param c входной символ
//! \return символ в верхнем регистре
uint8 cmtCharUpper( uint8 c );

//! Сравнение символов без учета регистра
uint_8 cmtCompareChar( uint8 c1, uint8 c2 );

//! Сравнение строк без учета регистра (строки равны, когда равны все знаки и совпадает длина)
//! \param s1 строка 1
//! \param s2 строка 2
//! \return -1 строка 1 меньше строки 2, 0 строки равны, 1 строка 1 больше строки 2
int cmtCompareStrNoReg( cpchar s1, cpchar s2 );

//! Сравнение строки с образцом без учета регистра (образец может быть короче строки)
//! \param pat образец
//! \param str строка
//! \return true когда есть совпадение с образцом, false в противном случае
bool cmtComparePatNoReg( cpchar pat, cpchar str );

//! Сравнение строк с ограничением по длине
//! \param src1 строка 1
//! \param src2 строка 2
//! \param len максимальное количество символов для сравнения
//! \return 0 строки равны, 1 строк не равны
uint_8 cmtStrNCmp( const uint8 *src1, fcpchar src2, uint_8 len );

//! Копирование строк с ограничением по длине
void strmcpy( pchar dest, cpchar sour, uint32 maximum );

//! Преобразование строки к нижнему регистру.
//! Включая русский алфавит. Преобразование производится на месте заменой символов.
//! \param str строка для преобразования
//! \return результат замещает входную строку
void cmtStrLower( pchar str );

//! Преобразование строки к верхнему регистру.
//! Включая русский алфавит. Преобразование производится на месте заменой символов.
//! \param str строка для преобразования
//! \return результат замещает входную строку
void cmtStrUpper( pchar str );

//! Поиск символа в строке с конца строки
//! \param sour строка для поиска
//! \param ch искомый символ
//! \return указатель на найденный символ первый с конца или нуль, если символов не найдено
pchar cmtStrFindBack( pchar sour, char ch );

//! Поиск символа в строке с конца строки
//! \param sour строка для поиска
//! \param ch искомый символ
//! \return указатель на найденный символ первый с конца или нуль, если символов не найдено
cpchar cmtStrFindBack( cpchar sour, char ch );

//! Поиск подстроки в строке без учета регистра
/*! \param str строка, в которой производится поиск
  \param subStr искомая подстрока
  \return указатель на подстроку в строке или нуль, если подстроки не найдено */
cpchar cmtStrStrNoReg( cpchar str, cpchar subStr );

#ifdef __cplusplus
inline int cmtIMin( int i1, int i2 ) { return i1 < i2 ? i1 : i2; }
inline int cmtIMax( int i1, int i2 ) { return i1 > i2 ? i1 : i2; }
inline int cmtAbs( int i1 ) { return i1 < 0 ? -i1 : i1; }

inline uint32 cmtUMin( uint32 i1, uint32 i2 ) { return i1 < i2 ? i1 : i2; }
inline uint32 cmtUMax( uint32 i1, uint32 i2 ) { return i1 > i2 ? i1 : i2; }

//! Ограничение (включительно, т.е. значения limMin и limMax - допустимы)
inline int32 cmtILimit( int32 val, int32 limMin, int32 limMax ) { if( val < limMin ) return limMin; if( val > limMax ) return limMax; return val; }

//! Заворачивание, значение limMin - допустимо, меньшие значения заворачиваются на limMax - 1, таким образом limMax не входит в диапазон
inline int32 cmtIWrap( int32 val, int32 limMin, int32 limMax ) { if( val < limMin ) return limMax - 1; if( val >= limMax ) return limMin; return val; }
#else
#define cmtMin( i1, i2 ) ((i1) < (i2) ? (i1) : (i2))
#define cmtMax( i1, i2 ) ((i1) > (i2) ? (i1) : (i2))
#define cmtAbs( i1 )     ((i1) < 0 ? -(i1) : (i1))
#endif

  /**
 * @}
 */
