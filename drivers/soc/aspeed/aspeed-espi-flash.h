/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2024 Aspeed Technology Inc.
 */
#ifndef _ASPEED_ESPI_FLASH_H_
#define _ASPEED_ESPI_FLASH_H_

#include <linux/mtd/mtd.h>

/*
 * eSPI cycle type encoding
 *
 * Section 5.1 Cycle Types and Packet Format,
 * Intel eSPI Interface Base Specification, Rev 1.0, Jan. 2016.
 */
#define ESPI_FLASH_READ                 0x00
#define ESPI_FLASH_WRITE                0x01
#define ESPI_FLASH_ERASE                0x02
#define ESPI_FLASH_SUC_CMPLT            0x06
#define ESPI_FLASH_SUC_CMPLT_D_MIDDLE   0x09
#define ESPI_FLASH_SUC_CMPLT_D_FIRST    0x0b
#define ESPI_FLASH_SUC_CMPLT_D_LAST     0x0d
#define ESPI_FLASH_SUC_CMPLT_D_ONLY     0x0f
#define ESPI_FLASH_UNSUC_CMPLT          0x0c
#define ESPI_FLASH_UNSUC_CMPLT_ONLY     0x0e

/*
 * we choose the longest header and the max payload size
 * based on the Intel specification to define the maximum
 * eSPI packet length
 */
#define ESPI_PLD_LEN_MIN        (1UL << 6)
#define ESPI_PLD_LEN_MAX        (1UL << 12)


enum aspeed_espi_flash_safs_mode {
	SAFS_MODE_MIX,
	SAFS_MODE_SW,
	SAFS_MODE_HW,
	SAFS_MODES,
};

struct aspeed_espi_flash_dma {
	void *tx_virt;
	dma_addr_t tx_addr;
	void *rx_virt;
	dma_addr_t rx_addr;
};

struct aspeed_espi_flash {

	unsigned short		page_offset;	/* offset in flash address */
	unsigned int		page_size;	   /* of bytes per page */
	struct mutex			lock;
	struct aspeed_espi_flash_dma	dma;
	struct mtd_info			mtd;
	struct aspeed_espi_ctrl		*ctrl;
	uint8_t				erase_mask;
	uint32_t			tx_sts;
	uint32_t			rx_sts;
	wait_queue_head_t		wq;
	spinlock_t			spinlock;
};

void aspeed_espi_flash_event(uint32_t sts, struct aspeed_espi_flash *espi_flash);
void aspeed_espi_flash_enable(struct aspeed_espi_flash *espi_flash);
void *aspeed_espi_flash_alloc(struct device *dev, struct aspeed_espi_ctrl *espi_ctrl);
void aspeed_espi_flash_free(struct device *dev, struct aspeed_espi_flash *espi_flash);

#endif

