
依照官方文件 https://docs.nvidia.com/cuda/cuda-installation-guide-linux/#network-repo-installation-for-debian
裏面有 Debian 12 的安裝方式：

**Update the Apt repository cache:**

`sudo apt-get update`

**Install CUDA SDK:**

`sudo apt-get -y install cuda-toolkit`

**Reboot the system:**

`sudo reboot`

- Perform the [post-installation actions.](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html#post-installation-actions)


[TensorFlow CUDA version Table](https://www.tensorflow.org/install/source#gpu)
目前已經安裝 `CUDA 12-6` 與 `cuDNN-9 `所以直接對到最頂列：
#### GPU

| Version           | Python version | Compiler     | Build tools | cuDNN | CUDA |
| ----------------- | -------------- | ------------ | ----------- | ----- | ---- |
| tensorflow-2.17.0 | 3.9-3.12       | Clang 17.0.6 | Bazel 6.5.0 | 8.9   | 12.3 |
| tensorflow-2.16.1 | 3.9-3.12       | Clang 17.0.6 | Bazel 6.5.0 | 8.9   | 12.3 |

