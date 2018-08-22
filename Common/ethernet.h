#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>

typedef struct 
{
	uint16_t (* packetReceive) (uint8_t* buf, uint16_t buflen);
	void (* packetSend)(uint8_t* buf, uint16_t buflen);
	uint8_t ipaddr[4];
	uint8_t * mac_address;
	uint8_t * frame_buffer;
	uint16_t frame_buffer_size;
}ethernet_t;

typedef struct eth_frame
{
  uint8_t addr_dest[6];
  uint8_t addr_src[6];
  uint16_t type;
  uint8_t data[];
}frame_ptr;

typedef struct arp_msg
{
  uint16_t net_tp;
  uint16_t proto_tp;
  uint8_t macaddr_len;
  uint8_t ipaddr_len;
  uint16_t op;
  uint8_t macaddr_src[6];
  uint8_t ipaddr_src[4];
  uint8_t macaddr_dst[6];
  uint8_t ipaddr_dst[4];
} arp_msg_ptr;

typedef struct ip_pkt{
	uint8_t verlen;//������ ��������� � ����� ���������
	uint8_t ts;//��� �������
	uint16_t len;//�����
	uint16_t id;//������������� ������
	uint16_t fl_frg_of;//����� � �������� ���������
	uint8_t ttl;//����� �����
	uint8_t prt;//��� ���������
	uint16_t cs;//����������� ����� ���������
	uint8_t ipaddr_src[4];//IP-����� �����������
	uint8_t ipaddr_dst[4];//IP-����� ����������
	uint8_t data[];//������
} ip_pkt_ptr;

typedef struct icmp_pkt{
	uint8_t msg_tp;//��� �������
	uint8_t msg_cd;//��� ���������
	uint16_t cs;//����������� ����� ���������
	uint16_t id;//������������� ������
	uint16_t num;//����� ������
	uint8_t data[];//������
} icmp_pkt_ptr;

/* Public fuctions prototypes */
ethernet_t * ethernet_Init (ethernet_t * this);
void net_poll(void);
#endif
