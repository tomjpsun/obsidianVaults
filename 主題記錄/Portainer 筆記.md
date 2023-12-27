1 Run up 指令, 依照
https://ivonblog.com/posts/portainer-installation/
```bash
docker volume create portainer_data


docker run -d -p 8000:8000 -p 9000:9000 -p 9443:9443 --name portainer --restart=always -v /var/run/docker.sock:/var/run/docker.sock -v portainer_data:/data portainer/portainer-ce:latest
```


然後 browser 開 http://localhost:9000
或是 secure http  https://localhost:9443

## 後記

1. 還沒有研究如何在 portainer 直接啓動 docker。
2. 在左側選單裏面有 App Template 可以直接安裝啓動 rabbitMQ, MariaDB ...(令人驚豔)。


#portainer_password 

