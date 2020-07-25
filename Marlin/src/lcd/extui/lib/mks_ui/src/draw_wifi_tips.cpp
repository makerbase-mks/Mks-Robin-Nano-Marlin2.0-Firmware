#include "../../../../../MarlinCore.h"
#if ENABLED(TFT_LITTLE_VGL_UI)
#include "lv_conf.h"
//#include "../../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../../lvgl/src/lv_objx/lv_img.h"
//#include "../../lvgl/src/lv_core/lv_disp.h"
//#include "../../lvgl/src/lv_core/lv_refr.h"
//#include "../../MarlinCore.h"
#include "../../../../../../Configuration.h"
#include "../inc/draw_ui.h"
#include "../../../../../module/temperature.h"

static lv_obj_t * scr;

TIPS_TYPE wifi_tips_type;
TIPS_DISP tips_disp;
tips_menu_def tips_menu;

void lv_draw_wifi_tips(void)
{
	static lv_obj_t * text_tips,*wifi_name;
	
	if(disp_state_stack._disp_state[disp_state_stack._disp_index] != WIFI_TIPS_UI)
	{
		disp_state_stack._disp_index++;
		disp_state_stack._disp_state[disp_state_stack._disp_index] = WIFI_TIPS_UI;
	}
	disp_state = WIFI_TIPS_UI;

	scr = lv_obj_create(NULL, NULL);
	
	lv_obj_set_style(scr, &tft_style_scr);
  	lv_scr_load(scr);
  	lv_obj_clean(scr);
  
  	lv_refr_now(lv_refr_get_disp_refreshing());

	text_tips = lv_label_create(scr, NULL);
	lv_obj_set_style(text_tips, &tft_style_lable_rel);
	

	wifi_name = lv_label_create(scr, NULL);
	lv_obj_set_style(wifi_name, &tft_style_lable_rel);

	if(wifi_tips_type == TIPS_TYPE_JOINING)
	{
		lv_label_set_text(text_tips, tips_menu.joining);
		lv_obj_align(text_tips, NULL, LV_ALIGN_CENTER,0, -60);
	}
	else if(wifi_tips_type == TIPS_TYPE_TAILED_JOIN)
	{
		lv_label_set_text(text_tips, tips_menu.failedJoin);
		lv_obj_align(text_tips, NULL, LV_ALIGN_CENTER,0, -60);
	}
	else if(wifi_tips_type == TIPS_TYPE_WIFI_CONECTED)
	{
		lv_label_set_text(text_tips, tips_menu.wifiConected);
		lv_obj_align(text_tips, NULL, LV_ALIGN_CENTER,0, -60);
	}
	
	lv_label_set_text(wifi_name, (const char *)wifi_list.wifiName[wifi_list.nameIndex]);
	lv_obj_align(wifi_name, NULL, LV_ALIGN_CENTER,0, -20);

	tips_disp.timer = TIPS_TIMER_START;
	tips_disp.timer_count = 0;
}

void lv_clear_wifi_tips()
{
	lv_obj_del(scr);
}
#endif