在 run  handson-ml2 的 docker 時發生這個訊息：
![[assets/2023-12-12 11-26-17 的螢幕擷圖.png]]

原因在這裏 https://stackoverflow.com/questions/75614728/cuda-12-tf-nightly-2-12-could-not-find-cuda-drivers-on-your-machine-gpu-will

arivero 的回覆裏面，提到 nvidia 的 docker 的 cuda 12 與 tensorflow 整合有問題，但是這是警告訊息，python program 還是可以跑.