/*
 * nand_boot.c
 *
 * Internal BootROM NAND_BOOT code for JZ4775.
 *
 * Copyright (C) 2011 - 2012 Ingenic Semiconductor Inc.
 *
 * Authors:	<xlsu@ingenic.cn>
 */
#include <irom.h>
#include <common.h>
#include <jz4775misc.h>
#include <jz4775gpio.h>
#include <jz4775cpm.h>

#include <jz4775nemc.h>
#include <jz4775bch.h>
#include <nand_boot.h>

#define SPL_SIZE	(14 * 1024) /* Loaded SPL Size */
#define FLAG_SIZE	256 /* MUST be a Even Integer */

#define SPL_OFFSET	FLAG_SIZE /* MUST Align to WORD*/

#define BACKUP_OFFSET	128
#define BACKUP_NUM	8
#define BACKUP_PAGE	(BACKUP_OFFSET * BACKUP_NUM)

/* ECC Level: 64 bit/256bytes */
#define ECC_SIZE		256
#define ECC_BYTES		112 /* 14 * ECC Level / 8 */
#define ECCBUF_SIZE		ECC_BYTES

/* NAND Initial Value */
#define PAGESIZE_DEFVALUE	512
#define BUSWIDTH_DEFVALUE	8 /* 8 buswidth 8bit, 16 buswidth 16bit */
#define ROWCYCLE_DEFVALUE	3
#define NANDTYPE_DEFVALUE	0 /* Common NAND default */

#define NANDTYPE_COMMON		0
#define NANDTYPE_TOGGLE		1

#define UNCOR_ECC		0xFFFF
#define TOGGLE_DQS_DELAY	0x1F

#define BUSWIDTH_FLAG_OFFSET	0				/* [0 : 63] */
#define NANDTYPE_FLAG_OFFSET	(BUSWIDTH_FLAG_OFFSET + 64)	/* [64 : 127] */
#define ROWCYCLE_FLAG_OFFSET	(NANDTYPE_FLAG_OFFSET + 64)	/* [128 : 159] */
#define PAGESIZE_FLAG2_OFFSET	(ROWCYCLE_FLAG_OFFSET + 32)	/* [160 : 191] */
#define PAGESIZE_FLAG1_OFFSET	(ROWCYCLE_FLAG_OFFSET + 32 * 2)	/* [192 : 223] */
#define PAGESIZE_FLAG0_OFFSET	(ROWCYCLE_FLAG_OFFSET + 32 * 3)	/* [224 : 255] */

#define FLAG_BUSWIDTH_8BIT	0x55
#define FLAG_BUSWIDTH_16BIT	0xaa
#define FLAG_NANDTYPE_COMMON	0x55
#define FLAG_NANDTYPE_TOGGLE	0xaa
#define FLAG_ROWCYCLE_2		0x55
#define FLAG_ROWCYCLE_3		0xaa
#define FLAG_PAGESIZE_512	0x555555
#define FLAG_PAGESIZE_2K	0x55aa55
#define FLAG_PAGESIZE_4K	0xaa5555
#define FLAG_PAGESIZE_8K	0xaaaa55
#define FLAG_PAGESIZE_16K	0xaaaaaa
#define FLAG_NULL		0

#define FLAG_STORE_OFFSET	0x800

#define USE_PN			1

extern u32 start_addr;
extern int cpustate;
extern int sd_boot(int);

/*
 * nand_wait_rb - Wait NAND R/B# Ready
 *
 * NOTE:
 *	NAND tWB MAX is 200ns at present.
 * 	timeout is for the NAND R/B#'s falling edge time.
 *	prevent trap in the loop after missed falling edge.
 */
static void nand_wait_rb(void)
{
	xtimeout(1, 1); /* timeout 1us */

	while ((REG_GPIO_PXPIN(0) & 0x00100000) && xtimeout(1, 0));
	while (!(REG_GPIO_PXPIN(0) & 0x00100000));
}

/*
 * nand_reset - Reset NAND
 */
static inline void nand_reset(void)
{
	__nand_cmd(NAND_CMD_RESET);
	nand_wait_rb();
}

/*
 * nand_send_rdcmd_and_addr - Send NAND read commond 0x00 and address then common 0x30
 */
static void nand_send_rdcmd_and_addr(struct nand_info *chip, u32 col, u32 row)
{
	int i;

	__nand_cmd(NAND_CMD_READ0); /* Send command 0x00 */

	__nand_addr(col & 0xFF);
	if (chip->page_size != 512) { /* if page size is 512B, NOT to write second col.addr */
		__nand_addr((col >> 8) & 0xFF);
	}

	for (i = 0; i < chip->row_cycle; i++) {
		__nand_addr(row & 0xFF);
		row >>= 8;
	}

	if (chip->page_size != 512) /* Send command 0x30 for 2KB or 4KB flash */
		__nand_cmd(NAND_CMD_READSTART);
}

/*
 * nand_read_buf - Read data from NAND
 */
static void (*nand_read_buf)(struct nand_info *chip, u8 *data_buf, u32 rd_size);
static void nand_read_buf8(struct nand_info *chip, u8 *data_buf, u32 rd_size)
{
	int i;

	for (i = 0; i < rd_size; i++)
		__nand_data_out8((u8 *)data_buf + i);
}

static void nand_read_buf16(struct nand_info *chip, u8 *data_buf, u32 rd_size)
{
	int i;

	/* All data size are Even Integer, need not to think Byte data */
	for (i = 0; i < rd_size; i += 2)
		__nand_data_out16((u8 *)data_buf + i);
}

/*
 * bch_enable_64bit - INIT Hardware BCH for 64bit ECC
 */
static void bch_enable_64bit(void)
{
	__bch_cnt_set(ECC_SIZE, ECC_BYTES);
	__bch_decoding_64bit();
}

static void bch_decode_fill_dr(u8 *data_buf, u8 *ecc_buf)
{
	int i;

	/* Write data to REG_BCH_DR */
	for (i = 0; i < ECC_SIZE; i++) {
		REG_BCH_DR = data_buf[i];
	}
	/* Write parities to REG_BCH_DR */
	for (i = 0; i < ECC_BYTES; i++) {
		REG_BCH_DR = ecc_buf[i];
	}
}

static void bch_error_correct(u16 *data_buf, int err_bit)
{
	u32 err_mask, idx; /* the 'bit' of idx half-word is error */

	idx = (REG_BCH_ERR(err_bit) & BCH_ERR_INDEX_MASK) >> BCH_ERR_INDEX_BIT;
	err_mask = (REG_BCH_ERR(err_bit) & BCH_ERR_MASK_MASK) >> BCH_ERR_MASK_BIT;

	data_buf[idx] ^= (u16)err_mask;
}

static void bch_correct_handle(u8 *data_buf, int *report)
{
	u32 err_cnt, status;
	int i;

	/* Get BCH Status */
	status = REG_BCH_INTS;

	if (status & BCH_INTS_UNCOR) {
		*report = UNCOR_ECC; /* Uncorrectable ECC Error*/
	} else if (status & BCH_INTS_ERR) {
		err_cnt = (status & BCH_INTS_ERRC_MASK) >> BCH_INTS_ERRC_BIT;
		*report = err_cnt;

		for (i = 0; i < err_cnt; i++)
			bch_error_correct((u16 *)data_buf, i);
	} else {
		*report = 0;
	}
}

/*
 * do_nand_hwbch_decode - BCH 64bit/256byte ECClevel decode
 */
static int do_nand_hwbch_decode(u8 *data_buf, u8 *ecc_buf)
{
	int report;

	bch_enable_64bit();
	bch_decode_fill_dr(data_buf, ecc_buf);

	/* Wait for BCH decode completion */
	__bch_decode_sync();
	bch_correct_handle(data_buf, &report);

	__bch_decints_clear();
	__bch_disable();

	return report != UNCOR_ECC ? 0 : -1;
}

/*
 * nand_page_read - Read 1 page data from NAND
 *
 * @chip:	nand_info structure
 * @page:	data page offset. Data's ECC in the Next page
 * @page_cnt:	readed page counter
 * @data_buf:	data pointer at Memory
 * @ecc_buf:	ECC pointer at Memory
 * @tail_size:	Residual Data need to Read
 *
 * NOTE:
 * 	Don't do PN when reading first 256 bytes of the Every Boot Backup.
 * 	col_offs MUST be multiply of 2 Bytes, Col.A0 MUST set to be 0.
 */
static int nand_page_read(struct nand_info *chip, u32 page, const u32 page_cnt,
			u8 *data_buf, u8 *ecc_buf, u32 tail_size)
{
	u32 ecc_cnt;
	u32 rd_size = tail_size == 0 ? chip->page_size : tail_size;
	int i, result;

	ecc_cnt = rd_size / ECC_SIZE;
	
	/* Toggle NAND Enable CE# */
	if (chip->nand_type == NANDTYPE_TOGGLE)
		__tnand_enable();

	/* Read data page data */
	nand_send_rdcmd_and_addr(chip, 0x00, page);
	nand_wait_rb();

	if (chip->nand_type == NANDTYPE_TOGGLE)
		__tnand_datard_perform();

	for (i = 0; i < ecc_cnt; i++) {
#ifdef USE_PN
		if (i || page_cnt)
			__pn_enable(); /* PN Enable */
#endif
		nand_read_buf(chip, data_buf + i * ECC_SIZE, ECC_SIZE);
	}
#ifdef USE_PN
	__pn_disable(); /* PN Disable */
#endif

	if (chip->nand_type == NANDTYPE_TOGGLE) {
		__tnand_disable();
		__tnand_enable();
	} else { /* chip->nand_type is NANDTYPE_COMMON */
		__nand_disable();
		__nand_enable();
	}

	/* Read ecc page data */
	nand_send_rdcmd_and_addr(chip, 0x00, page + 1);
	nand_wait_rb();

	if (chip->nand_type == NANDTYPE_TOGGLE)
		__tnand_datard_perform();
#ifdef USE_PN
	__pn_enable(); /* PN Enable */
#endif
	/* BCH ECC Correct data */
	for (i = 0; i < ecc_cnt; i++) {
		nand_read_buf(chip, ecc_buf, ECC_BYTES);
		result = do_nand_hwbch_decode(data_buf, ecc_buf);

		if (result < 0) { /* Uncorrectable ECC error */
#ifdef USE_PN
			__pn_disable(); /* PN Disable */
#endif
			if (chip->nand_type == NANDTYPE_TOGGLE)
				__tnand_disable();

			return -1;
		}

		data_buf += ECC_SIZE;
	}
#ifdef USE_PN
	__pn_disable(); /* PN Disable */
#endif

	/* Toggle NAND Disable FCE# */
	if (chip->nand_type == NANDTYPE_TOGGLE)
		__tnand_disable();

	return 0;
}

/*
 * nand_spl_load - Read boot data to cache
 *
 * @chip:	nand_info structure
 * @start_page:	the page in NAND start to read
 */
static int nand_spl_load(struct nand_info *chip, u32 start_page)
{
	u8 *data_buf = (u8 *)start_addr;
	u8 ecc_buf[ECCBUF_SIZE];
	u32 page_size = chip->page_size;
	u32 tail_size, pagecnt = 0;
	int i, result;

	for (i = 0; i * page_size < SPL_SIZE; i++) {
		if ((SPL_SIZE - i * page_size) < page_size)
			tail_size = (SPL_SIZE & (page_size - 1));
		else
			tail_size = 0;

		result = nand_page_read(chip, start_page + (i << 1), pagecnt,
				data_buf + i * page_size, ecc_buf, tail_size);
		if (result < 0)
			return -1;

		pagecnt++;
	}

	return 0;
}

//-----------------------------------------------------------------------------/
static void parse_flag(u8 *flag_buf, u32 *flag)
{
	int cnt_55 = 0, cnt_aa = 0;
	int i;

	for (i = 0; i < 32; i++) {
		switch (flag_buf[i]) {
		case 0x55 :
			cnt_55++;
			break;
		case 0xaa :
			cnt_aa++;
			break;
		default :
			break;
		}
	}

	if ((cnt_55 - cnt_aa) > 7)
		*flag = 0x55;
	else if ((cnt_aa - cnt_55) > 7)
		*flag = 0xaa;
	else
		*flag = FLAG_NULL;
}

static int nand_buswidth_get(struct nand_info *chip, u32 nand_flag)
{
	switch (nand_flag) {
	case FLAG_BUSWIDTH_8BIT :
		chip->bus_width = 8;
		break;
	case FLAG_BUSWIDTH_16BIT :
		chip->bus_width = 16;
		break;
	default :
		return -1;
	}

	return 0;
}

static int nand_nandtype_get(struct nand_info *chip, u8 *flag_buf)
{
	u32 nandtype_flag;

	parse_flag(flag_buf + NANDTYPE_FLAG_OFFSET, & nandtype_flag);

	switch (nandtype_flag) {
	case FLAG_NANDTYPE_COMMON :
		chip->nand_type = NANDTYPE_COMMON;
		break;
	case FLAG_NANDTYPE_TOGGLE :
		chip->nand_type = NANDTYPE_TOGGLE;
		break;
	default :
		return -1;
	}

	return 0;
}

static int nand_rowcycle_get(struct nand_info *chip, u8 *flag_buf)
{
	u32 row_flag;

	parse_flag(flag_buf + ROWCYCLE_FLAG_OFFSET, &row_flag);

	switch (row_flag) {
	case FLAG_ROWCYCLE_2 :
		chip->row_cycle = 2;
		break;
	case FLAG_ROWCYCLE_3 :
		chip->row_cycle = 3;
		break;
	default :
		return -1;
	}

	return 0;
}

static int nand_pagesize_get(struct nand_info *chip, u8 *flag_buf)
{
	u32 pagesize_flag0, pagesize_flag1, pagesize_flag2;

	parse_flag(flag_buf + PAGESIZE_FLAG2_OFFSET, &pagesize_flag2);
	parse_flag(flag_buf + PAGESIZE_FLAG1_OFFSET, &pagesize_flag1);
	parse_flag(flag_buf + PAGESIZE_FLAG0_OFFSET, &pagesize_flag0);

	switch ((pagesize_flag2 << 16) | (pagesize_flag1 << 8) | pagesize_flag0) {
	case FLAG_PAGESIZE_512 :
		chip->page_size = 512;
		break;
	case FLAG_PAGESIZE_2K :
		chip->page_size = 2 * 1024;
		break;
	case FLAG_PAGESIZE_4K :
		chip->page_size = 4 * 1024;
		break;
	case FLAG_PAGESIZE_8K :
		chip->page_size = 8 * 1024;
		break;
	case FLAG_PAGESIZE_16K :
		chip->page_size = 16 * 1024;
		break;
	default :
		return -1;
	}

	return 0;
}

/*
 * nand_set_8bit - Config NAND GPIO, NEMC_SMCR and select read function
 */
static void nand_set_8bit(void)
{
	/* We have already set CS#, FRE#, FWE#, CLE, ALE, FRB# before. */
	__gpio_as_nand_8bit();
	/* Set NEMC buswidth 8bit */
	__nand_smcr_8bit();
	/* Set buf read 8bit mode */
	nand_read_buf = nand_read_buf8;
}

/*
 * nand_set_16bit - Config NAND GPIO, NEMC_SMCR and select read function
 */
static void nand_set_16bit(void)
{
	/* We have already set CS#, FRE#, FWE#, CLE, ALE, FRB# before. */
	__gpio_as_nand_16bit();
	/* Set NEMC buswidth 16bit */
	__nand_smcr_16bit();
	/* Set buf read 16bit mode */
	nand_read_buf = nand_read_buf16;
}


static void nand_get_flags(struct nand_info *chip, u8 *flag_buf,
		u32 flag_offset, u32 page)
{
	if (chip->nand_type == NANDTYPE_TOGGLE)
		__tnand_enable();

	nand_send_rdcmd_and_addr(chip, flag_offset, page);
	nand_wait_rb();

	if (chip->nand_type == NANDTYPE_TOGGLE)
		__tnand_datard_perform();
	nand_read_buf(chip, flag_buf, ECC_SIZE - flag_offset);

	if (chip->nand_type == NANDTYPE_TOGGLE)
		__tnand_disable();
}

static int nand_setup(struct nand_info *chip, u32 start_page)
{
	u8 *flag_buf = (u8 *)(start_addr + FLAG_STORE_OFFSET);
	u32 check_flag;
	int ret;

	nand_get_flags(chip, flag_buf, 0, start_page);
	parse_flag(flag_buf, &check_flag);

	/* if 512 mode read failed or Toggle NAND, try to use 2048 mode */
	if (check_flag == FLAG_NULL) {
		chip->page_size = 2048; /* just 512 is different from 2k, 4k, 8, ... */
		nand_get_flags(chip, flag_buf, 0, start_page);
		parse_flag(flag_buf, &check_flag);
		if (check_flag == FLAG_NULL)
			return -1;
	}

	ret = nand_buswidth_get(chip, check_flag);
	if (ret < 0)
		return -1;

	/* if NAND buswidth is 16bit, config NAND BW 16bit and NAND type common */
	if (chip->bus_width == 16) {
		chip->nand_type = NANDTYPE_COMMON;
		nand_set_16bit();
	} else { /* Only 8bit NAND need to check nandtype */
		/* We need to read NAND type pattern again for toggle */
		nand_get_flags(chip, flag_buf + NANDTYPE_FLAG_OFFSET,
				NANDTYPE_FLAG_OFFSET, start_page);
		ret = nand_nandtype_get(chip, flag_buf);
		if (ret < 0)
			return -1;

		if (chip->nand_type == NANDTYPE_TOGGLE) {
			__nand_disable();
			__gpio_as_toggle_nand();

			/* Only CPU state is Power ON, Init DQS Delay */
			if ((cpustate & RSR_WR) || (cpustate & RSR_PR))
				__tnand_dqsdelay_init(TOGGLE_DQS_DELAY);
		}
	}

	/* if NAND buswdith is 16bit or toggle NAND, we need to reread NAND partern */
	if (chip->bus_width == 16 || chip->nand_type == NANDTYPE_TOGGLE)
		nand_get_flags(chip, flag_buf, 0, start_page);

	ret = nand_rowcycle_get(chip, flag_buf);
	if (ret < 0)
		return -1;

	ret = nand_pagesize_get(chip, flag_buf);
	if (ret < 0)
		return -1;

	return 0;
}

/*
 * nand_info_default - Init NAND arguments to default value
 */
static inline void nand_info_default(struct nand_info *chip)
{
	/* buswidth_flag init to 0, indecate buswidth is 8bit */
	chip->bus_width = BUSWIDTH_DEFVALUE;
	/* row_cycle init to 3 cycles */
	chip->row_cycle = ROWCYCLE_DEFVALUE;
	/* page_size init to 512 Bytes */
	chip->page_size = PAGESIZE_DEFVALUE;
	/* nand_type init to COMMON NAND */
	chip->nand_type = NANDTYPE_DEFVALUE;
}

/*
 * nand_boot - Boot from NAND
 *
 * Read SPL data from NAND to TCSM
 * SPL size 14KB
 */
int nand_boot(void)
{
	struct nand_info nand_params;
	struct nand_info *chip = &nand_params;
	u32 start_page = 0;
	int ret;

	/* Init NAND gpio without NAND I/O */
	__gpio_as_nand();

	do {
		/* nand_info default value */
		nand_info_default(chip);
		/* Init NAND buswidth 8 bit */
		nand_set_8bit();

		__nand_enable();
		/* Reset NAND Flash */
		nand_reset();

		ret = nand_setup(chip, start_page);

		if (ret >= 0) {
			ret = nand_spl_load(chip, start_page);
		}

		__nand_disable();
		start_page += BACKUP_OFFSET;
	} while (ret < 0 && start_page <= BACKUP_PAGE);

	if (ret < 0)
		return sd_boot(1);
	else
		return xfer_d2i(start_addr + SPL_OFFSET, 0);
}
