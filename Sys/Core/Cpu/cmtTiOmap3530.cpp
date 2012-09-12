//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
//==============================================================================

//==============================================================================
//                     Clock Card 0
#if CMT_CPU_CLK_MAP == 0
//------------------------------------------------------------------------------
//
//  Define: BSP_PRM_CLKSEL
//
//  Determines the system clock frequency.  Used to update PRM_CLKSEL
//
//  Allowed values:
//
//      0x0: Input clock is 12 MHz
//      0x1: Input clock is 13 MHz
//      0x2: Input clock is 19.2 MHz
//      0x3: Input clock is 26 MHz
//      0x4: Input clock is 38.4 MHz
//
#define BSP_PRM_CLKSEL                  (3)

//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL1_EMU
//
//  Determines EMU clock settings.  Used to update CM_CLKSEL1_EMU
//
//  Allowed values:
//
#define BSP_EMU_DIV_DPLL4              (3 << 24)    // DPLL4/3=288mhz
#define BSP_EMU_DIV_DPLL3              (2 << 16)    // DPLL3/2
#define BSP_EMU_CLKSEL_TRACECLK        (1 << 11)    // TRACECLK/1       (default)
#define BSP_EMU_CLKSEL_PCLK            (2 << 8)     // PCLK.FCLK/2      (default)
#define BSP_EMU_PCLKX2                 (1 << 6)     // PCLKx2.FCLK/1    (default)
#define BSP_EMU_CLKSEL_ATCLK           (1 << 4)     // ATCLK.FCLK/1     (default)
#define BSP_EMU_TRACE_MUX_CTRL         (0 << 2)     // TRACE src=sysclk (default)
#define BSP_EMU_MUX_CTRL               (0 << 0)     // ATCLK.PCLK=sysclk(default)

#define BSP_CM_CLKSEL1_EMU             (BSP_EMU_DIV_DPLL4 | \
                                        BSP_EMU_DIV_DPLL3 | \
                                        BSP_EMU_CLKSEL_TRACECLK | \
                                        BSP_EMU_CLKSEL_PCLK | \
                                        BSP_EMU_PCLKX2 | \
                                        BSP_EMU_CLKSEL_ATCLK | \
                                        BSP_EMU_TRACE_MUX_CTRL | \
                                        BSP_EMU_MUX_CTRL)


//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL_CAM
//
//  Determines CAM clock settings.  Used to update CM_CLKSEL_CAM
//
//  Allowed values:
//
#define BSP_CAM_CLKSEL_CAM             (4 << 0)     // DPLL4/4=216mhz

#define BSP_CM_CLKSEL_CAM              (BSP_CAM_CLKSEL_CAM)

//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL3_PLL
//
//  Determines divisor from DPLL4 for 96M.  Used to update CM_CLKSEL3_PLL
//
//  Allowed values:
//
// Note that 96MHz clock comes from the M2X2 port
#define BSP_DIV_96M                    (9 << 0)     // DPLL4 864MHz/96MHz = 9

#define BSP_CM_CLKSEL3_PLL             (BSP_DIV_96M)


//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL2_PLL
//
//  Determines PER clock settings.  Used to update CM_CLKSEL2_PLL (DPLL4)
//
//  Allowed values:
//

#define BSP_PERIPH_DPLL_MULT           (216 << 8)    // freq = 864MHz

// in 37xx, DPLL4_M2 is 192MHz
#define BSP_PERIPH_DPLL_MULT_37xx  (432 << 8)    // freq = 864MHz
#define BSP_PERIPH_DPLL_DIV            (12 << 0)     //

#define BSP_CM_CLKSEL2_PLL             (BSP_PERIPH_DPLL_MULT | \
                                        BSP_PERIPH_DPLL_DIV)


//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKEN_PLL
//
//  Determines the DPLL3 and DPLL4 internal frequency based on DPLL reference
//  clock and divider (n).  Used to update CM_CLKEN_PLL
//
//  Allowed values:
//
//      0x3: 0.75 MHz to 1.0 MHz
//      0x4: 1.0 MHz to 1.25 MHz
//      0x5: 1.25 MHz to 1.5 MHz
//      0x6: 1.5 MHz to 1.75 MHz
//      0x7: 1.75 MHz to 2.1 MHz
//      0xB: 7.5 MHz to 10 MHz
//      0xC: 10 MHz to 12.5 MHz
//      0xD: 12.5 MHz to 15 MHz
//      0xE: 15 MHz to 17.5 MHz
//      0xF: 17.5 MHz to 21 MHz
//
#define BSP_PWRDN_EMU_PERIPH            (0 << 31)   // enable DPLL4_M6X2
#define BSP_PWRDN_CAM                   (0 << 30)   // enable DPLL4_M5X2
#define BSP_PWRDN_DSS1                  (0 << 29)   // enable DPLL4_M4X2
#define BSP_PWRDN_TV                    (0 << 28)   // enable DPLL4_M3X2
#define BSP_PWRDN_96M                   (0 << 27)   // enable DPLL4_M2X2
#define BSP_EN_PERIPH_DPLL_LPMODE       (0 << 26)   // disable DPLL4 LP mode

#define BSP_PERIPH_DPLL_RAMPTIME    (0 << 24)   // disable DPLL4 ramptime
#define BSP_PERIPH_DPLL_FREQSEL     (7 << 20)   // freqsel = 1.75-2.1 mhz
   
#define BSP_EN_PERIPH_DPLL_DRIFTGUARD   (1 << 19)   // enable DPLL4 driftguard
#define BSP_EN_PERIPH_DPLL              (7 << 16)   // lock DPLL4
#define BSP_PWRDN_EMU_CORE              (0 << 12)   // enable DPLL3_M3X2
#define BSP_EN_CORE_DPLL_LPMODE         (0 << 10)   // disable DPLL3 LP mode
#define BSP_CORE_DPLL_RAMPTIME          (0 << 8)    // disable ramp time
#define BSP_EN_CORE_DPLL_DRIFTGUARD     (1 << 3)    // enable DPLL3 driftguard
#define BSP_EN_CORE_DPLL                (7 << 0)    // lock DPLL3

#define BSP_CORE_DPLL_FREQSEL       (7 << 4)    // freqsel=1.75 MHz to 2.1 MHz

#define BSP_CM_CLKEN_PLL                (BSP_PWRDN_EMU_PERIPH |         \
                                         BSP_PWRDN_CAM |                \
                                         BSP_PWRDN_DSS1 |               \
                                         BSP_PWRDN_TV |                 \
                                         BSP_PWRDN_96M |                \
                                         BSP_EN_PERIPH_DPLL_LPMODE |    \
                                         BSP_PERIPH_DPLL_RAMPTIME |     \
                                         BSP_PERIPH_DPLL_FREQSEL |      \
                                         BSP_EN_PERIPH_DPLL |           \
                                         BSP_PWRDN_EMU_CORE |           \
                                         BSP_EN_CORE_DPLL_LPMODE |      \
                                         BSP_CORE_DPLL_RAMPTIME |       \
                                         BSP_CORE_DPLL_FREQSEL |        \
                                         BSP_EN_CORE_DPLL)

#define DPLL_STATUS_MASK                    (0x1)
#define DPLL_STATUS_BYPASSED                (0x0)
#define DPLL_STATUS_LOCKED                  (0x1)

//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL_CORE
//
//  Determines CORE clock selection and dividers.  Used to update CM_CLKSEL_CORE
//
//  Allowed values:
//
#define BSP_CLKSEL_L3                  (2 << 0)    // L3 = CORE_CLK/2
#define BSP_CLKSEL_L4                  (2 << 2)    // L4 = L3 / 2
#define BSP_CLKSEL_GPT10               (0 << 6)    // GPT10 clk src = 32khz
#define BSP_CLKSEL_GPT11               (0 << 7)    // GPT11 clk src = 32khz
#define BSP_CLKSEL_SSI                 (3 << 8)    // SSI fclk src = COREX2_CLK / 3
#define BSP_CLKSEL_96M               (1 << 12)   // reserved, must be 1
   
#define BSP_CM_CLKSEL_CORE             (BSP_CLKSEL_96M | \
                                        BSP_CLKSEL_L3 | \
                                        BSP_CLKSEL_L4 | \
                                        BSP_CLKSEL_GPT10 | \
                                        BSP_CLKSEL_GPT11 | \
                                        BSP_CLKSEL_SSI)

//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL1_PLL
//
//  Determines master clock frequency.  Used to update CM_CLKSEL1_PLL
//
//  Allowed values:
//
#define BSP_CORE_DPLL_CLKOUT_DIV       (1 << 27)    // DPLL3 output is CORE_CLK/1
#define BSP_SOURCE_54M                 (0 << 5)     // 54Mhz clk src = DPLL4
#define BSP_SOURCE_48M                 (0 << 3)     // 48Mhz clk src = DPLL4

// Set Core DPLL based on attached DDR memory specification
// NOTE - Be sure to set BSP_CORE_DPLL_FREQSEL correctly based on the divider value
#define CORE_DPLL_MULT_260              (130 << 16)
#define CORE_DPLL_DIV_260               (12 << 8)
#define CORE_DPLL_MULT_330              (166 << 16)
#define CORE_DPLL_DIV_330               (12 << 8)
#define CORE_DPLL_MULT_400              (200 << 16)
#define CORE_DPLL_DIV_400               (12 << 8)

#define BSP_CORE_DPLL_MULT              CORE_DPLL_MULT_330  // Multiplier
#define BSP_CORE_DPLL_DIV               CORE_DPLL_DIV_330    // Divider
#define BSP_SOURCE_96M                  (0 << 6)     // 96Mhz clk src=CM_96M_FCLK

#define BSP_CM_CLKSEL1_PLL             (BSP_CORE_DPLL_CLKOUT_DIV | \
                                        BSP_CORE_DPLL_MULT | \
                                        BSP_CORE_DPLL_DIV | \
                                        BSP_SOURCE_54M | \
                                        BSP_SOURCE_48M | \
                                        BSP_SOURCE_96M)

//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL5_PLL
//
//  Determines divisor from DPLL5 for 120M.  Used to update CM_CLKSEL5_PLL
//
//  Allowed values:
//
#define BSP_DIV_120M                   (1 << 0)     // DPLL5/1 = 120Mhz

#define BSP_CM_CLKSEL5_PLL             (BSP_DIV_120M)

//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL4_PLL
//
//  Determines master clock frequency.  Used to update CM_CLKSEL4_PLL
//
//  Allowed values:
//
#define BSP_PERIPH2_DPLL_MULT          (60 << 8)    // Multiplier
#define BSP_PERIPH2_DPLL_DIV           (12  << 0)    // Divider

#define BSP_CM_CLKSEL4_PLL             (BSP_PERIPH2_DPLL_MULT | \
                                        BSP_PERIPH2_DPLL_DIV)

//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKEN2_PLL
//
//  Determines the DPLL5 internal frequency based on DPLL reference
//  clock and divider (n).  Used to update CM_CLKEN2_PLL
//
//  Allowed values:
//
//      0x3: 0.75 MHz to 1.0 MHz
//      0x4: 1.0 MHz to 1.25 MHz
//      0x5: 1.25 MHz to 1.5 MHz
//      0x6: 1.5 MHz to 1.75 MHz
//      0x7: 1.75 MHz to 2.1 MHz
//      0xB: 7.5 MHz to 10 MHz
//      0xC: 10 MHz to 12.5 MHz
//      0xD: 12.5 MHz to 15 MHz
//      0xE: 15 MHz to 17.5 MHz
//      0xF: 17.5 MHz to 21 MHz
//
#define BSP_EN_PERIPH2_DPLL_LPMODE      (0 << 10)   // disable DPLL5 LP mode
#define BSP_PERIPH2_DPLL_RAMPTIME       (0 << 8)    // disable ramp time
#define BSP_PERIPH2_DPLL_FREQSEL        (7 << 4)    // freqsel=1.75 - 2.1 MHz
#define BSP_EN_PERIPH2_DPLL_DRIFTGUARD  (1 << 3)    // enable DPLL4 driftguard
#define BSP_EN_PERIPH2_DPLL             (7 << 0)    // lock DPLL5


#define BSP_CM_CLKEN2_PLL               (BSP_EN_PERIPH2_DPLL_LPMODE |      \
                                         BSP_PERIPH2_DPLL_RAMPTIME |       \
                                         BSP_PERIPH2_DPLL_FREQSEL |        \
                                         BSP_EN_PERIPH2_DPLL)


//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL2_PLL_MPU
//
//  Determines the output clock divider for DPLL1.  Used to update
//  CM_CLKSEL2_PLL_MPU
//
//  Allowed values:
//
//      0x1: DPLL1 output clock is divided by 1
//      0x2: DPLL1 output clock is divided by 2
//      0x3: DPLL1 output clock is divided by 3
//      0x4: DPLL1 output clock is divided by 4
//      0x5: DPLL1 output clock is divided by 5
//      0x6: DPLL1 output clock is divided by 6
//      0x7: DPLL1 output clock is divided by 7
//      0x8: DPLL1 output clock is divided by 8
//      0x9: DPLL1 output clock is divided by 9
//      0xA: DPLL1 output clock is divided by 10
//      0xB: DPLL1 output clock is divided by 11
//      0xC: DPLL1 output clock is divided by 12
//      0xD: DPLL1 output clock is divided by 13
//      0xE: DPLL1 output clock is divided by 14
//      0xF: DPLL1 output clock is divided by 15
//      0x10: DPLL1 output clock is divided by 16
//
#define BSP_MPU_DPLL_CLKOUT_DIV         (1 << 0)    // CLKOUTX2 = DPLL1 freq

#define BSP_CM_CLKSEL2_PLL_MPU          (BSP_MPU_DPLL_CLKOUT_DIV)

//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL1_PLL_MPU
//
//  Determines master clock frequency.  Used to update CM_CLKSEL1_PLL_MPU
//
//  Allowed values:
//
#define BSP_MPU_CLK_SRC                (2 << 19)    // DPLL1 bypass = CORE.CLK/2
// Set desired MPU frequency
//  #define MPU_DPLL_MULT_500           (250 << 8)
//  #define MPU_DPLL_DIV_500            (12 << 0)
//#define BSP_MPU_DPLL_MULT           MPU_DPLL_MULT_500   // Multiplier
//#define BSP_MPU_DPLL_DIV            MPU_DPLL_DIV_500    // Divider

// BSP_SPEED_CPUMHZ is set by .BAT file, default value is 500
//#define BSP_MPU_DPLL_MULT           ((BSP_SPEED_CPUMHZ / 2) << 8)   // Multiplier
#define BSP_MPU_DPLL_DIV            (12 << 0)                       // Divider

/*#define BSP_CM_CLKSEL1_PLL_MPU         (BSP_MPU_CLK_SRC |   \
                                        BSP_MPU_DPLL_MULT | \
                                        BSP_MPU_DPLL_DIV) */


//------------------------------------------------------------------------------
//
//  Define: BSP_MPU_DPLL_FREQSEL
//
//  Determines the DPLL1 internal frequency based on DPLL reference clock
//  and divider (n).  Used to update CM_CLKEN_PLL_MPU
//
//  Allowed values:
//
//      0x3: 0.75 MHz to 1.0 MHz
//      0x4: 1.0 MHz to 1.25 MHz
//      0x5: 1.25 MHz to 1.5 MHz
//      0x6: 1.5 MHz to 1.75 MHz
//      0x7: 1.75 MHz to 2.1 MHz
//      0xB: 7.5 MHz to 10 MHz
//      0xC: 10 MHz to 12.5 MHz
//      0xD: 12.5 MHz to 15 MHz
//      0xE: 15 MHz to 17.5 MHz
//      0xF: 17.5 MHz to 21 MHz
//
#define BSP_EN_MPU_DPLL_LPMODE         (0 << 10)   // disable DPLL1 LP mode
#define BSP_MPU_DPLL_RAMPTIME          (2 << 8)    // ramp time = 20us
#define BSP_EN_MPU_DPLL_DRIFTGUARD     (1 << 3)    // enable DPLL1 driftguard
#define BSP_EN_MPU_DPLL                (7 << 0)    // lock DPLL1
#define MPU_DPLL_FREQSEL_500           (7 << 4)
#define BSP_MPU_DPLL_FREQSEL       MPU_DPLL_FREQSEL_500
   
#define BSP_CM_CLKEN_PLL_MPU           (BSP_EN_MPU_DPLL_LPMODE |      \
                                        BSP_MPU_DPLL_RAMPTIME |       \
                                        BSP_MPU_DPLL_FREQSEL |        \
                                        BSP_EN_MPU_DPLL)

//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL2_PLL_IVA2
//
//  Determines the output clock divider for DPLL2.  Used to update
//  CM_CLKSEL2_PLL_IVA2
//
//  Allowed values:
//
//      0x1: DPLL2 output clock is divided by 1
//      0x2: DPLL2 output clock is divided by 2
//      0x3: DPLL2 output clock is divided by 3
//      0x4: DPLL2 output clock is divided by 4
//      0x5: DPLL2 output clock is divided by 5
//      0x6: DPLL2 output clock is divided by 6
//      0x7: DPLL2 output clock is divided by 7
//      0x8: DPLL2 output clock is divided by 8
//      0x9: DPLL2 output clock is divided by 9
//      0xA: DPLL2 output clock is divided by 10
//      0xB: DPLL2 output clock is divided by 11
//      0xC: DPLL2 output clock is divided by 12
//      0xD: DPLL2 output clock is divided by 13
//      0xE: DPLL2 output clock is divided by 14
//      0xF: DPLL2 output clock is divided by 15
//      0x10: DPLL2 output clock is divided by 16
//
#define BSP_IVA2_DPLL_CLKOUT_DIV        (1 << 0)    // CLKOUTX2 = DPLL2 freq

#define BSP_CM_CLKSEL2_PLL_IVA2         (BSP_IVA2_DPLL_CLKOUT_DIV)

//------------------------------------------------------------------------------
//
//  Define: BSP_CM_CLKSEL1_PLL_IVA2
//
//  Determines master clock frequency.  Used to update CM_CLKSEL1_PLL_IVA2
//
//  Allowed values:
//
#define BSP_3730_IVA2_CLK_SRC           (4 << 19)    // DPLL1 bypass = CORE.CLK/4
#define BSP_3530_IVA2_CLK_SRC           (2 << 19)    // DPLL1 bypass = CORE.CLK/2
//#define BSP_IVA2_DPLL_MULT             ((BSP_SPEED_IVAMHZ / 2) << 8)   // Multiplier
#define BSP_IVA2_DPLL_MULT             ((BSP_SPEED_IVAMHZ / 2) << 8)   // Multiplier
#define BSP_IVA2_DPLL_DIV              (12 << 0)    // Divider

/*#define BSP_CM_CLKSEL1_PLL_IVA2        (BSP_IVA2_CLK_SRC |      \
                                        BSP_IVA2_DPLL_MULT |    \
                                        BSP_IVA2_DPLL_DIV) */

//------------------------------------------------------------------------------
//
//  Define: BSP_IVA2_DPLL_FREQSEL
//
//  Determines the DPLL1 internal frequency based on DPLL reference clock
//  and divider (n).  Used to update CM_CLKEN_PLL_IVA2
//
//  Allowed values:
//
//      0x3: 0.75 MHz to 1.0 MHz
//      0x4: 1.0 MHz to 1.25 MHz
//      0x5: 1.25 MHz to 1.5 MHz
//      0x6: 1.5 MHz to 1.75 MHz
//      0x7: 1.75 MHz to 2.1 MHz
//      0xB: 7.5 MHz to 10 MHz
//      0xC: 10 MHz to 12.5 MHz
//      0xD: 12.5 MHz to 15 MHz
//      0xE: 15 MHz to 17.5 MHz
//      0xF: 17.5 MHz to 21 MHz
//
#define BSP_EN_IVA2_DPLL_LPMODE         (0 << 10)   // disable DPLL2 LP mode
#define BSP_IVA2_DPLL_RAMPTIME          (2 << 8)    // ramp time = 20us
#define BSP_IVA2_DPLL_FREQSEL       (7 << 4)    // 1.75 MHz to 2.1 MHz
#define BSP_EN_IVA2_DPLL_DRIFTGUARD     (1 << 3)    // enable DPLL1 driftguard
#define BSP_EN_IVA2_DPLL                (7 << 0)    // lock DPLL2

#define BSP_CM_CLKEN_PLL_IVA2           (BSP_EN_IVA2_DPLL_LPMODE |      \
                                         BSP_IVA2_DPLL_RAMPTIME |       \
                                         BSP_IVA2_DPLL_FREQSEL |        \
                                         BSP_EN_IVA2_DPLL)


//------------------------------------------------------------------------------
//
//  Define:  BSP_VC_SMPS_SA_INIT
//
//  slave address for VP1 & VP2
//
#define VC_SMPS_SA1                     (0x12 << 0)
#define VC_SMPS_SA2                     (0x12 << 16)

#define BSP_VC_SMPS_SA_INIT             (VC_SMPS_SA1 | \
                                         VC_SMPS_SA2)

//------------------------------------------------------------------------------
//
//  Define:  BSP_VC_SMPS_CMD_RA_INIT
//
//  cmd address for VP1 & VP2
//
#define VC_SMPS_CMD_RA1                 (0 << 0)
#define VC_SMPS_CMD_RA2                 (0 << 16)

#define BSP_VC_SMPS_CMD_RA_INIT         (VC_SMPS_CMD_RA1 | \
                                         VC_SMPS_CMD_RA2)

//------------------------------------------------------------------------------
//
//  Define:  BSP_VC_SMPS_VOL_RA_INIT
//
//  volt address for VP1 & VP2
//
#define VC_SMPS_VOL_RA1                 (0 << 0)
#define VC_SMPS_VOL_RA2                 (1 << 16)

#define BSP_VC_SMPS_VOL_RA_INIT         (VC_SMPS_VOL_RA1 | \
                                         VC_SMPS_VOL_RA2)

//------------------------------------------------------------------------------
//
//  Define:  BSP_VC_CH_CONF_INIT
//
//  flag to determine which subaddress to use to control voltage for VP1 & VP2
//
#define VC_CH_CONF_SA0                  (0 << 0)
#define VC_CH_CONF_RAV0                 (0 << 1)
#define VC_CH_CONF_RAC0                 (0 << 2)
#define VC_CH_CONF_RACEN0               (0 << 3)
#define VC_CH_CONF_CMD0                 (0 << 4)
#define VC_CH_CONF_SA1                  (1 << 16)
#define VC_CH_CONF_RAV1                 (1 << 17)
#define VC_CH_CONF_RAC1                 (1 << 18)
#define VC_CH_CONF_RACEN1               (0 << 19)
#define VC_CH_CONF_CMD1                 (1 << 20)

#define BSP_VC_CH_CONF_INIT             (VC_CH_CONF_CMD1 |   \
                                         VC_CH_CONF_RACEN1 | \
                                         VC_CH_CONF_RAC1 |   \
                                         VC_CH_CONF_RAV1 |   \
                                         VC_CH_CONF_SA1 |    \
                                         VC_CH_CONF_CMD0 |   \
                                         VC_CH_CONF_RACEN0 | \
                                         VC_CH_CONF_RAC0 |   \
                                         VC_CH_CONF_RAV0 |   \
                                         VC_CH_CONF_SA0)

//------------------------------------------------------------------------------
//
//  Define:  BSP_PRM_VC_I2C_CFG_INIT
//
//  flag to determine which subaddress to use to control voltage for VP1 & VP2
//
#define VC_I2C_CFG_HSMASTER             (0 << 5)
#define VC_I2C_CFG_SREN                 (0 << 4)
#define VC_I2C_CFG_HSEN                 (0 << 3)
#define VC_I2C_CFG_MCODE                (0x5 << 0)

#define BSP_PRM_VC_I2C_CFG_INIT         (VC_I2C_CFG_HSMASTER |\
                                         VC_I2C_CFG_SREN |    \
                                         VC_I2C_CFG_HSEN |    \
                                         VC_I2C_CFG_MCODE)

//------------------------------------------------------------------------------
//
//  Define:  BSP_PRM_VP1_CONFIG_INIT
//
//  flag to determine which subaddress to use to control voltage for VP1 & VP2
//
#define VP1_CONFIG_ERROROFFSET          (0 << 24)
#define VP1_CONFIG_ERRORGAIN            (0x20 << 16)
//VP1 init vlotage is retried at run time based on CPU family
//#define VP1_CONFIG_INITVOLTAGE          (VDD1_INIT_VOLTAGE_VALUE << 8) // should same as VC_CMD_0_VOLT_ON
#define VP1_CONFIG_TIMEOUTEN            (1 << 3)
#define VP1_CONFIG_INITVDD              (0 << 2)
#define VP1_CONFIG_FORCEUPDATE          (0 << 1)
#define VP1_CONFIG_VPENABLE             (0 << 0)

#define BSP_PRM_VP1_CONFIG_INIT         (VP1_CONFIG_ERROROFFSET |   \
                                         VP1_CONFIG_ERRORGAIN |     \
                                         VP1_CONFIG_TIMEOUTEN |     \
                                         VP1_CONFIG_INITVDD |       \
                                         VP1_CONFIG_FORCEUPDATE |   \
                                         VP1_CONFIG_VPENABLE)

//------------------------------------------------------------------------------
//
//  Define:  BSP_PRM_VP2_CONFIG_INIT
//
//  flag to determine which subaddress to use to control voltage for VP2 & VP2
//
#define VP2_CONFIG_ERROROFFSET          (0 << 24)
#define VP2_CONFIG_ERRORGAIN            (0x20 << 16)
//VP2 init vlotage is retried at run time based on CPU family
//#define VP2_CONFIG_INITVOLTAGE          (VDD2_INIT_VOLTAGE_VALUE << 8) // should same as VC_CMD_1_VOLT_ON
#define VP2_CONFIG_TIMEOUTEN            (1 << 3)
#define VP2_CONFIG_INITVDD              (0 << 2)
#define VP2_CONFIG_FORCEUPDATE          (0 << 1)
#define VP2_CONFIG_VPENABLE             (0 << 0)

#define BSP_PRM_VP2_CONFIG_INIT         (VP2_CONFIG_ERROROFFSET |   \
                                         VP2_CONFIG_ERRORGAIN |     \
                                         VP2_CONFIG_TIMEOUTEN |     \
                                         VP2_CONFIG_INITVDD |       \
                                         VP2_CONFIG_FORCEUPDATE |   \
                                         VP2_CONFIG_VPENABLE)

//------------------------------------------------------------------------------
//
//  Define:  BSP_PRM_VC_CMD_VAL_0_INIT
//
//  initial voltage for ON, LP, RET, OFF states
//  using the following eq. {volt = 0.0125(val) + 0.6} setup voltage levels
//  Set Vdd1 voltages: ON=<see VDD1_INIT_VOLTAGE_VALUE>, ON_LP=1.0v, VDD1_RET=1.0v, VDD1_OFF=0.0v 
//
//VDD1 init vlotage is retried at run time based on CPU family
//#define VC_CMD_0_VOLT_ON                (VDD1_INIT_VOLTAGE_VALUE << 24)  // should be the same as VP1_CONFIG_INITVOLTAGE
#define VC_CMD_0_VOLT_LP                (0x20 << 16)
#define VC_CMD_0_VOLT_RET               (0x20 << 8)
#define VC_CMD_0_VOLT_OFF               (0x00 << 0)

#define BSP_PRM_VC_CMD_VAL_0_INIT       ( VC_CMD_0_VOLT_LP |  \
                                         VC_CMD_0_VOLT_RET | \
                                         VC_CMD_0_VOLT_OFF)

//------------------------------------------------------------------------------
//
//  Define:  BSP_PRM_VC_CMD_VAL_1_INIT
//
//  initial voltage for ON, LP, RET, OFF states
//  using the following eq. {volt = 0.0125(val) + 0.6} setup voltage levels
//  Set Vdd2 voltages: ON=<see VDD2_INIT_VOLTAGE_VALUE>, ON_LP=1.0v, VDD2_RET=1.0v, VDD2_OFF=0.0v
//
// VDD2 init vlotage is retried at run time based on CPU family
//#define VC_CMD_1_VOLT_ON                (VDD2_INIT_VOLTAGE_VALUE << 24)  // should be the same as VP2_CONFIG_INITVOLTAGE
#define VC_CMD_1_VOLT_LP                (0x20 << 16)
#define VC_CMD_1_VOLT_RET               (0x20 << 8)
#define VC_CMD_1_VOLT_OFF               (0x00 << 0)

#define BSP_PRM_VC_CMD_VAL_1_INIT       ( VC_CMD_1_VOLT_LP |  \
                                         VC_CMD_1_VOLT_RET | \
                                         VC_CMD_1_VOLT_OFF)

//------------------------------------------------------------------------------
//
//  Define:  BSP_PRM_VP1_VSTEPMIN_INIT
//
#define VP1_SMPSWAITTIMEMIN             (0x1F4 << 8)
#define VP1_VSTEPMIN                    (0x01 << 0)

#define BSP_PRM_VP1_VSTEPMIN_INIT       (VP1_VSTEPMIN |  \
                                         VP1_SMPSWAITTIMEMIN)

//------------------------------------------------------------------------------
//
//  Define:  BSP_PRM_VP1_VSTEPMAX_INIT
//
#define VP1_SMPSWAITTIMEMAX             (0x1F4 << 8)
#define VP1_VSTEPMAX                    (0x10 << 0)

#define BSP_PRM_VP1_VSTEPMAX_INIT       (VP1_VSTEPMAX |  \
                                         VP1_SMPSWAITTIMEMAX)

//------------------------------------------------------------------------------
//
//  Define:  BSP_PRM_VP2_VSTEPMIN_INIT
//
#define VP2_SMPSWAITTIMEMIN             (0x1F4 << 8)
#define VP2_VSTEPMIN                    (0x01 << 0)

#define BSP_PRM_VP2_VSTEPMIN_INIT       (VP2_VSTEPMIN |  \
                                         VP2_SMPSWAITTIMEMIN)

//------------------------------------------------------------------------------
//
//  Define:  BSP_PRM_VP2_VSTEPMAX_INIT
//
#define VP2_SMPSWAITTIMEMAX             (0x1F4 << 8)
#define VP2_VSTEPMAX                    (0x10 << 0)

#define BSP_PRM_VP2_VSTEPMAX_INIT       (VP2_VSTEPMAX |  \
                                         VP2_SMPSWAITTIMEMAX)

//------------------------------------------------------------------------------
//
//  Define:  BSP_PRM_VP1_VLIMITTO_INIT
//
#define VP1_VDDMAX                      (0x40 << 24)
#define VP1_VDDMMIN                     (0x00 << 16)
#define VP1_TIMEOUT                     (0xFFFF << 0)

#define BSP_PRM_VP1_VLIMITTO_INIT       (VP1_VDDMAX |   \
                                         VP1_VDDMMIN |  \
                                         VP1_TIMEOUT)

//------------------------------------------------------------------------------
//
//  Define:  BSP_PRM_VP2_VLIMITTO_INIT
//
#define VP2_VDDMAX                      (0x3C << 24)
#define VP2_VDDMMIN                     (0x00 << 16)
#define VP2_TIMEOUT                     (0xFFFF << 0)

#define BSP_PRM_VP2_VLIMITTO_INIT       (VP2_VDDMAX |   \
                                         VP2_VDDMMIN |  \
                                         VP2_TIMEOUT)

//------------------------------------------------------------------------------
//
//  Define:  BSP_MAX_VOLTTRANSITION_TIME
//
//  maximum time 32khz tick to wait for vdd to transition
//
#define BSP_MAX_VOLTTRANSITION_TIME     (100)

//------------------------------------------------------------------------------
//  NAND flash memory location
//------------------------------------------------------------------------------

#define BSP_NAND_REGS_PA         0x08000000
#define BSP_NAND_MASKADDRESS      GPMC_MASKADDRESS_16MB

//------------------------------------------------------------------------------
//  NOR flash memory location
//------------------------------------------------------------------------------

#define BSP_NOR_REGS_PA            0x11000000
#define BSP_NOR_MASKADDRESS         GPMC_MASKADDRESS_16MB

//------------------------------------------------------------------------------
//
//  Define:  BSP_GPMC_xxx
//
//  These constants are used to initialize general purpose memory configuration
//  registers
//
// NOTE - Settings below are based on CORE DPLL = 332MHz, L3 = CORE/2 (166MHz)

//  NAND settings, not optimized
//  CONFIGx for L3=166M
#define BSP_GPMC_NAND_CONFIG1_166       0x00001800      // 16 bit NAND interface
#define BSP_GPMC_NAND_CONFIG2_166       0x00060600      // 0x00141400
#define BSP_GPMC_NAND_CONFIG3_166       0x00060401      // 0x00141400
#define BSP_GPMC_NAND_CONFIG4_166       0x05010801      // 0x0F010F01
#define BSP_GPMC_NAND_CONFIG5_166       0x00080909      // 0x010C1414
#define BSP_GPMC_NAND_CONFIG6_166       0x050001C0      // 0x00000A80
//  CONFIGx for L3=200M
#define BSP_GPMC_NAND_CONFIG1_200       0x00001800      // 16 bit NAND interface
#define BSP_GPMC_NAND_CONFIG2_200       0x00141400      // 0x00141400
#define BSP_GPMC_NAND_CONFIG3_200       0x00141400      // 0x00141400
#define BSP_GPMC_NAND_CONFIG4_200       0x0F010F01      // 0x0F010F01
#define BSP_GPMC_NAND_CONFIG5_200       0x010C1414      // 0x010C1414
#define BSP_GPMC_NAND_CONFIG6_200       0x1F0F0A80      // 0x00000A80
#define BSP_GPMC_NAND_CONFIG7       ((BSP_NAND_REGS_PA >> 24) | BSP_NAND_MASKADDRESS | GPMC_CSVALID)

//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_MCFG_0
//
//  Determines memory configuration registers.  Used to update SDRC_MCFG_0
//
//  Allowed values:
//
#define BSP_HYNIX_RASWIDTH_0                 (3 << 24)     // 14 bits
#define BSP_HYNIX_RAMSIZE_0                  (128<< 8)    // 256mb SDRAM on Hynix

#define BSP_MICRON_RASWIDTH_0                 (2 << 24)    // 13 bits
#define BSP_MICRON_RAMSIZE_0                  (64 << 8)    // 128mb SDRAM on EVM3530

#define BSP_CASWIDTH_0                 (5 << 20)
#define BSP_ADDRMUXLEGACY_0            (1 << 19)    // flexible address mux
#define BSP_BANKALLOCATION_0           (2 << 6)     // bank-row-column
#define BSP_B32NOT16_0                 (1 << 4)     // Ext. SDRAM is x32 bit.
#define BSP_DEEPPD_0                   (1 << 3)     // supports deep-power down
#define BSP_DDRTYPE_0                  (0 << 2)     // SDRAM is MobileDDR
#define BSP_RAMTYPE_0                  (1 << 0)     // SDRAM is DDR

#define BSP_HYNIX_SDRC_MCFG_0                (BSP_HYNIX_RASWIDTH_0 | \
                                        BSP_CASWIDTH_0 | \
                                        BSP_ADDRMUXLEGACY_0 | \
                                        BSP_HYNIX_RAMSIZE_0 | \
                                        BSP_BANKALLOCATION_0 | \
                                        BSP_B32NOT16_0 | \
                                        BSP_DEEPPD_0 | \
                                        BSP_DDRTYPE_0 | \
                                        BSP_RAMTYPE_0)

#define BSP_MICRON_SDRC_MCFG_0                (BSP_MICRON_RASWIDTH_0 | \
                                        BSP_CASWIDTH_0 | \
                                        BSP_ADDRMUXLEGACY_0 | \
                                        BSP_MICRON_RAMSIZE_0 | \
                                        BSP_BANKALLOCATION_0 | \
                                        BSP_B32NOT16_0 | \
                                        BSP_DEEPPD_0 | \
                                        BSP_DDRTYPE_0 | \
                                        BSP_RAMTYPE_0)


//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_MCFG_1
//
//  Determines memory configuration registers.  Used to update SDRC_MCFG_1
//
//  Allowed values:
//
#define BSP_MICRON_RASWIDTH_1                 (2 << 24)
#define BSP_CASWIDTH_1                 (5 << 20)
#define BSP_ADDRMUXLEGACY_1            (1 << 19)    // flexible address mux
#if BSP_SDRAM_BANK1_ENABLE == 1
    #define BSP_MICRON_RAMSIZE_1              (64 << 8)    // 128mb SDRAM on EVM3530
#else
    #define BSP_MICRON_RAMSIZE_1              (0 << 8)     // 0mb SDRAM on EVM3530
#endif
#define BSP_BANKALLOCATION_1           (2 << 6)     // bank-row-column
#define BSP_B32NOT16_1                 (1 << 4)     // Ext. SDRAM is x32 bit.
#define BSP_DEEPPD_1                   (1 << 3)     // supports deep-power down
#define BSP_DDRTYPE_1                  (0 << 2)     // SDRAM is MobileDDR
#define BSP_RAMTYPE_1                  (1 << 0)     // SDRAM is DDR

#define BSP_MICRON_SDRC_MCFG_1                (BSP_MICRON_RASWIDTH_1 | \
                                        BSP_CASWIDTH_1 | \
                                        BSP_ADDRMUXLEGACY_1 | \
                                        BSP_MICRON_RAMSIZE_1 | \
                                        BSP_BANKALLOCATION_1 | \
                                        BSP_B32NOT16_1 | \
                                        BSP_DEEPPD_1 | \
                                        BSP_DDRTYPE_1 | \
                                        BSP_RAMTYPE_1)

#define BSP_HYNIX_RAMSIZE_1    (0 << 8)   // 0MB on EVM3730

#define BSP_HYNIX_SDRC_MCFG_1                (BSP_HYNIX_RASWIDTH_0 | \
                                        BSP_CASWIDTH_0 | \
                                        BSP_ADDRMUXLEGACY_0 | \
                                        BSP_HYNIX_RAMSIZE_1 | \
                                        BSP_BANKALLOCATION_0 | \
                                        BSP_B32NOT16_0 | \
                                        BSP_DEEPPD_0 | \
                                        BSP_DDRTYPE_0 | \
                                        BSP_RAMTYPE_0)

//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_SHARING
//
//  Determines the SDRC module attached memory size and position on the SDRC
//  module I/Os..  Used to update SDRC_SHARING
//
//  Allowed values:
//
#define BSP_CS1MUXCFG                  (0 << 12)    // 32-bit SDRAM on [31:0]
#define BSP_CS0MUXCFG                  (0 << 9)     // 32-bit SDRAM on [31:0]
#define BSP_SDRCTRISTATE               (1 << 8)     // Normal mode

#define BSP_SDRC_SHARING               (BSP_CS1MUXCFG | \
                                        BSP_CS0MUXCFG | \
                                        BSP_SDRCTRISTATE)


//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_ACTIM_CTRLA_0
//
//  Determines ac timing control register A.  Used to update SDRC_ACTIM_CTRLA_0
//
//  Allowed values:
//
// NOTE - Settings below are based on CORE DPLL = 332MHz, L3 = CORE/2 (166MHz)

/* Samsung version of EVM3530 [K5W1G1GACM-DL60](166MHz optimized) ~ 6.0ns
 * Micron version of EVM3530 [MT29C2G24MAKLAJG-6](166MHz optimized) ~ 6.0ns
 *
 * ACTIM_CTRLA -
 *  TWR = 12/6  = 2 (samsung)
 *  TWR = 15/6  = 3 (micron)
 *  TDAL = Twr/Tck + Trp/tck = 12/6 + 18/6 = 2 + 3 = 5  (samsung)
 *  TDAL = Twr/Tck + Trp/tck = 15/6 + 18/6 = 3 + 3 = 6  (micron)
 *  TRRD = 12/6 = 2
 *  TRCD = 18/6 = 3
 *  TRP = 18/6  = 3
 *  TRAS = 42/6 = 7
 *  TRC = 60/6  = 10
 *  TRFC = 72/6 = 12 (samsung)
 *  TRFC = 125/6 = 21 (micron)
 *
 * ACTIM_CTRLB -
 *  TCKE            = 2 (samsung)
 *  TCKE            = 1 (micron)
 *  XSR = 120/6   = 20  (samsung)
 *  XSR = 138/6   = 23  (micron)
 */

// Choose more conservative of memory timings when they differ between vendors
#define BSP_HYNIX_TRFC_0                     (20 << 27)   // Autorefresh to active
#define BSP_HYNIX_TRC_0                       (11 << 22)   // Row cycle time
#define BSP_HYNIX_TRAS_0                     (8 << 18)    // Row active time

#define BSP_MICRON_TRFC_0                     (21 << 27)   // Autorefresh to active
#define BSP_MICRON_TRC_0                       (10 << 22)   // Row cycle time
#define BSP_MICRON_TRAS_0                     (7 << 18)    // Row active time

#define BSP_TRP_0                      (3 << 15)    // Row precharge time
#define BSP_TRCD_0                     (3 << 12)    // Row to column delay time
#define BSP_TRRD_0                     (2 << 9)     // Active to active cmd per.
#define BSP_TWR_0                      (3 << 6)     // Data-in to precharge cmd
#define BSP_TDAL_0                     (6 << 0)     // Data-in to active command


#define BSP_MICRON_SDRC_ACTIM_CTRLA_0         (BSP_MICRON_TRFC_0 | \
                                        BSP_MICRON_TRC_0 | \
                                        BSP_MICRON_TRAS_0 | \
                                        BSP_TRP_0 | \
                                        BSP_TRCD_0 | \
                                        BSP_TRRD_0 | \
                                        BSP_TWR_0 | \
                                        BSP_TDAL_0)

#define BSP_HYNIX_SDRC_ACTIM_CTRLA_0         (BSP_HYNIX_TRFC_0 | \
                                        BSP_HYNIX_TRC_0 | \
                                        BSP_HYNIX_TRAS_0 | \
                                        BSP_TRP_0 | \
                                        BSP_TRCD_0 | \
                                        BSP_TRRD_0 | \
                                        BSP_TWR_0 | \
                                        BSP_TDAL_0)


//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_ACTIM_CTRLA_1
//
//  Determines ac timing control register A.  Used to update SDRC_ACTIM_CTRLA_1
//
//  Allowed values:
//
#define BSP_MICRON_SDRC_ACTIM_CTRLA_1    BSP_MICRON_SDRC_ACTIM_CTRLA_0
#define BSP_HYNIX_SDRC_ACTIM_CTRLA_1    BSP_HYNIX_SDRC_ACTIM_CTRLA_0


//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_ACTIM_CTRLB_0
//
//  Determines ac timing control register B.  Used to update SDRC_ACTIM_CTRLB_0
//
//  Allowed values:
//
#define BSP_HYNIX_TWTR_0                     (0x2 << 16)  // 1-cycle write to read delay
#define BSP_HYNIX_TCKE_0                      (1 << 12)    // CKE minimum pulse width
#define BSP_HYNIX_TXP_0                        (0x3 << 8)   // 5 minimum cycles
#define BSP_HYNIX_TXSR_0                      (28 << 0)    // Self Refresh Exit to Active period

#define BSP_HYNIX_SDRC_ACTIM_CTRLB_0         (BSP_HYNIX_TWTR_0 | \
                                        BSP_HYNIX_TCKE_0 | \
                                        BSP_HYNIX_TXP_0 | \
                                        BSP_HYNIX_TXSR_0)




#define BSP_MICRON_TWTR_0                     (0x1 << 16)  // 1-cycle write to read delay
#define BSP_MICRON_TCKE_0                     (2 << 12)    // CKE minimum pulse width
#define BSP_MICRON_TXP_0                      (0x5 << 8)   // 5 minimum cycles
#define BSP_MICRON_TXSR_0                     (20 << 0)    // Self Refresh Exit to Active period

#define BSP_MICRON_SDRC_ACTIM_CTRLB_0         (BSP_MICRON_TCKE_0 | \
                                        BSP_MICRON_TXSR_0)

//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_ACTIM_CTRLB_1
//
//  Determines ac timing control register A.  Used to update SDRC_ACTIM_CTRLB_1
//
//  Allowed values:
//
#define BSP_MICRON_SDRC_ACTIM_CTRLB_1          BSP_MICRON_SDRC_ACTIM_CTRLB_0
#define BSP_HYNIX_SDRC_ACTIM_CTRLB_1            BSP_HYNIX_SDRC_ACTIM_CTRLB_0


//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_RFR_CTRL_0
//
//  SDRAM memory autorefresh control.  Used to update SDRC_RFR_CTRL_0
//
//  Allowed values:
//
#define BSP_HYNIX_ARCV                       (0x5E6)
#define BSP_HYNIX_ARCV_LOW              (0x212)
#define BSP_MICRON_ARCV                       (0x4E2)
#define BSP_MICRON_ARCV_LOW              (0x255)

#define BSP_MICRON_ARCV_0                     (BSP_MICRON_ARCV << 8)  // Autorefresh counter val
#define BSP_HYNIX_ARCV_0                     (BSP_HYNIX_ARCV << 8)  // Autorefresh counter val

#define BSP_ARE_0                      (1 << 0)         // Autorefresh on counter x1

#define BSP_MICRON_SDRC_RFR_CTRL_0            (BSP_MICRON_ARCV_0 | \
                                        BSP_ARE_0)

#define BSP_HYNIX_SDRC_RFR_CTRL_0            (BSP_HYNIX_ARCV_0 | \
                                        BSP_ARE_0)

//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_RFR_CTRL_1
//
//  SDRAM memory autorefresh control.  Used to update SDRC_RFR_CTRL_1
//
//  Allowed values:
//
#define BSP_MICRON_SDRC_RFR_CTRL_1             BSP_MICRON_SDRC_RFR_CTRL_0
#define BSP_HYNIX_SDRC_RFR_CTRL_1               BSP_HYNIX_SDRC_RFR_CTRL_0


//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_MR_0
//
//  Corresponds to the JEDEC SDRAM MR register.  Used to update SDRC_MR_0
//
//  Allowed values:
//
#define BSP_CASL_0                     (3 << 4)    // CAS latency = 3
#define BSP_SIL_0                      (0 << 3)    // Serial mode
#define BSP_BL_0                       (2 << 0)    // Burst Length = 4(DDR only)

#define BSP_SDRC_MR_0                  (BSP_CASL_0 | \
                                        BSP_SIL_0 | \
                                        BSP_BL_0)


//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_MR_0
//
//  Corresponds to the JEDEC SDRAM MR register.  Used to update SDRC_MR_1
//
//  Allowed values:
//
#define BSP_SDRC_MR_1                  (BSP_SDRC_MR_0)

//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_DLLA_CTRL
//
//  Used to fine-tune DDR timings.  Used to update SDRC_DLLA_CTRL
//
//  Allowed values:
//
#define BSP_FIXEDELAY                  (38 << 24)
#define BSP_MODEFIXEDDELAYINITLAT      (0 << 16)
#define BSP_DLLMODEONIDLEREQ           (0 << 5)
#define BSP_ENADLL                     (1 << 3)     // enable DLLs
#define BSP_LOCKDLL                    (0 << 2)     // run in unlock mode
#define BSP_DLLPHASE                   (1 << 1)     // 72 deg phase
#define BSP_SDRC_DLLA_CTRL             (BSP_FIXEDELAY | \
                                        BSP_MODEFIXEDDELAYINITLAT | \
                                        BSP_DLLMODEONIDLEREQ | \
                                        BSP_ENADLL | \
                                        BSP_LOCKDLL | \
                                        BSP_DLLPHASE)

//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_DLLB_CTRL
//
//  Used to fine-tune DDR timings.  Used to update SDRC_DLLB_CTRL
//
//  Allowed values:
//
#define BSP_SDRC_DLLB_CTRL             (BSP_SDRC_DLLA_CTRL)

//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_EMR2_0
//
//  Corresponds to the low-power EMR register, as defined in the mobile DDR
//  JEDEC standard.  Used to update SDRC_EMR2_0
//
//  Allowed values:
//
#define BSP_DS_0                       (0 << 5)    // Strong-strength driver
#define BSP_TCSR_0                     (0 << 3)    // 70 deg max temp
#define BSP_PASR_0                     (0 << 0)    // All banks

#define BSP_SDRC_EMR2_0                (BSP_DS_0 | \
                                        BSP_TCSR_0 | \
                                        BSP_PASR_0)


//------------------------------------------------------------------------------
//
//  Define: BSP_SDRC_EMR2_1
//
//  Corresponds to the low-power EMR register, as defined in the mobile DDR
//  JEDEC standard.  Used to update SDRC_EMR2_1
//
//  Allowed values:
//
#define BSP_SDRC_EMR2_1                (BSP_SDRC_EMR2_0)

//-----------------------------------------------------------------------------
#define DPLL_PER_MODE_SHIFT                 (16)
#define DPLL_PER_STAT_SHIFT                 (1)
#define DPLL_PER_IDLE_SHIFT                 (3)
#define DPLL_CORE_CLKSEL_SHIFT              (8)

#define DPLL_MPU                            1
#define DPLL_IVA                            2
#define DPLL_CORE                           3
#define DPLL_PER                            4
#define DPLL_PER2                           5

#define DPLL_MODE_SHIFT                     (0)
#define DPLL_MODE_MASK                      (0x7 << DPLL_MODE_SHIFT)
#define DPLL_MODE_LOWPOWER_STOP             (0x1 << DPLL_MODE_SHIFT)
#define DPLL_MODE_LOWPOWER_BYPASS           (0x5 << DPLL_MODE_SHIFT)
#define DPLL_MODE_FASTRELOCK                (0x6 << DPLL_MODE_SHIFT)
#define DPLL_MODE_LOCK                      (0x7 << DPLL_MODE_SHIFT)

#define DPLL_STATUS_MASK                    (0x1)
#define DPLL_STATUS_BYPASSED                (0x0)
#define DPLL_STATUS_LOCKED                  (0x1)

#define DPLL_CLK_SRC_SHIFT                  (19)
#define DPLL_CLK_SRC_MASK                   (0x1F << DPLL_CLK_SRC_SHIFT)
#define DPLL_CLK_SRC(x)                     ((0x1F & x) << DPLL_CLK_SRC_SHIFT)

#define DPLL_FREQSEL_SHIFT                  (4)
#define DPLL_FREQSEL_MASK                   (0xF << DPLL_FREQSEL_SHIFT)
#define DPLL_FREQSEL(x)                     ((0xF & (x)) << DPLL_FREQSEL_SHIFT)

#define DPLL_MULT_SHIFT                     (8)
#define DPLL_MULT_MASK                      (0x7FF << DPLL_MULT_SHIFT)
#define DPLL_MULT(x)                        ((0x7FF & (x)) << DPLL_MULT_SHIFT)

#define DPLL_DIV_SHIFT                      (0)
#define DPLL_DIV_MASK                       (0x7F << DPLL_DIV_SHIFT)
#define DPLL_DIV(x)                         ((0x7F & (x)) << DPLL_DIV_SHIFT)

#define EN_DPLL_LPMODE_SHIFT                (10)
#define EN_DPLL_LPMODE_MASK                 (1 << EN_DPLL_LPMODE_SHIFT)
#define EN_DPLL_LPMODE                      (1 << EN_DPLL_LPMODE_SHIFT)

#define DPLL_RAMPTIME_SHIFT                 (8)
#define DPLL_RAMPTIME_MASK                  (3 << DPLL_RAMPTIME_SHIFT)
#define DPLL_RAMPTIME_DISABLE               (0 << DPLL_RAMPTIME_SHIFT)
#define DPLL_RAMPTIME_4                     (1 << DPLL_RAMPTIME_SHIFT)
#define DPLL_RAMPTIME_20                    (2 << DPLL_RAMPTIME_SHIFT)
#define DPLL_RAMPTIME_40                    (3 << DPLL_RAMPTIME_SHIFT)

#define EN_DPLL_DRIFTGUARD_SHIFT            (3)
#define EN_DPLL_DRIFTGUARD_MASK             (1 << EN_DPLL_DRIFTGUARD_SHIFT)
#define EN_DPLL_DRIFTGUARD                  (1 << EN_DPLL_DRIFTGUARD_SHIFT)

#define DPLL_AUTOIDLE_SHIFT                 (0)
#define DPLL_AUTOIDLE_MASK                  (0x7 << DPLL_AUTOIDLE_SHIFT)
#define DPLL_AUTOIDLE_DISABLED              (0 << DPLL_AUTOIDLE_SHIFT)
#define DPLL_AUTOIDLE_LOWPOWERSTOPMODE      (1 << DPLL_AUTOIDLE_SHIFT)
#define DPLL_AUTOIDLE_LOWPOWERBYPASS        (5 << DPLL_AUTOIDLE_SHIFT)
#define DPLL_AUTOIDLE_HIGHPOWERBYPASS       (6 << DPLL_AUTOIDLE_SHIFT)

#define DPLL_MPU_CLKOUT_DIV_SHIFT           (0)
#define DPLL_IVA2_CLKOUT_DIV_SHIFT          (0)
#define DPLL_CORE_CLKOUT_DIV_SHIFT          (27)
#define DPLL_CORE_CLKOUT_DIV_MASK           (0x1F << DPLL_CORE_CLKOUT_DIV_SHIFT)


#define BSP_MPUSpeed 600
#define BSP_IVASpeed 430
#define BSP_VDD1Init 0x3c
#define BSP_VDD2Init 0x2c
#else //Clock card 0
  #error "CMT: This card (CMT_CPU_CLK_MAP) not released for omap3530 "
#endif

#if 0
typedef struct CPU_OPP_SETTINGS {
  uint32 MPUSpeed;
  uint32 IVASpeed;
  uint32 VDD1Init;
  uint32 VDD2Init;   
} CPU_OPP_Settings, *pCPU_OPP_Settings;

CPU_OPP_Settings OMAP35x_OPP_Table[OMAP35x_OPP_NUM]=
{
     // MPU[125Mhz @ 0.975V], IVA2[ 90Mhz @ 0.975V]
    {125, 90, 0x1e, 0x2c},
     // MPU[250Mhz @ 1.000V], IVA2[180Mhz @ 1.00V]    
    {250, 180, 0x20, 0x2c},
     // MPU[500Mhz @ 1.2000V], IVA2[360Mhz @ 1.20V]    
    {500, 360, 0x30, 0x2c},
     // MPU[550Mhz @ 1.2750V], IVA2[400Mhz @ 1.27V]   
    {550, 400, 0x36, 0x2c},
     // MPU[600Mhz @ 1.3500V], IVA2[430Mhz @ 1.35V]    
    {600, 430, 0x3c, 0x2c},
     // MPU[720hz @ 1.3500V], IVA2[520Mhz @ 1.35V]    
    {720, 520, 0x3c, 0x2c}
};
#endif

void
cmtClockInit() {
  //------------------------------------------------------------
  //                  Тактовые частоты
  // setup input system clock
  __raw_writel( BSP_PRM_CLKSEL, PRM_CLKSEL );

  //------------------------------------------------------------
  //                  Core_dpll_init();
  __raw_writel( BSP_CM_CLKSEL_CORE, CM_CLKSEL_CORE );
  __raw_writel( BSP_CM_CLKSEL1_PLL, CM_CLKSEL1_PLL );
  
  //------------------------------------------------------------
  //                   Dpll4_init();
  __raw_writel( BSP_CM_CLKSEL1_EMU, CM_CLKSEL1_EMU );
  __raw_writel( BSP_CM_CLKSEL_CAM, CM_CLKSEL_CAM );
  __raw_writel( BSP_CM_CLKSEL3_PLL, CM_CLKSEL3_PLL );
  __raw_writel( (__raw_readl( CM_CLKSEL2_PLL ) & 0xfff00000) | BSP_CM_CLKSEL2_PLL, CM_CLKSEL2_PLL );
  
  // lock dpll with correct frequency selection
  __raw_writel( BSP_CM_CLKEN_PLL, CM_CLKEN_PLL );
  while( (__raw_readl(CM_IDLEST_CKGEN) & DPLL_STATUS_MASK) != DPLL_STATUS_LOCKED);

  //------------------------------------------------------------
  //             Mpu_dpll_init(opp_setting);
  unsigned int val, mpu_mult, iva_mult;

  // put mpu dpll in bypass
  val = __raw_readl( CM_CLKEN_PLL_MPU );
  val &= ~DPLL_MODE_MASK;
  val |= DPLL_MODE_LOWPOWER_BYPASS;
  __raw_writel( val, CM_CLKEN_PLL_MPU );
  while( (__raw_readl(CM_IDLEST_PLL_MPU) & DPLL_STATUS_MASK) != DPLL_STATUS_BYPASSED);

  // setup DPLL1 divider
  __raw_writel( BSP_CM_CLKSEL2_PLL_MPU, CM_CLKSEL2_PLL_MPU );
  
  // configure m:n clock ratios as well as frequency selection for mpu dpll
  mpu_mult = ((BSP_MPUSpeed / 2) << 8);
  val = BSP_MPU_CLK_SRC | mpu_mult | BSP_MPU_DPLL_DIV;
  __raw_writel( val, CM_CLKSEL1_PLL_MPU );

  // lock dpll with correct frequency selection
  __raw_writel( BSP_CM_CLKEN_PLL_MPU, CM_CLKEN_PLL_MPU );
  while( (__raw_readl(CM_IDLEST_PLL_MPU) & DPLL_STATUS_MASK) != DPLL_STATUS_LOCKED);
  
  //------------------------------------------------------------
  //               Iva_dpll_init(opp_setting);    
  // setup dpll timings for iva2 dpll
  //

  // put iva2 dpll in bypass
  val = __raw_readl( CM_CLKEN_PLL_IVA2 );

  val &= ~DPLL_MODE_MASK;
  val |= DPLL_MODE_LOWPOWER_BYPASS;
  __raw_writel( val, CM_CLKEN_PLL_IVA2 );
  while( (__raw_readl(CM_IDLEST_PLL_IVA2) & DPLL_STATUS_MASK) != DPLL_STATUS_BYPASSED );

  // setup DPLL1 divider
  __raw_writel( BSP_CM_CLKSEL2_PLL_IVA2, CM_CLKSEL2_PLL_IVA2 );
  
  // configure m:n clock ratios as well as frequency selection for iva dpll
  iva_mult = ((BSP_IVASpeed/ 2) << 8);
  val = BSP_3530_IVA2_CLK_SRC | iva_mult | BSP_IVA2_DPLL_DIV;
  __raw_writel( val, CM_CLKSEL1_PLL_IVA2 );

  // lock dpll with correct frequency selection
  __raw_writel( BSP_CM_CLKEN_PLL_IVA2, CM_CLKEN_PLL_IVA2 );
  while( (__raw_readl(CM_IDLEST_PLL_IVA2) & DPLL_STATUS_MASK) != DPLL_STATUS_LOCKED );
  
  //------------------------------------------------------------
  //                       Per_dpll_init();
  __raw_writel( BSP_CM_CLKSEL5_PLL, CM_CLKSEL5_PLL );
  __raw_writel( BSP_CM_CLKSEL4_PLL, CM_CLKSEL4_PLL );
  __raw_writel( BSP_CM_CLKEN2_PLL, CM_CLKEN2_PLL );
  while( (__raw_readl(CM_IDLEST2_CKGEN) & DPLL_STATUS_MASK) != DPLL_STATUS_LOCKED);
  
  }

void
cmtVoltageInit() {
  //------------------------------------------------------------
  //                    Установить напряжения питания
  // setup i2c for smps communication
  __raw_writel( BSP_VC_SMPS_SA_INIT, PRM_VC_SMPS_SA );
  __raw_writel( BSP_VC_SMPS_VOL_RA_INIT, PRM_VC_SMPS_VOL_RA );
  __raw_writel( BSP_VC_SMPS_CMD_RA_INIT, PRM_VC_SMPS_CMD_RA );
  __raw_writel( BSP_VC_CH_CONF_INIT, PRM_VC_CH_CONF );
  __raw_writel( BSP_PRM_VC_I2C_CFG_INIT, PRM_VC_I2C_CFG );

  // set intial voltage levels
  __raw_writel( (BSP_VDD1Init << 24) | BSP_PRM_VC_CMD_VAL_0_INIT, PRM_VC_CMD_VAL_0 );
  __raw_writel( (BSP_VDD2Init << 24) | BSP_PRM_VC_CMD_VAL_1_INIT, PRM_VC_CMD_VAL_1 );

  // set PowerIC error offset, gains, and initial voltage
  __raw_writel( (BSP_VDD1Init << 8) | BSP_PRM_VP1_CONFIG_INIT, PRM_VP1_CONFIG );
  __raw_writel( (BSP_VDD2Init << 8) | BSP_PRM_VP2_CONFIG_INIT, PRM_VP2_CONFIG );

  // set PowerIC slew range
  __raw_writel( BSP_PRM_VP1_VSTEPMIN_INIT, PRM_VP1_VSTEPMIN );
  __raw_writel( BSP_PRM_VP1_VSTEPMAX_INIT, PRM_VP1_VSTEPMAX );
  __raw_writel( BSP_PRM_VP2_VSTEPMIN_INIT, PRM_VP2_VSTEPMIN );
  __raw_writel( BSP_PRM_VP2_VSTEPMAX_INIT, PRM_VP2_VSTEPMAX );

  // set PowerIC voltage limits and timeout
  __raw_writel( BSP_PRM_VP1_VLIMITTO_INIT, PRM_VP1_VLIMITTO );
  __raw_writel( BSP_PRM_VP2_VLIMITTO_INIT, PRM_VP2_VLIMITTO );
  
  // enable voltage processor
  __raw_setl( SMPS_VPENABLE, PRM_VP1_CONFIG );
  __raw_setl( SMPS_VPENABLE, PRM_VP2_CONFIG );

  // enable timeout
  __raw_setl( SMPS_TIMEOUTEN, PRM_VP1_CONFIG );
  __raw_setl( SMPS_TIMEOUTEN, PRM_VP2_CONFIG );

  // flush commands to smps
  __raw_setl( SMPS_FORCEUPDATE | SMPS_INITVDD, PRM_VP1_CONFIG );
  __raw_setl( SMPS_FORCEUPDATE | SMPS_INITVDD, PRM_VP2_CONFIG );

  // allow voltage to settle
  cmtWaitMKS(100);

  // disable voltage processor
  __raw_clearl( SMPS_VPENABLE | SMPS_FORCEUPDATE | SMPS_INITVDD | SMPS_TIMEOUTEN, PRM_VP1_CONFIG );
  __raw_clearl( SMPS_VPENABLE | SMPS_FORCEUPDATE | SMPS_INITVDD | SMPS_TIMEOUTEN, PRM_VP2_CONFIG );
  
  }

void
cmtMemoryInit() {
  //  Global GPMC Configuration
  __raw_writel( 0x00000008, GPMC_SYSCONFIG ); // No idle, L3 clock free running
  __raw_writel( 0x00000000, GPMC_IRQENABLE ); // All interrupts disabled
  __raw_writel( 0x00000000, GPMC_TIMEOUT_CONTROL ); // Time out disabled
  __raw_writel( 0x00000011, GPMC_CONFIG ); // WP high, force posted write for NAND 

#ifdef FMD_ONENAND
  // Configure CS0 for OneNAND,  Base Address 0x0C000000
  OUTREG32(&pGpmc->GPMC_CONFIG1_0, BSP_GPMC_ONENAND_CONFIG1);
  OUTREG32(&pGpmc->GPMC_CONFIG2_0, BSP_GPMC_ONENAND_CONFIG2);
  OUTREG32(&pGpmc->GPMC_CONFIG3_0, BSP_GPMC_ONENAND_CONFIG3);
  OUTREG32(&pGpmc->GPMC_CONFIG4_0, BSP_GPMC_ONENAND_CONFIG4);
  OUTREG32(&pGpmc->GPMC_CONFIG5_0, BSP_GPMC_ONENAND_CONFIG5);
  OUTREG32(&pGpmc->GPMC_CONFIG6_0, BSP_GPMC_ONENAND_CONFIG6);
  OUTREG32(&pGpmc->GPMC_CONFIG7_0, BSP_GPMC_ONENAND_CONFIG7);
#endif

  // Configure CS0 for NAND,  Base Address 0x08000000
  __raw_writel( BSP_GPMC_NAND_CONFIG1_166, GPMC_CONFIG_CS( GPMC_CONFIG1, 0 ) );
  __raw_writel( BSP_GPMC_NAND_CONFIG2_166, GPMC_CONFIG_CS( GPMC_CONFIG2, 0 ) );
  __raw_writel( BSP_GPMC_NAND_CONFIG3_166, GPMC_CONFIG_CS( GPMC_CONFIG3, 0 ) );
  __raw_writel( BSP_GPMC_NAND_CONFIG4_166, GPMC_CONFIG_CS( GPMC_CONFIG4, 0 ) );
  __raw_writel( BSP_GPMC_NAND_CONFIG5_166, GPMC_CONFIG_CS( GPMC_CONFIG5, 0 ) );
  __raw_writel( BSP_GPMC_NAND_CONFIG6_166, GPMC_CONFIG_CS( GPMC_CONFIG6, 0 ) );
  __raw_writel( BSP_GPMC_NAND_CONFIG7, GPMC_CONFIG_CS( GPMC_CONFIG7, 0 ) );
  
  // read config register
  //INREG32(&pSdrc->SDRC_SYSCONFIG);
  //pDDR_param = &BSP_DDR_device_params[ddr_type];      

  //After warm reset, the SDRC will be unreliable and this will cause eboot image can't be copied from NAND flash to SDRAM correctly.
  //Therefore, we have to force SDRC to reset after warm reset to solve this issue.
      
  //SDRC reset by software
  //OUTREG32(&pSdrc->SDRC_SYSCONFIG, SDRC_SYSCONFIG_SOFTWARE_RESET);
  // wait for at least 1000us
  //OALStall(1000);

  //After SDRC reset, we do below steps to configure SDRC regisger again.
 
  // Disable SDRC power saving mode
  __raw_clearl( SDRC_POWER_PWDENA, SDRC_POWER );

  // update memory cofiguration
  __raw_writel( BSP_MICRON_SDRC_MCFG_0, SDRC_MCFG_0 );
  __raw_writel( BSP_MICRON_SDRC_MCFG_1, SDRC_MCFG_1 );
  __raw_writel( BSP_SDRC_SHARING, SDRC_SHARING );

  // wait for at least 200us
  cmtWaitMKS(2000);

  // set autorefresh
  __raw_writel( BSP_MICRON_SDRC_RFR_CTRL_0, SDRC_RFR_CTRL_0 );
  __raw_writel( BSP_MICRON_SDRC_RFR_CTRL_1, SDRC_RFR_CTRL_1 );

  // setup ac timings
  __raw_writel( BSP_MICRON_SDRC_ACTIM_CTRLA_0, SDRC_ACTIM_CTRLA_0 );
  __raw_writel( BSP_MICRON_SDRC_ACTIM_CTRLA_1, SDRC_ACTIM_CTRLA_1 );
  __raw_writel( BSP_MICRON_SDRC_ACTIM_CTRLB_0, SDRC_ACTIM_CTRLB_0 );
  __raw_writel( BSP_MICRON_SDRC_ACTIM_CTRLB_1, SDRC_ACTIM_CTRLB_1 );

  // manual command sequence to start bank 0
  __raw_writel( 0, SDRC_MANUAL_0 );
  // wait for at least 200us
  cmtWaitMKS(2000);
  __raw_writel( 1, SDRC_MANUAL_0 );
  __raw_writel( 2, SDRC_MANUAL_0 );
  __raw_writel( 2, SDRC_MANUAL_0 );
  __raw_writel( BSP_SDRC_MR_0, SDRC_MR_0 );
      
  #if BSP_MICRON_RAMSIZE_1
     // manual command sequence to start bank 1
     OUTREG32(&pSdrc->SDRC_MANUAL_1, 0);
     // wait for at least 200us
     OALStall(2000);
     OUTREG32(&pSdrc->SDRC_MANUAL_1, 1);
     OUTREG32(&pSdrc->SDRC_MANUAL_1, 2);
     OUTREG32(&pSdrc->SDRC_MANUAL_1, 2);
     OUTREG32(&pSdrc->SDRC_MR_1, BSP_SDRC_MR_1);
  #endif

  // re-enable power saving mode
  __raw_setl( SDRC_POWER_PWDENA | SDRC_POWER_SRFRONIDLEREQ, SDRC_POWER );

  // update sdrc dll timings
  __raw_writel( BSP_SDRC_DLLA_CTRL, SDRC_DLLA_CTRL );
  __raw_writel( BSP_SDRC_DLLB_CTRL, SDRC_DLLB_CTRL );
      
  // update sdram characteristics
  __raw_writel( BSP_SDRC_EMR2_0, SDRC_EMR2_0 );
  __raw_writel( BSP_SDRC_EMR2_1, SDRC_EMR2_1 );

  __raw_setl( SDRC_POWER_SRFRONRESET, SDRC_POWER );

  // allow SDRC to settle
  cmtWaitMKS(100);

  // release the force on the clke signals
  MUX_VAL( CP(SDRC_CKE0),  (IDIS | DIS | M0) );
  MUX_VAL( CP(SDRC_CKE1),  (IDIS | DIS | M0) );
  }

void
cmtInitSystem() {
  //Тактовые частоты
  cmtClockInit();
  
  //Напряжение питания
  cmtVoltageInit();
  
  //Память
  cmtMemoryInit();

  //Настроить ножки
  MUX_VAL( CP(SDRC_D0)             ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D1)             ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D2)             ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D3)             ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D4)             ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D5)             ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D6)             ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D7)             ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D8)             ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D9)             ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D10)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D11)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D12)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D13)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D14)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D15)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D16)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D17)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D18)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D19)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D20)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D21)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D22)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D23)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D24)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D25)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D26)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D27)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D28)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D29)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D30)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_D31)            ,(IEN | M0 ) );
  MUX_VAL( CP(SDRC_CLK)            ,(IEN | M0 ));
  MUX_VAL( CP(SDRC_DQS0)           ,(IEN | M0 ));
  MUX_VAL( CP(SDRC_DQS1)           ,(IEN | M0 ));
  MUX_VAL( CP(SDRC_DQS2)           ,(IEN | M0 ));
  MUX_VAL( CP(SDRC_DQS3)           ,(IEN | M0 ));
  MUX_VAL( CP(SDRC_CKE0)           ,(IEN | M0 ));
  MUX_VAL( CP(SDRC_CKE1)           ,(IEN | M0 ));

  }
