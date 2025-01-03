本文件說明如何製造 Debian 10 on Windows VirtualBox 6.0 image. 本目錄之
Virtual_box.tar.xz 是 VGHKS  Final 使用的映像檔.

### System
```
sudo apt install -y build-essential nginx-extras git mariadb-server git avahi-daemon autoconf re2c bison libxml2-dev nodejs wget curl cmake libssl-dev bzr libmariadbclient-dev libmariadb-dev libboost-all-dev git samba libavahi-client-dev libdbus-1-dev pkg-config libc-dev autoconf libmcrypt-dev unzip bzip2 zip
```

### PHP
```
sudo apt install -y php-gd php-imagick php-json php-mysql php-ssh2 php-xmlrpc php-fpm php-mbstring php-curl php-xml php-token-stream php-json php-dev php-fpm php-zip php-bz2 php-console-table php-cli php-mongodb php-php-gettext
```



## Nginx

Modify /etc/nginx/sites-available/default:

```

		index index.php index.html index.htm;
		:
		:
		:
        location ~ \.php$ {
                include snippets/fastcgi-php.conf;

                # With php-fpm (or other unix sockets):
                fastcgi_pass unix:/run/php/php7.3-fpm.sock;
                # With php-cgi (or other tcp sockets):
                # fastcgi_pass 127.0.0.1:9000;
        }

```
## Mariadb
```
sudo apt -y install mariadb-server mariadb-client
sudo mysql_secure_installation
# current password for root? [initially none, just press Enter]
# new password [123456]
# remove anonymous users? [Y]
# disallow root login remotely? [N, we want remote root login]
# remove test database? [Y]
# reload privilege tables now [Y]
```

Modify /etc/mysql/mariadb.conf.d/50-server.cnf:
mark '#' before the line of 'bind-address   =  127.0.0.1'



## Service

```shell
sudo systemctl enable mariadb;
sudo systemctl enable php7.3-fpm;
sudo systemctl enable nginx;
sudo systemctl start mariadb;
sudo systemctl start php7.3-fpm;
sudo systemctl start nginx
```



## ESLCore



```shell
cd /var/www/html
sudo git clone http://192.168.1.210:30030/ESL-11/ESL-Core.git eslcore
```



```shell
cd eslcore
sudo git checkout -b develop origin/develop
```

## Sudoer


使用root或具sudo 權限帳號:

```
sudo cp docs/eslcore.sudoers /etc/sudoers.d/eslcore
```

以下假設您已經在`eslcore`目錄中。

### Create/Import Database

首先要允許遠端 database GUI 連線：
add mark on 'bind-address' line in /etc/mysql/mariadb.conf.d/50-server.cnf
> sudo systemctl restart mariadb

建立一空白資料庫名為`esl_d2`編碼為`utf8m4_general_ci`，並將系統根目錄中的`docs\esl_d2.sql`匯入建立的資料表:

```
sudo mariadb -u root -p
敲入 123456 密碼進入
> CREATE DATABASE esl_d2 CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
> GRANT ALL PRIVILEGES ON *.* TO 'esl'@'192.168.%.%' IDENTIFIED BY 'esl123456' WITH GRANT OPTION;
> GRANT ALL PRIVILEGES ON *.* TO 'esl'@'localhost' IDENTIFIED BY 'esl123456' WITH GRANT OPTION;
> GRANT ALL PRIVILEGES ON *.* TO 'root'@'192.168.%.%' IDENTIFIED BY '123456' WITH GRANT OPTION;
> GRANT ALL PRIVILEGES ON *.* TO 'root'@'localhost' IDENTIFIED BY '123456' WITH GRANT OPTION;
> exit;
```
後面兩句 GRANT... 提供以後從其他 site 連進來操作及除錯的途徑.

透過 GUI 工具 import `docs/esl_d2.sql`:
Windows 下有 `HeidiSQL`
Mac 下有 `Sequel Pro`

## 設定資料庫連線

建立`config/config.inc.php`並新增：

```php
<?php
$default->db_user = 'esl';//帳號
$default->db_pass = 'esl123456';//密碼
$default->db_host = "localhost";//位址
$default->db_name = "esl_d2";//資料表名稱
$default->db_port = "3306";//連線port
$default->db_driver = 'mysql';//此項目照抄不要變更
```



## 設定Gateway連線

Gateway設定資料於資庫中的`gateway`資料表



![](docs/2020-06-10_20-54-45.png)

目前只有`gateway_address`、`gateway_port`設定有效，手動變更此項來改變與Gateway連線。


## 確認 www-data 權限
```shell
cd /var/www/html
sudo chown -R www-data:www-data /var/www/html

```

## 手動執行背景程式

執行系統之背景程式必需要以`www-data`權限執行，例如：

```shell
cd esl-server
sudo -u www-data ./eserver_cli.php
```

如果能正確執行，看到如下畫面，表示資料庫連線、`midware`與`Gateway`連線正常：

```shell
[2020-06-10 12:43:52]+=============+
[2020-06-10 12:43:52]|             |
[2020-06-10 12:43:52]| eserver_cli |
[2020-06-10 12:43:52]|             |
[2020-06-10 12:43:52]+=============+
[2020-06-10 12:43:52]Main:31010600
[2020-06-10 12:43:52]Connection:32010600
[2020-06-10 12:43:52]Midware:33010600
[2020-06-10 12:43:52]Jobqueue:34010600
[2020-06-10 12:43:52]+=============+
[2020-06-10 12:43:52]Parent pid:25063
[2020-06-10 12:43:52]Register SIGINT Handler
[2020-06-10 12:43:52]Register SIGTERM Handler
[2020-06-10 12:43:52]Register SIGHUP Handler
[2020-06-10 12:43:52]Register SIGUSR1 Handler
[2020-06-10 12:43:52]Empty TagConversionQueue Rows
[2020-06-10 12:43:52]empty Jobqueue Rows
[2020-06-10 12:43:52]Fork Connection
[2020-06-10 12:43:52]Socket Pid:25068
[2020-06-10 12:43:52]Fork Middleware
[2020-06-10 12:43:52]Midware Pid:25069
[2020-06-10 12:43:52]Fork Jobqueue
[2020-06-10 12:43:52]JobQueue Pid:25070
[2020-06-10 20:43:52.750193] [0x00007f0cecc49b80] [trace]   esl_module_factory(), p_module_instalce = 0x558f1972a7e0
[2020-06-10 12:43:52]esl_module_init
[2020-06-10 12:43:52]Gateway ip:192.168.19.42 cport:11081
```

此時其整個系統已完成安裝



## 開機執行背景程式

執行安裝程式時，確保執行帳號有`sudo`權限：

```shell
cd esl-server/service
./setup.sh install

==========================================================================
[  OK  ] Platform:x86_64
[  OK  ] Install eserver service
[  OK  ] Enable eserver service
==========================================================================
```



### 啟動、停止

```shell
sudo systemctl stop eserver.service
sudo systemctl start eserver.service
```



### 查看狀態

```shell
sudo systemctl status eserver.service
```



### 查看eserver輸出訊息

`eserver`使用systemd將程式推至背景執行，其使用`journalctl`查看當下的輸出訊息，使用此指令需要有`sudo`權限：

````shell
sudo journalctl -u eserver.service -f -n 50
````
