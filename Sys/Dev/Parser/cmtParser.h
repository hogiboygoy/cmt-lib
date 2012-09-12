//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Описание
//   Файл содержит описание абстрактного разборщика
//==============================================================================

//! Копирование бинарных данных из источника в приемник с заданным размером
uint_8 cmtParseBinCopy( CmtFile &src, CmtFile &dst, uint32 size );

uint_8 cmtParseBinCopyBuf( CmtFile &src, CmtFile &dst, uint32 size );

//! ELF загрузчик (разборщик)
uint_8 cmtParseElf( CmtFile &src, CmtFile &dst, uint32 *start );

//! ELF проба
uint_8 cmtProbeElf( CmtFile &src );

