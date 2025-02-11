2. PTP clock read/write
3. PTP TX/RX TS read
4. PTP offset write
5. PTP rate write
6. PTP 1-setp setup

========================================================================================
1. PTP init
=> PTP 1588 enable  #driver => dm9051_ptp_init()
   PTP function enable => write MAC register 0x60 bit 0 setup to 0x00 (default value 0x00!)
   PTP clock enable => write MAC register 0x61 bit 0 setup to 0x01 (只要寫一次就好,除非MAC register 0x61 bit 1 setup to 1,否則不用再寫一次,寫0不會影響!)
   PTP TX TS enable => write MAC register 0x02 bit 7 setup to 0x80
   ==Tom: 改到 single tx 的時候再寫這個 bit==
   PTP RX TS enable => write MAC register 0x64 bit 4 setup to 0x010
   PTP 1-step update check sum enable => write MAC register 0x63 bit 7 setup to 0x00 (default value 0x00 enable)
   PTP 1-step 8-byte TX TS insert add offset => write MAC register 0x65 bit 0:7 (default 0x4E => 1-setp sync packet!)
   PTP 1-step check sum address offset => write MAC register 0x66 bit 0:7 (default 0x3c => 1-setp sync packet!)

========================================================================================
2. PTP clock read/write
=> read:            #driver => ptp_9051_gettime()
        write MAC register 0x61 bit 2 & bit 7, setup to 0x84 (Clear MAC register 0x68 index to 0x0 and Read PTP clock)
        read MAC register 0x68 index 0-7 (讀8次,index 0-3 => nanosec; index 4-7 => sec 詳細的每個byte請參考data sheet)

=> write: driver => ptp_9051_settime()
         write MAC register 0x61 bit bit 7, setup to 0x80  (Clear MAC register 0x68 index to 0x0)
         write MAC register 0x68 index 0-7 (write 8次,index 0-3 => nanosec; index 4-7 => sec 詳細的每個byte請參考data sheet)
         write MAC register 0x61 bit bit 3, setup to 0x08  (write PTP clock form MAC register 0x68 index 0-7)

========================================================================================
3. PTP TX/RX TS read
=> TX TS read       #driver => dm9051_ptp_tx_hwtstamp()
             write MAC register 0x61 bit bit 7, setup to 0x80  (Clear MAC register 0x68 index to 0x0)
             write MAC register 0x62 bit 0, setup to 0x01
             read MAC register 0x68 index 0-7 (讀8次,index 0-3 => nanosec; index 4-7 => sec 詳細的每個byte請參考data sheet)

=> RX TS read  driver => dm9051_loop_rx()/dm9051_ptp_rx_hwtstamp()
             read RX packet header (rdy (1-byte), stauts (1-byte bit 5 => 0x20), 8-bytes RX TS (4-bytes sec + 4-bytes nanosec))

========================================================================================
4. PTP offset write
=> PTP offset write  #driver => ptp_9051_adjtime()
                   write MAC register 0x61 bit bit 7, setup to 0x80  (Clear MAC register 0x68 index to 0x0)
                   write MAC register 0x68 index 0-7 (write 8次,index 0-3 => nanosec; index 4-7 => sec 詳細的每個byte請參考data sheet)
                   write MAC register 0x61 bit 4, setup to 0x10

========================================================================================
5. PTP rate write
=> PTP rate write    #driver => ptp_9051_adjfine()
                 write MAC register 0x61 bit 7, setup to 0x80  (Clear MAC register 0x68 index to 0x0)
                 write MAC register 0x68 index 0-t7 (write 8次,index 0-3 => nanosec; index 4-7 => sec 詳細的每個byte請參考data sheet)
                 Write MAC register 0x61 bit 6,5 (bit 6, 0: add, 1: subtract)


========================================================================================
6. PTP 1-setp setup
=> 1-step sync insert TS and update check sum    #driver => dm9051_ptp_init()/dm9051_single_tx()
   PTP 1-step sync insert TX TS enable => write MAC reg 0x02 bit 6 to enable insert TX time stamp!


++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

PTP 1588 driver 注意事項:
1. TX skb中有特殊欄位可以讓driver確認上層要送的封包是1588應用當中的哪個封包需不需要給上層PTP TX time stamp的資料
   => skb_shinfo 中的tx_flags
      /* This data is invariant across clones and lives at
       * the end of the header data, ie. at skb->end.
       */
      struct skb_shared_info {
	   __u8		flags;
	   __u8		meta_len;
	   __u8		nr_frags;
	   __u8		tx_flags;
	   unsigned short	gso_size;
	   /* Warning: this field is not always filled in (UFO)! */
	   unsigned short	gso_segs;
	   struct sk_buff	*frag_list;
	   struct skb_shared_hwtstamps hwtstamps;
	   unsigned int	gso_type;
	   u32		tskey;

	  /*
	   * Warning : all fields before dataref are cleared in __alloc_skb()
	   */
	  atomic_t	dataref;
	  unsigned int	xdp_frags_size;

	  /* Intermediate layers must ensure that destructor_arg
	   * remains valid until skb destructor */
	  void *		destructor_arg;

	  /* must be last field, see pskb_expand_head() */
	  skb_frag_t	frags[MAX_SKB_FRAGS];
      };

   =>在dm9051_loop_tx()中加入

db->ptp_tx_flags = skb_shinfo(skb)->tx_flags; 
if (db->ptp_tx_flags){
		  db->ptp_class = ptp_classify_raw(skb);
		  db->ptp_hdr = ptp_parse_header(skb, db->ptp_class);
		  db->ptp_msgtype = ptp_get_msgtype(db->ptp_hdr, db->ptp_class);
		}

     可以讓在呼叫dm9051_single_tx()中可以判斷是否需要在TX封包內加入TX TS (1-step sync).

     或是後面可以再判斷是否要往ptp4l送上TX TS,或是其他資料.




2. ptp4l與Linux driver interface
   (目前查到的資料是kernel version 6.11, 但是測試環境是kernel verion 6.1左右的版本)
   struct ptp_clock_info - / include / linux / ptp_clock_kernel.h
/**
 * struct ptp_clock_info - describes a PTP hardware clock
 *
 * @owner:     The clock driver should set to THIS_MODULE.
 * @name:      A short "friendly name" to identify the clock and to
 *             help distinguish PHY based devices from MAC based ones.
 *             The string is not meant to be a unique id.
 * @max_adj:   The maximum possible frequency adjustment, in parts per billon.
 * @n_alarm:   The number of programmable alarms.
 * @n_ext_ts:  The number of external time stamp channels.
 * @n_per_out: The number of programmable periodic signals.
 * @n_pins:    The number of programmable pins.
 * @pps:       Indicates whether the clock supports a PPS callback.
 * @pin_config: Array of length 'n_pins'. If the number of
 *              programmable pins is nonzero, then drivers must
 *              allocate and initialize this array.
 *
 * clock operations
 *
 * @adjfine:  Adjusts the frequency of the hardware clock.
 *            parameter scaled_ppm: Desired frequency offset from
 *            nominal frequency in parts per million, but with a
 *            16 bit binary fractional field.
 *
 * @adjphase:  Indicates that the PHC should use an internal servo
 *             algorithm to correct the provided phase offset.
 *             parameter delta: PHC servo phase adjustment target
 *                              in nanoseconds.
 *
 * @getmaxphase:  Advertises maximum offset that can be provided
 *                to the hardware clock's phase control functionality
 *                through adjphase.
 *
 * @adjtime:  Shifts the time of the hardware clock.
 *            parameter delta: Desired change in nanoseconds.
 *
 * @gettime64:  Reads the current time from the hardware clock.
 *              This method is deprecated.  New drivers should implement
 *              the @gettimex64 method instead.
 *              parameter ts: Holds the result.
 *
 * @gettimex64:  Reads the current time from the hardware clock and optionally
 *               also the system clock.
 *               parameter ts: Holds the PHC timestamp.
 *               parameter sts: If not NULL, it holds a pair of timestamps from
 *               the system clock. The first reading is made right before
 *               reading the lowest bits of the PHC timestamp and the second
 *               reading immediately follows that.
 *
 * @getcrosststamp:  Reads the current time from the hardware clock and
 *                   system clock simultaneously.
 *                   parameter cts: Contains timestamp (device,system) pair,
 *                   where system time is realtime and monotonic.
 *
 * @settime64:  Set the current time on the hardware clock.
 *              parameter ts: Time value to set.
 *
 * @getcycles64:  Reads the current free running cycle counter from the hardware
 *                clock.
 *                If @getcycles64 and @getcyclesx64 are not supported, then
 *                @gettime64 or @gettimex64 will be used as default
 *                implementation.
 *                parameter ts: Holds the result.
 *
 * @getcyclesx64:  Reads the current free running cycle counter from the
 *                 hardware clock and optionally also the system clock.
 *                 If @getcycles64 and @getcyclesx64 are not supported, then
 *                 @gettimex64 will be used as default implementation if
 *                 available.
 *                 parameter ts: Holds the PHC timestamp.
 *                 parameter sts: If not NULL, it holds a pair of timestamps
 *                 from the system clock. The first reading is made right before
 *                 reading the lowest bits of the PHC timestamp and the second
 *                 reading immediately follows that.
 *
 * @getcrosscycles:  Reads the current free running cycle counter from the
 *                   hardware clock and system clock simultaneously.
 *                   If @getcycles64 and @getcyclesx64 are not supported, then
 *                   @getcrosststamp will be used as default implementation if
 *                   available.
 *                   parameter cts: Contains timestamp (device,system) pair,
 *                   where system time is realtime and monotonic.
 *
 * @enable:   Request driver to enable or disable an ancillary feature.
 *            parameter request: Desired resource to enable or disable.
 *            parameter on: Caller passes one to enable or zero to disable.
 *
 * @verify:   Confirm that a pin can perform a given function. The PTP
 *            Hardware Clock subsystem maintains the 'pin_config'
 *            array on behalf of the drivers, but the PHC subsystem
 *            assumes that every pin can perform every function. This
 *            hook gives drivers a way of telling the core about
 *            limitations on specific pins. This function must return
 *            zero if the function can be assigned to this pin, and
 *            nonzero otherwise.
 *            parameter pin: index of the pin in question.
 *            parameter func: the desired function to use.
 *            parameter chan: the function channel index to use.
 *
 * @do_aux_work:  Request driver to perform auxiliary (periodic) operations
 *                Driver should return delay of the next auxiliary work
 *                scheduling time (>=0) or negative value in case further
 *                scheduling is not required.
 *
 * Drivers should embed their ptp_clock_info within a private
 * structure, obtaining a reference to it using container_of().
 *
 * The callbacks must all return zero on success, non-zero otherwise.
 */

struct ptp_clock_info {
	struct module *owner;
	char name[PTP_CLOCK_NAME_LEN];
	s32 max_adj;
	int n_alarm;
	int n_ext_ts;
	int n_per_out;
	int n_pins;
	int pps;
	struct ptp_pin_desc *pin_config;
	int (*adjfine)(struct ptp_clock_info *ptp, long scaled_ppm);
	int (*adjphase)(struct ptp_clock_info *ptp, s32 phase);
	s32 (*getmaxphase)(struct ptp_clock_info *ptp);
	int (*adjtime)(struct ptp_clock_info *ptp, s64 delta);
	int (*gettime64)(struct ptp_clock_info *ptp, struct timespec64 *ts);
	int (*gettimex64)(struct ptp_clock_info *ptp, struct timespec64 *ts,
			  struct ptp_system_timestamp *sts);
	int (*getcrosststamp)(struct ptp_clock_info *ptp,
			      struct system_device_crosststamp *cts);
	int (*settime64)(struct ptp_clock_info *p, const struct timespec64 *ts);
	int (*getcycles64)(struct ptp_clock_info *ptp, struct timespec64 *ts);
	int (*getcyclesx64)(struct ptp_clock_info *ptp, struct timespec64 *ts,
			    struct ptp_system_timestamp *sts);
	int (*getcrosscycles)(struct ptp_clock_info *ptp,
			      struct system_device_crosststamp *cts);
	int (*enable)(struct ptp_clock_info *ptp,
		      struct ptp_clock_request *request, int on);
	int (*verify)(struct ptp_clock_info *ptp, unsigned int pin,
		      enum ptp_pin_function func, unsigned int chan);
	long (*do_aux_work)(struct ptp_clock_info *ptp);
};

----------------------------------------------------------------------------------------
/ include / uapi / linux / net_tstamp.h

/**
 * struct hwtstamp_config - %SIOCGHWTSTAMP and %SIOCSHWTSTAMP parameter
 *
 * @flags:	one of HWTSTAMP_FLAG_*
 * @tx_type:	one of HWTSTAMP_TX_*
 * @rx_filter:	one of HWTSTAMP_FILTER_*
 *
 * %SIOCGHWTSTAMP and %SIOCSHWTSTAMP expect a &struct ifreq with a
 * ifr_data pointer to this structure.  For %SIOCSHWTSTAMP, if the
 * driver or hardware does not support the requested @rx_filter value,
 * the driver may use a more general filter mode.  In this case
 * @rx_filter will indicate the actual mode on return.
 */
struct hwtstamp_config {
	int flags;
	int tx_type;
	int rx_filter;
};

/* possible values for hwtstamp_config->flags */
enum hwtstamp_flags {
	/*
	 * With this flag, the user could get bond active interface's
	 * PHC index. Note this PHC index is not stable as when there
	 * is a failover, the bond active interface will be changed, so
	 * will be the PHC index.
	 */
	HWTSTAMP_FLAG_BONDED_PHC_INDEX = (1<<0),
#define HWTSTAMP_FLAG_BONDED_PHC_INDEX	HWTSTAMP_FLAG_BONDED_PHC_INDEX

	HWTSTAMP_FLAG_LAST = HWTSTAMP_FLAG_BONDED_PHC_INDEX,
	HWTSTAMP_FLAG_MASK = (HWTSTAMP_FLAG_LAST - 1) | HWTSTAMP_FLAG_LAST
};

/* possible values for hwtstamp_config->tx_type */
enum hwtstamp_tx_types {
	/*
	 * No outgoing packet will need hardware time stamping;
	 * should a packet arrive which asks for it, no hardware
	 * time stamping will be done.
	 */
	HWTSTAMP_TX_OFF,

	/*
	 * Enables hardware time stamping for outgoing packets;
	 * the sender of the packet decides which are to be
	 * time stamped by setting %SOF_TIMESTAMPING_TX_SOFTWARE
	 * before sending the packet.
	 */
	HWTSTAMP_TX_ON,

	/*
	 * Enables time stamping for outgoing packets just as
	 * HWTSTAMP_TX_ON does, but also enables time stamp insertion
	 * directly into Sync packets. In this case, transmitted Sync
	 * packets will not received a time stamp via the socket error
	 * queue.
	 */
	HWTSTAMP_TX_ONESTEP_SYNC,

	/*
	 * Same as HWTSTAMP_TX_ONESTEP_SYNC, but also enables time
	 * stamp insertion directly into PDelay_Resp packets. In this
	 * case, neither transmitted Sync nor PDelay_Resp packets will
	 * receive a time stamp via the socket error queue.
	 */
	HWTSTAMP_TX_ONESTEP_P2P,

	/* add new constants above here */
	__HWTSTAMP_TX_CNT
};

/* possible values for hwtstamp_config->rx_filter */
enum hwtstamp_rx_filters {
	/* time stamp no incoming packet at all */
	HWTSTAMP_FILTER_NONE,

	/* time stamp any incoming packet */
	HWTSTAMP_FILTER_ALL,

	/* return value: time stamp all packets requested plus some others */
	HWTSTAMP_FILTER_SOME,

	/* PTP v1, UDP, any kind of event packet */
	HWTSTAMP_FILTER_PTP_V1_L4_EVENT,
	/* PTP v1, UDP, Sync packet */
	HWTSTAMP_FILTER_PTP_V1_L4_SYNC,
	/* PTP v1, UDP, Delay_req packet */
	HWTSTAMP_FILTER_PTP_V1_L4_DELAY_REQ,
	/* PTP v2, UDP, any kind of event packet */
	HWTSTAMP_FILTER_PTP_V2_L4_EVENT,
	/* PTP v2, UDP, Sync packet */
	HWTSTAMP_FILTER_PTP_V2_L4_SYNC,
	/* PTP v2, UDP, Delay_req packet */
	HWTSTAMP_FILTER_PTP_V2_L4_DELAY_REQ,

	/* 802.AS1, Ethernet, any kind of event packet */
	HWTSTAMP_FILTER_PTP_V2_L2_EVENT,
	/* 802.AS1, Ethernet, Sync packet */
	HWTSTAMP_FILTER_PTP_V2_L2_SYNC,
	/* 802.AS1, Ethernet, Delay_req packet */
	HWTSTAMP_FILTER_PTP_V2_L2_DELAY_REQ,

	/* PTP v2/802.AS1, any layer, any kind of event packet */
	HWTSTAMP_FILTER_PTP_V2_EVENT,
	/* PTP v2/802.AS1, any layer, Sync packet */
	HWTSTAMP_FILTER_PTP_V2_SYNC,
	/* PTP v2/802.AS1, any layer, Delay_req packet */
	HWTSTAMP_FILTER_PTP_V2_DELAY_REQ,

	/* NTP, UDP, all versions and packet modes */
	HWTSTAMP_FILTER_NTP_ALL,

	/* add new constants above here */
	__HWTSTAMP_FILTER_CNT
};

-----------------------------------------------------------------------------------------------
/ include / linux / netdevice.h

struct net_device_ops {}中有
int			(*ndo_eth_ioctl)(struct net_device *dev,
						 struct ifreq *ifr, int cmd);
可以設定network device用ioctl方式
其中有關於PTP的flag有
/* hardware time stamping: parameters in linux/net_tstamp.h */
#define SIOCSHWTSTAMP	0x89b0		/* set and get config		*/
#define SIOCGHWTSTAMP	0x89b1		/* get config			*/

driver_ddev2ops.c中dm9051_netdev_ioctl()即用此種方式去call到dm9051_ptp.c有關設定tx_type/rx_filter!

---------------------------------------------------------------------------------------------------
ethtool當中需要加入的function, 可以使用ethtool -T ethx 確認PTP的tx_type/rx_filters/so_timestamping/phc_index
const struct ethtool_ops {}中 .get_ts_info去實現

