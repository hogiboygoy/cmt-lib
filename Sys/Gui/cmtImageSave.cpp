//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

/* ������
  �������
  5-0 ���
  7-6 ��� �������
      00 ������ ���+1 ��� ���������� �� �������� �����)
      01 ������ � ���������� ���+2, +1 ���� ��������
      10 ���� �� �������� ����� ���+1
      11 ���� �� ���������� ������ �� �������� ���+1-32
  ���� �������� ������ (�����)
  5-0 ����
  6 - 0-������� ��������
      1-���������� ��������
  7 - 0-����� ������ + ����
      1-����� ������ + ���� - 64 + ����� ������

  */
bool
CmtImageCmiTestLine( uint32 *ptr, int ref, int len ) {
  //��������� �� ���������� ����� ref ����� ������ len
  for( int i = 0; i < len; i++ )
    if( ptr[i-ref] != ptr[i] ) return false;
  return true;
  }



bool
CmtImage::SaveCmt( CmtFile &os ) {
  int32 count = mFrameCount * mFrameSize;
  if( count == 0 || mFrames == 0 ) return false;
  //�������� ���������
  os.WriteBinU32( CMT_IMAGE_SIGNATURE );
  //��������� �����������
  os.WriteBinU32( mFrameCount );
  os.WriteBinU32( mFrameSize );
  os.WriteBinU32( mSize.x );
  os.WriteBinU32( mSize.y );
  //������� ������
  os.WriteBinU8( 0 );
  uint32 color = mFrames[0] & 0xffffff;
  os.Write( &color, 3 );
  //������ ������� ��������
  for( int32 i = 1; i < count; ) {
    //������� 0 (�����������)
    uint8 atr0 = 0;
    color = mFrames[i] & 0xffffff;
    while( atr0 < 63 ) {
      if( i + atr0 + 1 >= count || color != (mFrames[i+atr0+1] & 0xffffff) ) break;
      atr0 += 1;
      }

    //������� 2
    uint8 atr2;
    for( atr2 = 0; atr2 < 64 && (mFrames[i-atr2-1] & 0xffffff) != color; atr2++ )
      if( i == atr2 + 1 ) atr2 = 63;

    //������� 3
    uint8 atr3 = 64;
    if( atr2 > 63 && i > mSize.x + 32 ) {
      if( mSize.x > 32 ) {
        for( atr3 = 0; atr3 < 64 && (mFrames[i+32-mSize.x-atr3] & 0xffffff) != color; atr3++ )
          if( i <= mSize.x - 32 + atr3 ) atr3 = 63;
        }
      else {
        for( atr3 = 33 - mSize.x; atr3 < 32 + mSize.x && (mFrames[i+32-mSize.x-atr3] & 0xffffff) != color; atr3++ )
          if( i <= mSize.x - 32 + atr3 ) atr3 = 63;
        if( atr3 >= 32 + mSize.x ) atr3 = 64;
        }
      }

    //������� 1
    uint8 atr1 = 64;
    uint8 boff = 0;
    if( mSize.x > 32 ) {
      for( uint8 atr1t = i + 65 > count ? count - i : 65; atr1t > 1 && atr1 == 64; atr1t-- ) {
        //��������� ��� �������� ��������
        for( int offs = 0; offs < 256; offs++ ) {
          int back;
          if( offs & 0x80 ) {
            //���������� ��������
            if( offs & 0x40 ) {
              //�� ���������� ������ ���������� ��������
              back = atr1t + (offs & 0x3f) - 32 + mSize.x + mFrameSize;
              }
            else {
              //�� ������� ������ ���������� ��������
              back = atr1t + (offs & 0x3f) + mFrameSize;
              }
            }
          else {
            //������� ��������
            if( offs & 0x40 ) {
              //���������� ������
              back = atr1t + (offs & 0x3f) - 32 + mSize.x;
              }
            else {
              //������� ������
              back = atr1t + (offs & 0x3f);
              }
            }
          if( back <= i ) {
            if( CmtImageCmiTestLine( mFrames + i, back, atr1t ) ) {
              atr1 = atr1t - 2;
              boff = (uint8)offs;
              break;
              }
            }
          else break;
          }
        }
      }

    //����� ��������
    if( atr1 < 64 ) {
      if( atr0 < atr1+2 ) {
        //������� 1
        os.WriteBinU8( atr1 | 0x40 );
        os.WriteBinU8( boff );
        i += atr1+2;
        }
      else {
        //������� 0
        os.WriteBinU8( atr0 );
        os.Write( &color, 3 );
        i += atr0+1;
        }
      }
    else {
      if( (atr2 < 64 || atr3 < 64) && atr0 < 3 ) {
        if( atr2 < 64 ) {
          //������� 2
          os.WriteBinU8( atr2 | 0x80 );
          }
        else {
          //������� 3
          os.WriteBinU8( atr3 | 0xc0 );
          }
        i++;
        }
      else {
        //������� 0
        os.WriteBinU8( atr0 );
        os.Write( &color, 3 );
        i += atr0+1;
        }
      }

    }
  return true;
  }
