//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================
struct CmtUartBase : public CmtSerial {
  CmtUartBase( CmtTransfer *rx = 0, CmtTransfer *tx = 0 ) : CmtSerial( rx, tx ) {}
  
  };

//Аппаратные реализации
#if defined( CMT_STM32F105VCT6 ) || defined( CMT_STM32F103CBT6 )
  #include <Sys/Dev/Abstract/Uart/cmtSTM32F10x.h>
#else
#error "CMT: UART not supported for this CPU"
#endif
