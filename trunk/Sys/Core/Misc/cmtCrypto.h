//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! Вычисление CRC32 с кодом 0xEDB88320 
uint32 cmtCrc32( const void *src, uint32 len, uint32 crc = 0xFFFFFFFF );

//! Вычисление функции "Хеш функция Дженкинса" 
//! http://ru.wikipedia.org/wiki/%D0%A5%D0%B5%D1%88_%D1%84%D1%83%D0%BD%D0%BA%D1%86%D0%B8%D1%8F_%D0%94%D0%B6%D0%B5%D0%BD%D0%BA%D0%B8%D0%BD%D1%81%D0%B0
uint32 cmtJenkinsHash( const void *src, uint32 len, uint32 hash = 0 );
