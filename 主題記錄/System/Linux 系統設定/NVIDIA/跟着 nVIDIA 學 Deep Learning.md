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
```bash ln:false
pip install matplotlib==3.6
pip install numpy==1.19.2
```

> [!NOTE] 這個 docker 的相容 NumPy 
> docker 裏面是 Python 3.8 根據：
> https://matplotlib.org/devdocs/devel/min_dep_policy.html
> 
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

# 讀不到檔案...
### 如果用 Google Colab 

左邊可以上傳 **glove.6B.100d.txt** 給 `colab 的 /content/sample_data`
右邊就修改到 `/content/sample_data/glove.6B.100d.txt`

![[attachments/螢幕快照 2025-02-09 17-44-09.png]]

### 如果用 local docker 
以 6-2-glove_embeddings 爲例子， read_embeddings() 要開啓 `./glove.6B.100d.txt`


```python
import numpy as np
import scipy.spatial

# 定義一個載入詞向量的函式
def read_embeddings():
    FILE_NAME = './glove.6B.100d.txt'
    embeddings = {}
    file = open(FILE_NAME, 'r', encoding='utf-8')
    for line in file:
        values = line.split()
        word = values[0]
        vector = np.asarray(values[1:],
                            dtype='float32')
        embeddings[word] = vector
    file.close()
    print('Read %s embeddings.' % len(embeddings))
    return embeddings
read_embeddings()
```

意思就是在 6-2-glove_embeddings.ipynb 同目錄下，有 glove.6B.100d.txt 這個檔案：

[[主題記錄/System/Linux 系統設定/NVIDIA/attachments/20fb7b7e1fbe699476c346ac01807e68_MD5.jpeg|Open: Pasted image 20250209173903.png]]
![[主題記錄/System/Linux 系統設定/NVIDIA/attachments/20fb7b7e1fbe699476c346ac01807e68_MD5.jpeg]]
### 6-1-autocomplete_embedding 降版本

實際發現經過以上 docker 後，不需要降版本就可以執行：
[[主題記錄/System/Linux 系統設定/NVIDIA/attachments/280b09927cf25580602f1c66cb034e03_MD5.jpeg|Open: Pasted image 20250209224542.png]]
![[主題記錄/System/Linux 系統設定/NVIDIA/attachments/280b09927cf25580602f1c66cb034e03_MD5.jpeg]]

