當執行 `docker run --rm --gpus all nvidia/cuda:11.0-base nvidia-smi`
看到下面 error：

``` ln:false
Unable to find image 'nvidia/cuda:11.0-base' locally'
docker: Error response from daemon: manifest for nvidia/cuda:11.0-base not found: manifest unknown: manifest unknown.
See 'docker run --help'.
```

因爲那個 docker 已經拿掉不存在了，可以改用 ubuntu 的 cuda image：

`docker run --rm --gpus all nvidia/cuda:11.0.3-base-ubuntu20.04 nvidia-smi`

解決 #

