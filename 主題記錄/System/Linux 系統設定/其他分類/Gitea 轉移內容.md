# 需求緣起
要將原本的 Gitea 搬移到新的 Gitea

# 環境
使用 Docker composer 啓動 Gitea：
```yml

version: "3"

networks:
  gitea:
    external: false

services:
  server:
    image: gitea/gitea:latest
    container_name: gitea
    environment:
      - USER_UID=1000
      - USER_GID=1000
      - GITEA__repository__ENABLE_PUSH_CREATE_USER=true
      - GITEA__ssh__MINIMUM_KEY_SIZE=2048
      - GITEA__repository__ENABLE_PUSH_CREATE_ORG=true
    restart: always

    networks:
      - gitea
    volumes:
      - /home/tom/documents/gitea:/data
      - /etc/timezone:/etc/timezone:ro
      - /etc/localtime:/etc/localtime:ro
    ports:
      - "3000:3000"
      - "222:22"
```
重點是 Line 15-17 的
      `GITEA__repository__ENABLE_PUSH_CREATE_USER=true`
      `GITEA__repository__ENABLE_PUSH_CREATE_ORG=true`

我們之後希望 可以直接用 script 對每個 repository 直接搬移，那麼新的 Gitea 需要支援 Push To Create ，並且因爲我們有 4 個 Groups / Organizations，也希望可以用 organization  的方式 push 進來，因此打開這兩個 flags, 它們會放在`/data/gitea/conf/app.ini` 裏面， 也就是經過第 23 行，對應到 host
機器的`/home/tom/documents/gitea/` .... 裏面。


利用 Protainer 啓動 Gitea 的 Docker，把上面的 docker compose 寫入 stack，就可以啓動 Gitea：
![[Pasted image 20240726134849.png]]

# Moving script

Gitee 服務起來後，接下來，從舊的 service (192.168.1.210:30030) 拷貝到新的 service (localhost:3000)。 projects.txt 列表如下，每行代表一個 repo name：
```txt
tag
Testeslserver
heartgateway
ESL-Management
midware
ESL-Core
GW-Management
qt-dispenser
ESL_Project_Tracking
ESLCore-ImportExport
win-dashboard
scanner
win-sources
win-manufacturing
```

Script 如下，其中 `$project_name` 是 projects.txt 的一行，從舊服務 clone 下來，再推到新服務裏面，`--mirror` 是把涵蓋全部 branches，

```bash
  #!/bin/bash

# 舊的 Gitee 用户名
old_username="tom"
# 新的 Gitee 用户名
new_username="tom"

# 项目列表文件，每行一个项目名
project_list="projects.txt"
group="ESL-11"
password="123456"

while IFS= read -r project_name
do
  echo "Cloning $project_name..."
  git clone --mirror ssh://git@192.168.1.210:30031/ESL-11/$project_name.git

  cd $project_name.git

  echo "Setting new remote URL for $project_name..."
  git remote set-url origin http://$new_username:$password@localhost:3000/$group/$project_name.git
  echo "Pushing $project_name to new remote..."
  git push --mirror

  cd ..

  echo "Cleaning up local repository for $project_name..."
  rm -rf $project_name.git

done < "$project_list"

```
# Gitea dump

如果要備份 repos ，開啓 console

![[Pasted image 20240726154817.png]]

按 connect 可以開啓 console：

![[Pasted image 20240726155203.png]]

cd 到 `/tmp`然後執行：
```bash
/usr/local/bin/gitea dump
```
就可以備份，這將 repos 都壓縮到 `/tmp/gitea-dump-xxxxxx.zip`

執行如下圖：

![[Pasted image 20240726154052.png]]
Docker 是唯讀的，此時將 `/tmp/gitea-dump-xxxxx.zip` 拷貝到 `/data` 下面，因爲我們啓動 docker 時，有將 host 目錄對應給它（例如 `/home/tom/documents/gitea`），這樣就把備份檔案 copy 到 host 了。


> [!NOTE] 參考文件
>
> Push To Create
> https://docs.gitea.com/next/usage/push-to-create
>
> 備份
> https://docs.gitea.com/zh-cn/administration/backup-and-restore



