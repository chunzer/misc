#ifndef __OVISP_CSI_H__
#define __OVISP_CSI_H__

#include <linux/errno.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/ovisp-regs.h>

#define CSI_NUM			(2)

#define CSI_ERROR_STAT(id)	(CTL_CPHY0_ERROR_STAT + (id * 4))
#define CSI_STAT(id)		(CTL_CPHY0_STAT + (id * 4))
#define CSI_RSTZ(id)		(CTL_CPHY0_RSTZ + (id * 4))
#define CSI_CTRL0(id)		(CTL_CPHY0_CTRL0 + (id * 8))
#define CSI_CTRL1(id)		(CTL_CPHY0_CTRL1 + (id * 8))
#define CSI_MODE(id)		(CTL_CPHY0_MODE + (id * 4))

static inline unsigned int csi_reg_readl(unsigned int reg)
{
return 0;
}
static inline void csi_reg_writel(unsigned int value, unsigned int reg)
{
return 0;
}

static inline void csi_reg_write_part(unsigned int reg, unsigned int value,
			unsigned char shift, unsigned char width)
{
         unsigned int mask = (1 << width) - 1;
         unsigned int temp = csi_reg_readl(reg);

         temp &= ~(mask << shift);
         temp |= (value & mask) << shift;
         csi_reg_writel(temp, reg);
}

extern int csi_phy_init(void);
extern int csi_phy_release(void);
extern int csi_phy_start(unsigned int id, unsigned int freq);
extern int csi_phy_stop(unsigned int id);

#endif/*__OVISP_CSI_H__*/
