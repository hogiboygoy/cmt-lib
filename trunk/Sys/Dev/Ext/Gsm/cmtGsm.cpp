//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//=================================
//    CmtGsmSensor
//����������� ������ �� UART �� �����������
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
//������ ��������� �������� ����������
void
cmtGsmTask() {
  CmtGsm *gsm = (CmtGsm*)cmtTaskParam();
  gsm->Task();
  }

//=================================
//    CmtGsmMatch
//��������� � ��������
bool
CmtGsmMatch::compare( cpchar src ) {
  if( mLen == 0 ) return false;
  return strncmp( src, mStr, mLen ) == 0;
  }


//=================================
//    CmtGsm
//�������� ���������� ������� GSM
CmtGsm::CmtGsm( CmtSerial *serial, CmtGsmPow power, CmtGsmPow powkey ) :
  CmtDev(),
  mEvent(0),
  mState(CMT_GSM_OFF),
  mSerial(serial),
  fnPowKey(powkey),
  fnPower(power),
  mQueue(40) {
    //�������� ������ ���������� �������� (���������������, ��� � ������� ������������ ������ GSM)
    CMT_TASK_ADD( cmtGsmTask, this, 200 );
    //��������� �������
    fnPower( 0 );
    //���������� ������� �� ����� ���������� ��������
    fnPowKey( 1 );
    }
  
//������ ������������� �������
void
CmtGsm::PowerKeyImpuls( uint32 time ) {
  fnPowKey( 0 );
  cmtWaitTick( time );
  fnPowKey( 1 );
  }

//��������� ���� ������ ������
void
CmtGsm::Send( cpchar src ) {
  //������ �������� �����
  mSerial->PutBlock( src, strlen(src) );
  //������� ���������� ��������
  mSerial->WaitTransmitBlock( 10000 );
  }

//��������� �������� ������ �� ������������ ��������� ��������
void
CmtGsm::MatchReceived() {
  int i;
  for( i = 0; i < CMT_MATCH_COUNT; i++ )
    //��� ������� ������� ��������� �� ������������
    if( mMatch[i].compare( mReceiv ) ) {
      //� ������ ������������ - ���������� �������
      mEvent |= CMT_GSM_MATCH0 << i;
      break;
      }
  
  if( i == CMT_MATCH_COUNT && mDest ) {
    //��� ����������, ���������� � �����
    strcpy( mDest, mReceiv );
    mDest = 0; //����������� ����������� ������
    mEvent |= CMT_GSM_RECEIV;
    }
  }

//�������� �������� ������ ������ � �����
void
CmtGsm::SetupWaitString( pchar dst ) {
  mDest = dst;
  mEvent &= ~CMT_GSM_RECEIV;
  }

//������ ������������ ������ � ������� �� ����������������� ������
/*! ������������ ����� �������� �� ����������������� ������. ��������
  �������� ������ � ����� ������. ��������� ����������� ������ ���������� ������
  � ���� >. ��� ����������� ������� ����� ������ ��������� ������, � ������� ��
  � �������������� ���������. ��� ������ �������� ����� �������������� ����� �����
  ������ (����� ��� �� ��������� ������ ������) ��� ���������� ������� ������,
  ������ ����������� � ������������ � ���������. ���� ������ ��������� ������������
  ����� ����� ������ ������� ����� ����� ��������� �� �� ����� ������������ �����. */
void
CmtGsm::Task() {
  while(1) {
    //������� ���������� �������
    if( mQueue.WaitGE(1,100) == 0 ) {
      //������ ������, ���������� � ������ ������
      uint8 ch = mQueue.WaitAndGet();
      if( ch == '\n' ) {
        //������� ������
        mReceiv[mRcIndex] = 0;
        //��������� �������� ������
        MatchReceived();
        mRcIndex = 0; //��������� ������
        }
      else {
        //������� ������
        if( ch == '>' ) mEvent |= CMT_GSM_LESS;
        mReceiv[mRcIndex++] = ch;
        if( mRcIndex >= CMT_GSM_RMAX - 1 ) {
          //������������ ������
          //������� ������
          mReceiv[mRcIndex] = 0;
          //��������� �������� ������
          MatchReceived();
          mRcIndex = 0; //��������� ������
          }
        }
      }
    }
  }

//��������� ������ ��� ���������
void
CmtGsm::Match( uint_8 index, cpchar str ) {
  MatchStr( index, str, strlen(str) );
  }

//��������� ������ ��� ���������
void
CmtGsm::MatchStr( uint_8 index, cpchar str, uint_8 len ) {
  mEvent &= ~(CMT_GSM_MATCH0 << index);
  mMatch[index].mStr = str;
  mMatch[index].mLen = len;
  }

//��������� ������ ��� ��������� � ������ 0 � �����
//������� ������ ������� ������� � ������� ��������� �������.
bool
CmtGsm::Match0( cpchar str, uint32 timeOut ) {
  //������� �������� ������
  Match( 0, str );
  return cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0, timeOut ) != 0;
  }

//���������� ������ � ������ � ������� ��� ������������ ������
//� ������� ��������� �������
bool
CmtGsm::SendAndEcho( cpchar str, uint32 timeOut ) {
  //��������� ������
  Send( str );
  //������� ���
  return Match0( str, timeOut );
  }

//���������� ������� � ������� ��
const char szOK[]       = { "OK\r" };

bool
CmtGsm::Command( cpchar str, uint32 timeOut ) {
  //��������� ������
  if( !SendAndEcho( str, timeOut ) ) return false;
  //������� ��
  return Match0( szOK, timeOut );
  }

//������� AT - �������� �����
bool
CmtGsm::At( uint32 timeOut ) {
  //������ �������
  return Command( "AT\r", timeOut );
  }

//��������� ������� PowerKey
bool
CmtGsm::PowKeyOn() {
  //��������� ������� �����
  if( At(1000) ) return true;
  //����� ���, ������ �������
  PowerKeyImpuls( 300 );
  //������� ��� ����������� �������� ������
  for( int i = 0; i < 5; i++ ) {
    if( At(1000) ) return true;
    cmtWaitTick(1000);
    }
  //����������� �� �������
  return false;
  }

//��������� ������ ���������
bool
CmtGsm::SetVolumeLevel( int level ) {
  //����������� ������ � ��������
  char str[20];
  sprintf( "AT+CLVL=%d\r", level );
  //��������� �������
  return Command( str, 8000 );
  }

//��������� ������
const char szCFUNs1[]      = { "AT+CFUN=1\r" };

bool
CmtGsm::SetNormalFun() {
  //��������� ������
  //������� ���
  if( !SendAndEcho( "AT+CFUN?\r", 8000 ) ) return false;
  //������� ����� � ������� ��������
  Match( 0, "+CFUN: 0\r" );
  Match( 1, "+CFUN: 1\r" );
  Match( 2, "+CFUN: 4\r" );
  Match( 3, szOK );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1 | CMT_GSM_MATCH2, 8000 ) ) {
    default :
      //����-���
      return false;
    case CMT_GSM_MATCH0 :
    case CMT_GSM_MATCH2 :
      //����� ���������� � ������ �����
      //������� ��
      if( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH3, 8000 ) == 0 ) return false;
      Send( szCFUNs1 );
      //������� ���
      Match( 0, szCFUNs1 );
      if( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0, 8000 ) == 0 ) return false;
      //������ ��� ���������� (������� ��)
    case CMT_GSM_MATCH1 :
      //������� ��
      if( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH3, 8000 ) == 0 ) return false;
      return true;
    }
  return false;
  }

//���������� ��������� ����� ���
bool
CmtGsm::SetTextSMS() {
  //��������� �������
  return Command( "AT+CMGF=1\r", 8000 );
  }

//�������� ������� SIM �����
bool
CmtGsm::TestSimCard() {
  //������ ������� �������� SIM �����
  if( !SendAndEcho( "AT+CSMINS?\r", 2000 ) ) return false;
  //�������� �������
  Match( 0, "+CSMINS: 0,0\r" );
  Match( 1, "+CSMINS: 0,1\r" );
  Match( 2, szOK );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1, 2000 ) ) {
    default :
      //����-���
      return false;
    case CMT_GSM_MATCH0 :
      //SIM �� ���������
      //������� ��
      cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH2, 2000 );
      return false;
    case CMT_GSM_MATCH1 :
      //SIM ���������
      //������� ��
      if( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH2, 2000 ) == 0 ) return false;
      return true;
    }
  return false;
  }

//�������� �������� ������ Ready
void
CmtGsm::PrepareWaitReady() {
  Match( 7, "Call Ready\r" );
  }

//������� ������ Ready � ������� ��������� ���������
bool
CmtGsm::WaitReady( uint32 timeOut ) {
  return cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH7, timeOut );
  }

//����������� � ����
uint_8
CmtGsm::CheckNet() {
  char str[CMT_GSM_RMAX];
  //������ ������� �������� �����������
  if( !SendAndEcho( "AT+COPS?\r", 10000 ) ) return false;
  //�����
  Match( 0, "+COPS: 0\r" );
  Match( 1, szOK );
  SetupWaitString( str );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_RECEIV, 10000 ) ) {
    case 0 :
      //����-���
      SetupWaitString(0);
      return CMTE_TIME_OUT;
    case CMT_GSM_MATCH0 :
      SetupWaitString(0);
      mOperator = GSM_NO_CONNECT; //���������� ���������� �����������
      //������� ��
      return cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH1, 2000 ) == 0 ? CMTE_TIME_OUT : GSM_NO_CONNECT;
    case CMT_GSM_RECEIV :
      SetupWaitString(0);
      //���������������� ���������
      if( cmtStrStrNoReg( str, "Beeline" ) ) mOperator = GSM_OPER_BEELINE;
      else if( StrIStr( str, "Megafon" ) ) mOperator = GSM_OPER_MEGAFON;
      else if( StrIStr( str, "Tele" ) ) mOperator = GSM_OPER_TELE2;
      else if( StrIStr( str, "MTS" ) ) mOperator = GSM_OPER_MTS;
      else mOperator = GSM_OPER_OTHER;
      //������� ��
      return cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH1, 2000 ) == 0 ? CMTE_TIME_OUT : mOperator;
    }
  return CMTE_FAIL;
  }

//������������ � ����
bool
CmtGsm::LinkNet() {
  //������ ������� �� �����������
  return Command( "AT+COPS=0\r", 30000 );
  }

//������� ������ - �������� �� ������ (��������, ��� ������)
bool
CmtGsm::TubeOn() {
  //������ ������� �� �������� ������
  return Command( "ATA\r", 5000 );
  }

//�������� ������ - ���������� ��������
bool
CmtGsm::TubeOff() {
  return Command( "ATH\r", 5000 );
  }

//������� �����
bool
CmtGsm::Call( cpchar num ) {
  char str[30];
  strcpy( str, "ATD" );
  strcat( str, num );
  strcat( str, ";\r" );
  return Command( str, 2000 );
  }

//��������� SMS
bool
CmtGsm::SendSMS( cpchar num, cpchar text ) {
  char str[30];
  strcpy( str, "AT+CMGS=\"+7" );
  if( num[0] == '+' && num[1] == '7' ) strcat( str, num + 2 );
  else if( num[0] == '8' ) strcat( str, num + 1 );
  else strcat( str, num );
  //��������� ������
  strcat( str, "\"\r" );
  mEvent &= ~CMT_GSM_LESS;
  if( !SendAndEcho( str, 4000 ) ) return false;
  //������� ������ >
  if( !cmtWaitSetBitTime( &mEvent, CMT_GSM_LESS, 4000 ) ) return false;
  Send( text );
  Send( "\032" ); //������ ���������� SMS
  if( !Match0( "+CMGS:", 10000 ) ) return false;
  return Match0( szOK, 2000 );
  }

//������ ������ (��������, ������ �������)
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

//������ ������� GPRS
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
  //��������� ������ �������
  if( !SendAndEcho( "AT+CIPSTATUS\r", 8000) ) return CMTE_TIME_OUT;
  //������� ������
  Match( 0, "STATE: IP INITIAL\r" );
  Match( 1, "STATE: IP START\r" );
  Match( 2, "STATE: IP GPRSACT\r" );
  Match( 3, "STATE: IP STATUS\r" );
  Match( 4, "STATE: IP CLOSE\r" );
  Match( 5, "STATE: CONNECT OK\r" );
  Match( 6, "STATE: " );
  switch( cmtWaitSetBitTime( &mEvent, CMT_GSM_MATCH0 | CMT_GSM_MATCH1 | CMT_GSM_MATCH2 | CMT_GSM_MATCH3 | CMT_GSM_MATCH4 | CMT_GSM_MATCH5 | CMT_GSM_MATCH6, 5000 ) ) {
    case 0 :
      //����-���
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
  //������� ��
  if( Match0(szOK,5000) ) return ret;
  return CMTE_TIME_OUT;
  }

//��������� GPRS � ��������� ���������
const char szERROR[]    = { "ERROR\r" };

bool
CmtGsm::GprsShutDown() {
  //��������� �������
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

//����������� � ���� GPRS
bool
CmtGsm::GprsCSTTQ( cpchar net ) {
  char str[40];
  //������������ ������ �� ����������� � ����
  strcpy( str, "AT+CSTT=" );
  strcat( str, net );
  strcat( str, "\r" );
  if( !SendAndEcho( str, 4000 ) ) return false;
  return Match0( szOK, 4000 );
  }

//����� GPRS
bool
CmtGsm::GprsCIICR() {
  //��������� ������� ������
  if( !SendAndEcho( "AT+CIICR\r", 4000 ) ) return false;
  //��������� ������
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

//�������� IP
bool
CmtGsm::GprsIP() {
  //��������� ������ �� ��������� IP
  if( !SendAndEcho( "AT+CIFSR\r", 4000 ) ) return false;
  //�������� IP
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

//������������ � �������
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

//������� �����������
bool
CmtGsm::GprsClose() {
  //��������� ������� �� ��������
  if( !SendAndEcho( "AT+CIPCLOSE\r", 5000 ) ) return false;
  //�������� �������
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

//����������� ����� �������
bool
CmtGsm::GprsSend( cpchar src, pchar dest, uint32 destLen ) {
  //��������� ������� �� �����
  mEvent &= ~CMT_GSM_LESS;
  if( !SendAndEcho( "AT+CIPSEND\r", 5000 ) ) return false;
  //������� ������ >
  if( !cmtWaitSetBitTime( &mEvent, CMT_GSM_LESS, 4000 ) ) return false;
  Send( src );
  Send( "\032" ); //������ ���������� SMS
  //������� ����� �� ��������
  
  }

//��������� 6 ������� �����������
bool
CmtGsm::ModuleOn() {
  if( At(100) ) return true;
  for( int i = 0; i < 6; i++ ) {
    //������� �������
    PowerKeyImpuls( 300 );
    //��������� �������� ������
    for( int s = 0; s < 5; s++ ) {
      if( At(1000) ) return true;
      //��������
      cmtWaitTick( 500 );
      }
    }
  return false;
  }

//����������� ���������� ������
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

//����������� ������ � ����
//����������� ������� ����������� � ���� � ������� 1 ����, ����� ���� ����������� �������
bool
CmtGsm::ModuleNet() {
  uint32 time;
  cmtSetCheckTick( time, 60 * 60 * 1000 );
  while( !cmtCheckTick( time ) ) {
    //��������� ������� ����
    if( CheckNet() > GSM_NO_CONNECT ) return true;
    //���� ��� ������������
    if( !LinkNet() ) return false;
    }
  return false; //����������� � ������� ��������� ������� �� �������
  }

//������ ��������������� ���������� �������
void
CmtGsm::AutoTask() {
  while(1) {
    //���� ����������� ���������� �������
    
    //������ �������
    fnPower(1);
    cmtWaitTick(1000);
    
    //���������� ����� ����-���
    cmtSetCheckTick( mTimeOut, 2 * 60 * 60 * 1000 );
    
    while( !cmtCheckTick(mTimeOut) ) {
      //���� ������������ ���������� �������
      
      //��������� ������
      if( !ModuleOn() ) break;
      
      //�������������� ������ CallReady
      PrepareWaitReady();
      
      //��������� �������������
      //���������� ������������ ������� ���������
      if( !SetVolumeLevel( 100 ) ) break;
      
      //���������� ���������� �����
      if( !SetNormalFun() ) break;
      
      //����������� SMS �� ��������� ������
      if( !SetTextSMS() ) break;
      
      //��������� ������� SIM �����
      if( !TestSimCard() ) break;

      //������� CallReady
      WaitReady( 30000 );
      
      while( At(500) ) {
        //��������� ����� ����-���
        if( cmtCheckTick(mTimeOut) ) break;
        
        //������������ � ����
        if( !ModuleNet() ) break;
      
        //������� ����
        AutoTaskLoop();
        }
      //���������� ������� �����������
      if( !ModuleOff() ) break;
      }
    
    //��������� ���������� ���������� ������
    fnPower(0);
    
    //��������
    cmtWaitTick( 10000 );
    }
  }
 
//���������� �������� ����� �����������
void
CmtGsm::AutoTaskLoop() {
  //������� ���� �����������
  while(1) {
    //���������� �������� �������
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
//��������� ����� ������� �� GPRS
bool
CmtGsm::GprsLoop( cpchar net, cpchar ip ) {
  while( IsThereGprs() ) {
    //��������� ��������� �����������
    switch( GprsStatus() ) {
      case CMTE_TIME_OUT : return false;
      case CMT_GSM_IP_INITIAL :
        //��������� ���������, ��������� CTTQ
        if( !GprsCTTQ( net ) ) return false;
        break;
      case CMT_GSM_IP_START :
        //��������� CIICR
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
        //���������� �������� � ����� ������
        break;
      }
    }
  }
