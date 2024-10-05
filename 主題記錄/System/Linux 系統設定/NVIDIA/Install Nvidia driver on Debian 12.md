剛裝完新的 Debian 12 如果直接安裝 nvidia-driver 會報許多相依無法滿足的問題，無法順利安裝。

Nvida propietary driver 需要 apt source list 手動加入 contrib:
```
deb http://deb.debian.org/debian/ bookworm main contrib non-free non-free-firmware
deb-src http://deb.debian.org/debian/ bookworm main contrib non-free non-free-firmware
```

然後 kernel module 要能夠 build:

`sudo apt install linux-headers-$(uname -r)`

應該就可以順利安裝

`sudo apt install nvidia-driver`
