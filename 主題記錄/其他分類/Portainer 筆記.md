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

### 啓動 Portianer
依照
https://ivonblog.com/posts/portainer-installation/

```bash
docker volume create portainer_data
```

``` bash
docker run -d -p 8000:8000 -p 9000:9000 -p 9443:9443 --name portainer --restart=always -v /var/run/docker.sock:/var/run/docker.sock -v portainer_data:/data portainer/portainer-ce:latest
```

然後 browser 開 http://localhost:9000
或是 secure http  https://localhost:9443

## 依照 docker-compose.yml 啓動 container

現成的 docker compose yml 可從 GitHub 專案取得，例如：
[handson-ml2](git@github.com:ageron/handson-ml2.git)

我們使用該專案的 `docker/docker-compose.yml` 來實驗：
![[portainer_live_connect.png]]

首先 Live connect local portainer， 畫面就到 Dashboard：
![[portainer_dashboard.png]]

選 Stack 後
![[portainer_add_stack.png]]

 選 Add Stack， 填 Name (我們用 `handson-ml2-volume` )
![[portainer_create_stack.png]]

在 Web editor裏面填入 compose-docker.yml 的內容， 就是 copy past /docker/compose-docker.yml
來這裏：
![[portainer_edit_yml.png]]

修改一下 volumes，我希望它一開始就看到 source root，把 `/home/tom/work/PyCharm/handson-ml2` 填在冒號 (:) 前面.:

![[portainer-edit-volume.png]]

到最下面 deploy the stack：
![[portainer_deploy_stack.png]]


這裏要 download image 一些時間，如果事先 download 好就很快。
左邊選單選 container 頁面，已經看到 我們的 container 啓動了。
![[portainer_start_container.png]]

點選它，可以看到詳細資訊：
![[portainer_container_log.png]]

其中 Logs 可以找到 token ， 開 browser port 8888 連過來，需要輸入 token，成功。

## 後記

在左側選單裏面有 App Template 可以直接安裝啓動 rabbitMQ, MariaDB ...(令人驚豔)。



#portainer_password 

