#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>

typedef enum
{
	RESERVED,		//��������������
	CONNECT,		//������	�������	��	�����������	�	�������
	CONNACK,		//�������������	���������	�����������
	PUBLISH,		//����������	���������
	PUBACK,			//�������������	����������
	PUBREC,			//����������	��������
	PUBREL,			//����������	��	��������	���������
	PUBCOMP,		//����������	���������
	SUBSCRIBE,	//������	��	��������
	SUBACK,			//������	��	��������	������
	UNSUBSCRIBE,//������	��	�������
	UNSUBACK,		//������	��	�������	������
	PINGREQ,		//PING	������
	PINGRESP,		//PING	�����
	DISCONNECT,	//���������	��	����������	��	�������
}mqtt_msg_type_t;

typedef struct
{
	mqtt_msg_type_t msg_type : 4;
	uint8_t flags : 4;
	uint8_t len;
}mqtt_fixed_header_t;

#endif
