## up and running
遵照 parseable github
https://github.com/parseablehq/parseable
的 readme 就可以跑起來，然後用 curl 送 log message 可以成功
但是如果 portainer 已經佔領 8000, 啓動的 docker 命令要改 port ：
``` bash
# local port changed to 8200, map to service port 8000 in docker
docker run -p 8200:8000 \
  -v $HOME/parseable/data:/parseable/data \
  -v $HOME/parseable/staging:/parseable/staging \
  -e P_FS_DIR=/parseable/data \
  -e P_STAGING_DIR=/parseable/staging \
  parseable/parseable:latest \
  parseable local-store
```

內建 login 與 password 都是 'admin'

## 用 portainer 管理 ##

如果用 portainer 管理，可以先到 portainer Volumes 頁面新增一個 Volume：

![[Pasted image 20230823063208.png]]
如上圖，我們新增了一個 parseable_volume 到 docker 系統，
那麼 docker run 的命令就可以改爲：

```bash
docker run \
	-p 8200:8000 \
	-v parseable_volume:/parseable \
	-e P_FS_DIR=/parseable/data  \
	-e P_STAGING_DIR=/parseable/staging \
	parseable/parseable:latest \
	parseable local-store
```

告訴 docker 現在使用 docker 系統裏面的 parseable_volume ，而不要在我們的 home 下面多生出兩個 directories ，也便於管理。

這時候，console 看到畫面這樣：

![[Pasted image 20230823064802.png]]

現在 browser 到 http://localhost:9000 打開 portainer， 主畫面爲：

![[Pasted image 20230907105911.png]]

我們要看 local 跑起來的 container，目前只有 portainer。

打開 _local_ Containers 頁面，可以看到：

![[Pasted image 20230823064208.png]]

畫面右上角，將 parseable stop 然後 restart 就可以接管啦，原本執行命令的 console 也不會停留在啓動畫面。


