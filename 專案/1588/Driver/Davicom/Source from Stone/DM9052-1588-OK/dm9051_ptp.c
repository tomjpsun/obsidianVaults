/*
 * drivers/net/ethernet/davicom/dm9051_ptp.c
 * Copyright 2021 Davicom Semiconductor,Inc.
 *
 * 	This program is free software; you can redistribute it and/or
 * 	modify it under the terms of the GNU General Public License
 * 	as published by the Free Software Foundation; either version 2
 * 	of the License, or (at your option) any later version.
 *
 * 	This program is distributed in the hope that it will be useful,
 * 	but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 	GNU General Public License for more details.
 *
 *	http://www.davicom.com.tw/
 *	Stone SHYR <stone_shyr@davicom.com.tw>
 */

#include <linux/crc32.h>
//#include <linux/kernel.h>
#include <linux/if_vlan.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/net_tstamp.h>
#include <linux/ptp_classify.h>
#include <linux/ptp_clock_kernel.h>
#include <linux/math64.h>
#include <linux/spi/spi.h>
//Stone add for 1588
#include "dm9051.c"
//#include "dm9051.h"
#include "dm9051_ptp.h"
//#include "board_info.h"

#define MAX_RXTS	64
/* phyter seems to miss the mark by 16 ns */
#define ADJTIME_FIX	16
//#define SKB_TIMESTAMP_TIMEOUT	2 /* jiffies */
//#define SKB_TIMESTAMP_TIMEOUT	1 /* jiffies */
#define SKB_TIMESTAMP_TIMEOUT	0 /* jiffies */

#define N_EXT_TS	6
#define N_PER_OUT	7
#define PSF_PTPVER	2
#define PSF_EVNT	0x4000
#define PSF_RX		0x2000
#define PSF_TX		0x1000
#define EXT_EVENT	1
#define CAL_EVENT	7
#define CAL_TRIGGER	1
/*
 * Bits of the ptp_extts_request.flags field:
 */
#define PTP_ENABLE_FEATURE (1<<0)
#define PTP_RISING_EDGE    (1<<1)
#define PTP_FALLING_EDGE   (1<<2)
#define PTP_STRICT_FLAGS   (1<<3)
#define PTP_EXTTS_EDGES    (PTP_RISING_EDGE | PTP_FALLING_EDGE)

/*
 * flag fields valid for the new PTP_EXTTS_REQUEST2 ioctl.
 */
#define PTP_EXTTS_VALID_FLAGS	(PTP_ENABLE_FEATURE |	\
				 PTP_RISING_EDGE |	\
				 PTP_FALLING_EDGE |	\
				 PTP_STRICT_FLAGS)

/*
 * flag fields valid for the original PTP_EXTTS_REQUEST ioctl.
 * DO NOT ADD NEW FLAGS HERE.
 */
#define PTP_EXTTS_V1_VALID_FLAGS	(PTP_ENABLE_FEATURE |	\
					 PTP_RISING_EDGE |	\
					 PTP_FALLING_EDGE)

/* a list of clocks and a mutex to protect it */
static LIST_HEAD(phyter_clocks);
static DEFINE_MUTEX(phyter_clocks_lock);

/* flags controlling PTP/1588 function */
#define IGB_PTP_ENABLED		BIT(0)
#define IGB_PTP_OVERFLOW_CHECK	BIT(1)

//Stone add for GP1 trigger setup !
int trigger_ff = 0x01;
int trigger_70 = 0x01;
int tx_1588 = 0x00;



#if 1


int dm9051_ts_info(struct net_device *net_dev, struct ethtool_ts_info *ts_info)
{
   struct board_info *db = netdev_priv(net_dev);
#if DE_TIMESTAMP   
printk("[in %s] XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx*dm9051_ts_info*\n", __FUNCTION__);
#endif
        ts_info->so_timestamping = (SOF_TIMESTAMPING_RX_SOFTWARE |
				    SOF_TIMESTAMPING_TX_SOFTWARE |
				    SOF_TIMESTAMPING_SOFTWARE);
	ts_info->phc_index = -1;

        ts_info->so_timestamping =
		SOF_TIMESTAMPING_TX_HARDWARE |
		SOF_TIMESTAMPING_RX_HARDWARE |
		SOF_TIMESTAMPING_RAW_HARDWARE|
		SOF_TIMESTAMPING_RX_SOFTWARE |
				    SOF_TIMESTAMPING_TX_SOFTWARE |
				    SOF_TIMESTAMPING_SOFTWARE;
	ts_info->phc_index = ptp_clock_index(db->ptp_clock);
	ts_info->tx_types =
		(1 << HWTSTAMP_TX_OFF)|
		//(1 << HWTSTAMP_TX_ON);
    //Stoe add for DM9051A 1588 support one-step sync insert time stamp! 2024-08-15		
#if TT9052
		(1 << HWTSTAMP_TX_ON);
#endif

#if DM9051A	
	  (1 << HWTSTAMP_TX_ON) ;	
	  //(1 << HWTSTAMP_TX_ONESTEP_SYNC);
#endif
	   
	ts_info->rx_filters =
#if TT9052	
		(1 << HWTSTAMP_FILTER_NONE) |
		(1 << HWTSTAMP_FILTER_PTP_V1_L4_EVENT) |
		(1 << HWTSTAMP_FILTER_PTP_V2_L4_EVENT) |
		(1 << HWTSTAMP_FILTER_PTP_V2_L2_EVENT) |
		(1 << HWTSTAMP_FILTER_PTP_V2_EVENT);
#endif

#if DM9051A	
        (1 << HWTSTAMP_FILTER_ALL) |
		(1 << HWTSTAMP_FILTER_SOME);
#endif
		
#if DE_TIMESTAMP		
	printk("dm9051_ts_info tx_types %x,  rx_filters %x \n", ts_info->tx_types, ts_info->rx_filters);
#endif	
	return 0;


}

//Stone add for 1588 read register 2024-04-12

#if 0 //i40
static int ptp_9051_adjfine(struct ptp_clock_info *ptp, long scaled_ppm)
{
 	struct board_info *db = container_of(ptp, struct board_info,
					       ptp_caps);
	//struct phy_device *phydev = clock->chosen->phydev;
	u64 rate, rate_test;
    u32 test;
	int neg_adj = 0;
    int temp = 0;
    u8 s_ppm[8];
	u16 hi, lo;
	u64 adj = 0, base_adj = 0;
	u32 HIGH, LOW;
	printk("+++00112+++++ [in %s] scaled_ppm = %x+++++++++\n", __FUNCTION__ ,scaled_ppm);
    //printk(" ==> ptp_9051_adjfreq in\r\n");
     //rate = -ppb;

	base_adj = 0;
	adj = adjust_by_scaled_ppm(base_adj, adj);
	HIGH = adj >> 32;
	LOW = adj & 0xFFFFFFFF;
	
	s_ppm[0] = LOW & 0xff;
	s_ppm[1] = (LOW & 0xffff) >> 8;
	s_ppm[2] = (LOW >> 16) & 0xff;
	s_ppm[3] = LOW >> 24;
	s_ppm[4] = HIGH & 0xff;
	s_ppm[5] = (HIGH & 0xffff) >> 8;
	s_ppm[6] = (HIGH >> 16) & 0xff;
	s_ppm[7] = HIGH >> 24;
	
	iow(db, DM9051_1588_CLK_CTRL, 0x80); //R61 W80
	for (int i=0; i<8; i++)
    {
		iow(db, DM9051_1588_TS, s_ppm[i] & 0xff);
	}
	iow(db, DM9051_1588_CLK_CTRL, 0x20); 
	
    return 0;
}
#endif

#if 1 //test ver.
static int ptp_9051_adjfine(struct ptp_clock_info *ptp, long scaled_ppm)
{
 	struct board_info *db = container_of(ptp, struct board_info,
					       ptp_caps);
	//struct phy_device *phydev = clock->chosen->phydev;
	u64 rate;  //, rate_test;
    //u32 test;
	int neg_adj = 0;
    //int temp = 0;
    u8 s_ppm[4];
	u16 hi, lo;
#if DE_TIMESTAMP	
	printk("+++00112+++++ [in %s] scaled_ppm = %d +++++++++\n", __FUNCTION__ ,scaled_ppm);
#endif	
	//printk("+++00112+++++ [in %s] scaled_ppm = %llx +++++++++\n", __FUNCTION__ ,scaled_ppm);
	//printk("+++00112+++++ [in %s] sizeof(long) = %x +++++++++\n", __FUNCTION__ ,sizeof(long));
    //printk(" ==> ptp_9051_adjfreq in\r\n");
     //rate = -ppb;

    if (scaled_ppm < 0) {
		neg_adj = 1;
		scaled_ppm =  -scaled_ppm;
#if DE_TIMESTAMP		
		printk(" ==> adifine neg_adj = 1\n");
#endif		
	}else {
        neg_adj = 0;
#if DE_TIMESTAMP	
        printk(" ==> adjdine neg_adj =0 \r\n");
#endif	
    }    
    rate = scaled_ppm;
    //rate <<= 13;
    //rate = div_u64(rate, 15625);
    
    hi = (rate >> 16);
    //hi = (rate >> 16) & 0x3FF;
    if(neg_adj)
    {
		//hi |= (1 << 15);
	}

	lo = rate & 0xffff;
    
    
    
	s_ppm[0] = lo & 0xff;
	s_ppm[1] = (lo >> 8) & 0xff;
	s_ppm[2] = hi & 0xFF;
	s_ppm[3] = (hi >> 8) & 0xff;

	//iow(db, DM9051_1588_CLK_CTRL, 0x2); 
	    
	//mutex_lock(&clock->extreg_lock);
    //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock
    iow(db, DM9051_1588_CLK_CTRL, 0x80); //R61 W80
    
    if (neg_adj)
    {
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, s_ppm[0]);  //Write register 0x68
     iow(db, DM9051_1588_TS, s_ppm[1]);    //Write register 0x68  
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, s_ppm[2]);  //Write register 0x68
     iow(db, DM9051_1588_TS, s_ppm[3]);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1922); //Increase of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x60);     //Write register 0x61 to 0x21
     }
     else
     {
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, s_ppm[0]);
     iow(db, DM9051_1588_TS, s_ppm[1]);    //Write register 0x68 
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, s_ppm[2]);
     iow(db, DM9051_1588_TS, s_ppm[3]);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1b22); //Decrease of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x20);      //Write register 0x61 to 0x61
     }
     
#if 0    
    if (neg_adj)
    {
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, lo & 0xff);  //Write register 0x68
     iow(db, DM9051_1588_TS, lo >> 8);    //Write register 0x68  
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, hi & 0xff);  //Write register 0x68
     iow(db, DM9051_1588_TS, hi >> 8);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1922); //Increase of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x60);     //Write register 0x61 to 0x21
     }
     else
     {
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, lo & 0xff);
     iow(db, DM9051_1588_TS, lo >> 8);    //Write register 0x68 
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, hi & 0xff);
     iow(db, DM9051_1588_TS, hi >> 8);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1b22); //Decrease of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x20);      //Write register 0x61 to 0x61
     }
#endif     

    //mutex_unlock(&clock->extreg_lock);
    //ADDR_LOCK_TAIL_ESSENTIAL(db); //mutex_unlock

    //printk(" <== ptp_9051_adjfreq out\r\n");
    return 0;
}
#endif


#if 0 //DAVICOM Ver.
static int ptp_9051_adjfine(struct ptp_clock_info *ptp, long scaled_ppm)
{
 	struct board_info *db = container_of(ptp, struct board_info,
					       ptp_caps);
	//struct phy_device *phydev = clock->chosen->phydev;
	u64 rate, rate_test;
    u32 test;
	int neg_adj = 0;
    int temp = 0;
    u8 s_ppm[4];
	u16 hi, lo;
	printk("+++00112+++++ [in %s] scaled_ppm = %x+++++++++\n", __FUNCTION__ ,scaled_ppm);
    //printk(" ==> ptp_9051_adjfreq in\r\n");
     //rate = -ppb;

    if (scaled_ppm < 0) {
		neg_adj = 1;
		scaled_ppm =  -scaled_ppm;
		printk(" ==> adifine neg_adj = 1\n");
	}else {
        neg_adj = 0;
        printk(" ==> adjdine neg_adj =0 \r\n");
    }    
    rate = scaled_ppm;
    //rate <<= 13;
    //rate = div_u64(rate, 15625);
    
    hi = (rate >> 16);
    //hi = (rate >> 16) & 0x3FF;
    if(neg_adj)
    {
		//hi |= (1 << 15);
	}

	lo = rate & 0xffff;
    
    
    
	s_ppm[0] = lo & 0xff;
	s_ppm[1] = (lo >> 8) & 0xff;
	s_ppm[2] = hi & 0xFF;
	s_ppm[3] = (hi >> 8) & 0xff;

	//iow(db, DM9051_1588_CLK_CTRL, 0x2); 
	    
	//mutex_lock(&clock->extreg_lock);
    //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock
    iow(db, DM9051_1588_CLK_CTRL, 0x80); //R61 W80
    
    if (neg_adj)
    {
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, s_ppm[0]);  //Write register 0x68
     iow(db, DM9051_1588_TS, s_ppm[1]);    //Write register 0x68  
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, s_ppm[2]);  //Write register 0x68
     iow(db, DM9051_1588_TS, s_ppm[3]);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1922); //Increase of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x60);     //Write register 0x61 to 0x21
     }
     else
     {
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, s_ppm[0]);
     iow(db, DM9051_1588_TS, s_ppm[1]);    //Write register 0x68 
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, s_ppm[2]);
     iow(db, DM9051_1588_TS, s_ppm[3]);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1b22); //Decrease of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x20);      //Write register 0x61 to 0x61
     }
     
#if 0    
    if (neg_adj)
    {
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, lo & 0xff);  //Write register 0x68
     iow(db, DM9051_1588_TS, lo >> 8);    //Write register 0x68  
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, hi & 0xff);  //Write register 0x68
     iow(db, DM9051_1588_TS, hi >> 8);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1922); //Increase of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x60);     //Write register 0x61 to 0x21
     }
     else
     {
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, lo & 0xff);
     iow(db, DM9051_1588_TS, lo >> 8);    //Write register 0x68 
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, hi & 0xff);
     iow(db, DM9051_1588_TS, hi >> 8);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1b22); //Decrease of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x20);      //Write register 0x61 to 0x61
     }
#endif     

    //mutex_unlock(&clock->extreg_lock);
    //ADDR_LOCK_TAIL_ESSENTIAL(db); //mutex_unlock

    //printk(" <== ptp_9051_adjfreq out\r\n");
    return 0;
}
#endif



#if 0
static int ptp_9051_adjfreq(struct ptp_clock_info *ptp, s32 ppb)
{
 	struct board_info *db = container_of(ptp, struct board_info,
					       ptp_caps);
	//struct phy_device *phydev = clock->chosen->phydev;
	u64 rate, rate_test;
    u32 test;
	int neg_adj = 0;
    int temp = 0;
	u16 hi, lo;
	printk("+++00111+++++ [in %s] ppb = %x+++++++++\n", __FUNCTION__ ,ppb);
    //printk(" ==> ptp_9051_adjfreq in\r\n");
    
    if(ppb < 0)
    {
		printk(" ==> ppb < 0\n");
	}
	else if(ppb > 0)
	{
		printk(" ==> ppb > 0\n");
	}
	else
	{
		printk(" ==> ppb = 0\n");
	}
	
     //rate = ppb;

    if (ppb < 0) {
		ppb = -ppb;
		neg_adj = 1;
		printk(" ==> neg_adj = 1;r\n");
	}else {
        neg_adj = 0;
        printk(" ==> neg_adj =0 \r\n");
    }    
    rate = ppb;
    
    hi = (rate >> 16);

	lo = rate & 0xffff;
    
	//mutex_lock(&clock->extreg_lock);
    //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock
    iow(db, DM9051_1588_CLK_CTRL, 0x80);  
    if (neg_adj)
    {
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, lo & 0xff);  //Write register 0x68
     iow(db, DM9051_1588_TS, lo >> 8);    //Write register 0x68  
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, hi & 0xff);  //Write register 0x68
     iow(db, DM9051_1588_TS, hi >> 8);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1922); //Increase of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x61);     //Write register 0x61 to 0x21
     }else{
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, lo & 0xff);
     iow(db, DM9051_1588_TS, lo >> 8);    //Write register 0x68 
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, hi & 0xff);
     iow(db, DM9051_1588_TS, hi >> 8);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1b22); //Decrease of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x21);      //Write register 0x61 to 0x61
     }

    //mutex_unlock(&clock->extreg_lock);
    //ADDR_LOCK_TAIL_ESSENTIAL(db); //mutex_unlock

    //printk(" <== ptp_9051_adjfreq out\r\n");
    return 0;
}
#endif

#if 0 // adjfreq backup
static int ptp_9051_adjfreq(struct ptp_clock_info *ptp, s32 ppb)
{
 	struct board_info *db = container_of(ptp, struct board_info,
					       ptp_caps);
	//struct phy_device *phydev = clock->chosen->phydev;
	u64 rate;  //, rate_test;
    //u32 test;
	int neg_adj = 0;
    int temp = 0;
	u16 hi, lo;
	printk("+++00111+++++ [in %s] ppb = %x+++++++++\n", __FUNCTION__ ,ppb);
    //printk(" ==> ptp_9051_adjfreq in\r\n");
    
    if(ppb < 0)
    {
		printk(" ==> ppb < 0\n");
	}
	else if(ppb > 0)
	{
		printk(" ==> ppb > 0\n");
	}
	else
	{
		printk(" ==> ppb = 0\n");
	}
	
     rate = -ppb;

    if (-ppb < 0) {
		neg_adj = 1;
		printk(" ==> neg_adj = 1;r\n");
	}else {
        neg_adj = 0;
        printk(" ==> neg_adj =0 \r\n");
    }    
    
    hi = (rate >> 16);

	lo = rate & 0xffff;
    
	//mutex_lock(&clock->extreg_lock);
    //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock
    iow(db, DM9051_1588_CLK_CTRL, 0x80); 
    
    if (neg_adj)
    {
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, lo & 0xff);  //Write register 0x68
     iow(db, DM9051_1588_TS, lo >> 8);    //Write register 0x68  
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, hi & 0xff);  //Write register 0x68
     iow(db, DM9051_1588_TS, hi >> 8);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1922); //Increase of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x21);     //Write register 0x61 to 0x21
     }else{
     //phy_write(phydev, 0x2d2, lo);
     iow(db, DM9051_1588_TS, lo & 0xff);
     iow(db, DM9051_1588_TS, lo >> 8);    //Write register 0x68 
     //phy_write(phydev, 0x2d3, hi);
     iow(db, DM9051_1588_TS, hi & 0xff);
     iow(db, DM9051_1588_TS, hi >> 8);    //Write register 0x68
     //phy_write(phydev, 0x2d1, 0x1b22); //Decrease of fractional nanosecond
     iow(db, DM9051_1588_CLK_CTRL, 0x61);      //Write register 0x61 to 0x61
     }

    //mutex_unlock(&clock->extreg_lock);
    //ADDR_LOCK_TAIL_ESSENTIAL(db); //mutex_unlock

    //printk(" <== ptp_9051_adjfreq out\r\n");
    return 0;
}
#endif

static int ptp_9051_adjtime(struct ptp_clock_info *ptp, s64 delta)
{
 struct board_info *db = container_of(ptp, struct board_info,
					       ptp_caps);
 struct timespec64 ts;
    int sign = 1;
	//int err;
	u8 temp[8];
#if DE_TIMESTAMP	
    printk("+++00111+++++ [in %s] delta = %llx+++++++++\n", __FUNCTION__ ,delta);
#endif    
    //printk("@@@1 ptp_dm8806_adjtime delta %llx \n", delta);
#if 1
	delta += ADJTIME_FIX;
    //printk("@@@2 ptp_dm8806_adjtime delta %llx \n", delta);
        
    if (delta < 0) {
		sign = 0;
		delta = -delta;
#if DE_TIMESTAMP		
		printk("delta less han zero.. \n");
#endif		
	}
    
    //mutex_lock(&clock->extreg_lock);
    //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock
        
    //printk("@@@2-1 ptp_dm8806_adjtime delta %llx \n", delta);
    ts.tv_sec = div_s64(delta, 0x3b9aca00);
#if DE_TIMESTAMP
    printk("@@@2-2 ptp_dm8806_adjtime delta %llx sec %lx \n", delta, ts.tv_sec);
#endif    
    ts.tv_nsec = (delta - (ts.tv_sec * 0x3b9aca00))& 0xffffffff;
    //ts.tv_nsec = div_s64_rem(delta, 0x3b9aca00, &rem);
	//ts.tv_nsec = (delta - (ts.tv_sec * 1000000000));
    //ts.tv_sec = rem & 0xffffffff;
#if DE_TIMESTAMP    
     printk("@@@3 ptp_dm8806_adjtime delta %llx  nsec=%lx  \n", delta, ts.tv_nsec);
#endif      
    if (sign == 0)
        { 
         if (ts.tv_sec == 0)
           ts.tv_sec++;
         ts.tv_sec  = (0x100000000-ts.tv_sec);
         ts.tv_nsec = (1000000000 - ts.tv_nsec);
#if DE_TIMESTAMP    	 
         printk("delta equal to  zero.. \n");
#endif	 
         }
         
    //printk("@@@4 ptp_dm8806_adjtime delta %llx sec=%lx  \n", delta, ts.tv_sec );
#endif    
    
	//ts_1 = ns_to_timespec64(delta);
    
    //printk("@@@5 ptp_dm8806_adjtime delta %llx nsec=%lx  \n", delta, ts.tv_nsec);

	//err = tdr_write(1, phydev, &ts, PTP_STEP_CLK);

#if 1
	temp[0] = ts.tv_nsec & 0xff;
	temp[1] = (ts.tv_nsec & 0xffff) >> 8;
	temp[2] = (ts.tv_nsec >> 16) & 0xff;
	temp[3] = ts.tv_nsec >> 24;
	temp[4] = ts.tv_sec & 0xff;
	temp[5] = (ts.tv_sec & 0xffff) >> 8;
	temp[6] = (ts.tv_sec >> 16) & 0xff;
	temp[7] = ts.tv_sec >> 24;
	
	//iow(db, DM9051_1588_CLK_CTRL, 0x2); 

    iow(db, DM9051_1588_CLK_CTRL, 0x80);  
    for (int i=0; i<8; i++)
    {
		iow(db, DM9051_1588_TS, temp[i] & 0xff);
	}
	iow(db, DM9051_1588_CLK_CTRL, 0x10);  
#if DE_TIMESTAMP	
	printk(" ptp_9051_adjtime hwtstamp = %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n", 
			temp[0], temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7]);   
#endif
			
#if 0	
 //phy_write(phydev, 0x2d2, ts.tv_nsec & 0xffff);   /* ns[15:0]  */
 iow(db, DM9051_1588_TS, ts.tv_nsec & 0xff);             //Write register 0x68
 iow(db, DM9051_1588_TS, (ts.tv_nsec & 0xffff) >> 8);    //Write register 0x68
 //phy_write(phydev, 0x2d3, ts.tv_nsec >> 16);      /* ns[31:16] */
 iow(db, DM9051_1588_TS, (ts.tv_nsec >> 16) & 0xff);     //Write register 0x68
 iow(db, DM9051_1588_TS, ts.tv_nsec >> 24);             //Write register 0x68
 //=============================================================================
 //phy_write(phydev, 0x2d4, ts.tv_sec & 0xffff);    /* sec[15:0] */
 iow(db, DM9051_1588_TS, ts.tv_sec & 0xff);              //Write register 0x68
 iow(db, DM9051_1588_TS, (ts.tv_sec & 0xffff) >> 8);     //Write register 0x68
 //phy_write(phydev, 0x2d5, ts.tv_sec >> 16);       /* sec[31:16]*/
 iow(db, DM9051_1588_TS, (ts.tv_sec >> 16) & 0xff);      //Write register 0x68
 iow(db, DM9051_1588_TS, ts.tv_sec >> 24);               //Write register 0x68
 
 //phy_write(phydev, 0x2d1, 0x1832); //Enable PTP, Write Timestamp
 iow(db, DM9051_1588_CLK_CTRL, 0x9);          //Write register 0x61 to 0x11
#endif 
#endif

#if DE_TIMESTAMP
#if 1    //Stone add for 1588 2022-10-19
 printk("### sign %x delta %llx ts.tv_sec =  %lx, ts.tv_nsec = %llxx  ###\n", sign, delta, ts.tv_sec, ts.tv_nsec);
#endif 
#endif
	//mutex_unlock(&clock->extreg_lock);
    //ADDR_LOCK_TAIL_ESSENTIAL(db); //mutex_unlock

    return 0;

}







#if 0
static int ptp_9051_gettime(struct ptp_clock_info *ptp,
			       struct timespec64 *ts)
{
  struct board_info *db = container_of(ptp, struct board_info,
					       ptp_caps);
  unsigned int val[4];                         

    //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock 
    //phy_write(phydev, 0x2d1, 0x1812); //Enable PTP, read PTP clock
    iow(db, DM9051_1588_CLK_CTRL, 0x05);           //Write register 0x61 to 0x05
    //val[0] = phy_read(phydev, 0x2d2);
    val[0] = ior(db, DM9051_1588_TS);        //Read register 0x68
    //val[1] = phy_read(phydev, 0x2d3);
    val[1] = ior(db, DM9051_1588_TS);        //Read register 0x68

    //val[2] = phy_read(phydev, 0x2d4);
    val[2] = ior(db, DM9051_1588_TS);        //Read register 0x68
    //val[3] = phy_read(phydev, 0x2d5);
    val[3] = ior(db, DM9051_1588_TS);        //Read register 0x68
    
    //ADDR_LOCK_TAIL_ESSENTIAL(db); //mutex_unlock
    
    ts->tv_nsec = val[0] | (val[1] << 16);
	ts->tv_sec  = val[2] | (val[3] << 16);
    printk("!!! ptp_dm9051_gettime sec=%x nsec=%x \n", ts->tv_sec, ts->tv_nsec);
	return 0;
}
#endif

#if 1
static int ptp_9051_gettime(struct ptp_clock_info *ptp,
			       struct timespec64 *ts)
{
  struct board_info *db = container_of(ptp, struct board_info,
					       ptp_caps);
  //unsigned int val[4];  
  unsigned int temp[8];
  
  iow(db, DM9051_1588_CLK_CTRL, 0x80);
  iow(db, DM9051_1588_CLK_CTRL, 0x05);
  iow(db, DM9051_1588_CLK_CTRL, 0x80);
  
   for (int i=0; i<8; i++)
    temp[i] = ior(db, DM9051_1588_TS);  
                  

    //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock 
    //phy_write(phydev, 0x2d1, 0x1812); //Enable PTP, read PTP clock
//    iow(db, DM9051_1588_CLK_CTRL, 0x05);           //Write register 0x61 to 0x05
    //val[0] = phy_read(phydev, 0x2d2);
//    val[0] = ior(db, DM9051_1588_TS);        //Read register 0x68
    //val[1] = phy_read(phydev, 0x2d3);
//    val[1] = ior(db, DM9051_1588_TS);        //Read register 0x68

    //val[2] = phy_read(phydev, 0x2d4);
//    val[2] = ior(db, DM9051_1588_TS);        //Read register 0x68
    //val[3] = phy_read(phydev, 0x2d5);
//    val[3] = ior(db, DM9051_1588_TS);        //Read register 0x68
    
    //ADDR_LOCK_TAIL_ESSENTIAL(db); //mutex_unlock
    
    ts->tv_nsec = (((temp[2] | (temp[3] << 8)) << 16) | (temp[0] | (temp[1] << 8)));//val[0] | (val[1] << 16);
	ts->tv_sec  = (((temp[6] | (temp[7] << 8)) << 16) | (temp[4] | (temp[5] << 8)));//val[2] | (val[3] << 16);
#if DE_TIMESTAMP	
//#if 1
    printk("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ptp_dm9051_gettime sec=%x nsec=%x \r\n", ts->tv_sec, ts->tv_nsec);
#endif	
    return 0;
}
#endif

static int ptp_9051_settime(struct ptp_clock_info *ptp,
			      const struct timespec64 *ts)
{
  struct board_info *db = container_of(ptp, struct board_info,
					       ptp_caps);
  
  //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock
  printk(" ==********************************************************************************************************************************> ptp_9051_settime in \r\n");
  iow(db, DM9051_1588_CLK_CTRL, 0x80);
  //phy_write(dev, 0x2d2, ts->tv_nsec & 0xffff);   /* ns[15:0]  */
  iow(db, DM9051_1588_TS, ts->tv_nsec & 0xff);             //Write register 0x68
  iow(db, DM9051_1588_TS, (ts->tv_nsec & 0xffff) >> 8);    //Write register 0x68
  // phy_write(dev, 0x2d3, ts->tv_nsec >> 16);      /* ns[31:16] */
  iow(db, DM9051_1588_TS, (ts->tv_nsec >> 16) & 0xff);     //Write register 0x68
  iow(db, DM9051_1588_TS, ts->tv_nsec >> 24);             //Write register 0x68
  //==============================================================
  //phy_write(dev, 0x2d4, ts->tv_sec & 0xffff);    /* sec[15:0] */
  iow(db, DM9051_1588_TS, ts->tv_sec & 0xff);              //Write register 0x68
  iow(db, DM9051_1588_TS, (ts->tv_sec & 0xffff) >> 8);     //Write register 0x68 
  //phy_write(dev, 0x2d5, ts->tv_sec >> 16);       /* sec[31:16]*/
  iow(db, DM9051_1588_TS, (ts->tv_sec >> 16) & 0xff);      //Write register 0x68
  iow(db, DM9051_1588_TS, ts->tv_sec >> 24);               //Write register 0x68
  //phy_write(dev, 0x2d1, 0x1832); //Enable PTP, Write Timestamp
  
  iow(db, DM9051_1588_CLK_CTRL, 0x09);    //Write register 0x61 to 0x09  //Write PTP Clock Register offset 0x61, value 0x01 + 0x08                

  //ADDR_LOCK_TAIL_ESSENTIAL(db); //mutex_unlock
  //printk("#########################ptp_9051_settime### ts->tv_sec =  %x, ts->tv_nsec = %x  ###\n", ts->tv_sec, ts->tv_nsec);
  printk(" <== ptp_9051_settime out \r\n");
	return 0;
}


//Stone add for 1588 GP1 trigger edge output enale !
#define DM9051_PTP_SEC_DELAY 60
#define DEBUG_PPS            1 
void dm9051_pps_configure(struct ptp_clock_info *ptp)
{
 struct timespec64 now; //, tmp;
 //u64 ns;
 unsigned int temp[9];   
 
 struct board_info *db = container_of(ptp, struct board_info,
  					       ptp_caps);
 
 printk("dm9051_pps_configure \r\n");
 //Read dm9051 ptp clock now!
 //ptp_9051_gettime(ptp, &tmp);
 ptp_9051_gettime(ptp, &now);
 //Stone add for sec & nsec bug!!!!
 //printk("dm9051_pps_configure gettime tmp.tv_sec = 0x%llx, tmp.tv_nsec = 0x%lx \r\n", tmp.tv_sec, tmp.tv_nsec);
 //now.tv_nsec = tmp.tv_sec;
 //now.tv_sec = tmp.tv_nsec;
 printk("111 dm9051_pps_configure trigger time now.tv_sec = 0x%llx, now.tv_nsec = 0x%lx 111\r\n", now.tv_sec, now.tv_nsec);
 //printk("111 temp[4-7] %x %x %x %x \r\n", now.tv_sec & 0xff, (now.tv_sec>>8) & 0xff, (now.tv_sec>>16) & 0xff, (now.tv_sec>>24) & 0xff);
 //now.tv_sec = now.tv_sec + DM9051_PTP_SEC_DELAY;   //Stone add for 2024-05-14 GP1 trigger time setup to 0xabcdff sec, 0x00 nsec
 now.tv_nsec = 0;
 
 
 //now.tv_nsec
 temp[0]= 0x00;
 temp[1]= 0x00;
 temp[2]= 0x00;
 temp[3]= 0x00;
 
 //now.tv_sec
 //temp[4]= now.tv_sec & 0xff; //Stone add for 2024-05-14 GP1 trigger time setup to 0xabcdff sec, 0x00 nsec
 temp[4]= 0xff;
 temp[5]= (now.tv_sec>>8) & 0xff;
 temp[6]= (now.tv_sec>>16) & 0xff;
 temp[7]= (now.tv_sec>>24) & 0xff;
 
 
 //ns = timespec64_to_ns(&now);
 
#if 0 //===================================
 //Setup GP1 to edge trigger output!
 //Register 0x60 to 0x0 (GP page (bit 1), PTP Function(bit 0))
 iow(db, DM9051_1588_ST_GPIO, 0x00);

#if DEBUG_PPS 
 temp[8] = ior(db, DM9051_1588_ST_GPIO);
 printk("Register 0x60 (0x00) = 0x%x \r\n", temp[8]);
#endif
 
 //Register 0x6A to 0x06 (interrupt disable(bit 2), trigger or event enable(bit 1), trigger output(bit 0))
 iow(db, DM9051_1588_GPIO_CONF, 0x06);

#if DEBUG_PPS  
 temp[8] = ior(db, DM9051_1588_GPIO_CONF);
 printk("Register 0x6A (0x06) = 0x%x \r\n", temp[8]);
#endif

 //Register 0x6B to 0x02(trigger out type: edge output(bit 3:2),  triger output active high(bit 1)) 
 iow(db, DM9051_1588_GPIO_TE_CONF, 0x02);

#if DEBUG_PPS  
 temp[8] = ior(db, DM9051_1588_GPIO_TE_CONF);
 printk("Register 0x6B (0x02) = 0x%x \r\n", temp[8]);
#endif
#endif //===================================
 
 //Register 0x61 to 0x80
 iow(db, DM9051_1588_CLK_CTRL, 0x80);

#if DEBUG_PPS  
 temp[8] = ior(db, DM9051_1588_CLK_CTRL);
 printk("Register 0x61 (0x80) = 0x%x \r\n", temp[8]);
#endif
 
 mutex_lock(&db->spi_lockm); 
 //Register 0x68 to now.tv_nsec & now.tv_sec !
 for (int i=0; i<8 ;i++)
    iow(db, DM9051_1588_TS, temp[i]);
 mutex_unlock(&db->spi_lockm); 
    
 //Register 0x62 to 0x10 (GP 1 trigger load or event Read)
 iow(db, DM9051_1588_GP_TXRX_CTRL, 0x10);   

#if DEBUG_PPS  
 temp[8] = ior(db, DM9051_1588_GP_TXRX_CTRL);
 printk("Register 0x62 (0x10) = 0x%x \r\n", temp[8]);
#endif 

#if DEBUG_PPS 
 temp[8] = ior(db, DM9051_1588_ST_GPIO);
 printk("Register 0x60 (0x00) = 0x%x \r\n", temp[8]);
#endif

#if DEBUG_PPS  
 temp[8] = ior(db, DM9051_1588_GPIO_CONF);
 printk("Register 0x6A (0x06) = 0x%x \r\n", temp[8]);
#endif

#if DEBUG_PPS  
 temp[8] = ior(db, DM9051_1588_GPIO_TE_CONF);
 printk("Register 0x6B (0x02) = 0x%x \r\n", temp[8]);
 
 printk("temp[0-7] = %x %x %x %x %x %x %x %x \r\n", temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
#endif

    
 printk("@@@ dm9051_pps_configure trigger time now.tv_sec = 0x%llx, now.tv_nsec = 0x%lx @@@\r\n", now.tv_sec, now.tv_nsec);    
}


void dm9051_perout(struct ptp_clock_info *ptp, struct ptp_clock_request *rq)
{
 //struct timespec64 now;
 int i;
 unsigned int temp_1[8],temp;   
 
 struct board_info *db = container_of(ptp, struct board_info,
  					       ptp_caps);
 
 temp = ior(db, DM9051_1588_ST_GPIO);
 printk("Read Register 0x60 = 0x%x \r\n", temp);
 
 if ((temp & 0x80) == 0x80) //GP2 trigger/event status ready
 {
      if ((temp & 0x40) == 0x40) //GP2 event is rising edge!
      {
	//Stone add for 1588 read GP2 "event timestamp" ! 
	//Register 0x62 to 0x20 (GP1 trigger load or event read (bit 5), GP page (bit 1), PTP Function(bit 0))
	printk("GP 2 have event intput!!!, read event timestamp!\r\n"); 
        //iow(db, DM9051_1588_GP_TXRX_CTRL, 0x20);

        temp = ior(db, DM9051_1588_GP_TXRX_CTRL);
        printk("Read Register 0x62 (0x0) = 0x%x \r\n", temp);
	
	//ptp_9051_gettime(ptp, &now);
	iow(db, DM9051_1588_CLK_CTRL, 0x80);
	for (i=0; i<8; i++)
	 temp_1[i] = ior(db, DM9051_1588_TS);
	
	rq->perout.start.nsec = (((temp_1[2] | (temp_1[3] << 8)) << 16) | (temp_1[0] | (temp_1[1] << 8)));//val[0] | (val[1] << 16);
	rq->perout.start.sec  = (((temp_1[6] | (temp_1[7] << 8)) << 16) | (temp_1[4] | (temp_1[5] << 8)));//val[2] | (val[3] << 16); 
	
	printk("dm9051_perout => rq->perout.start.sec = 0x%llx, rq->perout.start.nsec = 0x%x\r\n", rq->perout.start.sec, rq->perout.start.nsec);
	}else printk("GP2 event is NOT rising edge!!!!!\r\n");
	
 }else printk("GP2 trigger/event status NOT ready!!!!\r\n");
					       
 					       
					       
 
    
}    

//Stone add for enable GP2 event input mode!!!
void dm9051_extts(struct ptp_clock_info *ptp)
{
 struct board_info *db = container_of(ptp, struct board_info,
  					       ptp_caps);   
 unsigned int temp, temp_1[8];
 struct ptp_clock_time g2_event;
 int i; 
 temp = ior(db, DM9051_1588_ST_GPIO);
 printk("Read Register 0x60 = 0x%x \r\n", temp);
 
 if ((temp & 0x80) == 0x80) //GP2 trigger/event status ready
 {
      if ((temp & 0x40) == 0x40) //GP2 event is rising edge!
      {
	//Stone add for 1588 read GP2 "event timestamp" ! 
	//Register 0x62 to 0x20 (GP1 trigger load or event read (bit 5), GP page (bit 1), PTP Function(bit 0))
	printk("GP 2 have event intput!!!, read event timestamp!\r\n"); 
        iow(db, DM9051_1588_GP_TXRX_CTRL, 0x20);

        temp = ior(db, DM9051_1588_GP_TXRX_CTRL);
        printk("Read Register 0x62 (0x0) = 0x%x \r\n", temp);
	
	//ptp_9051_gettime(ptp, &now);
	iow(db, DM9051_1588_CLK_CTRL, 0x80);
	for (i=0; i<8; i++)
	 temp_1[i] = ior(db, DM9051_1588_TS);
	
	g2_event.nsec = (((temp_1[2] | (temp_1[3] << 8)) << 16) | (temp_1[0] | (temp_1[1] << 8)));//val[0] | (val[1] << 16);
	g2_event.sec  = (((temp_1[6] | (temp_1[7] << 8)) << 16) | (temp_1[4] | (temp_1[5] << 8)));//val[2] | (val[3] << 16); 
	
	printk("dm9051_perout => g2_event.sec = 0x%llx, g2_event.nsec = 0x%x\r\n", g2_event.sec, g2_event.nsec);  
      }
  }else{     
  
 //Setup GP2 to event input!
 //Register 0x60 to 0x2 (GP page (bit 1), PTP Function(bit 0))
 iow(db, DM9051_1588_ST_GPIO, 0x02);

#if DEBUG_PPS 
 temp = ior(db, DM9051_1588_ST_GPIO);
 printk("Register 0x60 (0x02) = 0x%x \r\n", temp);
#endif
 
 //Register 0x6A to 0x07 (interrupt disable(bit 2), trigger or event enable(bit 1), trigger output(bit 0))
 iow(db, DM9051_1588_GPIO_CONF, 0x07);

#if DEBUG_PPS  
 temp = ior(db, DM9051_1588_GPIO_CONF);
 printk("Register 0x6A (0x07) = 0x%x \r\n", temp);
#endif

 //Register 0x6B to 0x60(trigger out type: edge output(bit 3:2),  triger output active high(bit 1)) 
 iow(db, DM9051_1588_GPIO_TE_CONF, 0x60);

#if DEBUG_PPS  
 temp = ior(db, DM9051_1588_GPIO_TE_CONF);
 printk("Register 0x6B (0x60) = 0x%x \r\n", temp);
#endif
 
 //Register 0x61 to 0x80
 //iow(db, DM9051_1588_CLK_CTRL, 0x80);

#if DEBUG_PPS  
 temp = ior(db, DM9051_1588_ST_GPIO);
 printk("Check Register 0x60  = 0x%x \r\n", temp);
#endif
 } 
    
}    

static int ptp_9051_feature_enable(struct ptp_clock_info *ptp,
			      struct ptp_clock_request *rq, int on)
{
  //struct board_info *db = container_of(ptp, struct board_info,
  //					       ptp_caps);
   printk("!!! 1. ptp_9051_feature_enable in rq->type = 0x%x,  on = 0x%x\n", rq->type, on);
   
    switch (rq->type) {
	case PTP_CLK_REQ_EXTTS:
	        printk("2. ptp_9051_feature_enable PTP_CLK_REQ_EXTTS \n");
		printk("   rq->extts.index = 0x%x, rq->extts.flags = 0x%x, rq->extts.rsv[0] = 0x%x, rq->extts.rsv[1] = 0x%x \r\n", rq->extts.index, rq->extts.flags, rq->extts.rsv[0], rq->extts.rsv[1]);
		//func = PTP_PF_EXTTS;
		//chan = rq->extts.index;
		dm9051_extts(ptp); //Stone add for enable GP2 event input!
		break;
	case PTP_CLK_REQ_PEROUT:
	        printk("3. ptp_9051_feature_enable PTP_CLK_REQ_PEROUT \n");	
		printk("   rq->perout.index = 0x%x, rq->perout.start.sec = 0x%llx, rq->perout.start.nsec = 0x%x, rq->perout.period.sec = 0x%llx, rq->perout.period.nsec = 0x%x\r\n", rq->perout.index, rq->perout.start.sec, rq->perout.start.nsec, rq->perout.period.sec, rq->perout.period.nsec);
		//func = PTP_PF_PEROUT;
		//chan = rq->perout.index;
		dm9051_perout(ptp, rq); //Stone add for read GP2 status, and read PTP clock data to ptp4l!
		break;
	case PTP_CLK_REQ_PPS:
	        printk("4. ptp_9051_feature_enable PTP_CLK_REQ_PPS \n");		
		//return i40e_pps_configure(ptp, rq, on);
		//Stone add for 1588 pps enable "echo 1 > /sys/class/ptp/ptp0/pps_enable"
		if (on){
		   dm9051_pps_configure(ptp); 
		}
		break;
	default:
		return 0;
	}
  
  //Stone add for 1588 pps 
  return 0;

}

void dm9051_GP1_setup(struct board_info *db)
{
 unsigned int TX_tmp[9]; 
 struct timespec64 now; 	
 int temp, i;	
 
 temp = ior(db, DM9051_1588_ST_GPIO);
 printk("Read Register 0x60 = 0x%x \r\n", temp);
 
 if ((temp & 0x20) == 0x20) //GP1 trigger/event status ready
 {
   //if ((temp & 0x10) == 0x10) //GP1 event is rising edge!
     {
       iow(db, DM9051_1588_ST_GPIO, 0x20); //Clear GP1 stauts!
        
       ptp_9051_gettime(&db->ptp_caps, &now);
 
       TX_tmp[4]= now.tv_sec & 0xff;
       //TX_tmp[5]= (now.tv_sec>>8) & 0xff;
       //TX_tmp[6]= (now.tv_sec>>16) & 0xff;
       //TX_tmp[7]= (now.tv_sec>>24) & 0xff;
 
//Stone add for GP1 trigger output!
	   //==============================================
	   if ((TX_tmp[4] & 0xf) == 0xf)
        {
	     //if (trigger_70){ 
	        TX_tmp[0] = 0x00;
	        TX_tmp[1] = 0x00;
	        TX_tmp[2] = 0x00;
	        TX_tmp[3] = 0x00; 
	        
	        now.tv_sec = now.tv_sec +0x10;  
	        
	        TX_tmp[4]= now.tv_sec & 0xff;
	        TX_tmp[5]= (now.tv_sec>>8) & 0xff;
            TX_tmp[6]= (now.tv_sec>>16) & 0xff;
            TX_tmp[7]= (now.tv_sec>>24) & 0xff;
	     
	        //Setup GP1 trigger time!  
            //Register 0x61 to 0x80
            iow(db, DM9051_1588_CLK_CTRL, 0x80);

//#if DEBUG_PPS  
#if 1 
            temp = ior(db, DM9051_1588_CLK_CTRL);
            printk("Register 0x61 (0x80) = 0x%x \r\n", temp);
#endif
 
            mutex_lock(&db->spi_lockm);  
            //Register 0x68 to now.tv_nsec & now.tv_sec !
            for (int i=0; i<8 ;i++)
             iow(db, DM9051_1588_TS, TX_tmp[i]);
            mutex_unlock(&db->spi_lockm);  
    
            //Register 0x62 to 0x10 (GP 1 trigger load or event Read)
            iow(db, DM9051_1588_GP_TXRX_CTRL, 0x10);   
   
	        //trigger_70 = 0x00;
	        //trigger_ff = 0x01;
	        printk(" Trigger time = %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n", 
			 TX_tmp[0], TX_tmp[1],TX_tmp[2],TX_tmp[3],TX_tmp[4],TX_tmp[5],TX_tmp[6],TX_tmp[7]);    
           //}
          }else { //TX_tmp[4] & 0xf != 0xf
			//if (trigger_70){ 
	        TX_tmp[0] = 0x00;
	        TX_tmp[1] = 0x00;
	        TX_tmp[2] = 0x00;
	        TX_tmp[3] = 0x00; 
	        
	        //now.tv_sec = now.tv_sec +0x10;  
	        
	        TX_tmp[4]= (now.tv_sec & 0xff) | 0x0f;
	        TX_tmp[5]= (now.tv_sec>>8) & 0xff;
            TX_tmp[6]= (now.tv_sec>>16) & 0xff;
            TX_tmp[7]= (now.tv_sec>>24) & 0xff;
	     
	        //Setup GP1 trigger time!  
            //Register 0x61 to 0x80
            iow(db, DM9051_1588_CLK_CTRL, 0x80);

//#if DEBUG_PPS  
#if 1 
            temp = ior(db, DM9051_1588_CLK_CTRL);
            printk("Register 0x61 (0x80) = 0x%x \r\n", temp);
#endif
 
            mutex_lock(&db->spi_lockm);  
            //Register 0x68 to now.tv_nsec & now.tv_sec !
            for (int i=0; i<8 ;i++)
             iow(db, DM9051_1588_TS, TX_tmp[i]);
            mutex_unlock(&db->spi_lockm);  
    
            //Register 0x62 to 0x10 (GP 1 trigger load or event Read)
            iow(db, DM9051_1588_GP_TXRX_CTRL, 0x10);   
   
	        //trigger_70 = 0x00;
	        //trigger_ff = 0x01;
	        printk(" Trigger time = %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n", 
			 TX_tmp[0], TX_tmp[1],TX_tmp[2],TX_tmp[3],TX_tmp[4],TX_tmp[5],TX_tmp[6],TX_tmp[7]);    
           //}  
		  } //end of "TX_tmp[4] & 0xf != 0xf"
	    }
	  }else{
		  
	   if (trigger_70){
		   ptp_9051_gettime(&db->ptp_caps, &now);
 
           TX_tmp[4]= now.tv_sec & 0xff;
       
//Stone add for GP1 trigger output!
	       //==============================================
	       if ((TX_tmp[4] & 0xf) == 0xf)
           {	   
	        TX_tmp[0] = 0x00;
	        TX_tmp[1] = 0x00;
	        TX_tmp[2] = 0x00;
	        TX_tmp[3] = 0x00;
	         
	        now.tv_sec = now.tv_sec +0x10;  
	        
	        TX_tmp[4]= now.tv_sec & 0xff;
	        TX_tmp[5]= (now.tv_sec>>8) & 0xff;
            TX_tmp[6]= (now.tv_sec>>16) & 0xff;
            TX_tmp[7]= (now.tv_sec>>24) & 0xff;
	     
	        //Setup GP1 trigger time!  
            //Register 0x61 to 0x80
            iow(db, DM9051_1588_CLK_CTRL, 0x80);

//#if DEBUG_PPS  
#if 1 
            temp = ior(db, DM9051_1588_CLK_CTRL);
            printk("Register 0x61 (0x80) = 0x%x \r\n", temp);
#endif
 
            mutex_lock(&db->spi_lockm);  
            //Register 0x68 to now.tv_nsec & now.tv_sec !
            for (int i=0; i<8 ;i++)
             iow(db, DM9051_1588_TS, TX_tmp[i]);
            mutex_unlock(&db->spi_lockm);  
    
            //Register 0x62 to 0x10 (GP 1 trigger load or event Read)
            iow(db, DM9051_1588_GP_TXRX_CTRL, 0x10);   
   
	        trigger_70 = 0x00;
	        //trigger_ff = 0x01;
	        printk(" Trigger time = %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n", 
			 TX_tmp[0], TX_tmp[1],TX_tmp[2],TX_tmp[3],TX_tmp[4],TX_tmp[5],TX_tmp[6],TX_tmp[7]);    
            }else{
				
			}  
	    }
	  }
         
#if 0           
	  }else if (TX_tmp[4] >= 0x7f) {
	     if (trigger_ff){
	        TX_tmp[0] = 0x00;
	        TX_tmp[1] = 0x00;
	        TX_tmp[2] = 0x00;
	        TX_tmp[3] = 0x00;   
	        TX_tmp[4] = 0xff;
	        //Setup GP1 trigger time! 
	        //Register 0x61 to 0x80
            iow(db, DM9051_1588_CLK_CTRL, 0x80);

//#if DEBUG_PPS  
#if 1 
            temp = ior(db, DM9051_1588_CLK_CTRL);
            printk("Register 0x61 (0x80) = 0x%x \r\n", temp);
#endif
 
            mutex_lock(&db->spi_lockm);  
            //Register 0x68 to now.tv_nsec & now.tv_sec !
            for (int i=0; i<8 ;i++)
             iow(db, DM9051_1588_TS, TX_tmp[i]);
            mutex_unlock(&db->spi_lockm);  
    
            //Register 0x62 to 0x10 (GP 1 trigger load or event Read)
            iow(db, DM9051_1588_GP_TXRX_CTRL, 0x10);      
	  
	        trigger_70 = 0x01;
	        trigger_ff = 0x00; 
	        printk(" Trigger time = %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n", 
			 TX_tmp[0], TX_tmp[1],TX_tmp[2],TX_tmp[3],TX_tmp[4],TX_tmp[5],TX_tmp[6],TX_tmp[7]);
	 }  
	}			
	//==============================================	
#endif

	
}



#if 1
static void dm9051_ptp_extts0_work(struct work_struct *work)
{
	//struct i40e_pf *pf = container_of(work, struct i40e_pf,
	//				  ptp_extts0_work);
        struct board_info *db = container_of(work, struct board_info,
					       ptp_extts0_work);					  
	//struct i40e_hw *hw = &pf->hw;
	struct ptp_clock_event event;
	u32 hi, lo;
         
	printk("dm9051_ptp_extts0_work in\r\n");  
	/* Event time is captured by one of the two matched registers
	 *      PRTTSYN_EVNT_L: 32 LSB of sampled time event
	 *      PRTTSYN_EVNT_H: 32 MSB of sampled time event
	 * Event is defined in PRTTSYN_EVNT_0 register
	 */
	 
	//lo = rd32(hw, I40E_PRTTSYN_EVNT_L(0));
	//hi = rd32(hw, I40E_PRTTSYN_EVNT_H(0));
#if 0
	event.timestamp = (((u64)hi) << 32) | lo;

	event.type = PTP_CLOCK_EXTTS;
	//event.index = hw->pf_id;
	event.index = 0x01;

	/* fire event */
	ptp_clock_event(db->ptp_clock, &event);
#endif

    //dm9051_pps_configure(&db->ptp_caps);
    dm9051_GP1_setup(db);
    

	
}
#endif


static int ptp_9051_verify_pin(struct ptp_clock_info *ptp, unsigned int pin,
			      enum ptp_pin_function func, unsigned int chan)
{
  //struct board_info *db = container_of(ptp, struct board_info,
  //			   	         ptp_caps);
  
  printk("!!! 1. ptp_9051_verify_pin in\n");
  
  return 0;
}

/**
 * dm9051_ptp_get_ts_config - get hardware time stamping config
 * @netdev:
 * @ifreq:
 *
 * Get the hwtstamp_config settings to return to the user. Rather than attempt
 * to deconstruct the settings from the registers, just return a shadow copy
 * of the last known settings.
 **/

int dm9051_ptp_get_ts_config(struct net_device *netdev, struct ifreq *ifr)
{
 struct board_info *db = netdev_priv(netdev);
 struct hwtstamp_config *config = &db->tstamp_config;
        printk("dm9051_ptp_get_ts_config \r\n"); 
	return copy_to_user(ifr->ifr_data, config, sizeof(*config)) ?
		-EFAULT : 0;

}

static int dm9051_ptp_set_timestamp_mode(struct board_info *db,
				      struct hwtstamp_config *config)
{
#if DE_TIMESTAMP    
    printk("[in %s] XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXconfig->tx_type = %X, config->rx_filter = %X, config->flags = %Xn", __FUNCTION__, config->tx_type, config->rx_filter, config->flags);
#endif    
  config->tx_type =
#if TT9052  
		//(1 << HWTSTAMP_TX_OFF) |
		//(1 << HWTSTAMP_TX_ON) |
		//(1 << HWTSTAMP_TX_ONESTEP_SYNC);
#endif

#if DM9051A  
		(1 << HWTSTAMP_TX_ON);
		//(1 << HWTSTAMP_TX_ONESTEP_SYNC)|
#endif
		
                (1 << HWTSTAMP_TX_OFF);
  config->rx_filter =
#if TT9052  
//		(1 << HWTSTAMP_FILTER_NONE) |
//		(1 << HWTSTAMP_FILTER_PTP_V1_L4_EVENT) |
//		(1 << HWTSTAMP_FILTER_PTP_V2_L4_EVENT) |
//		(1 << HWTSTAMP_FILTER_PTP_V2_L2_EVENT) |
//		(1 << HWTSTAMP_FILTER_PTP_V2_EVENT);
#endif

#if DM9051A  

        (1 << HWTSTAMP_FILTER_ALL) |
		(1 << HWTSTAMP_FILTER_SOME);

#endif

                (1 << HWTSTAMP_FILTER_NONE);   
#if DE_TIMESTAMP		
  printk("!!! 2. dm9051_ptp_set_timestamp_mode in config->tx_type=%x, config->rx_filter=%x\n", config->tx_type, config->rx_filter);
#endif  
  return 0;
}

/**
 * dm9051_ptp_set_ts_config - set hardware time stamping config
 * @netdev:
 * @ifreq:
 *
 **/
int dm9051_ptp_set_ts_config(struct net_device *netdev, struct ifreq *ifr)
{
 struct board_info *db = netdev_priv(netdev);
 struct hwtstamp_config config;
 int err;
 //printk("[in %sdm9051_ptp_set_ts_configdm9051_ptp_set_ts_configdm9051_ptp_set_ts_configdm9051_ptp_set_ts_configdm9051_ptp_set_ts_configdm9051_ptp_set_ts_configdm9051_ptp_set_ts_configdm9051_ptp_set_ts_confign", __FUNCTION__);
        printk("dm9051_ptp_set_ts_config\r\n");
 
 if (copy_from_user(&config, ifr->ifr_data, sizeof(config)))
		return -EFAULT;
 
 err = dm9051_ptp_set_timestamp_mode(db, &config);
	if (err)
		return err;

	/* save these settings for future reference */
	memcpy(&db->tstamp_config, &config,
	       sizeof(db->tstamp_config));

	return copy_to_user(ifr->ifr_data, &config, sizeof(config)) ?
		-EFAULT : 0;       
}




void dm9051_ptp_tx_hwtstamp(struct board_info *db, struct sk_buff *skb)
{
 //struct sk_buff *skb = db->ptp_tx_skb;
 struct skb_shared_hwtstamps shhwtstamps;
 //u64 regval;
 int i;  //, adjust = 0;
 u8 temp[9];
 u16 ns_hi, ns_lo, s_hi, s_lo;
 u32 sec;
 u64 ns;
 
 //Stone add for clear TX timestamp buffer
 for (i=0; i<8; i++)
    temp[i] = 0x00;
 
 //printk("==================================> TX_hwtstamp FROM in\r\n");
 //mutex_lock(&db->spi_lockm);
  iow(db, DM9051_1588_CLK_CTRL, 0x80);  
  iow(db, DM9051_1588_GP_TXRX_CTRL, 0x01); //Read TX Time Stamp Clock Register offset 0x62, value 0x01
 //mutex_unlock(&db->spi_lockm); 
 
 //mutex_lock(&db->spi_lockm);
 for (i=0; i<8; i++)
    temp[i] = ior(db, DM9051_1588_TS);
 //mutex_unlock(&db->spi_lockm);   
#if DE_TIMESTAMP    
            printk(" TXTXTXTXTX hwtstamp 0x68 = %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x \r\n", 
			temp[0], temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7]);   
#endif			
 //printk("==================================> TX_hwtstamp FROM OUT\r\n");			 
 ns_lo = temp[0] | (temp[1] << 8);
 ns_hi = temp[2] | (temp[3] << 8);
 
 s_lo = temp[4] | (temp[5] << 8);
 s_hi = temp[6] | (temp[7] << 8);
 
 sec = s_lo;
 sec |= s_hi << 16;
 
 ns = ns_lo;
 ns |= ns_hi  << 16;

#if DE_TIMESTAMP    
//#if 1
         printk(" TXTXTXTXTX hwtstamp sec = 0x%x, ns = 0x%x \r\n", sec, ns);
			
#endif			

 
 ns += ((u64)sec) * 1000000000ULL; 

#if 1
 memset(&shhwtstamps, 0, sizeof(shhwtstamps));
 shhwtstamps.hwtstamp = ns_to_ktime(ns);
 //skb_complete_tx_timestamp(skb, &shhwtstamps);
 skb_tstamp_tx(skb, &shhwtstamps);
#endif 
 
if(skb->len == 86){
	if (skb->data[42] == 0x00){
	  printk("++++++ master => Sync or Follow_up packet  (slave => Delay Reqest packet sequestid = %x %x )!!!!! +++++ \r\n", skb->data[73], skb->data[74]); 
	  printk(" TXTXTXTXTX hwtstamp 0x68 = %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x \r\n", temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
	  //dm9051_GP1_setup(db, temp);
	  schedule_work(&db->ptp_extts0_work);
    }
	
  }	
//dev_kfree_skb_any(db->ptp_tx_skb);
//db->ptp_tx_skb = NULL;
//clear_bit_unlock(__I40E_PTP_TX_IN_PROGRESS, &db->state);

//iow(db, DM9051_1588_ST_GPIO, 0x04); //Clear TX Time Stamp Clock Register offset 0x60, value 0x04

 //printk("<== dm9051_ptp_tx_hwtstamp out\r\n");
}


void dm9051_ptp_rx_hwtstamp(struct board_info *db, struct sk_buff *skb, u8 *rxTSbyte)
{
	//u8 temp[12];
    u16 ns_hi, ns_lo, s_hi, s_lo;
    //u32 prttsyn_stat, hi, lo, 
    u32 sec;
	u64 ns;
    //int i;
    struct skb_shared_hwtstamps *shhwtstamps = NULL;
    
    //printk("==> dm9051_ptp_rx_hwtstamp in\r\n");
	/* Since we cannot turn off the Rx timestamp logic if the device is
	 * doing Tx timestamping, check if Rx timestamping is configured.
	 */
#if 0     
	if (!(pf->flags & I40E_FLAG_PTP) || !pf->ptp_rx)
		return;

	hw = &pf->hw;
    
	prttsyn_stat = rd32(hw, I40E_PRTTSYN_STAT_1);

	if (!(prttsyn_stat & BIT(index)))
		return;

	lo = rd32(hw, I40E_PRTTSYN_RXTIME_L(index));
	hi = rd32(hw, I40E_PRTTSYN_RXTIME_H(index));

	ns = (((u64)hi) << 32) | lo;

	i40e_ptp_convert_to_hwtstamp(skb_hwtstamps(skb), ns);
#endif

#if 0
   		 printk(" REAL RX TSTAMP hwtstamp= %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n", 
			rxTSbyte[0], rxTSbyte[1],rxTSbyte[2],rxTSbyte[3],rxTSbyte[4],rxTSbyte[5],rxTSbyte[6],rxTSbyte[7]);   
#endif

  //iow(db, DM9051_1588_GP_TXRX_CTRL, 0x02); //Read RX Time Stamp Clock Register offset 0x62, value 0x02

   
    //for (i=0; i<8; i++)
     //temp[i] = ior(db, DM9051_1588_TS);
     
      //printk(" dm9051_ptp_tx_hwtstamp 0x68 = %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n", 
			//temp[0], temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7]);   
    
    //ns_lo = rxTSbyte[0] | (rxTSbyte[1] << 8);
    ////ns_hi = rxTSbyte[2] | (rxTSbyte[3] << 8);
 
    //s_lo = rxTSbyte[4] | (rxTSbyte[5] << 8);
    //s_hi = rxTSbyte[6] | (rxTSbyte[7] << 8);
    
    ns_lo = rxTSbyte[7] | (rxTSbyte[6] << 8);
    ns_hi = rxTSbyte[5] | (rxTSbyte[4] << 8);
 
    s_lo = rxTSbyte[3] | (rxTSbyte[2] << 8);
    s_hi = rxTSbyte[1] | (rxTSbyte[0] << 8);
 
    sec = s_lo;
    sec |= s_hi << 16;
 
    ns = ns_lo;
    ns |= ns_hi  << 16;
 
    ns += ((u64)sec) * 1000000000ULL;
 
    //printk("dm9051_ptp_rx_hwtstamp ns_lo=%x, ns_hi=%x s_lo=%x s_hi=%x \r\n", ns_lo, ns_hi, s_lo, s_hi);
     
    shhwtstamps = skb_hwtstamps(skb);
	memset(shhwtstamps, 0, sizeof(*shhwtstamps));
    shhwtstamps->hwtstamp = ns_to_ktime(ns);

    //iow(db, DM9051_1588_ST_GPIO, 0x08); //Clear RX Time Stamp Clock Register offset 0x60, value 0x08
   //printk("<== dm9051_ptp_rx_hwtstamp out\r\n");
}


static void ptp_9051_tx_work(struct work_struct *work)
{
 struct board_info *db = container_of(work, struct board_info,
						   ptp_tx_work);
 u8 tsynctxctl;
    printk("==> ptp_9051_tx_work in \r\n");
    if (!db->ptp_tx_skb)
		return;
        
#if 0  //Stone add 
	if (time_is_before_jiffies(db->ptp_tx_start +
				   IGB_PTP_TX_TIMEOUT)) {
		dev_kfree_skb_any(db->ptp_tx_skb);
		db->ptp_tx_skb = NULL;
		clear_bit_unlock(__IGB_PTP_TX_IN_PROGRESS, &db->state);
		db->tx_hwtstamp_timeouts++;
		dev_warn(&db->pdev->dev, "clearing Tx timestamp hang\n");
		return;
	}
#endif
    
    tsynctxctl = ior(db, DM9051_1588_ST_GPIO);        //Read register 0x60 bit 2 
        //printk("ptp_9051_tx_work register 0x60 = %x \r\n", tsynctxctl);
#if 0
	if (tsynctxctl & 0x04)   //
		dm9051_ptp_tx_hwtstamp(db);
	else
		/* reschedule to check later */
		schedule_work(&db->ptp_tx_work);
#endif
    printk("<== ptp_9051_tx_work out \r\n");   
}

#endif


const char dm9051_ptp__driver_name[] = "DM9051 PTP";


/* static const struct ptp_clock_info dm9051_ptp_data = {
	.owner		= THIS_MODULE,
	.name		= "DM9051 PTP",
	.max_adj	= 50000000,
	.n_ext_ts	= N_EXT_TS,
	.n_per_out	= N_PER_OUT,
	//.adjfreq	= ravb_ptp_adjfreq,
	//.adjtime	= ravb_ptp_adjtime,
	//.gettime64	= ravb_ptp_gettime64,
	//.settime64	= ravb_ptp_settime64,
	//.enable	= ravb_ptp_enable,
}; */
//#endif

void dm9051_ptp_init(struct board_info *db)
{
	uint temp;
#if 1
//      snprintf(db->ptp_caps.name, 16, "DM9051 PTP");
        strncpy(db->ptp_caps.name, dm9051_ptp__driver_name, sizeof(db->ptp_caps.name));
	db->ptp_caps.owner = THIS_MODULE;
	db->ptp_caps.max_adj = 50000000;
	db->ptp_caps.n_ext_ts = N_EXT_TS;
        db->ptp_caps.n_per_out = N_PER_OUT;
	//db->ptp_caps.n_pins = IGB_N_SDP;
	db->ptp_caps.pps = 1;  //Stone add for 1588 pps 
#endif
#if 1
	db->ptp_caps.adjfine = ptp_9051_adjfine;
	//db->ptp_caps.adjfreq = ptp_9051_adjfreq;
	db->ptp_caps.adjtime = ptp_9051_adjtime;
	db->ptp_caps.gettime64 = ptp_9051_gettime;
	db->ptp_caps.settime64 = ptp_9051_settime;
	db->ptp_caps.enable = ptp_9051_feature_enable;
	db->ptp_caps.verify = ptp_9051_verify_pin;
		

	spin_lock_init(&db->tmreg_lock);
	INIT_WORK(&db->ptp_tx_work, ptp_9051_tx_work);

	//if (db->ptp_flags & IGB_PTP_OVERFLOW_CHECK)
	//	INIT_DELAYED_WORK(&db->ptp_overflow_work,
	//			  ptp_9051_overflow_check);
#endif

       //db->ptp_caps = dm9051_ptp_data;      

        db->tstamp_config.flags = 0;
	db->tstamp_config.rx_filter = 
#if TT9052	
	                              //(1 << HWTSTAMP_FILTER_NONE) |
                        	      //(1 << HWTSTAMP_FILTER_PTP_V1_L4_EVENT) |
                                      //(1 << HWTSTAMP_FILTER_PTP_V2_EVENT);
#endif

#if DM9051A	
        (1 << HWTSTAMP_FILTER_ALL) |
		(1 << HWTSTAMP_FILTER_SOME);
#endif
                                      
                                      (1 << HWTSTAMP_FILTER_NONE);
	db->tstamp_config.tx_type =
                                   //(1 << HWTSTAMP_TX_OFF) |
#if TT9052                                   
		                   //(1 << HWTSTAMP_TX_ONESTEP_SYNC);  
#endif

#if DM9051A 
                                   //(1 << HWTSTAMP_TX_ONESTEP_SYNC) |
                                   (1 << HWTSTAMP_TX_ON);	
	                               
#endif		                   
                                   (1 << HWTSTAMP_TX_OFF);

	//igb_ptp_reset(adapter);

        printk("ptp_clock_register in \r\n");
	db->ptp_clock = ptp_clock_register(&db->ptp_caps,
						&db->ndev->dev);
        printk("ptp_clock_register end \r\n");
	if (IS_ERR(db->ptp_clock)) {
		db->ptp_clock = NULL;
		dev_err(&db->spidev->dev, "ptp_clock_register failed\n");
	}  else if (db->ptp_clock) {
		printk("added PHC on %s\n",
			 db->ndev->name);
		//db->ptp_flags |= IGB_PTP_ENABLED;
	}           
        db->ptp_flags |= IGB_PTP_ENABLED; 
#if 1
  INIT_WORK(&db->ptp_extts0_work, dm9051_ptp_extts0_work); 	
#endif

   //Stone add for 1588 register init.
  //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock
  iow(db, DM9051_1588_RX_CONF1, 0x98); 
  //Master/Slave Mode & 1588 Version Register offset 0x64, value 0x98 (Slave mode=0x80, no version checking, RX_EN=0x10, RX2_EN=0x08)
  
  iow(db, DM9051_1588_CLK_CTRL, 0x01); //Enable PTP clock function Register offset 0x61, value 0x01
  
  //phy_write(phydev, 0x2d1, 0x0002); //enable PTP function 
  iow(db, DM9051_1588_ST_GPIO, 0x00); //Enable PTP function Register offset 0x60, value 0x00

  //iow(db, DM9051_1588_TX_CONF, 0x10); //TX Timestamp capture Register offset 0x63, value 0x10
  
  //ADDR_LOCK_TAIL_ESSENTIAL(db); //mutex_unlock

#if 1 //===================================
 //Setup GP1 to edge trigger output!
 //Register 0x60 to 0x0 (GP page (bit 1), PTP Function(bit 0))
 iow(db, DM9051_1588_ST_GPIO, 0x00);

#if DEBUG_PPS 
 temp = ior(db, DM9051_1588_ST_GPIO);
 printk("Register 0x60 (0x00) = 0x%x \r\n", temp);
#endif
 
 //Register 0x6A to 0x06 (interrupt disable(bit 2), trigger or event enable(bit 1), trigger output(bit 0))
 iow(db, DM9051_1588_GPIO_CONF, 0x06);

#if DEBUG_PPS  
 temp = ior(db, DM9051_1588_GPIO_CONF);
 printk("Register 0x6A (0x06) = 0x%x \r\n", temp);
#endif

 //Register 0x6B to 0x02(trigger out type: edge output(bit 3:2),  triger output active high(bit 1)) 
 iow(db, DM9051_1588_GPIO_TE_CONF, 0x02);

#if DEBUG_PPS  
 temp = ior(db, DM9051_1588_GPIO_TE_CONF);
 printk("Register 0x6B (0x02) = 0x%x \r\n", temp);
#endif
#endif //===================================

#if DM9051A 
   if (check_TT9052){
    //Stone add for 1588 Read 0x68 in one SPI cycle disable (register 0x63 bit 6 0:enable, 1:disable => 0x40) 
    //Stone add for 1588 TX 1-Step checksum disable (register 0x63 bit 7 0:enable, 1:disable => 0x80)                                        
    iow(db, DM9051_1588_1_STEP_CHK, 0x40 | 0x80);
   }else {//Stone add for one-step Sync packet insert time stamp! 2024-08-14!
	//Stone add for 1588 Read 0x68 in one SPI cycle enable (register 0x63 bit 6 0:enable, 1:disable => 0x40) 
    //Stone add for 1588 TX 1-Step checksum enable (register 0x63 bit 7 0:enable, 1:disable => 0x80)                                        
    iow(db, DM9051_1588_1_STEP_CHK, 0x00);   
   }	   
   
#endif

}

void dm9051_ptp_stop(struct board_info *db)
{

 if (db->ptp_clock) { 
  ptp_clock_unregister(db->ptp_clock);
  db->ptp_clock = NULL;
  printk("dm9051_ptp_stop remove PTP clock!!!\r\n");
 }
}
