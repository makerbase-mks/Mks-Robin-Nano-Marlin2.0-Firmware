#include "../../../../../MarlinCore.h"

#if ENABLED(TFT_LITTLE_VGL_UI)

#include "../inc/draw_ui.h"

#include "../inc/wifiSerial.h"

#include <libmaple/libmaple.h>
#include <libmaple/gpio.h>
#include <libmaple/timer.h>
#include <libmaple/usart.h>
#include <libmaple/ring_buffer.h>

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

void __irq_usart1(void) {
   WIFISERIAL.wifi_usart_irq(USART1_BASE);
   if(wifi_link_state == WIFI_TRANS_FILE)
   {
	   if(WIFISERIAL.available() == (512))
	   {
	   	WIFI_IO1_SET();
	   }
	   if(WIFISERIAL.wifi_rb_is_full())
	   {
	   	if(esp_state == TRANSFER_IDLE)
		{
			esp_state = TRANSFERING;
		}
		if(storeRcvData(UART_RX_BUFFER_SIZE))
		{
            		if(wifiTransError.flag != 0x1)
			{
				WIFI_IO1_RESET();
			}
		}
		else
		{
            		WIFI_IO1_SET();
			esp_state = TRANSFER_STORE;

		}
	   }
   }
}

#if defined(__cplusplus)
}    /* Make sure we have C-declarations in C++ programs */
#endif

#endif