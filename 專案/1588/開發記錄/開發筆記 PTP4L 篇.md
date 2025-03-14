## pmc 命令
在 pmc_common.c 裏面,有許多 do_get_action() 

```
sudo pmc -u -b 1 'GET USER_DESCRIPTION'
```

## linuxPTP 使用到什麼 ethtool 的界面?

在 **Linux PTP**（例如使用 `ptp4l`）與網卡溝通、偵測硬體時間戳（Hardware Timestamp）功能的過程中，  
**實際上最主要使用的是內核驅動裡的 ethtool 相關介面**，也就是以下這些機制：

1. **ETHTOOL_GET_TS_INFO**（或在程式碼中常看到 `ETHTOOL_GTSINFO`）：
    
    - 這對應到驅動程式的 `ethtool_ops->get_ts_info` 回呼函式。
    - 用途是向網路卡驅動詢問「此裝置支援哪些時間戳模式（硬體／軟體、RX／TX 等）？是否支援 PTP？
    - 如果你在使用者層面直接下 `ethtool -T <介面名稱>`（例如 `ethtool -T eth0`），就會呼叫到這個介面，並列出該網卡對時間戳的支援狀況。
2. **SIOCSHWTSTAMP / SIOCGHWTSTAMP**：
    
    - 這是在使用者空間（例如 `ptp4l`）用 `ioctl` 方式，對網路介面下達「設定」或「查詢」硬體時間戳功能的指令。
    - 例如，`ptp4l` 會透過 `struct ifreq` 和 `struct hwtstamp_config` 來呼叫 `ioctl`，告訴驅動要啟用哪種硬體時間戳模式、收包／送包或兩者都要打時間戳、以及只針對特定封包類型（PTP 封包）或所有封包都打時間戳等等。

因此，如果單純從「ethtool 介面」角度來看，**Linux PTP 主要就是依賴驅動程式實作的** `ethtool_ops->get_ts_info`（對應 `ETHTOOL_GET_TS_INFO` 指令）來判斷網卡對 PTP／硬體時間戳的支援度；而實際啟動或調整硬體時間戳功能，則是靠 `ioctl(SIOCSHWTSTAMP / SIOCGHWTSTAMP)`。

## PTP4L clock_synnchronize()
使用[[Print call stack in C]] 可以在 clock_synchronize() 印出 call stack, 此時 PTP4L 的輸出如下:
```c
/ssh:tom@pi488:/home/tom/work/linuxptp $ sudo ptp4l -m -H -i eth1 -s
ptp4l[9535.712]: selected /dev/ptp0 as PTP clock
ptp4l[9535.714]: port 1 (eth1): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[9535.714]: port 0 (/var/run/ptp4l): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[9535.714]: port 0 (/var/run/ptp4lro): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[9536.453]: port 1 (eth1): new foreign master c0a2b6.fffe.a61312-1
ptp4l[9540.853]: selected best master clock c0a2b6.fffe.a61312
ptp4l[9540.853]: port 1 (eth1): LISTENING to UNCALIBRATED on RS_SLAVE
ptp4l[9542.583]: clock_synchronize+0x37c [0x5570e79e90]
ptp4l[9542.584]: port_synchronize+0x1b0 [0x5570e88af0]
ptp4l[9542.584]: port_syfufsm+0x224 [0x5570e88f3c]
ptp4l[9542.585]: process_follow_up+0xe8 [0x5570e8b8e8]
ptp4l[9542.585]: bc_event+0xb08 [0x5570e8de54]
ptp4l[9542.585]: port_event+0x24 [0x5570e8d344]
ptp4l[9542.586]: clock_poll+0x198 [0x5570e794c8]
ptp4l[9542.586]: main+0x81c [0x5570e73ff0]
ptp4l[9542.586]: __libc_init_first+0x80 [0x7f95ac7740]
ptp4l[9542.587]: __libc_start_main+0x98 [0x7f95ac7818]
ptp4l[9542.587]: _start+0x30 [0x5570e73670]
ptp4l[9542.587]: clock_synchronize(), master offset -434523474834872299 s0 freq -50000000 path delay 1305257522047863358
ptp4l[9543.683]: clock_synchronize+0x37c [0x5570e79e90]
ptp4l[9543.683]: port_synchronize+0x1b0 [0x5570e88af0]
ptp4l[9543.684]: port_syfufsm+0x224 [0x5570e88f3c]
ptp4l[9543.684]: process_follow_up+0xe8 [0x5570e8b8e8]
ptp4l[9543.684]: bc_event+0xb08 [0x5570e8de54]
ptp4l[9543.684]: port_event+0x24 [0x5570e8d344]
ptp4l[9543.684]: clock_poll+0x198 [0x5570e794c8]
ptp4l[9543.685]: main+0x81c [0x5570e73ff0]
ptp4l[9543.685]: __libc_init_first+0x80 [0x7f95ac7740]
ptp4l[9543.685]: __libc_start_main+0x98 [0x7f95ac7818]
ptp4l[9543.685]: _start+0x30 [0x5570e73670]
ptp4l[9543.685]: clock_synchronize(), master offset -434523474831285977 s1 freq -46247998 path delay 1305257522047863358
ptp4l[9544.783]: clock_synchronize+0x37c [0x5570e79e90]
ptp4l[9544.783]: port_synchronize+0x1b0 [0x5570e88af0]
ptp4l[9544.784]: port_syfufsm+0x224 [0x5570e88f3c]
ptp4l[9544.784]: process_follow_up+0xe8 [0x5570e8b8e8]
ptp4l[9544.784]: bc_event+0xb08 [0x5570e8de54]
ptp4l[9544.784]: port_event+0x24 [0x5570e8d344]
ptp4l[9544.784]: clock_poll+0x198 [0x5570e794c8]
ptp4l[9544.785]: main+0x81c [0x5570e73ff0]
ptp4l[9544.785]: __libc_init_first+0x80 [0x7f95ac7740]
ptp4l[9544.785]: __libc_start_main+0x98 [0x7f95ac7818]
ptp4l[9544.785]: _start+0x30 [0x5570e73670]
ptp4l[9544.785]: clock_synchronize(), master offset   21222222 s2 freq -25025776 path delay 1305257522047863358
ptp4l[9544.785]: port 1 (eth1): UNCALIBRATED to SLAVE on MASTER_CLOCK_SELECTED
ptp4l[9545.883]: clock_synchronize+0x37c [0x5570e79e90]
ptp4l[9545.884]: port_synchronize+0x1b0 [0x5570e88af0]
ptp4l[9545.884]: port_syfufsm+0x224 [0x5570e88f3c]
ptp4l[9545.884]: process_follow_up+0xe8 [0x5570e8b8e8]
ptp4l[9545.884]: bc_event+0xb08 [0x5570e8de54]
ptp4l[9545.884]: port_event+0x24 [0x5570e8d344]
ptp4l[9545.884]: clock_poll+0x198 [0x5570e794c8]
ptp4l[9545.885]: main+0x81c [0x5570e73ff0]
ptp4l[9545.885]: __libc_init_first+0x80 [0x7f95ac7740]
ptp4l[9545.885]: __libc_start_main+0x98 [0x7f95ac7818]
ptp4l[9545.885]: _start+0x30 [0x5570e73670]
ptp4l[9545.885]: clock_synchronize(), master offset   27003558 s2 freq -12877774 path delay 1305257522047863358
ptp4l[9546.983]: clock_synchronize+0x37c [0x5570e79e90]
ptp4l[9546.984]: port_synchronize+0x1b0 [0x5570e88af0]
ptp4l[9546.984]: port_syfufsm+0x224 [0x5570e88f3c]
ptp4l[9546.984]: process_follow_up+0xe8 [0x5570e8b8e8]
ptp4l[9546.985]: bc_event+0xb08 [0x5570e8de54]
ptp4l[9546.985]: port_event+0x24 [0x5570e8d344]
ptp4l[9546.985]: clock_poll+0x198 [0x5570e794c8]
ptp4l[9546.985]: main+0x81c [0x5570e73ff0]
ptp4l[9546.985]: __libc_init_first+0x80 [0x7f95ac7740]
ptp4l[9546.986]: __libc_start_main+0x98 [0x7f95ac7818]
ptp4l[9546.986]: _start+0x30 [0x5570e73670]
ptp4l[9546.986]: clock_synchronize(), master offset  -57717057 s2 freq -50000000 path delay 1305257522143270038
```

## PTP4L clock_synnchronize()
使用[[Print call stack in C]] 可以在 clock_synchronize() 印出 call stack, 此時 PTP4L 的輸出如下:
```c
/ssh:tom@pi488:/home/tom/work/linuxptp $ sudo ptp4l -m -H -i eth1 -s
ptp4l[9535.712]: selected /dev/ptp0 as PTP clock
ptp4l[9535.714]: port 1 (eth1): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[9535.714]: port 0 (/var/run/ptp4l): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[9535.714]: port 0 (/var/run/ptp4lro): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[9536.453]: port 1 (eth1): new foreign master c0a2b6.fffe.a61312-1
ptp4l[9540.853]: selected best master clock c0a2b6.fffe.a61312
ptp4l[9540.853]: port 1 (eth1): LISTENING to UNCALIBRATED on RS_SLAVE
ptp4l[9542.583]: clock_synchronize+0x37c [0x5570e79e90]
ptp4l[9542.584]: port_synchronize+0x1b0 [0x5570e88af0]
ptp4l[9542.584]: port_syfufsm+0x224 [0x5570e88f3c]
ptp4l[9542.585]: process_follow_up+0xe8 [0x5570e8b8e8]
ptp4l[9542.585]: bc_event+0xb08 [0x5570e8de54]
ptp4l[9542.585]: port_event+0x24 [0x5570e8d344]
ptp4l[9542.586]: clock_poll+0x198 [0x5570e794c8]
ptp4l[9542.586]: main+0x81c [0x5570e73ff0]
ptp4l[9542.586]: __libc_init_first+0x80 [0x7f95ac7740]
ptp4l[9542.587]: __libc_start_main+0x98 [0x7f95ac7818]
ptp4l[9542.587]: _start+0x30 [0x5570e73670]
ptp4l[9542.587]: clock_synchronize(), master offset -434523474834872299 s0 freq -50000000 path delay 1305257522047863358
ptp4l[9543.683]: clock_synchronize+0x37c [0x5570e79e90]
ptp4l[9543.683]: port_synchronize+0x1b0 [0x5570e88af0]
ptp4l[9543.684]: port_syfufsm+0x224 [0x5570e88f3c]
ptp4l[9543.684]: process_follow_up+0xe8 [0x5570e8b8e8]
ptp4l[9543.684]: bc_event+0xb08 [0x5570e8de54]
ptp4l[9543.684]: port_event+0x24 [0x5570e8d344]
ptp4l[9543.684]: clock_poll+0x198 [0x5570e794c8]
ptp4l[9543.685]: main+0x81c [0x5570e73ff0]
ptp4l[9543.685]: __libc_init_first+0x80 [0x7f95ac7740]
ptp4l[9543.685]: __libc_start_main+0x98 [0x7f95ac7818]
ptp4l[9543.685]: _start+0x30 [0x5570e73670]
ptp4l[9543.685]: clock_synchronize(), master offset -434523474831285977 s1 freq -46247998 path delay 1305257522047863358
ptp4l[9544.783]: clock_synchronize+0x37c [0x5570e79e90]
ptp4l[9544.783]: port_synchronize+0x1b0 [0x5570e88af0]
ptp4l[9544.784]: port_syfufsm+0x224 [0x5570e88f3c]
ptp4l[9544.784]: process_follow_up+0xe8 [0x5570e8b8e8]
ptp4l[9544.784]: bc_event+0xb08 [0x5570e8de54]
ptp4l[9544.784]: port_event+0x24 [0x5570e8d344]
ptp4l[9544.784]: clock_poll+0x198 [0x5570e794c8]
ptp4l[9544.785]: main+0x81c [0x5570e73ff0]
ptp4l[9544.785]: __libc_init_first+0x80 [0x7f95ac7740]
ptp4l[9544.785]: __libc_start_main+0x98 [0x7f95ac7818]
ptp4l[9544.785]: _start+0x30 [0x5570e73670]
ptp4l[9544.785]: clock_synchronize(), master offset   21222222 s2 freq -25025776 path delay 1305257522047863358
ptp4l[9544.785]: port 1 (eth1): UNCALIBRATED to SLAVE on MASTER_CLOCK_SELECTED
ptp4l[9545.883]: clock_synchronize+0x37c [0x5570e79e90]
ptp4l[9545.884]: port_synchronize+0x1b0 [0x5570e88af0]
ptp4l[9545.884]: port_syfufsm+0x224 [0x5570e88f3c]
ptp4l[9545.884]: process_follow_up+0xe8 [0x5570e8b8e8]
ptp4l[9545.884]: bc_event+0xb08 [0x5570e8de54]
ptp4l[9545.884]: port_event+0x24 [0x5570e8d344]
ptp4l[9545.884]: clock_poll+0x198 [0x5570e794c8]
ptp4l[9545.885]: main+0x81c [0x5570e73ff0]
ptp4l[9545.885]: __libc_init_first+0x80 [0x7f95ac7740]
ptp4l[9545.885]: __libc_start_main+0x98 [0x7f95ac7818]
ptp4l[9545.885]: _start+0x30 [0x5570e73670]
ptp4l[9545.885]: clock_synchronize(), master offset   27003558 s2 freq -12877774 path delay 1305257522047863358
ptp4l[9546.983]: clock_synchronize+0x37c [0x5570e79e90]
ptp4l[9546.984]: port_synchronize+0x1b0 [0x5570e88af0]
ptp4l[9546.984]: port_syfufsm+0x224 [0x5570e88f3c]
ptp4l[9546.984]: process_follow_up+0xe8 [0x5570e8b8e8]
ptp4l[9546.985]: bc_event+0xb08 [0x5570e8de54]
ptp4l[9546.985]: port_event+0x24 [0x5570e8d344]
ptp4l[9546.985]: clock_poll+0x198 [0x5570e794c8]
ptp4l[9546.985]: main+0x81c [0x5570e73ff0]
ptp4l[9546.985]: __libc_init_first+0x80 [0x7f95ac7740]
ptp4l[9546.986]: __libc_start_main+0x98 [0x7f95ac7818]
ptp4l[9546.986]: _start+0x30 [0x5570e73670]
ptp4l[9546.986]: clock_synchronize(), master offset  -57717057 s2 freq -50000000 path delay 1305257522143270038
```