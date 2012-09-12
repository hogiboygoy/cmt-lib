/*
 * (C) Copyright 2006
 * Texas Instruments, <www.ti.com>
 * Richard Woodruff <r-woodruff2@ti.com>
 * Syed Mohammed Khasim <x0khasim@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _OMAP3_SYS_H_
#define _OMAP3_SYS_H_

#include <Sys/Core/cmtSizes.h>

/*
 * OMAP3 specific Section
 */
/* Stuff on L3 Interconnect */
#define SMX_APE_BASE         0x68000000

/* GPMC */
#define OMAP34XX_GPMC_BASE      0x6E000000

/* SMS */
#define OMAP34XX_SMS_BASE      0x6C000000

/* SDRC */
#define OMAP34XX_SDRC_BASE      0x6D000000

/*
 * L4 Peripherals - L4 Wakeup and L4 Core now
 */
#define OMAP34XX_CORE_L4_IO_BASE   0x48000000
#define OMAP34XX_WAKEUP_L4_IO_BASE   0x48300000
#define OMAP34XX_ID_L4_IO_BASE      0x4830A200
#define OMAP34XX_L4_PER         0x49000000
#define OMAP34XX_L4_IO_BASE      OMAP34XX_CORE_L4_IO_BASE

/* CONTROL */
#define OMAP34XX_CTRL_BASE      (OMAP34XX_L4_IO_BASE + 0x2000)

/* UART */
#define OMAP34XX_UART1         (OMAP34XX_L4_IO_BASE + 0x6a000)
#define OMAP34XX_UART2         (OMAP34XX_L4_IO_BASE + 0x6c000)
#define OMAP34XX_UART3         (OMAP34XX_L4_PER + 0x20000)

/* General Purpose Timers */
#define OMAP34XX_GPT1         0x48318000
#define OMAP34XX_GPT2         0x49032000
#define OMAP34XX_GPT3         0x49034000
#define OMAP34XX_GPT4         0x49036000
#define OMAP34XX_GPT5         0x49038000
#define OMAP34XX_GPT6         0x4903A000
#define OMAP34XX_GPT7         0x4903C000
#define OMAP34XX_GPT8         0x4903E000
#define OMAP34XX_GPT9         0x49040000
#define OMAP34XX_GPT10         0x48086000
#define OMAP34XX_GPT11         0x48088000
#define OMAP34XX_GPT12         0x48304000

/* WatchDog Timers (1 secure, 3 GP) */
#define WD1_BASE         0x4830C000
#define WD2_BASE         0x48314000
#define WD3_BASE         0x49030000

/* 32KTIMER */
#define SYNC_32KTIMER_BASE      0x48320000

#ifndef __ASSEMBLY__

struct s32ktimer {
   unsigned char res[0x10];
   unsigned int s32k_cr;      /* 0x10 */
};

#endif /* __ASSEMBLY__ */



/* base address for indirect vectors (internal boot mode) */
#define SRAM_OFFSET0         0x40000000
#define SRAM_OFFSET1         0x00200000
#define SRAM_OFFSET2         0x0000F800
#define SRAM_VECT_CODE         (SRAM_OFFSET0 | SRAM_OFFSET1 | \
                SRAM_OFFSET2)

#define LOW_LEVEL_SRAM_STACK      0x4020FFFC

#define DEBUG_LED1         149   /* gpio */
#define DEBUG_LED2         150   /* gpio */

#define XDR_POP      5   /* package on package part */
#define SDR_DISCRETE   4   /* 128M memory SDR module */
#define DDR_STACKED   3   /* stacked part on 2422 */
#define DDR_COMBO   2   /* combo part on cpu daughter card */
#define DDR_DISCRETE   1   /* 2x16 parts on daughter card */

#define DDR_100      100   /* type found on most mem d-boards */
#define DDR_111      111   /* some combo parts */
#define DDR_133      133   /* most combo, some mem d-boards */
#define DDR_165      165   /* future parts */

/*
 * 343x real hardware:
 *  ES1     = rev 0
 *
 *  ES2 onwards, the value maps to contents of IDCODE register [31:28].
 *
 * Note : CPU_3XX_ES20 is used in cache.S.  Please review before changing.
 */
#define CPU_3XX_ES10      0
#define CPU_3XX_ES20      1
#define CPU_3XX_ES21      2
#define CPU_3XX_ES30      3
#define CPU_3XX_ES31      4
#define CPU_3XX_MAX_REV      (CPU_3XX_ES31 + 1)

#define CPU_3XX_ID_SHIFT   28

#define WIDTH_8BIT      0x0000
#define WIDTH_16BIT      0x1000   /* bit pos for 16 bit in gpmc */

/*
 * Hawkeye values
 */
#define HAWKEYE_OMAP34XX   0xb7ae
#define HAWKEYE_AM35XX      0xb868
#define HAWKEYE_OMAP36XX   0xb891

#define HAWKEYE_SHIFT      12

/*
 * Define CPU families
 */
#define CPU_OMAP34XX      0x3400   /* OMAP34xx/OMAP35 devices */
#define CPU_AM35XX      0x3500   /* AM35xx devices          */
#define CPU_OMAP36XX      0x3600   /* OMAP36xx devices        */

/*
 * Define CPUs
 */
#define OMAP3430      0x3430

#define OMAP3503      0x3503
#define OMAP3515      0x3515
#define OMAP3525      0x3525
#define OMAP3530      0x3530

#define AM3505         0x3505
#define AM3517         0x3517

#define OMAP3630      0x3630
#define OMAP3730      0x3730
/*
 * Control status register values corresponding to cpu variants
 */
#define CTRL_OMAP3503      0x5c00
#define CTRL_OMAP3515      0x1c00
#define CTRL_OMAP3525      0x4c00
#define CTRL_OMAP3530      0x0c00

#define CTRL_AM3505      0x5c00
#define CTRL_AM3517      0x1c00

#define CTRL_OMAP3730      0x0c00

/* Stuff on L3 Interconnect */
#define SMX_APE_BASE			0x68000000

/* L3 Firewall */
#define A_REQINFOPERM0		(SMX_APE_BASE + 0x05048)
#define A_READPERM0		(SMX_APE_BASE + 0x05050)
#define A_WRITEPERM0		(SMX_APE_BASE + 0x05058)

/* SMS */
#define OMAP34XX_SMS_BASE		0x6C000000

/* SDRC */
#define OMAP34XX_SDRC_BASE		0x6D000000

/*
 * L4 Peripherals - L4 Wakeup and L4 Core now
 */
#define OMAP34XX_CORE_L4_IO_BASE	0x48000000

#define OMAP34XX_WAKEUP_L4_IO_BASE	0x48300000

#define OMAP34XX_L4_PER			0x49000000

#define OMAP34XX_L4_IO_BASE		OMAP34XX_CORE_L4_IO_BASE

/* TAP information  dont know for 3430*/
#define OMAP34XX_TAP_BASE		(0x49000000) /*giving some junk for virtio */

/* General Purpose Timers */
#define OMAP34XX_GPT1			0x48318000
#define OMAP34XX_GPT2			0x49032000
#define OMAP34XX_GPT3			0x49034000
#define OMAP34XX_GPT4			0x49036000
#define OMAP34XX_GPT5			0x49038000
#define OMAP34XX_GPT6			0x4903A000
#define OMAP34XX_GPT7			0x4903C000
#define OMAP34XX_GPT8			0x4903E000
#define OMAP34XX_GPT9			0x49040000
#define OMAP34XX_GPT10			0x48086000
#define OMAP34XX_GPT11			0x48088000
#define OMAP34XX_GPT12			0x48304000

/* 32KTIMER */
#define S32K_CR				(SYNC_32KTIMER_BASE+0x10)

/*
 * SDP3430 specific Section
 */

/*
 *  The 343x's chip selects are programmable.  The mask ROM
 *  does configure CS0 to 0x08000000 before dispatch.  So, if
 *  you want your code to live below that address, you have to
 *  be prepared to jump though hoops, to reset the base address.
 *  Same as in SDP3430
 */
/* base address for indirect vectors (internal boot mode) */
#define SRAM_OFFSET0			0x40000000
#define SRAM_OFFSET1			0x00200000
#define SRAM_OFFSET2			0x0000F800

#define LOW_LEVEL_SRAM_STACK		0x4020FFFC

/*
 * TODO: Are they required for AM3517?
 */
#if defined(CONFIG_3430SDP) || defined(CONFIG_OMAP3EVM)
/* FPGA on Debug board.*/
#define ETH_CONTROL_REG			(DEBUG_BASE+0x30b)
#define LAN_RESET_REGISTER		(DEBUG_BASE+0x1c)

#define DIP_SWITCH_INPUT_REG2		(DEBUG_BASE+0x60)
#define LED_REGISTER			(DEBUG_BASE+0x40)
#define FPGA_REV_REGISTER		(DEBUG_BASE+0x10)
#define EEPROM_MAIN_BRD			(DEBUG_BASE+0x10000+0x1800)
#define EEPROM_CONN_BRD			(DEBUG_BASE+0x10000+0x1900)
#define EEPROM_UI_BRD			(DEBUG_BASE+0x10000+0x1A00)
#define EEPROM_MCAM_BRD			(DEBUG_BASE+0x10000+0x1B00)
#define ENHANCED_UI_EE_NAME		"750-2075"
#endif



#endif  /* _OMAP3430_SYS_H_ */
