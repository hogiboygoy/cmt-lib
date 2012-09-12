//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Описание
//   Файл содержит коды модулей для отладки системы
//   При необходимости отладки внесите сюда очередной номер и приведите
//   имя модуля в файле исходного кода
//==============================================================================
#ifndef CMTDEBUG_CPP
#define CMTDEBUG_CPP

#ifndef cmtDebugInfo
  #define cmtDebugInfo( module, classe, code, message, param1, param2, param3, param4 )
#endif

//cmtSdTiOmap3530.cpp
#ifndef cmtDebug1
  #define cmtDebug1( classe, code, message, param1 )
#endif

//cmtFileSystemBase
#ifndef cmtDebug2
  #define cmtDebug2( classe, code, message, param1 )
#endif

//cmtElfAbsParser
#ifndef cmtDebug3
  #define cmtDebug3( classe, code, message, param1 )
#endif

//cmtTiOmap3530 i2c
#ifndef cmtDebug4
  #define cmtDebug4( classe, code, message, param1 )
#endif

#endif // CMTDEBUG_H
