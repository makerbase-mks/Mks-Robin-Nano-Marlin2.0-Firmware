#ifndef _WIFI_MODULE_H_
#define _WIFI_MODULE_H_

#include "../../../../../inc/MarlinConfig.h"

#define UART_RX_BUFFER_SIZE   1024
#define UART_FIFO_BUFFER_SIZE 1024

#define ESP_WIFI		0x02

#define AP_MODEL		0x01
#define STA_MODEL		0x02

#define WIFI_DECODE_TYPE	1				//AUTO

#define IP_DHCP_FLAG	1

#define WIFI_AP_NAME		"TP-LINK_MKS"
#define WIFI_KEY_CODE		"makerbase"

#define IP_ADDR 	 	"192.168.3.100"
#define IP_MASK 	 	"255.255.255.0"
#define IP_GATE 	 	"192.168.3.1"
#define IP_DNS 	 	 	"192.168.3.1"


#define AP_IP_DHCP_FLAG	1
#define AP_IP_ADDR		"192.168.3.100"
#define AP_IP_MASK		"255.255.255.0"
#define AP_IP_GATE		"192.168.3.1"
#define AP_IP_DNS		"192.168.3.1"
#define IP_START_IP		"192.168.3.1"
#define IP_END_IP		"192.168.3.255"

#define WIFI_SET()			WRITE(WIFI_RESET_PIN, HIGH);//GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define WIFI_RESET()			WRITE(WIFI_RESET_PIN, LOW);//GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define WIFI_IO1_SET()		WRITE(WIFI_IO1_PIN, HIGH);//GPIO_SetBits(GPIOC, GPIO_Pin_7)      
#define WIFI_IO1_RESET()		WRITE(WIFI_IO1_PIN, LOW);//GPIO_ResetBits(GPIOC, GPIO_Pin_7)

#define UDISKBUFLEN 1024

typedef enum
{
	udisk_buf_empty = 0,
	udisk_buf_full,	
} UDISK_DATA_BUFFER_STATE;

#define TRANS_RCV_FIFO_BLOCK_NUM	8


typedef struct			
{
	unsigned char *bufferAddr[TRANS_RCV_FIFO_BLOCK_NUM];		
	unsigned char *p;	
	UDISK_DATA_BUFFER_STATE state[TRANS_RCV_FIFO_BLOCK_NUM];		
	unsigned char read_cur; 
	unsigned char write_cur;	
}WIFI_DMA_RCV_FIFO;		



typedef struct
{
	uint8_t flag; // 0x0: no error;  0x01: error
	uint32_t start_tick; //error start time
	uint32_t now_tick;
	
} WIFI_TRANS_ERROR;
extern volatile WIFI_TRANS_ERROR wifiTransError;


typedef  struct
{
	char ap_name[32];	//wifi-name
	char keyCode[64]; //wifi password
	int   decodeType; 
	int baud;
	int mode;
} WIFI_PARA;

typedef  struct
{
	char state;	
	char hostUrl[96];
	int port;		
	char id[21];
} CLOUD_PARA;

typedef struct
{
	char  dhcp_flag;	
	char  ip_addr[16];	
	char  mask[16];	
	char  gate[16];	
	char  dns[16];		
	
	char  dhcpd_flag; 
	char  dhcpd_ip[16];
	char  dhcpd_mask[16];
	char  dhcpd_gate[16];
	char  dhcpd_dns[16];
	char  start_ip_addr[16];
	char  end_ip_addr[16];
} IP_PARA;


typedef enum
{
	WIFI_NOT_CONFIG,
	WIFI_CONFIG_MODE,
	WIFI_CONFIG_DHCP,
	WIFI_CONFIG_AP,
	WIFI_CONFIG_IP_INF,
	WIFI_CONFIG_DNS,
	WIFI_CONFIG_TCP,
	WIFI_CONFIG_SERVER,
	WIFI_CONFIG_REMOTE_PORT,
	WIFI_CONFIG_BAUD,
	WIFI_CONFIG_COMMINT,
	WIFI_CONFIG_OK,
	WIFI_GET_IP_OK,
	WIFI_RECONN,
	WIFI_CONNECTED,
	WIFI_WAIT_TRANS_START,
	WIFI_TRANS_FILE,
	WIFI_CONFIG_DHCPD,
	WIFI_COFIG_DHCPD_IP,
	WIFI_COFIG_DHCPD_DNS,
	WIFI_EXCEPTION,
} WIFI_STATE;

typedef enum
{
	TRANSFER_IDLE,
	TRANSFERING,
	TRANSFER_STORE,
} TRANSFER_STATE;
extern volatile TRANSFER_STATE esp_state;

typedef struct
{
	char buf[20][80];
	int rd_index;
	int wt_index;
} QUEUE;


typedef enum
{
	WIFI_PARA_SET, 	//0x0:net parameter
	WIFI_PRINT_INF, 	//0x1:print message
	WIFI_TRANS_INF, 	//0x2:Pass through information
	WIFI_EXCEP_INF,	 //0x3:Exception information
	WIFI_CLOUD_CFG,	//0x4:cloud config
	WIFI_CLOUD_UNBIND, //0x5:Unbind ID
} WIFI_RET_TYPE;


typedef struct 
{  
  uint32_t uart_read_point;
  uint32_t uart_write_point;
  uint8_t uartTxBuffer[UART_FIFO_BUFFER_SIZE];
} SZ_USART_FIFO;

#define WIFI_GCODE_BUFFER_LEAST_SIZE	96
#define WIFI_GCODE_BUFFER_SIZE	(WIFI_GCODE_BUFFER_LEAST_SIZE * 3)
typedef struct
{
	uint8_t wait_tick;
	uint8_t Buffer[WIFI_GCODE_BUFFER_SIZE];
	uint32_t r;
	uint32_t w;
} WIFI_GCODE_BUFFER;

extern volatile WIFI_STATE wifi_link_state;
extern WIFI_PARA wifiPara;
extern IP_PARA ipPara;
extern CLOUD_PARA cloud_para;

extern WIFI_GCODE_BUFFER espGcodeFifo; 

extern uint32_t  getWifiTick();
extern uint32_t  getWifiTickDiff(int32_t lastTick, int32_t  curTick);

void esp_wifi_init();

extern char wifi_loop_time;

extern int cfg_cloud_flag;
extern int send_to_wifi(char *buf, int len);
extern void wifi_looping();
extern int raw_send_to_wifi(char *buf, int len);
extern int package_to_wifi(WIFI_RET_TYPE type, char *buf, int len);
extern void get_wifi_list_command_send();
extern void get_wifi_commands();
extern int readWifiBuf(int8_t *buf, int32_t len);
extern int storeRcvData(int32_t len);

#endif

