//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <math.h>
const double pi = 3.14159265358979323846 / 180000.0;

//==========================================================================
//----------------------- CmtPoint -------------------------------------------
//ѕолучение угла поворота точки относительно другой точки
double
CmtPoint::GetAngle( CmtPoint center ) {
  int dx = x - center.x;
  int dy = y - center.y;
  double radius = Distance(center);
  double ang;
  if( radius ) {
    ang = asin( dy / radius ) / pi;
    if( dx < 0 )  ang = 180000 - ang;
    if( ang < 0 ) ang = 360000 + ang;
    }
  else ang = 0;
  return ang;
  }

//ѕоворот точки относительно центра
//”гол в тыс€чных дол€х градуса
CmtPoint
CmtPoint::Rotate( CmtPoint center, double angle ) {
  double dx = x - center.x, dy = y - center.y;
  double sinAngle = sin(pi * angle), cosAngle = cos(pi * angle);
  CmtPoint p;
  p.x = (long)(center.x + dx*cosAngle - dy*sinAngle);
  p.y = (long)(center.y + dx*sinAngle + dy*cosAngle);
  return p;
  }

void 
CmtPoint::GetSinCos( int ang, double *sinVal, double *cosVal ) {
  double angle = ang;
  angle *= pi;
  *sinVal = sin( angle );
  *cosVal = cos( angle );
  }


//ѕоворот точки относительно центра с определением промежуточных позиций
//”гол в тыс€чных дол€х градуса
CmtPoint
CmtPoint::RotateL( CmtPoint center, double sinAngle, double cosAngle, int32 *levelTable ) {
  double dx = x - center.x, dy = y - center.y;
  CmtPoint p;
  double ddx = dx*cosAngle - dy*sinAngle;
  double ddy = dx*sinAngle + dy*cosAngle;
  dx = floor( ddx );
  dy = floor( ddy );
  ddx -= dx;
  ddy -= dy;
  levelTable[3] = (int32)( 256 * ddx * ddy );
  levelTable[2] = (int32)( 256 * (1.0 - ddx) * ddy );
  levelTable[1] = (int32)( 256 * ddx * (1.0 - ddy) );
  levelTable[0] = (int32)( 256 * (1.0 - ddx) * (1.0 - ddy) );
  p.x = (long)(center.x + dx);
  p.y = (long)(center.y + dy);
  return p;
  }

void   
CmtPoint::Ajust( CmtPoint origin, CmtPoint ajust, CmtPoint delta ) {
  x = origin.x + (ajust.x ? delta.x / ajust.x : 0);
  y = origin.y + (ajust.y ? delta.y / ajust.y : 0);
  }

//–ассто€ние между двум€ точками
double
CmtPoint::Distance( CmtPoint p ) {
  double dx = p.x - x;
  double dy = p.y - y;
  return sqrt( dx * dx + dy * dy );
  }

bool 
CmtPoint::IsOverRect( CmtPoint testPos, CmtPoint testSize, CmtPoint overPos, CmtPoint overSize ) {
  return overPos.x <= testPos.x && overPos.y <= testPos.y && (overPos.x + overSize.x) >= (testPos.x + testSize.x) && (overPos.y + overSize.y) >= (testPos.y + testSize.y);
  }

//! ќпераци€ получени€ точки с максимальными значени€ми
void     
CmtPoint::Max( CmtPoint test ) {
  if( x < test.x ) x = test.x;
  if( y < test.y ) y = test.y;
  }

//! ќпераци€ получени€ точки с минимальными значени€ми
void     
CmtPoint::Min( CmtPoint test ) {
  if( x > test.x ) x = test.x;
  if( y > test.y ) y = test.y;
  }

//==========================================================================
//----------------------- CmtRect --------------------------------------------
void   
CmtRect::Ajust( CmtRect &origin, CmtRect &ajust, CmtPoint delta ) {
  a.Ajust( origin.a, ajust.a, delta );
  b.Ajust( origin.b, ajust.b, delta );
  }

bool
CmtRect::IsPointInside( CmtPoint point ) {
  return a.x <= point.x && a.y <= point.y && b.x >= point.x && b.y >= point.y;
  }
