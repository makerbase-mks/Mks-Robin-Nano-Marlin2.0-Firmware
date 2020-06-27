#ifndef MKS_HARDWARE_TEST_H
#define MKS_HARDWARE_TEST_H

void mks_gpio_test();
void disp_char_1624(uint16_t x,uint16_t y,uint8_t c,uint16_t charColor,uint16_t bkColor);
void disp_string(uint16_t x,uint16_t y,const char * string,uint16_t charColor,uint16_t bkColor);
void mks_hardware_test();
void disp_pic_update();
void disp_font_update();
void mks_test_get();

extern uint8_t mks_test_flag;

#endif

