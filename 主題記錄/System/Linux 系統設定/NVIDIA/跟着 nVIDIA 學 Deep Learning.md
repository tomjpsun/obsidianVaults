這本書是翻譯原文書 [Learning Deep Learning](https://ldlbook.com/)，網頁中有 [Running LDL code examples in a Docker Container](https://ldlbook.com/2022/01/31/dl-in-docker/)


```bash ln:false
nvidia-docker run --shm-size=1g --ulimit memlock=-1 --ulimit stack=67108864 --rm -it -p 8888:8888 -v /home/USERNAME/LDL-main:/home/LDL nvcr.io/nvidia/tensorflow:21.07-tf2-py3 /bin/bash
```

## Docker run
將 /home/USERNAME/... 的路徑，換成我們下載的 LDL-main 路徑
如果 8888 已經佔用，改成其他例如 7777 的 port，matplotlib 需要指定 3.6 不然現在版本太新，會出現 TensorFlow 不相容的問題，就跟 docker 提供的不合了, 以我這邊環境爲例：

==Debian 12==
```bash ln=false
nvidia-docker run --shm-size=1g --ulimit memlock=-1 --ulimit stack=67108864 --rm -it -p 7777:8888 -v /home/tom/work/python_projects/LDL:/home/LDL nvcr.io/nvidia/tensorflow:21.07-tf2-py3 /bin/bash
```
==Ubuntu 22.04== Ubuntu 安裝 nvidia-container-toolkit 之後,也沒有 nvidia-docker 這個 file, 因爲是用` --gpus all ` 就相同了

```
docker run --gpus all  --shm-size=1g --ulimit memlock=-1 --ulimit stack=67108864 --rm -it -p 7777:8888 -v /home/tom/work/python_projects/LDL:/home/LDL nvcr.io/nvidia/tensorflow:21.07-tf2-py3 /bin/bash
```

# matplot & numpy
==Debian 12==
開起來後，下命令安裝 _matplotlib_, _numpy_

```bash ln:false
pip install matplotlib==3.6
pip install numpy==1.19.2
```

> [!NOTE] 這個 docker 需要 NumPy 1.19.2
> https://matplotlib.org/devdocs/devel/min_dep_policy.html
這裏 `matplotlib`3.6 對應 `NumPy` 是 1.19 才符合 TF 的需求

==Ubuntu 22.04==
docker 內提供 _tensorflow 2.5.0+nv_  , _numpy 1.19.5_, _matplotlib-inline             0.1.2_
不用另外安裝

# jupyter notebook
docker 的 bash 裏面可以跑 notebook：

```bash ln:false
cd /home/LDL

jupyter notebook --ip 0.0.0.0 --port 8888 --allow-root
```

畫面如下：
```
root@b8a4ffb7d8f6:/workspace# jupyter notebook --ip 0.0.0.0 --port 8888 --allow-root
[I 13:32:48.901 NotebookApp] Writing notebook server cookie secret to /root/.local/share/jupyter/runtime/notebook_cookie_secret
[I 13:32:49.335 NotebookApp] jupyter_tensorboard extension loaded.
[I 13:32:49.530 NotebookApp] JupyterLab extension loaded from /usr/local/lib/python3.8/dist-packages/jupyterlab
[I 13:32:49.530 NotebookApp] JupyterLab application directory is /usr/local/share/jupyter/lab
[I 13:32:49.532 NotebookApp] [Jupytext Server Extension] NotebookApp.contents_manager_class is (a subclass of) jupytext.TextFileContentsManager already - OK
[I 13:32:49.532 NotebookApp] Serving notebooks from local directory: /workspace
[I 13:32:49.532 NotebookApp] Jupyter Notebook 6.2.0 is running at:
[I 13:32:49.532 NotebookApp] http://hostname:8888/?token=844d74014b2b8fe6d31eb3c909878aa3e8ea36ee8a9a800e
[I 13:32:49.532 NotebookApp] Use Control-C to stop this server and shut down all kernels (twice to skip confirmation).
[C 13:32:49.535 NotebookApp]

    To access the notebook, open this file in a browser:
        file:///root/.local/share/jupyter/runtime/nbserver-348-open.html
    Or copy and paste this URL:
        http://hostname:8888/?token=844d74014b2b8fe6d31eb3c909878aa3e8ea36ee8a9a800e


```
點選 http://hostname:8888/?token=844d74014b2b8fe6d31eb3c909878aa3e8ea36ee8a9a800e
會找不到網頁， 因爲前面要改爲 http://localhost:7777/?token=...

> [!Warning] Port
> Docker 裏面是用 8888 ，對應到我們 Host 的 7777，開頭有說明
