/**
 * @file tft_lvgl_configuration.h
 * @date    2020-02-21
 * */

#ifndef TFT_LVGL_CONFIGURATION_H
#define TFT_LVGL_CONFIGURATION_H

//#ifdef __cplusplus
//extern "C" {
//#endif

#include "lvgl.h"

void tft_lvgl_init();
void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p);
bool my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data);

void LCD_Clear(uint16_t  Color);
void tft_set_point(uint16_t x,uint16_t y,uint16_t point);
void LV_TASK_HANDLER();
void ili9320_SetWindows(uint16_t StartX,uint16_t StartY,uint16_t width,uint16_t heigh);
void LCD_WriteRAM_Prepare(void);



#ifndef USE_XPT2046
#define USE_XPT2046         1
#define XPT2046_XY_SWAP  	1
#define XPT2046_X_INV   	0
#define XPT2046_Y_INV   	1
#endif

#if USE_XPT2046
#define XPT2046_HOR_RES     480
#define XPT2046_VER_RES     320
#define XPT2046_X_MIN       201 
#define XPT2046_Y_MIN       164
#define XPT2046_X_MAX       3919
#define XPT2046_Y_MAX       3776
#define XPT2046_AVG         4
#define XPT2046_INV         0
#endif

//#ifdef __cplusplus
//} /* extern "C" */
//#endif

#endif