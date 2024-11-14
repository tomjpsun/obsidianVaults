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

#if 1


int dm9051_ts_info(struct net_device *net_dev, struct ethtool_ts_info *ts_info)
{
   struct board_info *db = netdev_priv(net_dev);
printk("[in %s] XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx*dm9051_ts_infodm9051_ts_infodm9051_ts_infodm9051_ts_infodm9051_ts_info*\n", __FUNCTION__);
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
		(1 << HWTSTAMP_TX_ONESTEP_SYNC);
	ts_info->rx_filters =
		(1 << HWTSTAMP_FILTER_NONE) |
		//(1 << HWTSTAMP_FILTER_PTP_V1_L4_EVENT) |
		//(1 << HWTSTAMP_FILTER_PTP_V2_L4_EVENT) |
		//(1 << HWTSTAMP_FILTER_PTP_V2_L2_EVENT) |
		(1 << HWTSTAMP_FILTER_PTP_V2_EVENT);
	printk("dm9051_ts_info tx_types %x,  rx_filters %x \n", ts_info->tx_types, ts_info->rx_filters);
	return 0;


}


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
    //printk(" ==> ptp_9051_adjfreq in\r\n");
     rate = -ppb;

    if (-ppb < 0) {
		neg_adj = 1;
	}else {
        neg_adj = 0;
    }    
    
    hi = (rate >> 16);

	lo = rate & 0xffff;
    
	//mutex_lock(&clock->extreg_lock);
    //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock
    
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

static int ptp_9051_adjtime(struct ptp_clock_info *ptp, s64 delta)
{
 struct board_info *db = container_of(ptp, struct board_info,
					       ptp_caps);
 struct timespec64 ts;
    int sign = 1;
	int err;
    
    //printk("@@@1 ptp_dm8806_adjtime delta %llx \n", delta);
#if 1
	delta += ADJTIME_FIX;
    //printk("@@@2 ptp_dm8806_adjtime delta %llx \n", delta);
        
    if (delta < 0) {
		sign = 0;
		delta = -delta;
	}
    
    //mutex_lock(&clock->extreg_lock);
    //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock
        
    //printk("@@@2-1 ptp_dm8806_adjtime delta %llx \n", delta);
    ts.tv_sec = div_s64(delta, 0x3b9aca00);
    //printk("@@@2-2 ptp_dm8806_adjtime delta %llx sec %lx \n", delta, ts.tv_sec);
    ts.tv_nsec = (delta - (ts.tv_sec * 0x3b9aca00))& 0xffffffff;
    //ts.tv_nsec = div_s64_rem(delta, 0x3b9aca00, &rem);
	//ts.tv_nsec = (delta - (ts.tv_sec * 1000000000));
    //ts.tv_sec = rem & 0xffffffff;
     //printk("@@@3 ptp_dm8806_adjtime delta %llx  nsec=%lx  \n", delta, ts.tv_nsec);
      
    if (sign == 0)
        { 
         if (ts.tv_sec == 0)
           ts.tv_sec++;
         ts.tv_sec  = (0x100000000-ts.tv_sec);
         ts.tv_nsec = (1000000000 - ts.tv_nsec);
         }
         
    //printk("@@@4 ptp_dm8806_adjtime delta %llx sec=%lx  \n", delta, ts.tv_sec );
#endif    
    
	//ts_1 = ns_to_timespec64(delta);
    
    //printk("@@@5 ptp_dm8806_adjtime delta %llx nsec=%lx  \n", delta, ts.tv_nsec);

	//err = tdr_write(1, phydev, &ts, PTP_STEP_CLK);

#if 1
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
 iow(db, DM9051_1588_CLK_CTRL, 0x11);           //Write register 0x61 to 0x11
#endif

#if 0    //Stone add for 1588 2022-10-19
 printk("### sign %x delta %llx ts.tv_sec =  %lx, ts.tv_nsec = %llx  ###\n", sign, delta, ts.tv_sec, ts.tv_nsec);
#endif 

	//mutex_unlock(&clock->extreg_lock);
    //ADDR_LOCK_TAIL_ESSENTIAL(db); //mutex_unlock

    return 0;

}


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

static int ptp_9051_settime(struct ptp_clock_info *ptp,
			      const struct timespec64 *ts)
{
  struct board_info *db = container_of(ptp, struct board_info,
					       ptp_caps);
  
  //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock
  printk(" ==> ptp_9051_settime in \r\n");
  
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
  printk("### ts->tv_sec =  %x, ts->tv_nsec = %x  ###\n", ts->tv_sec, ts->tv_nsec);
  printk(" <== ptp_9051_settime out \r\n");
	return 0;
}


static int ptp_9051_feature_enable(struct ptp_clock_info *ptp,
			      struct ptp_clock_request *rq, int on)
{
  //struct board_info *db = container_of(ptp, struct board_info,
  //					       ptp_caps);
   printk("!!! 1. ptp_9051_feature_enable in\n");
  return -EOPNOTSUPP;

}



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
    printk("[in %s] XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXconfig->tx_type = %X, config->rx_filter = %X, config->flags = %Xn", __FUNCTION__, config->tx_type, config->rx_filter, config->flags);
  config->tx_type =
		//(1 << HWTSTAMP_TX_OFF) |
		//(1 << HWTSTAMP_TX_ON) |
		//(1 << HWTSTAMP_TX_ONESTEP_SYNC);
                (1 << HWTSTAMP_TX_OFF);
  config->rx_filter =
//		(1 << HWTSTAMP_FILTER_NONE) |
//		(1 << HWTSTAMP_FILTER_PTP_V1_L4_EVENT) |
//		(1 << HWTSTAMP_FILTER_PTP_V2_L4_EVENT) |
//		(1 << HWTSTAMP_FILTER_PTP_V2_L2_EVENT) |
//		(1 << HWTSTAMP_FILTER_PTP_V2_EVENT);
                (1 << HWTSTAMP_FILTER_NONE);   
  printk("!!! 2. dm9051_ptp_set_timestamp_mode in config->tx_type=%x, config->rx_filter=%x\n", config->tx_type, config->rx_filter);
  
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
 u64 regval;
 int i, adjust = 0;
 u8 temp[8];
 u16 ns_hi, ns_lo, s_hi, s_lo;
 u32 sec;
 u64 ns;
 
 printk("=================================================> dm9051_ptp_tx_hwtstamp in\r\n");
 
 
  iow(db, DM9051_1588_GP_TXRX_CTRL, 0x01); //Read TX Time Stamp Clock Register offset 0x62, value 0x01
 
 for (i=0; i<8; i++)
    temp[i] = ior(db, DM9051_1588_TS);
    
    
         printk(" TXTXTXTXTXTXTX dm9051_ptp_tx_hwtstamp 0x68 = %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n", 
			temp[0], temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7]);    
 ns_lo = temp[0] | (temp[1] << 8);
 ns_hi = temp[2] | (temp[3] << 8);
 
 s_lo = temp[4] | (temp[5] << 8);
 s_hi = temp[6] | (temp[7] << 8);
 
 sec = s_lo;
 sec |= s_hi << 16;
 
 ns = ns_lo;
 ns |= ns_hi  << 16;
 
 ns += ((u64)sec) * 1000000000ULL; 

#if 1
 memset(&shhwtstamps, 0, sizeof(shhwtstamps));
 shhwtstamps.hwtstamp = ns_to_ktime(ns);
 //skb_complete_tx_timestamp(skb, &shhwtstamps);
 skb_tstamp_tx(skb, &shhwtstamps);
#endif 
 
//dev_kfree_skb_any(db->ptp_tx_skb);
//db->ptp_tx_skb = NULL;
//clear_bit_unlock(__I40E_PTP_TX_IN_PROGRESS, &db->state);

//iow(db, DM9051_1588_ST_GPIO, 0x04); //Clear TX Time Stamp Clock Register offset 0x60, value 0x04

 //printk("<== dm9051_ptp_tx_hwtstamp out\r\n");
}


void dm9051_ptp_rx_hwtstamp(struct board_info *db, struct sk_buff *skb, u8 *rxTSbyte)
{
	u8 temp[12];
    u16 ns_hi, ns_lo, s_hi, s_lo;
    u32 prttsyn_stat, hi, lo, sec;
	u64 ns;
    int i;
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

   		 printk(" REAL RX TSTAMP in dm9051_ptp_rx_hwtstamp dm9051_ptp_tx_hwtstamp= %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n", 
			rxTSbyte[0], rxTSbyte[1],rxTSbyte[2],rxTSbyte[3],rxTSbyte[4],rxTSbyte[5],rxTSbyte[6],rxTSbyte[7]);   

  //iow(db, DM9051_1588_GP_TXRX_CTRL, 0x02); //Read RX Time Stamp Clock Register offset 0x62, value 0x02

   
    //for (i=0; i<8; i++)
     //temp[i] = ior(db, DM9051_1588_TS);
     
      //printk(" dm9051_ptp_tx_hwtstamp 0x68 = %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n", 
			//temp[0], temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7]);   
    
    ns_lo = rxTSbyte[0] | (rxTSbyte[1] << 8);
    ns_hi = rxTSbyte[2] | (rxTSbyte[3] << 8);
 
    s_lo = rxTSbyte[4] | (rxTSbyte[5] << 8);
    s_hi = rxTSbyte[6] | (rxTSbyte[7] << 8);
 
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
#if 1
//      snprintf(db->ptp_caps.name, 16, "DM9051 PTP");
        strncpy(db->ptp_caps.name, dm9051_ptp__driver_name, sizeof(db->ptp_caps.name));
	db->ptp_caps.owner = THIS_MODULE;
	db->ptp_caps.max_adj = 50000000;
	db->ptp_caps.n_ext_ts = N_EXT_TS;
        db->ptp_caps.n_per_out = N_PER_OUT;
	//db->ptp_caps.n_pins = IGB_N_SDP;
	db->ptp_caps.pps = 0;
#endif
#if 1
	db->ptp_caps.adjfreq = ptp_9051_adjfreq;
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
	db->tstamp_config.rx_filter = //(1 << HWTSTAMP_FILTER_NONE) |
                        	      //(1 << HWTSTAMP_FILTER_PTP_V1_L4_EVENT) |
                                      //(1 << HWTSTAMP_FILTER_PTP_V2_EVENT);
                                      HWTSTAMP_FILTER_NONE;
	db->tstamp_config.tx_type =
                                   //(1 << HWTSTAMP_TX_OFF) |
		                   //(1 << HWTSTAMP_TX_ONESTEP_SYNC);  
                                   HWTSTAMP_TX_OFF;

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

   //Stone add for 1588 register init.
  //ADDR_LOCK_HEAD_ESSENTIAL(db); //mutex_lock
  iow(db, DM9051_1588_RX_CONF1, 0x98); 
  //Master/Slave Mode & 1588 Version Register offset 0x64, value 0x98 (Slave mode=0x80, no version checking, RX_EN=0x10, RX2_EN=0x08)
  
  iow(db, DM9051_1588_CLK_CTRL, 0x01); //Enable PTP clock function Register offset 0x61, value 0x01
  
  //phy_write(phydev, 0x2d1, 0x0002); //enable PTP function 
  iow(db, DM9051_1588_ST_GPIO, 0x00); //Enable PTP function Register offset 0x60, value 0x00

  //iow(db, DM9051_1588_TX_CONF, 0x10); //TX Timestamp capture Register offset 0x63, value 0x10
  
  //ADDR_LOCK_TAIL_ESSENTIAL(db); //mutex_unlock

}

