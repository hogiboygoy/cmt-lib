//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   Timers, WatchDog timers
//==============================================================================
#ifndef TI_TIMERS_H
#define TI_TIMERS_H

/* General Purpose Timers */
#define GPTIMER1_BASE         0x48318000
#define GPTIMER2_BASE         0x49032000
#define GPTIMER3_BASE         0x49034000
#define GPTIMER4_BASE         0x49036000
#define GPTIMER5_BASE         0x49038000
#define GPTIMER6_BASE         0x4903A000
#define GPTIMER7_BASE         0x4903C000
#define GPTIMER8_BASE         0x4903E000
#define GPTIMER9_BASE         0x49040000
#define GPTIMER10_BASE        0x48086000
#define GPTIMER11_BASE        0x48088000
//#define GPTIMER12_BASE        0x48304000

/* WatchDog Timers (1 secure, 3 GP) */
#define WD1_BASE              0x4830C000
#define WD2_BASE              0x48314000
#define WD3_BASE              0x49030000

/* timer regs offsets (32 bit regs) */
#define TIDR           0x0      /* r */
#define TIOCP_CFG      0x10     /* rw */
#define TISTAT         0x14     /* r */
#define TISR           0x18     /* rw */
#define TIER           0x1C     /* rw */
#define TWER           0x20     /* rw */
#define TCLR           0x24     /* rw */
#define TCRR           0x28     /* rw */
#define TLDR           0x2C     /* rw */
#define TTGR           0x30     /* rw */
#define TWPS           0x34     /* r */
#define TMAR           0x38     /* rw */
#define TCAR1          0x3c     /* r */
#define TSICR          0x40     /* rw */
#define TCAR2          0x44     /* r */
#define GPT_EN         ((0<<2)|BIT1|BIT0) /* enable sys_clk NO-prescale /1 */

/* Watchdog */
#define WWPS           0x34     /* r */
#define WSPR           0x48     /* rw */
#define WD_UNLOCK1     0xAAAA
#define WD_UNLOCK2     0x5555

/* 32KTIMER */
#define SYNC_32KTIMER_BASE    0x48320000

void GPTIMER_Clock( uint32 gptimer, bool enable );


void WD_Clock( uint32 wd, bool enable );

void WD_Disable( uint32 wdBase );

void WD_Reset( uint32 wdBase );


#endif

