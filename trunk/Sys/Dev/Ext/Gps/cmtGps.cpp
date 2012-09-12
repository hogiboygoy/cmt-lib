//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <stdlib.h>

uint_8
cmtGpsGetNMEA( CmtFile *serial, uint8 *dest, uint32 destSize, CmtEvent *pBreak ) {
  if( serial == 0 || dest == 0 ) return CMTE_BAD_PARAM;
  serial->mBreak = pBreak;
  while(1) {
    serial->Read( dest, 1 );
#ifndef CMT_DESCTOP_QT
    if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;
#endif
    if( dest[0] == '\r' ) continue;
    if( dest[0] == '\n' ) break;
    if( destSize > 1 ) {
      dest++;
      destSize--;
      }
    }
  //Закрыть строку
  dest[0] = 0;
  return CMTE_OK;
  }


static const char szRMC[] = { "$GPRMC" };
static const char szVTG[] = { "$GPVTG" };
static const char szGSV[] = { "$GPGSV" };

uint8
Char2Digit( char ch ) {
  return ch - '0';
  }

uint8
DecodeTwoDigit( cpchar base ) {
  uint8 res = Char2Digit( base[0] );
  res *= 10;
  return res + Char2Digit( base[1] );
  }

cpchar
NextField( cpchar base ) {
  while( base && base[0] ) {
    if( base[0] == ',' ) return base + 1;
    base++;
    }
  return 0;
  }

//Декодировать значение, представить в сотых долях
uint16
DecodeValue100( cpchar base ) {
  uint16 value = 0;
  while( base[0] && base[0] != '.' ) {
    value *= 10;
    value += Char2Digit( *base++ );
    }
  base++;
  value *= 10;
  if( base[0] != ',' ) value += Char2Digit( *base++ );
  value *= 10;
  if( base[0] != ',' ) value += Char2Digit( base[0] );
  return value;
  }

int32
DecodeCoord( cpchar base ) {
  int32 res;
  //Градусы
  res = Char2Digit( base[0] );
  res *= 10;
  res += Char2Digit( base[1] );
  if( base[5] == '.' ) {
    res *= 10;
    res += Char2Digit( base[2] );
    base += 3;
    }
  else base += 2;
  //Минуты
  res *= 6;
  res += Char2Digit( base[0] );
  res *= 10;
  res += Char2Digit( base[1] );
  if( base[2] == '.' ) {
    res *= 10;
    res += Char2Digit( base[3] );
    res *= 10;
    res += Char2Digit( base[4] );
    return res;
    }
  return -1;
  }

uint_8 
cmtGpsDecodeGprmc( uint8 *buf, CmtGpsGprmc *rmc ) {
  //Проверить строку RMC
  if( cmtStrNCmp( buf, szRMC, 6 ) != 0 ) return CMTE_FAIL;
  //RMC
  //Начать детектирование
  rmc->mReal = 0;
  cpchar ptr = NextField( (cpchar)buf );
  if( ptr == 0 ) return CMTE_FAIL;
  //Время
  //Десятки часов, Единицы часов
  rmc->mDateTime.mHour = Char2Digit( ptr[0] ) * 10 + Char2Digit( ptr[1] );
  //Десятки минут, единицы минут
  rmc->mDateTime.mMinute = Char2Digit( ptr[2] ) * 10 + Char2Digit( ptr[3] );
  //Десятки секунд, единицы секунд
  rmc->mDateTime.mSecond = Char2Digit( ptr[4] ) * 10 + Char2Digit( ptr[5] );

  ptr = NextField( ptr );
  if( ptr == 0 ) return CMTE_FAIL;
  //Активность
  rmc->mActive = ptr[0] == 'a' || ptr[0] == 'A';
  
  ptr = NextField( ptr );
  if( ptr == 0 ) return CMTE_FAIL;
  //Широта
  rmc->mLattitude = DecodeCoord( ptr );
  
  ptr = NextField( ptr );
  if( ptr[0] != 'n' && ptr[0] != 'N' ) rmc->mLattitude = -rmc->mLattitude;
  
  ptr = NextField( ptr );
  if( ptr == 0 ) return CMTE_FAIL;
  //Долгота
  rmc->mLongitude = DecodeCoord( ptr );
  ptr = NextField( ptr );
  if( ptr[0] != 'e' && ptr[0] != 'E' ) rmc->mLongitude = -rmc->mLongitude;
  
  ptr = NextField( ptr );
  if( ptr == 0 ) return CMTE_FAIL;
  //Скорость у земли
  rmc->mSpeed = DecodeValue100( ptr );
  rmc->mSpeed *= 185;
  rmc->mSpeed /= 100; //Получена скорость в 10м/ч

  ptr = NextField( ptr );
  if( ptr == 0 ) return CMTE_FAIL;
  //Курс
  rmc->mCourse = DecodeValue100( ptr );
  
  ptr = NextField( ptr );
  if( ptr == 0 ) return CMTE_FAIL;
  //Дата
  rmc->mDateTime.mDay = DecodeTwoDigit( ptr );
  rmc->mDateTime.mMonth = DecodeTwoDigit( ptr + 2 );
  rmc->mDateTime.mYear = DecodeTwoDigit( ptr + 4 );
  if( rmc->mDateTime.mYear < 100 )
    rmc->mDateTime.mYear += 2000;
  //Установить флаг достоверности данных
  rmc->mReal = 1;
  return CMTE_OK;
  }

uint_8 
cmtGpsDecodeGpgsv( uint8 *buf, CmtGpsGpgsv *gsv ) {
  int i;
  //Проверить строку GSV
  if( cmtStrNCmp( buf, szGSV, 6 ) != 0 ) return CMTE_FAIL;
  //GSV
  //Начать детектирование
  cpchar ptr = NextField( (cpchar)buf );
  if( ptr == 0 ) return CMTE_FAIL;
  //Общее количество сообщений

  ptr = NextField( ptr );
  if( ptr == 0 ) return CMTE_FAIL;
  //Номер текущего сообщения
  if( ptr[0] != '1' ) return CMTE_OK;
  
  //Разбираем только первое сообщение
  gsv->mReal = 0;

  ptr = NextField( ptr );
  if( ptr == 0 ) return CMTE_FAIL;
  //Количество видимых спутников
  gsv->mSatellitesInView = atoi(ptr); //Char2Digit( ptr[0] ) * 10 + Char2Digit( ptr[1] );
  
  memset( gsv->mInfo, 0, sizeof(CmtGpsSatellitInfo) * 4 );
  
  for( i = 0; i < 4; i++ ) {
    if( gsv->mSatellitesInView == i ) {
      gsv->mReal = 1;
      return CMTE_OK;
      }    
      
    //Разбираем каждый из четырех спутников
    ptr = NextField( ptr );
    if( ptr == 0 ) return CMTE_FAIL;
    
    //Номер спутника
    if( ptr[0] == ',' ) gsv->mInfo[i].mNumber = 0;
    else gsv->mInfo[i].mNumber = atoi(ptr); //Char2Digit( ptr[0] ) * 10 + Char2Digit( ptr[1] );

    ptr = NextField( ptr );
    if( ptr == 0 ) return CMTE_FAIL;
    
    //Elevation
    if( ptr[0] == ',' ) gsv->mInfo[i].mElevation = 0;
    else gsv->mInfo[i].mElevation = atoi(ptr);
    
    ptr = NextField( ptr );
    if( ptr == 0 ) return CMTE_FAIL;

    //Azimuth
    if( ptr[0] == ',' ) gsv->mInfo[i].mAzimuth = 0;
    else gsv->mInfo[i].mAzimuth = atoi(ptr);
    
    ptr = NextField( ptr );
    if( ptr == 0 ) return CMTE_FAIL;

    //SNR
    if( ptr[0] == ',' ) gsv->mInfo[i].mSNR = 0;
    else gsv->mInfo[i].mSNR = atoi(ptr);
    
    }

  //Установить флаг достоверности данных
  gsv->mReal = 1;
  return CMTE_OK;
  }
