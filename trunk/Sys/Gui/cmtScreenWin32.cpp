//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//! Рисование завершено можно произвести смену фрейма
void
CmtScreenWin32::FrameComplete() {
  //Объявить экран недействительным
  InvalidateRect( hWnd, 0, FALSE );
  }


void
CmtScreenWin32::PaintDC(HDC hdc) {
  BITMAPINFO bmi;
  bmi.bmiHeader.biSize   = sizeof(bmi);
  bmi.bmiHeader.biWidth  = mSize.x;
  bmi.bmiHeader.biHeight = - mSize.y;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage   = mSize.x * mSize.y * 4;
  bmi.bmiHeader.biXPelsPerMeter = 0;
  bmi.bmiHeader.biYPelsPerMeter = 0;
  bmi.bmiHeader.biClrUsed = 0;
  bmi.bmiHeader.biClrImportant = 0;
  //Прямое копирование в контекст
  SetDIBitsToDevice( hdc, 0, 0, mSize.x, mSize.y,
    0, 0, 0, mSize.y, GetData(), &bmi, DIB_RGB_COLORS );
  }
