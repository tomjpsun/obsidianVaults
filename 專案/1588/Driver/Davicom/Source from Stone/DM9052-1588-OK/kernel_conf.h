//[1.kernel configurations], [Mandatory: for diff kernels' selection]

#define DM9051_VINF           kernel_conf.dm9051kc[DAVICOM_CONF].verify_info
#define BOARD_KT              kernel_conf.dm9051kc[BOARD_CONF].version
#define DM9051_KT             kernel_conf.dm9051kc[DAVICOM_CONF].version
#define DM9051_DRV_NAME       kernel_conf.driver_name //(e.g. "lnx_dm9051 driver")
#define DM9051_DRC            kernel_conf.driver_release_candidate //(e.g. "r2306_rc26.next.ss6")
#define LEGEND_K(a)          (DAVICOM_CONF <= a) //enum new to old
#define FIT_K(a)             (DAVICOM_CONF >= a) //enum old to new

/*
 * In some cases, linux kernel discard certain APIs,
 * and use newer design functions'-name instead.
 * BOARD CONFIGURE: Your H/W
 *
 * Usually define to be equal to BOARD CONFIGURE,
 * But it is also ok to be elder kernel's version
 * since they can be used also good in functions's
 * perform. 
 * DAVICOM CONFIGURE: Your S/W
 */
#define DM9051_KERNEL_3_18      0
#define DM9051_KERNEL_4_1       1
#define DM9051_KERNEL_4_9       2
#define DM9051_KERNEL_4_19      3

#define DM9051_KERNEL_5_4       4
#define DM9051_KERNEL_5_10      5
#define DM9051_KERNEL_5_15      6
#define DM9051_KERNEL_NUM (7)
//#define DM9051_KERNEL_NUM (3)

static const struct {
    char        	*driver_name;
    char        	*driver_release_candidate;
    struct dm9051_kc dm9051kc[DM9051_KERNEL_NUM];
} kernel_conf = {
    "lnx_dm9051 driver",
    "r2307_rc26.next.ss8.7",
    {
        { DM9051_KERNEL_3_18,   "KT3.18",   "to be verify one-day"},
        { DM9051_KERNEL_4_1,    "KT4.1",    "test to rpi2 bcm2836"},
        { DM9051_KERNEL_4_9,    "KT4.9",    "test to microchip ksz9477"},
        { DM9051_KERNEL_4_19,   "KT4.19",   "used for custom mt8766"},

        { DM9051_KERNEL_5_4,    "KT5.4",    "to be verify one-day"},
        { DM9051_KERNEL_5_10,   "KT5.10",   "test to rpi2 bcm2836"},
        { DM9051_KERNEL_5_15,   "KT5.15",   "test to rpi4 bcm2711"},
    },
};

//#define BOARD_CONF            	DM9051_KERNEL_5_15
//#define DAVICOM_CONF            DM9051_KERNEL_5_15

#define BOARD_CONF            	DM9051_KERNEL_5_15
#define DAVICOM_CONF            DM9051_KERNEL_5_15


/* Driver code compiler oriented configuration-
 */

#define CODE_SUPPORT_REGMAP					0	// default use the morden program code
#define CODE_SUPPORT_REGMISC				1	// default regmap misc usage source code (reserved)
#define CODE_SUPPORT_MISC					1	// default misc usage source code (reserved)

#if DAVICOM_CONF < DM9051_KERNEL_4_19
#undef CODE_SUPPORT_REGMAP
#define CODE_SUPPORT_REGMAP					0	// in case "KT4.1", "test to rpi2 bcm2836" which is no regmap in that linux
#endif
			   
#if CODE_SUPPORT_REGMAP
static int dm9051_map_init1(struct spi_device *spi, struct board_info *db);
static int dm9051_dumpblk1(struct board_info *db, u8 reg, size_t count);
static int dm9051_write_mem1(struct board_info *db, unsigned int reg, const void *buff,
			    size_t len);
static int dm9051_read_mem1(struct board_info *db, unsigned int reg, void *buff,
			   size_t len);

static int _dm9051_get_reg1(struct board_info *db, unsigned int reg, void *val);
static int dm9051_set_reg1(struct board_info *db, unsigned int reg, unsigned int val);
#endif

static int dm9051_map_init0(struct spi_device *spi, struct board_info *db);
static int dm9051_dumpblk0(struct board_info *db, u8 reg, size_t count);
static int dm9051_write_mem0(struct board_info *db, unsigned int reg, const void *buff,
			    size_t len);
static int dm9051_read_mem0(struct board_info *db, unsigned int reg, void *buff,
			   size_t len);

static int _dm9051_get_reg0(struct board_info *db, unsigned int reg, void *val) ;
static int dm9051_set_reg0(struct board_info *db, unsigned int reg, unsigned int val);
 
#if CODE_SUPPORT_REGMAP
//regmap
#define RAW_REGMAP	BIT(0)
static const struct dm9051_raw_spi on_regmap = {
	.xfer = RAW_REGMAP,
	.dm9051_map_init = dm9051_map_init1,
	.dm9051_dumpblk = dm9051_dumpblk1,
	.dm9051_write_mem = dm9051_write_mem1,
	.dm9051_read_mem = dm9051_read_mem1,
	
	._dm9051_get_reg = _dm9051_get_reg1,
	.dm9051_set_reg = dm9051_set_reg1,
};
#endif

//spisyn_tradition
#define RAW_SPISYN	BIT(1)
static const struct dm9051_raw_spi on_spisyn = {
	.xfer = RAW_SPISYN,
	.dm9051_map_init = dm9051_map_init0,
	.dm9051_dumpblk = dm9051_dumpblk0,
	.dm9051_write_mem = dm9051_write_mem0,
	.dm9051_read_mem = dm9051_read_mem0,
	
	._dm9051_get_reg = _dm9051_get_reg0,
	.dm9051_set_reg = dm9051_set_reg0,
};

//spisyn_burst
#define RAW_SPISYNB	BIT(2)  
static const struct dm9051_raw_spi on_spisynb = {
	.xfer = RAW_SPISYNB,
	.dm9051_map_init = dm9051_map_init0,
	.dm9051_dumpblk = dm9051_dumpblk0,
	.dm9051_write_mem = dm9051_write_mem0,
	.dm9051_read_mem = dm9051_read_mem0,
	
	._dm9051_get_reg = _dm9051_get_reg0,
	.dm9051_set_reg = dm9051_set_reg0,
};

/* Selection
 */
#define xfer				regmapsel.sel->xfer
#define dm9051_map_init		regmapsel.sel->dm9051_map_init
#define dm9051_dumpblk		regmapsel.sel->dm9051_dumpblk
#define dm9051_write_mem	regmapsel.sel->dm9051_write_mem
#define dm9051_read_mem		regmapsel.sel->dm9051_read_mem
#define dm9051_get_reg		regmapsel.sel->_dm9051_get_reg
#define dm9051_set_reg		regmapsel.sel->dm9051_set_reg

static const struct dm9051_raw_spisel regmapsel = {
	//(1) .sel = &on_regmap, [Only avail, #if CODE_SUPPORT_REGMAP, #endif]
	//(2) .sel = &on_spisyn,
	//(3) .sel = &on_spisynb,
	
	//.sel = on_spisyn,
	//=
	//to
	.sel = &on_spisyn,

	/*.sel.xfer 			= on_spisyn.xfer,
	.sel.dm9051_map_init 	= on_spisyn.dm9051_map_init,
	.sel.dm9051_dumpblk 	= on_spisyn.dm9051_dumpblk,
	.sel.dm9051_write_mem	= on_spisyn.dm9051_write_mem,
	.sel.dm9051_read_mem 	= on_spisyn.dm9051_read_mem,
	
	.sel._dm9051_get_reg 	= on_spisyn._dm9051_get_reg,
	.sel.dm9051_set_reg 	= on_spisyn.dm9051_set_reg,*/
	
	/*.sel = {
		.xfer				= on_spisyn.xfer,
		.dm9051_map_init 	= on_spisyn.dm9051_map_init,
		.dm9051_dumpblk 	= on_spisyn.dm9051_dumpblk,
		.dm9051_write_mem	= on_spisyn.dm9051_write_mem,
		.dm9051_read_mem 	= on_spisyn.dm9051_read_mem,
	
		._dm9051_get_reg 	= on_spisyn._dm9051_get_reg,
		.dm9051_set_reg 	= on_spisyn.dm9051_set_reg,
	},*/
};

/* Driver code compiler oriented configuration-
 */

#define CODE_SUPPORT_PHYLIB				1 //default: 1	// default phylib usage source code, 0 when KT4.1.11
#define CODE_SUPPORT_PHYMII				1	// default mii usage source code
#define CODE_SUPPORT_MDIO_INTERNAL		1	// default phy misc usage source code (reserved)

#if DAVICOM_CONF <= DM9051_KERNEL_4_19
#undef CODE_SUPPORT_PHYLIB
#define CODE_SUPPORT_PHYLIB				0	//custom "KT4.19", "used for custom mt8766" who had no phylib
#endif

#if DAVICOM_CONF <= DM9051_KERNEL_4_1   /* Or #if 1  ;force to not support  */
#undef CODE_SUPPORT_PHYMII
#define CODE_SUPPORT_PHYMII				0	//in case "KT4.1", "test to rpi2 bcm2836" which is no mii.ko installed
#endif

/* Note:
 * Note: KT_4_1_11 has mdiobus_alloc_size(), and phy_connect() APIs support,
 *       So we did compile succeed.
 *       How ever my rpi2 bcm2836 KT4.1.11 had no 'phy_device.c' added in the system,
 *       So I can not use by define MOD_PHYLIB 1 (= PHYCFG_LIB), 
 *       How ever my rpi2 bam2836 kt4.1.11 had no mii.ko installed,
 *       So I can not use MOD_OBJ_MIIExtMII 1 (= PHYCFG_MII).
 *       In this case, We use MOD_PHYLIB 0 && MOD_OBJ_MIIExtMII 0,
 *       Then it is (= PHYCFG_MDIO)
 */

/* Selection
 */

#define PHYCFG_LIB	BIT(0)  //phylib
#define PHYCFG_MII	BIT(1)  //mii
#define PHYCFG_MDIO	BIT(2)  //internal
 
static const struct dm9051_phy phycfg = {
	//(1) .setup_phy = PHYCFG_LIB,
	//(2) .setup_phy = PHYCFG_MII,
	//(3) .setup_phy = PHYCFG_MDIO,
	.setup_phy = PHYCFG_MII,//PHYCFG_LIB,
};

#define ETHn_NDEVOPS_FIXED
