因爲我們是小改, 所以採用 `.dtbo` (設備樹覆蓋文件) 的方式, 產生過程如下:

## 編譯 .dts 文件為 .dtbo
```bash
dtc -O dtb -o your_overlay.dtbo -b 0 -@ your_overlay.dts
```

`-O dtb`：指定輸出格式為 .dtbo。
`-o your_overlay.dtbo`：指定輸出文件名稱。
`-b 0`：這裡的 0 指定字節順序為原始的（無 byte swapping）。
`-@`：告訴編譯器包含符號（symbols），這對覆蓋文件很重要。

## 將 .dtbo 拷貝到目標設備
```
scp  your_overlay.dtbo  /boot/overlays/

```

## 配置覆蓋文件
修改 Bootloader 配置，例如在 Raspberry Pi 中修改 /boot/config.txt：

```
dtoverlay=your_overlay_name
```

your_overlay_name 不需要加 .dtbo 後綴，Bootloader 會自動查找該名稱的 .dtbo 文件。