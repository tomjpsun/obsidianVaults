// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2022 Davicom Semiconductor,Inc.
 * Davicom DM9051 SPI Fast Ethernet Linux driver
 */
#define DEBUG
#include <linux/cdev.h> //Neww.JJ.20230301.KT5.4
#include <linux/crc32.h> //Neww.JJ.20230301.KT5.4
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/gpio.h> //Neww.JJ.20220907
#include <linux/interrupt.h>
#include <linux/iopoll.h>
#include <linux/irq.h>
#include <linux/mii.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/phy.h>
#include <linux/regmap.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/spi/spi.h>
#include <linux/types.h>

//.#define prob_on 1

//Stone add for 1588

	#define spi_block 1
	#define prob_block 1

//.#define DRIVER_OPS 0
//.#define has_ndev_ops DRIVER_OPS
	
#include "dm9051.h"
#include "driver_conf.h"
#include "dm9051_ptp.h"

#include <linux/net_tstamp.h>
#include <linux/ptp_classify.h>
#include <linux/ptp_clock_kernel.h>


#if 0 //#if MOD_PHYLIB == 0
#undef phy_start_aneg
#undef phy_support_sym_pause
#undef phy_start
#undef phy_stop
#undef phy_connect
#undef phy_disconnect
//#undef phy_set_sym_pause
//#undef phy_speed_to_str
//#undef phy_duplex_to_str
//#undef dm9051_mdio_write
//#undef dm9051_mdio_read

#define phy_start_aneg(phydev)
#define phy_support_sym_pause(phydev)
#define phy_start(phydev)
#define phy_stop(phydev)
#define phy_connect(ndev, phy_id, handle, cmii)
#define phy_disconnect(phydev)
//#define phy_set_sym_pause(db, rx_pause, tx_pause, autoneg)
//#define phy_speed_to_str(speed) "speed"
//#define phy_duplex_to_str(duplex) "duplex"
//#define dm9051_mdio_write(mdiobus, addr, reg_num, val_in)
//#define dm9051_mdio_read(mdiobus, addr, reg_num)
#endif //#endif

/**
 * struct rx_ctl_mach - rx activities record
 * @status_err_counter: rx status error counter
 * @large_err_counter: rx get large packet length error counter
 * @rx_err_counter: receive packet error counter
 * @tx_err_counter: transmit packet error counter
 * @fifo_rst_counter: reset operation counter
 *
 * To keep track for the driver operation statistics
 */
struct rx_ctl_mach {
    u16             status_err_counter;
    u16             large_err_counter;
    u16             rx_err_counter;
    u16             tx_err_counter;
    u16             fifo_rst_counter;
};

/**
 * struct dm9051_rxctrl - dm9051 driver rx control
 * @hash_table: Multicast hash-table data
 * @rcr_all: KS_RXCR1 register setting
 *
 * The settings needs to control the receive filtering
 * such as the multicast hash-filter and the receive register settings
 */
struct dm9051_rxctrl {
    u16             hash_table[4];
    u8              rcr_all;
};

/**
 * struct dm9051_rxhdr - rx packet data header
 * @headbyte: lead byte equal to 0x01 notifies a valid packet
 * @status: status bits for the received packet
 * @rxlen: packet length
 *
 * The Rx packed, entered into the FIFO memory, start with these
 * four bytes which is the Rx header, followed by the ethernet
 * packet data and ends with an appended 4-byte CRC data.
 * Both Rx packet and CRC data are for check purpose and finally
 * are dropped by this driver
 */
struct dm9051_rxhdr {
    u8              headbyte;
    u8              status;
    __le16              rxlen;
};



#if 1
/**
 * struct board_info - maintain the saved data
 * @spidev: spi device structure
 * @ndev: net device structure
 * @mdiobus: mii bus structure
 * @phydev: phy device structure
 * @txq: tx queue structure
 * @regmap_dm: regmap for register read/write
 * @regmap_dmbulk: extra regmap for bulk read/write
 * @rxctrl_work: Work queue for updating RX mode and multicast lists
 * @tx_work: Work queue for tx packets
 * @pause: ethtool pause parameter structure
 * @spi_lockm: between threads lock structure
 * @reg_mutex: regmap access lock structure
 * @bc: rx control statistics structure
 * @rxhdr: rx header structure
 * @rctl: rx control setting structure
 * @msg_enable: message level value
 * @imr_all: to store operating imr value for register DM9051_IMR
 * @lcr_all: to store operating rcr value for register DM9051_LMCR
 *
 * The saved data variables, keep up to date for retrieval back to use
 */
struct board_info {
    u32             msg_enable;
    struct spi_device       *spidev;
    struct net_device       *ndev;
#if CODE_SUPPORT_PHYLIB
//.#if MOD_PHYLIB == 1
    struct mii_bus          *mdiobus;
    struct phy_device       *phydev;
//.#endif
#endif
    struct sk_buff_head     txq;
#if CODE_SUPPORT_REGMAP
    struct regmap           *regmap_dm;
    struct regmap           *regmap_dmbulk;
#endif
#if 1
    struct work_struct      rxctrl_work;
    struct work_struct      tx_work;
#endif

//#if CODE_SETUP_INT_FIXED == 0 //[SO IS POLLING]
	struct delayed_work		irq_workp;
//#elif DAVICOM_CONF < DM9051_KERNEL_4_19
    struct work_struct		irq_worki;
//#endif

//.#if MOD_OBJ_MII == 1
    struct mii_if_info      mii;
    struct delayed_work     phy_poll;
//.#endif
    struct ethtool_pauseparam   pause;
    struct mutex            spi_lockm;
    struct mutex            reg_mutex;
    struct rx_ctl_mach      bc;
    struct dm9051_rxhdr     rxhdr;
    struct dm9051_rxctrl    rctl;
    u8              imr_all;
    u8              lcr_all;
    u8              intcr_all;
    
//#if MOD_REGMAP0ExT // (not succeed coding), (MOD_REGMAP0ExT) == 1, in doing eliminate lagcy[] here
    //#if 0 //(not succeed coding), 1
    //
    // No lagcy[] here; (so not memcpy in _dm9051_write_mem() before _spi_sync())
    //
    //#else
    u8              lagcy_buf[1536+ 4];
    //#endif
//#endif


//#if DM9051_1588
#if 1
      //Stone add for PTP
        unsigned long state; 
        struct ptp_clock *ptp_clock;
	struct ptp_clock_info ptp_caps;
	struct delayed_work ptp_overflow_work;
	struct work_struct ptp_tx_work;
	struct work_struct ptp_extts0_work;
	struct sk_buff *ptp_tx_skb;
	struct hwtstamp_config tstamp_config;
	//unsigned long ptp_tx_start;
	//unsigned long last_rx_ptp_check;
	//unsigned long last_rx_timestamp;
	unsigned int ptp_flags;
	spinlock_t tmreg_lock;
	//struct cyclecounter cc;
	//struct timecounter tc;
	//u32 tx_hwtstamp_timeouts;
	//u32 rx_hwtstamp_cleared;
	//bool pps_sys_wrap_on; 
    
    //End of PTP
#endif


};

#endif

#if spi_block == 1
#include "linux/spi/readwrite_spi.c"
//#include "linux/interrupt/supporti.c"
//#include "linux/polling/supportp.c"
#endif

//#if prob_on
static int dm9051_map_chipid(struct board_info *db)
{
#if spi_block
    struct device *dev = &db->spidev->dev;
    unsigned int ret;
    unsigned short wid;
    unsigned int rb;
    u8 buff[6];

    ret = dm9051_get_regs(db, DM9051_VIDL, buff, sizeof(buff));
    if (ret < 0)
		return ret;

    wid = get_unaligned_le16(buff + 2);
    if (wid != DM9051_ID && wid != DM9055_ID) {
		dev_info(dev, "chipid error as %04x !\n", wid);
		//dev_err(dev, "chipid error as %04x !\n", wid);
		//printk("chipid error as %04x !\n", wid);
		return -ENODEV;
    }

    dm9051_get_reg(db, 0x5c, &rb);

    dev_info(dev, "chip %04x found\n", wid);
    dev_info(dev, "chip rev %02x\n", rb & 0xff);
    //printk("chip %04x found\n", wid);
    //printk("chip rev %02x\n", rb & 0xff);
#endif
    return 0;
}
//#endif //prob_on

//#if has_ndev_ops
/* Read DM9051_PAR registers which is the mac address loaded from EEPROM while power-on
 */
static int dm9051_map_etherdev_par(struct board_info *db)
{
	struct net_device *ndev = db->ndev;
    u8 addr[ETH_ALEN];
    int ret;

    ret = dm9051_get_regs(db, DM9051_PAR, addr, sizeof(addr));
    printk("0-MAC address %x:%x:%x:%x:%x:%x ret %x \r\n",addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], ret);
    if (ret < 0)
    return ret;

    if (!is_valid_ether_addr(addr)) {
    eth_hw_addr_random(ndev);
    printk("1-MAC address %x:%x:%x:%x:%x:%x \r\n",ndev->dev_addr[0], ndev->dev_addr[1], ndev->dev_addr[2], ndev->dev_addr[3], ndev->dev_addr[4], ndev->dev_addr[5]);
    ret = dm9051_set_regs(db, DM9051_PAR, ndev->dev_addr, ETH_ALEN);
    if (ret < 0)
        return ret;

    dev_dbg(&db->spidev->dev, "Use random MAC address\n");
    return 0;
    }

#if DAVICOM_CONF >= DM9051_KERNEL_5_15 // eth_hw_xx cpy to 'dev_addr'
    eth_hw_addr_set(ndev, addr);
#else
    memcpy(ndev->dev_addr, addr, 6); //such copy to it instead
#endif
    printk("2-MAC address %x:%x:%x:%x:%x:%x \r\n",ndev->dev_addr[0], ndev->dev_addr[1], ndev->dev_addr[2], ndev->dev_addr[3], ndev->dev_addr[4], ndev->dev_addr[5]);
    return 0;
}
//#endif

static int dm9051_update_fcr(struct board_info *db)
{
    u8 fcr = 0;

    if (db->pause.rx_pause)
    fcr |= FCR_BKPM | FCR_FLCE;
    if (db->pause.tx_pause)
    fcr |= FCR_TXPEN;
    
    //[cast to flow control  enable].jj
    //.#if MOD_REGMAP == 0
    //if  ((fcr & FCR_RXTX_BITS) != FCR_RXTX_BITS) {
	//	printk("dm9.update_fcr. 0x%02x cast to 0x%02x\n", (u8) fcr, (u8) (fcr | FCR_RXTX_BITS));
	//	fcr |= FCR_RXTX_BITS;
    //}
    //.#endif

    return dm9051_update_bits(db, DM9051_FCR, FCR_RXTX_BITS, fcr);
}

/* ethtool-ops
 */
//ethtool_ops

/* ndev-ops
 */
//ndev_ops

//#if has_ndev_ops || 1

/* transmit a packet,
 * return value,
 *   0 - succeed
 *  -ETIMEDOUT - timeout error
 */
static int dm9051_single_tx(struct board_info *db, u8 *buff, unsigned int len)
{
	int ret;
	

	ret = dm9051_nsr_poll(db);
	if (ret) {
		if (ret != -ETIMEDOUT) {
			printk("dm9051_nsr_poll ret %d (error!)\n", ret);
			return ret;
		}
	}

#if 0 // (not succeed coding), (MOD_REGMAP0ExT) == 1
	ret = dm9051_write_mem(db, DM_SPI_MWCMD, buff, ((len + 1) & (~1)) + 1); //'wb'
#else
	ret = dm9051_write_mem(db, DM_SPI_MWCMD, buff, (len + 1) & (~1)); //'wb'
#endif
	if (ret)
		return ret;

	ret = dm9051_set_regs(db, DM9051_TXPLL, &len, 2);
	if (ret < 0)
		return ret;

#if TT9052 		
	if (len == 86)
#endif
#if DM9051A
    if (len == 88)
#endif
	{
	 //printk("packet len = %x, skb->data[42] = %x  \r\n", len, buff[42]);	
	 if (buff[42] == 0x00){ //TX packet message ID is 0x00 Sync message!
	   printk("TX packet is Sync!!! \r\n");	 
#if TT9052 		
       return dm9051_set_reg(db, DM9051_TCR, (0x80 | TCR_TXREQ));
#endif

#if DM9051A //Stone add for one-step Sync packet insert time stamp! 2024-08-14! 
       if (check_TT9052){
		   
	   return dm9051_set_reg(db, DM9051_TCR, (0x80 | TCR_TXREQ));
	   
	   }else{//Stone add for one-step Sync packet insert time stamp! 2024-08-14!
		   	   
        return dm9051_set_reg(db, DM9051_TCR, (0x80 | TCR_TXREQ | 0x40)); 
       
       }
#endif       
       
      }
     }
     
#if 0     
    if (len == 86)
	{
	 //printk("packet len = %x, skb->data[42] = %x  \r\n", len, buff[42]);	
	 if (buff[42] == 0x08){ //TX packet message ID is 0x08 follow up message!
	   printk("TX packet is follow up!!! \r\n");	  		
       return dm9051_set_reg(db, DM9051_TCR, (0x80 | TCR_TXREQ));
      }
     } 
     
     if (len == 96)
	{
	 //printk("packet len = %x, skb->data[42] = %x  \r\n", len, buff[42]);	
	 if (buff[42] == 0x09){ //TX packet message ID is 0x09 Delay response message!
	   printk("TX packet is Delay response!!! \r\n");	  		
       return dm9051_set_reg(db, DM9051_TCR, (0x80 | TCR_TXREQ));
      }
     }
#endif
     
    if (len == 106)
	{
	 //printk("packet len = %x, skb->data[42] = %x  \r\n", len, buff[42]);
	 if (buff[42] == 0x0b){ //TX packet message ID is 0x0b Anounce message!
	   printk("TX packet is Anounce!!! \r\n");	  		
       return dm9051_set_reg(db, DM9051_TCR, (0x80 | TCR_TXREQ));
      }
     } 
     
    return dm9051_set_reg(db, DM9051_TCR, (TCR_TXREQ)); 
}

static int dm9051_loop_tx(struct board_info *db)
{
	struct net_device *ndev = db->ndev;
	int ntx = 0;
	int ret, i;
	unsigned char tmp, tmp_1, tmp_0;
	
#if 0    
printk("[in %s] \n", __FUNCTION__);
#endif
	while (!skb_queue_empty(&db->txq)) {
		struct sk_buff *skb;
		unsigned int skb_single_len;

		skb = skb_dequeue(&db->txq);
		if (skb) {
			
			skb_single_len = skb->len;
			function_monitor_tx(ndev, skb);

		#if 0 // (not succeed coding), (MOD_REGMAP0ExT) == 1
			/*
			 * Special procedure for _MOD_REGMAP0ExT_ to copy skb here (so not memcpy in _dm9051_write_mem() before _spi_sync()) !
			 */
			/*
			 * (not succeed coding), skb_copy_expand(skb, 1, 0, GFP_ATOMIC) with head-room 1, is one thing need further check!
			 */
			do {
				struct sk_buff *skb2;

			#ifdef DM9051_SKB_PROTECT
				if (skb->len & 1)
				skb2 = skb_copy_expand(skb, 1, 1, GFP_ATOMIC);
				else
			#endif
				skb2 = skb_copy_expand(skb, 1, 0, GFP_ATOMIC);
				if (!skb2)
					printk("[dm9051 _MOD_REGMAP0ExT_] - Error len %d, since _copy_expand with memory leak!\n", skb->len);

				dev_kfree_skb(skb);
				skb = skb2;
			} while(0);
		#else
			/*
			 * Standard normal procedure!
			 */
			#ifdef DM9051_SKB_PROTECT
			if (skb->len & 1) {
				struct sk_buff *skb2;

				skb2 = skb_copy_expand(skb, 0, 1, GFP_ATOMIC);
				if (!skb2)
					printk("[dm9051 MUST_WB_SUPPORT] - eliminate 1 tx packet which is len %d, since _copy_expand with memory leak!\n", skb->len);

				dev_kfree_skb(skb);
				skb = skb2;
			}
			#endif
		#endif
		
		    dm9051_get_reg(db, DM9051_NSR, &tmp);
	        //printk("0. register 0x01 = %x \r\n", tmp);
		
			/*
			 * Procedure for the identical part!
			 */
			if (skb) { //identical to.
				ntx++;
				ret = dm9051_single_tx(db, skb->data, skb_single_len); //org skb->len

               if (ret < 0) {
                                        dev_kfree_skb(skb);    
					db->bc.tx_err_counter++;
					return 0;
				}  
		
		//=================================================		
			dm9051_get_reg(db, DM9051_NSR, &tmp_0);
	        //printk("0.1 register 0x01 = %x \r\n", tmp_0);	
	        if (tmp != tmp_0){
				i = 0;
			//Stone add for check TX complete?
             do{
                
                dm9051_get_reg(db, DM9051_NSR, &tmp);
	            //printk("register 0x01 = %x \r\n", tmp);
	            tmp_0 = tmp & 0x0c;
	            i++;
			    }while (tmp_0 != 0x0c);
			    
			    //printk("i = 0x%x, register 0x01 = %x \r\n", i, tmp);
	         
			  }
	        //printk("===== packet len = 0x%x =====\r\n", skb_single_len);
		
		//=================================================
			
                                //Stone add for 1588    
#if 1
                                //ret = ior(db, DM9051_TCR);
                                //while( ret == 0x1 ){  ret = ior(db, DM9051_TCR); printk("TX \r\n"); }
                                           
                                //ret = ior(db, DM9051_1588_ST_GPIO);        //Read register 0x60 bit 2 
                                //printk("ptp_9051_tx_work register 0x60 = %x \r\n", ret);
                     	        //if (ret & 0x04) {  

                      //Stone add for 1588 TX timestamp debug! 
                      if (skb_single_len == 86)
	                  {
						//printk("packet len = %x, skb->data[42] = %x  \r\n", skb_single_len, skb->data[42]);  
	                   if (skb->data[42] == 0x00){ //TX packet message ID is 0x00 Sync message!
#if 0						      
                         //Stone add for check TX complete?
                         do{
		                    dm9051_get_reg(db, DM9051_NSR, &tmp);
		                    printk("Sync register 0x01 = %x \r\n", tmp);
		                    tmp_1 = tmp & 0x0c; //register 0x01 bit 2,3 TX packet index I & II complete status!
	                       //}while(tmp_1!= 0x0c);
	                       }while(!tmp_1);
	                     //dm9051_set_reg(db, DM9051_NSR, 0x0c); //Clear register 0x01 bit 2,3!  
#endif 
 		                 dm9051_ptp_tx_hwtstamp(db, skb); 
					   }
				      }
				      
				      if (skb_single_len == 86)
	                  {
						//printk("packet len = %x, skb->data[42] = %x  \r\n", skb_single_len, skb->data[42]);  
	                   if (skb->data[42] == 0x08){ //TX packet message ID is 0x08 follow up message!   

 		                 dm9051_ptp_tx_hwtstamp(db, skb); 
					   }
				      }
				      
				      if (skb_single_len == 96)
	                  {
						//printk("packet len = %x, skb->data[42] = %x  \r\n", skb_single_len, skb->data[42]);  
	                   if (skb->data[42] == 0x09){ //TX packet message ID is 0x09 Delay response message!   

 		                 dm9051_ptp_tx_hwtstamp(db, skb); 
					   }
				      }
				      
				      if (skb_single_len == 106)
	                  {
						//printk("packet len = %x, skb->data[42] = %x  \r\n", skb_single_len, skb->data[42]);  
	                   if (skb->data[42] == 0x0b){ //TX packet message ID is 0x0b Anounce message!   
#if 0                         
                        //Stone add for check TX complete?
                        do{
		                   dm9051_get_reg(db, DM9051_NSR, &tmp);
		                   printk("Anounce register 0x01 = %x \r\n", tmp);
		                   tmp_1 = tmp & 0x0c; //register 0x01 bit 2,3 TX packet index I & II complete status!
	                      }while(tmp_1!= 0x0c);
	                    //dm9051_set_reg(db, DM9051_NSR, 0x0c); //Clear register 0x01 bit 2,3!
#endif       
 		                 dm9051_ptp_tx_hwtstamp(db, skb); 
					   }
				      }
#if DE_TIMESTAMP				 
#if 1				
                         if(skb_single_len == 106){ printk("++++++ Announce packet !!!!! +++++ \r\n");}
#endif
			 if(skb_single_len == 86){ printk("++++++ master => Sync or Follow_up packet  (slave => Delay Reqest packet sequestid = %x %x )!!!!! +++++ \r\n", skb->data[73], skb->data[74]);}        
#endif				 				 
                                 //if (skb->len > 0x50) printk("dm9051_ptp_tx_hwtstamp ret %x, skb->len %x\r\n", ret, skb->len);   
                                //}//else printk("ptp_9051_tx_work register 0x60 = %x skb->len %x \r\n", ret, skb->len);     

#endif                    

				dev_kfree_skb(skb);
#if 0
				if (ret < 0) {
					db->bc.tx_err_counter++;
					return 0;
				}
#endif
                                //Stone add for 1588 debug
                                //printk("dm9051_single_tx len = %x \r\n", skb_single_len);
				ndev->stats.tx_bytes += skb_single_len; //org skb->len
				ndev->stats.tx_packets++;
				function_monitor_txe(ndev);
                                

			}
			//if (skb) { //identical to.
				//ntx++;
				//ret = dm9051_single_tx(db, skb->data, skb_single_len); //org skb->len
				//dev_kfree_skb(skb);
				//if (ret < 0) {
					//db->bc.tx_err_counter++;
					//return 0;
				//}
				//ndev->stats.tx_bytes += skb_single_len; //org skb->len
				//ndev->stats.tx_packets++;
			//}
		}

		if (netif_queue_stopped(ndev) &&
		    (skb_queue_len(&db->txq) < DM9051_TX_QUE_LO_WATER))
			netif_wake_queue(ndev);
	}

	return ntx;
}
//#endif

#include "linux/phy/phy.c"
#include "linux/phy/internal_mii.c"
#include "linux/ops/support_ops.c"
#include "linux/interrupt/supporti.c"
#include "linux/polling/supportp.c"
#include "linux/ops/driver_ops.c"
#include "linux/ops/driver_ndev2ops.c"
//#include "phy/drive_phy.c" (put to the previous location)

//-include prob_block.c

#if prob_block

static void dm9051_operation_clear(struct board_info *db)
{
    db->bc.status_err_counter = 0;
    db->bc.large_err_counter = 0;
    db->bc.rx_err_counter = 0;
    db->bc.tx_err_counter = 0;
    db->bc.fifo_rst_counter = 0;
}

//#if MOD_OBJ_MII == 1
static void phy_poll_delay(struct work_struct *w)
{
    struct delayed_work *dw = to_delayed_work(w);
    struct board_info *db = container_of(dw, struct board_info, phy_poll);
    
    #if CODE_SUPPORT_PHYMII == 1
	//.#if CODE_SUPP_MII == 1
		if (phycfg.setup_phy & PHYCFG_MII)
			dm_carrier_poll(db);
		else
	#endif
			mdio_check_link(db);

    re_sched_phy(db); //=schedule_work(&db->phy_poll);
}
//#endif

//#if has_ndev_ops || 1
static void dm9051_tx_delay(struct work_struct *work)
{
	struct board_info *db = container_of(work, struct board_info, tx_work);
	int result;
        //printk("==> dm9051_tx_delay in\r\n");
	mutex_lock(&db->spi_lockm);

	result = dm9051_loop_tx(db);
	if (result < 0)
		netdev_err(db->ndev, "transmit packet error\n");

	mutex_unlock(&db->spi_lockm);
}

static void dm9051_rxctl_delay(struct work_struct *work)
{
	struct board_info *db = container_of(work, struct board_info, rxctrl_work);
	struct net_device *ndev = db->ndev;
	int result;
printk("[in %s] \n", __FUNCTION__);
	mutex_lock(&db->spi_lockm);
	//dm9051_phywrite(db,0x00,0x2100);   //Stone add for Switch link 
	//dm9051_phywrite(db,0x00,0x0000);     //Stone add for HUB link
	dm9051_phywrite(db,0x00,0x2000);     //Stone add for HUB link
	
	dm9051_phywrite(db,0x1B,0xE000); //Stone add for 短線
	
	result = dm9051_set_regs(db, DM9051_PAR, ndev->dev_addr, ETH_ALEN);
	if (result < 0)
		goto out_unlock;

	dm9051_set_recv(db);

	/* To has mutex unlock and return from this function if regmap function fail
	 */
out_unlock:
	mutex_unlock(&db->spi_lockm);
}
//#endif
#endif //prob_block

#if 0
//Stone add for 1588 read/write register! 
#define DM9051_REGS_SIZE		0x7E

static ssize_t dm9051_registers_read(struct file *filp, struct kobject *kobj,
	struct bin_attribute *bin_attr, char *buf, loff_t off, size_t count)
{
	size_t i;
	u32 *addr;
	u16 *addr_16;
	u8 *addr_8;
	char reg;
	u32 val;
	struct device *dev;
	struct net_device *netdev;
	struct board_info *db;
	int ret;

	if (unlikely(off >= DM9051_REGS_SIZE))
		return 0;

	//if ((off + count) >= DM9051_REGS_SIZE)
	//	count = DM9051_REGS_SIZE - off;

	//if (unlikely(!count))
	//	return count;

	dev = container_of(kobj, struct device, kobj);
	netdev = to_net_dev(dev);
	db = netdev_priv(netdev);

        printk("dm9051_registers_read off = 0x%x, count = 0x%x \r\n", off, count);
	
	reg = off & 0xff;
	//addr = (u32 *) buf;
	//addr_16 = (u16 *) buf;
	addr_8 = (u8 *) buf;
	//if (4 == count && (reg & 3))
	//	return 0;
	//*addr = 0;
	
	//ret = dm9051_get_regs(db, reg, addr_8, sizeof(addr_8));
        //if (ret < 0)
        //  return ret;
	
	//dm9051_get_reg(db, reg, addr_8);
	mutex_lock(&db->spi_lockm);
	ret = dm9051_get_regs(db, reg, addr_8, 1);
	mutex_unlock(&db->spi_lockm);
	printk("dm9051_registers_read reg = 0x%x, addr_8 = 0x%x \r\n", reg, addr_8[0]);
	
	return 8;

#if 0
	if (reg >= 0x10000) {
		u32 cmd;
		u32 data;
		u32 phy_id_cmd;
		u32 phy_id_data;

		reg &= 0xffff;
		cmd = smi_reg(SW_CSR_CMD, &phy_id_cmd);
		data = smi_reg(SW_CSR_DATA, &phy_id_data);
		for (i = 0; i < count; i += 4, reg += 4, addr++)
			*addr = ind_r(bp->mii_bus, phy_id_cmd, cmd,
				phy_id_data, data, reg);
		return i;
	} else if (reg >= 0x1000) {
		u32 phy_id;

		reg = smi_reg(reg, &phy_id);
		for (i = 0; i < count; i += 4, reg += 4, addr++)
			*addr = csr_r(bp->mii_bus, phy_id, reg);
		return i;
	}

	if (reg >= 0x800) {
		int err;
		u32 phy_id = reg >> 6;
		struct mii_bus *bus = bp->mii_bus;

		reg &= (1 << 6) - 1;
		for (i = 0; i < count; i += 2, reg += 2, addr_16++) {
			err = bus->read(bus, phy_id, reg / 2);
			if (err >= 0)
				*addr_16 = (u16) err;
			else
				*addr_16 = 0;
		}
	} else {
		for (i = 0; i < count; i += 4, reg += 4, addr++) {
			val = __raw_readl(bp->regs + reg);
			if (count >= 4)
				*addr = val;
			else if (count >= 2)
				*addr_16 = (u16) val;
			else
				*addr_8 = (u8) val;
		}
	}
	
	return i;
#endif
	
}

static ssize_t dm9051_registers_write(struct file *filp, struct kobject *kobj,
	struct bin_attribute *bin_attr, char *buf, loff_t off, size_t count)
{
	size_t i;
	u32 *addr;
	u16 *addr_16;
	u8 *addr_8;
	u32 reg;
	struct device *dev;
	struct net_device *netdev;
	struct board_info *db;
	int ret;

	if (unlikely(off >= DM9051_REGS_SIZE))
		return -EFBIG;

	if ((off + count) >= DM9051_REGS_SIZE)
		count = DM9051_REGS_SIZE - off;

	if (unlikely(!count))
		return count;

	dev = container_of(kobj, struct device, kobj);
	netdev = to_net_dev(dev);
	db = netdev_priv(netdev);

	reg = off;
	//addr = (u32 *) buf;
	//addr_16 = (u16 *) buf;
	addr_8 = (u8 *) buf;
        
	printk("dm9051_registers_write reg = 0x%x, addr_8 = 0x%x \r\n", reg, addr_8[0]);
        mutex_lock(&db->spi_lockm);
	//dm9051_set_reg(db, reg, addr_8);
	ret = dm9051_set_regs(db, reg, addr_8, 1);
	mutex_unlock(&db->spi_lockm);
	printk("+++ dm9051_registers_write reg = 0x%x, addr_8 = 0x%x +++\r\n", reg, addr_8[0]);
        
        return 8;
//#ifdef SW_CSR_CMD
#if 0
	if (reg >= 0x10000) {
		u32 cmd;
		u32 data;
		u32 phy_id_cmd;
		u32 phy_id_data;

		reg &= 0xffff;
		cmd = smi_reg(SW_CSR_CMD, &phy_id_cmd);
		data = smi_reg(SW_CSR_DATA, &phy_id_data);
		for (i = 0; i < count; i += 4, reg += 4, addr++)
			ind_w(bp->mii_bus, phy_id_cmd, cmd,
				phy_id_data, data, reg, *addr);
		return i;
	} else if (reg >= 0x1000) {
		u32 phy_id;

		reg = smi_reg(reg, &phy_id);
		for (i = 0; i < count; i += 4, reg += 4, addr++)
			csr_w(bp->mii_bus, phy_id, reg, *addr);
		return i;
	}
//#endif
	if (reg >= 0x800) {
		int err;
		u32 phy_id = reg >> 6;
		struct mii_bus *bus = bp->mii_bus;

		reg &= (1 << 6) - 1;
		for (i = 0; i < count; i += 2, reg += 2, addr_16++)
			err = bus->write(bus, phy_id, reg / 2, *addr_16);
	} else {
		for (i = 0; i < count; i += 4, reg += 4, addr++) {
			if (count >= 4)
				__raw_writel(*addr, bp->regs + reg);
			else {
				u32 val = __raw_readl(bp->regs + reg);

				if (count >= 2) {
					val &= ~0xffff;
					val |= *addr_16;
				} else {
					val &= ~0xff;
					val |= *addr_8;
				}
				__raw_writel(val, bp->regs + reg);
			}
		}
	}
	return i;
#endif	
}

static struct bin_attribute dm9051_registers_attr = {
	.attr = {
		.name	= "registers",
		.mode	= S_IRUSR | S_IWUSR,
	},
	.size	= DM9051_REGS_SIZE,
	.read	= dm9051_registers_read,
	.write	= dm9051_registers_write,
};




//===============================================================
#endif

#include "dm9051_ptp.h"
static int dm9051_probe(struct spi_device *spi)
{
    struct device *dev = &spi->dev;
    struct net_device *ndev;
    struct board_info *db;
    int ret ;
unsigned int val = 0xffff;
  dm9051_cache_desc_init("dm9051_probe", 0);

    #if DAVICOM_CONF < DM9051_KERNEL_4_19
    ndev = alloc_etherdev(sizeof(struct board_info));
    #else
    ndev = devm_alloc_etherdev(dev, sizeof(struct board_info));
    #endif
    if (!ndev)
		return -ENOMEM;

    SET_NETDEV_DEV(ndev, dev);
    dev_set_drvdata(dev, ndev);

    db = netdev_priv(ndev);

    db->msg_enable = 0;
    db->spidev = spi;
    db->ndev = ndev;

    mutex_init(&db->spi_lockm);
    mutex_init(&db->reg_mutex);

	if (phycfg.setup_phy & (PHYCFG_MII | PHYCFG_MDIO)) {
		//#if MOD_OBJ_MII == 1
		INIT_DELAYED_WORK(&db->phy_poll, phy_poll_delay); //not BY INIT_WORK(&db->phy_poll, int_phy_poll);
		//#endif
	}
    
	//#if has_ndev_ops || 1
    INIT_WORK(&db->rxctrl_work, dm9051_rxctl_delay);
    INIT_WORK(&db->tx_work, dm9051_tx_delay);
	//#endif

	if (threadedcfg.interrupt_supp == THREADED_POLL)
		INIT_DELAYED_WORK(&db->irq_workp, dm9051_irq_delayp); //='dm9051_continue_poll'
	else if (threadedcfg.interrupt_supp == THREADED_INT2)
		INIT_WORK(&db->irq_worki, dm9051_irq_delayi);

    ret = dm9051_map_init(spi, db);
    if (ret)
		goto out; //return ret;

    ret = dm9051_map_chipid(db);
    if (ret) {
		//[chipid-error]
		dev_info(dev, "%s - spi%d.%d\n", "dm9051_probe.chipid.err", spi->master->bus_num, spi->chip_select);
		dev_info(dev, "%s - spi speed %d irq-no %d\n", "dm9051_probe.chipid.err", spi->max_speed_hz, spi->irq);
		//[Parameter-check1]
		dev_info(dev, "%s - pls check parameters below:", "dm9051_probe.info");
		dm9051_driver_info("dm9051_probe", "........", dev);
		dev_info(dev, "%s - spi speed %d irq-no %d\n", "dm9051_probe", spi->max_speed_hz, spi->irq);
		//[Parameter-check2]
		dev_info(dev, "%s - pls check parameters below:", "dm9051_open.info");
		dm9051_open_info(&spi->dev, spi);	
		goto out;
	}

	dm9051_driver_info("dm9051_probe", "........", dev);
	dev_info(dev, "%s - spi speed %d irq-no %d\n", "dm9051_probe", spi->max_speed_hz, spi->irq);

    ret = dm9051_map_etherdev_par(db);
    if (ret < 0)
		goto out; //return ret;

    ret = dm9051_mdio_register(db);
    if (ret)
		goto out; //return ret;

dm9051_phyread(db, 0x01, &val);
printk("[in %s] val = %X\n", __FUNCTION__, val);
	ret = dm9051_phy_connect(db);
    if (ret)
		goto out; //return ret;

    dm9051_operation_clear(db);
    skb_queue_head_init(&db->txq);
	
	//final-kickoff
	ret = dm9051_ndev_register(ndev, db);
	if (ret == 0){
//#if DM9051_1588
#if 1
        printk("dm9051_ptp_init........ \r\n"); 
        dm9051_ptp_init(db);  //Stone add for 1588
        printk("dm9051_ptp_init........end\r\n");  
	
	//Stone add for 1588 read/write register!
	//ret = sysfs_create_bin_file(&ndev->dev.kobj,&dm9051_registers_attr); 
#endif          
		return 0;
         }

out:
    #if DAVICOM_CONF < DM9051_KERNEL_4_19
    free_netdev(ndev);
    #else
    //;
    #endif

    return ret;
}

#define DM9051_REMOVE_CLEANUP()  printk("[dm9 remove cleanup]\n")

static void dm9051_drv_remove(struct spi_device *spi)
{
    #if CODE_SUPPORT_PHYLIB || DAVICOM_CONF < DM9051_KERNEL_5_15
    struct device *dev = &spi->dev;
    struct net_device *ndev = dev_get_drvdata(dev);
    #if CODE_SUPPORT_PHYLIB
		struct board_info *db = to_dm9051_board(ndev);
    #endif
    #endif
    
    //#if DAVICOM_CONF < DM9051_KERNEL_5_15
    //struct device *dev = &spi->dev;
    //struct net_device *ndev = dev_get_drvdata(dev);
    //#endif
    
    //#if CODE_SUPPORT_PHYLIB /* || DAVICOM_CONF < DM9051_KERNEL_5_15 */
    //struct board_info *db = to_dm9051_board(ndev);
    //#endif

	#if CODE_SUPPORT_PHYLIB
	if (phycfg.setup_phy & PHYCFG_LIB)
		phy_disconnect(db->phydev);
	#endif
	
	#if DAVICOM_CONF < DM9051_KERNEL_5_15 //&& MOD_PHYLIB == 1 // non-devm unreg (obsect)
	#if CODE_SUPPORT_PHYLIB
	if (phycfg.setup_phy & PHYCFG_LIB) {
		mdiobus_unregister(db->mdiobus); //20230302 for Linux 5.4
		mdiobus_free(db->mdiobus); //20230301 for Linux 5.4, tmp, ? devm_mdiobus_free(&spi->dev, db->mdiobus);
	}
	#endif
	#endif
	
	//Stone add for 1588 read/write register!
	//sysfs_remove_bin_file(&ndev->dev.kobj, &dm9051_registers_attr);
	//Stone add for 1588 remove!!
	dm9051_ptp_stop(db);

	#if DAVICOM_CONF < DM9051_KERNEL_5_15
	dm9051_ndev_unregister(ndev);
	#endif

	#if DAVICOM_CONF < DM9051_KERNEL_4_19
    free_netdev(ndev);
	#endif
	DM9051_REMOVE_CLEANUP();
    //return 0;
}

#if defined(LOCAL_PM_SUPPORT) || defined(CONFIG_PM_SLEEP)
#if MUST_PM_SUPPORT
static int dm9051_suspend(struct device *dev)
{
    struct net_device *ndev = dev_get_drvdata(dev);
    int err;

    if (ndev) {
    if (netif_running(ndev)) {
        //struct board_info *db = to_dm9051_board(ndev);
        //dm9051_all_stop(db);
        netif_device_detach(ndev);
        err = dm9051_stop(ndev);
        if (err)
        return err;
    }
    }
    return 0;
}
static int dm9051_resume(struct device *dev)
{
    struct net_device *ndev = dev_get_drvdata(dev);
    int err;

    if (ndev) {
    if (netif_running(ndev)) {
        //struct board_info *db = to_dm9051_board(ndev);
        //dm9051_all_start(db);
        err = dm9051_open(ndev);
        if (err)
        return err;
        netif_device_attach(ndev);
    }
    }
    return 0;
}

static SIMPLE_DEV_PM_OPS(dm9051_pm_ops, dm9051_suspend, dm9051_resume);
//static const struct dev_pm_ops dm9051_pm_ops = {
//  .suspend    = dm9051_suspend,
//  .resume     = dm9051_resume,
//};
#endif
#endif

static const struct of_device_id dm9051_match_table[] = {
    { .compatible = "davicom,dm9051" },
    {}
};

static const struct spi_device_id dm9051_id_table[] = {
    { "dm9051", 0 },
    {}
};

static struct spi_driver dm9051_driver = {
    .driver = {
		.name = DRVNAME_9051,
		.of_match_table = dm9051_match_table, //This to match DTS ".compatible"
	#if defined(LOCAL_PM_SUPPORT) || defined(CONFIG_PM_SLEEP)
	#if MUST_PM_SUPPORT
		.pm = &dm9051_pm_ops,
		//.pm = {
		//  .suspend    = dm9051_suspend,
		//  .resume     = dm9051_resume,
		//},
	#endif
	#endif
    },
    .probe = dm9051_probe,
    .remove = dm9051_drv_remove,
    .id_table = dm9051_id_table,
};

//#if 0 //[test 4.1.11]
#if 1
    #define RET_DM9051_INIT() printk("[dm9051_init]\n")
    #define DM9051_CLEANUP()  printk("[dm9 exit cleanup]\n")

    #undef  module_spi_driver
    #define module_spi_driver(dmdrvr)       \
    static struct device_node *nc;      \
    static struct device_node *conf_get_node(void)  \
    {                   \
        struct device_node *node;       \
        node = of_find_compatible_node(NULL, NULL, "davicom,dm9051");   \
        /* if (node) */         \
        /* printk("*dm9 init STATE, DTS 9051 found\n"); */  \
        dm9051_node_info(node); \
        if (node && dtscfg.cfg.adopte_dts == ADOPTE_DTS) { \
			/* debug dts */ \
			\
		} \
        return node;	\
    }                   \
                \
    static int __init           \
    dm9051_init(void)           \
    {                   \
        /*RET_DM9051_INIT();*/      \
        /* "dm9051_init" */ \
        dm9051_cache_desc_init("module", 1); \
        nc = conf_get_node();       \
        /*spi_device = NULL;*/	\
        /* #if 0 */	\
		if (dtscfg.cfg.adopte_dts == ADOPTE_NDTS) \
			dm9051_conf_spi_board(nc);  \
        /* #endif */	\
        if (nc && dtscfg.cfg.adopte_dts == ADOPTE_DTS) { \
			/* printk("spi_register_driver: %s\n", dm9051_driver.driver.name); */ \
		} \
        return spi_register_driver(&dmdrvr); \
    }                   \
    static void dm9051_cleanup(void)    \
    {                   \
		if (dtscfg.cfg.adopte_dts == ADOPTE_NDTS) \
			dm9051_unconf_spi_board(nc); /*if (!nc)*/ \
        /*DM9051_CLEANUP();*/       \
        spi_unregister_driver(&dmdrvr); \
        DM9051_CLEANUP();       \
    }                   \
    module_init(dm9051_init);       \
    module_exit(dm9051_cleanup)
#endif
//#endif //[test 4.1.11]

module_spi_driver(dm9051_driver);

MODULE_AUTHOR("Joseph CHANG <joseph_chang@davicom.com.tw>");
MODULE_DESCRIPTION("Davicom DM9051 network SPI driver");
MODULE_LICENSE("GPL");
