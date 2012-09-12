//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   MMC/SD/SDIO
//==============================================================================
#ifndef TI_MMC_SD_H
#define TI_MMC_SD_H
#define MMCSLOT_1         1
#define MMCSLOT_2         2

//
//  MMC/SD/SDIO Registers
//

 struct OMAP_MMCHS_REGS {
    uint32 unused0[4];
    uint32 MMCHS_SYSCONFIG;
    uint32 MMCHS_SYSSTATUS;
    uint32 unused1[3];
    uint32 MMCHS_CSRE;
    uint32 MMCHS_SYSTEST;
    uint32 MMCHS_CON;
    uint32 MMCHS_PWCNT;
    uint32 unused2[52];
    uint32 MMCHS_BLK;
    uint32 MMCHS_ARG;
    uint32 MMCHS_CMD;
    uint32 MMCHS_RSP10;
    uint32 MMCHS_RSP32;
    uint32 MMCHS_RSP54;
    uint32 MMCHS_RSP76;
    uint32 MMCHS_DATA;
    uint32 MMCHS_PSTATE;
    uint32 MMCHS_HCTL;
    uint32 MMCHS_SYSCTL;
    uint32 MMCHS_STAT;
    uint32 MMCHS_IE;
    uint32 MMCHS_ISE;
    uint32 MMCHS_AC12;
    uint32 MMCHS_CAPA;
    uint32 unused4[1];
    uint32 MMCHS_CUR_CAPA;
    uint32 unused5[44];
    uint32 MMCHS_REV;
 };
 
typedef OMAP_MMCHS_REGS *volatile OMAP_MMCHS_REGS_PTR;

#define MMCHS1_PTR   ((OMAP_MMCHS_REGS_PTR)0x4809C000)
#define MMCHS2_PTR   ((OMAP_MMCHS_REGS_PTR)0x480B4000)
#define MMCHS3_PTR   ((OMAP_MMCHS_REGS_PTR)0x480AD000)

#define MMCHS1_BASE 0x4809C000
#define MMCHS2_BASE 0x480B4000
#define MMCHS3_BASE 0x480AD000
// MMCHS_SYSCONFIG register fields

#define MMCHS_SYSCONFIG_AUTOIDLE                (1 << 0)
#define MMCHS_SYSCONFIG_SOFTRESET               (1 << 1)
#define MMCHS_SYSCONFIG_ENAWAKEUP               (1 << 2)
#define MMCHS_SYSCONFIG_SIDLEMODE(mode)         ((mode) << 3)
#define MMCHS_SYSCONFIG_CLOCKACTIVITY(act)      ((act) << 8)

#define SIDLE_FORCE                             (0)
#define SIDLE_IGNORE                            (1)
#define SIDLE_SMART                             (2)

// MMCHS_SYSSTATUS register fields

#define MMCHS_SYSSTATUS_RESETDONE               (1 << 0)

// MMCHS_IE register fields

#define MMCHS_IE_CC                             (1 << 0)
#define MMCHS_IE_TC                             (1 << 1)
#define MMCHS_IE_BGE                            (1 << 2)
#define MMCHS_IE_BWR                            (1 << 4)
#define MMCHS_IE_BRR                            (1 << 5)
#define MMCHS_IE_CIRQ                           (1 << 8)
#define MMCHS_IE_CTO                            (1 << 16)
#define MMCHS_IE_CCRC                           (1 << 17)
#define MMCHS_IE_CEB                            (1 << 18)
#define MMCHS_IE_CIE                            (1 << 19)
#define MMCHS_IE_DTO                            (1 << 20)
#define MMCHS_IE_DCRC                           (1 << 21)
#define MMCHS_IE_DEB                            (1 << 22)
#define MMCHS_IE_ACE                            (1 << 24)
#define MMCHS_IE_CERR                           (1 << 28)
#define MMCHS_IE_BADA                           (1 << 29)

// MMCHS_ISE register fields

#define MMCHS_ISE_CC                            (1 << 0)
#define MMCHS_ISE_TC                            (1 << 1)
#define MMCHS_ISE_BGE                           (1 << 2)
#define MMCHS_ISE_BWR                           (1 << 4)
#define MMCHS_ISE_BRR                           (1 << 5)
#define MMCHS_ISE_CIRQ                          (1 << 8)
#define MMCHS_ISE_CTO                           (1 << 16)
#define MMCHS_ISE_CCRC                          (1 << 17)
#define MMCHS_ISE_CEB                           (1 << 18)
#define MMCHS_ISE_CIE                           (1 << 19)
#define MMCHS_ISE_DTO                           (1 << 20)
#define MMCHS_ISE_DCRC                          (1 << 21)
#define MMCHS_ISE_DEB                           (1 << 22)
#define MMCHS_ISE_ACE                           (1 << 24)
#define MMCHS_ISE_CERR                          (1 << 28)
#define MMCHS_ISE_BADA                          (1 << 29)

// MMCHS_STAT register fields

#define MMCHS_STAT_CC                           (1 << 0)
#define MMCHS_STAT_TC                           (1 << 1)
#define MMCHS_STAT_BGE                          (1 << 2)
#define MMCHS_STAT_BWR                          (1 << 4)
#define MMCHS_STAT_BRR                          (1 << 5)
#define MMCHS_STAT_CIRQ                         (1 << 8)
#define MMCHS_STAT_ERRI                         (1 << 15)
#define MMCHS_STAT_CTO                          (1 << 16)
#define MMCHS_STAT_CCRC                         (1 << 17)
#define MMCHS_STAT_CEB                          (1 << 18)
#define MMCHS_STAT_CIE                          (1 << 19)
#define MMCHS_STAT_DTO                          (1 << 20)
#define MMCHS_STAT_DCRC                         (1 << 21)
#define MMCHS_STAT_DEB                          (1 << 22)
#define MMCHS_STAT_ACE                          (1 << 24)
#define MMCHS_STAT_CERR                         (1 << 28)
#define MMCHS_STAT_BADA                         (1 << 29)

// MMCHS_PSTAT register fields

#define MMCHS_PSTAT_CMDI                        (1 << 0)
#define MMCHS_PSTAT_DATI                        (1 << 1)
#define MMCHS_PSTAT_DLA                         (1 << 2)
#define MMCHS_PSTAT_WTA                         (1 << 8)
#define MMCHS_PSTAT_RTA                         (1 << 9)
#define MMCHS_PSTAT_BWE                         (1 << 10)
#define MMCHS_PSTAT_BRE                         (1 << 11)
#define MMCHS_PSTAT_WP                          (1 << 19)
#define MMCHS_PSTAT_DLEV                        (0xF << 20)
#define MMCHS_PSTAT_CLEV                        (1 << 24)

// MMCHS_HCTL register fields

#define MMCHS_HCTL_DTW                          (1 << 1)
#define MMCHS_HCTL_SDBP                         (1 << 8)
#define MMCHS_HCTL_SDVS(vol)                    ((vol) << 9)
#define MMCHS_HCTL_SBGR                         (1 << 16)
#define MMCHS_HCTL_CR                           (1 << 17)
#define MMCHS_HCTL_RWC                          (1 << 18)
#define MMCHS_HCTL_IBG                          (1 << 19)
#define MMCHS_HCTL_IWE                          (1 << 24)

#define MMCHS_HCTL_SDVS_1V8                     (5 << 9)
#define MMCHS_HCTL_SDVS_3V0                     (6 << 9)
#define MMCHS_HCTL_SDVS_3V3                     (7 << 9)

// MMCHS_SYSCTL register fields

#define MMCHS_SYSCTL_ICE                        (1 << 0)
#define MMCHS_SYSCTL_ICS                        (1 << 1)
#define MMCHS_SYSCTL_CEN                        (1 << 2)
#define MMCHS_SYSCTL_CLKD(clkd)                 ((clkd) << 6)
#define MMCHS_SYSCTL_DTO(dto)                   ((dto) << 16)
#define MMCHS_SYSCTL_SRA                        (1 << 24)
#define MMCHS_SYSCTL_SRC                        (1 << 25)
#define MMCHS_SYSCTL_SRD                        (1 << 26)

#define MMCHS_SYSCTL_DTO_MASK                   (0xF0000)
#define MMCHS_SYSCTL_CLKD_MASK                  (0xFFC0)

// MMCHS_CMD register fields

#define MMCHS_CMD_DE                            (1 << 0)
#define MMCHS_CMD_BCE                           (1 << 1)
#define MMCHS_CMD_ACEN                          (1 << 2)
#define MMCHS_CMD_DDIR                          (1 << 4)
#define MMCHS_CMD_MSBS                          (1 << 5)
#define MMCHS_CMD_RSP_TYPE                      ((rsp) << 16)
#define MMCHS_CMD_CCCE                          (1 << 19)
#define MMCHS_CMD_CICE                          (1 << 20)
#define MMCHS_CMD_DP                            (1 << 21)
#define MMCHS_CMD_TYPE(cmd)                     ((cmd) << 22)
#define MMCHS_INDX(indx)                        ((indx) << 24)

// MMCHS_CAPA register fields

#define MMCHS_CAPA_TCF(tcf)                     ((tcf) << 0)
#define MMCHS_CAPA_TCU                          (1 << 7)
#define MMCHS_CAPA_BCF(bcf)                     ((bcf) << 8)
#define MMCHS_CAPA_MBL(mbl)                     ((mbl) << 16)
#define MMCHS_CAPA_HSS                          (1 << 21)
#define MMCHS_CAPA_DS                           (1 << 22)
#define MMCHS_CAPA_SRS                          (1 << 23)
#define MMCHS_CAPA_VS33                         (1 << 24)
#define MMCHS_CAPA_VS30                         (1 << 25)
#define MMCHS_CAPA_VS18                         (1 << 26)

// MMCHS_CON register fields

#define MMCHS_CON_OD                            (1 << 0)
#define MMCHS_CON_INIT                          (1 << 1)
#define MMCHS_CON_HR                            (1 << 2)
#define MMCHS_CON_STR                           (1 << 3)
#define MMCHS_CON_MODE                          (1 << 4)
#define MMCHS_CON_DW8                           (1 << 5)
#define MMCHS_CON_MIT                           (1 << 6)
#define MMCHS_CON_WPP                           (1 << 8)
#define MMCHS_CON_DVAL(v)                       (v << 9)
#define MMCHS_CON_CTPL                          (1 << 11)
#define MMCHS_CON_CEATA                         (1 << 12)
#define MMCHS_CON_OBIP                          (1 << 13)
#define MMCHS_CON_OBIE                          (1 << 14)
#define MMCHS_CON_PADEN                         (1 << 15)
#define MMCHS_CON_CLKEXTFREE                    (1 << 16)

/* MMCHS_CMD */
/*Response lengths*/
#define MMCHS_RSP_MASK              (0x00030000)

#define MMCHS_RSP_LEN48             (0x00020000)
#define MMCHS_RSP_LEN48B            (0x00030000)
#define MMCHS_RSP_LEN136            (0x00010000)
#define MMCHS_RSP_NONE              (0x00000000)

/*command type*/
#define MMCHS_CMD_NORMAL            (0x00000000)
#define MMCHS_CMD_SUSP              (0x1 << 22)
#define MMCHS_FUNC_SEL              (0x2 << 22)
#define MMCHS_CMD_ABORT             (0x3 << 22)

#define MMCHS_CMD_READ              (0x00000010) /*read, card to host*/
#define MMCHS_CMD_WRITE             (0x00000000) /*write, host to card*/

#define MMC_CTO_CONTROL_MAX         0x00FD
#define MMC_CTO_CONTROL_DEFAULT     0x0005      // 5 * 1024 = 5K cycles
#define MMC_DTO_CONTROL_MAX         0xFFFD
#define MMC_DTO_CONTROL_DEFAULT     0x0200      // 512 * 1024 = 512K cycles

#define STD_HC_MAX_CLOCK_FREQUENCY  48000000    // 48 MHz

#define MMCSD_CLOCK_INPUT           (96 * 1000 * 1000)
#define MMCSD_CLOCK_INIT            (400 * 1000)

#define SDMMC_DEFAULT_BLOCK_LEN     512
#define SDMMC_DEFAULT_NUM_BLOCKS    1

#define STD_HC_MAX_SLOTS            2

#define STD_HC_MIN_BLOCK_LENGTH     1
#define STD_HC_MAX_BLOCK_LENGTH     2048

/////
#define SOFT_RESET_ALL              MMCHS_SYSCTL_SRA

#define MMC_SOFTRESET            (0x1 << 1)
#define RESETDONE               (0x1 << 0)
#define NOOPENDRAIN               (0x0 << 0)
#define OPENDRAIN               (0x1 << 0)
#define OD                     (0x1 << 0)
#define INIT_NOINIT               (0x0 << 1)
#define INIT_INITSTREAM            (0x1 << 1)
#define HR_NOHOSTRESP            (0x0 << 2)
#define STR_BLOCK                (0x0 << 3)
#define MODE_FUNC               (0x0 << 4)
#define DW8_1_4BITMODE             (0x0 << 5)
#define MIT_CTO                  (0x0 << 6)
#define CDP_ACTIVEHIGH            (0x0 << 7)
#define WPP_ACTIVEHIGH             (0x0 << 8)
#define RESERVED_MASK            (0x3 << 9)
#define CTPL_MMC_SD             (0x0 << 11)
#define BLEN_512BYTESLEN         (0x200 << 0)
#define NBLK_STPCNT               (0x0 << 16)
#define DE_DISABLE               (0x0 << 0)
#define BCE_DISABLE               (0x0 << 1)
#define ACEN_DISABLE            (0x0 << 2)
#define DDIR_OFFSET               (4)
#define DDIR_MASK               (0x1 << 4)
#define DDIR_WRITE               (0x0 << 4)
#define DDIR_READ               (0x1 << 4)
#define MSBS_SGLEBLK            (0x0 << 5)
#define RSP_TYPE_OFFSET            (16)
#define RSP_TYPE_MASK            (0x3 << 16)
#define RSP_TYPE_NORSP            (0x0 << 16)
#define RSP_TYPE_LGHT136         (0x1 << 16)
#define RSP_TYPE_LGHT48            (0x2 << 16)
#define RSP_TYPE_LGHT48B         (0x3 << 16)
#define CCCE_NOCHECK            (0x0 << 19)
#define CCCE_CHECK               (0x1 << 19)
#define CICE_NOCHECK            (0x0 << 20)
#define CICE_CHECK               (0x1 << 20)
#define DP_OFFSET               (21)
#define DP_MASK                  (0x1 << 21)
#define DP_NO_DATA               (0x0 << 21)
#define DP_DATA                  (0x1 << 21)
#define CMD_TYPE_NORMAL            (0x0 << 22)
#define INDEX_OFFSET            (24)
#define INDEX_MASK               (0x3f << 24)
#define INDEX(i)               (i << 24)
#define DATI_MASK               (0x1 << 1)
#define DATI_CMDDIS               (0x1 << 1)
#define DTW_1_BITMODE            (0x0 << 1)
#define DTW_4_BITMODE            (0x1 << 1)
#define SDBP_PWROFF               (0x0 << 8)
#define SDBP_PWRON               (0x1 << 8)
#define SDVS_1V8               (0x5 << 9)
#define SDVS_3V0               (0x6 << 9)
#define ICE_MASK               (0x1 << 0)
#define ICE_STOP               (0x0 << 0)
#define ICS_MASK               (0x1 << 1)
#define ICS_NOTREADY            (0x0 << 1)
#define ICE_OSCILLATE            (0x1 << 0)
#define CEN_MASK               (0x1 << 2)
#define CEN_DISABLE               (0x0 << 2)
#define CEN_ENABLE               (0x1 << 2)
#define CLKD_OFFSET               (6)
#define CLKD_MASK               (0x3FF << 6)
#define DTO_MASK               (0xF << 16)
#define DTO_15THDTO               (0xE << 16)
#define SOFTRESETALL            (0x1 << 24)
#define CC_MASK                  (0x1 << 0)
#define TC_MASK                  (0x1 << 1)
#define BWR_MASK               (0x1 << 4)
#define BRR_MASK               (0x1 << 5)
#define ERRI_MASK               (0x1 << 15)
#define IE_CC                  (0x01 << 0)
#define IE_TC                  (0x01 << 1)
#define IE_BWR                  (0x01 << 4)
#define IE_BRR                  (0x01 << 5)
#define IE_CTO                  (0x01 << 16)
#define IE_CCRC                  (0x01 << 17)
#define IE_CEB                  (0x01 << 18)
#define IE_CIE                  (0x01 << 19)
#define IE_DTO                  (0x01 << 20)
#define IE_DCRC                  (0x01 << 21)
#define IE_DEB                  (0x01 << 22)
#define IE_CERR                  (0x01 << 28)
#define IE_BADA                  (0x01 << 29)

#define VS30_3V0SUP               (1 << 25)
#define VS18_1V8SUP               (1 << 26)

#endif

