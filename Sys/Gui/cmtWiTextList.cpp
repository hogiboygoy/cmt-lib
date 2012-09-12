//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Gui/cmtWidjetSystem.h>

CmtWiTextList::CmtWiTextList( CmtContext *cash, uint32 color ) :
  CmtWiTextListBase( cash, color ),
  mBuffer( 1024 ),
  mDelta( 2048 ),
  mPos(0),
  mStringCount(0),
  mAddEnable(true ) {
  
  }

void
CmtWiTextList::Add(cpchar str) {
  if( str == 0 || str[0] == 0 ) return;
  //Длина добавляемой строки
  uint32 len = strlen(str) + 1;
  //Проверить влезет ли добавляемая строка в буфер
  if( len + mPos > mBuffer.Size() ) 
    if( !mBuffer.Resize( mBuffer.Size() + mDelta + len ) ) return;
  //Копировать строку в буфер
  strcpy( mBuffer.Str() + mPos, str );
  //Обновить позицию в буфере для новых строк
  mPos += len;
  //Инкремент счетчика строк
  mStringCount++;
  }

void
CmtWiTextList::SetupTextListParam(CmtPoint cellSize, uint32 gap, uint32 curCellPos) {
  if( mAddEnable ) {
    //Добавление разрешено, инициализировать список отображения
    Alloc( mStringCount + 1 );
    //Установить указатели на строки буфера
    uint32 k = 0;
    for( uint32 i = 0; i < mPos; i++ ) {
      //Указатель на добавляемую строку
      cpchar ptr = (cpchar)(mBuffer.Buffer() + i);
      //Добавить строку
      SetCellText( k++, ptr ) ;
      i += strlen( ptr );
      }
    //Последняя строка нулевая
    SetCellText( mStringCount, 0 );
    //Все строки добавлены, установить параметры
    SetupTextPtrParam( cellSize, gap, curCellPos );
    //Заблокировать последующее добавление строк
    mAddEnable = false;
    }
  }
