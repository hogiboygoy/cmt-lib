//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! �������� �������� ������ �� �������� ������ � ������� ����� �����
/*! ������ �������� ������ � ����������� ��������, ������ � ���� ������
  \param ctx �������� ������
  \param pos ������� ���� ������
  \param size ������ ���� ������
  \param color ���� �������
  \param offset �������� ������ (� ������ ������)
  \param scale ������� ����������� � 1/1024 ��������� �� �����
  \param values ������ ��������
  \param count ���������� �������� */
void 
CmtDrawLinearGraphic( CmtContext *ctx, CmtPoint pos, CmtPoint size, uint32 color, CmtPoint offset, CmtPoint scale, int32 *values, int count ) {
  //��������� ��������� ����� �������
  //������� ��������� � 0, ��������������� �������� �������������� ��������� ���������
  int32 x1, x2, y1, y2, y;
  size.y--;
  //��������� � ��������� �����
  y1 = y2 = offset.y + (values[0] * scale.y >> 10);
  x1 = offset.x;
  
  for( int index = 1; index < count; index++ ) {
    //���������� x
    x2 = offset.x + (index * scale.x >> 10);
    //����������� x
    if( x2 > size.x ) break;
    
    //���������� y
    y = offset.y + (values[index] * scale.y >> 10);
    if( y1 > y ) y1 = y;
    if( y2 < y ) y2 = y;
    
    //��������� ������������� ���������
    if( x1 != x2 ) {
      //��������� �����������
      if( y1 < size.y && y2 >= 0 ) {
        if( y1 < 0 ) y1 = 0;
        if( y2 > size.y ) y2 = size.y;
        ctx->VertLine( x1 + pos.x, y1 + pos.y, y2 + pos.y, color, true );
        if( x1 + 1 != x2 ) {
          if( y >= 0 && y <= size.y )
            ctx->HorzLine( x1 + pos.x, x2 + pos.x, y + pos.y, color, true );
          }
        }
      y1 = y2 = y;
      x1 = x2;
      }
    }
  }
