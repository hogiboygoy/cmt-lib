//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#define CMT_IMAGE_SIGNATURE 0x43495063
//==============================================================================
//------------------ CmtImage ----------------------------------------------------
CmtPath CmtImage::mDefPath;

CmtImage::CmtImage( uint32 *src ) :
  mDinamic(false),
  mFrameCount(0),
  mFrames(0),
  mFrameSize(0),
  mSize(0,0) {
    if( src ) Setup( src );
    }

CmtImage::CmtImage( CmtImage *src ) :
  mDinamic(false),
  mFrameCount(0),
  mFrames(0),
  mFrameSize(0),
  mSize(0,0) {
    if( src ) CopyFrom( src, false );
    }

CmtImage::~CmtImage() {
  Free();
  }

void
CmtImage::CreateShadow() {
  if( mDinamic ) {
    for( uint32 i = 1; i < mFrameSize; i++ ) {
      uint32 color = mFrames[i]; //�������� ����
      uint32 val = (CMT_R(color) + CMT_G(color) + CMT_B(color)) / 3; //��������� �������
      mFrames[i] = (color & 0xff000000) | ARGB32(0,val,val,val); //�������� �����
      }
    }
  }


void
CmtImage::Free() {
  if( mDinamic && mFrames ) {
    delete mFrames;
    mDinamic = false;
    mFrames  = 0;
    mFrameCount = 0;
    }
  else {
    mDinamic = false;
    mFrames  = 0;
    mFrameCount = mSize.x = mSize.y = 0;
    }
  }


void
CmtImage::Setup( uint32 *src ) {
  Free();
  if( src ) {
    if( src[0] == 0x3a676d69 ) {
      //��������� �� �������� �� ���������
      DefLoad( (cpchar)(src+1) );
      }
    else {
      mDinamic    = false;
      mSize.x     = src[0];
      mSize.y     = src[1];
      mFrameSize  = src[2];
      mFrameCount = src[3];
      mFrames     = src + 4;
      }
    }
  }

void
CmtImage::CopyFrom( CmtImage *src, bool noRef ) {
  if( src == 0 ) {
    Free();
    mDinamic = false;
    mSize.x = mSize.y = 0;
    }
  else if( !src->mDinamic && !noRef ) {
    //�������� ���������� ��������������
    Free();
    mDinamic    = false;
    mSize       = src->mSize;
    mFrameCount = src->mFrameCount;
    mFrameSize  = src->mFrameSize;
    mFrames     = src->mFrames;
    }
  else {
    //����������� �������������� ��������
    Free();
    mDinamic    = true;
    mSize       = src->mSize;
    mFrameCount = src->mFrameCount;
    mFrameSize  = src->mFrameSize;
    uint32 size = mFrameCount * mFrameSize;
    mFrames = new uint32[size];
    memcpy( mFrames, src->mFrames, size * sizeof(uint32) );
    }
  }

void
CmtImage::Alloc( CmtPoint size, uint32 frameCount ) {
  Free();
  if( size.x <= 0 || size.y <= 0 || frameCount == 0 ) return;
  mSize       = size;
  mFrameCount = frameCount;
  mFrameSize  = size.x * size.y + 1;
  mFrames = new uint32[mFrameSize * mFrameCount];
  memset( mFrames, 0, sizeof(uint32) * mFrameSize * mFrameCount );
  }

uint32*  
CmtImage::FrameData( CmtPoint pos, uint32 frameIndex ) {
  //��������� �������
  if( pos.x < 0 || pos.x >= mSize.x || pos.y < 0 || pos.y > mSize.y || frameIndex >= mFrameCount ) return 0;
  return Frame( frameIndex ) + pos.x + pos.y * mSize.x;
  }

//===============================================================================
//----------------- ��������� ������ � ClipBoard --------------------------------
#if 0
bool
CmtImage::PutOnClipBoard( uint32 frame ) {
  bool res = true;
  if( OpenClipboard( MApplication::mWnd ) ) {
    EmptyClipboard();
    HANDLE hMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, sizeof( BITMAPINFO ) + mFrameSize * sizeof(uint32) );
    if( hMem ) {
      void *ptr = GlobalLock( hMem );
      if( ptr ) {
        //�������� ��������
        BITMAPINFO *bmi = (BITMAPINFO*)ptr;
        bmi->bmiHeader.biSize   = sizeof(BITMAPINFO);
        bmi->bmiHeader.biWidth  = mSize.x;
        bmi->bmiHeader.biHeight = mSize.y;
        bmi->bmiHeader.biPlanes = 1;
        bmi->bmiHeader.biBitCount = 32;
        bmi->bmiHeader.biCompression = BI_RGB;
        bmi->bmiHeader.biSizeImage   = mSize.x * mSize.y * 4;
        bmi->bmiHeader.biXPelsPerMeter = 0;
        bmi->bmiHeader.biYPelsPerMeter = 0;
        bmi->bmiHeader.biClrUsed = 0;
        bmi->bmiHeader.biClrImportant = 0;
        //������ ��������
        memcpy( ((uint8*)ptr) + sizeof(BITMAPINFO), mFrames + 1 + mFrameSize * frame, mSize.x * mSize.y * 4 );
        GlobalUnlock( hMem );
        if( SetClipboardData( CF_DIB, hMem ) == 0 ) {
          GlobalFree( hMem );
          res = false;
          }
        }
      else res = false;
      }
    else res = false;
    CloseClipboard();
    }
  else res = false;
  return res;
  }

bool
CmtImage::GetFromClipBoard() {
  bool res = true;
  //���������� ������
  Free();
  if( OpenClipboard( MApplication::mWnd ) ) {
    if( IsClipboardFormatAvailable( CF_DIB ) ) {
      HANDLE hMem = GetClipboardData( CF_DIB );
      if( hMem ) {
        void *ptr = GlobalLock( hMem );
        if( ptr ) {
          //�������� ��������
          BITMAPINFO *bmi = (BITMAPINFO*)ptr;
          //�������� ���������� ������
          if( bmi->bmiHeader.biPlanes == 1 && bmi->bmiHeader.biCompression == BI_RGB ) {
            mSize.x = bmi->bmiHeader.biWidth;
            mSize.y = bmi->bmiHeader.biHeight;
            mDinamic = true;
            mFrameCount = 1;
            mFrameSize = mSize.x * mSize.y + 1;
            mFrames = new uint32[mFrameSize];
            mFrames[0] = 0;
            if( bmi->bmiHeader.biBitCount == 32 ) {
              memcpy( mFrames + 1, ((uint8*)ptr) + bmi->bmiHeader.biSize, (mFrameSize - 1) * sizeof(uint32) );
              }
            else if( bmi->bmiHeader.biBitCount == 24 ) {
              //�������� ���������� ������
              uint8 *pixel = ((uint8*)ptr) + bmi->bmiHeader.biSize;
              int x = 0;
              //��������� ������ ����������
              for( int y = mSize.y - 1; y >= 0; y-- ) {
                for( int x = 0; x < mSize.x; x++ ) {
                  mFrames[x + y * mSize.x + 1] = RGB( pixel[0], pixel[1], pixel[2] );
                  pixel += 3;
                  }
                //������ � BMP ��������� �� 4 ������
                pixel += (4 - ((mSize.x * 3) & 3)) & 3;
                }
              }
            else res = false;
            }
          GlobalUnlock( hMem );
          }
        else res = false;
        }
      else res = false;
      }
    else res = false;
    CloseClipboard();
    }
  else res = false;
  return res;
  }
#endif

#ifdef CMT_FS
//===============================================================================
//----------------- ��������� �������� �������� �� ����� ------------------------
int tampax;
bool
CmtImage::Load( cpchar fileName ) {
  //���������� ������
  Free();
  //��������� ������� �����
  if( cmtTestFileExist( fileName ) != CMTE_OK ) return false;
  CmtPath ext( fileName );
  ext.CutExtension();
  CmtFileRead is( fileName );
  if( !is.Good() ) return false;
  if( ext.CmpEq( ".bmp", false ) ) 
    return LoadBMP( is );
  if( ext.CmpEq( ".gif", false ) ) 
    return LoadGIF( is );
  if( ext.CmpEq( ".jpg", false ) ) 
    return LoadJPG( is );
  if( ext.CmpEq( ".cmi", false ) )
    return LoadCmt( is );
  return false;
  }
#endif

CmtImage* 
CmtImage::DefLoad( cpchar fileName ) {
  CmtPath name( mDefPath );
  name.Add( fileName );
  Load( name.Buf() );
  return this;
  }

//===============================================================================
//----------------- �������� �� ����� ���� CMI ----------------------------------
bool
CmtImage::LoadCmt( CmtFile &is ) {
  //��������� � ��������� ���������
  if( is.ReadBinU32() != CMT_IMAGE_SIGNATURE ) return false;
  mFrameCount = is.ReadBinU32();
  mFrameSize  = is.ReadBinU32();
  mSize.x     = is.ReadBinU32();
  mSize.y     = is.ReadBinU32();
  //�������� ������
  int32 count = mFrameCount * mFrameSize;

  mFrames = new uint32[count];
  mDinamic = true;

  int i = 0;
  while( i < count ) {
    //������� ���
    uint8 len = is.ReadBinU8();
    uint8 cmd = len & 0xc0;
    len &= 0x3f;
    if( cmd == 0 ) {
      //������ �����
      uint32 color = 0;
      is.Read( &color, 3 );
      do {
        mFrames[i++] = color;
        }
      while( len-- != 0 );
      }
    else if( cmd == 0x40 ) {
      //����������� �����
      int offs = is.ReadBinU8();
      int back;
      len += 2;
      if( offs & 0x80 ) {
        //���������� ��������
        if( offs & 0x40 ) {
          //�� ���������� ������ ���������� ��������
          back = len + (offs & 0x3f) - 32 + mSize.x + mFrameSize;
          }
        else {
          //�� ������� ������ ���������� ��������
          back = len + (offs & 0x3f) + mFrameSize;
          }
        }
      else {
        //������� ��������
        if( offs & 0x40 ) {
          //���������� ������
          back = len + (offs & 0x3f) - 32 + mSize.x;
          }
        else {
          //������� ������
          back = len + (offs & 0x3f);
          }
        }
      for( int k = 0; k < len; k++ ) {
        mFrames[i] = mFrames[i-back];
        i++;
        }
      }
    else if( cmd == 0x80 ) {
      mFrames[i] = mFrames[i-len-1];
      i++;
      }
    else {
      mFrames[i] = mFrames[i+32-mSize.x-len];
      i++;
      }
    }
  return true;
  }
  
//===============================================================================
//----------------- �������� �� ����� ���� BMP ----------------------------------
struct mvnRGBQUAD {
  uint8   rgbBlue;
  uint8   rgbGreen;
  uint8   rgbRed;
  uint8   rgbReserved;
  };

uint32
RGB32( mvnRGBQUAD rgb ) {
  return MRGB32(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
  }

uint32 backColor;

uint32
ColorT32( uint32 color ) {
  if( color == backColor )
    return 0x1000000 | color;
  return color;
  }

#define WIDTHBYTES(bits)      ((((bits) + 31) & ~31) >> 3)

#pragma pack(1)
struct mvnBITMAPINFOHEADER {
  uint32 biSize;
  int32  biWidth;
  int32  biHeight;
  uint16 biPlanes;
  uint16 biBitCount;
  uint32 biCompression;
  uint32 biSizeImage;
  int32  biXPelsPerMeter;
  int32  biYPelsPerMeter;
  uint32 biClrUsed;
  uint32 biClrImportant;
  };

bool
CmtImage::LoadBMP( CmtFile &is ) {
  uint32 bfSize = 0;
  uint32 bfOffBits;

  // read the BITMAPFILEHEADER from our file
  is.Read( &bfSize, 2 ); //���

  //�������� ���� �����
  if( bfSize != 0x4d42 )  // 'BM'
    return false;

  is.Read( &bfSize, 4 ); //������

  is.Read( &bfOffBits, 4 ); //Reserved
  is.Read( &bfOffBits, 4 ); //Reserved

  // Allocate memory for header & color table.
  mvnBITMAPINFOHEADER *lpbih;
  lpbih = (mvnBITMAPINFOHEADER*)malloc( sizeof(mvnBITMAPINFOHEADER) + 256 * sizeof(mvnRGBQUAD) );

  if( !lpbih ) return false;

  // read the BITMAPINFOHEADER
  is.Read( lpbih, sizeof(mvnBITMAPINFOHEADER) );

  // Check to see that it's a Windows DIB -- an OS/2 DIB would cause
  // problems with some of the functions in this sample since the fields
  // in the header are different and color table entries are smaller.
  // If it's not a Windows DIB (e.g. if biSize is wrong), return NULL.
  if( lpbih->biSize != sizeof(mvnBITMAPINFOHEADER) ) {
    //lpbih->biSize = sizeof(mvnBITMAPINFOHEADER);
    free( lpbih );
    return false;
    }

  // check to see if it's a format we support
  switch (lpbih->biBitCount) {
    case 1:
    case 4:
    case 8:
    case 16:
    case 24:
    //case 32:
      // the format is supported so continue
      break;

    default:
      // the format isn't supported so fail
      free( lpbih );
      return false;
    }

  // Now determine the size of the color table and read it.  Since the
  // bitmap bits are offset in the file by bfOffBits, do some special
  // processing to make sure the bits directly follow the color table
  int nNumColors = lpbih->biClrUsed;
  if( !nNumColors ) {
    // color table only for 8 bits per pixel or less
    if( lpbih->biBitCount <= 8 )
      nNumColors = 1 << lpbih->biBitCount; // standard size table
    }

  // fill in some default values if they are zero
  if( lpbih->biClrUsed == 0 )
    lpbih->biClrUsed = nNumColors;

  if( lpbih->biSizeImage == 0 )
    lpbih->biSizeImage = WIDTHBYTES(lpbih->biWidth * lpbih->biBitCount) * lpbih->biHeight;

  if( nNumColors > 256 ) {  // very rare, but possible
    free( lpbih );
    return false;
    }

  // read the color table
  if( nNumColors )
    is.Read( (pchar)(lpbih) + lpbih->biSize, nNumColors * sizeof(mvnRGBQUAD) );

  mvnRGBQUAD *colorTable = (mvnRGBQUAD*)((pchar)(lpbih) + lpbih->biSize);

  uint8 *_array = new uint8 [lpbih->biSizeImage];
  is.Seek( bfOffBits );
  is.Read( _array, lpbih->biSizeImage );

  //���������
  int hy = lpbih->biHeight > 0 ? lpbih->biHeight : -lpbih->biHeight;
  //��������� ���������� ��������
  Free();
  mDinamic    = true;
  mSize.x     = lpbih->biWidth;
  mSize.y     = hy;
  mFrameCount = 1; //���������� �������
  //���������� �����
    mFrameSize  = 1 + mSize.x * mSize.y; //������ ������ (� ������)
    //������������ ������ ��� ������
    mFrames = new uint32[mFrameSize];
    mFrames[0] = 0; //����� ����������� ������
    uint32 *wimage = mFrames + 1;
    int  wimageSize = 0;
    int strLen = WIDTHBYTES(lpbih->biWidth * lpbih->biBitCount);
    switch (lpbih->biBitCount) {
      case 1: {
        //��� ���� �����
        for( int y = 0; y < hy; y++ ) {
          //��� ������ ������
          uint8 *start = _array + (lpbih->biHeight < 0 ? y : hy - y - 1) * strLen;
          for( int x = 0; x < lpbih->biWidth; x += 8 ) {
            uint8 byte = start[ x >> 3 ];
            for( int dx = 0; dx < 8 && x + dx < lpbih->biWidth; dx++ ) {
              //����� ����
              wimage[wimageSize++] = RGB32(colorTable[byte & 1]);
              byte >>= 1;
              }
            }
          }
        }
        break;
      case 4: {
        //���� ����
        backColor = RGB32(colorTable[_array[ (lpbih->biHeight < 0 ? 0 : hy - 1) * strLen ] & 0xf ] );
        //��� ���� �����
        for( int y = 0; y < hy; y++ ) {
          //��� ������ ������
          uint8 *start = _array + (lpbih->biHeight < 0 ? y : hy - y - 1) * strLen;
          for( int x = 0; x < lpbih->biWidth; x++ ) {
            uint8 byte = start[ x >> 1 ];
            if( x & 1 ) byte >>= 4;
            //����� ��� �����
            wimage[wimageSize++] = ColorT32( RGB32(colorTable[byte & 0xf]) );
            }
          }
        }
        break;
      case 8: {
        //���� ����
        backColor = RGB32( colorTable[_array[ (lpbih->biHeight < 0 ? 0 : hy - 1) * strLen ] ] );
        //��� ���� �����
        for( int y = 0; y < hy; y++ ) {
          //��� ������ ������
          uint8 *start = _array + (lpbih->biHeight < 0 ? y : hy - y - 1) * strLen;
          for( int x = 0; x < lpbih->biWidth; x++ ) {
            //����� ����
            wimage[wimageSize++] = ColorT32( RGB32( colorTable[start[x]] ) );
            }
          }
        }
        break;
      case 16:
        if( lpbih->biCompression == 0 ) {
          //��� ���� �����
          for( int y = 0; y < hy; y++ ) {
            //��� ������ ������
            //uint8 *start = _array + (lpbih->biHeight < 0 ? y : hy - y - 1) * strLen;
            for( int x = 0; x < lpbih->biWidth; x++ ) {
              //����� ����
              wimage[wimageSize++] = 0; //RotateColor( start[x << 1], start[(x << 1) + 1] );
              }
            }
          }
        else {
          //��� ���� �����
          for( int y = 0; y < hy; y++ ) {
            //��� ������ ������
            //uint8 *start = _array + (lpbih->biHeight < 0 ? y : hy - y - 1) * strLen;
            for( int x = 0; x < lpbih->biWidth; x++ ) {
              //����� ����
              wimage[wimageSize++] = 0; //RGB555( colorTable[start[x << 1] | (start[(x << 1) + 1] << 8)] );
              }
            }
          }
        break;
      case 24: {
        uint8 *start = _array + (lpbih->biHeight < 0 ? 0 : hy - 1) * strLen;
        //���� ����
        backColor = MRGB32( start[0], start[1], start[2] );
        //��� ���� �����
        for( int y = 0; y < hy; y++ ) {
          //��� ������ ������
          start = _array + (lpbih->biHeight < 0 ? y : hy - y - 1) * strLen;
          for( int x = 0; x < lpbih->biWidth; x++ ) {
            //����� ����
            int px = x * 3;
            wimage[wimageSize++] = ColorT32( MRGB32( start[px], start[px+1], start[px+2] ) );
            }
          }
        }
        break;
      }
  if( lpbih ) free( lpbih );
  if( _array ) delete _array;
  return true;
  }

//===============================================================================
//----------------- �������� �� ����� ���� GIF ----------------------------------
struct GIFScreenDesc {
  int16 width;
  int16 height;
  uint8 dif;
  uint8 fonColor;
  uint8 nulls;
  };

#define HAS_GMAP(r)    (((r).dif & 0x80) != 0)
#define RESOLUTION(r)  ((((r).dif & 0x70) >> 4) + 1)
#define MAXCOLOR(r)    (((r).dif & 0x07) + 1)

struct GIFImageDesc {
  int16 left;
  int16 top;
  int16 width;
  int16 height;
  uint8 dif;
  };

#define IMAGE_DESC 0x2c
#define FUN_DESC   0x21
#define USE_GMAP(r) (((r).dif & 0x80) == 0)
#define SEQUENCE(r) (((r).dif & 0x40) == 0)

struct GIFrgb {
  uint8 red;
  uint8 green;
  uint8 blue;

  //void operator = ( int val ) { red = val; }
  };

#define LZ_MAX_CODE      4095
#define NOT_CODE         4098
#define MAX_BIT_PER_CODE 12

typedef uint32 (*FPGetColor)( uint8 *sour, int code );

uint32
GIFGetColor( uint8 *sour, int code ) {
  sour += code * 3;
  return (sour[2] << 16) | (sour[1] << 8) | sour[0] | 0xff000000;
  }

uint32
GIFGetColorBGR( uint8 *sour, int code ) {
  sour += code * 3;
  return (sour[0] << 16) | (sour[1] << 8) | sour[2] | 0xff000000;
  }

FPGetColor fpGetColor;

class GIFReader {
    int    bitPerCode;
    int    bitPerPixel;
    int    EOFCode;
    int    ClearCode;
    uint8  buffer[256];
    int    nextByte;
    uint32 codeBuffer;
    uint32 runCode;
    int    curBufferShift;
    int    stack[LZ_MAX_CODE];
    int    stackCount;
    int    prefix[LZ_MAX_CODE + 1];
    int    sufix[LZ_MAX_CODE + 1];
    int    oldCode;
    int    curPrefix;
    int    maxCodeMult2;
    int    stackCode;
    bool   transparent;
    int    transparentIndex;
    uint8 *globColorMap;
    uint8 *locColorMap;
    uint8 *colorMap;
  public:
    GIFReader(bool b) :
      globColorMap(0),
      locColorMap(0),
      colorMap(0) {
        fpGetColor = b ? GIFGetColorBGR : GIFGetColor;
        }
    ~GIFReader();

    bool  Load( CmtFile &is, CmtImage *image );
  protected:
    void  GIFReadColor( CmtFile &is, int size, bool glb );
    void  GIFInitDecompress( CmtFile &is );
    bool  GIFDecompresLine( CmtFile &is, uint32 *line, int lineLen );
    void  GIFSetPixel( int index, uint32 *dest, int code );
  protected:
    uint8 GetByte( CmtFile &is );
    int   GIFDecompressCode( CmtFile &is );
  };

GIFReader::~GIFReader()  {
  if( globColorMap ) delete globColorMap;
  if( locColorMap )  delete locColorMap;
  }

int
GetPrefixChar( int *prefix, int code, int ClearCode ) {
  for( int i = 0; code > ClearCode && i++ <= LZ_MAX_CODE; )
    code = prefix[code];
  return code;
  }

void
GIFReader::GIFReadColor( CmtFile &is, int size, bool glb ) {
  if( glb ) {
    if( globColorMap ) delete globColorMap;
    colorMap = globColorMap = new uint8[size];
    }
  else {
    if( locColorMap ) delete locColorMap;
    colorMap = locColorMap = new uint8[size];
    }
  is.Read( colorMap, size );
  }

void
GIFReader::GIFSetPixel( int index, uint32 *dest, int code ) {
  if( transparent && code == transparentIndex );
    //dest[index] = TRANSPARENT32;
  else
  //if( !transparent || code != transparentIndex )
    dest[index] = fpGetColor( colorMap, code );
  }

void
GIFReader::GIFInitDecompress( CmtFile &is ) {
  bitPerPixel     = 0; //(d->dif & 0x7) + 1;
  is.Read( &bitPerPixel, 1 );
  buffer[0]       = 0;
  nextByte        = 0;
  ClearCode       = 1 << bitPerPixel;
  EOFCode         = ClearCode + 1;
  runCode         = EOFCode + 1;
  bitPerCode      = bitPerPixel + 1;
  maxCodeMult2    = 1 << bitPerCode;
  stackCount      = 0;
  oldCode         = NOT_CODE;
  curBufferShift  = 0;
  codeBuffer      = 0;
  for( int i = 0; i <= LZ_MAX_CODE; ++i )
    sufix[i] = prefix[i] = NOT_CODE;
  }

bool
GIFReader::GIFDecompresLine( CmtFile &is, uint32 *line, int lineLen ) {
  int i = 0,
      j,
      code;

  if( stackCount != 0 ) {
    while( stackCount != 0 && i < lineLen )
      GIFSetPixel( i++, line, stack[--stackCount] );
    }

  while( i < lineLen ) {
    code = GIFDecompressCode( is );
    if( code == EOFCode ) i++;
    else if( code == ClearCode ) {
      for( j = 0; j < LZ_MAX_CODE; ++j )
        prefix[j] = NOT_CODE;
      runCode = EOFCode + 1;
      bitPerCode = bitPerPixel + 1;
      maxCodeMult2 = 1 << bitPerCode;
      oldCode = NOT_CODE;
      }
    else {
      if( code < ClearCode )
        GIFSetPixel( i++, line, code );
      else {
        if( prefix[code] == NOT_CODE ) {
          if( (unsigned)(code) == runCode - 2 ) {
            curPrefix = oldCode;
            sufix[runCode-2] = GetPrefixChar( prefix, oldCode, ClearCode );
            stack[stackCount++] = sufix[runCode - 2];
            }
          else return false;
          }
        else curPrefix = code;

        j = 0;
        while( j++ <= LZ_MAX_CODE && curPrefix > ClearCode && curPrefix <= LZ_MAX_CODE ) {
          stack[stackCount++] = sufix[curPrefix];
          curPrefix           = prefix[curPrefix];
          }
        stack[stackCount++] = curPrefix;
        while( stackCount != 0 && i < lineLen )
          GIFSetPixel( i++, line, stack[--stackCount] );
        }
      if( oldCode != NOT_CODE ) {
        prefix[runCode - 2]= oldCode;
        if( (unsigned)(code) == runCode - 2 )
          sufix[runCode-2] = GetPrefixChar( prefix, oldCode, ClearCode );
        else
          sufix[runCode-2] = GetPrefixChar( prefix, code, ClearCode );
        }
      oldCode = code;
      }
    }
  return true;
  }

int
GIFReader::GIFDecompressCode( CmtFile &is ) {
  uint8  byte;
  uint32 longByte;

  while( curBufferShift < bitPerCode ) {
    byte = GetByte( is );
    longByte = byte;
    longByte <<= curBufferShift;
    codeBuffer |= longByte;
    curBufferShift += 8;
    }

  int code = codeBuffer & (0xffffffff >> (32 - bitPerCode));

  codeBuffer     >>= bitPerCode;

  curBufferShift -= bitPerCode;

  if( ++runCode > (unsigned)(maxCodeMult2) && bitPerCode < MAX_BIT_PER_CODE ) {
    maxCodeMult2 <<= 1;
    bitPerCode++;
    }

  return code;
  }

uint8
GIFReader::GetByte( CmtFile &is ) {
  uint8 result;
  if( buffer[0] == 0 ) {
    is.Read( buffer, 1 );
    is.Read( buffer + 1, buffer[0] );
    result = buffer[1];
    nextByte = 2;
    buffer[0]--;
    }
  else {
    result = buffer[nextByte++];
    buffer[0]--;
    }
  return result;
  }

bool
GIFReader::Load( CmtFile &is, CmtImage *image ) {
  char            label[8];
  GIFScreenDesc   screenD;
  GIFImageDesc    imageD;
  uint32         *work, *prevImage = 0;
  int             time = 0;
  int             mode = 0;

  transparent = false;

  //����� �����
  is.Read( label, 6 );
  label[6] = 0;
  if( strcmp( label, "GIF87a" ) && strcmp( label, "GIF89a" ) ) return false;

  //��������� ������
  is.Read( &screenD, 7 );

  //������������ ������ ��� �������� ������
  work = new uint32[screenD.width * screenD.height];
  image->mFrameSize = screenD.width * screenD.height + 1;
  if( !work ) return false;
  memset( work, 0, sizeof(uint32) * screenD.width * screenD.height );

  //��������� ��������
  image->mSize.x = screenD.width;
  image->mSize.y = screenD.height;

  //������� ���������� ������� ������ (���� ����)
  if( HAS_GMAP(screenD) )
    GIFReadColor( is, 3 * (0x1 << MAXCOLOR(screenD)), true );

  uint8 ch;
  while( is.GetPos() < is.Size() ) {
    if( !is.Good() ) break;
    is.Read( &ch, 1 );

    switch( ch ) {
      case FUN_DESC   :
        is.Read( &ch, 1 );  //�������� ����� �������
        if( ch == 0xf9 ) {
          //���� ���������� �������� (��������)
          is.Read( &ch, 1 );  //������ �����
          is.Read( &ch, 1 );  //������� ����
          mode = (ch >> 2) & 0x7; //����� ����������
          transparent = ch & 1;   //����� ������������
          is.Read( &time, 2 );    //����� ��������
          is.Read( &ch, 1 );  //������ ����������� �����
          transparentIndex = ch & 0xff;
          is.ReadBinU8(); //���������� ���������� �����
          }
        else if( ch == 0xff || ch == 0xfe || ch == 0x01 ) {
          //��������� ����������
          is.Read( &ch, 1 ); //���������� ���������� �������
          is.Seek( ch + is.GetPos() ); //���������� ���������
          is.Read( &ch, 1 );
          while( ch ) {
            is.Seek( ch + is.GetPos() );
            is.Read( &ch, 1 );
            if( !is.Good() ) break;
            }
          }
        else {
          is.Read( &ch, 1 ); //���������� ���������� �������
          is.Seek( ch + is.GetPos() + 1 ); //���������� ���������
          }
        continue;
      case IMAGE_DESC : {
        is.Read( &imageD, 9 );
        if( HAS_GMAP(imageD) )
          GIFReadColor( is, 3 * (0x1 << ((imageD.dif & 0x7) + 1)), false );
        else
          colorMap = globColorMap;
        GIFInitDecompress( is );
        if( SEQUENCE(imageD) ) {
          for( int y = 0; y < imageD.height; ++y )
            if( !GIFDecompresLine( is, work + (y + imageD.top) * screenD.width + imageD.left, imageD.width ) ) {
              delete work;
              return false;
              }
          }
        else {
          int y;
          for( y = 0; y < imageD.height; y += 8 )
            if( !GIFDecompresLine( is, work + (y + imageD.top) * screenD.width + imageD.left, imageD.width ) ) {
              delete work;
              return false;
              }
          for( y = 4; y < imageD.height; y += 8 )
            if( !GIFDecompresLine( is, work + (y + imageD.top) * screenD.width + imageD.left, imageD.width ) ) {
              delete work;
              return false;
              }
          for( y = 2; y < imageD.height; y += 4 )
            if( !GIFDecompresLine( is, work + (y + imageD.top) * screenD.width + imageD.left, imageD.width ) ) {
              delete work;
              return false;
              }
          for( y = 1; y < imageD.height; y += 2 )
            if( !GIFDecompresLine( is, work + (y + imageD.top) * screenD.width + imageD.left, imageD.width ) ) {
              delete work;
              return false;
              }
          }
        is.ReadBinU8(); //���������� ���������� �����
        image->mFrameCount++;
        uint32 *imgData = new uint32[ image->mFrameCount * image->mFrameSize ];
        if( !imgData ) {
          delete work;
          return false;
          }
        if( image->mFrames ) {
          //���������� ���������� ��������
          memcpy( imgData, image->mFrames, (image->mFrameCount - 1) * image->mFrameSize * sizeof(uint32) );
          delete image->mFrames;
          image->mFrames = 0;
          }
        //���������� �����������
        memcpy( imgData + ((image->mFrameCount - 1) * image->mFrameSize + 1), work, screenD.width * screenD.height * sizeof(uint32) );
        image->mFrames = imgData;
        image->mDinamic = true;
        //����������� ��� ���������� �����������
        switch( mode ) {
          case 0 :
          case 1 : break;
          case 2 : {
            uint32 backColor = GIFGetColor( globColorMap, screenD.fonColor ) & 0xffffff;
            //����������� ��������� �����
            for( int y = 0; y < imageD.height; y++ )
              for( int x = 0; x < imageD.width; x++ )
                work[(y + imageD.top) * screenD.width + x + imageD.left] = backColor;
            }
            break;
          case 3 :
            //������������ ���������� ������������
            if( prevImage )
              memcpy( work, prevImage, screenD.width * screenD.height * sizeof(uint32) );
            break;
          }
        //��������� Frame
        image->mFrames[ (image->mFrameCount - 1) * image->mFrameSize ] = time;
        //����������� ��� ��������� ��������
        time = 0;
        transparent = false;
        //������� ��������
        prevImage = imgData + ((image->mFrameCount - 1) * screenD.width * screenD.height);
        }
        continue;
      case 0x3b : break;
      }
    break;
    }
  if( work ) delete work;
  return true;
  }

bool
CmtImage::LoadGIF( CmtFile &is ) {
  static GIFReader reader( true );
  return reader.Load( is, this );
  }

//===============================================================================
//----------------- �������� �� ����� ���� JPEG ---------------------------------
// Used markers:
#define SOI 0xD8
#define EOI 0xD9
#define APP0 0xE0
//#define APP1 0xE1
#define SOF 0xC0
#define DQT 0xDB
#define DHT 0xC4
#define SOS 0xDA
#define DRI 0xDD
#define COM 0xFE

struct Huffman_table {
  uint8  Length[17];  // k =1-16 ; L[k] indicates the number of Huffman codes of length k
  uint16 minor_code[17];  // indicates the value of the smallest Huffman code of length k
  uint16 major_code[17];  // similar, but the highest code
  uint8  V[65536];  // V[k][j] = Value associated to the j-th Huffman code of length k
	// High nibble = nr of previous 0 coefficients
	// Low nibble = size (in bits) of the coefficient which will be taken from the data stream
  };

struct JPEGreader {
  uint8  *buf; // the buffer we use for storing the entire JPG file
  uint8   bp; //current byte
  uint16  wp; //current word

  uint32  byte_pos; // current byte position
  #define BYTE_p(i) bp=buf[(i)++]
  #define WORD_p(i) wp=(((uint16)(buf[(i)]))<<8) + buf[(i)+1]; (i)+=2

  // WORD X_image_size,Y_image_size; // X,Y sizes of the image
  uint16  X_round,Y_round; // The dimensions rounded to multiple of Hmax*8 (X_round)
			  // and Ymax*8 (Y_round)

  uint32  X_image_bytes; // size in bytes of 1 line of the image = X_round * 4
  uint32  y_inc_value ; // 32*X_round; // used by decode_MCU_1x2,2x1,2x2

  uint8   YH,YV,CbH,CbV,CrH,CrV; // sampling factors (horizontal and vertical) for Y,Cb,Cr
  uint16  Hmax,Vmax;
  float   QT[4][64]; // quantization tables, no more than 4 quantization tables (QT[0..3])
  Huffman_table HTDC[4]; //DC huffman tables , no more than 4 (0..3)
  Huffman_table HTAC[4]; //AC huffman tables                  (0..3)

  uint8   YQ_nr,CbQ_nr,CrQ_nr; // quantization table number for Y, Cb, Cr
  uint8   YDC_nr,CbDC_nr,CrDC_nr; // DC Huffman table number for Y,Cb, Cr
  uint8   YAC_nr,CbAC_nr,CrAC_nr; // AC Huffman table number for Y,Cb, Cr

  uint8   Restart_markers; // if 1 => Restart markers on , 0 => no restart markers
  uint16  MCU_restart; //Restart markers appears every MCU_restart MCU blocks

  int16   DCY, DCCb, DCCr; // Coeficientii DC pentru Y,Cb,Cr
  int16   DCT_coeff[64]; // Current DCT_coefficients
  uint8   Y[64],Cb[64],Cr[64]; // Y, Cb, Cr of the current 8x8 block for the 1x1 case
  uint8   Y_1[64],Y_2[64],Y_3[64],Y_4[64];
  uint8   tab_1[64],tab_2[64],tab_3[64],tab_4[64]; // tabelele de supraesantionare pt cele 4 blocuri

  int16   Cr_tab[256],Cb_tab[256]; // Precalculated Cr, Cb tables
  int16   Cr_Cb_green_tab[65536];

// Initial conditions:
// byte_pos = start position in the Huffman coded segment
// WORD_get(w1); WORD_get(w2);wordval=w1;

  uint8   d_k;   // Bit displacement in memory, relative to the offset of w1
			 // it's always <16
  uint16  w1,w2; // w1 = First word in memory; w2 = Second word
  uint32  wordval ; // the actual used value in Huffman decoding.
  uint32  mask[17];
  //uint32  start_neg_pow2;

  uint8  *rlimit_table;

  JPEGreader();
  ~JPEGreader();

  void    skipKbits( int k );
  int16   getKbits( int k );
  void    load_quant_table( float *quant_table );
  void    load_Huffman_table( Huffman_table *HT );
  void    process_Huffman_data_unit( int DC_nr, int AC_nr, int16 *previous_DC );
  void    convert_8x8_YCbCr_to_RGB(uint8 *Y, uint8 *Cb, uint8 *Cr, uint32 im_loc, uint32 X_image_bytes, uint8 *im_buffer );
  //void    convert_8x8_YCbCr_to_RGB_tab(BYTE *Y, BYTE *Cb, BYTE *Cr, BYTE *tab, DWORD im_loc, DWORD X_image_bytes, BYTE *im_buffer);
  void    convert_8x8_YCbCr_to_RGB_tab(uint8 *Y, uint8 *Cb, uint8 *Cr, uint8 *tab, uint32 im_loc, uint32 X_image_bytes, uint8 *im_buffer );
  void    calculate_tabs();
  int     load_JPEG_header( CmtFile &is, CmtPoint &imgSize );
  void    resync();
  void    decode_MCU_1x1( uint32 im_loc, uint32 *image );
  void    decode_MCU_2x1( uint32 im_loc, uint32 *image );
  void    decode_MCU_2x2( uint32 im_loc, uint32 *image );
  void    decode_MCU_1x2( uint32 im_loc, uint32 *image );
  void    decode_JPEG_image( uint32 *image );
  int     get_JPEG_buffer(uint16 X_image, uint16 Y_image, uint8 **address_dest_buffer);
  };

static const uint8 zigzag[64]={ 0, 1, 5, 6,14,15,27,28,
				  2, 4, 7,13,16,26,29,42,
				  3, 8,12,17,25,30,41,43,
				  9,11,18,24,31,40,44,53,
				 10,19,23,32,39,45,52,54,
				 20,22,33,38,46,51,55,60,
				 21,34,37,47,50,56,59,61,
				 35,36,48,49,57,58,62,63 };

//static const int16 neg_pow2[17] = { 0,-1,-3,-7,-15,-31,-63,-127,-255,-511,-1023,-2047,-4095,-8191,-16383,-32767};

typedef void (*decode_MCU_func)(uint32);


#define RIGHT_SHIFT(x,shft)  \
	((shift_temp = (x)) < 0 ? \
	 (shift_temp >> (shft)) | ((~(0L)) << (32-(shft))) : \
	 (shift_temp >> (shft)))

#define DESCALE(x,n)  RIGHT_SHIFT((x) + (1L << ((n)-1)), n)

#define RANGE_MASK 1023L

inline uint16 lookKbits( int k, uint32 val ) { return val >> (16 - k); }

inline uint16 WORD_hi_lo( uint16 hi, uint16 lo ) { return (hi << 8) | lo; }

inline int16 get_svalue( int k, uint16 val ) {
  k = 16 - k;
  if( val & 0x8000 ) return (val >> k);
  //int16 res = (int16)(~val);
  val = ~val;
  val >>= k;
  return - val;
  }

void
JPEGreader::skipKbits( int k ) {
  uint8 b_high, b_low;
  d_k += k;
  if( d_k >= 16 ) {
    d_k -= 16;
		w1 = w2;
		// Get the next word in w2
		BYTE_p(byte_pos);
		if( bp != 0xFF ) b_high=bp;
		else {
			if( buf[byte_pos] == 0 ) byte_pos++; //skip 00
			else byte_pos--; // stop byte_pos pe restart marker
			b_high = 0xFF;
		  }
		BYTE_p(byte_pos);
		if( bp != 0xFF ) b_low=bp;
		else {
			if( buf[byte_pos] == 0 ) byte_pos++; //skip 00
			else byte_pos--; // stop byte_pos pe restart marker
			b_low = 0xFF;
		  }
		w2 = WORD_hi_lo( b_high, b_low );
    }

  wordval = ((uint32)(w1)<<16) + w2;
  wordval <<= d_k;
  wordval >>= 16;
  }

int16
JPEGreader::getKbits( int k ) {
  int16 signed_wordvalue;
  signed_wordvalue = get_svalue( k, wordval );
  skipKbits(k);
  return signed_wordvalue;
  }


void
JPEGreader::load_quant_table( float *quant_table ) {
  float scalefactor[8] = {1.0f, 1.387039845f, 1.306562965f, 1.175875602f,
					   1.0f, 0.785694958f, 0.541196100f, 0.275899379f};
  int j,row,col;
  // Load quantization coefficients from JPG file, scale them for DCT and reorder
  // from zig-zag order
  for( j = 0; j <= 63; j++ )
    quant_table[j] = buf[ byte_pos + zigzag[j] ];
  j = 0;
  for( row = 0; row <= 7; row++ )
    for( col = 0; col <= 7; col++ ) {
		  quant_table[j] *= scalefactor[row] * scalefactor[col];
		  j++;
      }
  byte_pos += 64;
  }

void
JPEGreader::load_Huffman_table(Huffman_table *HT) {
  int k,j;
  uint32 code;

  for( j = 1; j <= 16; j++ ) {
	  BYTE_p(byte_pos);
	  HT->Length[j] = bp;
    }
  for( k = 1; k <= 16; k++ )
	  for( j = 0; j < HT->Length[k]; j++ ) {
		  BYTE_p(byte_pos);
		  HT->V[ WORD_hi_lo(k,j) ] = bp;
	    }

  code = 0;
  for( k = 1; k <= 16; k++ ) {
	 HT->minor_code[k] = (uint16)code;
	 for( j=1; j <= HT->Length[k]; j++ )
     code++;
	 HT->major_code[k] = (uint16)(code-1);
	 code *= 2;
	 if( HT->Length[k] == 0 ) {
		 HT->minor_code[k] = 0xFFFF;
		 HT->major_code[k] = 0;
	   }
   }
 }

void
JPEGreader::process_Huffman_data_unit( int DC_nr, int AC_nr, int16 *previous_DC ) {
  // Process one data unit. A data unit = 64 DCT coefficients
  // Data is decompressed by Huffman decoding, then the array is dezigzag-ed
  // The result is a 64 DCT coefficients array: DCT_coeff
  int nr, k, j, EOB_found;
  register uint16 tmp_Hcode;
  int size_val, count_0;
  uint16 *min_code,*maj_code; // min_code[k]=minimum code of length k, maj_code[k]=similar but maximum
  uint16 *max_val, *min_val;
  uint8  *huff_values;
  int16   DCT_tcoeff[64];
  uint8   byte_temp;

  // Start Huffman decoding
  // First the DC coefficient decoding
  min_code = HTDC[DC_nr].minor_code;
  maj_code = HTDC[DC_nr].major_code;
  huff_values = HTDC[DC_nr].V;

  for( nr = 0; nr < 64 ; nr++)
    DCT_tcoeff[nr] = 0; //Initialize DCT_tcoeff

  nr = 0;// DC coefficient

  min_val = &min_code[1];
  max_val = &maj_code[1];
  for( k = 1; k <= 16; k++ ) {
	  tmp_Hcode = lookKbits( k, wordval );
    //max_val = &maj_code[k]; min_val = &min_code[k];
	  if( (tmp_Hcode <= *max_val) && (tmp_Hcode >= *min_val) ) { //Found a valid Huffman code
	    skipKbits(k);
	    size_val = huff_values[ WORD_hi_lo( k, tmp_Hcode-*min_val) ];
	    if( size_val == 0 )
        DCT_tcoeff[0] = *previous_DC;
	    else {
        DCT_tcoeff[0] = *previous_DC + getKbits( size_val );
	      *previous_DC = DCT_tcoeff[0];
        }
      break;
      }
    min_val++;
    max_val++;
    }

  // Second, AC coefficient decoding
  min_code = HTAC[AC_nr].minor_code;
  maj_code = HTAC[AC_nr].major_code;
  huff_values = HTAC[AC_nr].V;

  nr = 1; // AC coefficient
  EOB_found = 0;
  while( (nr<=63) && (!EOB_found) ) {
    max_val = &maj_code[1];
    min_val =&min_code[1];
    for( k = 1; k <= 16; k++ ) {
      tmp_Hcode = lookKbits( k, wordval );
      //max_val = &maj_code[k]; &min_val = min_code[k];
      if( (tmp_Hcode <= *max_val) && (tmp_Hcode >= *min_val) ) {
        skipKbits(k);
        byte_temp = huff_values[ WORD_hi_lo( k, tmp_Hcode-*min_val )  ];
        size_val = byte_temp & 0xF;
        count_0 = byte_temp >> 4;
        if( size_val == 0 ) {
          if( count_0 == 0 ) EOB_found=1;
          else if( count_0 == 0xF ) nr += 16; //skip 16 zeroes
          }
        else {
          nr += count_0; //skip count_0 zeroes
          DCT_tcoeff[nr++] = getKbits(size_val);
          }
        break;
        }
      min_val++;
      max_val++;
      }
    if( k > 16) nr++;  // This should not occur
    }
  for( j = 0; j <= 63; j++ )
    DCT_coeff[j] = DCT_tcoeff[ zigzag[j] ]; // Et, voila ... DCT_coeff
  }

void
IDCT_transform( int16 *incoeff, uint8 *outcoeff, float *quantptr, uint8 *rlimit_table ) {
  // Fast float IDCT transform
 int    shift_temp;
 int16 *inptr;
 uint8 *outptr;
 float workspace[64];
 float *wsptr;//Workspace pointer
 float dcval;
 float tmp0,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7;
 float tmp10,tmp11,tmp12,tmp13;
 float z5,z10,z11,z12,z13;
 uint8 *range_limit = rlimit_table + 128;
 // Pass 1: process COLUMNS from input and store into work array.
 wsptr = workspace;
 inptr = incoeff;
 //quantptr = QT[Q_nr];
 for( int y = 0; y <= 7; y++ ) {
   if( (inptr[8]|inptr[16]|inptr[24]|inptr[32]|inptr[40]|inptr[48]|inptr[56])==0) {
     // AC terms all zero (in a column)
     dcval=inptr[0]*quantptr[0];
     wsptr[0]  = dcval;
     wsptr[8]  = dcval;
     wsptr[16] = dcval;
     wsptr[24] = dcval;
     wsptr[32] = dcval;
     wsptr[40] = dcval;
     wsptr[48] = dcval;
     wsptr[56] = dcval;
     inptr++;quantptr++;wsptr++;//advance pointers to next column
     continue ;
     }
  //Even part
	tmp0 = inptr[0] *quantptr[0];
	tmp1 = inptr[16]*quantptr[16];
	tmp2 = inptr[32]*quantptr[32];
	tmp3 = inptr[48]*quantptr[48];

	tmp10 = tmp0 + tmp2;// phase 3
	tmp11 = tmp0 - tmp2;

	tmp13 = tmp1 + tmp3;// phases 5-3
	tmp12 = (tmp1 - tmp3) * 1.414213562f - tmp13; // 2*c4

	tmp0 = tmp10 + tmp13;// phase 2
	tmp3 = tmp10 - tmp13;
	tmp1 = tmp11 + tmp12;
	tmp2 = tmp11 - tmp12;

	// Odd part
	tmp4 = inptr[8] *quantptr[8];
	tmp5 = inptr[24]*quantptr[24];
	tmp6 = inptr[40]*quantptr[40];
	tmp7 = inptr[56]*quantptr[56];

	z13 = tmp6 + tmp5;// phase 6
	z10 = tmp6 - tmp5;
	z11 = tmp4 + tmp7;
	z12 = tmp4 - tmp7;

	tmp7 = z11 + z13;// phase 5
	tmp11= (z11 - z13) * 1.414213562f; // 2*c4

	z5 = (z10 + z12) * 1.847759065f; // 2*c2
	tmp10 = 1.082392200f * z12 - z5; // 2*(c2-c6)
	tmp12 = -2.613125930f * z10 + z5;// -2*(c2+c6)

	tmp6 = tmp12 - tmp7;// phase 2
	tmp5 = tmp11 - tmp6;
	tmp4 = tmp10 + tmp5;

	wsptr[0]  = tmp0 + tmp7;
	wsptr[56] = tmp0 - tmp7;
	wsptr[8]  = tmp1 + tmp6;
	wsptr[48] = tmp1 - tmp6;
	wsptr[16] = tmp2 + tmp5;
	wsptr[40] = tmp2 - tmp5;
	wsptr[32] = tmp3 + tmp4;
	wsptr[24] = tmp3 - tmp4;
	inptr++;
	quantptr++;
	wsptr++;//advance pointers to the next column
  }

 //  Pass 2: process ROWS from work array, store into output array.
 // Note that we must descale the results by a factor of 8 = 2^3
 outptr=outcoeff;
 wsptr=workspace;
 for( int x = 0; x <= 7; x++) {
   // Even part
	tmp10 = wsptr[0] + wsptr[4];
	tmp11 = wsptr[0] - wsptr[4];

	tmp13 = wsptr[2] + wsptr[6];
	tmp12 =(wsptr[2] - wsptr[6]) * 1.414213562f - tmp13;

	tmp0 = tmp10 + tmp13;
	tmp3 = tmp10 - tmp13;
	tmp1 = tmp11 + tmp12;
	tmp2 = tmp11 - tmp12;

   // Odd part
	z13 = wsptr[5] + wsptr[3];
	z10 = wsptr[5] - wsptr[3];
	z11 = wsptr[1] + wsptr[7];
	z12 = wsptr[1] - wsptr[7];

	tmp7 = z11 + z13;
	tmp11= (z11 - z13) * 1.414213562f;

	z5 = (z10 + z12) * 1.847759065f; // 2*c2
	tmp10 = 1.082392200f * z12 - z5;  // 2*(c2-c6)
	tmp12 = -2.613125930f * z10 + z5; // -2*(c2+c6)

	tmp6 = tmp12 - tmp7;
	tmp5 = tmp11 - tmp6;
	tmp4 = tmp10 + tmp5;

 // Final output stage: scale down by a factor of 8
	outptr[0] = range_limit[(DESCALE((int) (tmp0 + tmp7), 3)) & 1023L];
	outptr[7] = range_limit[(DESCALE((int) (tmp0 - tmp7), 3)) & 1023L];
	outptr[1] = range_limit[(DESCALE((int) (tmp1 + tmp6), 3)) & 1023L];
	outptr[6] = range_limit[(DESCALE((int) (tmp1 - tmp6), 3)) & 1023L];
	outptr[2] = range_limit[(DESCALE((int) (tmp2 + tmp5), 3)) & 1023L];
	outptr[5] = range_limit[(DESCALE((int) (tmp2 - tmp5), 3)) & 1023L];
	outptr[4] = range_limit[(DESCALE((int) (tmp3 + tmp4), 3)) & 1023L];
	outptr[3] = range_limit[(DESCALE((int) (tmp3 - tmp4), 3)) & 1023L];

	wsptr+=8;//advance pointer to the next row
	outptr+=8;
  }
}


void
JPEGreader::convert_8x8_YCbCr_to_RGB(uint8 *Y, uint8 *Cb, uint8 *Cr, uint32 im_loc, uint32 X_image_bytes, uint8 *im_buffer )
// Functia (ca optimizare) poate fi apelata si fara parametrii Y,Cb,Cr
// Stim ca va fi apelata doar in cazul 1x1
{
  uint32 x,y;
  uint8 im_nr;
  uint8 *Y_val = Y, *Cb_val = Cb, *Cr_val = Cr;
  uint8 *ibuffer = im_buffer + im_loc;

  for (y=0;y<8;y++)
   {
	im_nr=0;
	for (x=0;x<8;x++)
	  {
	   ibuffer[im_nr++] = rlimit_table[*Y_val + Cb_tab[*Cb_val]]; //B
	   ibuffer[im_nr++] = rlimit_table[*Y_val + Cr_Cb_green_tab[WORD_hi_lo(*Cr_val,*Cb_val)]]; //G
	   ibuffer[im_nr++] = rlimit_table[*Y_val + Cr_tab[*Cr_val]]; // R
/*
// Monochrome display
	   im_buffer[im_nr++] = *Y_val;
	   im_buffer[im_nr++] = *Y_val;
	   im_buffer[im_nr++] = *Y_val;
*/
	   Y_val++; Cb_val++; Cr_val++;
     ibuffer[im_nr++] = 0xff; //������������ �������
	  }
	ibuffer+=X_image_bytes;
   }
}

void
JPEGreader::convert_8x8_YCbCr_to_RGB_tab(uint8 *Y, uint8 *Cb, uint8 *Cr, uint8 *tab, uint32 im_loc, uint32 X_image_bytes, uint8 *im_buffer ) {
// Functia (ca optimizare) poate fi apelata si fara parametrii Cb,Cr
  uint32 x,y;
  int nr, im_nr;
  uint8 Y_val, Cb_val, Cr_val;
  uint8 *ibuffer = im_buffer + im_loc;

  nr=0;
  for( y = 0; y < 8; y++ ) {
    im_nr=0;
    for( x = 0; x < 8; x++ ) {
      Y_val = Y[nr];
      Cb_val = Cb[ tab[nr] ];
      Cr_val = Cr[ tab[nr] ]; // reindexare folosind tabelul
      // de supraesantionare precalculat
      ibuffer[im_nr++] = rlimit_table[Y_val + Cb_tab[Cb_val]]; //B
      ibuffer[im_nr++] = rlimit_table[Y_val + Cr_Cb_green_tab[WORD_hi_lo(Cr_val,Cb_val)]]; //G
      ibuffer[im_nr++] = rlimit_table[Y_val + Cr_tab[Cr_val]]; // R
      nr++;
      ibuffer[im_nr++] = 0xff; //������������ �������
      }
    ibuffer += X_image_bytes;
    }
  }

void
JPEGreader::calculate_tabs() {
 uint8 tab_temp[256];
 uint8 x,y;

 // Tabelul de supraesantionare 16x16
 for (y=0;y<16;y++)
	 for (x=0;x<16;x++)
	   tab_temp[y*16+x] = (y/YV)* 8 + x/YH;

 // Din el derivam tabelele corespunzatoare celor 4 blocuri de 8x8 pixeli
 for (y=0;y<8;y++)
	{
	 for (x=0;x<8;x++)
	  tab_1[y*8+x]=tab_temp[y*16+x];
	 for (x=8;x<16;x++)
	  tab_2[y*8+(x-8)]=tab_temp[y*16+x];
	}
 for (y=8;y<16;y++)
	{
	 for (x=0;x<8;x++)
	  tab_3[(y-8)*8+x]=tab_temp[y*16+x];
	 for (x=8;x<16;x++)
	  tab_4[(y-8)*8+(x-8)]=tab_temp[y*16+x];
	}
}



int
JPEGreader::load_JPEG_header( CmtFile &is, CmtPoint &imgSize ) {
  //WORD Xdensity,Ydensity,Xthumbnail,Ythumbnail;
  uint32 length_of_file = is.Size();
  int    length;
  float *qtable;
  uint32 old_byte_pos;
  Huffman_table *htable;
  uint32 j;
  //BYTE precision,comp_id,nr_components;
  //BYTE QT_info,HT_info;

  bool SOS_found = false, SOF_found = false;

  buf = new uint8[ length_of_file + 4 ];
  if( buf == 0 ) return 0;
  is.Read( buf, is.Size() );

  if( (buf[0] != 0xFF) || (buf[1] != SOI) ) return 0;
  if( (buf[2] != 0xFF) || (buf[3] != APP0) ) return 0; //exit_func("Invalid JPG file.");
  if( (buf[6] != 'J') || (buf[7] != 'F') || (buf[8] != 'I') || (buf[9] != 'F') ||
	  (buf[10] != 0) ) return 0; //exit_func("Invalid JPG file.");

  byte_pos = 11;

  BYTE_p(byte_pos);
  if( bp != 1 ) //������
    return 0; //exit_func("JFIF version not supported");
  BYTE_p(byte_pos); // vers_lo=bp;
  BYTE_p(byte_pos);
  //units if( bp != 0 ) //exit_func("JPG format not supported");
  WORD_p(byte_pos);
  //Xdensity = wp;
  WORD_p(byte_pos);
  //Ydensity = wp;
  //if ((Xdensity!=1)||(Ydensity!=1)) //exit_func("JPG format not supported");
	// ;  //{printf("X density = %d\n",Xdensity); printf("Y density = %d\n",Ydensity);}
  BYTE_p(byte_pos);
  if( bp != 0 ) return 0;
  //Xthumbnail=bp;
  BYTE_p(byte_pos);
  if( bp != 0 ) return 0;
  //Ythumbnail=bp;
  //if ((Xthumbnail!=0)||(Ythumbnail!=0))
	//return 0; //exit_func(" Cannot process JFIF thumbnailed files\n");

  // Start decoding process
  Restart_markers = 0;
  while( (byte_pos < length_of_file) && !SOS_found ) {
    BYTE_p(byte_pos);
    if(bp != 0xFF) continue;
    // A marker was found
    BYTE_p(byte_pos);
    switch(bp) {
      case DQT:
        WORD_p(byte_pos);
        length = wp; // length of the DQT marker
			  for( j = 0; j < (uint32)(wp-2); ) {
          old_byte_pos = byte_pos;
          BYTE_p(byte_pos);
          //QT_info = bp;
          if( (bp >> 4) != 0)
            return 0; //exit_func("16 bit quantization table not supported");
          qtable = QT[bp & 0xF];
          load_quant_table( qtable );
          j += byte_pos - old_byte_pos;
          }
        break;
      case DHT:
        WORD_p(byte_pos);
        length = wp;
        for( j = 0; j < (uint32)(wp-2); ) {
          old_byte_pos = byte_pos;
          BYTE_p(byte_pos);
          //HT_info = bp;
          if( (bp & 0x10) != 0) htable = &HTAC[ bp & 0xF ];
          else htable = &HTDC[ bp & 0xF];
          load_Huffman_table( htable );
          j += byte_pos - old_byte_pos;
          }
        break;
      case COM:
        WORD_p(byte_pos);
        length = wp;
        byte_pos += wp - 2;
        break;
      case DRI:
        Restart_markers = 1;
        WORD_p(byte_pos);
        length = wp; //should be = 4
        WORD_p(byte_pos);
        MCU_restart = wp;
        if( MCU_restart == 0 )
          Restart_markers = 0;
        break;
      case SOF:
        WORD_p(byte_pos);
        length = wp; //should be = 8+3*3=17
        BYTE_p(byte_pos);
        //precision = bp;
        if( bp != 8 )
          return 0; //exit_func("Only 8 bit precision supported");
        WORD_p(byte_pos);
        imgSize.y = wp;
        WORD_p(byte_pos);
        imgSize.x = wp;
		    BYTE_p(byte_pos);
        //nr_components = bp;
        if( bp != 3 )
          return 0; //exit_func("Only truecolor JPGS supported");
        for( j = 1; j <= 3; j++ ) {
          BYTE_p(byte_pos);
          //comp_id=bp;
          if( (bp == 0) || (bp > 3) )
            return 0; //exit_func("Only YCbCr format supported");
				  switch( bp ) {
            case 1: // Y
							BYTE_p(byte_pos);
              YH = bp >> 4;
              YV = bp & 0xF;
							BYTE_p(byte_pos);
              YQ_nr = bp;
							break;
            case 2: // Cb
							BYTE_p(byte_pos);
              CbH = bp >> 4;
              CbV = bp & 0xF;
							BYTE_p(byte_pos);
              CbQ_nr = bp;
							break;
            case 3: // Cr
							BYTE_p(byte_pos);
              CrH = bp >> 4;
              CrV = bp & 0xF;
							BYTE_p(byte_pos);
              CrQ_nr = bp;
							break;
            }
          }
        SOF_found = true;
        break;
      case SOS:
        WORD_p(byte_pos);
        length = wp; //should be = 6+3*2=12
        BYTE_p(byte_pos);
        //nr_components=bp;
        if( bp != 3 )
          return 0; //exit_func("Invalid SOS marker");
        for( j = 1; j <= 3; j++ ) {
          BYTE_p(byte_pos);
          //comp_id=bp;
          if( (bp == 0) || (bp > 3) )
            return 0; //exit_func("Only YCbCr format supported");
				  switch( bp ) {
            case 1: // Y
              BYTE_p(byte_pos);
              YDC_nr = bp >> 4;
              YAC_nr = bp & 0xF;
              break;
            case 2: // Cb
              BYTE_p(byte_pos);
              CbDC_nr = bp >> 4;
              CbAC_nr = bp & 0xF;
              break;
            case 3: // Cr
              BYTE_p(byte_pos);
              CrDC_nr = bp >> 4;
              CrAC_nr = bp & 0xF;
              break;
            }
          }
        BYTE_p(byte_pos);
        BYTE_p(byte_pos);
        BYTE_p(byte_pos); // Skip 3 bytes
        SOS_found = true;
        break;
      case 0xFF:
        break; // do nothing for 0xFFFF, sequence of consecutive 0xFF are for
               // filling purposes and should be ignored
      default:
        WORD_p(byte_pos);
        length = wp;
        byte_pos += wp - 2; //skip unknown marker
        break;
      }
    }
  if( !SOS_found )
    return 0; //exit_func("Invalid JPG file. No SOS marker found.");
  if( !SOF_found )
    return 0; //exit_func("Progressive JPEGs not supported");

  if( (CbH > YH) || (CrH > YH) )
    return 0; //exit_func("Vertical sampling factor for Y should be >= sampling factor for Cb,Cr");
  if( (CbV > YV) || (CrV > YV) )
    return 0; //exit_func("Horizontal sampling factor for Y should be >= sampling factor for Cb,Cr");

  if( (CbH >= 2) || (CbV >= 2) )
    return 0; //exit_func("Cb sampling factors should be = 1");
  if( (CrV >= 2) || (CrV >= 2) )
    return 0; //exit_func("Cr sampling factors should be = 1");

  // Restricting sampling factors for Y,Cb,Cr should give us 4 possible cases for sampling factors
  // YHxYV,CbHxCbV,CrHxCrV: 2x2,1x1,1x1;  1x2,1x1,1x1; 2x1,1x1,1x1;
  // and 1x1,1x1,1x1 = no upsampling needed

  Hmax = YH;
  Vmax = YV;
  if( imgSize.x % (Hmax*8) == 0) X_round = imgSize.x; // X_round = Multiple of Hmax*8
  else X_round = (imgSize.x / (Hmax * 8)+1) * (Hmax * 8);
  if( imgSize.y % (Vmax*8) == 0) Y_round = imgSize.y; // Y_round = Multiple of Vmax*8
  else Y_round = (imgSize.y / (Vmax * 8)+1) * (Vmax * 8);

  return 1;
  }

void
JPEGreader::resync() {
  // byte_pos  = pozitionat pe restart marker
  byte_pos += 2;
  BYTE_p(byte_pos);
  if( bp == 0xFF ) byte_pos++; // skip 00
  w1 = WORD_hi_lo(bp, 0);
  BYTE_p(byte_pos);
  if( bp == 0xFF ) byte_pos++; // skip 00
  w1 += bp;
  BYTE_p(byte_pos);
  if( bp == 0xFF ) byte_pos++; // skip 00
  w2 = WORD_hi_lo(bp, 0);
  BYTE_p(byte_pos);
  if( bp == 0xFF ) byte_pos++; // skip 00
  w2 += bp;
  wordval = w1;
  d_k = 0; // Reinit bitstream decoding
  DCY=0; DCCb=0; DCCr=0; // Init DC coefficients
  }

void
JPEGreader::decode_MCU_1x1( uint32 im_loc, uint32 *image ) {
  // Y
  process_Huffman_data_unit(YDC_nr,YAC_nr,&DCY);
  IDCT_transform( DCT_coeff, Y, QT[YQ_nr], rlimit_table );
  // Cb
  process_Huffman_data_unit(CbDC_nr,CbAC_nr,&DCCb);
  IDCT_transform( DCT_coeff, Cb, QT[CbQ_nr], rlimit_table );
  // Cr
  process_Huffman_data_unit(CrDC_nr,CrAC_nr,&DCCr);
  IDCT_transform( DCT_coeff, Cr, QT[CrQ_nr], rlimit_table );

  convert_8x8_YCbCr_to_RGB( Y, Cb, Cr, im_loc, X_image_bytes, (uint8*)image );
  }

void
JPEGreader::decode_MCU_2x1( uint32 im_loc, uint32 *image ) {
  // Y
  process_Huffman_data_unit(YDC_nr,YAC_nr,&DCY);
  IDCT_transform( DCT_coeff, Y_1, QT[YQ_nr], rlimit_table );
  process_Huffman_data_unit(YDC_nr,YAC_nr,&DCY);
  IDCT_transform( DCT_coeff, Y_2, QT[YQ_nr], rlimit_table );
  // Cb
  process_Huffman_data_unit(CbDC_nr,CbAC_nr,&DCCb);
  IDCT_transform( DCT_coeff, Cb, QT[CbQ_nr], rlimit_table );
  // Cr
  process_Huffman_data_unit(CrDC_nr,CrAC_nr,&DCCr);
  IDCT_transform( DCT_coeff, Cr, QT[CrQ_nr], rlimit_table );

  convert_8x8_YCbCr_to_RGB_tab(Y_1,Cb,Cr,tab_1,im_loc,X_image_bytes, (uint8*)image );
  convert_8x8_YCbCr_to_RGB_tab(Y_2,Cb,Cr,tab_2,im_loc+32,X_image_bytes, (uint8*)image );
  }

void
JPEGreader::decode_MCU_2x2( uint32 im_loc, uint32 *image ) {
  // Y
  process_Huffman_data_unit(YDC_nr,YAC_nr,&DCY);
  IDCT_transform( DCT_coeff, Y_1, QT[YQ_nr], rlimit_table );
  process_Huffman_data_unit(YDC_nr,YAC_nr,&DCY);
  IDCT_transform( DCT_coeff, Y_2, QT[YQ_nr], rlimit_table );
  process_Huffman_data_unit(YDC_nr,YAC_nr,&DCY);
  IDCT_transform( DCT_coeff, Y_3, QT[YQ_nr], rlimit_table );
  process_Huffman_data_unit(YDC_nr,YAC_nr,&DCY);
  IDCT_transform( DCT_coeff, Y_4, QT[YQ_nr], rlimit_table );
  // Cb
  process_Huffman_data_unit(CbDC_nr,CbAC_nr,&DCCb);
  IDCT_transform( DCT_coeff, Cb, QT[CbQ_nr], rlimit_table );
  // Cr
  process_Huffman_data_unit(CrDC_nr,CrAC_nr,&DCCr);
  IDCT_transform( DCT_coeff, Cr, QT[CrQ_nr], rlimit_table );

  convert_8x8_YCbCr_to_RGB_tab(Y_1,Cb,Cr,tab_1,im_loc,X_image_bytes, (uint8*)image );
  convert_8x8_YCbCr_to_RGB_tab(Y_2,Cb,Cr,tab_2,im_loc+32,X_image_bytes, (uint8*)image );
  convert_8x8_YCbCr_to_RGB_tab(Y_3,Cb,Cr,tab_3,im_loc+y_inc_value,X_image_bytes, (uint8*)image );
  convert_8x8_YCbCr_to_RGB_tab(Y_4,Cb,Cr,tab_4,im_loc+y_inc_value+32,X_image_bytes, (uint8*)image );
  }

void
JPEGreader::decode_MCU_1x2( uint32 im_loc, uint32 *image ) {
  // Y
  process_Huffman_data_unit(YDC_nr,YAC_nr,&DCY);
  IDCT_transform( DCT_coeff, Y_1, QT[YQ_nr], rlimit_table );
  process_Huffman_data_unit(YDC_nr,YAC_nr,&DCY);
  IDCT_transform( DCT_coeff, Y_2, QT[YQ_nr], rlimit_table );
  // Cb
  process_Huffman_data_unit(CbDC_nr,CbAC_nr,&DCCb);
  IDCT_transform( DCT_coeff, Cb, QT[CbQ_nr], rlimit_table );
  // Cr
  process_Huffman_data_unit(CrDC_nr,CrAC_nr,&DCCr);
  IDCT_transform( DCT_coeff, Cr, QT[CrQ_nr], rlimit_table );

  convert_8x8_YCbCr_to_RGB_tab(Y_1,Cb,Cr,tab_1,im_loc,X_image_bytes, (uint8*)image );
  convert_8x8_YCbCr_to_RGB_tab(Y_2,Cb,Cr,tab_3,im_loc+y_inc_value,X_image_bytes, (uint8*)image );
  }

void
JPEGreader::decode_JPEG_image( uint32 *image ) {
  int x_mcu_cnt,y_mcu_cnt;
  int nr_mcu;
  int X_MCU_nr, Y_MCU_nr; // Nr de MCU-uri
  int MCU_dim_x; //dimensiunea in bufferul imagine a unui MCU pe axa x
  int im_loc_inc; // = 7 * X_round * 4 sau 15*X_round*4;
  int im_loc; //locatia in bufferul imagine

  byte_pos -= 2;
  resync();

  y_inc_value = 32 * X_round;
  calculate_tabs(); // Calcul tabele de supraesantionare, tinand cont de YH si YV

  int decType;

  if ((YH==1)&&(YV==1)) decType = 0;
  else {
	  if( YH == 2 ) {
		  if( YV == 2 ) decType = 1;
		  else          decType = 2;
      }
    else decType = 3;
    }


  Y_MCU_nr = Y_round / (Vmax * 8); // nr of MCUs on Y axis
  X_MCU_nr = X_round / (Hmax * 8); // nr of MCUs on X axis

  MCU_dim_x = Hmax * 8 * 4;
  X_image_bytes = X_round * 4;
  im_loc_inc = (Vmax*8-1) * X_image_bytes;
  nr_mcu=0;
  im_loc=0; // memory location of the current MCU
  for( y_mcu_cnt = 0; y_mcu_cnt < Y_MCU_nr; y_mcu_cnt++ ) {
    for( x_mcu_cnt = 0; x_mcu_cnt < X_MCU_nr; x_mcu_cnt++ ) {
      switch( decType ) {
        case 0 : decode_MCU_1x1( im_loc, image ); break;
        case 1 : decode_MCU_2x2( im_loc, image ); break;
        case 2 : decode_MCU_2x1( im_loc, image ); break;
        case 3 : decode_MCU_1x2( im_loc, image ); break;
        }
      if( (Restart_markers) && ((nr_mcu + 1) % MCU_restart == 0) )
        resync();
	    nr_mcu++;
	    im_loc += MCU_dim_x;
      }
    im_loc += im_loc_inc;
    }
  }



JPEGreader::JPEGreader() :
  buf(0),
  rlimit_table(0) {
    byte_pos = 0;
    d_k = 0;  // Bit displacement in memory, relative to the offset of w1

    //init_QT
    int i;

    //calculate_mask
    uint32 tmpdv;
    for( i = 0; i <= 16; i++ ) {
      tmpdv = 0x10000;
      mask[i] = (tmpdv >> i)-1; //precalculated bit mask
      }

    /* Allocate and fill in the sample_range_limit table */
    int j;
    rlimit_table = new uint8[ 5 * 256 + 128 ];
    /* First segment of "simple" table: limit[x] = 0 for x < 0 */
    memset((void *)rlimit_table,0,256);
    rlimit_table += 256;	/* allow negative subscripts of simple table */
    /* Main part of "simple" table: limit[x] = x */
    for (j = 0; j < 256; j++) rlimit_table[j] = j;
    /* End of simple table, rest of first half of post-IDCT table */
    for (j = 256; j < 640; j++) rlimit_table[j] = 255;
    /* Second half of post-IDCT table */
    memset((void *)(rlimit_table + 640),0,384);
    for (j = 0; j < 128 ; j++) rlimit_table[j+1024] = j;

    uint16 Cr_v,Cb_v;
    for( i = 0; i <= 255; i++)
      Cr_tab[i]=(int16)((i - 128.0) * 1.402);

    for( i = 0; i <= 255; i++)
      Cb_tab[i]=(int16)((i - 128.0) * 1.772);

    for( Cr_v = 0; Cr_v <= 255; Cr_v++)
      for( Cb_v = 0; Cb_v <= 255; Cb_v++)
        Cr_Cb_green_tab[((uint16)(Cr_v)<<8)+Cb_v]=(int)(-0.34414*(Cb_v-128.0)-0.71414*(Cr_v-128.0));
    }

JPEGreader::~JPEGreader() {
  if( buf ) delete buf;
  if( rlimit_table ) {
    rlimit_table -= 256;
    delete rlimit_table;
    }
  }

bool
CmtImage::LoadJPG( CmtFile &is ) {
  JPEGreader *reader = new JPEGreader();
  if( reader == 0 ) return false;
  CmtPoint ps;
  if( reader->load_JPEG_header( is, ps ) ) {
    Free();
    mSize = ps;
    mDinamic = true; //������ ����������� ����������
    mFrameSize = reader->X_round * reader->Y_round + 1;
    mFrames = new uint32[ mFrameSize ];
    mFrameCount = 1;
    if( mFrames ) {
      mFrames[0] = 0;
      reader->decode_JPEG_image( mFrames + 1 );
      //����������
      if( reader->X_round > mSize.x ) {
        for( int i = 1; i < mSize.y; i++ )
          memcpy( mFrames + 1 + i * mSize.x, mFrames + 1 + i * reader->X_round, sizeof(uint32) * mSize.x );
        }
      }
    }
  delete reader;
  return true;
  }
