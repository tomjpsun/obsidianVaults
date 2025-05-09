> [!NOTE] 
> **場景**
> Pi5 PTP Master --> Pi 4 PTP Slave --> Pi 4 System Time
> 
> **確認裝置**
> ethtool -T eth1 | grep 'PTP Hardware Clock
> PTP Hardware Clock: 1  --> 就是 /dev/ptp1
> PTP Hardware Clock: 0  --> 就是 /dev/ptp0`

## master
- Master hw clock 設定 : 
	先確認裝置 /dev/ptp? 假設得到 1
	`sudo phc_ctl /dev/ptp1 get`
	如果是 1970 年,就用 set 設定爲現在時間
	**這樣才不會把 1970 的時間給 slave**
	`sudo phc_ctl /dev/ptp1 set`
	再 get 回來看看是否是現在時間?
- system time --> dm9051a hw clock:
```c
sudo phc2sys -c CLOCK_REALTIME -s /dev/ptp1 -m -r
```
參數說明：
`-c CLOCK_REALTIME`：來源是系統時間
 `-s /dev/ptp1`：目標是網卡的 PHC 裝置（Intel NIC 通常是 `/dev/ptp0` 或 `/dev/ptp1`）
 `-m`：使用硬體 timestamp（需要支援）
 `-r`：run continuously（持續執行）
 
- 啓動 ptp4l Master:
	`sudo ptp4l -m -H -i eth1 -f one_step_master.cfg`
	[[#what is one-step-master.cfg ?]] 
## slave
- Slaver 的 ptp 裝置確認, 假設得到 1
- Slaver hw clock 設定 : 
	`sudo phc_ctl /dev/ptp1 get
	如果是 1970 年,就用 set 設定爲現在時間
	**這樣才不會超過 max offset 導致調不動
	`sudo phc_ctl /dev/ptp1 set￼￼
	再 get 回來看看是否是現在時間? 

-  disable 其他時間來源:
`ps -ef | grep -E 'ntpd|chronyd|timesyncd'

停止 systemd-timesyncd（Ubuntu/Debian 預設）：
sudo systemctl stop systemd-timesyncd
sudo systemctl disable systemd-timesyncd

另外 chronyd, ntpd 也是一樣要停止

-  啓動 ptp4l :
`sudo ptp4l -m -H -i eth1 -s`

- 最後才啓動 phc2sys， 從網卡 timestamp 同步到 system timer：
`sudo phc2sys -s eth1 -c CLOCK_REALTIME -m

如果需要減少 spi 的 path delay, 可以將 phc2sys 放在 core 1-4 上, 下面是放在 core-2 
`sudo taskset -c 2 phc2sys ....

採用設定檔:
`sudo phc2sys -s eth1 -c CLOCK_REALTIME -m -f ptp4l_configs/phc2sys_gentle_converge.conf `
[[#what is phc2sys_gentle_converge.conf ?]]
## what is one-step-master.cfg ?
`diff one_step_master.cfg ../linuxptp/configs/default.cfg   `
```c                            
5c5                                                                             
< twoStepFlag           0   
---             
> twoStepFlag           1                                                        
56c56                                                                            
< assume_two_step               1                                                
---                                                                              
> assume_two_step               0  
```       
## what is phc2sys_gentle_converge.conf ?
只有下面內容, 讓 Pi servo 溫和收斂
```c
[global]          
pi_proportional_const  0.1
pi_integral_const      0.0001   
```

