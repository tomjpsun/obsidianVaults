
要使支援 IEEE-1588 的網路卡與 Linux 核心時鐘同步，通常需要以下步驟：

1. **安裝 PTP 工具和驅動程式**： 首先，確保系統上安裝了 'linuxptp' 軟體包，它提供了 'ptp4l' 和 'phc2sys' 等工具，用於配置和管理 PTP。
    
在 Debian/Ubuntu 上，可以使用以下命令安裝：
    
`sudo apt-get install linuxptp`
    
在 CentOS/RHEL 上，可以使用以下命令安裝：
    
`sudo yum install linuxptp`
    
2. **配置網路卡**： 確保網路卡的驅動程序支援 PTP。 一般來說，Intel 的 ixgbe 驅動程式和 Mellanox 的 mlx5 驅動程式都支援 PTP。
    
檢查網路卡是否支援 PTP，可以使用以下命令：
    
`ethtool -T eth0`
    
如果輸出顯示支援 『hardware-software』 或 'software'， 說明網络卡支援 PTP。
    
3. **啟動 PTP 服務**： 使用 'ptp4l' 工具來同步網路卡的 PHC （PTP Hardware Clock） 與 PTP 網络中的主時鐘。
    
創建一個 'ptp4l' 配置檔，例如 '/etc/linuxptp/ptp4l.conf'：
    
``` bash
[global]
twoStepFlag 1
txTimestampTimeout 20
[eth0]
```
    
然後啟動 'ptp4l'：
       
sudo ptp4l -f /etc/linuxptp/ptp4l.conf -i eth0 -m
    
4. **同步 PHC 與系統時鐘**： 使用 'phc2sys' 工具將網络卡的 PHC 同步到系統時鐘。
    
創建一個 'phc2sys' 配置檔，例如 '/etc/linuxptp/phc2sys.conf'：
    
```bash
[global]
syncInterval -4
[eth0]
```
    
然後啟動 'phc2sys'：

```bash
   sudo phc2sys -s eth0 -w -m
```

5. **驗證同步狀態**： 檢查 'ptp4l' 和 'phc2sys' 的日誌輸出，以確保同步正在進行且沒有錯誤。
    
可以透過以下命令檢視系統時鐘的狀態：
    
`chronyc tracking`
    
或者使用 'timedatectl' 命令：
    
`timedatectl status`
    
這樣，支持 IEEE-1588 的網路卡就能與 Linux 核心時鐘同步。 確保 PTP 網路中的所有設備都正確配置並且時鐘同步正常。