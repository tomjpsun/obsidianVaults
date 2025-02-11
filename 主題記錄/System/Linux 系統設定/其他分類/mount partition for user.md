#ACL

# 摘要
如果要把新增的 partition mount 給 user : tom , 可以透過 ACL 管理, 這裏介紹設定過程.
# Setup
確認你的系統支持 ACL：
```
sudo apt install acl
```

修改掛載選項，在 /etc/fstab 中添加 acl：
```
UUID=9876-5432-10fe-dcba-876543210fed /mnt/new_partition ext4 defaults,acl 0 2
```

重新掛載分區：
```
sudo mount -o remount /mnt/new_partition
```

設置 ACL 權限：以我們的例子,要把下面 username 換成 tom
```
sudo setfacl -R -m u:username:rwx /mnt/new_partition
```
結論: 使用 ACL 可以更靈活地設置權限，而不影響其他用戶。



