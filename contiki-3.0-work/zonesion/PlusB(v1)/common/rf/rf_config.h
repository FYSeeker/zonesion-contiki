#ifndef _rf_config_h_
#define _rf_config_h_

//xlabTools���ÿ�/��
#define XLABTOOLS_ON    1


#define RF_CONFIG_FILE
/*********************************************************************************************
zigbee config
*********************************************************************************************/
#define ZIGBEE_CONFIG_PANID     0x2100
#define ZIGBEE_CONFIG_CHANNEL   11

/*********************************************************************************************
ble config
*********************************************************************************************/

/*********************************************************************************************
WiFi config
*********************************************************************************************/
#define WIFI_CONFIG_SSID        "AndroidAP"
#define WIFI_CONFIG_KEYTYPE     0
#define WIFI_CONFIG_KEY         "123456789"
#define WIFI_CONFIG_SIP         "192.168.43.1"

/*********************************************************************************************
lora config
*********************************************************************************************/
//#define LORA_CONFIG_NODEID  0xE852          //�ڵ��ַ:0x01-0xFFFF
#define LORA_CONFIG_NETID   50              //����ID:0-254
#define LORA_CONFIG_FP      433             //��Ƶ
#define LORA_CONFIG_PV      15              //���书��:1-20
#define LORA_CONFIG_SF      8               //��Ƶ����:6-12
#define LORA_CONFIG_CR      1               //������:1-4
#define LORA_CONFIG_PS      15              //ǰ���볤��
#define LORA_CONFIG_BW      5               //����:0-9
#define LORA_CONFIG_HOP     0               //��Ƶ����:0/1
//��Ƶ��
#define LORA_CONFIG_HOPTAB  "431,435,431,435,431,435,431,435,431,435"


#endif