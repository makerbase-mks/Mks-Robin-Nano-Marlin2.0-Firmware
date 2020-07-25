#ifndef WIFI_SERIAL
#define WIFI_SERIAL

#define USE_WIFI_FUNCTION		1
#define WIFI_BAUDRATE			115200
#define WIFI_UPLOAD_BAUDRATE	1958400

#if USE_WIFI_FUNCTION

#define WIFI_RX_BUF_SIZE	(1024+1)

#include <libmaple/libmaple_types.h>
#include <libmaple/usart.h>

#define DEFINE_WFSERIAL(name, n)                                   \
	WifiSerial name(USART##n,                                  \
	BOARD_USART##n##_TX_PIN,                   \
	BOARD_USART##n##_RX_PIN)

class WifiSerial{

    public:
		
    uint8 wifiRxBuf[WIFI_RX_BUF_SIZE];
	
    public:
    WifiSerial(struct usart_dev *usart_device,
                   uint8 tx_pin,
                   uint8 rx_pin);

    /* Set up/tear down */
    void begin(uint32 baud);
    void begin(uint32 baud,uint8_t config);
    void end();
    int available(void);
    int read(void);
    size_t write(uint8_t);
	
    //private:
    struct usart_dev *usart_device;
    uint8 tx_pin;
    uint8 rx_pin;
};

extern WifiSerial WifiSerial1;

#define	WIFISERIAL	WifiSerial1

#endif//USE_WIFI_FUNCTION

#endif//WIFI_SERIAL