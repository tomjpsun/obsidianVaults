[參考文件](https://shengyu7697.github.io/git-format-patch/)

如果需要 sync 兩個 git repo

在 source git repo 這樣匯出
```bash
git format-patch [from commit]..[to commit] -o /tmp/patches
```

在 target git repo 這樣匯入

``` bash
git am --abort # 假如之前匯入失敗的話 可以使用 git am --abort 取消之前的匯入  
git am 0001-xxx.patch # 匯入 patch 檔案  
# or   
git am -s 0001-xxx.patch # 匯入 patch 檔案並加上 sign off 簽字
```
 
 git 匯入指定目錄下的 patch，  

```bash
git am /home/shengyu/Download/
```
