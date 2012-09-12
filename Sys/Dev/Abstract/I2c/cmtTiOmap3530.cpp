//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
CmtI2cMaster::CmtI2cMaster(uint32 baseAddr) :
  CmtI2cBase(),
  mBase(baseAddr) {
  
  }

#define XDR  MBIT14
#define RDR  MBIT13
#define XRDY MBIT4
#define RRDY MBIT3
#define ARDY MBIT2
#define NACK MBIT1
#define AL   MBIT0

void
CmtI2cMaster::SetupMasterAddr(uint8 adr) {
  //Адрес
  __raw_writew( adr, mBase + I2C_SA );
  }

uint_8
CmtI2cMaster::WriteBlock( cpvoid src, uint32 count, CmtEvent* pBreak ) {
  if( src == 0 || count == 0 ) return CMTE_OK;
  uint32 stat;
  const uint8 *ptr = (const uint8*)src;
  //Ожидать освобождение шины
  CmtEvClrBit32( (uint32*)(mBase + I2C_STAT), MBIT12 ).Wait2( pBreak );
  //cmtDebug4( 1, 1, "BB check result %1", stat );
  //Проверить свободность шины
  if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;
  //Очистить очереди
  __raw_setw( MBIT14 | MBIT6, mBase + I2C_BUF );
  __raw_clearw( MBIT14 | MBIT6, mBase + I2C_BUF );
  //Количество
  __raw_writew( count, mBase + I2C_CNT );
  //Программировать I2C_CON
  __raw_setw( 
        MBIT(1,10) | //MST Master/slave mode selection
                     //  0x0: Slave mode
                     //  0x1: Master mode
        MBIT(1,9) | //TRX Transmitter/Receiver mode (master mode only)
                    //  0x0: Receiver mode
                    //  0x1: Transmitter mode
        MBIT(1,1) | //STP Stop condition (master mode only). The STP bit is cleared by the module itself once it has 
                    //generated and detected the programmed stop condition on the bus.
                    //  0x0: No action or generated stop (P) condition detected on the bus (by the module)
                    //  0x1: Stop condition queried
        MBIT(1,0),  //Start condition (master mode only). The STT bit is cleared by the module itself once it has
                    //generated and detected the programmed start condition on the bus.
                    //  0x0: No action or generated start (S) condition detected (by the module)
                    //  0x1: Start condition queried
        mBase + I2C_CON
        );
  //cmtDebug4(1,2, "WriteBlock programming complete %1", __raw_readw( mBase + I2C_CON ) );
  CmtEvSetBit32 flags( (uint32*)(mBase + I2C_STAT), AL | NACK | ARDY | XRDY | XDR );
  while( 1 ) {
    flags.Wait2( pBreak );
    stat = flags.Mask();
    if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;
    //cmtDebug4(1,3, "WriteBlock %1", stat );
    if( stat == 0 || (stat & (AL | NACK)) ) {
      //Прерывание операции
      __raw_clearw( 
            MBIT(1,10) | //MST Master/slave mode selection
                         //  0x0: Slave mode
                         //  0x1: Master mode
            MBIT(1,9) | //TRX Transmitter/Receiver mode (master mode only)
                        //  0x0: Receiver mode
                        //  0x1: Transmitter mode
            MBIT(1,1) | //STP Stop condition (master mode only). The STP bit is cleared by the module itself once it has 
                        //generated and detected the programmed stop condition on the bus.
                        //  0x0: No action or generated stop (P) condition detected on the bus (by the module)
                        //  0x1: Stop condition queried
            MBIT(1,0),  //Start condition (master mode only). The STT bit is cleared by the module itself once it has
                        //generated and detected the programmed start condition on the bus.
                        //  0x0: No action or generated start (S) condition detected (by the module)
                        //  0x1: Start condition queried
            mBase + I2C_CON 
            );
      __raw_writel( AL | NACK | ARDY | XRDY | XDR,  mBase + I2C_STAT );
      return CMTE_FAIL;
      }
    if( stat & ARDY ) {
       __raw_writew( 0x7ff,  mBase + I2C_STAT );
      if( count == 0 ) {
        //cmtDebug4(1,4, "WriteBlock STAT %1", __raw_readw(mBase + I2C_STAT) );
        //cmtDebug4(1,6, "WriteBlock CON %1", __raw_readw( mBase + I2C_CON ) );
        return CMTE_OK;
        }
      }
    if( stat & XRDY ) {
      __raw_writew( *ptr++, mBase + I2C_DATA );
      count--;
      __raw_writew( XRDY,  mBase + I2C_STAT );
      }
    }
  }

uint_8
CmtI2cMaster::ReadBlock( pvoid dest, uint32 count, CmtEvent* pBreak ) {
  if( dest == 0 || count == 0 ) return CMTE_OK;
  uint32 stat;
  uint8 *ptr = (uint8*)dest;
  //Ожидать освобождение шины
  CmtEvClrBit32( (uint32*)(mBase + I2C_STAT), MBIT12 ).Wait2( pBreak );
  //Проверить свободность шины
  if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;
  //Очистить очереди
  __raw_setw( MBIT14 | MBIT6, mBase + I2C_BUF );
  __raw_clearw( MBIT14 | MBIT6, mBase + I2C_BUF );
  //Количество
  __raw_writew( count, mBase + I2C_CNT );
  //Программировать I2C_CON
  __raw_clearw(
        MBIT(1,9),  //TRX Transmitter/Receiver mode (master mode only)
                    //  0x0: Receiver mode
                    //  0x1: Transmitter mode
        mBase + I2C_CON 
        );
  __raw_setw( 
        MBIT(1,10) | //MST Master/slave mode selection
                     //  0x0: Slave mode
                     //  0x1: Master mode
        MBIT(0,9) | //TRX Transmitter/Receiver mode (master mode only)
                    //  0x0: Receiver mode
                    //  0x1: Transmitter mode
        MBIT(1,1) | //STP Stop condition (master mode only). The STP bit is cleared by the module itself once it has 
                    //generated and detected the programmed stop condition on the bus.
                    //  0x0: No action or generated stop (P) condition detected on the bus (by the module)
                    //  0x1: Stop condition queried
        MBIT(1,0),  //Start condition (master mode only). The STT bit is cleared by the module itself once it has
                    //generated and detected the programmed start condition on the bus.
                    //  0x0: No action or generated start (S) condition detected (by the module)
                    //  0x1: Start condition queried
        mBase + I2C_CON
        );
  //cmtDebug4(1,8, "ReadBlock STAT %1", __raw_readw( mBase + I2C_STAT ) );
  //cmtDebug4(1,2, "ReadBlock programming complete %1", __raw_readw( mBase + I2C_CON ) );
  CmtEvSetBit32 flags( (uint32*)(mBase + I2C_STAT), AL | NACK | ARDY | RRDY );
  while( 1 ) {
    flags.Wait2( pBreak );
    stat = flags.Mask();
    if( pBreak && pBreak->IsSignaled() ) return CMTE_BREAK;
    //cmtDebug4(1,3, "ReadBlock %1", __raw_readw( mBase + I2C_STAT ) );
    if( stat == 0 || (stat & (AL | NACK)) ) {
      //Прерывание операции
      __raw_clearw( 
            MBIT(1,10) | //MST Master/slave mode selection
                         //  0x0: Slave mode
                         //  0x1: Master mode
            MBIT(1,9) | //TRX Transmitter/Receiver mode (master mode only)
                        //  0x0: Receiver mode
                        //  0x1: Transmitter mode
            MBIT(1,1) | //STP Stop condition (master mode only). The STP bit is cleared by the module itself once it has 
                        //generated and detected the programmed stop condition on the bus.
                        //  0x0: No action or generated stop (P) condition detected on the bus (by the module)
                        //  0x1: Stop condition queried
            MBIT(1,0),  //Start condition (master mode only). The STT bit is cleared by the module itself once it has
                        //generated and detected the programmed start condition on the bus.
                        //  0x0: No action or generated start (S) condition detected (by the module)
                        //  0x1: Start condition queried
            mBase + I2C_CON 
            );
      __raw_writew( AL | NACK | ARDY | RRDY | RDR,  mBase + I2C_STAT );
      return CMTE_FAIL;
      }
    if( stat & ARDY ) {
       __raw_writew( 0x7ff,  mBase + I2C_STAT );
      if( count == 0 ) {
        //cmtDebug4(1,4, "ReadBlock %1", __raw_readw(mBase + I2C_STAT) );
        //cmtWaitTick(100);
        //cmtDebug4(1,5, "ReadBlock %1", __raw_readw(mBase + I2C_STAT) );
        return CMTE_OK;
        }
      }
    if( stat & RRDY ) {
      *ptr++ = __raw_readw( mBase + I2C_DATA ) & 0xff;
      count--;
      __raw_writew( RRDY,  mBase + I2C_STAT );
      }
    }
  
  }
