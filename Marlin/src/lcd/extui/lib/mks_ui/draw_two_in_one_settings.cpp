/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "../../../../inc/MarlinConfigPre.h"

#if BOTH(HAS_TFT_LVGL_UI, HAS_CUTTER)

#include "../../../../MarlinCore.h"
#include "lv_conf.h"
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"
#include "../../../../../Configuration.h"
#include "../../../../module/servo.h"
#include "draw_ui.h"

extern lv_group_t * g;
static lv_obj_t * scr;

#define ID_PRINTER_FUNCTION     1 
#define ID_ENGRAVING_FUNCTION   2
#define ID_ENGRAVING_RETURN     3

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_PRINTER_FUNCTION:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        gCfgItems.uiStyle = PRINT_STYLE;
        disp_language_init();
        lv_clear_cur_ui();
        lv_draw_ready_print();
        #if HAS_SERVOS
          servo_init();
        #endif
        update_spi_flash();
      }
      break;
    case ID_ENGRAVING_FUNCTION:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        gCfgItems.uiStyle = LASER_STYLE;
        disp_language_init();
        lv_clear_cur_ui();
        lv_draw_ready_print();
        #if ENABLED(SPINDLE_LASER_USES_SOFT_PWM)
          spindleLaserSetupSoftPWM();
          spindleLaserSoftPwmSetDuty(0);
        #endif
        update_spi_flash();
        break;
      }
    case ID_ENGRAVING_RETURN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_cur_ui();
        lv_draw_return_ui();
      }
      break;
  }
}

void lv_draw_two_in_one_settings(void) {
  lv_obj_t *buttonPrint, *buttonLaser, *buttonBack;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != TWO_IN_ON_FUNCTION_SET_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = TWO_IN_ON_FUNCTION_SET_UI;
  }
  disp_state = TWO_IN_ON_FUNCTION_SET_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());


  /*Create an Image button*/
  buttonPrint = lv_imgbtn_create(scr, NULL);
  buttonLaser = lv_imgbtn_create(scr, NULL);
  buttonBack = lv_imgbtn_create(scr, NULL);

  lv_obj_set_event_cb_mks(buttonPrint, event_handler, ID_PRINTER_FUNCTION, NULL, 0);
  lv_imgbtn_set_src(buttonPrint, LV_BTN_STATE_REL, "F:/bmp_Print_ui.bin");
  lv_imgbtn_set_src(buttonPrint, LV_BTN_STATE_PR, "F:/bmp_Print_ui.bin");
  lv_imgbtn_set_style(buttonPrint, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonPrint, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_obj_clear_protect(buttonPrint, LV_PROTECT_FOLLOW);
  
  lv_obj_set_event_cb_mks(buttonLaser, event_handler, ID_ENGRAVING_FUNCTION, NULL, 0);
  lv_imgbtn_set_src(buttonLaser, LV_BTN_STATE_REL, "F:/bmp_Laser_ui.bin");
  lv_imgbtn_set_src(buttonLaser, LV_BTN_STATE_PR, "F:/bmp_Laser_ui.bin");
  lv_imgbtn_set_style(buttonLaser, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonLaser, LV_BTN_STATE_REL, &tft_style_label_rel);
  
  lv_obj_set_event_cb_mks(buttonBack, event_handler,ID_ENGRAVING_RETURN, NULL,0);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_return.bin");
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_return.bin");
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
    
  lv_obj_set_pos(buttonPrint, INTERVAL_V, titleHeight);
  lv_obj_set_pos(buttonLaser, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight);
  lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

  /*Create a label on the Image button*/
  lv_btn_set_layout(buttonPrint, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonLaser, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

  lv_obj_t * labelPrint= lv_label_create(buttonPrint, NULL);
  lv_obj_t * labelLaser = lv_label_create(buttonLaser, NULL);
  lv_obj_t * label_Back = lv_label_create(buttonBack, NULL);


  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(labelPrint, twoInOneSet_menu.print_ui);
    lv_obj_align(labelPrint, buttonPrint, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelLaser, twoInOneSet_menu.laser_ui);
    lv_obj_align(labelLaser, buttonLaser, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }
  #if BUTTONS_EXIST(EN1, EN2, ENC)
	if (gCfgItems.encoder_enable == true) {
		lv_group_add_obj(g, buttonPrint);
  	lv_group_add_obj(g, buttonLaser);
		lv_group_add_obj(g, buttonBack);
	}
  #endif // BUTTONS_EXIST(EN1, EN2, ENC)
}

void lv_clear_two_in_one_settings() {
	#if BUTTONS_EXIST(EN1, EN2, ENC)
	if (gCfgItems.encoder_enable == true) {
		lv_group_remove_all_objs(g);
	}
  	#endif // BUTTONS_EXIST(EN1, EN2, ENC)
	lv_obj_del(scr); 
}

#endif // HAS_TFT_LVGL_UI
