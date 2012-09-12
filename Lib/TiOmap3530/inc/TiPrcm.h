//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   Power, Reset and Clock Management
//==============================================================================
#ifndef TI_PRCM_H
#define TI_PRCM_H

/* PRCM */
#define CM_FCLKEN_IVA2      0x48004000
#define CM_CLKEN_PLL_IVA2   0x48004004
#define CM_IDLEST_PLL_IVA2  0x48004024
#define CM_CLKSEL1_PLL_IVA2 0x48004040
#define CM_CLKSEL2_PLL_IVA2 0x48004044
#define CM_CLKEN_PLL_MPU    0x48004904
#define CM_IDLEST_PLL_MPU   0x48004924
#define CM_CLKSEL1_PLL_MPU  0x48004940
#define CM_CLKSEL2_PLL_MPU  0x48004944
#define CM_FCLKEN1_CORE     0x48004a00
#define CM_ICLKEN1_CORE     0x48004a10
#define CM_ICLKEN2_CORE     0x48004a14
#define CM_IDLEST1_CORE     0x48004a20
#define CM_CLKSEL_CORE      0x48004a40
#define CM_FCLKEN_GFX       0x48004b00
#define CM_ICLKEN_GFX       0x48004b10
#define CM_CLKSEL_GFX       0x48004b40
#define CM_FCLKEN_WKUP      0x48004c00
#define CM_ICLKEN_WKUP      0x48004c10
#define CM_CLKSEL_WKUP      0x48004c40
#define CM_IDLEST_WKUP      0x48004c20
#define CM_CLKEN_PLL        0x48004d00
#define CM_CLKEN2_PLL       0x48004d04
#define CM_IDLEST_CKGEN     0x48004d20
#define CM_IDLEST2_CKGEN    0x48004d24
#define CM_CLKSEL1_PLL      0x48004d40
#define CM_CLKSEL2_PLL      0x48004d44
#define CM_CLKSEL3_PLL      0x48004d48
#define CM_CLKSEL4_PLL      0x48004d4c
#define CM_CLKSEL5_PLL      0x48004d50
#define CM_FCLKEN_DSS       0x48004e00
#define CM_ICLKEN_DSS       0x48004e10
#define CM_CLKSEL_DSS       0x48004e40
#define CM_FCLKEN_CAM       0x48004f00
#define CM_ICLKEN_CAM       0x48004f10
#define CM_CLKSEL_CAM       0x48004F40
#define CM_FCLKEN_PER       0x48005000
#define CM_ICLKEN_PER       0x48005010
#define CM_CLKSEL_PER       0x48005040
#define CM_CLKSEL1_EMU      0x48005140

#define PRM_CLKSEL           0x48306d40
#define PRM_RSTCTRL          0x48307250
#define PRM_RSTST            0x48307258
#define PRM_CLKSRC_CTRL      0x48307270

//#define PRM_GLOBAL
#define PRM_VC_SMPS_SA       0x48307220
#define PRM_VC_SMPS_VOL_RA   0x48307224
#define PRM_VC_SMPS_CMD_RA   0x48307228
#define PRM_VC_CMD_VAL_0     0x4830722C
#define PRM_VC_CMD_VAL_1     0x48307230
#define PRM_VC_CH_CONF       0x48307234
#define PRM_VC_I2C_CFG       0x48307238
#define PRM_VP1_CONFIG       0x483072B0
#define PRM_VP1_VSTEPMIN     0x483072B4
#define PRM_VP1_VSTEPMAX     0x483072B8
#define PRM_VP1_VLIMITTO     0x483072BC
#define PRM_VP2_CONFIG       0x483072D0
#define PRM_VP2_VSTEPMIN     0x483072D4
#define PRM_VP2_VSTEPMAX     0x483072D8
#define PRM_VP2_VLIMITTO     0x483072DC

//-----------------------------------------------------------------------------
// voltage Control Registers

#define SMPS_SA0                            (1 << 0)
#define SMPS_RAV0                           (1 << 1)
#define SMPS_RAC0                           (1 << 2)
#define SMPS_RACEN0                         (1 << 3)
#define SMPS_CMD0                           (1 << 4)
#define SMPS_SA1                            (1 << 16)
#define SMPS_RAV1                           (1 << 17)
#define SMPS_RAC1                           (1 << 18)
#define SMPS_RACEN1                         (1 << 19)
#define SMPS_CMD1                           (1 << 20)

#define SMPS_SA0_SHIFT                      (0)
#define SMPS_SA0_MASK                       (0x7F << SMPS_SA0_SHIFT)
#define SMPS_SA1_SHIFT                      (16)
#define SMPS_SA1_MASK                       (0x7F << SMPS_SA1_SHIFT)

#define SMPS_VOLRA0_SHIFT                   (0)
#define SMPS_VOLRA0_MASK                    (0xFF << SMPS_VOLRA0_SHIFT)
#define SMPS_VOLRA1_SHIFT                   (16)
#define SMPS_VOLRA1_MASK                    (0xFF << SMPS_VOLRA1_SHIFT)

#define SMPS_CMDRA0_SHIFT                   (0)
#define SMPS_CMDRA0_MASK                    (0xFF << SMPS_CMDRA0_SHIFT)
#define SMPS_CMDRA1_SHIFT                   (16)
#define SMPS_CMDRA1_MASK                    (0xFF << SMPS_CMDRA1_SHIFT)

#define SMPS_MCODE_SHIFT                    (0)
#define SMPS_MCODE_MASK                     (0x7 << SMPS_MCODE_SHIFT)
#define SMPS_SREN                           (1 << 3)
#define SMPS_HSEN                           (1 << 4)
#define SMPS_HSMASTER                       (1 << 5)

#define SMPS_ON_SHIFT                       (24)
#define SMPS_ON_MASK                        (0xFF << SMPS_ON_SHIFT)
#define SMPS_ONLP_SHIFT                     (16)
#define SMPS_ONLP_MASK                      (0xFF << SMPS_ONLP_SHIFT)
#define SMPS_RET_SHIFT                      (8)
#define SMPS_RET_MASK                       (0xFF << SMPS_RET_SHIFT)
#define SMPS_OFF_SHIFT                      (0)
#define SMPS_OFF_MASK                       (0xFF << SMPS_OFF_SHIFT)

#define SMPS_ERROROFFSET_SHIFT              (24)
#define SMPS_ERROROFFSET_MASK               (0xFF << SMPS_ERROROFFSET_SHIFT)
#define SMPS_ERRORGAIN_SHIFT                (16)
#define SMPS_ERRORGAIN_MASK                 (0xFF << SMPS_ERRORGAIN_SHIFT)

#define SMPS_INITVOLTAGE_SHIFT              (8)
#define SMPS_INITVOLTAGE_MASK               (0xFF << SMPS_INITVOLTAGE_SHIFT)

#define SMPS_VOLTAGE_SHIFT                  (0)
#define SMPS_VOLTAGE_MASK                   (0xFF << SMPS_VOLTAGE_SHIFT)

#define SMPS_SMPSWAITTIMEMIN_SHIFT          (8)
#define SMPS_SMPSWAITTIMEMIN_MASK           (0xFFFF << SMPS_SMPSWAITTIMEMIN_SHIFT)
#define SMPS_VSTEPMIN_SHIFT                 (0)
#define SMPS_VSTEPMIN_MASK                  (0xFF << SMPS_VSTEPMIN_SHIFT)

#define SMPS_SMPSWAITTIMEMAX_SHIFT          (8)
#define SMPS_SMPSWAITTIMEMAX_MASK           (0xFFFF << SMPS_SMPSWAITTIMEMAX_SHIFT)
#define SMPS_VSTEPMAX_SHIFT                 (0)
#define SMPS_VSTEPMAX_MASK                  (0xFF << SMPS_VSTEPMAX_SHIFT)

#define SMPS_VDDMAX_SHIFT                   (24)
#define SMPS_VDDMAX_MASK                    (0xFF << SMPS_VDDMAX_SHIFT)
#define SMPS_VDDMIN_SHIFT                   (16)
#define SMPS_VDDMIN_MASK                    (0xFF << SMPS_VDDMIN_SHIFT)

#define SMPS_TIMEOUT_SHIFT                  (0)
#define SMPS_TIMEOUT_MASK                   (0xFFFF << SMPS_TIMEOUT_SHIFT)

#define SMPS_VPENABLE                       (1 << 0)
#define SMPS_FORCEUPDATE                    (1 << 1)
#define SMPS_INITVDD                        (1 << 2)
#define SMPS_TIMEOUTEN                      (1 << 3)

#define SMPS_VPINIDLE                       (1 << 0)

//-----------------------------------------------------------------------------
// sysconfig flags
//
#define SYSCONFIG_AUTOIDLE                  (1 << 0)

#define SYSCONFIG_SOFTRESET                 (1 << 1)
#define SYSSTATUS_RESETDONE                 (1 << 0)

#define SYSCONFIG_ENAWAKEUP                 (1 << 2)

#define SYSCONFIG_FORCEIDLE                 (0 << 3)
#define SYSCONFIG_NOIDLE                    (1 << 3)
#define SYSCONFIG_SMARTIDLE                 (2 << 3)
#define SYSCONFIG_IDLE_MASK                 (3 << 3)

#define SYSCONFIG_CLOCKACTIVITY_AUTOOFF     (0 << 8)
#define SYSCONFIG_CLOCKACTIVITY_F_ON        (1 << 8)
#define SYSCONFIG_CLOCKACTIVITY_I_ON        (2 << 8)
#define SYSCONFIG_CLOCKACTIVITY_IF_ON       (3 << 8)

#define SYSCONFIG_FORCESTANDBY              (0 << 12)
#define SYSCONFIG_NOSTANDBY                 (1 << 12)
#define SYSCONFIG_SMARTSTANDBY              (2 << 12)
#define SYSCONFIG_STANDBY_MASK              (3 << 12)


void PRCM_Clock( uint32 base, bool enable );

#endif
