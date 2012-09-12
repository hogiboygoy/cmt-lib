#ifndef TI_CPU_H
#define TI_CPU_H

#define __raw_readl(a)    (*(volatile unsigned int *)(a))
#define __raw_readb(a)    (*(volatile unsigned char *)(a))
#define __raw_writel(v,a) (*(volatile unsigned int *)(a) = (v))
#define __raw_writeb(v,a) (*(volatile unsigned char *)(a) = (v))
#define __raw_readw(a)    (*(volatile unsigned short *)(a))
#define __raw_writew(v,a) (*(volatile unsigned short *)(a) = (v))
#define __raw_clearl(v,a) (__raw_writel(__raw_readl(a) & ~(v), a))
#define __raw_setl(v,a) (__raw_writel(__raw_readl(a) | (v), a))
#define __raw_field( v, bits, pos, a ) (__raw_writel( (__raw_readl(a) & (((1 << bits) - 1) << pos) ) | ((v) << pos), a))
#define __raw_setw(v,a) (__raw_writew(__raw_readw(a) | (v), a))
#define __raw_clearw(v,a) (__raw_writew(__raw_readw(a) & ~(v), a))


/* Register offsets of common modules */
/* Tap Information */
#define TAP_IDCODE_REG      (OMAP34XX_TAP_BASE+0x204)
#define PRODUCTION_ID      (OMAP34XX_TAP_BASE+0x208)

/* OMAP 34XX/35XX/36xx/37xx Control ID */
#define OMAP34XX_CONTROL_ID      (OMAP34XX_WAKEUP_L4_IO_BASE + 0xa204)

/* device type */
#define DEVICE_MASK      (BIT8|BIT9|BIT10)
#define TST_DEVICE      0x0
#define EMU_DEVICE      0x1
#define HS_DEVICE      0x2
#define GP_DEVICE      0x3

/* We are not concerned with BIT5 as it only determines
 *  the prirotiy between memory or perpheral booting
 */
#define SYSBOOT_MASK      (BIT0|BIT1|BIT2|BIT3|BIT4)

/* SMX-APE */
#define PM_RT_APE_BASE_ADDR_ARM      (SMX_APE_BASE + 0x10000)
#define PM_GPMC_BASE_ADDR_ARM      (SMX_APE_BASE + 0x12400)
#define PM_OCM_RAM_BASE_ADDR_ARM   (SMX_APE_BASE + 0x12800)
#define PM_OCM_ROM_BASE_ADDR_ARM   (SMX_APE_BASE + 0x12C00)
#define PM_IVA2_BASE_ADDR_ARM      (SMX_APE_BASE + 0x14000)

#define RT_REQ_INFO_PERMISSION_1   (PM_RT_APE_BASE_ADDR_ARM + 0x68)
#define RT_READ_PERMISSION_0   (PM_RT_APE_BASE_ADDR_ARM + 0x50)
#define RT_WRITE_PERMISSION_0   (PM_RT_APE_BASE_ADDR_ARM + 0x58)
#define RT_ADDR_MATCH_1   (PM_RT_APE_BASE_ADDR_ARM + 0x60)

#define GPMC_REQ_INFO_PERMISSION_0   (PM_GPMC_BASE_ADDR_ARM + 0x48)
#define GPMC_READ_PERMISSION_0   (PM_GPMC_BASE_ADDR_ARM + 0x50)
#define GPMC_WRITE_PERMISSION_0   (PM_GPMC_BASE_ADDR_ARM + 0x58)

#define OCM_REQ_INFO_PERMISSION_0   (PM_OCM_RAM_BASE_ADDR_ARM + 0x48)
#define OCM_READ_PERMISSION_0   (PM_OCM_RAM_BASE_ADDR_ARM + 0x50)
#define OCM_WRITE_PERMISSION_0   (PM_OCM_RAM_BASE_ADDR_ARM + 0x58)
#define OCM_ADDR_MATCH_2   (PM_OCM_RAM_BASE_ADDR_ARM + 0x80)

#define IVA2_REQ_INFO_PERMISSION_0   (PM_IVA2_BASE_ADDR_ARM + 0x48)
#define IVA2_READ_PERMISSION_0   (PM_IVA2_BASE_ADDR_ARM + 0x50)
#define IVA2_WRITE_PERMISSION_0   (PM_IVA2_BASE_ADDR_ARM + 0x58)

#define IVA2_REQ_INFO_PERMISSION_1   (PM_IVA2_BASE_ADDR_ARM + 0x68)
#define IVA2_READ_PERMISSION_1      (PM_IVA2_BASE_ADDR_ARM + 0x70)
#define IVA2_WRITE_PERMISSION_1      (PM_IVA2_BASE_ADDR_ARM + 0x78)

#define IVA2_REQ_INFO_PERMISSION_2   (PM_IVA2_BASE_ADDR_ARM + 0x88)
#define IVA2_READ_PERMISSION_2      (PM_IVA2_BASE_ADDR_ARM + 0x90)
#define IVA2_WRITE_PERMISSION_2      (PM_IVA2_BASE_ADDR_ARM + 0x98)

#define IVA2_REQ_INFO_PERMISSION_3   (PM_IVA2_BASE_ADDR_ARM + 0xA8)
#define IVA2_READ_PERMISSION_3      (PM_IVA2_BASE_ADDR_ARM + 0xB0)
#define IVA2_WRITE_PERMISSION_3      (PM_IVA2_BASE_ADDR_ARM + 0xB8)

/* I2C base */
#define I2C_BASE1      (OMAP34XX_CORE_L4_IO_BASE + 0x70000)
#define I2C_BASE2      (OMAP34XX_CORE_L4_IO_BASE + 0x72000)
#define I2C_BASE3      (OMAP34XX_CORE_L4_IO_BASE + 0x60000)

#endif
