這本書是翻譯原文書 [Learning Deep Learning](https://ldlbook.com/)，網頁中有 [Running LDL code examples in a Docker Container](https://ldlbook.com/2022/01/31/dl-in-docker/)
``` ln:false
nvidia-docker run --shm-size=1g --ulimit memlock=-1 --ulimit stack=67108864 --rm -it -p 8888:8888 -v /home/USERNAME/LDL-main:/home/LDL nvcr.io/nvidia/tensorflow:21.07-tf2-py3 /bin/bash
```
將 /home/USERNAME/... 的路徑，換成我們下載的 LDL-main 路徑
如果 8888 已經佔用，改成其他例如 7777 的 port，matplotlib 需要指定 3.6 不然現在版本太新，會出現 TensorFlow 不相容的問題，就跟 docker 提供的不合了。
``` ln:false
pip install matplotlib==3.6
```

> [!NOTE] 這個 docker 需要 NumPy 1.19.2
> https://matplotlib.org/devdocs/devel/min_dep_policy.html 
這裏 `matplotlib`3.6 對應 `NumPy` 是 1.19 才符合 TF 的需求

docker 的 bash 裏面可以跑 notebook：

``` ln:false
cd /home/LDL

jupyter notebook --ip 0.0.0.0 --port 8888 --allow-root
```


> [!Warning] Port
> Docker 裏面是用 8888 ，對應到我們 Host 的 7777，開頭有說明
