//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtContext.h>

CmtContext::CmtContext(CmtPoint size, uint32 *src) {
  //Заполнить базовые поля
  mSize = size;
  mClipSize = mSize;
  if( src == 0 && mSize.x && mSize.y ) {
    //Область не задана, выделить
    mData = new uint32[mSize.x * mSize.y];
    mOwner = true;
    }
  else {
    //Область задана
    mData = src;
    mOwner = false;
    }
  mPos = CmtPoint();
#ifdef CMT_FONT
  mFont = 0;
  mFontDir = 0;
#endif
  }

CmtContext::~CmtContext() {
  if( mOwner )
    delete mData;
  }

void
CmtContext::SetSize(CmtPoint size) {
  if( size.x <= mSize.x && size.y <= mSize.y ) {
    //Новый размер равен или меньше предыдущего, просто устанавливаем новую границу отсечения
    mClipSize = size;
    }
  else {
    if( mOwner && mData) delete mData; 
    mSize = size; 
    mClipSize = size;
    if( size.x && size.y ) {
      mData = new uint32[size.x * size.y]; 
      mOwner = true;
      }
    else {
      mData = 0;
      mOwner = false;
      }
    }
  }

inline void
CmtColorAdd( uint32 &dest, uint32 color ) {
  uint32 prev = dest;
  int level = CMT_A(color);
  if( level == 0 ) return;
  if( level == 256 ) dest = (prev & 0xff000000) | (color & 0xffffff);
  else {
    int plevel = 256 - level;
    int pr = ((CMT_R(prev) * plevel) + (CMT_R(color) * level)) >> 8;
    int pg = ((CMT_G(prev) * plevel) + (CMT_G(color) * level)) >> 8;
    int pb = ((CMT_B(prev) * plevel) + (CMT_B(color) * level)) >> 8;
    prev &= 0xff000000;
    dest = prev | ARGB32(0,pr,pg,pb);
    }
  }

void   
CmtContext::FilledLine( CmtPoint a, CmtPoint size, uint32 *colors, bool horz, bool set ) {
  if( CheckBound( a.x, a.y ) ) {
    if( a.x + size.x >= mClipSize.x ) size.x = mClipSize.x - a.x;
    if( a.y + size.y >= mClipSize.y ) size.y = mClipSize.y - a.y;
    uint32 *ptr = mData + a.x + a.y * mSize.x;
    if( horz ) {
      //По горизонтали
      if( set ) {
        while( size.y ) {
          for( int i = 0; i < size.x; i++ ) ptr[i] = *colors;
          ptr += mSize.x;
          size.y--;
          colors++;
          }
        }
      else {
        while( size.y ) {
          for( int i = 0; i < size.x; i++ ) 
            CmtColorAdd( ptr[i], *colors );
          ptr += mSize.x;
          size.y--;
          colors++;
          }
        }
      }
    else {
      //По вертикали
      if( set ) {
        while( size.y ) {
          for( int i = 0; i < size.x; i++ ) 
            ptr[i] = colors[i];
          ptr += mSize.x;
          size.y--;
          }
        }
      else {
        while( size.y ) {
          for( int i = 0; i < size.x; i++ ) 
            ptr[i] = colors[i];
          ptr += mSize.x;
          size.y--;
          }
        }
      }
    }
  }

void
CmtContext::SetPixel( CmtPoint p, uint32 color, bool set ) {
  if( CheckBound( p.x, p.y ) ) {
    if( set ) {
      mData[p.x + mSize.x * p.y] = color;
      }
    else {
      CmtColorAdd( mData[p.x + mSize.x * p.y], color );
      }
    }
  }

inline uint32
SumPixel( uint32 prev, uint32 color, int level ) {
  int plevel = 256 - level;
  int pr = ((CMT_R(prev) * plevel) + (CMT_R(color) * level)) >> 8;
  int pg = ((CMT_G(prev) * plevel) + (CMT_G(color) * level)) >> 8;
  int pb = ((CMT_B(prev) * plevel) + (CMT_B(color) * level)) >> 8;
  return ARGB32(CMT_A(prev),pr,pg,pb);
  }

void
CmtContext::AddPixel( CmtPoint p, uint32 color, int level ) {
  if( CheckBound( p.x, p.y ) ) {
    uint32 prev = mData[p.x + mSize.x * p.y];
    mData[p.x + mSize.x * p.y] = SumPixel( prev, color, level );
    }
  }

uint32
CmtContext::GetPixel( CmtPoint p ) {
  if( CheckBound( p.x, p.y ) )
    return mData[p.x + mSize.x * p.y];
  return 0;
  }

uint32*
CmtContext::GetData( CmtPoint p ) {
  if( CheckBound( p.x, p.y ) )
    return mData + p.x + mSize.x * p.y;
  return 0;
  }

void
CmtContext::Pop(CmtContextInfo &info) {
  mData     = info.mData;
  mSize     = info.mSize;
  mClipSize = info.mClipSize;
  }

bool
CmtContext::Push(CmtContextInfo &info, CmtPoint pos, CmtPoint size, CmtContext *src) {
  info.mData     = mData;
  info.mSize     = mSize;
  info.mClipSize = mClipSize;
  if( src ) {
    mData     = src->mData;
    mSize     = src->mSize;
    mClipSize = src->mClipSize;
    }
  else {
    mData = GetData( pos );
    if( mData == 0 ) {
      Pop( info );
      return false;
      }
    mSize.y -= pos.y;
    mClipSize.x = mSize.x - pos.x > size.x ? size.x : mSize.x - pos.x;
    mClipSize.y = mSize.y > size.y ? size.y : mSize.y;
    }
  return true;
  }

void
CmtContext::FilledRect( CmtPoint a, CmtPoint b, uint32 color, bool set ) {
  if( CMT_A(color) == 0 && !set ) return; //Цвет полностью прозрачный
  //Ограничения
  Clip( a );
  Clip( b );
  int32 x,y;
  if( a.y > b.y ) {
    //Обмен координат
    y   = a.y;
    a.y = b.y;
    b.y = y;
    }
  if( a.x > b.x ) {
    //Обмен координат
    x = a.x;
    a.x = b.x;
    b.x = x;
    }
  for( y = a.y; y < b.y; y++ ) {
    HorzLine( a.x, b.x - 1, y, color, set );
    }
  }


void   
CmtContext::HorzLine( int32 x1, int32 x2, int32 y, uint32 color, bool set ) {
  if( x2 < 0 || x1 >= mClipSize.x || y < 0 || y >= mClipSize.y ) return; //Линия вне окна
  //Обмен
  if( x2 < x1 ) {
    int32 x = x1;
    x1 = x2;
    x2 = x;
    }
  //Отсечение
  if( x1 < 0 ) x1 = 0;
  if( x2 >= mClipSize.x ) x2 = mClipSize.x - 1;
  //Указатель
  uint32 *ptr = mData + mSize.x * y;
  if( set ) {
    while( x1 <= x2 )
      ptr[x1++] = color;
    }
  else {
    while( x1 <= x2 )
      CmtColorAdd( ptr[x1++], color );
    }
  }

void
CmtContext::VertLine( int32 x, int32 y1, int32 y2, uint32 color, bool set ) {
  if( x < 0 || x >= mClipSize.x || y2 < 0 || y1 >= mClipSize.y ) return; //Линия вне окна
  //Обмен
  if( y2 < y1 ) {
    int32 y = y1;
    y1 = y2;
    y2 = y;
    }
  //Отсечение
  if( y1 < 0 ) y1 = 0;
  if( y2 >= mClipSize.y ) y2 = mClipSize.y - 1;
  //Указатель
  uint32 *ptr = mData + mSize.x * y1 + x;
  if( set ) {
    while( y1 <= y2 ) {
      ptr[0] = color;
      ptr += mSize.x;
      y1++;
      }
    }
  else {
    while( y1 <= y2 ) {
      CmtColorAdd( ptr[0], color );
      ptr += mSize.x;
      y1++;
      }
    }
  }


void   
CmtContext::ThickLineTo( CmtPoint to, uint32 thickness, uint32 color, bool set ) { //Линия заданной толщины
  CmtPoint a( mPos );
  CmtPoint b( to );
  mPos = to;
  int thickDown = (thickness + 1) >> 1;
  int thickUp = thickness >> 1;
  if( ClipLine( a, b ) ) {
    //Пропорциональная линия
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    int sum = 0;
    int stepX = dx >= 0 ? 1 : -1;
    int stepY = dy >= 0 ? 1 : -1;
    if( dx < 0 ) dx = -dx;
    if( dy < 0 ) dy = -dy;
    if( dx > dy ) {
      sum += dy / 2;
      if( set ) {
        for( ; a.x != b.x; a.x += stepX ) {
          mData[a.x + a.y * mSize.x] = color;
          //Вверх
          for( int y = 1; y <= thickUp && a.y - y >= 0; y++ )
            mData[a.x + (a.y - y) * mSize.x] = color;
          //Вниз
          for( int y = 1; y < thickDown && a.y + y < mSize.y; y++ )
            mData[a.x + (a.y + y) * mSize.x] = color;
          sum += dy;
          if( sum >= dx ) {
            sum -= dx;
            a.y += stepY;
            }
          }
        mData[a.x + a.y * mSize.x] = color;
        }
      else {
        for( ; a.x != b.x; a.x += stepX ) {
          CmtColorAdd( mData[a.x + a.y * mSize.x], color );
          sum += dy;
          if( sum >= dx ) {
            sum -= dx;
            a.y += stepY;
            }
          }
        CmtColorAdd( mData[a.x + a.y * mSize.x], color );
        }
      }
    else {
      sum += dx / 2;
      if( set ) {
        for( ; a.y != b.y; a.y += stepY ) {
          mData[a.x + a.y * mSize.x] = color;
          //Влево
          for( int x = 1; x <= thickUp && a.x - x >= 0; x++ )
            mData[a.x - x + a.y * mSize.x] = color;
          //Вправо
          for( int x = 1; x < thickDown && a.x + x < mSize.x; x++ )
            mData[a.x + x + a.y * mSize.x] = color;
          sum += dx;
          if( sum >= dy ) {
            sum -= dy;
            a.x += stepX;
            }
          }
        mData[a.x + a.y * mSize.x] = color;
        }
      else {
        for( ; a.y != b.y; a.y += stepY ) {
          CmtColorAdd( mData[a.x + a.y * mSize.x], color );
          sum += dx;
          if( sum >= dy ) {
            sum -= dy;
            a.x += stepX;
            }
          }
        CmtColorAdd( mData[a.x + a.y * mSize.x], color );
        }
      }
    }
  }


void
CmtContext::LineTo( CmtPoint to, uint32 color, bool set ) {
  CmtPoint a( mPos );
  CmtPoint b( to );
  mPos = to;
  if( ClipLine( a, b ) ) {
    //Пропорциональная линия
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    int sum = 0;
    int stepX = dx >= 0 ? 1 : -1;
    int stepY = dy >= 0 ? 1 : -1;
    if( dx < 0 ) dx = -dx;
    if( dy < 0 ) dy = -dy;
    if( dx > dy ) {
      sum += dy / 2;
      if( set ) {
        for( ; a.x != b.x; a.x += stepX ) {
          mData[a.x + a.y * mSize.x] = color;
          sum += dy;
          if( sum >= dx ) {
            sum -= dx;
            a.y += stepY;
            }
          }
        mData[a.x + a.y * mSize.x] = color;
        }
      else {
        for( ; a.x != b.x; a.x += stepX ) {
          CmtColorAdd( mData[a.x + a.y * mSize.x], color );
          sum += dy;
          if( sum >= dx ) {
            sum -= dx;
            a.y += stepY;
            }
          }
        CmtColorAdd( mData[a.x + a.y * mSize.x], color );
        }
      }
    else {
      sum += dx / 2;
      if( set ) {
        for( ; a.y != b.y; a.y += stepY ) {
          mData[a.x + a.y * mSize.x] = color;
          sum += dx;
          if( sum >= dy ) {
            sum -= dy;
            a.x += stepX;
            }
          }
        mData[a.x + a.y * mSize.x] = color;
        }
      else {
        for( ; a.y != b.y; a.y += stepY ) {
          CmtColorAdd( mData[a.x + a.y * mSize.x], color );
          sum += dx;
          if( sum >= dy ) {
            sum -= dy;
            a.x += stepX;
            }
          }
        CmtColorAdd( mData[a.x + a.y * mSize.x], color );
        }
      }
    }
  }

void
CmtContext::Copy( CmtContext *sour, CmtPoint pos, CmtPoint size, bool set ) {
  if( !sour ) return;
  CopyRectSafe( size, sour->GetData(), sour->GetSize().x, pos, set );
  }

const uint32*     
CmtContext::ClipRect( CmtPoint &dest, CmtPoint &size, const uint32 *srcData, int32 srcSizeX ) {
  //Проверить попадание в область
  if( dest.x >= mClipSize.x || dest.y >= mClipSize.y || srcData == 0 ) return 0;
  
  //Выполнить ограничения
  if( dest.x < 0 ) {
    size.x += dest.x;
    srcData += -dest.x;
    dest.x = 0;
    }
  if( dest.y < 0 ) {
    size.y += dest.y;
    srcData += (-dest.y) * srcSizeX;
    dest.y = 0;
    }
  if( dest.x + size.x > mClipSize.x ) size.x = mClipSize.x - dest.x;
  if( dest.y + size.y > mClipSize.y ) size.y = mClipSize.y - dest.y;
  
  if( size.x <= 0 || size.y <= 0 ) return 0;
  return srcData;
  }

void     
CmtContext::CopyRectSafe( CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest, bool set ) {
  srcData = ClipRect( dest, size, srcData, srcSizeX );
  if( srcData != 0 )
    CopyRect( size, srcData, srcSizeX, dest, set );
  }

void     
CmtContext::CopyRect( CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest, bool set ) {
  uint32 *py = mData + dest.y * mSize.x + dest.x;
  int32 x, y;
  if( set ) {
    for( y = 0; y < size.y; y++ ) {
      //Скопировать строки
      for( x = 0; x < size.x; x++ ) {
        py[x] = srcData[x];
        }
      //Перейти к следующим строкам
      py += mSize.x;
      srcData += srcSizeX;
      }
    }
  else {
    for( y = 0; y < size.y; y++ ) {
      //Скопировать строки
      for( x = 0; x < size.x; x++ ) {
        CmtColorAdd( py[x], srcData[x] );
        }
      //Перейти к следующим строкам
      py += mSize.x;
      srcData += srcSizeX;
      }
    }
  }

void
CmtContext::CopyTranspRectSafe(uint32 transpColor, CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest) {
  srcData = ClipRect( dest, size, srcData, srcSizeX );
  if( srcData != 0 )
    CopyTranspRect( transpColor, size, srcData, srcSizeX, dest );
  }

void     
CmtContext::CopyTranspRect( uint32 transpColor, CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest ) {
  uint32 *py = mData + dest.y * mSize.x + dest.x;
  int32 x, y;
  for( y = 0; y < size.y; y++ ) {
    //Скопировать строки
    for( x = 0; x < size.x; x++ ) {
      uint32 color = srcData[x];
      if( color != transpColor )
        py[x] = color;
      }
    //Перейти к следующим строкам
    py += mSize.x;
    srcData += srcSizeX;
    }
  }

void
CmtContext::AddRectSafe(int transpFactor, CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest) {
  srcData = ClipRect( dest, size, srcData, srcSizeX );
  if( srcData != 0 )
    AddRect( transpFactor, size, srcData, srcSizeX, dest );
  }

void     
CmtContext::AddRect( int transpFactor, CmtPoint size, const uint32 *srcData, int32 srcSizeX, CmtPoint dest ) {
  uint32 *py = mData + dest.y * mSize.x + dest.x;
  int32 x, y;
  for( y = 0; y < size.y; y++ ) {
    //Скопировать строки
    for( x = 0; x < size.x; x++ ) {
      py[x] = SumPixel( py[x], srcData[x], transpFactor );
      }
    //Перейти к следующим строкам
    py += mSize.x;
    srcData += srcSizeX;
    }
  }

#ifdef CMT_IMAGE
void
CmtContext::Image( CmtPoint p, CmtImage *image, int32 frame, bool set ) {
  //Копируем из такой-же области
  if( image == 0 || image->mFrames == 0 ) return;
  uint32 *src = (uint32*)(image->mFrames + image->mFrameSize * frame + 1);
  CopyRectSafe( image->mSize, src, image->mSize.x, p, set );
  }

void
CmtContext::ImageBorder( CmtImage *image, int32 frame, CmtPoint pos, CmtPoint size, bool filled, CmtPoint center, bool set ) {
  int nextY = mSize.x;
  if( image->mFrames == 0 ) return;
  //Копируем из такой-же области
  uint32 *src = (uint32*)(image->mFrames + image->mFrameSize * frame + 1);
  uint32 *py = mData + (pos.y * mSize.x + pos.x);
  int nextSY = image->mSize.x;
  int x,y;
  if( center.x < 0 ) center.x = image->mSize.x >> 1;
  if( center.y < 0 ) center.y = image->mSize.y >> 1;
  if( set ) {
    //Верхняя граница
    for( y = 0; y < center.y; y++ ) {
      if( pos.y >= mClipSize.y ) return;
      if( pos.y >= 0 ) {
        int tx = pos.x;
        //Левая граница
        for( x = 0; x < center.x; x++ ) {
          if( tx >= 0 && tx < mClipSize.x ) py[x] = src[x];
          tx++;
          }
        //Удлинение
        uint32 v = src[x];
        int sx = size.x + center.x - image->mSize.x;
        while( x < sx ) {
          if( tx >= 0 && tx < mClipSize.x ) py[x] = v;
          x++;
          tx++;
          }
        //Правая граница
        sx = center.x;
        while( x < size.x ) {
          if( tx >= 0 && tx < mClipSize.x ) py[x] = src[sx];
          x++;
          sx++;
          tx++;
          }
        }
      pos.y++;
      src += nextSY;
      py += nextY;
      }
    //Вертикальное удлинение
    int sy = size.y + center.y - image->mSize.y;
    while( y < sy ) {
      if( pos.y >= mClipSize.y ) return;
      if( pos.y >= 0 ) {
        int tx = pos.x;
        //Левая граница
        for( x = 0; x < center.x; x++ ) {
          if( tx >= 0 && tx < mClipSize.x ) py[x] = src[x];
          tx++;
          }
        //Удлинение
        int sx = size.x + center.x - image->mSize.x;
        if( filled ) {
          uint32 v = src[x];
          while( x < sx ) {
            if( tx >= 0 && tx < mClipSize.x ) py[x] = v;
            x++;
            tx++;
            }
          }
        else {
          tx += sx - x;
          x = sx;
          }
        //Правая граница
        sx = center.x;
        while( x < size.x ) {
          if( tx >= 0 && tx < mClipSize.x ) py[x] = src[sx];
          x++;
          sx++;
          tx++;
          }
        }
      pos.y++;
      py += nextY;
      y++;
      }
    //Нижняя граница
    while( y < size.y ) {
      if( pos.y >= mClipSize.y ) return;
      if( pos.y >= 0 ) {
        int tx = pos.x;
        //Левая граница
        for( x = 0; x < center.x; x++ ) {
          if( tx >= 0 && tx < mClipSize.x ) py[x] = src[x];
          tx++;
          }
        //Удлинение
        uint32 v = src[x];
        int sx = size.x + center.x - image->mSize.x;
        while( x < sx ) {
          if( tx >= 0 && tx < mClipSize.x ) py[x] = v;
          x++;
          tx++;
          }
        //Правая граница
        sx = center.x;
        while( x < size.x ) {
          if( tx >= 0 && tx < mClipSize.x ) py[x] = src[sx];
          x++;
          sx++;
          tx++;
          }
        }
      pos.y++;
      src += nextSY;
      py += nextY;
      y++;
      }
    }
  else {
    //Верхняя граница
    for( y = 0; y < center.y; y++ ) {
      //Левая граница
      for( x = 0; x < center.x; x++ )
        CmtColorAdd( py[x], src[x]);
      //Удлинение
      uint32 v = src[x];
      int sx = size.x + center.x - image->mSize.x;
      while( x < sx )
        CmtColorAdd( py[x++], v);
      //Правая граница
      sx = center.x;
      while( x < size.x )
        CmtColorAdd( py[x++], src[sx++] );
      src += nextSY;
      py += nextY;
      }
    //Вертикальное удлинение
    int sy = size.y + center.y - image->mSize.y;
    while( y < sy ) {
      //Левая граница
      for( x = 0; x < center.x; x++ )
        CmtColorAdd( py[x], src[x] );
      //Удлинение
      int sx = size.x + center.x - image->mSize.x;
      if( filled ) {
        uint32 v = src[x];
        while( x < sx )
          CmtColorAdd( py[x++], v );
        }
      else x = sx + 1;
      //Правая граница
      sx = center.x;
      while( x < size.x )
        CmtColorAdd( py[x++], src[sx++] );
      py += nextY;
      y++;
      }
    //Нижняя граница
    while( y < size.y ) {
      //Левая граница
      for( x = 0; x < center.x; x++ )
        CmtColorAdd( py[x], src[x] );
      //Удлинение
      uint32 v = src[x];
      int sx = size.x + center.x - image->mSize.x;
      while( x < sx )
        CmtColorAdd( py[x++], v );
      //Правая граница
      sx = center.x;
      while( x < size.x )
        CmtColorAdd( py[x++], src[sx++] );
      src += nextSY;
      py += nextY;
      y++;
      }
    }
  }

void   
CmtContext::HImage( CmtImage *image, int32 frame, CmtPoint a, CmtPoint b, int32 align, CmtPoint *lt, CmtPoint *rb, bool set ) {
  CmtPoint s = image->mSize;
  CmtPoint p;
  //Вычислить начальную позицию x
  switch( align & MP_PHORZ_MASK ) {
    case MP_PCENTER  : p.x = (b.x - a.x - s.x) / 2 + a.x; break;
    case MP_PRIGHT   : p.x = b.x - s.x; break;
    default          : p.x = a.x;
    }
  switch( align & MP_PVERT_MASK ) {
    case MP_PMIDDLE  : p.y = (b.y - a.y - s.y) / 2 + a.y; break;
    case MP_PBOTTOM  : p.y = b.y - s.y; break;
    default          : p.y = a.y;
    }
  if( lt ) {
    if( align & MP_OVER_TEXT ) *lt = a;
    else {
      lt->x = (align & MP_PHORZ_MASK) == MP_PLEFT ? a.x + s.x : a.x;
      lt->y = (align & MP_PHORZ_MASK) == MP_PCENTER ? p.y + s.y : a.y;
      }
    }
  if( rb ) {
    if( align & MP_OVER_TEXT ) *rb = b;
    else {
      rb->x = (align & MP_PHORZ_MASK) == MP_PRIGHT ? p.x : b.x;
      rb->y = b.y;
      }
    }
  Image( p, image, frame, set );
  }

#endif
 
void
CmtContext::Clip( CmtPoint &p ) {
  if( p.x < 0 ) p.x = 0;
  if( p.y < 0 ) p.y = 0;
  if( p.x > mClipSize.x ) p.x = mClipSize.x;
  if( p.y > mClipSize.y ) p.y = mClipSize.y;
  }

int32
CmtContext::ClipCode( CmtPoint p ) {
  return (p.x < 0 ? 8:0) | (p.x >= mClipSize.x ? 4:0) | (p.y < 0 ? 2:0) | (p.y >= mClipSize.y ? 1:0);
  }

bool
CmtContext::ClipLine( CmtPoint &a, CmtPoint &b ) {
  //Обрезание
  int c1 = ClipCode(a);
  int c2 = ClipCode(b);
  while( c1 | c2 ) {
    if( c1 & c2 ) return false; //Граница над окном
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    if( c1 ) {
      if( a.y < 0 ) {
        a.x += dx * -a.y / dy;
        a.y = 0;
        }
      else if( a.y >= mClipSize.y ) {
        a.x += dx * (mClipSize.y - a.y) / dy;
        a.y = mClipSize.y - 1;
        }
      else if( a.x < 0 ) {
        a.y += dy * -a.x / dx;
        a.x = 0;
        }
      else if( a.x >= mClipSize.x ) {
        //Добавить проекцию
        a.y += dy * (mClipSize.x - a.x) / dx;
        a.x = mClipSize.x - 1;
        }
      c1 = ClipCode(a);
      }
    else {
      if( b.y < 0 ) {
        b.x += dx * -b.y / dy;
        b.y = 0;
        }
      else if( b.y >= mClipSize.y ) {
        b.x += dx * (mClipSize.y - b.y) / dy;
        b.y = mClipSize.y - 1;
        }
      else if( b.x < 0 ) {
        //Добавить проекцию
        b.y += dy * -b.x / dx;
        b.x = 0;
        }
      else if( b.x >= mClipSize.x ) {
        //Добавить проекцию
        b.y += dy * (mClipSize.x - b.x) / dx;
        b.x = mClipSize.x - 1;
        }
      c2 = ClipCode(b);
      }
    }
  return true;
  }


void
CmtContext::Line( CmtPoint a, CmtPoint b, uint32 color, bool set ) {
  MoveTo( a );
  LineTo( b, color, set );
  }

void   
CmtContext::ThickLine( CmtPoint a, CmtPoint b, uint32 thickness, uint32 color, bool set ) { //Линия заданной толщины
  MoveTo( a );
  ThickLineTo( b, thickness, color, set );
  }


void
CmtContext::Rect( CmtPoint a, CmtPoint b, uint32 color, bool set ) {
  MoveTo( a );
  LineTo( CmtPoint(a.x,b.y), color, set );
  LineTo( b, color, set );
  LineTo( CmtPoint(b.x,a.y), color, set );
  LineTo( a, color, set );
  }

void
CmtContext::Clear( uint32 color, bool set ) {
  FilledRect( CmtPoint(), mSize, color, set );
  }

#ifdef CMT_FONT
void
CmtContext::HText( uint32 color, cpchar str, CmtPoint a, CmtPoint b, int32 align, bool set ) {
  if( str == 0 || str[0] == 0 ) return;
  CmtPoint s = GetTextExtent( str );
  //Вычислить начальную позицию x
  switch( align & MP_THORZ_MASK ) {
    case MP_TCENTER  : a.x = (b.x - a.x - s.x) / 2 + a.x; break;
    case MP_TRIGHT   : a.x = b.x - s.x; break;
    }
  switch( align & MP_TVERT_MASK ) {
    case MP_TMIDDLE  : a.y = (b.y - a.y - s.y) / 2 + a.y; break;
    case MP_TBOTTOM  : a.y = b.y - s.y; break;
    }
  //Вывод текста
  while( *str ) {
    cpchar ptr = strchr( str, '\n' );
    int len = ptr ? ptr - str : strlen(str);
    CmtPoint ps = GetTextExtent( str, len );
    //Вычислить начальную позицию x
    switch( align & MP_THORZ_MASK ) {
      case MP_TLEFT    : ps.x = a.x; break;
      case MP_TCENTER  : ps.x = a.x + (s.x - ps.x) / 2; break;
      case MP_TRIGHT   : ps.x = a.x + (s.x - ps.x); break;
      }
    Text( color, CmtPoint(ps.x,a.y), str, len, set );
    a.y += ps.y;
    str += len;
    if( str[0] == '\n' ) str++;
    }
  }

cpchar
strchrBack( cpchar src, char ch, cpchar bound ) {
  if( src == 0 ) return bound;
  while( src != bound && src[-1] != ch ) src--;
  return src;
  }

//Вывод текста строго ограниченного заданными координатами
void
CmtContext::RText( uint32 color, cpchar str, CmtPoint a, CmtPoint b, int32 align, bool set ) {
  if( str == 0 || str[0] == 0 || mFont == 0 ) return;
  int width = b.x - a.x;
  CmtPoint s;
  if( align & MP_WRAP_TEXT ) s = GetWrapTextExtent( str, width );
  else s = GetTextExtent( str );
  //Вычислить начальную позицию x
  switch( align & MP_THORZ_MASK ) {
    case MP_TCENTER  : a.x = (b.x - a.x - s.x) / 2 + a.x; break;
    case MP_TRIGHT   : a.x = b.x - s.x; break;
    }
  switch( align & MP_TVERT_MASK ) {
    case MP_TMIDDLE  : a.y = (b.y - a.y - s.y) / 2 + a.y; break;
    case MP_TBOTTOM  : a.y = b.y - s.y; break;
    }
  //Вывод текста
  cpchar ptr;
  int len;
  CmtPoint ps;
  while( *str ) {
    ptr = strchr( str, '\n' );
    len = ptr ? ptr - str : strlen(str);
    ps = GetTextExtent( str, len );
    while( ps.x > width ) {
      ptr = str + len;
      //Текст не влезает в строку
      if( align & MP_WRAP_TEXT ) {
        //Обнаружить границу перехода
        ptr = strchrBack( ptr, ' ', str );
        //cpchar nextWord = ptr;
        //while( ptr != str && 
        if( ptr == str ) {
          //Ни одно слово не влезает целиком
          len = GetFilledLen( mFont, str, width );
          if( len == 0 ) return; //Не влезает ни одного символа
          break;
          }
        else {
          len = ptr - str;
          while( len && str[len-1] == ' ' ) len--;
          if( len == 0 ) break;
          ps = GetTextExtent( str, len );
          }
        }
      else {
        //Определить то, что влезает
        len = GetFilledLen( mFont, str, width );
        if( len == 0 ) return; //Не влезает ни одного символа
        ps = GetTextExtent( str, len );
        }
      }
    //Вычислить начальную позицию x
    switch( align & MP_THORZ_MASK ) {
      case MP_TLEFT    : ps.x = a.x; break;
      case MP_TCENTER  : ps.x = a.x + (s.x - ps.x) / 2; break;
      case MP_TRIGHT   : ps.x = a.x + (s.x - ps.x); break;
      }
    if( a.y + ps.y > b.y ) return;
    Text( color, CmtPoint(ps.x,a.y), str, len, set );
    a.y += ps.y;
    str += len;
    if( str[0] == '\n' ) str++;
    else {
      while( str[0] == ' ' ) str++;
      }
    }
  }
#endif

#if( defined(CMT_GR_SIMPLE_POLYGON) || defined(CMT_GR_POLYGON) )
//==============================================================================
//------------------- Механизм простых полигонов -------------------------------
struct CmtEdge {
  int mNext;      //Индекс следующего Edge с данным y
  int mNumLines;  //Количество строк пересекаемых данным Edge
  int mIncX;      //Инкремент X на один шаг Y (в 1/1024)
  int mX;         //Последняя позиция X

  bool Next() { mX += mIncX; return (--mNumLines) != 0; }
  void Setup( CmtPoint a, CmtPoint b );
  bool Great( CmtPoint a, int bx );
  //void Change( CmtEdge &sour );
  void operator = ( CmtEdge &sour ) { mNext = sour.mNext; mNumLines = sour.mNumLines; mIncX = sour.mIncX; mX = sour.mX; }
  };

void
CmtEdge::Setup( CmtPoint a, CmtPoint b ) {
  mNext = a.y;
  mNumLines = b.y - a.y;
  mIncX = ((b.x - a.x) << 10) / (b.y - a.y);
  mX = a.x << 10;
  }

bool
CmtEdge::Great( CmtPoint a, int incX ) {
  if( mNext > a.y ) return true;
  if( mNext < a.y ) return false;
  if( (mX >> 10) < a.x ) return false;
  if( (mX >> 10) > a.x ) return true;
  if( mIncX > incX ) return true;
  return false;
  }

void
AddSimpleEdge( CmtPoint a, CmtPoint b, CmtEdge *simpleEdge, int32 &simpleEdgeCount ) {
  //Проверить горизонтальность
  if( a.y == b.y ) return;
  //Проверить сортировку
  if( b.y < a.y ) AddSimpleEdge( b, a, simpleEdge, simpleEdgeCount );
  else {
    //Собственно добавление
    int incX = ((b.x - a.x) << 10) / (b.y - a.y);
    for( int i = simpleEdgeCount; i >= 0; i-- )
      if( i == 0 ) {
        //Вставка
        simpleEdge[i].Setup( a, b );
        break;
        }
      else if( simpleEdge[i - 1].Great( a, incX ) ) {
        //Сдвижка
        simpleEdge[i] = simpleEdge[i - 1];
        }
      else {
        //Вставка
        simpleEdge[i].Setup( a, b );
        break;
        }
    simpleEdgeCount++;
    }
  }


typedef CmtEdge          *PEdge;
#endif

#ifdef CMT_GR_POLYGON
static CmtEdge edgeTable[CMT_GR_POLYGON];
static int   bucket[1200];
static int   edgeCount;

void
EdgeSort( PEdge *pEdge, int c ) {
  int i = 0, j = c;
  PEdge temp;
  int x = pEdge[ c >> 1 ]->mX;
  do {
    while( pEdge[i]->mX < x ) i++;
    while( pEdge[j]->mX > x ) j--;

    if( i < j ) {
      temp = pEdge[i];
      pEdge[i] = pEdge[j];
      pEdge[j] = temp;
      }
    if( i <= j ) {
      i++;
      j--;
      }
    }
  while( i <= j );

  if( j > 0 ) EdgeSort( pEdge, j );
  if( c > i ) EdgeSort( pEdge + i, c - i );
  }

void
CmtContext::AddEdge( CmtPoint a, CmtPoint b ) {
  //Проверить горизонтальность
  if( a.y == b.y ) return;
  //Проверить сортировку
  if( b.y < a.y ) AddEdge( b, a );
  else {
    CmtEdge edge;
    //Обрезание
    int c1 = ClipCode(a);
    int c2 = ClipCode(b);
    while( c1 | c2 ) {
      if( c1 & c2 ) {
        if( c1 & c2 & 1 ) return; //Граница над окном
        if( c1 & c2 & 2 ) return; //Граница под окном
        //Отрезок целиком сбоку вне границ, добавить проекцию
        if( a.y < 0 ) a.y = 0;
        if( b.y >= mClipSize.y ) b.y = mClipSize.y - 1;
        if( a.x < 0 ) a.x = 0;
        if( a.x >= mClipSize.x ) a.x = mClipSize.x - 1;
        if( b.x < 0 ) b.x = 0;
        if( b.x >= mClipSize.x ) b.x = mClipSize.x - 1;
        break;
        }
      int dx = b.x - a.x;
      int dy = b.y - a.y;
      if( c1 ) {
        if( a.y < 0 ) { a.x += dx * -a.y / dy; a.y = 0; }
        else if( a.y >= mClipSize.y ) { a.x += dx * (mClipSize.y - a.y) / dy; a.y = mClipSize.y - 1; }
        else if( a.x < 0 ) {
          //Добавить проекцию
          CmtPoint p(a);
          a.y += dy * -a.x / dx;
          p.x = a.x = 0;
          AddEdge( a, p );
          }
        else if( a.x >= mSize.x ) {
          //Добавить проекцию
          CmtPoint p(a);
          a.y += dy * (mSize.x - a.x) / dx;
          p.x = a.x = mSize.x - 1;
          AddEdge( a, p );
          }
        c1 = ClipCode(a);
        }
      else {
        if( b.y < 0 ) { b.x += dx * -b.y / dy; b.y = 0; }
        else if( b.y >= mClipSize.y ) { b.x += dx * (mClipSize.y - b.y) / dy; b.y = mClipSize.y - 1; }
        else if( b.x < 0 ) {
          //Добавить проекцию
          CmtPoint p(b);
          b.y += dy * -b.x / dx;
          p.x = b.x = 0;
          AddEdge( b, p );
          }
        else if( b.x >= mClipSize.x ) {
          //Добавить проекцию
          CmtPoint p(b);
          b.y += dy * (mClipSize.x - b.x) / dx;
          p.x = b.x = mClipSize.x - 1;
          AddEdge( b, p );
          }
        c2 = ClipCode(b);
        }
      }
    //Проверить горизонтальность
    if( a.y == b.y || edgeCount >= MAX_EDGE ) return;
    edge.mNumLines = b.y - a.y;
    edge.mIncX = ((b.x - a.x) << 10) / (b.y - a.y);
    edge.mX = a.x << 10;
    edge.mNext = bucket[a.y];
    bucket[a.y] = edgeCount; //edgeTable.GetNumber();
    edgeTable[edgeCount++] = edge;
    }
  }
#endif

#ifdef CMT_GR_SIMPLE_POLYGON
void
CmtContext::SimpleFill( CmtPoint p1, CmtPoint p2, CmtPoint p3, CmtPoint p4, uint32 color, bool set ) {
  CmtPoint p[16];
  int    vertex = 0;
  CmtPoint a,b;
  int32  simpleEdgeCount = 0;
  CmtEdge  simpleEdge[32];

  //Анализ граней
  if( ClipCode(p1) & ClipCode(p2) & ClipCode(p3) & ClipCode(p4) ) return; //Полигон вне окна
  a = p1; b = p2;
  if( ClipLine( a, b ) ) {
    p[0] = a;
    p[1] = b;
    vertex = 2;
    }
  a = p2; b = p3;
  if( ClipLine( a, b ) ) {
    if( !vertex || p[vertex - 1] != a ) p[vertex++] = a;
    p[vertex++] = b;
    }
  a = p3; b = p4;
  if( ClipLine( a, b ) ) {
    if( !vertex || p[vertex - 1] != a ) p[vertex++] = a;
    p[vertex++] = b;
    }
  a = p4; b = p1;
  if( ClipLine( a, b ) ) {
    if( !vertex || p[vertex - 1] != a ) p[vertex++] = a;
    if( p[0] != b ) p[vertex++] = b;
    }
  if( vertex <= 2 ) return;
  //Добавить грани
  for( int i = 0; i < vertex - 1; i++ ) {
    AddSimpleEdge( p[i], p[i + 1], simpleEdge, simpleEdgeCount );
    }
  //Завершающий
  AddSimpleEdge( p[vertex-1], p[0], simpleEdge, simpleEdgeCount );
  //Заполнение
  SimpleFillCore( simpleEdge, simpleEdgeCount, color, set );
  }

void
CmtContext::SimpleFillCore( CmtEdge *simpleEdge, int32 simpleEdgeCount, uint32 color, bool set ) {
  //Заполнение
  int y = simpleEdge[0].mNext;
  int lp = 0;
  int pp = 1;
  int i = 2;
  while( i <= simpleEdgeCount ) {
    //Взять пару
    int x1 = simpleEdge[lp].mX >> 10;
    int x2 = simpleEdge[pp].mX >> 10;
    //Штриховка строки
    HorzLine( x1, x2, y, color, set );
    //Перейти к следующей строке
    y++;
    if( !simpleEdge[lp].Next() ) lp = i++;
    if( !simpleEdge[pp].Next() ) pp = i++;
    }
  }
#endif

#ifdef CMT_GR_POLYGON
//==============================================================================
//------------------- Механизм полигонов ---------------------------------------
void   
CmtContext::InitPoly() {
  edgeCount = 0;
  memset( bucket, -1, sizeof(int) * mSize.y );
  }

void   
CmtContext::AddPolyVertex( CmtPoint table[], int32 count ) {
  if( count < 2 ) return;
  count--;
  for( int i = 0; i < count; i++ )
    AddEdge( table[i], table[i+1] );
  }

void   
CmtContext::Polygon( CmtPoint table[], int32 count, uint32 color, bool set ) {
  if( count < 3 || mClipSize.x == 0 || mClipSize.y == 0 ) return;
  //Инициализация механизма полигона
  InitPoly();
  //Добавить вершины полигона
  AddPolyVertex( table, count );
  //Штриховка полигона
  PEdge active[22];
  int activeCount = 0, i1, i2;
  for( int y = 0; y < mClipSize.y; y++ ) {
    //Уплотнить
    for( i1 = i2 = 0; i2 < activeCount; i2++ ) {
      if( active[i2] ) {
        active[i1] = active[i2];
        i1++;
        }
      }
    activeCount = i1;

    if( bucket[y] >= 0 ) {
      //Добавить в активный список
      for( int i = bucket[y]; i >= 0; i = edgeTable[i].mNext ) {
        if( activeCount >= 21 )
          break;
        active[activeCount++] = &(edgeTable[i]);
        }
      }

    if( !activeCount ) continue;

    //Сортировать полученный список
    EdgeSort( active, activeCount - 1 );

    //Заполнять между парами
    for( int p = 0; p < activeCount-1; p += 2 ) {
      //Взять пару
      int x1 = active[p]->mX >> 10;
      int x2 = active[p+1]->mX >> 10;
      //Штриховать
      HorzLine( x1, x2, y, color, set );
      //Обновить пару
      if( !active[p]->Next()   ) active[p] = 0;
      if( !active[p+1]->Next() ) active[p+1] = 0;
      }
    }
  }
#endif

void
CmtContext::Circle( CmtPoint center, int32 radius, uint32 color, bool set ) {
  int x = 0, y = radius, d = 3 - 2 * radius;
  while( x < y ) {
    PixelCircle( center, x, y, color, set );
    if( d < 0 ) d = d + 4 * x + 6;
    else {
      d= d + 4*(x-y) + 10;
      --y;
      }
    ++x;
    }
  if( x == y )
    PixelCircle( center, x, y, color, set );
  }

void
CmtContext::FilledCircle( CmtPoint center, int32 radius, uint32 color, bool set ) {
  int x = 0, y = radius, d = 3 - 2 * radius;
  while( x < y ) {
    FillPixelCircle( center, x, y, color, set );
    if( d < 0 ) d = d + 4 * x + 6;
    else {
      d= d + 4*(x-y) + 10;
      --y;
      }
    ++x;
    }
  if( x == y )
    FillPixelCircle( center, x, y, color, set );
  }

void
CmtContext::PixelCircle( CmtPoint center, int32 x, int32 y, uint32 color, bool set ) {
  SetPixel( CmtPoint(center.x+x, center.y+y), color, set);
  SetPixel( CmtPoint(center.x+y, center.y+x), color, set);
  SetPixel( CmtPoint(center.x+y, center.y-x), color, set);
  SetPixel( CmtPoint(center.x+x, center.y-y), color, set);
  SetPixel( CmtPoint(center.x-x, center.y-y), color, set);
  SetPixel( CmtPoint(center.x-y, center.y-x), color, set);
  SetPixel( CmtPoint(center.x-y, center.y+x), color, set);
  SetPixel( CmtPoint(center.x-x, center.y+y), color, set);
  }

void
CmtContext::FillPixelCircle( CmtPoint center, int32 x, int32 y, uint32 color, bool set ) {
  HorzLine( center.x-x, center.x+x, center.y-y, color, set );
  HorzLine( center.x-x, center.x+x, center.y+y, color, set );
  HorzLine( center.x-y, center.x+y, center.y+x, color, set );
  HorzLine( center.x-y, center.x+y, center.y-x, color, set );
  }


//==============================================================================
//------------------------- Шрифты ---------------------------------------------
#ifdef CMT_FONT
#ifdef CMT_FONT_W5
static const uint32 fontImages5[] = {
  #include <Sys/Gui/fonts/mvnFntImagesWx5.c>
  };

static CmtFontW font5 = {
  5,
  false,
  false,
  CMT_FONT_W5,
  6,
  3,
  fontImages5,
  {
   #include <Sys/Gui/fonts/mvnFntCharWx5.c>
  }
  };
#endif

#ifdef CMT_FONT_W7
static const uint32 fontImages7[] = {
  #include <Sys/Gui/fonts/mvnFntImagesWx7.c>
  };

static CmtFontW font7 = {
  7,
  false,
  false,
  CMT_FONT_W7,
  9,
  6,
  fontImages7,
  {
   #include <Sys/Gui/fonts/mvnFntCharWx7.c>
  }
  };
#endif

#ifdef CMT_FONT_W8
static const uint32 fontImages8[] = {
  #include <Sys/Gui/fonts/mvnFntImagesWx8.c>
  };

static CmtFontW font8 = {
  8,
  false,
  false,
  CMT_FONT_W8,
  9,
  6,
  fontImages8,
  {
   #include <Sys/Gui/fonts/mvnFntCharWx8.c>
  }
  };
#endif

#ifdef CMT_FONT_W10
static const uint32 fontImages10[] = {
  #include <Sys/Gui/fonts/mvnFntImagesWx10.c>
  };

static CmtFontW font10 = {
  10,
  false,
  false,
  CMT_FONT_W10,
  15,
  6,
  fontImages10,
  {
   #include <Sys/Gui/fonts/mvnFntCharWx10.c>
  }
  };
#endif

#ifdef CMT_FONT_W12
static const uint32 fontImages12[] = {
  #include <Sys/Gui/fonts/mvnFntImagesWx12.c>
  };

static CmtFontW font12 = {
  12,
  false,
  false,
  CMT_FONT_W12,
  17,
  6,
  fontImages12,
  {
   #include <Sys/Gui/fonts/mvnFntCharWx12.c>
  }
  };
#endif

#ifdef CMT_FONT_W14
static const uint32 fontImages14[] = {
  #include <Sys/Gui/fonts/mvnFntImagesWx14.c>
  };

static CmtFontW font14 = {
  14,
  false,
  false,
  CMT_FONT_W14,
  19,
  6,
  fontImages14,
  {
   #include <Sys/Gui/fonts/mvnFntCharWx14.c>
  }
  };
#endif

#ifdef CMT_FONT_W16
static const uint32 fontImages16[] = {
  #include <Sys/Gui/fonts/mvnFntImagesWx16.c>
  };

static CmtFontW font16 = {
  16,
  false,
  false,
  CMT_FONT_W16,
  22,
  6,
  fontImages16,
  {
   #include <Sys/Gui/fonts/mvnFntCharWx16.c>
  }
  };
#endif

#ifdef CMT_FONT_W18
static const uint32 fontImages18[] = {
  #include <Sys/Gui/fonts/mvnFntImagesWx18.c>
  };

static CmtFontW font18 = {
  18,
  false,
  false,
  CMT_FONT_W18,
  26,
  9,
  fontImages18,
  {
   #include <Sys/Gui/fonts/mvnFntCharWx18.c>
  }
  };
#endif

#ifdef CMT_FONT_W30
static const uint32 fontImages30[] = {
  #include <Sys/Gui/fonts/mvnFntImagesWx30.c>
  };

static CmtFontW font30 = {
  30,
  false,
  false,
  CMT_FONT_W30,
  43,
  18,
  fontImages30,
  {
   #include <Sys/Gui/fonts/mvnFntCharWx30.c>
  }
  };
#endif

//=========== Courier =============
#ifdef CMT_FONT_C8
static const uint32 fontImagesC8[] = {
  #include <Sys/Gui/fonts/mvnFntImagesCx8.c>
  };

static CmtFontW fontC8 = {
  8,
  false,
  false,
  2,
  9,
  7,
  fontImagesC8,
  {
   #include <Sys/Gui/fonts/mvnFntCharCx8.c>
  }
  };
#endif


CmtFontW*
CmtContext::GetFont( uint32 sizePixel, bool, bool, uint32 fontType ) {
  if( fontType == FT_COURIER ) {
#ifdef CMT_FONT_C8
    if( sizePixel == 8 ) return &fontC8;
#endif
    return 0;
    }
  else {
    if( sizePixel < 4 )        return 0; //Надпись не видна
#ifdef CMT_FONT_W30
    else if( sizePixel >= 30 ) return &font30;
#endif
#ifdef CMT_FONT_W18
    else if( sizePixel >= 18 ) return &font18;
#endif
#ifdef CMT_FONT_W16
    else if( sizePixel >= 16 ) return &font16;
#endif
#ifdef CMT_FONT_W14
    else if( sizePixel >= 14 ) return &font14;
#endif
#ifdef CMT_FONT_W12
    else if( sizePixel >= 12 ) return &font12;
#endif
#ifdef CMT_FONT_W10
    else if( sizePixel >= 10 ) return &font10;
#endif
#ifdef CMT_FONT_W8
    else if( sizePixel >= 8 )  return &font8;
#endif
#ifdef CMT_FONT_W7
    else if( sizePixel >= 7 )  return &font7;
#endif
#ifdef CMT_FONT_W5
    else if( sizePixel >= 5 )  return &font5;
#endif
    return 0;
    }
  }

void
CmtContext::SetFont( uint32 sizePixel, bool bold, bool italic, uint32 fontType ) {
  mFont = GetFont( sizePixel, bold, italic, fontType );
  }

void
CmtContext::Text( uint32 color, CmtPoint point, const char *str, uint32 len, bool set ) {
  if( mFont == 0 ) return;
  int c;
  if( str == 0 ) return; // если строка пустая  -> выход
  CmtFontW *font = (CmtFontW*)mFont;
  CmtPoint p;
  CmtPoint center(point);
  int x = 0;
  double sinVal, cosVal;
  const uint32 *img;
  int32 pixelLevels[4];
  if( mFontDir ) CmtPoint::GetSinCos( mFontDir, &sinVal, &cosVal );
  while( *str && len ) { // перекладка символов пока не конец строки или не граница по x-координате
    if( *str == '\n' ) {
      //Перенос строки
      point.y += font->mDeltaY;
      str++;
      len--;
      x = 0;
      continue;
      }
    if( *str == ' ' ) {
      x += font->mSpaceX;
      str++;
      len--;
      continue;
      }
    CmtFontCharW *ch = font->mCharTable + ((*str) & 0xff);
    int cx = ch->mX + font->mDeltaX;
    int cy = ch->mY & 0xff; //Высота символа
    cy -= (ch->mY >> 8) & 0xff; //Смещение символа под базу
    img = font->mImages + ch->mIndex;
    c = font->mDeltaY - cy;
    cy = font->mDeltaY + ((ch->mY >> 8) & 0xff); //Смещение символа под базу
    if( c < 0 ) c = 0;
    for( ; c < cy; c++ ) {
      uint32 mask = *img;
      if( c < 0 ) mask = 0;
      else img++;
      for( int b = 0; b < cx && mask; b++ ) {
        if( b == 32 && ch->mX > 32 ) mask = *img++;
        p = point;
        p.x += x + b;
        p.y += c;
        //Проверить необходимость отображения 
        if( mask & 0x1 ) {
          if( mFontDir ) {
            //Повернуть в соответствии с direction
            p = p.RotateL( center, sinVal, cosVal, pixelLevels );
            //Вывести пиксел в режиме добавления
            AddPixel( p, color, pixelLevels[0] );
            AddPixel( CmtPoint(p.x+1,p.y), color, pixelLevels[1] );
            AddPixel( CmtPoint(p.x,p.y+1), color, pixelLevels[2] );
            AddPixel( CmtPoint(p.x+1,p.y+1), color, pixelLevels[3] );
            }
          else {
            //Прямой вывод
            if( p.x >= 0 && p.x < mClipSize.x && p.y >= 0 && p.y < mClipSize.y ) {
              //Координаты попадают в окно
              SetPixel( p, color, set );
              }
            }
          }
        mask >>= 1;
        }
      }
    str++;
    len--;
    x += cx;
    }
  }

CmtPoint 
CmtContext::GetTextExtent( CmtFontW *font, const char *str, uint32 len ) {
  CmtPoint size;
  if( font == 0 || str == 0 ) {
    if( font ) size.y = font->mDeltaY;
    return size; // если строка пустая  -> выход
    }
  int x = 0;
  while( *str && len ) { // перекладка символов пока не конец строки или не граница по x-координате
    if( *str == '\n' ) {
      //Перенос строки
      size.y += font->mDeltaY;
      str++;
      len--;
      x = 0;
      continue;
      }
    if( *str == ' ' ) x += font->mSpaceX;
    else {
      CmtFontCharW *ch = font->mCharTable + ((*str) & 0xff);
      x += ch->mX + font->mDeltaX;
      }
    if( x > size.x ) size.x = x;
    str++;
    len--;
    }
  size.y += font->mDeltaY;
  return size;
  }

CmtPoint 
CmtContext::GetWrapTextExtent( CmtFontW *font, const char *str, int width, uint32 glen ) {
  CmtPoint size;
  if( font == 0 || str == 0 )
    return size; // если строка пустая  -> выход
  //Определение размера текста
  cpchar ptr;
  int len;
  CmtPoint ps;
  while( *str && glen ) {
    ptr = strchr( str, '\n' );
    len = ptr ? ptr - str : strlen(str);
    if( (uint32)len > glen ) len = glen;
    ps = GetTextExtent( font, str, len );
    while( ps.x > width ) {
      //Текст не влезает в строку
      ptr = str + len;
      //Обнаружить границу перехода
      ptr = strchrBack( ptr, ' ', str );
      if( ptr == str ) {
        //Ни одно слово не влезает целиком
        len = GetFilledLen( font, str, width );
        if( len == 0 ) return CmtPoint(0,0); //Не влезает ни одного символа
        break;
        }
      else {
        len = ptr - str;
        while( len && str[len-1] == ' ' ) len--;
        if( len == 0 ) break;
        ps = GetTextExtent( font, str, len );
        }
      }
    if( size.x < ps.x ) size.x = ps.x;
    size.y += font->mDeltaY;
    str += len;
    glen -= len;
    if( str[0] == '\n' && glen ) {
      str++;
      glen--;
      }
    }
  return size;
  }

CmtPoint
CmtContext::GetTextExtent( cpchar str, uint32 len ) {
  return GetTextExtent( mFont, str, len );
  }

CmtPoint
CmtContext::GetWrapTextExtent(const char *str, int width, uint32 len ) {
  return GetWrapTextExtent( mFont, str, width, len );
  }

//Определить длину текста влезающего в данную ширину
int
CmtContext::GetFilledLen(CmtFontW *font, cpchar str, int width) {
  int len = 0;
  if( font == 0 || str == 0 )
    return len; // если строка пустая  -> выход
  int x = 0;
  while( *str ) { // перекладка символов пока не конец строки или не граница по x-координате
    if( *str == '\n' ) {
      //Перенос строки
      str++;
      x = 0;
      continue;
      }
    if( *str == ' ' ) x += font->mSpaceX;
    else {
      CmtFontCharW *ch = font->mCharTable + ((*str) & 0xff);
      x += ch->mX + font->mDeltaX;
      }
    if( x > width ) return len;
    str++;
    len++;
    }
  return len;
  }
#endif //CMT_FONT

//Реализации аппаратного ускорения
#ifdef CMT_TI_OMAP3530
  #include <Sys/Gui/cmtContextTiOmap3530.cpp>
#endif
