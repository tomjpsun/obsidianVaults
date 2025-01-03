以前是 `nvidia-docker2` 套件，現在改爲 `nvidia-cotainer-toolkit`
參考這篇：
https://www.server-world.info/en/note?os=Debian_12&p=nvidia&f=2
還有這篇：
[NVidia 官方網站](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html)
節錄內容：

### Install with APT

Configure the production repository:

```bash ln:false
curl -fsSL https://nvidia.github.io/libnvidia-container/gpgkey | sudo gpg --dearmor -o /usr/share/keyrings/nvidia-container-toolkit-keyring.gpg \
  && curl -s -L https://nvidia.github.io/libnvidia-container/stable/deb/nvidia-container-toolkit.list | \
    sed 's#deb https://#deb [signed-by=/usr/share/keyrings/nvidia-container-toolkit-keyring.gpg] https://#g' | \
    sudo tee /etc/apt/sources.list.d/nvidia-container-toolkit.list
```

Optionally, configure the repository to use experimental packages:

```bash ln:false
sed -i -e '/experimental/ s/^#//g' /etc/apt/sources.list.d/nvidia-container-toolkit.list
```

Update the packages list from the repository:

```bash ln:false
sudo apt-get update
```

- Install the NVIDIA Container Toolkit packages:

```bash ln:false
sudo apt-get install -y nvidia-container-toolkit
```

### Configuring Docker

1. Configure the container runtime by using the `nvidia-ctk` command:
```bash ln:false
sudo nvidia-ctk runtime configure --runtime=docker
```

- The `nvidia-ctk` command modifies the `/etc/docker/daemon.json` file on the host. The file is updated so that Docker can use the NVIDIA Container Runtime.

2. Restart the Docker daemon:

```bash ln:false
sudo systemctl restart docker
```

### Install CDI
CDI 是 Container Device Interface， 在 container runtime 時提供可以使用的 NVidia Device（例如 GPU）它們會在是系統的 `/etc/cdi` 與 `/var/run/cdi` 下產生 specification file。
有時候 docker 啓動後會反應沒有 CDI spec. 這時候就依照下列官方網站來安裝 CDI：

[官方網站](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/cdi-support.html)

\#

