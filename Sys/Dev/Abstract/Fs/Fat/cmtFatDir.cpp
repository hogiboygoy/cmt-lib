//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#pragma pack(1)
struct FatDirEntry1x {
  uint8  mFileName[8];
  uint8  mExtension[3];
  uint8  mAttrib;
  uint8  mReserved[2];
  uint16 mCreationTime;
  uint16 mCreationDate;
  uint16 mAccessDate;
  uint16 mFirstClusterHigh; // higher
  uint16 mUpdateTime;       //time create/update
  uint16 mUpdateDate;       //date create/update
  uint16 mFirstCluster;
  uint32 mFileSize;

  uint_8 MatchName( cpchar longName, cpchar tst, cpchar tail );      //�������� �������� ������� ����� �� ������ ����������
  void   FillAttr( cpchar longName, CMT_FILE_ATTR *lpFileAttr );     //��������� �������� �����
  uint_8 PatternName( cpchar longName, cpchar tst, uint8 findFlag ); //�������� � ��������
  uint32 GetFirstCluster32();
  void   SetFirstCluster32( uint32 cluster );
  uint_8 CheckLongName( pchar dest );
  };
    
#pragma pack()

char
Rus( uint8 ch, uint8 cd ) {
  if( cd == 0x04 ) return ch + 0xb0;
  return ch;
  }

char
Rus16( uint16 ch ) {
  if( (ch & 0xff00) == 0x0400 ) return (char)((ch & 0xff) + 0xb0);
  return (char)(ch & 0xff);
  }

uint_8
FatDirEntry1x::CheckLongName( pchar dest ) {
  if( mFileName[0] == 0xe5 ) {
    dest[0] = 0;
    return 0;
    }
  if( mAttrib == 0xf ) {
    //��� ����� �������� �����
    int offset = mFileName[0] & 0x1f;
    if( offset ) offset--;
    //� ���� ���� ����� ���� ������� 13 ������
    offset *= 13;
    //���������
    dest[offset] = Rus( mFileName[1], mFileName[2] );
    dest[offset + 1] = Rus( mFileName[3], mFileName[4] );
    dest[offset + 2] = Rus( mFileName[5], mFileName[6] );
    dest[offset + 3] = Rus( mFileName[7], mExtension[0] );
    dest[offset + 4] = Rus( mExtension[1], mExtension[2] );
    dest[offset + 5] = Rus16( mCreationTime );
    dest[offset + 6] = Rus16( mCreationDate );
    dest[offset + 7] = Rus16( mAccessDate );
    dest[offset + 8] = Rus16( mFirstClusterHigh );
    dest[offset + 9] = Rus16( mUpdateTime );
    dest[offset + 10] = Rus16( mUpdateDate );
    dest[offset + 11] = Rus16( mFileSize & 0xffff );
    dest[offset + 12] = Rus16( mFileSize >> 16 );
    //����������� ������
    if( (mFileName[0] & 0x40) == 0x40 )
      dest[offset + 13] = 0;
    return 1;
    }
  return 0;
  }

void
FatDirEntry1x::FillAttr( cpchar longName, CMT_FILE_ATTR *lpFileAttr ) {
  int i = 0,c;
  //���������� ���
  for( c = 0; c < 8; c++ ) {
    if( mFileName[c] == ' ' ) break;
    lpFileAttr->cAlternateFileName[i++] = mFileName[c];
    }
  //������ �����
  lpFileAttr->cAlternateFileName[i++] = '.';
  //�������� ����������
  for( c = 0; c < 3; c++ ) {
    if( mExtension[c] == ' ' ) break;
    lpFileAttr->cAlternateFileName[i++] = mExtension[c];
    }
  //������� ���
  lpFileAttr->cAlternateFileName[i] = 0;
  //������ ��������
  lpFileAttr->mCreationTime  = mCreationTime;
  lpFileAttr->mCreationDate  = mCreationDate;
  lpFileAttr->mUpdateTime    = mUpdateTime;
  lpFileAttr->mUpdateDate    = mUpdateDate;
  lpFileAttr->nFileSizeLow   = mFileSize;
  lpFileAttr->nFileSizeHigh  = 0;
  lpFileAttr->fileAttributes = mAttrib;
  if( longName[0] ) {
    //������� ��� ������������, �����������
    strcpy( (pchar)(lpFileAttr->cFileName), longName );
    }
  else {
    //����������� � ��� �������������� ���
    for( c = 0; c < 14; c++ )
      lpFileAttr->cFileName[c] = lpFileAttr->cAlternateFileName[c];
    }
  }

uint_8
FatDirEntry1x::MatchName( cpchar longName, cpchar tst, cpchar tail ) {
  int c;
  if( longName[0] ) {
    //���������� � ������� ������
    for( c = 0; c < 255 && longName[c]; c++ )
      if( cmtCompareChar( longName[c], *tst) ) tst++;
      else break;
    return longName[c] == 0 && tst == tail;
    }
  //��������� ��� �� ����������
  for( c = 0; c < 8 && mFileName[c] != ' '; c++ )
    if( cmtCompareChar( mFileName[c], *tst) ) tst++;
    else break;
  //������� �������� ��� ����������
  if( tst == tail ) {
    if( (c == 8 || mFileName[c] == ' ') && mExtension[0] == ' ' )
      return 1; //��� ���������, ���������� ���
    return 0; //���������� ����, � � ������� ���
    }
  if( *tst != '.' || (c < 8 && mFileName[c] != ' ') ) return 0; //������ �� ���������
  tst++;
  //��������� ���������� �� ����������
  for( c = 0; c < 3 && mExtension[c] != ' '; c++ )
    if( cmtCompareChar( mExtension[c], *tst) ) tst++;
    else break;
  if( tst == tail ) return 1; //��� ���������
  return 0; //������ �� ���������
  }

uint_8
FatDirEntry1x::PatternName( cpchar longName, cpchar tst, uint8 findFlag ) {
  int i;
  //��������� ��� �� ����������
  if( mAttrib & FA_LABEL ) return 0;
  if( findFlag && (mAttrib & findFlag) == 0 ) return false;
  if( mFileName[0] == '.' && mFileName[1] == '.' && tst[0] == '*' ) return 1;
  if( mFileName[0] && mFileName[0] != 0xe5 ) {

    if( longName[0] ) {
      //���������� � ������� ������
      for( i = 0; longName[i] && i < 255; ++i )
        if( *tst == '*' ) {
          if( cmtCompareChar( tst[1], longName[i] ) ) tst += 2;
          }
        else if( *tst == '?' ) tst++; //����� ������
        else if( cmtCompareChar( *tst, longName[i] ) ) tst++; //������ ���������� ��������
        else return 0;
      return longName[i] == 0 && (tst[0] == 0 || (tst[0] == '*' && tst[1] == 0) ||
           (tst[0] == '*' && tst[1] == '.' && tst[2] == '*' && tst[3] == 0));
      }
    //Name comparison
    for( i = 0; mFileName[i] != ' ' && i < 8; ++i )
      if( *tst == '*' ) {
        if( cmtCompareChar( tst[1], mFileName[i] ) ) tst += 2;
        }
      else if( *tst == '?' ) tst++; //����� ������
      else if( cmtCompareChar( *tst, mFileName[i] ) ) tst++; //������ ���������� ��������
      else return 0;

    if( *tst == '*' ) tst++;
    if( *tst == '.' ) tst++;
    else return 0;
    if( !*tst ) return mExtension[0] == ' ';

    //Extension comparison
    for( i = 0; mExtension[i] != ' ' && i < 3; ++i )
      if( *tst == '*' ) {
        if( cmtCompareChar( tst[1], mExtension[i] ) ) tst += 2;
        }
      else if( *tst == '?' ) tst++;
      else if( cmtCompareChar( *tst, mExtension[i] ) ) tst++;
      else return 0;

    return (*tst == '*' && tst[1] == 0) || (*tst == 0);
    }
  return 0;
  }

uint32
FatDirEntry1x::GetFirstCluster32() {
  uint32 cluster = mFirstClusterHigh;
  cluster <<= 16;
  cluster |= mFirstCluster;
  return cluster;
  }

void
FatDirEntry1x::SetFirstCluster32( uint32 cluster ) {
  mFirstCluster = (uint16)cluster;
  mFirstClusterHigh = (uint16)(cluster >> 16);
  }
