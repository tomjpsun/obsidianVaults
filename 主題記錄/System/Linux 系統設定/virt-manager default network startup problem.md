
 [Question from SO](https://askubuntu.com/questions/tagged/kvm-virtualization "show questions tagged 'kvm-virtualization'")

After first shutdown I can't start it. I get this error message:

```
Error starting domain: Requested operation is not valid: network 'default' is not active

Traceback (most recent call last):
  File "/usr/share/virt-manager/virtManager/asyncjob.py", line 89, in cb_wrapper
    callback(asyncjob, *args, **kwargs)
  File "/usr/share/virt-manager/virtManager/asyncjob.py", line 125, in tmpcb
    callback(*args, **kwargs)
  File "/usr/share/virt-manager/virtManager/libvirtobject.py", line 82, in newfn
```

### solution
```bash
sudo virsh net-list --all
# 沒有i看到任何裝置
sudo virsh net-start default
sudo virsh net-list --all
 名稱      狀態   自動啟動   Persistent
-----------------------------------------
 default   啟用   yes        yes

```

