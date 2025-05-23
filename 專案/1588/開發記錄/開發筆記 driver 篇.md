# build IGB driver with Emacs LSP support:
```bash
cd <Intel IGB driver source>
CC=clang LLVM=1 bear -- make
```
# build LinuxPTP with Emacs LSP support:
```bash
cd <LinuxPTP source>
CC=clang LLVM=1 bear -- make
```


# DHCP list


![[Pasted image 20241107171036.png]]
# nmcli 指令範例

### show

```bash
$ nmcli con show
NAME                UUID                                  TYPE      DEVICE
Wired connection 2  60dece34-a3da-3e06-ad10-9ff34f495087  ethernet  eth1
lo                  2fd164b3-c657-4287-b6b6-bd4de15f6e39  loopback  lo
Shader              3b9ea857-db27-461f-84a8-d0d45ffef661  wifi      wlan0
Wired connection 1  556d2b8f-7143-35ad-874c-469f7c649424  ethernet  --
```

### Link up { by conn name }
```bash
$ sudo nmcli connection up Wired\ connection\ 2
```
### Link up { by device }
```
sudo nmcli device
```

```
DEVICE     TYPE      STATE                   CONNECTION
eth1           ethernet  connected               Wired connection 2
lo             loopback  connected (externally)  lo
wlan0          wifi      connected               Shader
p2p-dev-wlan0  wifi-p2p  disconnected            --
eth0           ethernet  unavailable             --
```

```
sudo nmcli device connect eth1
```
### 移除 driver 

```
sudo nmcli device down eth1
sudo rmmod dm9051
```

### 安裝 driver 

```
sudo make install
sudo nmcli device up eth1

# Emacs LSP support on RaspBPi

### 安裝 clangd ARM 版本
```bash
sudo apt install clangd lld
```
### init.el 加上
```bash
(setq lsp-clients-clangd-executable "/usr/bin/clangd")
```

### generate compile_commands.json

```bash
cd <Davicom driver source>
# 更正, 進入 kernel 的 make 系統, 不會使用 clang, 所以 CC=clang 是無效的.
# 用 bear 產生 compile_commands.json 就可以, compiler 不一定要是 clang
# CC=clang LLVM=1 bear -- make
bear -- make
```

## lsp 抱怨沒有 AST

從 `*clangd::stderr*` 裏面看到是因爲下面錯誤:
`clangd:Couldn‘t build compiler instance`
這是因爲 clang 不支援 API 'lp64'
導致後續 lsp 抱怨沒有 AST, 然後無法工作

解決辦法, 在 source 目錄下,新增一個檔案 : `.clangd` 內容是:

```bash
CompileFlags:
  Remove: -mabi=lp64
```

這樣 emacs lsp-mode 就可以順利啓動了
# Github Repo Analyzer Key

LangChain tool: https://github.com/jitinchekka/github-repo-analyzer

> [!NOTE]
> > 心得: 一個很快就消耗 OpenAI quota 的東西, 不好用

```
GITHUB_TOKEN="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"
OPENAI_API_KEY="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"
ACTIVELOOP_TOKEN="eyJhbGciOiJub25lIiwidHlwIjoiSldUIn0.eyJpZCI6InN3ZWV0aG9tZTAyMTIyMyIsImFwaV9rZXkiOiJzVk5TZE5fUjE5NnhWVk9sLTNQeHhrOEd2cDhxcURFbnNxVVBEdHRZcXlTWkoifQ."
```

> [!NOTE] Token Key 的關聯帳號
>GitHub: tomjpsun
>OpenAI: sweethome021223
>ActiveLoop: sweethome021223


> [!NOTE] 沒有依照操作, 容易出現下面現象:
> [  185.153053] dm9051 spi0.1 eth1: Link is Up - 100Mbps/Full - flow control rx/tx
[  191.464947] dm9051_irq_flag(db) 8
[  191.464976] Set Reg, IRQF_TRIGGER_LOW 8, IRQF_TRIGGER_HIGH 4
[  194.713079] dm9051_irq_flag(db) 8
[  194.713107] Set Reg, IRQF_TRIGGER_LOW 8, IRQF_TRIGGER_HIGH 4
[  214.297602] dm9051_irq_flag(db) 8
[  214.297617] Set Reg, IRQF_TRIGGER_LOW 8, IRQF_TRIGGER_HIGH 4
[  221.465857] dm9051_irq_flag(db) 8
[  221.465883] Set Reg, IRQF_TRIGGER_LOW 8, IRQF_TRIGGER_HIGH 4
[  251.470378] dm9051_irq_flag(db) 8
[  251.470410] Set Reg, IRQF_TRIGGER_LOW 8, IRQF_TRIGGER_HIGH 4
[  281.462749] dm9051_irq_flag(db) 8
[  281.462780] Set Reg, IRQF_TRIGGER_LOW 8, IRQF_TRIGGER_HIGH 4
[  311.471052] dm9051_irq_flag(db) 8
[  311.471081] Set Reg, IRQF_TRIGGER_LOW 8, IRQF_TRIGGER_HIGH 4
[  319.731133] dm9051_irq_flag(db) 8
[  319.731162] Set Reg, IRQF_TRIGGER_LOW 8, IRQF_TRIGGER_HIGH 4

### Change debug level
```
cat /proc/sys/kernel/printk
3       4       1       3
```
以我們的 driver 爲例, debug level 是 6:

```                                       
#define DM_DEBUG_LEVEL KERN_INFO   // (6)  
```

如果要顯示, 就將系統 debug level 設定到 6 以上:

```
echo 6 >/proc/sys/kernel/printk
```


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

# kernel ptp_clock_adjtime(... struct __kernel_timex \*tx)
在 kernel source/drivers/ptp/ptp_clock.c :: ptp_clock_adjtime()

判斷 tx->modex

| modes         |                              |     |
| ------------- | ---------------------------- | --- |
| ADJ_SETOFFSET | ops->adjtime(ops, delta);    |     |
| ADJ_FREQUENCY | ops->adjfine(ops, tx->freq); |     |
| ADJ_OFFSET    | ops->adjphase(ops, offset);  |     |
前兩項就會分別呼叫 network driver 註冊的 ptp_clock 裏面的 adjtime(), adjfine()

# ptp4l packet length

Log from ==wireshark== :
"No.","Time","Source","Destination","Protocol","Length","Info"
"53","12.000658758","192.168.193.92","224.0.1.129","PTPv2","86","Sync Message"
"54","12.000898825","192.168.193.92","224.0.1.129","PTPv2","86","Follow_Up Message"
"55","12.065159841","192.168.193.146","224.0.1.129","PTPv2","86","Delay_Req Message"
"56","12.065364230","192.168.193.92","224.0.1.129","PTPv2","96","Delay_Resp Message"

| Len | Info       | skb->data\[42] |
| --- | ---------- | -------------- |
| 86  | SYNC       | 0x00           |
| 86  | FOLLOW_UP  | 0x08           |
| 86  | DELAY_REQ  | 0x01           |
| 96  | DELAY_RESP | 0x09           |

## 如何知道 driver 目前是 /dev/ptpx ?

ethtool -T eth1
```
tom@rpi4:~ $ ethtool -T eth1
Time stamping parameters for eth1:
Capabilities:
	hardware-transmit
	software-transmit
	hardware-receive
	software-receive
	software-system-clock
	hardware-raw-clock
PTP Hardware Clock: 0
Hardware Transmit Timestamp Modes:
	off
	on
	onestep-sync
Hardware Receive Filter Modes:
	none
	all
```

PTP Hardware Clock: 0 表示目前是 /dev/ptp0

## 如何讓 ptp_9051_gettime() / ptp_9051_settime() 動作?

```c

//用 gettime() 讀取:
tom@rpi4:~ $ sudo phc_ctl /dev/ptp0 get
phc_ctl[2325.668]: clock time is 1745574025.738807442 or Fri Apr 25 17:40:25 2025

// 設定系統時間到 chip
tom@rpi4:~ $ sudo phc_ctl /dev/ptp0 set
phc_ctl[2374.565]: set clock time to 1745574075.027246251 or Fri Apr 25 17:41:15 2025

// 設定 1970-1-1 08:00:00 到 chip
tom@rpi4:~ $ sudo phc_ctl /dev/ptp0 set 0
phc_ctl[2400.900]: set clock time to 0.000000000 or Thu Jan  1 08:00:00 1970

tom@rpi4:~ $ sudo phc_ctl /dev/ptp0 get
phc_ctl[2404.493]: clock time is 3.592358487 or Thu Jan  1 08:00:03 1970

// 還原系統時間, set 不加參數即可
tom@rpi4:~ $ sudo phc_ctl /dev/ptp0 set
phc_ctl[2534.180]: set clock time to 1745574234.643028445 or Fri Apr 25 17:43:54 2025

// 確定完成
tom@rpi4:~ $ sudo phc_ctl /dev/ptp0 get
phc_ctl[2537.885]: clock time is 1745574238.347172279 or Fri Apr 25 17:43:58 2025

```
# PMC
sudo ./ptp_collector -u -f ./configs/ptp_collector.json

sudo pmc -u -i eth1 'GET TIME_STATUS_NP'

# ptp_collector
ptp_collector 可以使用 （確認 pmc 打開 port 是 /var/run/ptp4l 即可）
`sudo ./ptp_collector -u -f ./configs/ptp_collector.json`
然後 ptp_collector.json 內容，原本 interface_name 拿掉， 有 uds_address 即可。如下：
```
   "ptp": {                                                                    
        "config_path": "/home/tom/work/richard_ptp/linuxptp/configs/ptp4l_slave.conf",    
        "uds_address": "/var/run/ptp4l",                                       
        "remote_address": "",                                                  
        "transport_type": "UDS",                                               
        "boundary_hops": 1,                                                    
        "domain_number": 0,                                                    
        "transport_specific": 0,                                               
        "allow_unauth": 0                                                      
    },     
```                      
