在網路的 Link Layer 要自行編寫 IP packet 內容，可以參考以下做法：

### 1. 使用原始套接字（Raw Socket）

原始套接字允許應用程式直接訪問網路層，手動構建和發送 IP 封包。以下是使用原始套接字的一般步驟：

1. **建立原始套接字：** 在大多數操作系統中（例如 Linux），可以使用 `socket` 函數來創建原始套接字。
    `int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);`

2. **設置套接字選項：** 通過設置套接字選項來告訴內核不要自動填充 IP 首部。
    `int one = 1; if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {     perror("setsockopt");     exit(1); }`

3. **構建 IP 封包：** 手動構建 IP 封包，包括 IP 首部和數據部分。需要填寫 IP 首部的各個欄位，如版本、報頭長度、服務類型、總長度、標識、片段偏移、TTL、協議、首部校驗和、源 IP 地址和目的 IP 地址等。

```c
struct ipheader {
unsigned char      iph_ihl:4, iph_ver:4;
unsigned char      iph_tos;
unsigned short int iph_len;
unsigned short int iph_ident;
unsigned short int iph_flag:3, iph_offset:13;
unsigned char      iph_ttl;
unsigned char      iph_protocol;
unsigned short int iph_chksum;
unsigned int       iph_sourceip;
unsigned int       iph_destip;
};

struct ipheader ip;
ip.iph_ver = 4;
ip.iph_ihl = 5;
ip.iph_tos = 0;
ip.iph_len = htons(sizeof(struct ipheader) + datalen);
ip.iph_ident = htons(54321);
ip.iph_ttl = 255; ip.iph_protocol = IPPROTO_TCP;
ip.iph_sourceip = inet_addr("192.168.1.1");
ip.iph_destip = inet_addr("192.168.1.2");
ip.iph_chksum = 0; // 校驗和需要在所有字段設置完成後計算
ip.iph_chksum = checksum((unsigned short *)&ip,
						 sizeof(struct ipheader));

```

4. **發送封包：** 使用 `sendto` 函數發送自構建的封包。
```c
if (sendto(sockfd, packet, packet_len, 0,
		   (struct sockaddr *)&dest, sizeof(dest)) < 0) {
	perror("sendto");
	exit(1);
}
```

### 2. 使用封包構建庫

有一些庫和工具可以幫助構建和發送自定義封包，例如：

- **Scapy（Python）：** Scapy 是一個強大的 Python 庫，用於封包處理，可以用來構建和發送自定義 IP 封包。

```c
from scapy.all import *
ip = IP(src="192.168.1.1", dst="192.168.1.2")
tcp = TCP(sport=12345, dport=80, flags="S")
packet = ip/tcp  send(packet)
```

- **libpcap/WinPcap（C/C++）：** libpcap 和 WinPcap 是跨平台的封包捕獲庫，除了捕獲封包外，也可以用來發送封包。

```c
pcap_t *handle; char errbuf[PCAP_ERRBUF_SIZE];
handle = pcap_open_live("eth0", BUFSIZ, 1, 1000, errbuf);
if (handle == NULL) {
	fprintf(stderr, "Couldn't open device: %s\n", errbuf);
	return(2);
}
if (pcap_sendpacket(handle, packet, packet_len) != 0) {
	fprintf(stderr, "Error sending packet: %s\n",
		pcap_geterr(handle));
	return(2);
}
pcap_close(handle);
```


如果你只需要構建並發送一個包含 IP 頭部的封包而不包含 TCP 或其他協議的數據，可以參考下面的範例。這個範例展示了如何構建一個僅包含 IP 頭部的封包，並通過原始套接字發送：
這些方法允許你手動構建和發送 IP 封包，但也需要對 TCP/IP 協議有較深入的了解，特別是如何計算首部校驗和等技術細節。

### 3. 定義 IP 頭部結構

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
// IP 頭部結構
struct ipheader {
	unsigned char      iph_ihl:4, iph_ver:4;
	unsigned char      iph_tos;
	unsigned short int iph_len;
	unsigned short int iph_ident;
	unsigned short int iph_flag:3, iph_offset:13;
	unsigned char      iph_ttl;
	unsigned char      iph_protocol;
	unsigned short int iph_chksum;
	unsigned int       iph_sourceip;
	unsigned int       iph_destip;
};
// 計算校驗和的函數
unsigned short checksum(void *b, int len) {
	unsigned short *buf = b;
	unsigned int sum = 0;
	unsigned short result;
	for (sum = 0; len > 1; len -= 2)
		sum += *buf++;
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}
```


### 4. 構建和發送 IP 封包


```c
int main() {
	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (sockfd < 0) {
		perror("socket");
		exit(1);
	}
	// 設置套接字選項，告訴內核不要自動填充 IP 頭部
	int one = 1;
	if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &one,
		sizeof(one)) < 0) {
		perror("setsockopt");
		exit(1);
	}
	// 分配封包緩衝區
	char packet[4096];
	memset(packet, 0, 4096);
	// 填寫 IP 頭部
	struct ipheader *ip = (struct ipheader *) packet;
	ip->iph_ver = 4;
	ip->iph_ihl = 5;
	ip->iph_tos = 0;
	ip->iph_len = htons(sizeof(struct ipheader));
	ip->iph_ident = htons(54321);
	ip->iph_ttl = 255;
	ip->iph_protocol = IPPROTO_RAW;
	ip->iph_sourceip = inet_addr("192.168.1.1");
	ip->iph_destip = inet_addr("192.168.1.2");
	ip->iph_chksum = 0;
	ip->iph_chksum = checksum((unsigned short *)packet,
	sizeof(struct ipheader));
	// 設置目的地址
	struct sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = ip->iph_destip;
	// 發送封包
	if (sendto(sockfd, packet, sizeof(struct ipheader), 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
		perror("sendto");
		exit(1);
	}
	close(sockfd);
	return 0;
}
```


在這個範例中，代碼構建了一個僅包含 IP 頭部的封包，並通過 `sendto` 函數將其發送到目的地。該封包沒有包含 TCP、UDP 或任何其他協議的數據部分。確保根據你的需求正確填寫 IP 頭部的各個欄位，例如源 IP、目的 IP 等。