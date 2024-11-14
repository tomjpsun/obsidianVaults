/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022 Davicom Semiconductor,Inc.
 * Davicom DM9051 SPI Fast Ethernet Linux driver
 */
#define DM9051_1588_ST_GPIO		0x60
#define DM9051_1588_CLK_CTRL		0x61
#define DM9051_1588_GP_TXRX_CTRL	0x62
#define DM9051_1588_TX_CONF		0x63
#define DM9051_1588_RX_CONF1		0x64
#define DM9051_1588_RX_CONF2		0x65
#define DM9051_1588_RX_CONF3		0x66
#define DM9051_1588_CLK_P		0x67
#define DM9051_1588_TS  		0x68
#define DM9051_1588_AUTO		0x69
#define DM9051_1588_GPIO_CONF		0x6A
#define DM9051_1588_GPIO_TE_CONF	0x6B
#define DM9051_1588_GPIO_TA_L		0x6C
#define DM9051_1588_GPIO_TA_H		0x6D
#define DM9051_1588_GPIO_DTA_L		0x6E
#define DM9051_1588_GPIO_DTA_H		0x6F
//Stone add for debug message flags 2024-04-19
#define DE_TIMESTAMP 0  

void dm9051_ptp_rx_hwtstamp(struct board_info *db, struct sk_buff *skb, u8 *rxTSbyte);
void dm9051_ptp_tx_hwtstamp(struct board_info *db, struct sk_buff *skb);
void dm9051_ptp_init(struct board_info *db);
void dm9051_ptp_stop(struct board_info *db);
int dm9051_ptp_set_ts_config(struct net_device *netdev, struct ifreq *ifr);
int dm9051_ptp_get_ts_config(struct net_device *netdev, struct ifreq *ifr);
int dm9051_ts_info(struct net_device *net_dev, struct ethtool_ts_info *ts_info);



