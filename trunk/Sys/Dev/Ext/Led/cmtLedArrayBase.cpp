//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#include <Sys/Dev/Ext/Led/cmtLed.h>

void
CmtLedArrayBase::Loop() {
  //Выключить светодиоды
  SetRow(0);
  //Установить новую колонку
  SetColon( mColonIndex );
  //Включить соответствующие светодиоды
  SetRow( mRows[mColonIndex++] );
  //Проверить заворачивание индекса
  if( mColonIndex >= mColonCount )
    mColonIndex = 0;
  }

void 
cmtLedArrayTask() {
  CmtLedArrayBase *ledArray = (CmtLedArrayBase*)(cmtTaskParam());
  while(1) {
    //Выполнить цикл
    ledArray->Loop();
    //Ожидание периода
    cmtWaitTick(5);
    }
  }
