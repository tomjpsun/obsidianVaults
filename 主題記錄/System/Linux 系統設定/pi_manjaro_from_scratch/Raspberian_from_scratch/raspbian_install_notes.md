### add user as sudoer

```bash
> su
> [...type passwd]
> /sbin/adduser tom sudo
```

然後重新開機即可


### Avahi setup

Install avahi-daemon:

	apt install avahi-daemon


Add wlan0 to allowed interfaces in /etc/avahi/avahi-daemon.conf:

	allow-interfaces=eth0,wlan0

Add /etc/avahi/services/multiple.service with the following content:

```xml
<?xml version="1.0" standalone='no'?>
<!DOCTYPE service-group SYSTEM "avahi-service.dtd">
<service-group>
  <name replace-wildcards="yes">%h</name>
  <service>
    <type>_device-info._tcp</type>
    <port>0</port>
    <txt-record>model=RackMac</txt-record>
  </service>
  <service>
    <type>_ssh._tcp</type>
    <port>22</port>
  </service>
  <service>
    <type>_http._tcp</type>
    <port>80</port>
  </service>
</service-group>
```


Verification:

On another PC, use avahi-discover -a. The raspbi host should appear.

### eth0/wlan0 route setup

#### Goal:

Connect eth0 to RFID Reader (LAN 192.168.88.0)
Connect wlan0 to wireless AP (LAN 192.168.68.0) with internet access

#### Current route table:

Destination    Gateway       Genmask    Flags Metric Ref  Use Iface
0.0.0.0       192.168.68.1  0.0.0.0    UG    303    0    0 wlan0
0.0.0.0       192.168.88.1  0.0.0.0    UG    202    0    0 eth0
192.168.68.0  0.0.0.0       255.255.255.0 U    303    0    0 wlan0
192.168.88.0  0.0.0.0       255.255.255.0 U    202    0    0 eth0

#### Steps:

Install ifmetric:

	sudo apt install ifmetric

Set eth0 with lower priority (higher metric):

	sudo ifmetric eth0 305

Configure permanent settings in /etc/dhcpcd.conf:

	static domain_name_servers=8.8.8.8
	static routers=192.168.88.1
	static ip_address=192.168.88.1
	metric 305

Verify route table:

	route -n

Note: Setting wlan0 as higher priority (lower metric) can help solve internet access issues.


