User space 驅動程式（User Space Driver）是指運行在使用者空間中的驅動程式，與傳統的核心空間驅動程式相比，它具有更高的靈活性和安全性。下面是 User Space Driver 架構的主要組件和特點：

### 架構組件

1. **使用者空間應用程式（User Space Application）**：

    - 這是最終使用驅動程式的應用程式。它通過標準的系統呼叫（如 `open`、`read`、`write` 等）與使用者空間驅動程式進行交互。
2. **使用者空間驅動程式（User Space Driver）**：

    - 這部分代碼運行在使用者空間，負責管理和控制硬體設備。它通過系統呼叫與核心通信，並通過設備檔案與使用者空間應用程式通信。
3. **設備檔案（Device File）**：

    - 這是位於 `/dev` 目錄下的特殊檔案，使用者空間應用程式通過它們與驅動程式進行交互。例如，通過讀取或寫入設備檔案，可以與底層硬體通信。
4. **核心模組（Kernel Module）**：

    - 這些模組提供了必要的核心接口，使使用者空間驅動程式能夠與硬體設備通信。它們通常包括設備節點的創建和基本的 I/O 操作的支持。
5. **使用者空間與核心空間通信機制**：

    - 一些常用的通信機制包括 `ioctl`（I/O 控制）、`mmap`（記憶體映射）、`sysfs`（檔案系統接口）以及使用專用的中間庫（如 `libusb`）。

### 主要特點

1. **安全性**：

    - 使用者空間驅動程式運行在使用者模式下，這意味著如果驅動程式出現故障，不會影響整個系統的穩定性，減少了核心崩潰的風險。
2. **開發與調試便捷**：

    - 在使用者空間中開發和調試驅動程式比在核心空間中更容易。開發者可以使用標準的調試工具（如 GDB），並且不需要頻繁地重啟核心來測試代碼。
3. **性能**：

    - 雖然使用者空間驅動程式的性能可能稍遜於核心空間驅動程式，但對於大多數應用場景，性能差異是可以接受的，特別是在非實時性要求較高的應用中。
4. **可移植性**：

    - 使用者空間驅動程式更容易移植到不同的操作系統和硬體平台上，因為它們與核心接口的耦合度較低。

### 範例：UIO（Userspace I/O）

Linux 核心中提供了一種框架稱為 UIO，專門用於開發使用者空間驅動程式。以下是 UIO 的基本流程：

1. **核心模組**：

    - 編寫一個簡單的核心模組，註冊設備，並通過 `uio_register_device` 函數將設備信息傳遞給使用者空間。
2. **使用者空間驅動**：

    - 使用者空間的驅動程式可以直接通過 `/dev/uioX` 設備檔案訪問硬體設備。
3. **應用程式**：

    - 使用者空間應用程式通過標準檔案操作函數訪問 `/dev/uioX` 設備檔案，並可以通過 `ioctl` 或 `mmap` 進行更多的操作。

```c
#include <linux/kernel.h>
#include <linux/uio_driver.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>

static struct uio_info uio_info = {
    .name = "example_uio",
    .version = "1.0",
    .irq = UIO_IRQ_NONE,
};

static int example_probe(struct platform_device *pdev)
{
    int ret;
    struct resource *res;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
        dev_err(&pdev->dev, "No memory resource\n");
        return -ENODEV;
    }

    uio_info.mem[0].addr = res->start;
    uio_info.mem[0].size = resource_size(res);
    uio_info.mem[0].memtype = UIO_MEM_PHYS;

    ret = uio_register_device(&pdev->dev, &uio_info);
    if (ret) {
        dev_err(&pdev->dev, "Unable to register UIO device\n");
        return ret;
    }

    return 0;
}

static int example_remove(struct platform_device *pdev)
{
    uio_unregister_device(&uio_info);
    return 0;
}

static struct platform_driver example_driver = {
    .probe = example_probe,
    .remove = example_remove,
    .driver = {
        .name = "example_uio",
        .owner = THIS_MODULE,
    },
};

module_platform_driver(example_driver);

MODULE_AUTHOR("Your Name");
MODULE_LICENSE("GPL");

```

通過這種方式，使用者空間驅動程式可以在應用程式中以更安全和高效的方式與硬體設備進行交互，同時保持系統的穩定性和靈活性。


# 開發環境建置

## 安裝需要的 packages
```
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r)
```
## Makefile

檔名一定要是 `Makefile` 不是 `makefile` 哦！
```shell
obj-m += my_uio_driver.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

## 操作畫面
```bash
tom@lambda:~/work/uio_driver$ make
make -C /lib/modules/6.1.0-22-amd64/build M=/home/tom/work/uio_driver modules
make[1]: 進入目錄「/usr/src/linux-headers-6.1.0-22-amd64」
  CC [M]  /home/tom/work/uio_driver/my_uio_driver.o
  MODPOST /home/tom/work/uio_driver/Module.symvers
  CC [M]  /home/tom/work/uio_driver/my_uio_driver.mod.o
  LD [M]  /home/tom/work/uio_driver/my_uio_driver.ko
  BTF [M] /home/tom/work/uio_driver/my_uio_driver.ko
Skipping BTF generation for /home/tom/work/uio_driver/my_uio_driver.ko due to unavailability of vmlinux
make[1]: 離開目錄「/usr/src/linux-headers-6.1.0-22-amd64」
tom@lambda:~/work/uio_driver$ ls
makefile~      Module.symvers	 my_uio_driver.ko     my_uio_driver.mod.o
Makefile       my_uio_driver.c	 my_uio_driver.mod    my_uio_driver.o
modules.order  my_uio_driver.c~  my_uio_driver.mod.c
```
那個 BTF 警告是說找不到 vmlinx 未壓縮的檔案，以產生 debug 相關的東西，並不會影響我們的 driver 正確性。

