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
#include "../../../../../Configuration.h"
#include "../../../../module/temperature.h"
#include "../../../../gcode/queue.h"
#include "../../../../gcode/gcode.h"

#include "lv_conf.h"
#include "draw_ui.h"

extern lv_group_t * g;
static lv_obj_t * scr;
static lv_obj_t * powerText;
static lv_obj_t * labelStep;
static lv_obj_t * buttonStep;

#define ID_POWER_ADD          1
#define ID_POWER_DEC          2
#define ID_POWER_PERCENT_STEP 3
#define ID_POWER_OFF          4
#define ID_POWER_RETURN       5

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_POWER_ADD:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if(uiCfg.spindleLaserPowerPercent < 100 - uiCfg.powerStep) {
          uiCfg.spindleLaserPowerPercent += uiCfg.powerStep;
        }
        else {
          uiCfg.spindleLaserPowerPercent = 100;
        }
        disp_power_percent();
      }
      break;
    case ID_POWER_DEC:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if(uiCfg.spindleLaserPowerPercent > uiCfg.powerStep) {
          uiCfg.spindleLaserPowerPercent -= uiCfg.powerStep;
        }
        else {
          uiCfg.spindleLaserPowerPercent = 0 ;
        }
        disp_power_percent();
      }

      break;
    case ID_POWER_PERCENT_STEP:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if(uiCfg.powerStep == 1) {
          uiCfg.powerStep = 10;
          lv_imgbtn_set_src(obj, LV_BTN_STATE_REL, "F:/bmp_step10_percent.bin");
          lv_imgbtn_set_src(obj, LV_BTN_STATE_PR, "F:/bmp_step10_percent.bin");
          lv_label_set_text(labelStep, speed_menu.step_10percent);
          lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
				}
        else {
          uiCfg.powerStep = 1;
          lv_imgbtn_set_src(obj, LV_BTN_STATE_REL, "F:/bmp_step1_percent.bin");
          lv_imgbtn_set_src(obj, LV_BTN_STATE_PR, "F:/bmp_step1_percent.bin");
          lv_label_set_text(labelStep, speed_menu.step_1percent);
          lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
        }
      }
      break;
    case ID_POWER_OFF:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.spindleLaserPowerPercent = 0 ;
        disp_power_percent();
      }
      break;
    case ID_POWER_RETURN:
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

void lv_draw_spindle_laser_power(void) {
  lv_obj_t *buttonAdd, *buttonDec;
  lv_obj_t *buttonOff, *buttonBack;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != SPINDLE_LASER_POWER_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = SPINDLE_LASER_POWER_UI;
  }
  disp_state = SPINDLE_LASER_POWER_UI;

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
  buttonAdd  = lv_imgbtn_create(scr, NULL);
  buttonDec  = lv_imgbtn_create(scr, NULL);
  buttonStep = lv_imgbtn_create(scr, NULL);
  buttonOff  = lv_imgbtn_create(scr, NULL);
  buttonBack = lv_imgbtn_create(scr, NULL);

  lv_obj_set_event_cb_mks(buttonAdd, event_handler, ID_POWER_ADD, NULL, 0);
  lv_imgbtn_set_src(buttonAdd, LV_BTN_STATE_REL, "F:/bmp_Add.bin");
  lv_imgbtn_set_src(buttonAdd, LV_BTN_STATE_PR, "F:/bmp_Add.bin");
  lv_imgbtn_set_style(buttonAdd, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonAdd, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_obj_clear_protect(buttonAdd, LV_PROTECT_FOLLOW);
  
  lv_obj_set_event_cb_mks(buttonDec, event_handler, ID_POWER_DEC, NULL, 0);
  lv_imgbtn_set_src(buttonDec, LV_BTN_STATE_REL, "F:/bmp_Dec.bin");
  lv_imgbtn_set_src(buttonDec, LV_BTN_STATE_PR, "F:/bmp_Dec.bin");
  lv_imgbtn_set_style(buttonDec, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonDec, LV_BTN_STATE_REL, &tft_style_label_rel);

  lv_obj_set_event_cb_mks(buttonStep, event_handler, ID_POWER_PERCENT_STEP, NULL, 0);
  if(uiCfg.powerStep == 1) {
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_REL, "F:/bmp_step1_percent.bin");
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_PR, "F:/bmp_step1_percent.bin");
  }
  else {
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_REL, "F:/bmp_step10_percent.bin");
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_PR, "F:/bmp_step10_percent.bin");
  }
  lv_imgbtn_set_style(buttonStep, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonStep, LV_BTN_STATE_REL, &tft_style_label_rel);
  
  lv_obj_set_event_cb_mks(buttonOff, event_handler,ID_POWER_OFF, NULL,0);
  lv_imgbtn_set_src(buttonOff, LV_BTN_STATE_REL, "F:/bmp_speed0.bin");
  lv_imgbtn_set_src(buttonOff, LV_BTN_STATE_PR, "F:/bmp_speed0.bin");
  lv_imgbtn_set_style(buttonOff, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonOff, LV_BTN_STATE_REL, &tft_style_label_rel);
  
  lv_obj_set_event_cb_mks(buttonBack, event_handler,ID_POWER_RETURN, NULL,0);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_return.bin");
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_return.bin");
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
    
  lv_obj_set_pos(buttonAdd, INTERVAL_V, titleHeight);
  lv_obj_set_pos(buttonDec, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);
  lv_obj_set_pos(buttonOff, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonStep, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

  /*Create a label on the Image button*/
  lv_btn_set_layout(buttonAdd, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonDec, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonStep, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonOff, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

  lv_obj_t * labelAdd   = lv_label_create(buttonAdd, NULL);
  lv_obj_t * labelDec   = lv_label_create(buttonDec, NULL);
  labelStep             = lv_label_create(buttonStep, NULL);
  lv_obj_t * labelOff   = lv_label_create(buttonOff, NULL);
  lv_obj_t * label_Back = lv_label_create(buttonBack, NULL);


  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(labelAdd, fan_menu.add);
    lv_obj_align(labelAdd, buttonAdd, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelDec, fan_menu.dec);
    lv_obj_align(labelDec, buttonDec, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    if(uiCfg.powerStep == 1) {
      lv_label_set_text(labelStep, speed_menu.step_1percent);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else {
      lv_label_set_text(labelStep, speed_menu.step_10percent);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }

    lv_label_set_text(labelOff, fan_menu.off);
    lv_obj_align(labelOff, buttonOff, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }
  #if BUTTONS_EXIST(EN1, EN2, ENC)
    if (gCfgItems.encoder_enable == true) {
      lv_group_add_obj(g, buttonAdd);
      lv_group_add_obj(g, buttonDec);
      lv_group_add_obj(g, buttonStep);
      lv_group_add_obj(g, buttonOff);
      lv_group_add_obj(g, buttonBack);
    }
  #endif // BUTTONS_EXIST(EN1, EN2, ENC)

  powerText = lv_label_create(scr, NULL);
  lv_obj_set_style(powerText, &tft_style_label_rel);
  disp_power_percent();
}

void disp_power_percent() {
  char buf1[10] = {0};
  public_buf_l[0] = '\0';
  strcat(public_buf_l, spindle_laser_menu.power_percent_state);
  strcat_P(public_buf_l, PSTR(": "));
  if(uiCfg.spindleLaserPowerPercent <= 0) strcat(buf1, PSTR("OFF"));
  else sprintf_P(buf1, PSTR("%3d%%"), uiCfg.spindleLaserPowerPercent);
  strcat(public_buf_l, buf1);
  lv_label_set_text(powerText, public_buf_l);
  lv_obj_align(powerText, NULL, LV_ALIGN_CENTER, 0, -65);
}

void lv_clear_spindle_laser_power() {
	#if BUTTONS_EXIST(EN1, EN2, ENC)
    if (gCfgItems.encoder_enable == true) {
      lv_group_remove_all_objs(g);
    }
  #endif // BUTTONS_EXIST(EN1, EN2, ENC)
	lv_obj_del(scr); 
}

#endif // HAS_TFT_LVGL_UI
