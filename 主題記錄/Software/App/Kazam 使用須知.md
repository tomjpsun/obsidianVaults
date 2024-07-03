Kazam 是 Linux 上好用的錄影工具，簡單易用。

首先要記得是 *如何結束* ！對！因爲常常發生錄影後 Kazam 面板不知道如何叫出來的窘境～結果影片還要後處理才能看。

先上 Kazam 的 Hot-keys:

- **開始錄影 (Recording)**：Super+Ctrl+R
- **暫停、恢復錄影 (Pause)**：Super+Ctrl+P
- **完成錄影 (Finish)**：Super+Ctrl+F
- **結束 Kazam (Quit)**：Super+Ctrl+Q

## 無法在 Windows 上播放

另外一個遇到的麻煩是，我錄出來的 mp4 影片在 Windows 上不能播放，也不能說是完全不能播放，如果是用 Chrome 可以播，但若改用其他的播放器就不行了。後來找到 StackExchange 發現相同的[問題](https://video.stackexchange.com/questions/20162/convert-kazam-video-file-to-a-file-playable-in-windows-media-player)，根據下面的回答，可以使用 ==fmpeg== 來轉檔，指令如下：

``` bash
$ sudo apt install ffmpeg
$ ffmpeg -i in.mp4 -pix_fmt yuv420p -c:a copy -movflags +faststart out.mp4
```

原文參考：
https://cynthiachuang.github.io/Kazam-A-Record-Screen-on-Ubuntu/
