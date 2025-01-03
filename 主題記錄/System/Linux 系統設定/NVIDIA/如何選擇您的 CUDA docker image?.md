nvcr.io 提供的 docker image, 從命名看不出是 對應到 那一個 TensorFlow 版本， 如果我們需要爲特定的 TensorFlow 找到支援的 CUDA docker image，需要到這裏對照 compability matrix
https://docs.nvidia.com/deeplearning/frameworks/support-matrix/index.html

# NVidia Container Supoort Matrix
如果執行 `nvidia-smi` 發現 `Driver`版本是 535.183 如下：
```
+---------------------------------------------------------------------------------------+
| NVIDIA-SMI 535.183.01             Driver Version: 535.183.01   CUDA Version: 12.2     |
|-----------------------------------------+----------------------+----------------------+
| GPU  Name                 Persistence-M | Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp   Perf          Pwr:Usage/Cap |         Memory-Usage | GPU-Util  Compute M. |
|                                         |                      |               MIG M. |
|=========================================+======================+======================|
|   0  NVIDIA GeForce RTX 2080 Ti     On  | 00000000:01:00.0  On |                  N/A |
| 27%   42C    P8              26W / 250W |    649MiB / 11264MiB |      4%      Default |
|                                         |                      |                  N/A |
+-----------------------------------------+----------------------+----------------------+

+---------------------------------------------------------------------------------------+
| Processes:                                                                            |
|  GPU   GI   CI        PID   Type   Process name                            GPU Memory |
|        ID   ID                                                             Usage      |
|=======================================================================================|
|    0   N/A  N/A      2171      G   /usr/lib/xorg/Xorg                          302MiB |
|    0   N/A  N/A      2373      G   /usr/bin/gnome-shell                         88MiB |
|    0   N/A  N/A      6397      G   /usr/bin/nautilus                            14MiB |
|    0   N/A  N/A     15579      G   ...erProcess --variations-seed-version       56MiB |
|    0   N/A  N/A     21291      G   /usr/lib/firefox-esr/firefox-esr            182MiB |
+---------------------------------------------------------------------------------------+
```
根據 [Compatibility Matrix](https://docs.nvidia.com/deeplearning/frameworks/support-matrix/index.html#framework-matrix-2023)

**Driver 535.183** 出現在 **2.23.xx** Framework Containers Support Matrix 裏面。

就知道應該搭配 **CUDA 12.2.2** 到 **CUDA 12.1.1** , 也就是 container **23.10**  到  **23.07**。

繼續對到 **TensorFlow** 版本, 就是 container 裏面可以跑 **TensorFlow 2.13.0** 的程式。


> [!NOTE] 結論
>用這個 matrix 可以找到相容 driver 的 container 版本。

