# RaspBerry Pi OS information

> uname -a
Linux raspberrypi 6.6.31+rpt-rpi-v8 #1 SMP PREEMPT Debian 1:6.6.31-1+rpt1 (2024-05-29) aarch64 GNU/Linux

> echo $XDG_SESSION_TYPE
> wayland

這個版本 display manager 採用 `wayland`，以前在 x11 的截圖工具，沒有移植的話，是無法使用的，以下介紹在 `wayland`可以使用的工具：
# screen shot
按 **PrnScr** 按鍵，它會呼叫 `grim` 指令，做全螢幕的 screen shot，產生圖檔放在該 user 的 **Pictures** 目錄下。

# screen capture
打開 console 下指令 `wf-recorder` 可以開始 錄製螢幕，按 Ctrl-C 停止錄製，產生 .mp4 放在 user 目錄下

==finish==

