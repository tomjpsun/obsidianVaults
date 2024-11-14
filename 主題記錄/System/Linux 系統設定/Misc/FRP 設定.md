FRP 是利用 public server 提供代理，可以代理防火牆內部的服務，提供給防火牆外面的世界使用這個服務。專案位置在這裏：
https://github.com/fatedier/frp
在它的 [Release](https://github.com/fatedier/frp/releases)頁面可以下載 binary，可以得到 X86 或 ARM 架構下的
frps (server 執行檔) 、frpc(client 執行檔)、frps.toml(server 設定檔)、frpc.toml(client設定檔)

# Public 的 Server A （代理伺服器）
需要在 /etc/systemd/system/ 下建立一個 frps.service，內容是：
```json
[Unit]
Description=frp server
After=network.target

[Service]
Type=simple
ExecStart=/usr/local/bin/frps -c /etc/frps/frps.toml
Restart=alwayse
ExecStop=/usr/bin/pkill frps

[Install]
WantedBy=multi-user.target

```

其中 frps 執行檔需要手動 copy 到 /usr/local/bin/ 下面
frps.toml copy 到 /etc/frps/ 沒有目錄就建立一個，內容如下：
```json
bindPort = 7000
```

然後啓動並enable：
```bash
sudo systemctl start frps
sudo systemctl enable frps
```
# 防火牆內的 server B（原伺服器）
在 /etc/systemd/system 手動建立 frpc.service:
```json
[Unit]
Description=Frp Client Service
After=network.target

[Service]
Type=simple
User=nobody
Restart=on-failure
RestartSec=5s
ExecStart=/usr/local/bin/frpc -c /etc/frp/frpc.toml
ExecReload=/usr/local/bin/frpc reload -c /etc/frp/frpc.toml
LimitNOFILE=1048576

[Install]
WantedBy=multi-user.target
```

其中 frpc 執行檔需要手動 copy 到 /usr/local/bin/ 下面然後啓動並enable：

```bash
sudo systemctl start frpc
sudo systemctl enable frpc
```


類似與 server 那邊，手動建立 /etc/frp/frpc.toml
```json
serverAddr = "61.230.13.84"
serverPort = 7000

[[proxies]]
name = "nx"
type = "tcp"
localIP = "127.0.0.1"
localPort = 4000
remotePort = 4000
```
其中 serverAddr 改指到 public 的 server address，這個設定是 server B 的服務 port 4000 ，那個 port 7000 是 frp client 與 server 在使用的，並不是改變服務的 port ， 也i就是說，將來 server A 的代理服務，還是由 server A 的 port 4000 提供哦！

然後啓動並enable：

```bash
sudo systemctl start frpc
sudo systemctl enable frpc
```

`finish`
