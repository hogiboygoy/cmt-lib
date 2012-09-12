//==============================================================================
//              Cooperative MultiTasking system
//                    CMT system
// By Alexander Sibilev
// Contens
//   Display Subsystem
//==============================================================================
#ifndef TI_DISPLAY_H
#define TI_DISPLAY_H
//Регистры
#define DISPC_SYSCONFIG      0x48050410
#define DISPC_SYSSTATUS      0x48050414
#define DISPC_IRQSTATUS      0x48050418
#define DISPC_IRQENABLE      0x4805041C
#define DISPC_CONTROL        0x48050440
#define DISPC_CONFIG         0x48050444
#define DISPC_DEFAULT_COLOR0 0x4805044C
#define DISPC_TRANS_COLOR0   0x48050454
#define DISPC_TIMING_H       0x48050464
#define DISPC_TIMING_V       0x48050468
#define DISPC_POL_FREQ       0x4805046C
#define DISPC_DIVISOR        0x48050470
#define DISPC_SIZE_LCD       0x4805047C

#define DISPC_GFX_BA0        0x48050480
#define DISPC_GFX_BA1        0x48050484
#define DISPC_GFX_POSITION   0x48050488
#define DISPC_GFX_SIZE       0x4805048C
#define DISPC_GFX_ATTRIBUTES 0x480504A0
#define DISPC_GFX_FIFO_THRESHOLD 0x480504A4
#define DISPC_GFX_ROW_INC    0x480504AC
#define DISPC_GFX_PIXEL_INC  0x480504B0
#define DISPC_GFX_WINDOW_SKIP 0x480504B4

#define DSS_CONTROL          0x48050040
#define DSS_SYSCONFIG        0x48050010

// DSS_SYSCONFIG register fields

#define DSS_SYSCONFIG_AUTOIDLE                  (1 << 0)
#define DSS_SYSCONFIG_SOFTRESET                 (1 << 1)

// DSS_SYSSTATUS register fields

#define DSS_SYSSTATUS_RESETDONE                 (1 << 0)

// DSS_CONTROL register fields

#define DSS_CONTROL_DISPC_CLK_SWITCH_DSS1_ALWON (0 << 0)
#define DSS_CONTROL_DISPC_CLK_SWITCH_DSI1_PLL   (1 << 0)
#define DSS_CONTROL_DSI_CLK_SWITCH_DSS1_ALWON   (0 << 1)
#define DSS_CONTROL_DSI_CLK_SWITCH_DSI2_PLL     (1 << 1)
#define DSS_CONTROL_VENC_CLOCK_MODE_0           (0 << 2)
#define DSS_CONTROL_VENC_CLOCK_MODE_1           (1 << 2)
#define DSS_CONTROL_VENC_CLOCK_4X_ENABLE        (1 << 3)
#define DSS_CONTROL_DAC_DEMEN                   (1 << 4)
#define DSS_CONTROL_DAC_POWERDN_BGZ             (1 << 5)
#define DSS_CONTROL_DAC_VENC_OUT_SEL            (1 << 6)

// DSS_PSA_LCD_REG_2 register fields

#define DSS_PSA_LCD_2_SIG_MSB(sig)              ((sig) << 0)
#define DSS_PSA_LCD_2_DATA_AVAIL                (1 << 31)

// DSS_PSA_VIDEO_REG register fields

#define DSS_PSA_VIDEO_SIG(sig)                  ((sig) << 0)
#define DSS_PSA_VIDEO_DATA_AVAIL                (1 << 31)

// DSS_STATUS register fields

#define DSS_STATUS_DPLL_ENABLE                  (1 << 0)
#define DSS_STATUS_APLL_ENABLE                  (1 << 1)

// DISPC_SYSCONFIG register fields

#define DISPC_SYSCONFIG_AUTOIDLE                (1 << 0)
#define DISPC_SYSCONFIG_SOFTRESET               (1 << 1)
#define DISPC_SYSCONFIG_SIDLEMODE(mode)         ((mode) << 3)
#define DISPC_SYSCONFIG_MIDLEMODE(mode)         ((mode) << 12)

// DISPC_SYSSTATUS register fields

#define DISPC_SYSSTATUS_RESETDONE               (1 << 0)

// DISPC_CONTROL register fields

#define DISPC_CONTROL_LCDENABLE                 (1 << 0)
#define DISPC_CONTROL_DIGITALENABLE             (1 << 1)
#define DISPC_CONTROL_MONCOLOR                  (1 << 2)
#define DISPC_CONTROL_STNTFT                    (1 << 3)
#define DISPC_CONTROL_M8B                       (1 << 4)
#define DISPC_CONTROL_GOLCD                     (1 << 5)
#define DISPC_CONTROL_GODIGITAL                 (1 << 6)
#define DISPC_CONTROL_TFTDITHER_ENABLE          (1 << 7)
#define DISPC_CONTROL_TFTDATALINES_12           (0 << 8)
#define DISPC_CONTROL_TFTDATALINES_16           (1 << 8)
#define DISPC_CONTROL_TFTDATALINES_18           (2 << 8)
#define DISPC_CONTROL_TFTDATALINES_24           (3 << 8)
#define DISPC_CONTROL_SECURE                    (1 << 10)
#define DISPC_CONTROL_RFBIMODE                  (1 << 11)
#define DISPC_CONTROL_OVERLAY_OPTIMIZATION      (1 << 12)
#define DISPC_CONTROL_GPOUT0                    (1 << 15)
#define DISPC_CONTROL_GPOUT1                    (1 << 16)
#define DISPC_CONTROL_HT(ht)                    ((ht) << 17)
#define DISPC_CONTROL_TDMENABLE                 (1 << 20)
#define DISPC_CONTROL_TDMPARALLEL_MODE_8        (0 << 21)
#define DISPC_CONTROL_TDMPARALLEL_MODE_9        (1 << 21)
#define DISPC_CONTROL_TDMPARALLEL_MODE_12       (2 << 21)
#define DISPC_CONTROL_TDMPARALLEL_MODE_16       (3 << 21)
#define DISPC_CONTROL_TDMCYCLE_FORMAT_11        (0 << 23)
#define DISPC_CONTROL_TDMCYCLE_FORMAT_21        (1 << 23)
#define DISPC_CONTROL_TDMCYCLE_FORMAT_31        (2 << 23)
#define DISPC_CONTROL_TDMCYCLE_FORMAT_32        (3 << 23)
#define DISPC_CONTROL_TDMUNUSED_BITS_LO         (0 << 25)
#define DISPC_CONTROL_TDMUNUSED_BITS_HI         (1 << 25)
#define DISPC_CONTROL_TDMUNUSED_BITS_SAME       (2 << 25)
#define DISPC_CONTROL_PCKFREEENABLE_DISABLED    (0 << 27)
#define DISPC_CONTROL_PCKFREEENABLE_ENABLED     (1 << 27)
#define DISPC_CONTROL_LCDENABLESIGNAL_DISABLED  (0 << 28)
#define DISPC_CONTROL_LCDENABLESIGNAL_ENABLED   (1 << 28)
#define DISPC_CONTROL_LCDENABLEPOL_ACTIVELOW    (0 << 29)
#define DISPC_CONTROL_LCDENABLEPOL_ACTIVEHIGH   (1 << 29)
#define DISPC_CONTROL_TFTDITHERING_SPATIAL      (0 << 30)
#define DISPC_CONTROL_TFTDITHERING_TEMPORAL_2   (1 << 30)
#define DISPC_CONTROL_TFTDITHERING_TEMPORAL_4   (2 << 30)

// DISPC_CONFIG register fields

#define DISPC_CONFIG_PIXELGATED                 (1 << 0)
#define DISPC_CONFIG_LOADMODE(mode)             ((mode) << 1)
#define DISPC_CONFIG_PALETTEGAMMATABLE          (1 << 3)
#define DISPC_CONFIG_PIXELDATAGATED             (1 << 4)
#define DISPC_CONFIG_PIXELCLOCKGATED            (1 << 5)
#define DISPC_CONFIG_HSYNCGATED                 (1 << 6)
#define DISPC_CONFIG_VSYNCGATED                 (1 << 7)
#define DISPC_CONFIG_ACBIASGATED                (1 << 8)
#define DISPC_CONFIG_FUNCGATED                  (1 << 9)
#define DISPC_CONFIG_TCKLCDENABLE               (1 << 10)
#define DISPC_CONFIG_TCKLCDSELECTION            (1 << 11)
#define DISPC_CONFIG_TCKDIGENABLE               (1 << 12)
#define DISPC_CONFIG_TCKDIGSELECTION            (1 << 13)
#define DISPC_CONFIG_FIFOMERGE                  (1 << 14)
#define DISPC_CONFIG_CPR                        (1 << 15)
#define DISPC_CONFIG_FIFOHANDCHECK              (1 << 16)
#define DISPC_CONFIG_FIFOFILLING                (1 << 17)
#define DISPC_CONFIG_LCDALPHABLENDERENABLE      (1 << 18)
#define DISPC_CONFIG_DIGALPHABLENDERENABLE      (1 << 19)


// DISPC_TIMING_H register fields

#define DISPC_TIMING_H_HSW(hsw)                 ((hsw) << 0)
#define DISPC_TIMING_H_HFP(hfp)                 ((hfp) << 8)
#define DISPC_TIMING_H_HBP(hbp)                 ((hbp) << 20)

// DISPC_TIMING_V register fields

#define DISPC_TIMING_V_VSW(vsw)                 ((vsw) << 0)
#define DISPC_TIMING_V_VFP(vfp)                 ((vfp) << 8)
#define DISPC_TIMING_V_VBP(vbp)                 ((vbp) << 20)

// DISPC_POL_FREQ register fields

#define DISPC_POL_FREQ_ACB(acb)                 ((acb) << 0)
#define DISPC_POL_FREQ_ACBI(acbi)               ((acbi) << 8)
#define DISPC_POL_FREQ_IVS                      (1 << 12)
#define DISPC_POL_FREQ_IHS                      (1 << 13)
#define DISPC_POL_FREQ_IPC                      (1 << 14)
#define DISPC_POL_FREQ_IEO                      (1 << 15)
#define DISPC_POL_FREQ_RF                       (1 << 16)
#define DISPC_POL_FREQ_ONOFF                    (1 << 17)

// DISPC_DIVISOR register fields

#define DISPC_DIVISOR_PCD(pcd)                  ((pcd) << 0)
#define DISPC_DIVISOR_LCD(lcd)                  ((lcd) << 16)

// DISPC_GLOBAL_ALPHA register fields

#define DISPC_GLOBAL_ALPHA_GFX(color)           ((color & 0x000000ff) << 0)
#define DISPC_GLOBAL_ALPHA_VID2(color)          ((color & 0x000000ff) << 16)

// DISPC_SIZE_DIG register fields

#define DISPC_SIZE_DIG_PPL(ppl)                 (((ppl) - 1) << 0)
#define DISPC_SIZE_DIG_LPP(lpp)                 (((lpp) - 1) << 16)

// DISPC_SIZE_LCD register fields

#define DISPC_SIZE_LCD_PPL(ppl)                 (((ppl) - 1) << 0)
#define DISPC_SIZE_LCD_LPP(lpp)                 (((lpp) - 1) << 16)

// DISPC_GFX_POSITION register fields

#define DISPC_GFX_POS_GFXPOSX(x)                ((x) << 0)
#define DISPC_GFX_POS_GFXPOSY(y)                ((y) << 16)

// DISPC_GFX_SIZE register fields

#define DISPC_GFX_SIZE_GFXSIZEX(x)              (((x) - 1) << 0)
#define DISPC_GFX_SIZE_GFXSIZEY(y)              (((y) - 1) << 16)

// DISPC_GFX_ATTRIBUTES register fields

#define DISPC_GFX_ATTR_GFXENABLE                (1 << 0)
#define DISPC_GFX_ATTR_GFXFORMAT(fmt)           ((fmt) << 1)
#define DISPC_GFX_ATTR_GFXREPLICATIONENABLE     (1 << 5)
#define DISPC_GFX_ATTR_GFXBURSTSIZE_4x32        (0 << 6)
#define DISPC_GFX_ATTR_GFXBURSTSIZE_8x32        (1 << 6)
#define DISPC_GFX_ATTR_GFXBURSTSIZE_16x32       (2 << 6)
#define DISPC_GFX_ATTR_GFXCHANNELOUT            (1 << 8)
#define DISPC_GFX_ATTR_GFXNIBBLEMODE            (1 << 9)
#define DISPC_GFX_ATTR_FIFOPRELOAD              (1 << 11)
#define DISPC_GFX_ATTR_VIDROTATION_0            (0 << 12)
#define DISPC_GFX_ATTR_VIDROTATION_90           (1 << 12)
#define DISPC_GFX_ATTR_VIDROTATION_180          (2 << 12)
#define DISPC_GFX_ATTR_VIDROTATION_270          (3 << 12)
#define DISPC_GFX_ATTR_ARBITRATION              (1 << 14)
#define DISPC_GFX_ATTR_SELFREFRESH              (1 << 15)
#define DISPC_GFX_ATTR_PREMULTIPLYALPHA         (1 << 28)

// DISPC_GFX_FIFO_THRESHOLD register fields

#define DISPC_GFX_FIFO_THRESHOLD_LOW(low)       ((low) << 0)
#define DISPC_GFX_FIFO_THRESHOLD_HIGH(high)     ((high) << 16)

// DISPC_VID1_POSITION and DISPC_VID2_POSITION register fields

#define DISPC_VID_POS_VIDPOSX(x)                ((x) << 0)
#define DISPC_VID_POS_VIDPOSY(y)                ((y) << 16)

// DISPC_VID1_SIZE and DISPC_VID2_SIZE register fields

#define DISPC_VID_SIZE_VIDSIZEX(x)              (((x) - 1) << 0)
#define DISPC_VID_SIZE_VIDSIZEY(y)              (((y) - 1) << 16)

// DISPC_VID1_ATTRIBUTES and DISPC_VID2_ATTRIBUTES register fields

#define DISPC_VID_ATTR_VIDENABLE                (1 << 0)
#define DISPC_VID_ATTR_VIDFORMAT(fmt)           ((fmt) << 1)
#define DISPC_VID_ATTR_VIDRESIZE_NONE           (0 << 5)
#define DISPC_VID_ATTR_VIDRESIZE_HORIZONTAL     (1 << 5)
#define DISPC_VID_ATTR_VIDRESIZE_VERTICAL       (2 << 5)
#define DISPC_VID_ATTR_VIDRESIZE_BOTH           (3 << 5)
#define DISPC_VID_ATTR_VIDHRESIZE_CONF_UP       (0 << 7)
#define DISPC_VID_ATTR_VIDHRESIZE_CONF_DOWN     (1 << 7)
#define DISPC_VID_ATTR_VIDVRESIZE_CONF_UP       (0 << 8)
#define DISPC_VID_ATTR_VIDVRESIZE_CONF_DOWN     (1 << 8)
#define DISPC_VID_ATTR_VIDRESIZE_MASK           (0xF << 5)
#define DISPC_VID_ATTR_VIDCOLORCONVENABLE       (1 << 9)
#define DISPC_VID_ATTR_VIDREPLICATIONENABLE     (1 << 10)
#define DISPC_VID_ATTR_VIDFULLRANGE             (1 << 11)
#define DISPC_VID_ATTR_VIDROTATION_0            (0 << 12)
#define DISPC_VID_ATTR_VIDROTATION_90           (1 << 12)
#define DISPC_VID_ATTR_VIDROTATION_180          (2 << 12)
#define DISPC_VID_ATTR_VIDROTATION_270          (3 << 12)
#define DISPC_VID_ATTR_VIDROTATION_MASK         (3 << 12)
#define DISPC_VID_ATTR_VIDBURSTSIZE_4x32        (0 << 14)
#define DISPC_VID_ATTR_VIDBURSTSIZE_8x32        (1 << 14)
#define DISPC_VID_ATTR_VIDBURSTSIZE_16x32       (2 << 14)
#define DISPC_VID_ATTR_VIDCHANNELOUT            (1 << 16)
#define DISPC_VID_ATTR_VIDENDIANNESS            (1 << 17)
#define DISPC_VID_ATTR_VIDROWREPEATENABLE       (1 << 18)
#define DISPC_VID_ATTR_VIDFIFOPRELOAD           (1 << 19)
#define DISPC_VID_ATTR_VIDDMAOPTIMIZATION       (1 << 20)
#define DISPC_VID_ATTR_VIDVERTICALTAPS_3        (0 << 21)
#define DISPC_VID_ATTR_VIDVERTICALTAPS_5        (1 << 21)
#define DISPC_VID_ATTR_VIDLINEBUFFERSPLIT       (1 << 22)
#define DISPC_VID_ATTR_VIDARBITRATION           (1 << 23)
#define DISPC_VID_ATTR_VIDSELFREFRESH           (1 << 24)
#define DISPC_VID_ATTR_PREMULTIPLYALPHA         (1 << 28)

// DISPC_VID1_FIFO_THRESHOLD and DISPC_VID2_FIFO_THRESHOLD register fields

#define DISPC_VID_FIFO_THRESHOLD_LOW(low)       ((low) << 0)
#define DISPC_VID_FIFO_THRESHOLD_HIGH(high)     ((high) << 16)

// DISPC_VID1_PICTURE_SIZE and DISPC_VID2_PICTURE_SIZE register fields

#define DISPC_VID_PICTURE_SIZE_VIDORGSIZEX(x)   (((x) - 1) << 0)
#define DISPC_VID_PICTURE_SIZE_VIDORGSIZEY(y)   (((y) - 1) << 16)

// DISPC_VID1_FIR and DISPC_VID2_FIR register fields

#define DISPC_VID_FIR_VIDFIRHINC(inc)           ((inc) << 0)
#define DISPC_VID_FIR_VIDFIRVINC(inc)           ((inc) << 16)

// DISPC_VID1_CONV_COEF0 and DISPC_VID2_CONV_COEF0 register fields

#define DISPC_VID_CONV_COEF0_RY(ry)             ((ry&0x7FF) << 0)
#define DISPC_VID_CONV_COEF0_RCR(rcr)           ((rcr&0x7FF) << 16)

// DISPC_VID1_CONV_COEF1 and DISPC_VID2_CONV_COEF1 register fields

#define DISPC_VID_CONV_COEF1_RCB(rcb)           ((rcb&0x7FF) << 0)
#define DISPC_VID_CONV_COEF1_GY(gy)             ((gy&0x7FF) << 16)

// DISPC_VID1_CONV_COEF2 and DISPC_VID2_CONV_COEF2 register fields

#define DISPC_VID_CONV_COEF2_GCR(gcr)           ((gcr&0x7FF) << 0)
#define DISPC_VID_CONV_COEF2_GCB(gcb)           ((gcb&0x7FF) << 16)

// DISPC_VID1_CONV_COEF3 and DISPC_VID2_CONV_COEF3 register fields

#define DISPC_VID_CONV_COEF3_BY(by)             ((by&0x7FF) << 0)
#define DISPC_VID_CONV_COEF3_BCR(bcr)           ((bcr&0x7FF) << 16)

// DISPC_VID1_CONV_COEF4 and DISPC_VID2_CONV_COEF4 register fields

#define DISPC_VID_CONV_COEF4_BCB(bcb)           ((bcb&0x7FF) << 0)


// VENC_F_CONTROL register fields

#define VENC_F_CONTROL_FMT(fmt)                 ((fmt) << 0)
#define VENC_F_CONTROL_BACKGND(clr)             ((clr) << 2)
#define VENC_F_CONTROL_RGBF_RANGE_FULL          (0 << 5)
#define VENC_F_CONTROL_RGBF_RANGE_ITU601        (1 << 5)
#define VENC_F_CONTROL_SVDS_EXT                 (0 << 6)
#define VENC_F_CONTROL_SVDS_COLOR_BAR           (1 << 6)
#define VENC_F_CONTROL_SVDS_BACKGND_COLOR       (2 << 6)
#define VENC_F_CONTROL_RESET                    (1 << 8)



//DISPC_IRQSTATUS

#define DISPC_IRQSTATUS_FRAMEDONE               (1 << 0)
#define DISPC_IRQSTATUS_VSYNC                   (1 << 1)
#define DISPC_IRQSTATUS_EVSYNC_EVEN             (1 << 2)
#define DISPC_IRQSTATUS_EVSYNC_ODD              (1 << 3)
#define DISPC_IRQSTATUS_ACBIASCOUNTSTATUS       (1 << 4)
#define DISPC_IRQSTATUS_PROGRAMMEDLINENUMBER    (1 << 5)
#define DISPC_IRQSTATUS_GFXFIFOUNDERFLOW        (1 << 6)
#define DISPC_IRQSTATUS_GFXENDWINDOW            (1 << 7)
#define DISPC_IRQSTATUS_PALLETEGAMMALOADING     (1 << 8)
#define DISPC_IRQSTATUS_OCPERROR                (1 << 9)
#define DISPC_IRQSTATUS_VID1FIFOUNDERFLOW       (1 << 10)
#define DISPC_IRQSTATUS_VID1ENDWINDOW           (1 << 11)
#define DISPC_IRQSTATUS_VID2FIFOUNDERFLOW       (1 << 12)
#define DISPC_IRQSTATUS_VID2ENDWINDOW           (1 << 13)
#define DISPC_IRQSTATUS_SYNCLOST                (1 << 14)
#define DISPC_IRQSTATUS_SYNCLOSTDIGITAL         (1 << 15)
#define DISPC_IRQSTATUS_WAKEUP                  (1 << 16)

//DISPC_IRQENABLE

#define DISPC_IRQENABLE_FRAMEDONE               (1 << 0)
#define DISPC_IRQENABLE_VSYNC                   (1 << 1)
#define DISPC_IRQENABLE_EVSYNC_EVEN             (1 << 2)
#define DISPC_IRQENABLE_EVSYNC_ODD              (1 << 3)
#define DISPC_IRQENABLE_ACBIASCOUNTSTATUS       (1 << 4)
#define DISPC_IRQENABLE_PROGRAMMEDLINENUMBER    (1 << 5)
#define DISPC_IRQENABLE_GFXFIFOUNDERFLOW        (1 << 6)
#define DISPC_IRQENABLE_GFXENDWINDOW            (1 << 7)
#define DISPC_IRQENABLE_PALLETEGAMMALOADING     (1 << 8)
#define DISPC_IRQENABLE_OCPERROR                (1 << 9)
#define DISPC_IRQENABLE_VID1FIFOUNDERFLOW       (1 << 10)
#define DISPC_IRQENABLE_VID1ENDWINDOW           (1 << 11)
#define DISPC_IRQENABLE_VID2FIFOUNDERFLOW       (1 << 12)
#define DISPC_IRQENABLE_VID2ENDWINDOW           (1 << 13)
#define DISPC_IRQENABLE_SYNCLOST                (1 << 14)
#define DISPC_IRQENABLE_SYNCLOSTDIGITAL         (1 << 15)
#define DISPC_IRQENABLE_WAKEUP                  (1 << 16)


//DISPC_PIXELFORMAT (pixel formats for GFX, VID1 and VID2)

#define DISPC_PIXELFORMAT_BITMAP1               (0x0)
#define DISPC_PIXELFORMAT_BITMAP2               (0x1)
#define DISPC_PIXELFORMAT_BITMAP4               (0x2)
#define DISPC_PIXELFORMAT_BITMAP8               (0x3)
#define DISPC_PIXELFORMAT_RGB12                 (0x4)
#define DISPC_PIXELFORMAT_ARGB16                (0x5)
#define DISPC_PIXELFORMAT_RGB16                 (0x6)
#define DISPC_PIXELFORMAT_RGB32                 (0x8)
#define DISPC_PIXELFORMAT_RGB24                 (0x9)
#define DISPC_PIXELFORMAT_YUV2                  (0xA)
#define DISPC_PIXELFORMAT_UYVY                  (0xB)
#define DISPC_PIXELFORMAT_ARGB32                (0xC)
#define DISPC_PIXELFORMAT_RGBA32                (0xD)
#define DISPC_PIXELFORMAT_RGBx32                (0xE)


// DSI REGISTERS FIELDS

// NOTE: Only the relevant registers reqd for clock configuration
//       is current defined in this file.

// DSS_SYSCONFIG register fields
#define DSI_CLK_CTRL_LP_CLK_DIVISOR(x)           (x << 0)    //[12:0]
#define DSI_CLK_CTRL_DDR_CLK_ALWAYS_ON_ENABLED   (1 << 13)
#define DSI_CLK_CTRL_CIO_CLK_ICG_ENABLED         (1 << 14)
#define DSI_CLK_CTRL_LP_CLK_NULL_PKT_ENABLED     (1 << 15)
#define DSI_CLK_CTRL_LP_CLK_NULL_PKT_SIZE(x)     (x << 16)   //[17:16]
#define DSI_CLK_CTRL_HS_AUTO_STOP_ENABLED        (1 << 18)
#define DSI_CLK_CTRL_HS_MANUEL_STOP_CTRL         (1 << 19)
#define DSI_CLK_CTRL_LP_CLK_ENABLE               (1 << 20)
#define DSI_CLK_CTRL_LP_RX_SYNCHRO_ENABLE        (1 << 21)

#define DSI_CLK_CTRL_PLL_PWR_STATUS_OFF          (0 << 28)
#define DSI_CLK_CTRL_PLL_PWR_STATUS_ON_PLLONLY   (1 << 28)
#define DSI_CLK_CTRL_PLL_PWR_STATUS_ON_PLLANDHS  (2 << 28)
#define DSI_CLK_CTRL_PLL_PWR_STATUS_ON_DISPCONLY (3 << 28)

#define DSI_CLK_CTRL_PLL_PWR_CMD_OFF             (0 << 30)
#define DSI_CLK_CTRL_PLL_PWR_CMD_ON_PLLONLY      (1 << 30)
#define DSI_CLK_CTRL_PLL_PWR_CMD_ON_PLLANDHS     (2 << 30)
#define DSI_CLK_CTRL_PLL_PWR_CMD_ON_DISPCONLY    (3 << 30)
#define DSI_CLK_CTRL_PLL_PWR_CMD_MASK            (3 << 30)


#define DSI_PLL_AUTOMODE                         (1 << 0)
#define DSI_PLL_GATEMODE                         (1 << 1)
#define DSI_PLL_HALTMODE                         (1 << 2)
#define DSI_PLL_SYSRESET                         (1 << 3)
#define DSI_HSDIV_SYSRESET                       (1 << 4) //SYSRESET forced active

#define DSI_PLLCTRL_RESET_DONE_STATUS            (1 << 0)
#define DSI_PLL_LOCK_STATUS                      (1 << 1)
#define DSI_PLL_RECAL_STATUS                     (1 << 2)
#define DSI_PLL_LOSSREF_STATUS                   (1 << 3)
#define DSI_PLL_LIMP_STATUS                      (1 << 4)
#define DSI_PLL_HIGHJITTER_STATUS                (1 << 5)
#define DSI_PLL_BYPASS_STATUS                    (1 << 6)
#define DSS_CLOCK_ACK_STATUS                     (1 << 7)
#define DSIPROTO_CLOCK_ACK_STATUS                (1 << 8)
#define DSI_BYPASSACKZ_STATUS                    (1 << 9)

#define DSI_PLL_GO_CMD                           (1 << 0) //Request PLL (re)locking/locking pending

#define DSI_PLL_STOPMODE                         (1 << 0)
#define DSI_PLL_REGN(x)                          (x << 1) // [7:1]
#define DSI_PLL_REGM(x)                          (x << 8) // [18:8]
#define DSS_CLOCK_DIV(x)                         (x << 19)// [22:19]
#define DSIPROTO_CLOCK_DIV(x)                    (x << 23)// [26:23]



#define DSI_PLL_FREQSEL_1                        (1)
#define DSI_PLL_FREQSEL_2                        (2)
#define DSI_PLL_FREQSEL_7                        (7)


#define DSI_PLL_IDLE                             (1 << 0)
#define DSI_PLL_FREQSEL(x)                       (x << 1)
#define DSI_PLL_PLLLPMODE_REDUCEDPWR             (1 << 5)
#define DSI_PLL_LOWCURRSTBY                      (1 << 6)
#define DSI_PLL_TIGHTPHASELOCK                   (1 << 7)
#define DSI_PLL_DRIFTGUARDEN                     (1 << 8)
#define DSI_PLL_LOCKSEL(x)                       (1 << 9)
#define DSI_PLL_CLKSEL_PCLKFREE                  (1 << 11)
#define DSI_PLL_CLKSEL_DSS2ALWON_FCLK            (0 << 11)
#define DSI_PLL_HIGHFREQ_PIXELCLKBY2             (1 << 12)
#define DSI_PLL_REFEN                            (1 << 13)
#define DSIPHY_CLKINEN                           (1 << 14)
#define DSI_BYPASSEN_USEDSSFCLK                  (1 << 15)
#define DSS_CLOCK_EN                             (1 << 16)
#define DSS_CLOCK_PWDN                           (1 << 17)
#define DSI_PROTO_CLOCK_EN                       (1 << 18)
#define DSI_PROTO_CLOCK_PWDN                     (1 << 19)
#define DSI_HSDIVBYPASS                          (1 << 20)

#define DSS_FCLK_MAX                             (172000000)
#define DSI_HIGHFREQ_MAX                         (32000000)

#endif

