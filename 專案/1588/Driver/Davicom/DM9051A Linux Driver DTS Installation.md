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

##  DM9051_config.dts

``` c
// SPDX-license-Identifier: GPL-2.0-or-later
/*
 * Device tree overlay for the davicom dm9051 Ethernet Controller
 *
 * target: tag_spidevice --> spidev1
 * target: tag_spi0 --> spi0
 * target: tag_gpio --> gpio
 *
 */

/dts-v1/;
/plugin/;

/ {
	compatible = "brcm,bcm2835";
	// tested in Raspberry Pi Device

	fragment@0 {
		target = <&spi0>;
		__overlay__ {
			status = "okay";
			#address-cells = <0x01>;
			#size-cells = <0x00>;

			netdev: dm9051@1 { /* device_spi */
				compatible = "davicom,dm9051";
				pinctrl-names = "default";
				pinctrl-0 = <&intc>;
				interrupt-parent = <&gpio>;
				interrupts = <26 8>;
				reg = <0x01>; /* CS1: rpi GPIO7 */
				spi-max-frequency = <31200000>;
				status = "okay";
			};
		};
	};

	fragment@1 {
		target = <&gpio>;
		__overlay__ {
			intc: device_gpio { /* device_gpio */
				brcm,pins = <26>;
				brcm,function = <0>;
				brcm,pull = <0>;
				interrupt-controller;
				#interrupt-cells = <2>;
				#address-cells = <0x01>;
			};
		};
	};

	/* In Raspberry Pi OS, In config.txt, use overlay or by default. */
	/* dtoverlay=dm9051_conf,speed=31200000,int=26 */
	/* dtoverlay=dm9051_conf */

	__overrides__ {
		speed = <&netdev>, "spi-max-frequency:0";
		int = <&netdev>,"interrupts:0",
		      <&intc>, "brcm,pins:0";
	};
};
```