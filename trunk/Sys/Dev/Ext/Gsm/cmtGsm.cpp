//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//=================================
//    CmtGsmSensor
//Обеспечение приема по UART по прерываниям
struct CmtGsmSensor : public CmtSerialSensor {
  CmtGsm *mGsm;
  
  
  CmtGsmSensor( CmtGsm *gsm ) : CmtSerialSensor(), mGsm(gsm) {}
  
  virtual void Complete( CmtSerial *serial );
  };

void
CmtGsmSensor::Complete(CmtSerial *serial) {
  mGsm->mQueue.Put( serial->GetByte() );
  }

 
//=================================
//    cmtGsmTask
//Задача обработки принятой информации
void
cmtGsmTask() {
  CmtGsm *gsm = (CmtGsm*)cmtTaskParam();
  gsm->Task();
  }

//=================================
//    CmtGsmMatch
//Сравнение с образцом
bool
CmtGsmMatch::compare( cpchar src ) {
  if( mLen == 0 ) return false;
  return strncmp( src, mStr, mLen ) == 0;
  }


//=================================
//    CmtGsm
//Механизм управления модулем GSM
CmtGsm::CmtGsm( CmtSerial *serial, CmtGsmPow power, CmtGsmPow powkey ) :
  CmtDev(),
  mEvent(0),
  mState(CMT_GSM_OFF),
  mSerial(serial),
  fnPowKey(powkey),
  fnPower(power),
  mQueue(40) {
    //Добавить задачу обработчик запросов (подразумевается, что в системе единственный модуль GSM)
    CMT_TASK_ADD( cmtGsmTask, this, 200 );
    //Выключить питание
    fnPower( 0 );
    //Установить единицу на входе управления питанием
    fnPowKey( 1 );
    }
  
//Подать отрицательный импульс
void
CmtGsm::PowerKeyImpuls( uint32 time ) {
  fnPowKey( 0 );
  cmtWaitTick( time );
  fnPowKey( 1 );
  }

//Отправить блок данных модулю
void
CmtGsm::Send( cpchar src ) {
  //Начать передачу блока
  mSerial->PutBlock( src, strlen(src) );
  //Ожидать завершения передачи
  mSerial->WaitTransmitBlock( 10000 );
  }

//Проверяет принятую строку на соответствие имеющимся шаблонам
void
CmtGsm::MatchReceived() {
  int i;
  for( i = 0; i < CMT_MATCH_COUNT; i++ )
    //Для каждого шаблона проверить на соответствие
    if( mMatch[i].compare( mReceiv ) ) {
      //В случае соответствия - образовать событие
      mEvent |= CMT_GSM_MATCH0 << i;
      break;
      }
  
  if( i == CMT_MATCH_COUNT && mDest ) {
    //Нет совпадений, копировать в буфер
    strcpy( mDest, mReceiv );
    mDest = 0; //Блокировать последующую запись
    mEvent |= CMT_GSM_RECEIV;
    }
  }

//Включаем ожидание приема строки в буфер
void
CmtGsm::SetupWaitString( pchar dst ) {
  mDest = dst;
  mEvent &= ~CMT_GSM_RECEIV;
  }

//Задача обслуживания обмена с модулем по последовательному каналу
/*! Осуществляет прием символов по последовательному каналу. Помещает
  принятый символ в буфер приема. Проверяет специальный символ завершения строки
  и знак >. При обнаружении символа конца строки закрывает строку, и сверяет ее
  с установленными шаблонами. При приеме символов также контролируется общая длина
  строки (чтобы она не превышала размер буфера) при превышении размера буфера,
  строка закрывается и сравнивается с шаблонами. Этот подход позволяет осуществлять
  прием сколь угодно длинных строк путем разбиения их на блоки ограниченной длины. */
void
CmtGsm::Task() {
  while(1) {
    //Ожидать готовности символа
    if( mQueue.WaitGE(1,100) == 0 ) {
      //Принят символ, разместить в буфере команд
      uint8 ch = mQueue.WaitAndGet();
      if( ch == '\n' ) {
        //Закрыть строку
        mReceiv[mRcIndex] = 0;
        //Обработка принятой строки
        MatchReceived();
        mRcIndex = 0; //Следующая строка
        }
      else {
        //Обычный символ
        if( ch == '>' ) mEvent |= CMT_GSM_LESS;
        mReceiv[mRcIndex++] = ch;
        if( mRcIndex >= CMT_GSM_RMAX - 1 ) {
          //Переполнение строки
          //Закрыть строку
          mReceiv[mRcIndex] = 0;
          //Обработка принятой строки
          MatchReceived();
          mRcIndex = 0; //Следующая строка
          }
        }
      }
    }
  }

//Фиксирует шаблон для сравнения
void
CmtGsm::Match( uint_8 index, cpchar str ) {
  MatchStr( index, str, strlen(str) );
  }

//Фиксирует шаблон для сравнения
void
CmtGsm::MatchStr( uint_8 index, cpchar str, uint_8 len ) {
  mEvent &= ~(CMT_GSM_MATCH0 << index);
  mMatch[index].mStr = str;
  mMatch[index].mLen = len;
  }

//Фиксирует шаблон для сравнения в ячейку 0 и затем
//ожидает приема данного шаблона в течение заданного времени.
bool
CmtGsm::Match0( cpchar str, uint32 timeOut ) {
  //Ожидаем заданную строку
  Match( 0, str );
  return cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0, timeOut ) != 0;
  }

//Отправляет строку в модуль и ожидает эхо отправленной строки
//в течение заданного времени
bool
CmtGsm::SendAndEcho( cpchar str, uint32 timeOut ) {
  //Отправить запрос
  Send( str );
  //Ожидаем эхо
  return Match0( str, timeOut );
  }

//Отправляет команду и ожидает ОК
const char szOK[]       = { "OK\r" };

bool
CmtGsm::Command( cpchar str, uint32 timeOut ) {
  //Отправить запрос
  if( !SendAndEcho( str, timeOut ) ) return false;
  //Ожидаем ОК
  return Match0( szOK, timeOut );
  }

//Команда AT - проверка связи
bool
CmtGsm::At( uint32 timeOut ) {
  //Подать команду
  return Command( "AT\r", timeOut );
  }

//Включение кнопкой PowerKey
bool
CmtGsm::PowKeyOn() {
  //Проверить наличие связи
  if( At(1000) ) return true;
  //Связи нет, подать импульс
  PowerKeyImpuls( 300 );
  //Команды для определения скорости обмена
  for( int i = 0; i < 5; i++ ) {
    if( At(1000) ) return true;
    cmtWaitTick(1000);
    }
  //Подключение не удалось
  return false;
  }

//Установка уровня громкости
bool
CmtGsm::SetVolumeLevel( int level ) {
  //Подготовить строку с запросом
  char str[20];
  sprintf( "AT+CLVL=%d\r", level );
  //Отправить команду
  return Command( str, 8000 );
  }

//Установка режима
const char szCFUNs1[]      = { "AT+CFUN=1\r" };

bool
CmtGsm::SetNormalFun() {
  //Отправить запрос
  //Ожидаем эхо
  if( !SendAndEcho( "AT+CFUN?\r", 8000 ) ) return false;
  //Ожидаем ответ с текущей функцией
  Match( 0, "+CFUN: 0\r" );
  Match( 1, "+CFUN: 1\r" );
  Match( 2, "+CFUN: 4\r" );
  Match( 3, szOK );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1 | CMT_GSM_MATCH2, 8000 ) ) {
    default :
      //Тайм-аут
      return false;
    case CMT_GSM_MATCH0 :
    case CMT_GSM_MATCH2 :
      //Нужно переводить в другой режим
      //Ожидаем ОК
      if( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH3, 8000 ) == 0 ) return false;
      Send( szCFUNs1 );
      //Ожидаем эхо
      Match( 0, szCFUNs1 );
      if( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0, 8000 ) == 0 ) return false;
      //Ничего нет специально (ожидаем ОК)
    case CMT_GSM_MATCH1 :
      //Ожидаем ОК
      if( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH3, 8000 ) == 0 ) return false;
      return true;
    }
  return false;
  }

//Установить текстовый режим СМС
bool
CmtGsm::SetTextSMS() {
  //Отправить команду
  return Command( "AT+CMGF=1\r", 8000 );
  }

//Проверка наличия SIM карты
bool
CmtGsm::TestSimCard() {
  //Подать команду проверки SIM карты
  if( !SendAndEcho( "AT+CSMINS?\r", 2000 ) ) return false;
  //Варианты ответов
  Match( 0, "+CSMINS: 0,0\r" );
  Match( 1, "+CSMINS: 0,1\r" );
  Match( 2, szOK );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1, 2000 ) ) {
    default :
      //Тайм-аут
      return false;
    case CMT_GSM_MATCH0 :
      //SIM не вставлена
      //Ожидаем ОК
      cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH2, 2000 );
      return false;
    case CMT_GSM_MATCH1 :
      //SIM вставлена
      //Ожидаем ОК
      if( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH2, 2000 ) == 0 ) return false;
      return true;
    }
  return false;
  }

//Механизм ожидания строки Ready
void
CmtGsm::PrepareWaitReady() {
  Match( 7, "Call Ready\r" );
  }

//Ожидает строку Ready в течение заданного интервала
bool
CmtGsm::WaitReady( uint32 timeOut ) {
  return cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH7, timeOut );
  }

//Подключение к сети
uint_8
CmtGsm::CheckNet() {
  char str[CMT_GSM_RMAX];
  //Подать команду проверки подключения
  if( !SendAndEcho( "AT+COPS?\r", 10000 ) ) return false;
  //Ответ
  Match( 0, "+COPS: 0\r" );
  Match( 1, szOK );
  SetupWaitString( str );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_RECEIV, 10000 ) ) {
    case 0 :
      //Тайм-аут
      SetupWaitString(0);
      return CMTE_TIME_OUT;
    case CMT_GSM_MATCH0 :
      SetupWaitString(0);
      mOperator = GSM_NO_CONNECT; //Обозначить отсутствие подключения
      //Ожидать ОК
      return cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH1, 2000 ) == 0 ? CMTE_TIME_OUT : GSM_NO_CONNECT;
    case CMT_GSM_RECEIV :
      SetupWaitString(0);
      //Проанализировать оператора
      if( cmtStrStrNoReg( str, "Beeline" ) ) mOperator = GSM_OPER_BEELINE;
      else if( StrIStr( str, "Megafon" ) ) mOperator = GSM_OPER_MEGAFON;
      else if( StrIStr( str, "Tele" ) ) mOperator = GSM_OPER_TELE2;
      else if( StrIStr( str, "MTS" ) ) mOperator = GSM_OPER_MTS;
      else mOperator = GSM_OPER_OTHER;
      //Ожидать ОК
      return cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH1, 2000 ) == 0 ? CMTE_TIME_OUT : mOperator;
    }
  return CMTE_FAIL;
  }

//Подключиться к сети
bool
CmtGsm::LinkNet() {
  //Подать команду на подключение
  return Command( "AT+COPS=0\r", 30000 );
  }

//Поднять трубку - ответить на звонок (например, при вызове)
bool
CmtGsm::TubeOn() {
  //Подать команду на поднятие трубки
  return Command( "ATA\r", 5000 );
  }

//Положить трубку - прекратить разговор
bool
CmtGsm::TubeOff() {
  return Command( "ATH\r", 5000 );
  }

//Набрать номер
bool
CmtGsm::Call( cpchar num ) {
  char str[30];
  strcpy( str, "ATD" );
  strcat( str, num );
  strcat( str, ";\r" );
  return Command( str, 2000 );
  }

//Отправить SMS
bool
CmtGsm::SendSMS( cpchar num, cpchar text ) {
  char str[30];
  strcpy( str, "AT+CMGS=\"+7" );
  if( num[0] == '+' && num[1] == '7' ) strcat( str, num + 2 );
  else if( num[0] == '8' ) strcat( str, num + 1 );
  else strcat( str, num );
  //Завершить строку
  strcat( str, "\"\r" );
  mEvent &= ~CMT_GSM_LESS;
  if( !SendAndEcho( str, 4000 ) ) return false;
  //Ожидаем символ >
  if( !cmtWaitSetBitTime( &mEvent, CMT_GSM_LESS, 4000 ) ) return false;
  Send( text );
  Send( "\032" ); //Символ завершения SMS
  if( !Match0( "+CMGS:", 10000 ) ) return false;
  return Match0( szOK, 2000 );
  }

//Запрос службы (например, запрос баланса)
bool
CmtGsm::QueryService( cpchar num, pchar dest, uint32 timeOut ) {
  if( !Call( num ) ) return false;
  Match( 0, szOK );
  SetupWaitString( dest );
  while(1) {
    switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_RECEIV, timeOut ) ) {
      case 0 :
        SetupWaitString(0);
        return false;
      case CMT_GSM_MATCH0 :
        SetupWaitString(0);
        return true;
      case CMT_GSM_RECEIV :
        dest += strlen(dest);
        SetupWaitString( dest );
        break;
      }
    }
  }

//Запрос статуса GPRS
#define CMT_GSM_IP_INITIAL CMTE_USER + 1
#define CMT_GSM_IP_START   CMTE_USER + 2
#define CMT_GSM_IP_GPRSACT CMTE_USER + 3
#define CMT_GSM_IP_STATUS  CMTE_USER + 4
#define CMT_GSM_IP_CLOSE   CMTE_USER + 5
#define CMT_GSM_IP_CONNECT CMTE_USER + 6
#define CMT_GSM_IP_OTHER   CMTE_USER + 7

uint_8
CmtGsm::GprsStatus() {
  uint_8 ret;
  //Отправить запрос статуса
  if( !SendAndEcho( "AT+CIPSTATUS\r", 8000) ) return CMTE_TIME_OUT;
  //Ожидать статус
  Match( 0, "STATE: IP INITIAL\r" );
  Match( 1, "STATE: IP START\r" );
  Match( 2, "STATE: IP GPRSACT\r" );
  Match( 3, "STATE: IP STATUS\r" );
  Match( 4, "STATE: IP CLOSE\r" );
  Match( 5, "STATE: CONNECT OK\r" );
  Match( 6, "STATE: " );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1 | CMT_GSM_MATCH2 | CMT_GSM_MATCH3 | CMT_GSM_MATCH4 | CMT_GSM_MATCH5 | CMT_GSM_MATCH6, 5000 ) ) {
    case 0 :
      //Тайм-аут
      return CMTE__TIME_OUT;
    case CMT_GSM_MATCH0 :
      ret = CMT_GSM_IP_INITIAL;
      break;
    case CMT_GSM_MATCH1 :
      ret = CMT_GSM_IP_START;
      break;
    case CMT_GSM_MATCH2 :
      ret = CMT_GSM_IP_GPRSACT;
      break;
    case CMT_GSM_MATCH3 :
      ret = CMT_GSM_IP_STATUS;
      break;
    case CMT_GSM_MATCH4 :
      ret = CMT_GSM_IP_CLOSE;
      break;
    case CMT_GSM_MATCH5 :
      ret = CMT_GSM_IP_CONNECT;
      break;
    case CMT_GSM_MATCH6 :
      ret = CMT_GSM_IP_OTHER;
      break;
    }
  //Ожидаем ОК
  if( Match0(szOK,5000) ) return ret;
  return CMTE_TIME_OUT;
  }

//Перевести GPRS в начальное состояние
const char szERROR[]    = { "ERROR\r" };

bool
CmtGsm::GprsShutDown() {
  //Отправить команду
  if( !SendAndEcho( "AT+CIPSHUT\r", 5000 ) ) return false;
  Match( 0, "SHUT OK\r" );
  Match( 1, szERROR );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1, 5000 ) ) {
    case 0 :
    case CMT_GSM_MATCH1 :
      return false;
    case CMT_GSM_MATCH0 :
      return true;
    }
  return false;
  }

//Подключение к сети GPRS
bool
CmtGsm::GprsCSTTQ( cpchar net ) {
  char str[40];
  //Сформировать запрос на подключение к сети
  strcpy( str, "AT+CSTT=" );
  strcat( str, net );
  strcat( str, "\r" );
  if( !SendAndEcho( str, 4000 ) ) return false;
  return Match0( szOK, 4000 );
  }

//Вынос GPRS
bool
CmtGsm::GprsCIICR() {
  //Отправить команду выноса
  if( !SendAndEcho( "AT+CIICR\r", 4000 ) ) return false;
  //Ожидаемые ответы
  Match( 0, szOK );
  Match( 1, szERROR );
  Match( 2, "+PDP: DEACT\r" );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1 | CMT_GSM_MATCH2, 60000 ) ) {
    case 0 :
      return false;
    case CMT_GSM_MATCH0 :
      return true;
    case CMT_GSM_MATCH1 :
      return false;
    case CMT_GSM_MATCH2 :
      Match0( szOK, 4000 );
      return false;
    }
  return false;
  }

//Получить IP
bool
CmtGsm::GprsIP() {
  //Отправить запрос на получение IP
  if( !SendAndEcho( "AT+CIFSR\r", 4000 ) ) return false;
  //Получить IP
  Match( 0, szOK );
  Match( 1, szERROR );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1, 10000 ) ) {
    case 0 :
      return false;
    case CMT_GSM_MATCH0 :
      return true;
    case CMT_GSM_MATCH1 :
      return false;
    }
  return false;
  }

//Подключиться к серверу
bool
CmtGsm::GprsConnect( cpchar ip ) {
  char str[40];
  strcpy( str, "AT+CIPSTART=\"TCP\",\"" );
  strcat( str, ip );
  strcat( str, "\",\"80\"\r" );
  if( !SendAndEcho( str, 20000 ) ) return false;
  Match( 0, szOK );
  Match( 1, szERROR );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1, 10000 ) ) {
    case 0 :
      return false;
    case CMT_GSM_MATCH0 :
      break;
    case CMT_GSM_MATCH1 :
      return false;
    }
  Match( 0, "CONNECT OK\r" );
  Match( 1, "CONNECT FAIL\r" );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1, 10000 ) ) {
    case 0 :
      return false;
    case CMT_GSM_MATCH0 :
      return true;
    case CMT_GSM_MATCH1 :
      return false;
    }
  return false;
  }

//Закрыть подключение
bool
CmtGsm::GprsClose() {
  //Отправить команду на закрытие
  if( !SendAndEcho( "AT+CIPCLOSE\r", 5000 ) ) return false;
  //Варианты ответов
  Match( 0, "CLOSE OK\r" );
  Match( 1, szERROR );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1, 10000 ) ) {
    case 0 :
      return false;
    case CMT_GSM_MATCH0 :
      return true;
    case CMT_GSM_MATCH1 :
      return false;
    }
  return false;
  }

//Осуществить обмен данными
bool
CmtGsm::GprsSend( cpchar src, pchar dest, uint32 destLen ) {
  //Отправить команду на обмен
  mEvent &= ~CMT_GSM_LESS;
  if( !SendAndEcho( "AT+CIPSEND\r", 5000 ) ) return false;
  //Ожидаем символ >
  if( !cmtWaitSetBitTime( &mEvent, CMT_GSM_LESS, 4000 ) ) return false;
  Send( src );
  Send( "\032" ); //Символ завершения SMS
  //Ожидаем ответ об отправке
  
  }

//Выполняет 6 попыток подключения
bool
CmtGsm::ModuleOn() {
  if( At(100) ) return true;
  for( int i = 0; i < 6; i++ ) {
    //Импульс питания
    PowerKeyImpuls( 300 );
    //Настройка скорости обмена
    for( int s = 0; s < 5; s++ ) {
      if( At(1000) ) return true;
      //Задержка
      cmtWaitTick( 500 );
      }
    }
  return false;
  }

//Программное отключение модуля
bool
CmtGsm::ModuleOff() {
  int c = 0;
  do {
    PowerKeyImpuls( 300 );
    if( c > 5 ) return false;
    }
  while( At(500) );
  return true;
  }

//Подключение модуля к сети
//Выполняются попытки подключения к сети в течение 1 часа, после чего фиксируется неуспех
bool
CmtGsm::ModuleNet() {
  uint32 time;
  cmtSetCheckTick( time, 60 * 60 * 1000 );
  while( !cmtCheckTick( time ) ) {
    //Проверить наличие сети
    if( CheckNet() > GSM_NO_CONNECT ) return true;
    //Сети нет подключиться
    if( !LinkNet() ) return false;
    }
  return false; //Подключение в течение заданного времени не удалось
  }

//Задача автоматического управления модулем
void
CmtGsm::AutoTask() {
  while(1) {
    //Цикл физического отключения питания
    
    //Подача питания
    fnPower(1);
    cmtWaitTick(1000);
    
    //Установить общий тайм-аут
    cmtSetCheckTick( mTimeOut, 2 * 60 * 60 * 1000 );
    
    while( !cmtCheckTick(mTimeOut) ) {
      //Цикл программного отключения питания
      
      //Включение модуля
      if( !ModuleOn() ) break;
      
      //Подготавливаем чтение CallReady
      PrepareWaitReady();
      
      //Некоторая инициализация
      //Установить максимальный уровень громкости
      if( !SetVolumeLevel( 100 ) ) break;
      
      //Обеспечить нормальный режим
      if( !SetNormalFun() ) break;
      
      //Переключить SMS на текстовый формат
      if( !SetTextSMS() ) break;
      
      //Проверить наличие SIM карты
      if( !TestSimCard() ) break;

      //Ожидать CallReady
      WaitReady( 30000 );
      
      while( At(500) ) {
        //Проверить общий тайм-аут
        if( cmtCheckTick(mTimeOut) ) break;
        
        //Подключиться к сети
        if( !ModuleNet() ) break;
      
        //Главный цикл
        AutoTaskLoop();
        }
      //Отключение питания программное
      if( !ModuleOff() ) break;
      }
    
    //Выполнить физическое отключение модуля
    fnPower(0);
    
    //Ожидание
    cmtWaitTick( 10000 );
    }
  }
 
//Реализация главного цикла подключения
void
CmtGsm::AutoTaskLoop() {
  //Главный цикл подключения
  while(1) {
    //Установить контроль событий
    Match( 0, "RING" );
    Match( 1, "+CMTI: " );
    Match( 2, "+CMGL: " );
    while(1) {
      if( cmtCheckTick(mTimeOut) ) return;
      uint32 bits = cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1 | CMT_GSM_MATCH2 | CMT_GSM_EVENT, 10000 );
      if( bits == 0 ) continue;
      if( bits & CMT_GSM_MATCH0 ) {
        if( !OnRing() ) return;
        }
      else if( bits & (CMT_GSM_MATCH1 | CMT_GSM_MATCH2) ) {
        if( !OnSms() ) return;
        }
      else {
        if( !OnEvent() ) return;
        }
      break;
      }
    }
  }

//  Match( 1, "BUSY" );
//  Match( 2, "NO CARRIER" );
//Выполнить обмен данными по GPRS
bool
CmtGsm::GprsLoop( cpchar net, cpchar ip ) {
  while( IsThereGprs() ) {
    //Проверить состояние подключения
    switch( GprsStatus() ) {
      case CMTE_TIME_OUT : return false;
      case CMT_GSM_IP_INITIAL :
        //Начальное состояние, выполнить CTTQ
        if( !GprsCTTQ( net ) ) return false;
        break;
      case CMT_GSM_IP_START :
        //Выполнить CIICR
        if( !GprsCIICR() ) return false;
        break;
      case CMT_GSM_IP_GPRSACT :
        if( !GprsIP() ) return false;
        break;
      case CMT_GSM_IP_STATUS :
      case CMT_GSM_IP_CLOSE :
        if( !GprsConnect( ip ) ) return false;
        break;
      case CMT_GSM_IP_CONNECT :
        //Собственно отправка и прием данных
        break;
      }
    }
  }
