當 make kernel driver 時遇到兩個問題: 
The target kernel has CONFIG_MODULE_SIG_ALL enabled, but the ==signing key cannot be found==. Module signing has been **disabled** for this build. 
make\[1]: Entering directory '/usr/src/linux-headers-6.10.11+bpo-common' 
make\[2]: Entering directory '/usr/src/linux-headers-6.10.11+bpo-amd64' LD 
\[M] /home/tom/work/ethernet-linux-igb/src/igb.ko BTF 
\[M] /home/tom/work/ethernet-linux-igb/src/igb.ko ==Skipping BTF generation for /home/tom/work/ethernet-linux-igb/src/igb.ko due to unavailability of vmlinux==

# 模組 signing (GPT 答案, 沒有驗證過)
1. 確認是否真的需要模組簽名
如果你只是開發或測試內核驅動，而不是正式部署，這個警告可以忽略，模組仍然可以載入，除非你的系統開啟了 Secure Boot（安全啟動）。

2. 產生或提供簽名金鑰（如果需要）
如果你的系統需要簽名模組，則你需要產生新的簽名金鑰：

cd /usr/src/linux-headers-$(uname -r)
openssl req -new -x509 -newkey rsa:2048 -keyout certs/signing_key.pem \
-out certs/signing_key.x509 -days 365 -nodes -subj "/CN=Module Signing Key"
然後重新編譯你的驅動程式。

3. 停用模組簽名檢查
如果你只是想略過這個問題，可以在開機時禁用模組簽名驗證：

執行以下命令：
```
sudo grubby --update-kernel=ALL --args="module.sig_unenforce"
```
或手動編輯 /etc/default/grub，找到這一行：
```
GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"
```
修改為：
```
GRUB_CMDLINE_LINUX_DEFAULT="quiet splash module.sig_unenforce"
```
然後更新 GRUB：
```
sudo update-grub
```
重新開機後，系統就不會強制要求模組簽名了。


# 修復 Skipping BTF generation 的問題

這個錯誤表示你的系統缺少 `vmlinux`，導致 BTF（BPF Type Format）無法生成。可以嘗試安裝 `linux-image-*-dbg` 來解決：
`sudo apt install linux-image-$(uname -r)-dbg`
這樣就有 debug symbol 的 vmlinuz 了, 然後 link 這個檔案
`ln -s /boot/vmlinuz-$(uname -r) /usr/src/linux-headers-$(uname -r)/vmlinuz`

或者，手動生成 `vmlinux`：

`sudo bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h`

如果你是自己編譯的內核，請確認 `CONFIG_DEBUG_INFO_BTF=y` 已啟用。

---

