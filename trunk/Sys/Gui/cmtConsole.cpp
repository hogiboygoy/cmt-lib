//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

CmtConsole::CmtConsole(CmtContext *screen, CmtPoint sizeInSymbols) :
  mScreen(screen),
  mConsole(0),
  mSize(sizeInSymbols) {
    mConsole = new char[mSize.x * mSize.y];
    
    if( mConsole ) {
      //�������� ����� ������
      for( int i = 0; i < mSize.y; i++ ) {
        mConsole[ i * mSize.x ] = ' ';
        mConsole[ i * mSize.x + 1] = 0;
        }    
      }
    }

void
CmtConsole::Scroll() {
  if( mConsole ) {
    //���������� ��� ������
    for( int i = 0; i < (mSize.y - 1); i++ ) {
      //�������� ������
      memcpy( mConsole + i * mSize.x, mConsole + (i + 1) * mSize.x, mSize.x );
      }
    mConsole[mSize.x * (mSize.y - 1)] = 0;
    }
  }

void
CmtConsole::PrintS_W(cpchar str) {
  if( mConsole ) {
    //���������� ������
    Scroll();
    //�������� ������
    memcpy( mConsole + (mSize.y - 1) * mSize.x, str, mSize.x );
    mConsole[mSize.x * mSize.y - 1] = 0;
    }
  }

void
CmtConsole::PrintS(cpchar str) {
  while( str ) {
    //���������� ����� ���������� ������
    cpchar ptr = strchr( str, '\n' );
    int len;
    if( ptr ) len = (ptr - str) - 1;
    else len = strlen( str );
    int cplen = len >= mSize.x ? mSize.x - 1 : len;
    //���������� ������
    Scroll();
    //�������� ������
    memcpy( mConsole + (mSize.y - 1) * mSize.x, str, cplen );
    mConsole[(mSize.y - 1) * mSize.x + cplen] = 0;
    //��������� � ��������� ������
    if( cplen == len ) {
      if( ptr ) str = ptr + 1;
      else str = 0;
      }
    else {
      str += cplen;
      }
    }
  }

void
CmtConsole::Update() {
  mScreen->Clear( MC_WHITE32, true );
  mScreen->SetFont( 10 );
  //���������� �����
  for( int y = 0; y < mSize.y; y++ )
    mScreen->Text( MC_BLACK32, CmtPoint( 10, y * 14 ), mConsole + y * mSize.x );
  }

void 
CmtConsole::PrintAndUpdate( cpchar str ) {
  PrintS_W( str );
  Update();
  }
