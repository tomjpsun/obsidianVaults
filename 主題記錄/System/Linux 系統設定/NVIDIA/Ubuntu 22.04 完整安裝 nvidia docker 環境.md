### 安裝 nvidia-container-toolkit：

+ 添加 NVIDIA Docker Repository：

```
distribution=$(. /etc/os-release;echo $ID$VERSION_ID) \
    && curl -s -L https://nvidia.github.io/nvidia-docker/gpgkey | sudo apt-key add - \
    && curl -s -L https://nvidia.github.io/nvidia-docker/$distribution/nvidia-docker.list | sudo tee /etc/apt/sources.list.d/nvidia-docker.list
```

+ 更新套件列表並安裝 nvidia-container-toolkit：

```
sudo apt-get update
sudo apt-get install -y nvidia-container-toolkit
```
重新啟動 Docker：

```
sudo systemctl restart docker
```

### 安裝 nvidia-cuda-toolkit
```
sudo apt install nvidia-cuda-toolkit
```

### 安裝套件 nvidia-cudnn

```
sudo apt install nvidia-cudnn
```
