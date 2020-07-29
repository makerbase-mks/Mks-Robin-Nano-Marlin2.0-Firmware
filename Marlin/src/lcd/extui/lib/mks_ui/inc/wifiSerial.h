#ifndef WIFI_SERIAL
#define WIFI_SERIAL

#define USE_WIFI_FUNCTION		1
#define WIFI_BAUDRATE			115200
#define WIFI_UPLOAD_BAUDRATE	1958400

#define USART_SAFE_INSERT

#if USE_WIFI_FUNCTION

#define WIFI_RX_BUF_SIZE	(1024+1)

#include <libmaple/libmaple_types.h>
#include <libmaple/usart.h>
#include <libmaple/libmaple.h>
#include <libmaple/gpio.h>
#include <libmaple/timer.h>
#include <libmaple/ring_buffer.h>

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
    inline void wifi_usart_irq(usart_reg_map *regs){
		/* Handling RXNEIE and TXEIE interrupts. 
	     * RXNE signifies availability of a byte in DR.
	     *
	     * See table 198 (sec 27.4, p809) in STM document RM0008 rev 15.
	     * We enable RXNEIE. */
	    if ((regs->CR1 & USART_CR1_RXNEIE) && (regs->SR & USART_SR_RXNE)) {
		#ifdef USART_SAFE_INSERT
	        /* If the buffer is full and the user defines USART_SAFE_INSERT,
	         * ignore new bytes. */
	        rb_safe_insert(this->usart_device->rb, (uint8)regs->DR);
		#else
	        /* By default, push bytes around in the ring buffer. */
	        rb_push_insert(this->usart_device->rb, (uint8)regs->DR);
		#endif
	    }
	    /* TXE signifies readiness to send a byte to DR. */
	    if ((regs->CR1 & USART_CR1_TXEIE) && (regs->SR & USART_SR_TXE)) {
	        if (!rb_is_empty(this->usart_device->wb))
	            regs->DR=rb_remove(this->usart_device->wb);
	        else
	            regs->CR1 &= ~((uint32)USART_CR1_TXEIE); // disable TXEIE
	    }
    }
	
    int wifi_rb_is_full(void);
	
    private:
    struct usart_dev *usart_device;
    uint8 tx_pin;
    uint8 rx_pin;
};

extern WifiSerial WifiSerial1;

#define	WIFISERIAL	WifiSerial1

#endif//USE_WIFI_FUNCTION

#endif//WIFI_SERIAL