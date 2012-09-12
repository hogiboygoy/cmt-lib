//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

/** \addtogroup core
 *  @{
 */

//! ќжидание заданный интервал (максимум 3 дн€).

//! \param tick размер задержки
void cmtWaitTick( uint32 tick );


//–еализации
#if defined(CMT_LPC1768)
  #include <Sys/Core/SysTimer/cmtLPC1768.h>

#elif defined(CMT_ATMEGA32)
  #include <Sys/Core/SysTimer/cmtAtMega32.h>

#elif defined(CMT_STM32F103CBT6) || defined(CMT_STM32F105VCT6) || defined(CMT_STM32F100C8T)
  #include <Sys/Core/SysTimer/cmtStm32f10x.h>

#elif defined(CMT_STM8L151K4)
  #include <Sys/Core/SysTimer/cmtStm8l15x.h>

#elif defined(CMT_TI_OMAP3530)
  #include <Sys/Core/SysTimer/cmtTiOMAP3530.h>

#elif defined(CMT_DESCTOP_QT)
#else
  #error "CMT:No system timer defined for this CPU"
#endif

                                                                
//! ”становка порога времени дл€ последующего контрол€ таймером.

//! \param var переменна€ контрол€ uint32
//! \param val пороговое значение
#define cmtSetCheckTick( var, val ) var = val + cmtTickCount


//!  онтроль порога времени через переменную порога.

//! \param var переменна€ контрол€ uint32
#define cmtCheckTick( var ) (cmtTickCount - var < 0x10000000)



//!  онтроль порога времени через указатель на переменную порога.

//! \param ptr указатель на переменную контрол€ uint32
#define cmtCheckTickPtr( ptr ) (cmtTickCount - *(uint32*)ptr < 0x10000000)

//! —обытие, определ€ющее временной интервал
struct CmtEvTick : public CmtEvent {
    uint32 mTime; //!< ѕороговое врем€ в мс

    //!  онструктор. —разу определ€ет пороговое врем€
    CmtEvTick( uint32 time ) : CmtEvent(), mTime( cmtTickCount + time ) {}

    //! —осто€ние событи€
    virtual bool IsSignaled() { return cmtCheckTick( mTime ); }

    //! ”станавливает новый временной интервал
    void Set( uint32 time ) { mTime = cmtTickCount + time; }

    //! ”станавливает следующий временной интервал
    void Add( uint32 time ) { mTime += time; }
  };


/**@}*/

