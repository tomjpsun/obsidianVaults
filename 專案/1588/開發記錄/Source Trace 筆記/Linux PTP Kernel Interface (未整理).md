# Kernel 裏面關於 Linux PTP 的 directory

`drivers/ptp/ `- This directory contains the driver implementations for PTP. It includes the following files:
Multiple driver files such as `ptp_clock.c`, `ptp_chardev.c`, `ptp_sysfs.c`, and others related to different PTP devices and implementations.

`include/linux/ptp_clock.h` - This header file contains the definitions and structures for the PTP clock interface in Linux.

`tools/testing/selftests/ptp/` - This directory is likely for testing PTP implementations, though I haven't retrieved its exact content.

# 列出 Kernel 裏面 相關 ETHTOOL_GET_TS_INFO 的部分

### In net/ethtool/ioctl.c:

A function called ethtool_get_ts_info is responsible for handling timestamping information.

```c
static int ethtool_get_ts_info(struct net_device *dev, void __user *useraddr) {
    struct kernel_ethtool_ts_info kernel_info;
    struct ethtool_ts_info info = {};
```

### In net/ethtool/common.h:

There is a declaration for the function \_\_ethtool_get_ts_info.

```c
int __ethtool_get_ts_info(struct net_device *dev, struct kernel_ethtool_ts_info *info);
```

### In net/ethtool/common.c:

The \_\_ethtool_get_ts_info function sets up the command number to ETHTOOL_GET_TS_INFO.

```c
int __ethtool_get_ts_info(struct net_device *dev, struct kernel_ethtool_ts_info *info) {
    memset(info, 0, sizeof(*info));
    info->cmd = ETHTOOL_GET_TS_INFO;
    info->phc_index = -1;
```

In include/uapi/linux/ethtool.h:

Defines ETHTOOL_GET_TS_INFO as a command for retrieving timestamping and PHC (Precision Hardware Clock) information.

```c
#define ETHTOOL_GET_TS_INFO 0x00000041 /* Get time stamping and PHC info */

```

In Documentation/networking/ethtool-netlink.rst:

The documentation mentions the ETHTOOL_GET_TS_INFO ioctl request.

```
Gets timestamping information like ``ETHTOOL_GET_TS_INFO`` ioctl request.
```

These findings show that ETHTOOL_GET_TS_INFO is primarily used for retrieving timestamping information and Precision Hardware Clock (PHC) association in network devices.