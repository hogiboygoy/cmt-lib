//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
#ifndef CMT_LED_H
#define CMT_LED_H

class CmtLedArrayBase {
    uint8  mColonCount;
    uint8  mColonIndex;
    uint8 *mRows;
  public:
    CmtLedArrayBase( uint8 colonCount, uint8 *rows ) : mColonCount(colonCount), mColonIndex(0), mRows(rows) {}
    
    virtual void Loop();
    
    virtual void SetRow( uint8 segmentMask ) = 0;
    
    virtual void SetColon( uint8 colon ) = 0;
  };

void cmtLedArrayTask();

//CMT_TASK_ADD( cmtLedArrayTask, new CmtLedArrayBase, 200 );

#endif
