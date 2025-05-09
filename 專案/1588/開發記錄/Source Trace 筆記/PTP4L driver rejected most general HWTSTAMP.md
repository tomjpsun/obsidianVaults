執行時出現 
ptp4l[182981.130]: selected /dev/ptp0 as PTP clock               
==ptp4l[182981.131]: driver rejected most general HWTSTAMP filter==
ptp4l[182981.136]: port 1 (eth1): INITIALIZING to LISTENING on 


INIT_COMPLETE       
in LinuxPTP  sk.c:: hwts_init() 有一段是這樣:
```c
	case HWTS_FILTER_NORMAL:
		cfg.tx_type   = tx_type;
		cfg.rx_filter = orig_rx_filter = rx_filter;
		err = ioctl(fd, SIOCSHWTSTAMP, &ifreq);
		if (err < 0) {
			pr_info("driver rejected most general HWTSTAMP filter");

			init_ifreq(&ifreq, &cfg, device);
			cfg.tx_type   = tx_type;
			cfg.rx_filter = orig_rx_filter = rx_filter2;

			err = ioctl(fd, SIOCSHWTSTAMP, &ifreq);
			if (err < 0) {
				pr_err("ioctl SIOCSHWTSTAMP failed: %m");
				return err;
			}
		}
```

原因是 driver 的 dm9051_netdev_ioctl( *SIOCSHWTSTAMP*)  
-> dm9051_ptp_set_ts_config()    
-> dm9051_ptp_set_timestamp_mode() 傳回    -22 (-EINVAL)    

``` c
/* reserved for future extensions */
        if (config->flags)            
                return -EINVAL;    
```

