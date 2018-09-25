#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>

typedef enum
{
	MQTT_SUCCESS = 0,
	MQTT_NOT_INIT,
	MQTT_AUTH_ERROR,
	MQTT_PING_FAIL,
	MQTT_NO_MESSAGES,
	MQTT_FORMAT_ERR,
	MQTT_NO_MESSAGES_FOR_THIS_TOPIC,
}mqtt_err_t;

typedef struct
{
	uint8_t (* sendReceive) (uint8_t * buf, uint8_t len);
	char * clientId, * username, * password;
	uint8_t * buf;
}mqtt_client_t;

typedef enum
{
	RESERVED 	= 0x0,		//��������������
	CONNECT		= 0x1,		//������	�������	��	�����������	�	�������
	CONNACK		= 0x2,		//�������������	���������	�����������
	PUBLISH		= 0x3,		//����������	���������
	PUBACK		= 0x4,		//�������������	����������
	PUBREC		=	0x5,		//����������	��������
	PUBREL		=	0x6,		//����������	��	��������	���������
	PUBCOMP		=	0x7,		//����������	���������
	SUBSCRIBE	=	0x8,		//������	��	��������
	SUBACK		=	0x9,		//������	��	��������	������
	UNSUBSCRIBE = 0xA,	//������	��	�������
	UNSUBACK		= 0xB,	//������	��	�������	������
	PINGREQ			=	0xC,	//PING	������
	PINGRESP		= 0xD,	//PING	�����
	DISCONNECT	= 0xE,	//���������	��	����������	��	�������
}sendMessageToServerMessage_type_t;

typedef struct
{
	uint8_t retain : 1;
	uint8_t qos : 2;
	uint8_t dup : 1;
	uint8_t msg_type : 4;
	uint8_t len;
	uint8_t data[];
}mqtt_fixed_header_t;

typedef struct
{
	uint8_t flags;
	uint8_t len;
	char data[];
}mqtt_flex_header_t;

mqtt_client_t * mqtt_init (mqtt_client_t * ptr);
mqtt_err_t mqtt_connect (void);
mqtt_err_t mqtt_ping (void);
mqtt_err_t sendMessageToServerMessage (char * topic, char * message);
mqtt_err_t mqtt_subscribe (char * topic);
mqtt_err_t mqtt_checkMessages (char * topic, char * messageOut);
mqtt_err_t mqtt_getMessages (char * topicOut, char * messageOut);
#endif
