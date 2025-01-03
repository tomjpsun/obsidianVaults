節錄自
[https://www.server-world.info/en/note?os=Debian_12&p=nvidia&f=2](https://www.server-world.info/en/note?os=Debian_12&p=nvidia&f=2)

+ Add `contrib` `contrib non-free` to /etc/apt/sources.list
+ `sudo apt install linux-headers-amd64`
+ `sudo apt install nvidia-driver firmware-misc-nonfree`
+ reboot
+ `sudo apt install nvidia-smi`
+ `sudo apt install docker`
+ `sudo usermod -aG docker $USER`
+ reboot
+  `su` 然後執行下面指令， 以安裝 nvidia-container-toolkit
```bash
curl -s -L https://nvidia.github.io/nvidia-docker/gpgkey > /etc/apt/keyrings/nvidia-docker.key

curl -s -L https://nvidia.github.io/nvidia-docker/debian11/nvidia-docker.list > /etc/apt/sources.list.d/nvidia-docker.list

sed -i -e "s/^deb/deb \[signed-by=\/etc\/apt\/keyrings\/nvidia-docker.key\]/g" /etc/apt/sources.list.d/nvidia-docker.list

apt update

apt -y install nvidia-container-toolkit

systemctl restart docker
```
以上

接着依照 [[Portainer 筆記]] 裏面安裝 handson-ml2 就可以順利有 GPU 環境了

