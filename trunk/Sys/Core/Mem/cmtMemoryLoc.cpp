//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <string.h>

#ifndef CMT_MEMORY_HEAP
uint32  cmtHeap[CMT_MEMORY_LOC];
#endif

void
cmtInitHeap() {
  cmtHeap[0] = (CMT_MEMORY_LOC - 1) << 1;
  }

pvoid
cmtMalloc( uint32 size ) {
  uint32 i = 0;
  size = (size + 3) >> 2; //Переходим к размеру, выраженному в словах
  //Поиск подходящего размера
  do {
    uint32 header = cmtHeap[i];
    if( (header & 1) == 0 && (header >> 1) > size ) {
      uint32 *ptr = cmtHeap + i + 1;
      //Блок подходит, выделить
      cmtHeap[i] = 1 | (size << 1);
      //Оформить следующий блок
      i += 1 + size;
      if( i < CMT_MEMORY_LOC && (header >> 1) > 1 + size )
        cmtHeap[i] = ( (header >> 1) - size - 1) << 1;
      return ptr;
      }
    //Блок не подходит, перейти к следующему
    i += 1 + (header >> 1);
    }
  while( i < CMT_MEMORY_LOC );
  //Подходящий блок не найден, вернуть 0
  return 0;
  }

pvoid
cmtCalloc( uint32 count, uint32 size ) {
  size *= count;
  pvoid ptr = cmtMalloc( size );
  if( ptr ) memset( ptr, 0, size );
  return ptr;
  }

void
cmtFree( pvoid ptr ) {
  uint32 header;
  if( ptr ) {
    uint32 *wptr = (uint32*)ptr;
    uint32 i = (wptr - cmtHeap); //Перейдем к словам
    uint32 k;
    if( i < CMT_MEMORY_LOC && i >= 1 ) {
      //Указатель внутри кучи
      i--; //Указатель указывает на заголовок
      //Проверить следующий фрагмент
      header = (cmtHeap[i] >> 1) + 1 + i;
      if( header < CMT_MEMORY_LOC ) {
        if( (cmtHeap[header] & 1) == 0 ) {
          //Следующий фрагмент свободен, приплюсовать к текущему
          cmtHeap[i] = ((cmtHeap[i] >> 1) + 1 + (cmtHeap[header] >> 1)) << 1;
          }
        }
      //Освободить текущий фрагмент
      cmtHeap[i] &= 0xfffffffe;
      for( k = 0; k < i; ) {
        header = cmtHeap[k];
        if( (header & 1) == 0 && ((header >> 1) + 1 + k) == i ) {
          //Обнаружен предыдущий блок, свободный, свяжем с текущим
          cmtHeap[k] = ((header >> 1) + 1 + (cmtHeap[i] >> 1)) << 1;
          }
        k += (cmtHeap[k] >> 1) + 1;
        }
      }
    }
  }

#ifdef __cplusplus
void* operator new ( size_t size ) {
  return cmtMalloc( size );
  }

void operator delete ( void *ptr ) {
  cmtFree( ptr );
  }
#endif
 
