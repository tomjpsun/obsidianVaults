
# [譯] Linux 非同步 I/O 框架 io_uring：基本原理、程序示例與性能壓測（2020）

Published at 2021-09-01 | Last Update 2021-11-19

### 譯者序

本文組合翻譯了以下兩篇文章的乾貨部分，作為 `io_uring` 相關的入門參考：

- [How io_uring and eBPF Will Revolutionize Programming in Linux](https://thenewstack.io/how-io_uring-and-ebpf-will-revolutionize-programming-in-linux), ScyllaDB, 2020
- [An Introduction to the io_uring Asynchronous I/O Framework](https://medium.com/oracledevs/an-introduction-to-the-io-uring-asynchronous-i-o-framework-fad002d7dfc1), Oracle, 2020

`io_uring` 是 2019 年 **==Linux 5.1==** 核心首次引入的高性能 **==非同步 I/O 框架==**，能顯著加速 I/O 密集型應用的性能。 但如果你的應用**==已經在使用==** 傳統 Linux AIO 了，**==並且使用方式恰當==**， 那 `io_uring` **==並不會帶來太大的性能提升==** —— 根據原文測試（以及我們 自己的復現），即便打開高級特性，也只有 5%。除非你真的需要這 5% 的額外性能，否則 **==切換==**成 `io_uring` **==代價可能也挺大==**，因為要 **==重寫應用==**來適配 `io_uring`（或者讓依賴的平台或框架去適配，總之需要改程式碼）。

既然性能跟傳統 AIO 差不多，那為什麼還稱 `io_uring` 為革命性技術呢？

1. 它首先和最大的貢獻在於：**==統一了 Linux 非同步 I/O 框架==**，

    - Linux AIO **==只支援 direct I/O==** 模式的**==儲存檔案==** （storage file），而且主要用在**==資料庫這一細分領域==**；
    - `io_uring` 支援儲存檔案和網路檔案（network sockets），也支援更多的非同步系統呼叫 （`accept/openat/stat/...`），而非僅限於 `read/write` 系統呼叫。
2. 在**==設計上是真正的非同步 I/O==**，作為對比，Linux AIO 雖然也 是非同步的，但仍然可能會阻塞，某些情況下的行為也無法預測；

    似乎之前 Windows 在這塊反而是領先的，更多參考：

    - [淺析開放原始碼專案之 io_uring](https://zhuanlan.zhihu.com/p/361955546)，“分步試儲存”專欄，知乎
    - [Is there really no asynchronous block I/O on Linux?](https://stackoverflow.com/questions/13407542/is-there-really-no-asynchronous-block-i-o-on-linux)，stackoverflow
3. **==靈活性和可擴展性==**非常好，甚至能基於 `io_uring` 重寫所有系統呼叫，而 Linux AIO 設計時就沒考慮擴展性。


eBPF 也算是非同步框架（事件驅動），但與 `io_uring` 沒有本質聯絡，二者屬於不同子系統， 並且在模型上有一個本質區別：

1. **==eBPF 對使用者是透明的==**，只需升級核心（到合適的版本），**==應用程式無需任何改造==**；
2. `io_uring` 提供了**==新的系統呼叫和使用者空間 API==**，因此**==需要應用程式做改造==**。

eBPF 作為動態跟蹤工具，能夠更方便地排查和觀測 `io_uring` 等模組在執行層面的具體問題。

本文介紹 Linux 非同步 I/O 的發展歷史，`io_uring` 的原理和功能， 並給出了一些**==程序示例==**和**==性能壓測==**結果（我們在 5.10 核心做了類似測試，結論與原文差不多）。

> Ceph 程式碼上已經支援了 `io_uring`，但發行版在編譯時沒有打開這個組態，判斷是否支援 io_uring [直接返回的 `false`](https://github.com/ceph/ceph/blob/a67d1cf2a7a4031609a5d37baa01ffdfef80e993/src/blk/kernel/io_uring.cc#L256)， 因此想測試得自己重新編譯。測試時的參考組態：
>
> ```
> $ cat /etc/ceph/ceph.conf
> [osd]
> bluestore_ioring = true
> ...
> ```
>
> 確認組態生效（這是只是隨便挑一個 OSD）：
>
> ```
> $ ceph config show osd.16 | grep ioring
> bluestore_ioring                       true                                            file
> ```
>
> 還要去看下日誌，是否因為檢測 io_uring 失敗而 fallback 回了 libaio。

**由於譯者水平有限，本文不免存在遺漏或錯誤之處。如有疑問，請查閱原文。**

以下是譯文。

---

- [譯者序](http://arthurchiao.art/blog/intro-to-io-uring-zh/#译者序)
- [1 Linux I/O 系統呼叫演進](http://arthurchiao.art/blog/intro-to-io-uring-zh/#1-linux-io-系统调用演进)
    - [1.1 基於 fd 的阻塞式 I/O：`read()/write()`](http://arthurchiao.art/blog/intro-to-io-uring-zh/#11-基于-fd-的阻塞式-ioreadwrite)
    - [1.2 非阻塞式 I/O：`select()/poll()/epoll()`](http://arthurchiao.art/blog/intro-to-io-uring-zh/#12-非阻塞式-ioselectpollepoll)
    - [1.3 執行緒池方式](http://arthurchiao.art/blog/intro-to-io-uring-zh/#13-线程池方式)
    - [1.4 Direct I/O（資料庫軟體）：繞過 page cache](http://arthurchiao.art/blog/intro-to-io-uring-zh/#14-direct-io数据库软件绕过-page-cache)
    - [1.5 非同步 IO（AIO）](http://arthurchiao.art/blog/intro-to-io-uring-zh/#15-异步-ioaio)
    - [1.6 小結](http://arthurchiao.art/blog/intro-to-io-uring-zh/#16-小结)
- [2 `io_uring`](http://arthurchiao.art/blog/intro-to-io-uring-zh/#2-io_uring)
    - [2.1 與 Linux AIO 的不同](http://arthurchiao.art/blog/intro-to-io-uring-zh/#21-与-linux-aio-的不同)
    - [2.2 原理及核心資料結構：SQ/CQ/SQE/CQE](http://arthurchiao.art/blog/intro-to-io-uring-zh/#22-原理及核心数据结构sqcqsqecqe)
    - [2.3 帶來的好處](http://arthurchiao.art/blog/intro-to-io-uring-zh/#23-带来的好处)
    - [2.4 三種工作模式](http://arthurchiao.art/blog/intro-to-io-uring-zh/#24-三种工作模式)
    - [2.5 `io_uring` 系統呼叫 API](http://arthurchiao.art/blog/intro-to-io-uring-zh/#25-io_uring-系统调用-api)
        - [2.5.1 `io_uring_setup()`](http://arthurchiao.art/blog/intro-to-io-uring-zh/#251-io_uring_setup)
        - [2.5.2 `io_uring_register()`](http://arthurchiao.art/blog/intro-to-io-uring-zh/#252-io_uring_register)
            - [註冊的緩衝區（buffer）性質](http://arthurchiao.art/blog/intro-to-io-uring-zh/#注册的缓冲区buffer性质)
            - [通過 `eventfd()` 訂閱 completion 事件](http://arthurchiao.art/blog/intro-to-io-uring-zh/#通过-eventfd-订阅-completion-事件)
        - [2.5.3 `io_uring_enter()`](http://arthurchiao.art/blog/intro-to-io-uring-zh/#253-io_uring_enter)
    - [2.6 高級特性](http://arthurchiao.art/blog/intro-to-io-uring-zh/#26-高级特性)
    - [2.7 使用者空間庫 `liburing`](http://arthurchiao.art/blog/intro-to-io-uring-zh/#27-用户空间库-liburing)
- [3 基於 liburing 的示例應用](http://arthurchiao.art/blog/intro-to-io-uring-zh/#3-基于-liburing-的示例应用)
    - [3.1 `io_uring-test`](http://arthurchiao.art/blog/intro-to-io-uring-zh/#31-io_uring-test)
        - [原始碼及註釋](http://arthurchiao.art/blog/intro-to-io-uring-zh/#源码及注释)
        - [其他說明](http://arthurchiao.art/blog/intro-to-io-uring-zh/#其他说明)
    - [3.2 `link-cp`](http://arthurchiao.art/blog/intro-to-io-uring-zh/#32-link-cp)
        - [I/O chain](http://arthurchiao.art/blog/intro-to-io-uring-zh/#io-chain)
        - [原始碼及註釋](http://arthurchiao.art/blog/intro-to-io-uring-zh/#源码及注释-1)
        - [其他說明](http://arthurchiao.art/blog/intro-to-io-uring-zh/#其他说明-1)
- [4 `io_uring` 性能壓測（基於 `fio`）](http://arthurchiao.art/blog/intro-to-io-uring-zh/#4-io_uring-性能压测基于-fio)
    - [4.1 測試環境](http://arthurchiao.art/blog/intro-to-io-uring-zh/#41-测试环境)
    - [4.2 場景一：direct I/O `1KB` 隨機讀（繞過 page cache）](http://arthurchiao.art/blog/intro-to-io-uring-zh/#42-场景一direct-io-1kb-随机读绕过-page-cache)
    - [4.2 場景二：buffered I/O `1KB` 隨機讀（資料提前載入到記憶體，100% hot cache）](http://arthurchiao.art/blog/intro-to-io-uring-zh/#42-场景二buffered-io-1kb-随机读数据提前加载到内存100-hot-cache)
    - [4.3 性能測試小結](http://arthurchiao.art/blog/intro-to-io-uring-zh/#43-性能测试小结)
    - [4.4 ScyllaDB 與 io_uring](http://arthurchiao.art/blog/intro-to-io-uring-zh/#44-scylladb-与-io_uring)
- [5 eBPF](http://arthurchiao.art/blog/intro-to-io-uring-zh/#5-ebpf)
- [6 結束語](http://arthurchiao.art/blog/intro-to-io-uring-zh/#6-结束语)
- [參考資料](http://arthurchiao.art/blog/intro-to-io-uring-zh/#参考资料)

---

很多人可能還沒意識到，Linux 核心在過去幾年已經發生了一場革命。這場革命源於 **==兩個激動人心的新介面==**的引入：**==eBPF 和 io_uring==**。 我們認為，二者將會完全**==改變應用與核心互動的方式==**，以及 **==應用開發者思考和看待核心的方式==**。

本文介紹 io_uring（我們在 ScyllaDB 中有 io_uring 的深入使用經驗），並略微提及一下 eBPF。

# 1 Linux I/O 系統呼叫演進

## 1.1 基於 fd 的阻塞式 I/O：`read()/write()`

作為大家最熟悉的讀寫方式，Linux 核心提供了**==基於檔案描述符的系統呼叫==**， 這些描述符指向的可能是**==儲存檔案==**（storage file），也可能是 **==network sockets==**：

```
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```

二者稱為**==阻塞式系統呼叫==**（blocking system calls），因為程序呼叫 這些函數時會進入 sleep 狀態，然後被調度出去（讓出處理器），直到 I/O 操作完成：

- 如果資料在檔案中，並且檔案內容**==已經快取在 page cache 中==**，呼叫會**==立即返回==**；
- 如果資料在另一台機器上，就需要通過網路（例如 TCP）獲取，會阻塞一段時間；
- 如果資料在硬碟上，也會阻塞一段時間。

但很容易想到，隨著儲存**==裝置越來越快，程序越來越複雜==**， 阻塞式（blocking）已經這種最簡單的方式已經不適用了。

## 1.2 非阻塞式 I/O：`select()/poll()/epoll()`

阻塞式之後，出現了一些新的、非阻塞的系統呼叫，例如 `select()`、`poll()` 以及更新的 `epoll()`。 應用程式在呼叫這些函數讀寫時不會阻塞，而是**==立即返回==**，返回的是一個 **==已經 ready 的檔案描述符列表==**。

![](http://arthurchiao.art/assets/img/intro-to-io-uring/epoll.png)

但這種方式存在一個致命缺點：**==只支援 network sockets 和 pipes==** —— `epoll()` 甚至連 storage files 都不支援。

## 1.3 執行緒池方式

對於 storage I/O，經典的解決思路是 [thread pool](https://en.wikipedia.org/wiki/Thread_pool)： 主執行緒將 I/O 分發給 worker 執行緒，後者代替主執行緒進行阻塞式讀寫，主執行緒不會阻塞。

![](http://arthurchiao.art/assets/img/intro-to-io-uring/thread-pools.png)

這種方式的問題是**==執行緒上下文切換開銷可能非常大==**，後面性能壓測會看到。

## 1.4 Direct I/O（資料庫軟體）：繞過 page cache

隨後出現了更加靈活和強大的方式：**==資料庫軟體==**（database software） 有時 [==並不想使用作業系統的 page cache==](https://www.scylladb.com/2018/07/26/how-scylla-data-cache-works)， 而是希望打開一個檔案後，**==直接從裝置讀寫這個檔案==**（direct access to the device）。 這種方式稱為**==直接訪問==**（direct access）或**==直接 I/O==**（direct I/O），

- 需要指定 **==`O_DIRECT`==** flag；
- 需要**==應用自己管理自己的快取==** —— 這正是資料庫軟體所希望的；
- 是 **==zero-copy I/O==**，因為應用的緩衝資料直接傳送到裝置，或者直接從裝置讀取。

## 1.5 非同步 IO（AIO）

前面提到，隨著儲存裝置越來越快，主執行緒和 worker 線性之間的上下文切換開銷佔比越來越高。 現在市場上的一些裝置，例如 [Intel Optane](https://pcper.com/2018/12/intels-optane-dc-persistent-memory-dimms-push-latency-closer-to-dram) ，**==延遲已經低到和上下文切換一個量級==**（微秒 `us`）。換個方式描述， 更能讓我們感受到這種開銷： **==上下文每切換一次，我們就少一次 dispatch I/O 的機會==**。

因此，Linux **==2.6==** 核心引入了非同步 I/O（asynchronous I/O）介面， 方便起見，本文簡寫為 `linux-aio`。AIO **==原理==**是很簡單的：

- 使用者通過 `io_submit()` 提交 I/O 請求，
- 過一會再呼叫 `io_getevents()` 來檢查哪些 events 已經 ready 了。
- 使程式設計師**==能編寫完全非同步的程式碼==**。

近期，[Linux AIO 甚至支援了](https://lwn.net/Articles/742978/) `epoll()`：也就是說 不僅能提交 storage I/O 請求，還能提交網路 I/O 請求。照這樣發展下去，linux-aio **==似乎能成為一個王者==**。但由於它糟糕的演進之路，這個願望幾乎不可能實現了。 我們從 **==Linus 標誌性的激烈言辭中就能略窺一斑==**：

> Reply to: [to support opening files asynchronously](https://lwn.net/Articles/671657/)
>
> _So I think this is ridiculously ugly._
>
> _AIO is a horrible ad-hoc design, with the main excuse being “other, less gifted people, made that design, and we are implementing it for compatibility because database people — who seldom have any shred of taste — actually use it”._
>
> — Linus Torvalds (on lwn.net)

首先，作為資料庫從業人員，我們想借此機會為我們的沒品（lack of taste）向 Linus 道歉。 但更重要的是，我們要進一步解釋一下**==為什麼 Linus 是對的==**：Linux AIO 確實問題纏身，

1. **==只支援 `O_DIRECT` 檔案==**，因此**==對常規的非資料庫應用==** （normal, non-database applications）**==幾乎是無用的==**；
2. 介面在**==設計時並未考慮擴展性==**。雖然可以擴展 —— 我們也確實這麼做了 —— 但每加一個東西都相當複雜；
3. 雖然從**==技術上說介面是非阻塞的==**，但實際上有 [==很多可能的原因都會導致它阻塞==](https://lwn.net/Articles/724198)，而且引發的方式難以預料。

## 1.6 小結

以上可以清晰地看出 Linux I/O 的演進：

- 最開始是同步（阻塞式）系統呼叫；
- 然後隨著**==實際需求和具體場景==**，不斷加入新的非同步介面，還要保持與老介面的相容和協同工作。

另外也看到，在非阻塞式讀寫的問題上**==並沒有形成統一方案==**：

1. Network socket 領域：新增一個非同步介面，然後去輪詢（poll）請求是否完成（readiness）；
2. Storage I/O 領域：**==只針對某一細分領域==**（資料庫）在某一特定時期的需求，新增了一個定製版的非同步介面。

**==這就是 Linux I/O 的演進歷史==** —— 只著眼當前，出現一個問題就引入一種設計，而並沒有多少前瞻性 —— 直到 `io_uring` 的出現。

# 2 `io_uring`

io_uring 來自資深核心開發者 Jens Axboe 的想法，他在 Linux I/O stack 領域頗有研究。 從最早的 patch [aio: support for IO polling](https://lwn.net/ml/linux-fsdevel/20181221192236.12866-9-axboe@kernel.dk) 可以看出，這項工作始於一個很簡單的觀察：隨著裝置越來越快， **==中斷驅動（interrupt-driven）模式效率已經低於輪詢模式==** （polling for completions） —— 這也是高性能領域最常見的主題之一。

- `io_uring` 的**==基本邏輯與 linux-aio 是類似的==**：提供兩個介面，一個將 I/O 請求提交到核心，一個從核心接收完成事件。
- 但隨著開發深入，它逐漸變成了一個完全不同的介面：設計者開始從源頭思考 **==如何支援完全非同步的操作==**。

## 2.1 與 Linux AIO 的不同

`io_uring` 與 `linux-aio` 有著本質的不同：

1. **==在設計上是真正非同步的==**（truly asynchronous）。只要 設定了合適的 flag，它在**==系統呼叫上下文中就只是將請求放入佇列==**， 不會做其他任何額外的事情，**==保證了應用永遠不會阻塞==**。

2. **==支援任何類型的 I/O==**：cached files、direct-access files 甚至 blocking sockets。

    由於設計上就是非同步的（async-by-design nature），因此**==無需 poll+read/write 來處理 sockets==**。 只需提交一個阻塞式讀（blocking read），請求完成之後，就會出現在 completion ring。

3. **==靈活、可擴展==**：基於 `io_uring` 甚至能重寫（re-implement）Linux 的每個系統呼叫。


## 2.2 原理及核心資料結構：SQ/CQ/SQE/CQE

每個 io_uring 實例都有**==兩個環形佇列==**（ring），在核心和應用程式之間共享：

- **==提交佇列==**：submission queue (SQ)
- **==完成佇列==**：completion queue (CQ)

![](http://arthurchiao.art/assets/img/intro-to-io-uring/io_uring.png)

這兩個佇列：

- 都是**==單生產者、單消費者==**，size 是 2 的冪次；
- 提供**==無鎖介面==**（lock-less access interface），內部使用 **==記憶體屏障==**做同步（coordinated with memory barriers）。

**==使用方式==**：

- 請求

    - 應用建立 SQ entries (SQE)，更新 SQ tail；
    - 核心消費 SQE，更新 SQ head。
- 完成

    - 核心為完成的一個或多個請求建立 CQ entries (CQE)，更新 CQ tail；
    - 應用消費 CQE，更新 CQ head。
    - 完成事件（completion events）可能以任意順序到達，到總是與特定的 SQE 相關聯的。
    - 消費 CQE 過程無需切換到核心態。

## 2.3 帶來的好處

`io_uring` 這種請求方式還有一個好處是：原來需要多次系統呼叫（讀或寫），現在變成批處理一次提交。

還記得 Meltdown 漏洞嗎？當時我還寫了[一篇文章](https://www.scylladb.com/2018/01/07/cost-of-avoiding-a-meltdown/) 解釋為什麼我們的 Scylla NoSQL 資料庫受影響很小：`aio` 已經將我們的 I/O 系統呼叫批處理化了。

`io_uring` **==將這種批處理能力帶給了==** storage I/O 系統呼叫之外的 **==其他一些系統呼叫==**，包括：

- `read`
- `write`
- `send`
- `recv`
- `accept`
- `openat`
- `stat`
- 專用的一些系統呼叫，例如 `fallocate`

此外，`io_uring` 使非同步 I/O 的使用場景也不再僅限於資料庫應用，**==普通的 非資料庫應用也能用==**。這一點值得重複一遍：

> 雖然 `io_uring` 與 `aio` 有一些相似之處，但它的**==擴展性和架構是革命性的==**： 它**==將非同步操作的強大能力帶給了所有應用==**（及其開發者），而 **==不再僅限於是資料庫應用這一細分領域==**。

我們的 CTO Avi Kivity 在 the Core C++ 2019 event 上 [有一次關於 async 的分享](https://www.scylladb.com/2020/03/26/avi-kivity-at-core-c-2019)。 核心點包括：**==從延遲上來說==**，

1. 現代多核、多 CPU 裝置，其內部本身就是一個基礎網路；
2. **==CPU 之間==**是另一個網路；
3. **==CPU 和磁碟 I/O 之間==**又是一個網路。

因此網路程式設計採用非同步是明智的，而現在開發自己的應用也應該考慮非同步。 這**==從根本上改變了 Linux 應用的設計方式==**：

- 之前都是一段順序程式碼流，需要系統呼叫時才執行系統呼叫，
- 現在需要思考一個檔案是否 ready，因而自然地引入 event-loop，不斷通過共享 buffer 提交請求和接收結果。

## 2.4 三種工作模式

io_uring 實例可工作在三種模式：

1. **==中斷驅動模式==**（interrupt driven）

    **==默認模式==**。可通過 io_uring_enter() 提交 I/O 請求，然後直接檢查 CQ 狀態判斷是否完成。

2. **==輪詢模式==**（polled）

    Busy-waiting for an I/O completion，而不是通過非同步 IRQ（Interrupt Request）接收通知。

    這種模式需要檔案系統（如果有）和塊裝置（block device）支援輪詢功能。 相比中斷驅動方式，這種方式延遲更低（[連系統呼叫都省了](https://www.phoronix.com/scan.php?page=news_item&px=Linux-io_uring-Fast-Efficient)）， 但可能會消耗更多 CPU 資源。

    目前，只有指定了 O_DIRECT flag 打開的檔案描述符，才能使用這種模式。當一個讀 或寫請求提交給輪詢上下文（polled context）之後，應用（application）必須呼叫 `io_uring_enter()` 來輪詢 CQ 佇列，判斷請求是否已經完成。

    對一個 io_uring 實例來說，**==不支援混合使用輪詢和非輪詢模式==**。

3. **==核心輪詢模式==**（kernel polled）

    這種模式中，會 **==建立一個核心執行緒==**（kernel thread）來執行 SQ 的輪詢工作。

    使用這種模式的 io_uring 實例， **==應用無需切到到核心態==** 就能觸發（issue）I/O 操作。 通過 SQ 來提交 SQE，以及監控 CQ 的完成狀態，應用無需任何系統呼叫，就能提交和收割 I/O（submit and reap I/Os）。

    如果核心執行緒的空閒時間超過了使用者的組態值，它會通知應用，然後進入 idle 狀態。 這種情況下，應用必須呼叫 `io_uring_enter()` 來喚醒核心執行緒。如果 I/O 一直很繁忙，核心線性是不會 sleep 的。


## 2.5 `io_uring` 系統呼叫 API

有三個：

- `io_uring_setup(2)`
- `io_uring_register(2)`
- `io_uring_enter(2)`

下面展開介紹。完整文件見 [manpage](https://github.com/axboe/liburing/tree/master/man)。

### 2.5.1 `io_uring_setup()`

執行非同步 I/O 需要先**==設定上下文==**：

```
int io_uring_setup(u32 entries, struct io_uring_params *p);
```

這個系統呼叫

- **==建立一個 SQ 和一個 CQ==**，
- queue size 至少 `entries` 個元素，
- 返回一個檔案描述符，隨後用於在這個 io_uring 實例上執行操作。

SQ 和 CQ 在應用和核心之間共享，避免了在初始化和完成 I/O 時（initiating and completing I/O）複製資料。

參數 p：

- 應用用來組態 io_uring，
- 核心返回的 SQ/CQ 組態資訊也通過它帶回來。

io_uring_setup() 成功時返回一個檔案描述符（fd）。應用隨後可以將這個 fd 傳給 mmap(2) 系統呼叫，來 map the submission and completion queues 或者傳給 to the io_uring_register() or io_uring_enter() system calls.

### 2.5.2 `io_uring_register()`

註冊用於非同步 I/O 的**==檔案或使用者緩衝區==**（files or user buffers）：

```
int io_uring_register(unsigned int fd, unsigned int opcode, void *arg, unsigned int nr_args);
```

註冊檔案或使用者緩衝區，使核心能**==長時間持有對該檔案在核心內部的資料結構引用==**（internal kernel data structures associated with the files）， 或建立**==應用記憶體的長期對應==**（long term mappings of application memory associated with the buffers）， 這個操作只會在註冊時執行一次，而不是每個 I/O 請求都會處理，因此減少了 per-I/O overhead。

#### 註冊的緩衝區（buffer）性質

- Registered buffers 將會**==被鎖定在記憶體中==**（be locked in memory），並**==計入使用者的 RLIMIT_MEMLOCK==** 資源限制。
- 此外，每個 buffer 有 **==1GB 的大小限制==**。
- 當前，buffers 必須是**==匿名、非檔案後端的記憶體==**（anonymous, non-file-backed memory），例如 malloc(3) or mmap(2) with the MAP_ANONYMOUS flag set 返回的記憶體。
- Huge pages 也是支援的。整個 huge page 都會被 pin 到核心，即使只用到了其中一部分。
- 已經註冊的 buffer 無法調整大小，想調整隻能先 unregister，再重新 register 一個新的。

#### 通過 `eventfd()` 訂閱 completion 事件

可以用 `eventfd(2)` 訂閱 io_uring 實例的 completion events。 將 eventfd 描述符通過這個系統呼叫註冊就行了。

> The credentials of the running application can be registered with io_uring which returns an id associated with those credentials. Applications wishing to share a ring between separate users/processes can pass in this credential id in the SQE personality field. If set, that particular SQE will be issued with these credentials.

### 2.5.3 `io_uring_enter()`

```
int io_uring_enter(unsigned int fd, unsigned int to_submit, unsigned int min_complete, unsigned int flags, sigset_t *sig);
```

這個系統呼叫用於初始化和完成（initiate and complete）I/O，使用共享的 SQ 和 CQ。 單次呼叫同時執行：

1. 提交新的 I/O 請求
2. 等待 I/O 完成

參數：

1. `fd` 是 `io_uring_setup()` 返回的檔案描述符；
2. `to_submit` 指定了 SQ 中提交的 I/O 數量；
3. 依據不同模式：

    - 默認模式，如果指定了 `min_complete`，會等待這個數量的 I/O 事件完成再返回；
    - 如果 io_uring 是 polling 模式，這個參數表示：

        1. 0：要求核心返回當前以及完成的所有 events，無阻塞；
        2. 非零：如果有事件完成，核心仍然立即返回；如果沒有完成事件，核心會 poll，等待指定的次數完成，或者這個處理程序的時間片用完。

注意：對於 interrupt driven I/O，**==應用無需進入核心就能檢查 CQ 的 event completions==**。

`io_uring_enter()` 支援很多操作，包括：

- Open, close, and stat files
- Read and write into multiple buffers or pre-mapped buffers
- Socket I/O operations
- Synchronize file state
- Asynchronously monitor a set of file descriptors
- Create a timeout linked to a specific operation in the ring
- Attempt to cancel an operation that is currently in flight
- Create I/O chains
- Ordered execution within a chain
- Parallel execution of multiple chains

當這個系統呼叫返回時，表示一定數量的 SEQ 已經被消費和提交了，此時可以安全的重用佇列中的 SEQ。 此時 IO 提交有可能還停留在非同步上下文中，即實際上 SQE 可能還沒有被提交 —— 不過 使用者不用關心這些細節 —— 當隨後核心需要使用某個特定的 SQE 時，它已經複製了一份。

## 2.6 高級特性

`io_uring` 提供了一些用於特殊場景的高級特性：

1. **==File registration==**（檔案註冊）：每次發起一個指定檔案描述的操 作，核心都需要**==花費一些時鐘週期==**（cycles）**==將檔案描述符對應到內部表示==**。 對於那些**==針對同一檔案進行重複操作==**的場景，`io_uring` 支援**==提前註冊這些檔案==**，後面直接尋找就行了。
2. **==Buffer registration==**（緩衝區註冊）：與 file registration 類 似，direct I/O 場景中，核心需要 map/unmap memory areas。`io_uring` 支援提前 註冊這些緩衝區（buffers）。
3. **==Poll ring==**（輪詢環形緩衝區）：對於非常快是裝置，處理中斷的開 銷是比較大的。`io_uring` 允許使用者關閉中斷，使用輪詢模式。前面“三種工作模式”小節 也介紹到了這一點。
4. **==Linked operations==**（連結操作）：允許使用者傳送串聯的請求。這兩 個請求同時提交，但後面的會等前面的處理完才開始執行。

## 2.7 使用者空間庫 `liburing`

[`liburing`](https://github.com/axboe/liburing/) 提供了一個簡單的高層 API， 可用於一些基本場景，應用程式避免了直接使用更底層的系統呼叫。 此外，這個 API 還避免了一些重複操作的程式碼，如設定 io_uring 實例。

舉個例子，在 `io_uring_setup()` 的 manpage 描述中，呼叫這個系統呼叫獲得一個 ring 文 件描述符之後，應用必須呼叫 `mmap()` 來這樣的邏輯需要一段略長的程式碼，而用 `liburing` 的話，下面的函數已經將上述流程封裝好了：

```
int io_uring_queue_init(unsigned entries, struct io_uring *ring, unsigned flags);
```

下一節來看兩個例子基於 liburing 的例子。

# 3 基於 liburing 的示例應用

編譯：

```
$ git clone https://github.com/axboe/liburing.git
$ git co -b liburing-2.0 tags/liburing-2.0

$ cd liburing
$ ls examples/
io_uring-cp  io_uring-cp.c  io_uring-test  io_uring-test.c  link-cp  link-cp.c  Makefile  ucontext-cp  ucontext-cp.c

$ make -j4

$ ./examples/io_uring-test <file>
Submitted=4, completed=4, bytes=16384

$ ./examples/link-cp <in-file> <out-file>
```

## 3.1 `io_uring-test`

這個程序使用 4 個 SQE，從輸入檔案中**==讀取最多 16KB 資料==**。

### 原始碼及註釋

為方便看清主要邏輯，忽略了一些錯誤處理程式碼，完整程式碼見 [io_uring-test.c](https://github.com/axboe/liburing/blob/liburing-2.0/examples/io_uring-test.c)。

```
/* SPDX-License-Identifier: MIT */
/*
 * Simple app that demonstrates how to setup an io_uring interface,
 * submit and complete IO against it, and then tear it down.
 *
 * gcc -Wall -O2 -D_GNU_SOURCE -o io_uring-test io_uring-test.c -luring
 */
#include "liburing.h"

#define QD    4 // io_uring 佇列長度

int main(int argc, char *argv[]) {
    int i, fd, pending, done;
    void *buf;

    // 1. 初始化一個 io_uring 實例
    struct io_uring ring;
    ret = io_uring_queue_init(QD,    // 佇列長度
                              &ring, // io_uring 實例
                              0);    // flags，0 表示默認組態，例如使用中斷驅動模式

    // 2. 打開輸入檔案，注意這裡指定了 O_DIRECT flag，核心輪詢模式需要這個 flag，見前面介紹
    fd = open(argv[1], O_RDONLY | O_DIRECT);
    struct stat sb;
    fstat(fd, &sb); // 獲取檔案資訊，例如檔案長度，後面會用到

    // 3. 初始化 4 個讀緩衝區
    ssize_t fsize = 0;             // 程序的最大讀取長度
    struct iovec *iovecs = calloc(QD, sizeof(struct iovec));
    for (i = 0; i < QD; i++) {
        if (posix_memalign(&buf, 4096, 4096))
            return 1;
        iovecs[i].iov_base = buf;  // 起始地址
        iovecs[i].iov_len = 4096;  // 緩衝區大小
        fsize += 4096;
    }

    // 4. 依次準備 4 個 SQE 讀請求，指定將隨後讀入的資料寫入 iovecs
    struct io_uring_sqe *sqe;
    offset = 0;
    i = 0;
    do {
        sqe = io_uring_get_sqe(&ring);  // 獲取可用 SQE
        io_uring_prep_readv(sqe,        // 用這個 SQE 準備一個待提交的 read 操作
                            fd,         // 從 fd 打開的檔案中讀取資料
                            &iovecs[i], // iovec 地址，讀到的資料寫入 iovec 緩衝區
                            1,          // iovec 數量
                            offset);    // 讀取操作的起始地址偏移量
        offset += iovecs[i].iov_len;    // 更新偏移量，下次使用
        i++;

        if (offset > sb.st_size)        // 如果超出了檔案大小，停止準備後面的 SQE
            break;
    } while (1);

    // 5. 提交 SQE 讀請求
    ret = io_uring_submit(&ring);       // 4 個 SQE 一次提交，返回提交成功的 SQE 數量
    if (ret < 0) {
        fprintf(stderr, "io_uring_submit: %s\n", strerror(-ret));
        return 1;
    } else if (ret != i) {
        fprintf(stderr, "io_uring_submit submitted less %d\n", ret);
        return 1;
    }

    // 6. 等待讀請求完成（CQE）
    struct io_uring_cqe *cqe;
    done = 0;
    pending = ret;
    fsize = 0;
    for (i = 0; i < pending; i++) {
        io_uring_wait_cqe(&ring, &cqe);  // 等待系統返回一個讀完成事件
        done++;

        if (cqe->res != 4096 && cqe->res + fsize != sb.st_size) {
            fprintf(stderr, "ret=%d, wanted 4096\n", cqe->res);
        }

        fsize += cqe->res;
        io_uring_cqe_seen(&ring, cqe);   // 更新 io_uring 實例的完成佇列
    }

    // 7. 列印統計資訊
    printf("Submitted=%d, completed=%d, bytes=%lu\n", pending, done, (unsigned long) fsize);

    // 8. 清理工作
    close(fd);
    io_uring_queue_exit(&ring);
    return 0;
}
```

### 其他說明

程式碼中已經新增了註釋，這裡再解釋幾點：

- 每個 SQE 都執行一個 allocated buffer，後者是用 `iovec` 結構描述的；
- 第 3 & 4 步：初始化所有 SQE，用於接下來的 `IORING_OP_READV` 操作，後者 **==提供了 `readv(2)` 系統呼叫的非同步介面==**。
- 操作完成之後，這個 SQE iovec buffer 中存放的是相關 `readv` 操作的結果；
- 接下來呼叫 `io_uring_wait_cqe()` 來 reap CQE，並通過 `cqe->res` 欄位驗證讀取的位元組數；
- `io_uring_cqe_seen()` 通知核心這個 CQE 已經被消費了。

## 3.2 `link-cp`

link-cp 使用 io_uring 高級特性 SQE chaining 特性來複製檔案。

### I/O chain

io_uring 支援建立 I/O chain。一個 chain 內的 I/O 是順序執行的，多個 I/O chain 可以平行執行。

`io_uring_enter()` manpage 中對 `IOSQE_IO_LINK` 有 [詳細解釋](https://www.mankier.com/2/io_uring_enter#Description-IOSQE_IO_LINK)：

> When this flag is specified, it forms a link with the next SQE in the submission ring. That next SQE will not be started before this one completes. This, in effect, forms a chain of SQEs, which can be arbitrarily long. The tail of the chain is denoted by the first SQE that does not have this flag set. This flag has no effect on previous SQE submissions, nor does it impact SQEs that are outside of the chain tail. This means that multiple chains can be executing in parallel, or chains and individual SQEs. Only members inside the chain are serialized. A chain of SQEs will be broken, if any request in that chain ends in error. io_uring considers any unexpected result an error. This means that, eg, a short read will also terminate the remainder of the chain. If a chain of SQE links is broken, the remaining unstarted part of the chain will be terminated and completed with -ECANCELED as the error code. Available since 5.3.

為實現複製檔案功能，link-cp 建立一個長度為 2 的 SQE chain。

- 第一個 SQE 是一個讀請求，將資料從輸入檔案讀到 buffer；
- 第二個請求，與第一個請求是 linked，是一個寫請求，將資料從 buffer 寫入輸出檔案。

### 原始碼及註釋

```
/* SPDX-License-Identifier: MIT */
/*
 * Very basic proof-of-concept for doing a copy with linked SQEs. Needs a
 * bit of error handling and short read love.
 */
#include "liburing.h"

#define QD    64         // io_uring 佇列長度
#define BS    (32*1024)

struct io_data {
    size_t offset;
    int index;
    struct iovec iov;
};

static int infd, outfd;
static unsigned inflight;

// 建立一個 read->write SQE chain
static void queue_rw_pair(struct io_uring *ring, off_t size, off_t offset) {
    struct io_uring_sqe *sqe;
    struct io_data *data;
    void *ptr;

    ptr = malloc(size + sizeof(*data));
    data = ptr + size;
    data->index = 0;
    data->offset = offset;
    data->iov.iov_base = ptr;
    data->iov.iov_len = size;

    sqe = io_uring_get_sqe(ring);                            // 獲取可用 SQE
    io_uring_prep_readv(sqe, infd, &data->iov, 1, offset);   // 準備 read 請求
    sqe->flags |= IOSQE_IO_LINK;                             // 設定為 LINK 模式
    io_uring_sqe_set_data(sqe, data);                        // 設定 data

    sqe = io_uring_get_sqe(ring);                            // 獲取另一個可用 SQE
    io_uring_prep_writev(sqe, outfd, &data->iov, 1, offset); // 準備 write 請求
    io_uring_sqe_set_data(sqe, data);                        // 設定 data
}

// 處理完成（completion）事件：釋放 SQE 的記憶體緩衝區，通知核心已經消費了 CQE。
static int handle_cqe(struct io_uring *ring, struct io_uring_cqe *cqe) {
    struct io_data *data = io_uring_cqe_get_data(cqe);       // 獲取 CQE
    data->index++;

    if (cqe->res < 0) {
        if (cqe->res == -ECANCELED) {
            queue_rw_pair(ring, BS, data->offset);
            inflight += 2;
        } else {
            printf("cqe error: %s\n", strerror(cqe->res));
            ret = 1;
        }
    }

    if (data->index == 2) {        // read->write chain 完成，釋放緩衝區記憶體
        void *ptr = (void *) data - data->iov.iov_len;
        free(ptr);
    }

    io_uring_cqe_seen(ring, cqe);  // 通知核心已經消費了 CQE 事件
    return ret;
}

static int copy_file(struct io_uring *ring, off_t insize) {
    struct io_uring_cqe *cqe;
    size_t this_size;
    off_t offset;

    offset = 0;
    while (insize) {                      // 資料還沒處理完
        int has_inflight = inflight;      // 當前正在進行中的 SQE 數量
        int depth;  // SQE 閾值，當前進行中的 SQE 數量（inflight）超過這個值之後，需要阻塞等待 CQE 完成

        while (insize && inflight < QD) { // 資料還沒處理完，io_uring 佇列也還沒用完
            this_size = BS;
            if (this_size > insize)       // 最後一段資料不足 BS 大小
                this_size = insize;

            queue_rw_pair(ring, this_size, offset); // 建立一個 read->write chain，佔用兩個 SQE
            offset += this_size;
            insize -= this_size;
            inflight += 2;                // 正在進行中的 SQE 數量 +2
        }

        if (has_inflight != inflight)     // 如果有新建立的 SQE，
            io_uring_submit(ring);        // 就提交給核心

        if (insize)                       // 如果還有 data 等待處理，
            depth = QD;                   // 閾值設定 SQ 的佇列長度，即 SQ 佇列用完才開始阻塞等待 CQE；
        else                              // data 處理已經全部提交，
            depth = 1;                    // 閾值設定為 1，即只要還有 SQE 未完成，就阻塞等待 CQE

        // 下面這個 while 只有 SQ 佇列用完或 data 全部提交之後才會執行到
        while (inflight >= depth) {       // 如果所有 SQE 都已經用完，或者所有 data read->write 請求都已經提交
            io_uring_wait_cqe(ring, &cqe);// 等待核心 completion 事件
            handle_cqe(ring, cqe);        // 處理 completion 事件：釋放 SQE 記憶體緩衝區，通知核心 CQE 已消費
            inflight--;                   // 正在進行中的 SQE 數量 -1
        }
    }

    return 0;
}

static int setup_context(unsigned entries, struct io_uring *ring) {
    io_uring_queue_init(entries, ring, 0);
    return 0;
}

static int get_file_size(int fd, off_t *size) {
    struct stat st;

    if (fstat(fd, &st) < 0)
        return -1;
    if (S_ISREG(st.st_mode)) {
        *size = st.st_size;
        return 0;
    } else if (S_ISBLK(st.st_mode)) {
        unsigned long long bytes;

        if (ioctl(fd, BLKGETSIZE64, &bytes) != 0)
            return -1;

        *size = bytes;
        return 0;
    }

    return -1;
}

int main(int argc, char *argv[]) {
    struct io_uring ring;
    off_t insize;
    int ret;

    infd = open(argv[1], O_RDONLY);
    outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (setup_context(QD, &ring))
        return 1;
    if (get_file_size(infd, &insize))
        return 1;

    ret = copy_file(&ring, insize);

    close(infd);
    close(outfd);
    io_uring_queue_exit(&ring);
    return ret;
}
```

### 其他說明

程式碼中實現了三個函數：

1. `copy_file()`：高層複製循環邏輯；它會呼叫 `queue_rw_pair(ring, this_size, offset)` 來構造 SQE pair； 並通過一次 `io_uring_submit()` 呼叫將所有建構的 SQE pair 提交。

    這個函數維護了一個最大 DQ 數量的 inflight SQE，只要資料 copy 還在進行中；否則，即資料已經全部讀取完成，就開始等待和收割所有的 CQE。

2. `queue_rw_pair()` 構造一個 read-write SQE pair.

    read SQE 的 `IOSQE_IO_LINK` flag 表示開始一個 chain，write SQE 不用設定這個 flag，標誌著這個 chain 的結束。 使用者 data 欄位設定為同一個 data 描述符，並且在隨後的 completion 處理中會用到。

3. `handle_cqe()` 從 CQE 中提取之前由 `queue_rw_pair()` 保存的 data 描述符，並在描述符中記錄處理進展（index）。

    如果之前請求被取消，它還會重新提交 read-write pair。

    一個 CQE pair 的兩個 member 都處理完成之後（`index==2`），釋放共享的 data descriptor。 最後通知核心這個 CQE 已經被消費。


# 4 `io_uring` 性能壓測（基於 `fio`）

**==對於已經在使用 linux-aio 的應用==**，例如 ScyllaDB， **==不要期望換成 io_uring 之後能獲得大幅的性能提升==**，這是因為： `io_uring` 性能相關的底層機制與 `linux-aio` 並無本質不同（都是非同步提交，輪詢結果）。

在此，本文也希望使讀者明白：`io_uring` **==首先和最重要的貢獻==**在於： **==將 linux-aio 的所有優良特性帶給了普羅大眾==**（而非侷限於資料庫這樣的細分領域）。

## 4.1 測試環境

本節使用 `fio` 測試 4 種模式：

1. synchronous reads
2. `posix-aio` (implemented as a thread pool)
3. `linux-aio`
4. `io_uring`

硬體：

- NVMe 儲存裝置，物理極限能打到 **==3.5M IOPS==**。
- 8 核處理器

## 4.2 場景一：direct I/O `1KB` 隨機讀（繞過 page cache）

第一組測試中，我們希望所有的讀請求都能**==命中儲存裝置==**（all reads to hit the storage），**==完全繞開作業系統的頁快取==**（page cache）。

測試組態：

- 8 個 CPU 執行 72 `fio` job，
- 每個 job 隨機讀取 4 個檔案，
- `iodepth=8`（number of I/O units to keep in flight against the file.）。

這種組態**==保證了 CPU 處於飽和狀態==**，便於觀察 I/O 性能。 如果 CPU 數量足夠多，那每組測試都可能會打滿裝置頻寬，結果對 I/O 壓測就沒意義了。

表 1. Direct I/O（繞過系統頁快取）：1KB 隨機讀，CPU 100% 下的 I/O 性能

|   |   |   |   |
|---|---|---|---|
|**backend**|**IOPS**|**context switches**|**IOPS ±% vs io_uring**|
|sync|814,000|27,625,004|**-42.6%**|
|posix-aio (thread pool)|433,000|64,112,335|**-69.4%**|
|linux-aio|1,322,000|10,114,149|**-6.7%**|
|io_uring (basic)|1,417,000|11,309,574|**—**|
|io_uring (enhanced)|1,486,000|11,483,468|**4.9%**|

![](http://arthurchiao.art/assets/img/intro-to-io-uring/benchmark-1.png)

幾點分析：

1. `io_uring` 相比 `linux-aio` 確實有一定提升，但並非革命性的。
2. 開啟高級特性，例如 buffer & file registration 之後性能有進一步提升 —— 但也還 沒有到為了這些性能而重寫整個應用的地步，除非你是搞資料庫研發，想搾取硬體的最後一分性能。
3. `io_uring` and `linux-aio` **==都比同步 read 介面快 2 倍，而後者又比 posix-aio 快 2 倍==** —— 初看有點差異。但看看**==上下文切換次數==**，就不難理解為什麼 posix-aio 這麼慢了。

    - 同步 read 性能差是因為：在這種沒有 page cache 的情況下， **==每次 read 系統呼叫都會阻塞，因此就會涉及一次上下文切換==**。
    - `posix-aio` 性能更差是因為：不僅核心和應用程式之間要頻繁上下文切換，執行緒池的**==多個執行緒之間也在頻繁切換==**。

## 4.2 場景二：buffered I/O `1KB` 隨機讀（資料提前載入到記憶體，100% hot cache）

第二組測試 buffered I/O：

1. 將檔案資料提前載入到記憶體，然後再測隨機讀。

    - 由於**==資料全部在 page cache==**，因此**==同步 read 永遠不會阻塞==**。
    - 這種場景下，我們預期**==同步讀和 io_uring 的性能差距不大（都是最好的）==**。
2. 其他測試條件不變。


表 2. Buffered I/O（資料全部來自 page cache，==100% hot cache==）：1KB 隨機讀，100% CPU 下的 I/O 性能

|   |   |   |   |
|---|---|---|---|
|**Backend**|**IOPS**|**context switches**|**IOPS ±% vs io_uring**|
|sync|4,906,000|105,797|**-2.3%**|
|posix-aio (thread pool)|1,070,000|114,791,187|**-78.7%**|
|linux-aio|4,127,000|105,052|**-17.9%**|
|io_uring|5,024,000|106,683|**—**|

![](http://arthurchiao.art/assets/img/intro-to-io-uring/benchmark-2.png)

結果分析：

1. 同步讀和 `io_uring` 性能差距確實很小，二者都是最好的。

    但注意，**==實際的應用==**不可能一直 100% 時間執行 IO 操作，因此 基於同步讀的真實應用性能**==還是要比基於 io_uring 要差的==**，因為 io_uring 會將多個系統呼叫批處理化。

2. `posix-aio` 性能最差，直接原因是**==上下文切換次數太多==**，這也和場景相關： 在這種 **==CPU 飽和的情況下==**，它的執行緒池反而是累贅，會完全拖慢性能。

3. `linux-aio` 並**==不是針對 buffered I/O 設計的==**，在這種 page cache 直接返回的場景， 它的**==非同步介面反而會造成性能損失==** —— 將操作分 為 dispatch 和 consume 兩步不但沒有性能收益，反而有額外開銷。


## 4.3 性能測試小結

最後再次提醒，本節是極端應用/場景（**==100% CPU + 100% cache miss/hit==**）測試， 真實應用的行為通常處於同步讀和非同步讀之間：時而一些阻塞操作，時而一些非阻塞操作。 但不管怎麼說，用了 io_uring 之後，使用者就無需擔心同步和非同步各佔多少比例了，因為它**==在任何場景下都表現良好==**。

1. 如果操作是非阻塞的，`io_uring` 不會有額外開銷；
2. 如果操作是阻塞式的，也沒關係，`io_uring` 是完全非同步的，並且不依賴執行緒池或昂貴的上下文切換來實現這種非同步能力；

本文測試的都是隨機讀，但對**==其他類型的操作==**，`io_uring` 表現也是非常良好的。例如：

1. 打開/關閉檔案
2. 設定定時器
3. 通過 network sockets 傳輸資料

而且**==使用的是同一套 io_uring 介面==**。

## 4.4 ScyllaDB 與 io_uring

Scylla 重度依賴 direct I/O，從一開始就使用 `linux-aio`。 在我們轉向 `io_uring` 的過程中，最開始測試顯示對某些 workloads，能取得 50% 以上的性能提升。 但**==深入研究之後發現==**，這是因為我們**==之前的 linux-aio 用的不夠好==**。 這也揭示了一個**==經常被忽視的事實==**：獲得高性能沒有那麼難（前提是你得弄對了）。 在對比 `io_uring` 和 `linux-aio` 應用之後，我們**==很快改進了一版，二者的性能差距就消失了==**。 但坦率地說，解決這個問題**==需要一些工作量==**，因為要改動一個已經使用 了很多年的基於 `linux-aio` 的介面。而對 `io_uring` 應用來說，做類似的改動是輕而 易舉的。

以上只是一個場景，`io_uring` 相比 `linux-aio` 的**==優勢==**是能應用於 file I/O 之外的場景。 此外，它還自帶了特殊的 [高性能](https://www.p99conf.io/) 介面，例如 buffer registration、file registration、輪詢模式等等。

啟用 `io_uring` 高級特性之後，我們看到性能確實有提升：Intel Optane 裝置，單個 CPU 讀取 512 位元組，觀察到 5% 的性能提升。與 表 1 & 2 對得上。雖然 5% 的提升 看上去不是太大，但對於希望壓榨出硬體所有性能的資料庫來說，還是非常寶貴的。

|   |   |
|---|---|
|==linux-aio:==<br><br>Throughput : 330 MB/s  <br>Lat average : 1549 usec  <br>Lat quantile= 0.5 : 1547 usec  <br>Lat quantile= 0.95 : 1694 usec  <br>Lat quantile= 0.99 : 1703 usec  <br>Lat quantile=0.999 : 1950 usec  <br>Lat max : 2177 usec|==io_uring, with buffer and file registration and poll:==<br><br>Throughput : 346 MB/s  <br>Lat average : 1470 usec  <br>Lat quantile= 0.5 : 1468 usec  <br>Lat quantile= 0.95 : 1558 usec  <br>Lat quantile= 0.99 : 1613 usec  <br>Lat quantile=0.999 : 1674 usec  <br>Lat max : 1829 usec|
|使用 1 個 CPU 從 Intel Optane 裝置讀取 512 位元組。1000 並行請求。linux-aio 和 io_uring basic interface 性能差異很小。 但啟用 io_uring 高級特性後，有 5% 的性能差距。|   |

# 5 eBPF

eBPF 也是一個**==事件驅動框架==**（因此也是非同步的），允許使用者空間程序動態向核心注入位元組碼，主要有兩個使用場景：

1. Networking：[本站](https://arthurchiao.art) 已經有相當多的文章
2. Tracing & Observability：例如 [bcc](https://github.com/iovisor/bcc) 等工具

eBPF 在核心 4.9 首次引入，4.19 以後功能已經很強大。更多關於 eBPF 的演進資訊，可參考： [==（譯）大規模微服務利器：eBPF + Kubernetes（KubeCon, 2020）==](http://arthurchiao.art/blog/ebpf-and-k8s-zh/)。

談到與 io_uring 的結合，就是用 bcc 之類的工具跟蹤一些 I/O 相關的核心函數，例如：

1. Trace how much time an application spends sleeping, and what led to those sleeps. (`wakeuptime`)
2. Find all programs in the system that reached a particular place in the code (`trace`)
3. Analyze network TCP throughput aggregated by subnet (`tcpsubnet`)
4. Measure how much time the kernel spent processing softirqs (`softirqs`)
5. Capture information about all short-lived files, where they come from, and for how long they were opened (`filelife`)

# 6 結束語

`io_uring` 和 eBPF 這兩大特性**==將給 Linux 程式設計帶來革命性的變化==**。 有了這兩個特性的加持，開發者就能更充分地利用 [Amazon i3en meganode systems](https://www.scylladb.com/2019/05/28/aws-new-i3en-meganode) 之類的多核/多處理器系統，以及 [Intel Optane 持久儲存](https://www.scylladb.com/2017/09/27/intel-optane-scylla-providing-speed-memory-database-persistency) 之類的 `us` 級延遲儲存裝置。

# 參考資料

- [Efficient IO with io_uring](https://kernel.dk/io_uring.pdf), pdf
- [Ringing in a new asynchronous I/O API](https://lwn.net/Articles/776703/), lwn.net
- [The rapid growth of io_uring](https://lwn.net/Articles/810414/), lwn.net
- [System call API](https://github.com/axboe/liburing/tree/master/man), manpage

[« [譯] Linux Socket Filtering (LSF, aka BPF)（KernelDoc，2021）](http://arthurchiao.art/blog/linux-socket-filtering-aka-bpf-zh/) [[譯] Control Group v2（cgroupv2 權威指南）（KernelDoc, 2021） »](http://arthurchiao.art/blog/cgroupv2-zh/)
