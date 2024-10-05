# 問題陳述

因爲在 HP 系統裏面安裝兩個 Linux, 然後陰錯陽差地, 插排線的時候插錯硬碟,導致原本 UEFI 跳轉的地方跟原本不同, 引起 boot 失敗, BIOS 後就報錯說 *找不到開機系統* ,因此有這篇記錄.

# 解決方案 1

1  用[救援系統](https://www.system-rescue.org/Installing-SystemRescue-on-a-USB-memory-stick/)開機, 選單選擇 **找出硬碟裏面的作業系統, 啓用此系統**.
2  進入之前的 Linux 後, 使用 `grub-install` 重新安裝, 此時會覆寫 UEFI 讓它下次找得到我們的 Linux

# 解決方案 2
`Power On` ==>  `ESC`  進入 BIOS
![[image-1.jpg]]

選 BIOS 設定後:
![[image-2.jpg]]

選上面 `UEFI 驅動` tag, 選`協力廠商 ROM 管理` list
選`檔案` , 選 `/dist`, 選 `shim` 即可進入 Linux, 然後一樣, 使用 `grub-install` 重新安裝, 連同 UEFI 一起覆寫入正確的開機檔案.



