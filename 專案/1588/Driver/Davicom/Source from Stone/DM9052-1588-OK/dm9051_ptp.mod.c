#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x92997ed8, "_printk" },
	{ 0x154aa398, "_dev_info" },
	{ 0x5f754e5a, "memset" },
	{ 0x828ce6bb, "mutex_lock" },
	{ 0x87c213a1, "spi_sync" },
	{ 0x9618ede0, "mutex_unlock" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xccd5a830, "spi_write_then_read" },
	{ 0x41453899, "_dev_err" },
	{ 0x57288b5f, "mii_ethtool_set_link_ksettings" },
	{ 0xc5a3eac, "mii_ethtool_get_link_ksettings" },
	{ 0xd5328002, "mii_link_ok" },
	{ 0x9f452270, "mii_nway_restart" },
	{ 0xeea0399, "strscpy" },
	{ 0x5314bbee, "of_find_compatible_node" },
	{ 0xc09e10da, "__spi_register_driver" },
	{ 0xeacedcb4, "driver_unregister" },
	{ 0x55e77c6a, "ptp_clock_index" },
	{ 0xf7802486, "__aeabi_uidivmod" },
	{ 0xff178f6, "__aeabi_idivmod" },
	{ 0xc358aaf8, "snprintf" },
	{ 0x1852d57b, "skb_queue_tail" },
	{ 0x2d3385d3, "system_wq" },
	{ 0xb2d48a2e, "queue_work_on" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0x8c731149, "skb_tstamp_tx" },
	{ 0xc13cfb84, "mii_check_link" },
	{ 0xcf86cdac, "queue_delayed_work_on" },
	{ 0x9d669763, "memcpy" },
	{ 0x8e865d3c, "arm_delay_ops" },
	{ 0xa6a7a2ad, "div_s64_rem" },
	{ 0x29d9f26e, "cancel_delayed_work_sync" },
	{ 0x92b57248, "flush_work" },
	{ 0x96207d26, "skb_queue_purge" },
	{ 0x80c4c319, "crc32_le" },
	{ 0x71c90087, "memcmp" },
	{ 0x57999cfe, "netdev_printk" },
	{ 0x772d3dbe, "ptp_clock_unregister" },
	{ 0xec153eb9, "eth_prepare_mac_addr_change" },
	{ 0x9ca67b71, "eth_commit_mac_addr_change" },
	{ 0xf9a482f9, "msleep" },
	{ 0x1f731d98, "netif_tx_wake_queue" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0xaa9ddef6, "consume_skb" },
	{ 0x1ba328c8, "skb_dequeue" },
	{ 0x27f99537, "netdev_err" },
	{ 0xf090eaa1, "eth_type_trans" },
	{ 0x519f980, "netif_rx" },
	{ 0xb7c5a08c, "__netdev_alloc_skb" },
	{ 0xfed1d72f, "skb_put" },
	{ 0x328a05f1, "strncpy" },
	{ 0x110eb7b0, "ptp_clock_register" },
	{ 0xd11ef13c, "devm_alloc_etherdev_mqs" },
	{ 0xde4bf88b, "__mutex_init" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x3e82f3e9, "get_random_bytes" },
	{ 0xebd8d44b, "dev_addr_mod" },
	{ 0xa05450f0, "_dev_printk" },
	{ 0x328052bb, "devm_register_netdev" },
	{ 0xdd3ba0bf, "dev_err_probe" },
	{ 0xd40caad0, "eth_validate_addr" },
	{ 0xc84d16dc, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D4E46ECACAF934C599B7F88");
