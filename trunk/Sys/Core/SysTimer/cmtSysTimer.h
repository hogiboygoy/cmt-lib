//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

/** \addtogroup core
 *  @{
 */

//! �������� �������� �������� (�������� 3 ���).

//! \param tick ������ ��������
void cmtWaitTick( uint32 tick );


//����������
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

                                                                
//! ��������� ������ ������� ��� ������������ �������� ��������.

//! \param var ���������� �������� uint32
//! \param val ��������� ��������
#define cmtSetCheckTick( var, val ) var = val + cmtTickCount


//! �������� ������ ������� ����� ���������� ������.

//! \param var ���������� �������� uint32
#define cmtCheckTick( var ) (cmtTickCount - var < 0x10000000)



//! �������� ������ ������� ����� ��������� �� ���������� ������.

//! \param ptr ��������� �� ���������� �������� uint32
#define cmtCheckTickPtr( ptr ) (cmtTickCount - *(uint32*)ptr < 0x10000000)

//! �������, ������������ ��������� ��������
struct CmtEvTick : public CmtEvent {
    uint32 mTime; //!< ��������� ����� � ��

    //! �����������. ����� ���������� ��������� �����
    CmtEvTick( uint32 time ) : CmtEvent(), mTime( cmtTickCount + time ) {}

    //! ��������� �������
    virtual bool IsSignaled() { return cmtCheckTick( mTime ); }

    //! ������������� ����� ��������� ��������
    void Set( uint32 time ) { mTime = cmtTickCount + time; }

    //! ������������� ��������� ��������� ��������
    void Add( uint32 time ) { mTime += time; }
  };


/**@}*/

