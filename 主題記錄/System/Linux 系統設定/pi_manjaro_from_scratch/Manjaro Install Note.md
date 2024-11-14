# Package Management

> [!NOTE] Note
>build package::file info 

sudo pacman -S pkgfile
sudo pkgfile -u
# search package which contains file
pkgfile file

# add admin user 
sudo useradd -m admin
sudo passwd admin
//45101524

# add to wheel group for sudo
sudo usermod --append --groups wheel admin

> [!NOTE] Note
> Re-login to take effect

# add to video group 
//can use vcgencmd utility 
sudo usermod -a -G video admin

# tftp server 

sudo pacman -S tftp-hpa
### /srv/tftp 權限要打開:
chmod 777 /srv/tftp

###  設定檔修改成 可以放檔案(--create)
emacs /etc/conf.d/tftpd
TFTPD_ARGS="--secure --create /srv/tftp/"

### 打開服務
sudo systemctl enable tftpd.service
sudo systemctl start tftpd.service

# Change Hostname 

Check hosts file:
	getent hosts
Change to new name: 'aidia-gw' for example:
	sudo hostnamectl set-hostname aidia-gw

Update /etc/hosts:

from :

127.0.0.1        localhost
::1              localhost
127.0.1.1        olddog.localdomain olddog

to:

127.0.0.1        localhost
::1              localhost
127.0.1.1        aidia-gw.localdomain aidia-gw


# ssh daemon 

Install:
	sudo pacman -S openssh
	sudo pacman -S manjaro-settings-samba
Check:
	sudo systemctl status sshd.service
Enable:
	sudo systemctl enable sshd.service
     	sudo systemctl start sshd.service

# avahi 

Install:
	sudo pacman -S avahi nss-mdns gtk3 python-dbus python-gobject
Enable:
	sudo systemctl enable avahi-daemon.service
	sudo systemctl start dbus.service
	sudo systemctl start avahi-daemon.service

# samba 

Install:
	sudo pacman -S samba
	sudo pacman -S manjaro-settings-samba

Add Login Account:
        sudo smbpasswd -a aidia
Enable:
	sudo systemctl enable --now smb nmb


# temperature measurement 

sudo pacman -S sysstat

新增 bash script:

cat << EOF > ./temperature_measurement.sh
#!/bin/bash
### Script: my-pi-temp.sh
Purpose: Display the ARM CPU and GPU  temperature of Raspberry Pi 2/3
Author: Vivek Gite <www.cyberciti.biz> under GPL v2.x+

```sh
thermal=$(</sys/class/thermal/thermal_zone0/temp)
cpu_utilization=$(mpstat | awk '$12 ~ /[0-9.]+/ { print 100 - $13"%" }')
echo "$(date +"%m/%d %H:%M:%S") @ $(uname -n) $((thermal/1000)) Celsius, cpu = $cpu_utilization"
#echo "CPU => $((cpu/1000))'C"
EOF
```

chmod +x ./temperature_measurement.sh

新增 cron job --> 每分鐘跑一次:

### Install package chronie
sudo pacman -S cronie

### See package content
sudo pacman -Ql cronie

### export default editor
export EDITOR=emacs

### edit cron job
crontab -e

### add entry
```sh
* * * * * /home/tom/temperature_measurement.sh >> /home/tom/temperature_measurement.txt
```

### Enable service
sudo systemctl enable cronie.service

### Start service
sudo systemctl start cronie.service

# access control service 

### install aidia_server
git clone ssh://git@192.168.1.130:30001/aiot/access_control.git
cd access_control
make install

### install service
sudo cat << EOF > /etc/systemd/system/access_control.service
[Unit]
Description=Aidia Access Control Service
After=syslog.target network.target network-online.target

[Service]
Type=simple
ExecStart=/usr/local/bin/aidia_server 0.0.0.0 8899 /tmp
Restart=always
ExecStop=/usr/bin/pkill aidia_server

[Install]
WantedBy=multi-user.target

### Enable service
sudo systemctl enable access_control

### Start service
sudo systemctl start access_control


#  mariadb service 
sudo pacman -Syu mariadb
sudo mysql_install_db --user=mysql --basedir=/usr --datadir=/var/lib/mysql
sudo systemctl start mariadb
sudo systemctl enable mariadb

// enable remote, check /etc/my.cnf.d/server.cnf
// must have 'bind-address' marked, like this:
#bind-address=0.0.0.0

// in the same file, use more RAM,
// less SD wear and tear , set to 75% or RAM
// i.e. For Pi 4G -> set pool size to 3G
[mariadb]
innodb_buffer_pool_size = 3G

sudo mysql_secure_installation
Enter current password for root (enter for none):123456

> [!NOTE] Note
> Please refer to mariadb_secure_setup_example.txt

sudo systemctl restart mariadb

If we use ssh to remote setup mariadb now, restart to allow root access

sudo mysql -uroot -p

> [!NOTE]
> Enter sudo password, not mysql user password

# template to set database 
### create user: `dbuser` in database
```d
	CREATE USER '<dbuser>'@'localhost' IDENTIFIED BY '<PASSWORD>';
```

### create new table: `dbname`
```d
CREATE DATABASE <dbname> CHARACTER SET utf8 COLLATE utf8_bin;
```

### grant this user from remote:
```d
GRANT ALL PRIVILEGES ON <dbname>.* TO '<dbuser>'@'%' IDENTIFIED BY '<PASSWORD>';
FLUSH PRIVILEGES;
```

## example 
```d
CREATE USER 'dbuser'@'localhost' IDENTIFIED BY '123456';
CREATE DATABASE inventory CHARACTER SET utf8 COLLATE utf8_bin;
GRANT ALL PRIVILEGES ON inventory.* TO 'dbuser'@'%' IDENTIFIED BY '123456';
```

> [!NOTE] In case that you want to be another super user

```d
GRANT ALL PRIVILEGES ON *.* TO 'dbuser'@'%';
FLUSH PRIVILEGES;
```


========== Mariadb import/export example ==========
# export DB 'inventory' to dump.sql
mysqldump inventory > dump.sql
# DB 'inventory' import from file
mysql inventory < dump.sql


========== Mariadb Backup ( hardware backup: file level ) ==========
# mariadb user id is 'mysql' under Manjaro
sudo mysql -u root -p
CREATE USER 'mariabackup'@'localhost' IDENTIFIED BY 'mypassword';
GRANT RELOAD, PROCESS, LOCK TABLES, BINLOG MONITOR ON *.* TO 'mariabackup'@'localhost';
exit;

sudo mkdir -p /var/mariadb/backup
sudo chown mysql:mysql /var/mariadb/ -R
sudo runuser -u mysql -- mariabackup --backup    --target-dir=/var/mariadb/backup/    --user=mariabackup --password=mypassword

=========== Restoration =============================

sudo systemctl stop mariadb
# clean restoration target dir
sudo rm -r /var/lib/mysql/*
# sync db timing
sudo runuser -u mysql -- mariabackup --prepare --target-dir=/var/mariadb/backup
sudo runuser -u mysql -- mariabackup --copy-back --target-dir=/var/mariadb/backup
# now check the user:group id of /var/lib/mysql, should be mysql:mysql
sudo systemctl start mariadb
========================================


========== docker install ==========
sudo pacman -Syu
sudo pacman -S docker
sudo systemctl enable --now docker
sudo systemctl status docker
docker --version
docker info

# run docker w/o sudo
sudo usermod -aG docker $USER
newgrp docker

# Create Docker Container on Manjaro Linux
docker pull centos
docker run -it centos
========================================

========== uwsgi [ install on Host ] ==========
sudo pacman -Syu python uwsgi uwsgi-plugin-python
===========================

========== uwsgi [ install on Python Virtual Environment ] ==========
sudo pacman -Syu cmake gcc python3
[ after sourcing virtual environment ... ]
(vent) pip install uwsgi
===========================

========== openvpn client ==========
1. Upgrade packages:
sudo pacman -Syu

2. Install prerequest package: OpenVPN plugin for NetworkManager:
sudo pacman -Syu networkmanager-openvpn

3. Export cosmos.ovpn from ASUS router.

4. Import cosmos.ovpn:

sudo nmcli connection import type openvpn file cosmos.ovpn
nmcli connection show

check that 'cosmos' connection name does exist

5. Edit /etc/NetworkManager/system-connections/cosmos.nmconnection:

   	[vpn]
   	# Change this from 1 to 0 so that it doesn't try to load the keyring
   	password-flags=0

   	# Add this under the [vpn] section
   	username=aidialink

	# Add the section
   	[vpn-secrets]
   	password=VPN45101524


    sudo nmcli connection reload cosmos
    sudo nmcli connection up cosmos

6. Auto connection, edit /bin/keepvpnup.sh:

	#!/bin/bash

	VPNNAME='cosmos'
	VPNSTATUS=$(nmcli connection show --active $VPNNAME | wc -l)
	if [ "$VPNSTATUS" == "0" ]
	then
	    nmcli connection up $VPNNAME > /dev/null 2>&1
	fi

   chmod +x /bin/keepvpnup.sh

7. Add the following line to crontab -e

*/1 * * * * /bin/keepvpnup.sh

Remember to enable cronie service

====================


========== 升級可能的問題 ==========
https://forum.manjaro.org/t/pacman-syu-failed-on-raspberry-pi/92858

有人升級後發生此問題：
error: failed to commit transaction (conflicting files)
rpi4-post-install: /etc/udev/rules.d/99-vcio-rewrite.rules exists in filesystem
Errors occurred, no packages were upgraded.

這是因為發佈者為了解決某些現象，臨時增加這個檔案，不屬於任何 package，但是後來把它歸類在
rpi4-post-install package 內，結果造成後續發生這個錯誤回報。

如何修正？
sudo pacman -Syu --overwrite /etc/udev/rules.d/99-vcio-rewrite.rules

證據：
修正之前，查不到所屬 package：
root@pi4b-12 ~]# pacman -Qo /etc/udev/rules.d/99-vcio-rewrite.rules
error: No package owns /etc/udev/rules.d/99-vcio-rewrite.rules

之後歸屬 rpi-post-install：
[root@pi4b-12 ~]# pacman -Qo /etc/udev/rules.d/99-vcio-rewrite.rules
/etc/udev/rules.d/99-vcio-rewrite.rules is owned by rpi4-post-install 20211125-3
====================


========== 升級可能的問題 : mouse 不正常 ==========
Raspberry Pi kernels upstream, they no longer support FKMS,
Open /boot/config.txt, change fkms to kms in the dtoverlay line. Save the file and reboot.
====================


==================== mysql log error ====================
當執行 journalctl 時，發現一堆 mariadb 的錯誤如下：

Aug 25 17:15:07 aidia-gw mariadbd[438]: 2022-08-25 17:15:07 34 [ERROR] Incorrect definition of table mysql.column_stats: expected column 'histogram' at position 10 to have type longblob, found type varbinary(255).
Aug 25 17:15:07 aidia-gw mariadbd[438]: 2022-08-25 17:15:07 34 [ERROR] Incorrect definition of table mysql.column_stats: expected column 'hist_type' at position 9 to have type enum('SINGLE_PREC_HB','DOUBLE_PREC_HB','JSON_HB'), found type enum('SINGLE_PREC_HB','DOUBLE_PREC_HB').
Aug 25 17:15:07 aidia-gw mariadbd[438]: 2022-08-25 17:15:07 34 [ERROR] Incorrect definition of table mysql.column_stats: expected column 'histogram' at position 10 to have type longblob, found type varbinary(255).
Aug 25 17:15:07 aidia-gw mariadbd[438]: 2022-08-25 17:15:07 34 [ERROR] Incorrect definition of table mysql.column_stats: expected column 'hist_type' at position 9 to have type enum('SINGLE_PREC_HB','DOUBLE_PREC_HB','JSON_HB'), found type enum('SINGLE_PREC_HB','DOUBLE_PREC_HB').

這是欄位型態不合，不知道是不是 django migrate 的時候跑掉了，
執行下面命令即可修正：
  mysql_upgrade --user=root --password;
再次啟動後，log 已經沒有錯誤訊息了。
==================== dbeaver ====================

https://snapcraft.io/install/dbeaver-ce/manjaro

==================== frp client service ====================
[Unit]
Description=Frp Client Service
After=network.target

[Service]
Type=simple
User=nobody
Restart=on-failure
RestartSec=5s
ExecStart=/usr/local/bin/frpc -c /etc/frp/%i.ini
ExecReload=/usr/local/bin/frpc reload -c /etc/frp/%i.ini
LimitNOFILE=1048576

[Install]
WantedBy=multi-user.target

==================== frp client ini ====================
[common]
server_addr = 192.168.88.16
server_port = 7000

[ssh]
type = tcp
local_ip = 127.0.0.1
local_port = 22
remote_port = 6000
