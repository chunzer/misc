/*
 * linux/include/asm-mips/mach-jz4780/jz4780msc.h
 *
 * JZ4780 MSC register definition.
 *
 * Copyright (C) 2010 Ingenic Semiconductor Co., Ltd.
 */

#ifndef __JZ4780MSC_H__
#define __JZ4780MSC_H__

#define JZ_MAX_MSC_NUM 3

#define JZ_MSC_ID_INVALID(msc_id) ( ((msc_id) < 0) || ( (msc_id) > JZ_MAX_MSC_NUM ) )

#define	MSC0_BASE	0xB3450000
#define	MSC1_BASE	0xB3460000
#define	MSC2_BASE	0xB3470000

#define	MSC_STRPCL(n)		(MSC0_BASE + (n)*0x10000 + 0x000)
#define	MSC_STAT(n)		(MSC0_BASE + (n)*0x10000 + 0x004)
#define	MSC_CLKRT(n)		(MSC0_BASE + (n)*0x10000 + 0x008)
#define	MSC_CMDAT(n)		(MSC0_BASE + (n)*0x10000 + 0x00C)
#define	MSC_RESTO(n)		(MSC0_BASE + (n)*0x10000 + 0x010)
#define	MSC_RDTO(n)		(MSC0_BASE + (n)*0x10000 + 0x014)
#define	MSC_BLKLEN(n)		(MSC0_BASE + (n)*0x10000 + 0x018)
#define	MSC_NOB(n)		(MSC0_BASE + (n)*0x10000 + 0x01C)
#define	MSC_SNOB(n)		(MSC0_BASE + (n)*0x10000 + 0x020)
#define	MSC_IMASK(n)		(MSC0_BASE + (n)*0x10000 + 0x024)
#define	MSC_IREG(n)		(MSC0_BASE + (n)*0x10000 + 0x028)
#define	MSC_CMD(n)		(MSC0_BASE + (n)*0x10000 + 0x02C)
#define	MSC_ARG(n)		(MSC0_BASE + (n)*0x10000 + 0x030)
#define	MSC_RES(n)		(MSC0_BASE + (n)*0x10000 + 0x034)
#define	MSC_RXFIFO(n)		(MSC0_BASE + (n)*0x10000 + 0x038)
#define	MSC_TXFIFO(n)		(MSC0_BASE + (n)*0x10000 + 0x03C)
#define	MSC_LPM(n)		(MSC0_BASE + (n)*0x10000 + 0x040)
#define MSC_CTRL2(n)            (MSC0_BASE + (n)*0x10000 + 0x058)

#define	REG_MSC_STRPCL(n)	REG16(MSC_STRPCL(n))
#define	REG_MSC_STAT(n)		REG32(MSC_STAT(n))
#define	REG_MSC_CLKRT(n)	REG16(MSC_CLKRT(n))
#define	REG_MSC_CMDAT(n)	REG32(MSC_CMDAT(n))
#define	REG_MSC_RESTO(n)	REG16(MSC_RESTO(n))
#define	REG_MSC_RDTO(n)		REG32(MSC_RDTO(n))
#define	REG_MSC_BLKLEN(n)	REG16(MSC_BLKLEN(n))
#define	REG_MSC_NOB(n)		REG16(MSC_NOB(n))
#define	REG_MSC_SNOB(n)		REG16(MSC_SNOB(n))
#define	REG_MSC_IMASK(n)	REG32(MSC_IMASK(n))
#define	REG_MSC_IREG(n)		REG32(MSC_IREG(n))
#define	REG_MSC_CMD(n)		REG8(MSC_CMD(n))
#define	REG_MSC_ARG(n)		REG32(MSC_ARG(n))
#define	REG_MSC_RES(n)		REG16(MSC_RES(n))
#define	REG_MSC_RXFIFO(n)	REG32(MSC_RXFIFO(n))
#define	REG_MSC_TXFIFO(n)	REG32(MSC_TXFIFO(n))
#define	REG_MSC_LPM(n)		REG32(MSC_LPM(n))
#define REG_MSC_CTRL2(n)        REG32(MSC_CTRL2(n))

/* MSC Clock and Control Register (MSC_STRPCL) */
#define MSC_STRPCL_SEND_CCSD		(1 << 15) /*send command completion signal disable to ceata */
#define MSC_STRPCL_SEND_AS_CCSD		(1 << 14) /*send internally generated stop after sending ccsd */
#define MSC_STRPCL_EXIT_MULTIPLE	(1 << 7)
#define MSC_STRPCL_EXIT_TRANSFER	(1 << 6)
#define MSC_STRPCL_START_READWAIT	(1 << 5)
#define MSC_STRPCL_STOP_READWAIT	(1 << 4)
#define MSC_STRPCL_RESET		(1 << 3)
#define MSC_STRPCL_START_OP		(1 << 2)
#define MSC_STRPCL_CLOCK_CONTROL_BIT	0
#define MSC_STRPCL_CLOCK_CONTROL_MASK	(0x3 << MSC_STRPCL_CLOCK_CONTROL_BIT)
#define MSC_STRPCL_CLOCK_CONTROL_STOP	  (0x1 << MSC_STRPCL_CLOCK_CONTROL_BIT) /* Stop MMC/SD clock */
#define MSC_STRPCL_CLOCK_CONTROL_START  (0x2 << MSC_STRPCL_CLOCK_CONTROL_BIT) /* Start MMC/SD clock */

#define MSC_CTRL2_RST_EN                (1 << 23)
 
/* MSC Status Register (MSC_STAT) */
#define MSC_STAT_AUTO_CMD_DONE		(1 << 31) /*12 is internally generated by controller has finished */
#define MSC_STAT_BCE                    (1 << 20)
#define MSC_STAT_BDE                    (1 << 19)
#define MSC_STAT_BAE                    (1 << 18)
#define MSC_STAT_BAR                    (1 << 17)
#define MSC_STAT_IS_RESETTING		(1 << 15)
#define MSC_STAT_SDIO_INT_ACTIVE	(1 << 14)
#define MSC_STAT_PRG_DONE		(1 << 13)
#define MSC_STAT_DATA_TRAN_DONE		(1 << 12)
#define MSC_STAT_END_CMD_RES		(1 << 11)
#define MSC_STAT_DATA_FIFO_AFULL	(1 << 10)
#define MSC_STAT_IS_READWAIT		(1 << 9)
#define MSC_STAT_CLK_EN			(1 << 8)
#define MSC_STAT_DATA_FIFO_FULL		(1 << 7)
#define MSC_STAT_DATA_FIFO_EMPTY	(1 << 6)
#define MSC_STAT_CRC_RES_ERR		(1 << 5)
#define MSC_STAT_CRC_READ_ERROR		(1 << 4)
#define MSC_STAT_CRC_WRITE_ERROR_BIT	2
#define MSC_STAT_CRC_WRITE_ERROR_MASK	(0x3 << MSC_STAT_CRC_WRITE_ERROR_BIT)
#define MSC_STAT_CRC_WRITE_ERROR_NO	(0 << MSC_STAT_CRC_WRITE_ERROR_BIT) /* No error on transmission of data */
#define MSC_STAT_CRC_WRITE_ERROR	(1 << MSC_STAT_CRC_WRITE_ERROR_BIT) /* Card observed erroneous transmission of data */
#define MSC_STAT_CRC_WRITE_ERROR_NOSTS	(2 << MSC_STAT_CRC_WRITE_ERROR_BIT) /* No CRC status is sent back */
#define MSC_STAT_TIME_OUT_RES		(1 << 1)
#define MSC_STAT_TIME_OUT_READ		(1 << 0)

/* MSC Bus Clock Control Register (MSC_CLKRT) */
#define	MSC_CLKRT_CLK_RATE_BIT		0
#define	MSC_CLKRT_CLK_RATE_MASK		(0x7 << MSC_CLKRT_CLK_RATE_BIT)
#define MSC_CLKRT_CLK_RATE_DIV_1	(0x0 << MSC_CLKRT_CLK_RATE_BIT) /* CLK_SRC */
#define MSC_CLKRT_CLK_RATE_DIV_2	(0x1 << MSC_CLKRT_CLK_RATE_BIT) /* 1/2 of CLK_SRC */
#define MSC_CLKRT_CLK_RATE_DIV_4	(0x2 << MSC_CLKRT_CLK_RATE_BIT) /* 1/4 of CLK_SRC */
#define MSC_CLKRT_CLK_RATE_DIV_8	(0x3 << MSC_CLKRT_CLK_RATE_BIT) /* 1/8 of CLK_SRC */
#define MSC_CLKRT_CLK_RATE_DIV_16	(0x4 << MSC_CLKRT_CLK_RATE_BIT) /* 1/16 of CLK_SRC */
#define MSC_CLKRT_CLK_RATE_DIV_32	(0x5 << MSC_CLKRT_CLK_RATE_BIT) /* 1/32 of CLK_SRC */
#define MSC_CLKRT_CLK_RATE_DIV_64	(0x6 << MSC_CLKRT_CLK_RATE_BIT) /* 1/64 of CLK_SRC */
#define MSC_CLKRT_CLK_RATE_DIV_128	(0x7 << MSC_CLKRT_CLK_RATE_BIT) /* 1/128 of CLK_SRC */

/* MSC Command Sequence Control Register (MSC_CMDAT) */
#define	MSC_CMDAT_CCS_EXPECTED		(1 << 31) /* interrupts are enabled in ce-ata */
#define	MSC_CMDAT_READ_CEATA		(1 << 30)
#define MSC_CMDAT_DIS_BOOT              (1 << 27)
#define MSC_CMDAT_ENB_BOOT              (1 << 26)
#define MSC_CMDAT_EXP_BOOT_ACK          (1 << 25)
#define MSC_CMDAT_BOOT_MODE_A           (1 << 24)
#define	MSC_CMDAT_SDIO_PRDT		(1 << 17) /* exact 2 cycle */
#define	MSC_CMDAT_SEND_AS_STOP		(1 << 16)
#define	MSC_CMDAT_RTRG_BIT		14
#define MSC_CMDAT_RTRG_EQUALT_8		(0x0 << MSC_CMDAT_RTRG_BIT)
#define MSC_CMDAT_RTRG_EQUALT_16	(0x1 << MSC_CMDAT_RTRG_BIT) /* reset value */
#define MSC_CMDAT_RTRG_EQUALT_24	(0x2 << MSC_CMDAT_RTRG_BIT)
#define	MSC_CMDAT_TTRG_BIT		12
#define MSC_CMDAT_TTRG_LESS_8		(0x0 << MSC_CMDAT_TTRG_BIT)
#define MSC_CMDAT_TTRG_LESS_16		(0x1 << MSC_CMDAT_TTRG_BIT) /*reset value  */
#define MSC_CMDAT_TTRG_LESS_24		(0x2 << MSC_CMDAT_TTRG_BIT)
#define	MSC_CMDAT_STOP_ABORT		(1 << 11)
#define	MSC_CMDAT_BUS_WIDTH_BIT		9
#define	MSC_CMDAT_BUS_WIDTH_MASK	(0x3 << MSC_CMDAT_BUS_WIDTH_BIT)
#define MSC_CMDAT_BUS_WIDTH_1BIT	(0x0 << MSC_CMDAT_BUS_WIDTH_BIT) /* 1-bit data bus */
#define MSC_CMDAT_BUS_WIDTH_4BIT	(0x2 << MSC_CMDAT_BUS_WIDTH_BIT) /* 4-bit data bus */
#define MSC_CMDAT_BUS_WIDTH_8BIT	(0x3 << MSC_CMDAT_BUS_WIDTH_BIT) /* 8-bit data bus */
#define	MSC_CMDAT_DMA_EN		(1 << 8)
#define	MSC_CMDAT_INIT			(1 << 7)
#define	MSC_CMDAT_BUSY			(1 << 6)
#define	MSC_CMDAT_STREAM_BLOCK		(1 << 5)
#define	MSC_CMDAT_WRITE			(1 << 4)
#define	MSC_CMDAT_READ			(0 << 4)
#define	MSC_CMDAT_DATA_EN		(1 << 3)
#define	MSC_CMDAT_RESPONSE_BIT	0
#define	MSC_CMDAT_RESPONSE_MASK	(0x7 << MSC_CMDAT_RESPONSE_BIT)
#define MSC_CMDAT_RESPONSE_NONE (0x0 << MSC_CMDAT_RESPONSE_BIT) /* No response */
#define MSC_CMDAT_RESPONSE_R1	  (0x1 << MSC_CMDAT_RESPONSE_BIT) /* Format R1 and R1b */
#define MSC_CMDAT_RESPONSE_R2	  (0x2 << MSC_CMDAT_RESPONSE_BIT) /* Format R2 */
#define MSC_CMDAT_RESPONSE_R3	  (0x3 << MSC_CMDAT_RESPONSE_BIT) /* Format R3 */
#define MSC_CMDAT_RESPONSE_R4	  (0x4 << MSC_CMDAT_RESPONSE_BIT) /* Format R4 */
#define MSC_CMDAT_RESPONSE_R5	  (0x5 << MSC_CMDAT_RESPONSE_BIT) /* Format R5 */
#define MSC_CMDAT_RESPONSE_R6	  (0x6 << MSC_CMDAT_RESPONSE_BIT) /* Format R6 */

/* MSC Interrupts Mask Register (MSC_IMASK) */
#define	MSC_IMASK_AUTO_CMD_DONE		BIT15
#define MSC_IMASK_DATA_FIFO_FULL	BIT14
#define MSC_IMASK_DATA_FIFO_EMP		BIT13
#define MSC_IMASK_CRC_RES_ERR		BIT12
#define MSC_IMASK_CRC_READ_ERR		BIT11
#define MSC_IMASK_CRC_WRITE_ERR		BIT10
#define MSC_IMASK_TIMEOUT_RES		BIT9
#define MSC_IMASK_TIMEOUT_READ		BIT8
#define	MSC_IMASK_SDIO			BIT7
#define	MSC_IMASK_TXFIFO_WR_REQ		BIT6
#define	MSC_IMASK_RXFIFO_RD_REQ		BIT5
#define	MSC_IMASK_END_CMD_RES		BIT2
#define	MSC_IMASK_PRG_DONE		BIT1
#define	MSC_IMASK_DATA_TRAN_DONE	BIT0

/* MSC Interrupts Status Register (MSC_IREG) */
#define	MSC_IREG_AUTO_CMD_DONE		BIT15
#define MSC_IREG_DATA_FIFO_FULL		BIT14
#define MSC_IREG_DATA_FIFO_EMP		BIT13
#define MSC_IREG_CRC_RES_ERR		BIT12
#define MSC_IREG_CRC_READ_ERR		BIT11
#define MSC_IREG_CRC_WRITE_ERR		BIT10
#define MSC_IREG_TIMEOUT_RES		BIT9
#define MSC_IREG_TIMEOUT_READ		BIT8
#define	MSC_IREG_SDIO			BIT7
#define	MSC_IREG_TXFIFO_WR_REQ		BIT6
#define	MSC_IREG_RXFIFO_RD_REQ		BIT5
#define	MSC_IREG_END_CMD_RES		BIT2
#define	MSC_IREG_PRG_DONE		BIT1
#define	MSC_IREG_DATA_TRAN_DONE		BIT0

/* MSC Low Power Mode Register (MSC_LPM) */
#define	MSC_SET_LPM			(1 << 0)


#ifndef __MIPS_ASSEMBLER

/***************************************************************************
 * MSC
 ***************************************************************************/
/* n = 0, 1 (MSC0, MSC1) */

#define __msc_start_op(n)						\
	( REG_MSC_STRPCL(n) = MSC_STRPCL_START_OP | MSC_STRPCL_CLOCK_CONTROL_START )

#define __msc_set_resto(n, to)  	( REG_MSC_RESTO(n) = to )
#define __msc_set_rdto(n, to)   	( REG_MSC_RDTO(n) = to )
#define __msc_set_cmd(n, cmd)   	( REG_MSC_CMD(n) = cmd )
#define __msc_set_arg(n, arg)   	( REG_MSC_ARG(n) = arg )
#define __msc_set_nob(n, nob)      	( REG_MSC_NOB(n) = nob )
#define __msc_get_nob(n)        	( REG_MSC_NOB(n) )
#define __msc_set_blklen(n, len)        ( REG_MSC_BLKLEN(n) = len )
#define __msc_set_cmdat(n, cmdat)   	( REG_MSC_CMDAT(n) = cmdat )
#define __msc_set_cmdat_ioabort(n) 	( REG_MSC_CMDAT(n) |= MSC_CMDAT_IO_ABORT )
#define __msc_clear_cmdat_ioabort(n) 	( REG_MSC_CMDAT(n) &= ~MSC_CMDAT_IO_ABORT )

#define __msc_set_cmdat_bus_width1(n)				\
	do { 							\
		REG_MSC_CMDAT(n) &= ~MSC_CMDAT_BUS_WIDTH_MASK; 	\
		REG_MSC_CMDAT(n) |= MSC_CMDAT_BUS_WIDTH_1BIT; 	\
	} while(0)

#define __msc_set_cmdat_bus_width4(n)				\
	do { 							\
		REG_MSC_CMDAT(n) &= ~MSC_CMDAT_BUS_WIDTH_MASK; 	\
		REG_MSC_CMDAT(n) |= MSC_CMDAT_BUS_WIDTH_4BIT; 	\
	} while(0)

#define __msc_set_cmdat_dma_en(n)       ( REG_MSC_CMDAT(n) |= MSC_CMDAT_DMA_EN )
#define __msc_set_cmdat_init(n) 	( REG_MSC_CMDAT(n) |= MSC_CMDAT_INIT )
#define __msc_set_cmdat_busy(n) 	( REG_MSC_CMDAT(n) |= MSC_CMDAT_BUSY )
#define __msc_set_cmdat_stream(n)       ( REG_MSC_CMDAT(n) |= MSC_CMDAT_STREAM_BLOCK )
#define __msc_set_cmdat_block(n)        ( REG_MSC_CMDAT(n) &= ~MSC_CMDAT_STREAM_BLOCK )
#define __msc_set_cmdat_read(n) 	( REG_MSC_CMDAT(n) &= ~MSC_CMDAT_WRITE_READ )
#define __msc_set_cmdat_write(n)        ( REG_MSC_CMDAT(n) |= MSC_CMDAT_WRITE_READ )
#define __msc_set_cmdat_data_en(n)      ( REG_MSC_CMDAT(n) |= MSC_CMDAT_DATA_EN )

/* r is MSC_CMDAT_RESPONSE_FORMAT_Rx or MSC_CMDAT_RESPONSE_FORMAT_NONE */
#define __msc_set_cmdat_res_format(n, r)				\
	do { 								\
		REG_MSC_CMDAT(n) &= ~MSC_CMDAT_RESPONSE_FORMAT_MASK; 	\
		REG_MSC_CMDAT(n) |= (r);				\
	} while(0)

#define __msc_clear_cmdat(n)						\
	REG_MSC_CMDAT(n) &= ~( MSC_CMDAT_IO_ABORT | MSC_CMDAT_DMA_EN | MSC_CMDAT_INIT| \
			       MSC_CMDAT_BUSY | MSC_CMDAT_STREAM_BLOCK | MSC_CMDAT_WRITE_READ | \
			       MSC_CMDAT_DATA_EN | MSC_CMDAT_RESPONSE_FORMAT_MASK )

#define __msc_get_imask(n) 		( REG_MSC_IMASK(n) )
#define __msc_mask_all_intrs(n) 	( REG_MSC_IMASK(n) = 0xff )
#define __msc_unmask_all_intrs(n) 	( REG_MSC_IMASK(n) = 0x00 )
#define __msc_mask_rd(n) 		( REG_MSC_IMASK(n) |= MSC_IMASK_RXFIFO_RD_REQ )
#define __msc_unmask_rd(n) 		( REG_MSC_IMASK(n) &= ~MSC_IMASK_RXFIFO_RD_REQ )
#define __msc_mask_wr(n) 		( REG_MSC_IMASK(n) |= MSC_IMASK_TXFIFO_WR_REQ )
#define __msc_unmask_wr(n) 		( REG_MSC_IMASK(n) &= ~MSC_IMASK_TXFIFO_WR_REQ )
#define __msc_mask_endcmdres(n) 	( REG_MSC_IMASK(n) |= MSC_IMASK_END_CMD_RES )
#define __msc_unmask_endcmdres(n) 	( REG_MSC_IMASK(n) &= ~MSC_IMASK_END_CMD_RES )
#define __msc_mask_datatrandone(n) 	( REG_MSC_IMASK(n) |= MSC_IMASK_DATA_TRAN_DONE )
#define __msc_unmask_datatrandone(n) 	( REG_MSC_IMASK(n) &= ~MSC_IMASK_DATA_TRAN_DONE )
#define __msc_mask_prgdone(n) 		( REG_MSC_IMASK(n) |= MSC_IMASK_PRG_DONE )
#define __msc_unmask_prgdone(n) 	( REG_MSC_IMASK(n) &= ~MSC_IMASK_PRG_DONE )

/* m=0,1,2,3,4,5,6,7 */
#define __msc_set_clkrt(n, m)			\
	do {					\
		REG_MSC_CLKRT(n) = m;		\
	} while(0)

#define __msc_get_ireg(n) 	        	( REG_MSC_IREG(n) )
#define __msc_ireg_rd(n) 	        	( REG_MSC_IREG(n) & MSC_IREG_RXFIFO_RD_REQ )
#define __msc_ireg_wr(n) 	        	( REG_MSC_IREG(n) & MSC_IREG_TXFIFO_WR_REQ )
#define __msc_ireg_end_cmd_res(n)       	( REG_MSC_IREG(n) & MSC_IREG_END_CMD_RES )
#define __msc_ireg_data_tran_done(n)     	( REG_MSC_IREG(n) & MSC_IREG_DATA_TRAN_DONE )
#define __msc_ireg_prg_done(n) 	        	( REG_MSC_IREG(n) & MSC_IREG_PRG_DONE )
#define __msc_ireg_clear_end_cmd_res(n)         ( REG_MSC_IREG(n) = MSC_IREG_END_CMD_RES )
#define __msc_ireg_clear_data_tran_done(n)      ( REG_MSC_IREG(n) = MSC_IREG_DATA_TRAN_DONE )
#define __msc_ireg_clear_prg_done(n)     	( REG_MSC_IREG(n) = MSC_IREG_PRG_DONE )

#define __msc_get_stat(n) 		( REG_MSC_STAT(n) )
#define __msc_stat_not_end_cmd_res(n) 	( (REG_MSC_STAT(n) & MSC_STAT_END_CMD_RES) == 0)
#define __msc_stat_crc_err(n)						\
	( REG_MSC_STAT(n) & (MSC_STAT_CRC_RES_ERR | MSC_STAT_CRC_READ_ERROR | MSC_STAT_CRC_WRITE_ERROR_YES) )
#define __msc_stat_res_crc_err(n) 	( REG_MSC_STAT(n) & MSC_STAT_CRC_RES_ERR )
#define __msc_stat_rd_crc_err(n) 	( REG_MSC_STAT(n) & MSC_STAT_CRC_READ_ERROR )
#define __msc_stat_wr_crc_err(n) 	( REG_MSC_STAT(n) & MSC_STAT_CRC_WRITE_ERROR_YES )
#define __msc_stat_resto_err(n) 	( REG_MSC_STAT(n) & MSC_STAT_TIME_OUT_RES )
#define __msc_stat_rdto_err(n) 		( REG_MSC_STAT(n) & MSC_STAT_TIME_OUT_READ )

#define __msc_rd_resfifo(n) 		( REG_MSC_RES(n) )
#define __msc_rd_rxfifo(n)  		( REG_MSC_RXFIFO(n) )
#define __msc_wr_txfifo(n, v)  		( REG_MSC_TXFIFO(n) = v )

#define __msc_reset(n)							\
	do { 								\
		REG_MSC_STRPCL(n) = MSC_STRPCL_RESET;			\
		REG_MSC_STRPCL(n) &= ~MSC_STRPCL_RESET;			\
	} while (0)

#define __msc_start_clk(n)						\
	do { 								\
		REG_MSC_STRPCL(n) = MSC_STRPCL_CLOCK_CONTROL_START;	\
	} while (0)

#define __msc_stop_clk(n)						\
	do { 								\
		REG_MSC_STRPCL(n) = MSC_STRPCL_CLOCK_CONTROL_STOP;	\
	} while (0)

#define MMC_CLK 19169200
#define SD_CLK  24576000

/* msc_clk should little than pclk and little than clk retrieve from card */
#define __msc_calc_clk_divisor(type,dev_clk,msc_clk,lv)	\
	do {						\
		unsigned int rate, pclk, i;		\
		pclk = dev_clk;				\
		rate = type?SD_CLK:MMC_CLK;		\
		if (msc_clk && msc_clk < pclk)		\
			pclk = msc_clk;			\
		i = 0;					\
		while (pclk < rate)			\
		{					\
			i ++;				\
			rate >>= 1;			\
		}					\
		lv = i;					\
	} while(0)

/* divide rate to little than or equal to 400kHz */
#define __msc_calc_slow_clk_divisor(type, lv)		\
	do {						\
		unsigned int rate, i;			\
		rate = (type?SD_CLK:MMC_CLK)/1000/400;	\
		i = 0;					\
		while (rate > 0)			\
		{					\
			rate >>= 1;			\
			i ++;				\
		}					\
		lv = i;					\
	} while(0)



#endif /* __MIPS_ASSEMBLER */

#endif /* __JZ4780MSC_H__ */

