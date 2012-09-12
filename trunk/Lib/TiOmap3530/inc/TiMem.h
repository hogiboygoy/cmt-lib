//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   Memory Subsistem
//==============================================================================
#ifndef TI_MEM_H
#define TI_MEM_H

/* GPMC CS3/cs4/cs6 not avaliable */
#define GPMC_BASE             (OMAP34XX_GPMC_BASE)
#define GPMC_SYSCONFIG        (OMAP34XX_GPMC_BASE+0x10)
#define GPMC_SYSSTATUS        (OMAP34XX_GPMC_BASE+0x14)
#define GPMC_IRQSTATUS        (OMAP34XX_GPMC_BASE+0x18)
#define GPMC_IRQENABLE        (OMAP34XX_GPMC_BASE+0x1C)
#define GPMC_TIMEOUT_CONTROL  (OMAP34XX_GPMC_BASE+0x40)
#define GPMC_CONFIG           (OMAP34XX_GPMC_BASE+0x50)
#define GPMC_STATUS           (OMAP34XX_GPMC_BASE+0x54)

#define GPMC_CONFIG_CS0       (OMAP34XX_GPMC_BASE+0x60)

#define GPMC_CONFIG_WIDTH     (0x30)

#define GPMC_CONFIG1          (0x00)
#define GPMC_CONFIG2          (0x04)
#define GPMC_CONFIG3          (0x08)
#define GPMC_CONFIG4          (0x0C)
#define GPMC_CONFIG5          (0x10)
#define GPMC_CONFIG6          (0x14)
#define GPMC_CONFIG7          (0x18)
#define GPMC_NAND_CMD         (0x1C)
#define GPMC_NAND_ADR         (0x20)
#define GPMC_NAND_DAT         (0x24)

#define GPMC_CONFIG_CS( a, i )   (a + GPMC_CONFIG_WIDTH * i + GPMC_CONFIG_CS0)


#define GPMC_ECC_CONFIG       (0x1F4)
#define GPMC_ECC_CONTROL      (0x1F8)
#define GPMC_ECC_SIZE_CONFIG  (0x1FC)
#define GPMC_ECC1_RESULT      (0x200)
#define GPMC_ECC2_RESULT      (0x204)
#define GPMC_ECC3_RESULT      (0x208)
#define GPMC_ECC4_RESULT      (0x20C)
#define GPMC_ECC5_RESULT      (0x210)
#define GPMC_ECC6_RESULT      (0x214)
#define GPMC_ECC7_RESULT      (0x218)
#define GPMC_ECC8_RESULT      (0x21C)
#define GPMC_ECC9_RESULT      (0x220)


/* GPMC Mapping */
# define FLASH_BASE            0x10000000  /* NOR flash (aligned to 256 Meg) */
# define FLASH_BASE_SDPV1      0x04000000  /* NOR flash (aligned to 64 Meg) */
# define FLASH_BASE_SDPV2      0x10000000  /* NOR flash (aligned to 256 Meg) */
# define DEBUG_BASE            0x08000000  /* debug board */
# define NAND_BASE             0x30000000  /* NAND addr (actual size small port)*/
# define PISMO2_BASE           0x18000000  /* PISMO2 CS1/2 */
# define ONENAND_MAP           0x20000000  /* OneNand addr (actual size small port */

/* SMS */
#define SMS_SYSCONFIG          (OMAP34XX_SMS_BASE+0x10)
#define SMS_RG_ATT0            (OMAP34XX_SMS_BASE+0x48)
#define SMS_CLASS_ARB0         (OMAP34XX_SMS_BASE+0xD0)
#define SMS_ROT_CONTROL(n)     (OMAP34XX_SMS_BASE+0x180+0x10*(n))
#define SMS_ROT_SIZE(n)        (OMAP34XX_SMS_BASE+0x184+0x10*(n))
#define SMS_ROT_PHYSICAL_BA(n) (OMAP34XX_SMS_BASE+0x188+0x10*(n))
#define BURSTCOMPLETE_GROUP7   MBIT31

/* SDRC */
#define SDRC_SYSCONFIG        (OMAP34XX_SDRC_BASE+0x10)
#define SDRC_STATUS           (OMAP34XX_SDRC_BASE+0x14)
#define SDRC_CS_CFG           (OMAP34XX_SDRC_BASE+0x40)
#define SDRC_SHARING          (OMAP34XX_SDRC_BASE+0x44)
#define SDRC_DLLA_CTRL        (OMAP34XX_SDRC_BASE+0x60)
#define SDRC_DLLA_STATUS      (OMAP34XX_SDRC_BASE+0x64)
#define SDRC_DLLB_CTRL        (OMAP34XX_SDRC_BASE+0x68)
#define SDRC_DLLB_STATUS      (OMAP34XX_SDRC_BASE+0x6C)
#define SDRC_EMR2_0           (OMAP34XX_SDRC_BASE+0x8C)
#define SDRC_EMR2_1           (OMAP34XX_SDRC_BASE+0x8C+0x30)
#define DLLPHASE              MBIT1
#define LOADDLL               MBIT2
#define DLL_DELAY_MASK        0xFF00
#define DLL_NO_FILTER_MASK    (MBIT8|MBIT9)

#define SDRC_POWER            (OMAP34XX_SDRC_BASE+0x70)
#define WAKEUPPROC            MBIT26

#define SDRC_MCFG_0          (OMAP34XX_SDRC_BASE+0x80)
#define SDRC_MCFG_1          (OMAP34XX_SDRC_BASE+0x80+0x30)
#define SDRC_MR_0            (OMAP34XX_SDRC_BASE+0x84)
#define SDRC_ACTIM_CTRLA_0   (OMAP34XX_SDRC_BASE+0x9C)
#define SDRC_ACTIM_CTRLB_0   (OMAP34XX_SDRC_BASE+0xA0)
#define SDRC_ACTIM_CTRLA_1   (OMAP34XX_SDRC_BASE+0xC4)
#define SDRC_ACTIM_CTRLB_1   (OMAP34XX_SDRC_BASE+0xC8)
#define SDRC_RFR_CTRL_0      (OMAP34XX_SDRC_BASE+0xA4)
#define SDRC_RFR_CTRL_1      (OMAP34XX_SDRC_BASE+0xA4+0x30)
#define SDRC_MANUAL_0        (OMAP34XX_SDRC_BASE+0xA8)
#define OMAP34XX_SDRC_CS0    0x80000000
#define OMAP34XX_SDRC_CS1    0xA0000000
#define CMD_NOP              0x0
#define CMD_PRECHARGE        0x1
#define CMD_AUTOREFRESH      0x2
#define CMD_ENTR_PWRDOWN     0x3
#define CMD_EXIT_PWRDOWN     0x4
#define CMD_ENTR_SRFRSH      0x5
#define CMD_CKE_HIGH         0x6
#define CMD_CKE_LOW          0x7
#define SOFTRESET            MBIT1
#define SMART_IDLE           (0x2 << 3)
#define REF_ON_IDLE          (0x1 << 6)

/* EMIF4 register */
#define   EMIF4_BASE      OMAP34XX_SDRC_BASE
#define   EMIF4_MOD_ID      (EMIF4_BASE + 0x00)
#define   EMIF4_SDRAM_STS      (EMIF4_BASE + 0x04)
#define   EMIF4_SDRAM_CFG      (EMIF4_BASE + 0x08)
#define   EMIF4_SDRAM_RFCR   (EMIF4_BASE + 0x10)
#define   EMIF4_SDRAM_RFCR_SHDW   (EMIF4_BASE + 0x14)
#define   EMIF4_SDRAM_TIM1   (EMIF4_BASE + 0x18)
#define   EMIF4_SDRAM_TIM1_SHDW   (EMIF4_BASE + 0x1C)
#define   EMIF4_SDRAM_TIM2   (EMIF4_BASE + 0x20)
#define   EMIF4_SDRAM_TIM2_SHDW   (EMIF4_BASE + 0x24)
#define   EMIF4_SDRAM_TIM3   (EMIF4_BASE + 0x28)
#define   EMIF4_SDRAM_TIM3_SHDW   (EMIF4_BASE + 0x2c)
#define   EMIF4_PWR_MGT_CTRL   (EMIF4_BASE + 0x38)
#define   EMIF4_PWR_MGT_CTRL_SHDW   (EMIF4_BASE + 0x3C)
#define   EMIF4_IODFT_TLGC   (EMIF4_BASE + 0x60)
#define   EMIF4_DDR_PHYCTL1   (EMIF4_BASE + 0xE4)
#define   EMIF4_DDR_PHYCTL1_SHDW   (EMIF4_BASE + 0xE8)
#define   EMIF4_DDR_PHYCTL2   (EMIF4_BASE + 0xEC)

//------------------------------------------------------------------------------

#define GPMC_IRQSTATUS_FIFOEVENT        (1 << 0)
#define GPMC_IRQSTATUS_TERMINALCOUNT    (1 << 1)
#define GPMC_IRQSTATUS_WAIT0_EDGEDETECT (1 << 8)
#define GPMC_IRQSTATUS_WAIT1_EDGEDETECT (1 << 9)
#define GPMC_IRQSTATUS_WAIT2_EDGEDETECT (1 << 10)
#define GPMC_IRQSTATUS_WAIT3_EDGEDETECT (1 << 11)

#define GPMC_IRQENABLE_FIFOEVENT        (1 << 0)
#define GPMC_IRQENABLE_TERMINALCOUNT    (1 << 1)
#define GPMC_IRQENABLE_WAIT0_EDGEDETECT (1 << 8)
#define GPMC_IRQENABLE_WAIT1_EDGEDETECT (1 << 9)
#define GPMC_IRQENABLE_WAIT2_EDGEDETECT (1 << 10)
#define GPMC_IRQENABLE_WAIT3_EDGEDETECT (1 << 11)

#define GPMC_CONFIG_NANDFORCEPOSTEDWRITE (1 << 0)
#define GPMC_CONFIG_LIMITEDADDRESS      (1 << 1)
#define GPMC_CONFIG_WRITEPROTECT        (1 << 4)
#define GPMC_CONFIG_WAIT0PINPOLARITY    (1 << 8)
#define GPMC_CONFIG_WAIT1PINPOLARITY    (1 << 9)
#define GPMC_CONFIG_WAIT2PINPOLARITY    (1 << 10)
#define GPMC_CONFIG_WAIT3PINPOLARITY    (1 << 11)

#define GPMC_STATUS_EMPTYWRITEBUFFER    (1 << 0)
#define GPMC_STATUS_WAIT0_ASSERTED      (1 << 8)
#define GPMC_STATUS_WAIT1_ASSERTED      (1 << 9)
#define GPMC_STATUS_WAIT2_ASSERTED      (1 << 10)
#define GPMC_STATUS_WAIT3_ASSERTED      (1 << 11)

#define GPMC_ECC_CONFIG_ENABLE          (1 << 0)
#define GPMC_ECC_CONFIG_CS0             (0 << 1)
#define GPMC_ECC_CONFIG_CS1             (1 << 1)
#define GPMC_ECC_CONFIG_CS2             (2 << 1)
#define GPMC_ECC_CONFIG_CS3             (3 << 1)
#define GPMC_ECC_CONFIG_CS4             (4 << 1)
#define GPMC_ECC_CONFIG_CS5             (5 << 1)
#define GPMC_ECC_CONFIG_CS6             (6 << 1)
#define GPMC_ECC_CONFIG_CS7             (7 << 1)
#define GPMC_ECC_CONFIG_8BIT            (0 << 7)
#define GPMC_ECC_CONFIG_16BIT           (1 << 7)
#define GPMC_ECC_CONFIG_BCH4            (0 << 12)
#define GPMC_ECC_CONFIG_BCH8            (1 << 12)
#define GPMC_ECC_CONFIG_HAMMING     (0 << 16)
#define GPMC_ECC_CONFIG_BCH              (1 << 16)


#define GPMC_ECC_CONTROL_POINTER1       (1 << 0)
#define GPMC_ECC_CONTROL_POINTER2       (2 << 0)
#define GPMC_ECC_CONTROL_POINTER3       (3 << 0)
#define GPMC_ECC_CONTROL_POINTER4       (4 << 0)
#define GPMC_ECC_CONTROL_POINTER5       (5 << 0)
#define GPMC_ECC_CONTROL_POINTER6       (6 << 0)
#define GPMC_ECC_CONTROL_POINTER7       (7 << 0)
#define GPMC_ECC_CONTROL_POINTER8       (8 << 0)
#define GPMC_ECC_CONTROL_POINTER9       (9 << 0)
#define GPMC_ECC_CONTROL_CLEAR          (1 << 8)

#define GPMC_PREFETCH_CONFIG_WRITEPOST              (1 << 0)
#define GPMC_PREFETCH_CONFIG_DMAMODE                (1 << 2)
#define GPMC_PREFETCH_CONFIG_SYNCHROMODE            (1 << 3)
#define GPMC_PREFETCH_CONFIG_ENABLEENGINE           (1 << 7)
#define GPMC_PREFETCH_CONFIG_PFPWENROUNDROBIN       (1 << 23)
#define GPMC_PREFETCH_CONFIG_ENABLEOPTIMIZEDACCESS  (1 << 27)
#define GPMC_PREFETCH_CONFIG_WAITPINSELECTOR_SHIFT  (4)
#define GPMC_PREFETCH_CONFIG_WAITPINSELECTOR_MASK   (0x3 << GPMC_PREFETCH_CONFIG_WAITPINSELECTOR_SHIFT)
#define GPMC_PREFETCH_CONFIG_WAITPINSELECTOR(x)     ((x << GPMC_PREFETCH_CONFIG_WAITPINSELECTOR_SHIFT) & \
                                                        GPMC_PREFETCH_CONFIG_WAITPINSELECTOR_MASK)
#define GPMC_PREFETCH_CONFIG_FIFOTHRESHOLD_SHIFT    (8)
#define GPMC_PREFETCH_CONFIG_FIFOTHRESHOLD_MASK     (0x7F << GPMC_PREFETCH_CONFIG_FIFOTHRESHOLD_SHIFT)
#define GPMC_PREFETCH_CONFIG_FIFOTHRESHOLD(x)       ((x << GPMC_PREFETCH_CONFIG_FIFOTHRESHOLD_SHIFT) & \
                                                        GPMC_PREFETCH_CONFIG_FIFOTHRESHOLD_MASK)
#define GPMC_PREFETCH_CONFIG_PFPWEIGHTEDPRIO_SHIFT  (16)
#define GPMC_PREFETCH_CONFIG_PFPWEIGHTEDPRIO_MASK   (0xF << GPMC_PREFETCH_CONFIG_PFPWEIGHTEDPRIO_SHIFT)
#define GPMC_PREFETCH_CONFIG_PFPWEIGHTEDPRIO(x)     ((x << GPMC_PREFETCH_CONFIG_PFPWEIGHTEDPRIO_SHIFT) & \
                                                        GPMC_PREFETCH_CONFIG_PFPWEIGHTEDPRIO_MASK)
#define GPMC_PREFETCH_CONFIG_ENGINECSSELECTOR_SHIFT (24)
#define GPMC_PREFETCH_CONFIG_ENGINECSSELECTOR_MASK  (0x7 << GPMC_PREFETCH_CONFIG_ENGINECSSELECTOR_SHIFT)
#define GPMC_PREFETCH_CONFIG_ENGINECSSELECTOR(x)    ((x << GPMC_PREFETCH_CONFIG_ENGINECSSELECTOR_SHIFT) & \
                                                        GPMC_PREFETCH_CONFIG_ENGINECSSELECTOR_MASK)
#define GPMC_PREFETCH_CONFIG_CYCLEOPTIMIZATION_SHIFT (28)
#define GPMC_PREFETCH_CONFIG_CYCLEOPTIMIZATION_MASK (0x7 << GPMC_PREFETCH_CONFIG_CYCLEOPTIMIZATION_SHIFT)
#define GPMC_PREFETCH_CONFIG_CYCLEOPTIMIZATION(x)   ((x << GPMC_PREFETCH_CONFIG_CYCLEOPTIMIZATION_SHIFT) & \
                                                        GPMC_PREFETCH_CONFIG_CYCLEOPTIMIZATION_MASK)

#define GPMC_PREFETCH_STATUS_FIFOSHIFT  (24)
#define GPMC_PREFETCH_STATUS_FIFOMASK   (0x7F << GPMC_PREFETCH_STATUS_FIFOSHIFT)

#define GPMC_PREFETCH_CONTROL_STARTENGINE (1 << 0)

#define GPMC_MASKADDRESS_SHIFT   (8)
#define GPMC_MASKADDRESS_MASK   (0xF << GPMC_MASKADDRESS_SHIFT)
#define GPMC_MASKADDRESS_128MB   (0x8 << GPMC_MASKADDRESS_SHIFT)
#define GPMC_MASKADDRESS_64MB   (0xC << GPMC_MASKADDRESS_SHIFT)
#define GPMC_MASKADDRESS_32MB   (0xE << GPMC_MASKADDRESS_SHIFT)
#define GPMC_MASKADDRESS_16MB   (0xF << GPMC_MASKADDRESS_SHIFT)

#define GPMC_BASEADDRESS_SHIFT   (0)
#define GPMC_BASEADDRESS_MASK   (0x3F << GPMC_BASEADDRESS_SHIFT)

#define GPMC_CSVALID            (1<<6)

//------------------------------------------------------------------------------

//  SDRC Modes

#define SDRC_MODE_NORMAL                       0x00000010
#define SDRC_MODE_RESET                        0x00000012



//  SDRC Manual command codes

#define SDRC_CMDCODE_NOP                       0x00
#define SDRC_CMDCODE_PRECHARGE                 0x01
#define SDRC_CMDCODE_AUTOREFRESH               0x02
#define SDRC_CMDCODE_ENTER_DEEP_POWER_DOWN     0x03
#define SDRC_CMDCODE_EXIT_DEEP_POWER_DOWN      0x04
#define SDRC_CMDCODE_ENTER_SELF_REFRESH        0x05
#define SDRC_CMDCODE_EXIT_SELF_REFRESH         0x06
#define SDRC_CMDCODE_SET_CKE_HIGH              0x07
#define SDRC_CMDCODE_SET_CKE_LOW               0x08


//  SDRC DLL Control codes
#define SDRC_DLL_ENABLE                        (1 << 3)
#define SDRC_DLL_LOCK                          (1 << 2)
#define SDRC_DLL_PHASE_72                      (0 << 1)
#define SDRC_DLL_PHASE_90                      (1 << 1)
#define SDRC_72DEG_PHASE_LIMIT                 (133)

//  SDRC POWER modes
#define SDRC_POWER_DELAY                       (6 << 8)
#define SDRC_POWER_SRFRONRESET                 (1 << 7)
#define SDRC_POWER_SRFRONIDLEREQ               (1 << 6)
#define SDRC_POWER_EXTCLKDIS                   (1 << 3)
#define SDRC_POWER_PWDENA                      (1 << 2)
#define SDRC_POWER_CLKCTRL_1                   (1 << 4)
#define SDRC_POWER_CLKCTRL_2                   (2 << 4)
#define SDRC_POWER_AUTOCOUNT(x)                (x << 8)

#endif

