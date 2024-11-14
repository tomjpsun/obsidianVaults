# 先將 Jetson Nano 接上顯示器

# Install VINO

```bash
sudo apt update
sudo apt install vino
```

# Bypass VNC verification

```bash
gsettings set org.gnome.Vino prompt-enabled false
gsettings set org.gnome.Vino require-encryption false
```

# 將網卡加入 VINO 服務

```bash
nmcli connection show
```

## Output

```bash
NAME                UUID                                  TYPE      DEVICE
Wired connection 1  fce16d59-0fd1-3ec2-aeab-c4a8a3c1147c  ethernet  eth0
```

## 使用 dconf 寫入

```bash
dconf write /org/gnome/settings-daemon/plugins/sharing/vino-server/enabled-connections "['Your UUID']"
```

## Example

```bash
dconf write /org/gnome/settings-daemon/plugins/sharing/vino-server/enabled-connections "['fce16d59-0fd1-3ec2-aeab-c4a8a3c1147c']"
```

## export DISPLAY

```bash
export DISPLAY=:0
```

# 取得 ip

```bash
ip a
```

## Output

```bash
3: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP group default qlen 1000
    link/ether 48:b0:2d:2d:e7:7d brd ff:ff:ff:ff:ff:ff
    inet 192.168.68.116/24 brd 192.168.68.255 scope global dynamic noprefixroute eth0
       valid_lft 6050sec preferred_lft 6050sec
    inet6 ::100d/128 scope global dynamic noprefixroute
       valid_lft 85256sec preferred_lft 85256sec
    inet6 fe80::25f7:f4d9:2a5c:8fcc/64 scope link noprefixroute
       valid_lft forever preferred_lft forever
```

可得知 ip 是 `192.168.68.116`

# 開機自動啟動 VINO

```bash
cd ~/.config
mkdir autostart
sudo vim ~/.config/autostart/vino-server.desktop
```

## vino-server.desktop

```bash
[Desktop Entry]
Type=Application
Exec=/usr/lib/vino/vino-server
Hidden=false
NoDisplay=false
X-GNOME-Autostart-enabled=true
Name[en_US]=Vino server startup
Name=Vino server startup
Comment[en_US]=Remote desktop
Comment=Remote desktop
```

# 調整連線解析度

```bash
sudo vim /etc/X11/xorg.conf
```

## xorg.conf

在最下面加入以下內容 (EndSection 下面)

```bash
Section "Screen"
    Identifier "Default Screen"
    Monitor "Configured Monitor"
    Device "Default Device"
    SubSection "Display"
        Depth 24
        Virtual 1280 800
    EndSubSection
EndSection
```

# 防止 Jetson Nano 休眠 (LXDE)

左下角 -> Preferences -> Screensaver -> Display Modes -> Disable Screen Saver

# 開機自動登入 Jetson Nano

## 關閉密碼

左下角 -> Preferences -> Users and Groups -> Password -> Change -> Not asked on login

## 自動登入

```bash
cd /etc/lightdm/lightdm.conf.d
sudo vim 50-myconfig.conf
```

## 50-myconfig.conf

```bash
[Seat:*]
greeter-show-manual-login=false
allow-guest=false
autologin-user=rocket
```

Replace rocket as your user name.

## Reboot

```bash
sudo reboot
```

# Run VINO

```bash
/usr/lib/vino/vino-server
```

# 完成

接下來在要連過去的電腦下載 VNC Viewer

https://www.realvnc.com/en/connect/download/viewer/

輸入 Jetson Nano 的 ip + port 5900 來連線

## Example

```bash
192.168.68.116:5900
```


