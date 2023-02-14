#pragma once

#include <pcap.h>

/* 以太网卡测试用 */
// ethernet headers are always exactly 14 bytes [1]
#define SIZE_ETHERNET 14

// Ethernet addresses are 6 bytes
#define ETHER_ADDR_LEN    6

// Ethernet header
struct sniff_ethernet {
	u_char ether_dhost[ETHER_ADDR_LEN];     // Destination host address
	u_char ether_shost[ETHER_ADDR_LEN];     // Source host address
	u_short ether_type;                     // IP? ARP? RARP? etc
};

/* ***Test required*** WIFI 802.11 头部定义 */
// 不考虑分布式ap、简单pad的802.11标准省事处理如下
#define SIZE_80211_HEADER 24
#define SIZE_LLC_HEADER 8
#define SIZE_RADIOTAP_HEADER 18
/* monitor mode网卡有三种工作模式，包含来自内核的不同附加信息 */
//Libpcap link type:  DLT_IEEE802_11_RADIO
//radiotap header 长度由it_len指示
struct radiotap_header {
	u_int8_t it_version;     /* set to 0 */
	u_int8_t it_pad;
	u_int16_t it_len;        /* entire length */
	u_int32_t it_present;    /* fields present */
}__attribute__((__packed__));

//Libpcap link type:  DLT_PRISM_HEADER
//prism value
struct prism_value {
	u_int32_t did;
	u_int16_t status;
	u_int16_t len;
	u_int32_t data;
};

//prism header 144 bytes
struct prism_header {
	u_int32_t msgcode;
	u_int32_t msglen;
	u_char devname[16];
	struct prism_value hosttime;
	struct prism_value mactime;
	struct prism_value channel;
	struct prism_value rssi;
	struct prism_value sq;
	struct prism_value signal;
	struct prism_value noise;
	struct prism_value rate;
	struct prism_value istx;
	struct prism_value frmlen;
};

//Libpcap link type:  DLT_IEEE802_11_RADIO_AVS
//avs header 64 bytes
struct avs_header {
	u_int32_t magic;
	u_int32_t version;
	u_int32_t length;
	u_int32_t mactime;
	u_int32_t hosttime;
	u_int32_t phytype;
	u_int32_t channel;
	u_int32_t datarate;
	u_int32_t antenna;
	u_int32_t priority;
	u_int32_t ssi_type;
	u_int32_t ssi_signal;
	u_int32_t ssi_noise;
	u_int32_t preamble;
	u_int32_t encoding;
};

//wifi 802.11 header  ***Test required***
struct sniff_wlan {
	u_int16_t frame_control;
	u_int16_t duration_id;
	u_char addr1[6];
	u_char addr2[6];
	u_char addr3[6];
	u_int16_t seq_ctrl;
	union {
		u_int16_t qos;
		u_int8_t addr4[6];
		struct {
			u_int16_t qos;
			u_int32_t ht;
		} __attribute__((packed)) ht;
		struct {
			u_int8_t addr4[6];
			u_int16_t qos;
			u_int32_t ht;
		} __attribute__((packed)) addr4_qos_ht;
	}u;
}__attribute__((__packed__));

//LLC
struct sniff_llc {
	u_char dsap;
	u_char ssap;
	u_char ctrl;			//1 or 2 bytes?
	u_char org_code[3];		//snap
	u_int16_t eth_type;		//snap
}__attribute__((__packed__));

// IP header
struct sniff_ip {
	u_char ip_vhl;                          // version << 4 | header length >> 2
	u_char ip_tos;                          // type of service
	u_short ip_len;                         // total length
	u_short ip_id;                          // identification
	u_short ip_off;                         // fragment offset field
#define IP_RF 0x8000                    // reserved fragment flag
#define IP_DF 0x4000                    // dont fragment flag
#define IP_MF 0x2000                    // more fragments flag
#define IP_OFFMASK 0x1fff               // mask for fragmenting bits
	u_char ip_ttl;                          // time to live
	u_char ip_p;                            // protocol
	u_short ip_sum;                         // checksum
	struct in_addr ip_src, ip_dst;          // source and dest address
};

#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)

// TCP header
typedef u_int tcp_seq;

struct sniff_tcp {
	u_short th_sport;               // source port
	u_short th_dport;               // destination port
	tcp_seq th_seq;                 // sequence number
	tcp_seq th_ack;                 // acknowledgement number
	u_char th_offx2;                // data offset, rsvd
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
	u_char th_flags;
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20
#define TH_ECE  0x40
#define TH_CWR  0x80
#define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
	u_short th_win;                 // window
	u_short th_sum;                 // checksum
	u_short th_urp;                 // urgent pointer
};

//TODO
// 连接数据库、将数据包存入文件
//Save HTTP Payload

void set_wired(bool);

void got_packet(u_char* args, const struct pcap_pkthdr* header, const u_char* packet);

void print_payload(const u_char* payload, int len);

void print_hex_ascii_line(const u_char* payload, int len, int offset);

void save_http_payload(const u_char* payload, int len);
