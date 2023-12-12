## 權限
當我們執行docker 指令時若出現以下錯誤訊息
```
docker: Got permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock.....
```

表示目前的使用者身分沒有權限去存取docker engine, 因為docker的服務基本上都是以root的身分在執行的, 所以在指令前加sudo就能成功執行指令
但每次實行docker指令(就連docker ps)都還要加sudo實在有點麻煩,

### 正確做法
我們可以把目前使用者加到 docker 群組裡面, 當 docker service 起來時, 會以這個群組的成員來初始化相關服務:

``` bash
sudo usermod -aG docker tom
```

Run up 指令, 依照
https://ivonblog.com/posts/portainer-installation/
```bash
docker volume create portainer_data

docker run -d -p 8000:8000 -p 9000:9000 -p 9443:9443 --name portainer --restart=always -v /var/run/docker.sock:/var/run/docker.sock -v portainer_data:/data portainer/portainer-ce:latest
```


然後 browser 開 http://localhost:9000
或是 secure http  https://localhost:9443


## 依照 docker-compose.yml 啓動 container

## 後記

1. 還沒有研究如何在 portainer 直接啓動 docker。
2. 在左側選單裏面有 App Template 可以直接安裝啓動 rabbitMQ, MariaDB ...(令人驚豔)。



#portainer_password 

