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

#if HAS_TFT_LVGL_UI

#include "lv_conf.h"
#include "draw_ui.h"
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"

#include "../../../../MarlinCore.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/temperature.h"

extern lv_group_t * g; 
static lv_obj_t * scr;

#define ID_T_PRE_HEAT   1
#define ID_T_EXTRUCT    2
#define ID_T_MOV        3
#define ID_T_HOME       4
#define ID_T_LEVELING   5
#define ID_T_FILAMENT   6
#define ID_T_MORE       7
#define ID_T_RETURN     8
#define ID_T_POSITION   9
#define ID_T_LIGHT      10
#define ID_T_POWER      11

#if ENABLED(MKS_TEST)
  extern uint8_t curent_disp_ui;
#endif

#if HAS_CUTTER
  static lv_obj_t * label_Light = NULL;
  static lv_obj_t * buttonLight = NULL;
#endif

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_T_PRE_HEAT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_tool();
        lv_draw_preHeat();
      }
      break;
    case ID_T_EXTRUCT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_tool();
        lv_draw_extrusion();
      }
      break;
    case ID_T_MOV:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_tool();
        lv_draw_move_motor();
      }
      break;
    case ID_T_HOME:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_tool();
        lv_draw_home();
      }
      break;
    case ID_T_LEVELING:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
          //queue.enqueue_one_P(PSTR("G28"));
          //queue.enqueue_one_P(PSTR("G29"));
          get_gcode_command(AUTO_LEVELING_COMMAND_ADDR,(uint8_t *)public_buf_m);
          public_buf_m[sizeof(public_buf_m)-1] = 0;
          queue.inject_P(PSTR(public_buf_m));
        #else
          uiCfg.leveling_first_time = 1;
          lv_clear_tool();
          lv_draw_manualLevel();
        #endif
      }
      break;
    case ID_T_FILAMENT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
	      uiCfg.desireSprayerTempBak = thermalManager.temp_hotend[uiCfg.curSprayerChoose].target;
        lv_clear_tool();
        lv_draw_filament_change();
      }
      break;
    case ID_T_MORE:
      #if ENABLED(CUSTOM_USER_MENUS)
        if (event == LV_EVENT_CLICKED) {
          // nothing to do
        }
        else if (event == LV_EVENT_RELEASED) {
          lv_clear_tool();
          lv_draw_more();
        }
      #endif
      break;
    case ID_T_RETURN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        TERN_(MKS_TEST, curent_disp_ui = 1);
        lv_clear_tool();
        lv_draw_ready_print();
      }
      break;
      #if HAS_CUTTER
        case ID_T_POSITION:
          if (event == LV_EVENT_CLICKED) {
            // nothing to do
          }
          else if (event == LV_EVENT_RELEASED) {
            queue.enqueue_one_P(PSTR("G92 X0 Y0 Z0"));
          }
          break;
        case ID_T_LIGHT:
          if (event == LV_EVENT_CLICKED) {
            // nothing to do
          }
          else if (event == LV_EVENT_RELEASED) {
            if(uiCfg.lightState) {
              uiCfg.lightState = false;
              lv_imgbtn_set_src(obj, LV_BTN_STATE_REL, "F:/bmp_Light_open.bin");
              lv_imgbtn_set_src(obj, LV_BTN_STATE_PR, "F:/bmp_Light_open.bin");
              lv_label_set_text(label_Light, tool_menu.lightOpen);
              lv_obj_align(label_Light, buttonLight, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
              queue.enqueue_one_P(PSTR("M5"));
            }
            else {
              uiCfg.lightState = true;
              lv_imgbtn_set_src(obj, LV_BTN_STATE_REL, "F:/bmp_Light_close.bin");
              lv_imgbtn_set_src(obj, LV_BTN_STATE_PR, "F:/bmp_Light_close.bin");
              lv_label_set_text(label_Light, tool_menu.lightClose);
              lv_obj_align(label_Light, buttonLight, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
              sprintf_P(public_buf_l, "M3 S%d", uiCfg.spindleLaserPowerPercent);
              queue.enqueue_one_P(PSTR(public_buf_l));
            }
          }
          break;
        case ID_T_POWER:
          if (event == LV_EVENT_CLICKED) {
            // nothing to do
          }
          else if (event == LV_EVENT_RELEASED) {
            lv_clear_tool();
            lv_draw_spindle_laser_power();
          }
          break;
      #endif
  }
}
#if HAS_CUTTER

void lv_draw_tool(void) {
  lv_obj_t *buttonPreHeat, *buttonExtrusion, *buttonMove, *buttonHome, *buttonLevel, *buttonFilament;
  #if ENABLED(CUSTOM_USER_MENUS)
    lv_obj_t *buttonMore;
  #endif
  lv_obj_t *buttonBack;
  lv_obj_t * buttonPosition = NULL;
  lv_obj_t * buttonPower = NULL;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != TOOL_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = TOOL_UI;
  }
  disp_state = TOOL_UI;

  scr = lv_obj_create(NULL, NULL);

  //static lv_style_t tool_style;

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  if(gCfgItems.uiStyle == PRINT_STYLE) {
    /*Create an Image button*/
    buttonPreHeat   = lv_imgbtn_create(scr, NULL);
    buttonExtrusion = lv_imgbtn_create(scr, NULL);
    buttonMove      = lv_imgbtn_create(scr, NULL);
    buttonHome      = lv_imgbtn_create(scr, NULL);
    buttonLevel     = lv_imgbtn_create(scr, NULL);
    buttonFilament  = lv_imgbtn_create(scr, NULL);
    #if ENABLED(CUSTOM_USER_MENUS)
      buttonMore    = lv_imgbtn_create(scr, NULL);
    #endif
    buttonBack      = lv_imgbtn_create(scr, NULL);

    lv_obj_set_event_cb_mks(buttonPreHeat, event_handler, ID_T_PRE_HEAT, NULL, 0);
    lv_imgbtn_set_src(buttonPreHeat, LV_BTN_STATE_REL, "F:/bmp_preHeat.bin");
    lv_imgbtn_set_src(buttonPreHeat, LV_BTN_STATE_PR, "F:/bmp_preHeat.bin");
    lv_imgbtn_set_style(buttonPreHeat, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonPreHeat, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonExtrusion, event_handler, ID_T_EXTRUCT, NULL, 0);
    lv_imgbtn_set_src(buttonExtrusion, LV_BTN_STATE_REL, "F:/bmp_extruct.bin");
    lv_imgbtn_set_src(buttonExtrusion, LV_BTN_STATE_PR, "F:/bmp_extruct.bin");
    lv_imgbtn_set_style(buttonExtrusion, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonExtrusion, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonMove, event_handler, ID_T_MOV, NULL, 0);
    lv_imgbtn_set_src(buttonMove, LV_BTN_STATE_REL, "F:/bmp_mov.bin");
    lv_imgbtn_set_src(buttonMove, LV_BTN_STATE_PR, "F:/bmp_mov.bin");
    lv_imgbtn_set_style(buttonMove, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonMove, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonHome, event_handler, ID_T_HOME, NULL, 0);
    lv_imgbtn_set_src(buttonHome, LV_BTN_STATE_REL, "F:/bmp_zero.bin");
    lv_imgbtn_set_src(buttonHome, LV_BTN_STATE_PR, "F:/bmp_zero.bin");
    lv_imgbtn_set_style(buttonHome, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonHome, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonLevel, event_handler, ID_T_LEVELING, NULL, 0);
    lv_imgbtn_set_src(buttonLevel, LV_BTN_STATE_REL, "F:/bmp_leveling.bin");
    lv_imgbtn_set_src(buttonLevel, LV_BTN_STATE_PR, "F:/bmp_leveling.bin");
    lv_imgbtn_set_style(buttonLevel, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonLevel, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonFilament, event_handler,ID_T_FILAMENT,NULL,0);
    lv_imgbtn_set_src(buttonFilament, LV_BTN_STATE_REL, "F:/bmp_filamentchange.bin");
    lv_imgbtn_set_src(buttonFilament, LV_BTN_STATE_PR, "F:/bmp_filamentchange.bin");
    lv_imgbtn_set_style(buttonFilament, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonFilament, LV_BTN_STATE_REL, &tft_style_label_rel);

    #if ENABLED(CUSTOM_USER_MENUS)
      lv_obj_set_event_cb_mks(buttonMore, event_handler,ID_T_MORE,NULL,0);
      lv_imgbtn_set_src(buttonMore, LV_BTN_STATE_REL, "F:/bmp_more.bin");
      lv_imgbtn_set_src(buttonMore, LV_BTN_STATE_PR, "F:/bmp_more.bin");
      lv_imgbtn_set_style(buttonMore, LV_BTN_STATE_PR, &tft_style_label_pre);
      lv_imgbtn_set_style(buttonMore, LV_BTN_STATE_REL, &tft_style_label_rel);
    #endif

    lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_T_RETURN, NULL, 0);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_return.bin");
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_return.bin");
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
    
    lv_obj_set_pos(buttonPreHeat, INTERVAL_V, titleHeight);
    lv_obj_set_pos(buttonExtrusion, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight);
    lv_obj_set_pos(buttonMove, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight);
    lv_obj_set_pos(buttonHome, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);
    lv_obj_set_pos(buttonLevel, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
    lv_obj_set_pos(buttonFilament,BTN_X_PIXEL+INTERVAL_V*2,BTN_Y_PIXEL+INTERVAL_H+titleHeight);
    #if ENABLED(CUSTOM_USER_MENUS)
    lv_obj_set_pos(buttonMore ,BTN_X_PIXEL*2+INTERVAL_V*3, BTN_Y_PIXEL+INTERVAL_H+titleHeight);
    #endif
    lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

    /*Create a label on the Image button*/
    lv_btn_set_layout(buttonPreHeat, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonExtrusion, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonMove, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonHome, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonLevel, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonFilament, LV_LAYOUT_OFF);
    #if ENABLED(CUSTOM_USER_MENUS)
      lv_btn_set_layout(buttonMore, LV_LAYOUT_OFF);
    #endif
    lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

    lv_obj_t * labelPreHeat   = lv_label_create(buttonPreHeat, NULL);
    lv_obj_t * labelExtrusion = lv_label_create(buttonExtrusion, NULL);
    lv_obj_t * label_Move     = lv_label_create(buttonMove, NULL);
    lv_obj_t * label_Home     = lv_label_create(buttonHome, NULL);
    lv_obj_t * label_Level    = lv_label_create(buttonLevel, NULL);
    lv_obj_t * label_Filament = lv_label_create(buttonFilament, NULL);
    #if ENABLED(CUSTOM_USER_MENUS)
      lv_obj_t * label_More   = lv_label_create(buttonMore, NULL);
    #endif
    lv_obj_t * label_Back     = lv_label_create(buttonBack, NULL);

    if (gCfgItems.multiple_language != 0) {
      lv_label_set_text(labelPreHeat, tool_menu.preheat);
      lv_obj_align(labelPreHeat, buttonPreHeat, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(labelExtrusion, tool_menu.extrude);
      lv_obj_align(labelExtrusion, buttonExtrusion, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Move, tool_menu.move);
      lv_obj_align(label_Move, buttonMove, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Home, tool_menu.home);
      lv_obj_align(label_Home, buttonHome, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Level, tool_menu.TERN(AUTO_BED_LEVELING_BILINEAR, autoleveling, leveling));
      lv_obj_align(label_Level, buttonLevel, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Filament, tool_menu.filament);
      lv_obj_align(label_Filament, buttonFilament, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      #if ENABLED(CUSTOM_USER_MENUS)
        lv_label_set_text(label_More, tool_menu.more);
        lv_obj_align(label_More, buttonMore, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      #endif

      lv_label_set_text(label_Back, common_menu.text_back);
      lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    #if BUTTONS_EXIST(EN1, EN2, ENC)
        if (gCfgItems.encoder_enable == true) {
      lv_group_add_obj(g, buttonPreHeat);
      lv_group_add_obj(g, buttonExtrusion);
      lv_group_add_obj(g, buttonMove);
      lv_group_add_obj(g, buttonHome);
      lv_group_add_obj(g, buttonLevel);
      lv_group_add_obj(g, buttonFilament);
      #if ENABLED(CUSTOM_USER_MENUS)
        lv_group_add_obj(g, buttonMore);
      #endif
      lv_group_add_obj(g, buttonBack);
    }
    #endif // BUTTONS_EXIST(EN1, EN2, ENC)
  }
  else {
    buttonHome     = lv_imgbtn_create(scr, NULL);
    buttonPosition = lv_imgbtn_create(scr, NULL);
    buttonMove     = lv_imgbtn_create(scr, NULL);
    buttonLight    = lv_imgbtn_create(scr, NULL);
    buttonPower    = lv_imgbtn_create(scr, NULL);
    buttonBack     = lv_imgbtn_create(scr, NULL);

    lv_obj_set_event_cb_mks(buttonHome, event_handler, ID_T_HOME, NULL, 0);
    lv_imgbtn_set_src(buttonHome, LV_BTN_STATE_REL, "F:/bmp_zero.bin");
    lv_imgbtn_set_src(buttonHome, LV_BTN_STATE_PR, "F:/bmp_zero.bin");
    lv_imgbtn_set_style(buttonHome, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonHome, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonPosition, event_handler, ID_T_POSITION, NULL, 0);
    lv_imgbtn_set_src(buttonPosition, LV_BTN_STATE_REL, "F:/bmp_Position.bin");
    lv_imgbtn_set_src(buttonPosition, LV_BTN_STATE_PR, "F:/bmp_Position.bin");
    lv_imgbtn_set_style(buttonPosition, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonPosition, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonMove, event_handler, ID_T_MOV, NULL, 0);
    lv_imgbtn_set_src(buttonMove, LV_BTN_STATE_REL, "F:/bmp_mov.bin");
    lv_imgbtn_set_src(buttonMove, LV_BTN_STATE_PR, "F:/bmp_mov.bin");
    lv_imgbtn_set_style(buttonMove, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonMove, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonLight, event_handler, ID_T_LIGHT, NULL, 0);
    if(uiCfg.lightState) {
      lv_imgbtn_set_src(buttonLight, LV_BTN_STATE_REL, "F:/bmp_Light_close.bin");
      lv_imgbtn_set_src(buttonLight, LV_BTN_STATE_PR, "F:/bmp_Light_close.bin");
    }
    else {
      lv_imgbtn_set_src(buttonLight, LV_BTN_STATE_REL, "F:/bmp_Light_open.bin");
      lv_imgbtn_set_src(buttonLight, LV_BTN_STATE_PR, "F:/bmp_Light_open.bin");
    }
    lv_imgbtn_set_style(buttonLight, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonLight, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonPower, event_handler, ID_T_POWER, NULL, 0);
    lv_imgbtn_set_src(buttonPower, LV_BTN_STATE_REL, "F:/bmp_Power.bin");
    lv_imgbtn_set_src(buttonPower, LV_BTN_STATE_PR, "F:/bmp_Power.bin");
    lv_imgbtn_set_style(buttonPower, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonPower, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_T_RETURN, NULL, 0);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_return.bin");
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_return.bin");
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_pos(buttonHome, INTERVAL_V, titleHeight);
    lv_obj_set_pos(buttonPosition, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight);
    lv_obj_set_pos(buttonMove, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight);
    lv_obj_set_pos(buttonLight, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);
    lv_obj_set_pos(buttonPower, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
    lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

    lv_btn_set_layout(buttonHome, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonPosition, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonMove, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonLight, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonPower, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

    lv_obj_t * labelHome      = lv_label_create(buttonHome, NULL);
    lv_obj_t * labelPosition  = lv_label_create(buttonPosition, NULL);
    lv_obj_t * label_Move     = lv_label_create(buttonMove, NULL);
    label_Light               = lv_label_create(buttonLight, NULL);
    lv_obj_t * label_Power    = lv_label_create(buttonPower, NULL);
    lv_obj_t * label_Back     = lv_label_create(buttonBack, NULL);

    if (gCfgItems.multiple_language != 0) {
      lv_label_set_text(labelHome, tool_menu.home);
      lv_obj_align(labelHome, buttonHome, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(labelPosition, tool_menu.position);
      lv_obj_align(labelPosition, buttonPosition, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Move, tool_menu.move);
      lv_obj_align(label_Move, buttonMove, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      if(uiCfg.lightState) lv_label_set_text(label_Light, tool_menu.lightClose);
      else lv_label_set_text(label_Light, tool_menu.lightOpen);
      
      lv_obj_align(label_Light, buttonLight, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Power, tool_menu.power);
      lv_obj_align(label_Power, buttonPower, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Back, common_menu.text_back);
      lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    #if BUTTONS_EXIST(EN1, EN2, ENC)
      if (gCfgItems.encoder_enable == true) {
        lv_group_add_obj(g, buttonHome);
        lv_group_add_obj(g, buttonPosition);
        lv_group_add_obj(g, buttonMove);
        lv_group_add_obj(g, buttonLight);
        lv_group_add_obj(g, buttonPower);
        lv_group_add_obj(g, buttonBack);
      }
    #endif // BUTTONS_EXIST(EN1, EN2, ENC)
  }
}

#else

  void lv_draw_tool(void) {
    lv_obj_t *buttonPreHeat, *buttonExtrusion, *buttonMove, *buttonHome, *buttonLevel, *buttonFilament;
    #if ENABLED(CUSTOM_USER_MENUS)
      lv_obj_t *buttonMore;
    #endif
    lv_obj_t *buttonBack;

    if (disp_state_stack._disp_state[disp_state_stack._disp_index] != TOOL_UI) {
      disp_state_stack._disp_index++;
      disp_state_stack._disp_state[disp_state_stack._disp_index] = TOOL_UI;
    }
    disp_state = TOOL_UI;

    scr = lv_obj_create(NULL, NULL);

    //static lv_style_t tool_style;

    lv_obj_set_style(scr, &tft_style_scr);
    lv_scr_load(scr);
    lv_obj_clean(scr);

    lv_obj_t * title = lv_label_create(scr, NULL);
    lv_obj_set_style(title, &tft_style_label_rel);
    lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
    lv_label_set_text(title, creat_title_text());

    lv_refr_now(lv_refr_get_disp_refreshing());

    /*Create an Image button*/
    buttonPreHeat   = lv_imgbtn_create(scr, NULL);
    buttonExtrusion = lv_imgbtn_create(scr, NULL);
    buttonMove      = lv_imgbtn_create(scr, NULL);
    buttonHome      = lv_imgbtn_create(scr, NULL);
    buttonLevel     = lv_imgbtn_create(scr, NULL);
    buttonFilament  = lv_imgbtn_create(scr, NULL);
    #if ENABLED(CUSTOM_USER_MENUS)
      buttonMore    = lv_imgbtn_create(scr, NULL);
    #endif
    buttonBack      = lv_imgbtn_create(scr, NULL);

    lv_obj_set_event_cb_mks(buttonPreHeat, event_handler, ID_T_PRE_HEAT, NULL, 0);
    lv_imgbtn_set_src(buttonPreHeat, LV_BTN_STATE_REL, "F:/bmp_preHeat.bin");
    lv_imgbtn_set_src(buttonPreHeat, LV_BTN_STATE_PR, "F:/bmp_preHeat.bin");
    lv_imgbtn_set_style(buttonPreHeat, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonPreHeat, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonExtrusion, event_handler, ID_T_EXTRUCT, NULL, 0);
    lv_imgbtn_set_src(buttonExtrusion, LV_BTN_STATE_REL, "F:/bmp_extruct.bin");
    lv_imgbtn_set_src(buttonExtrusion, LV_BTN_STATE_PR, "F:/bmp_extruct.bin");
    lv_imgbtn_set_style(buttonExtrusion, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonExtrusion, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonMove, event_handler, ID_T_MOV, NULL, 0);
    lv_imgbtn_set_src(buttonMove, LV_BTN_STATE_REL, "F:/bmp_mov.bin");
    lv_imgbtn_set_src(buttonMove, LV_BTN_STATE_PR, "F:/bmp_mov.bin");
    lv_imgbtn_set_style(buttonMove, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonMove, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonHome, event_handler, ID_T_HOME, NULL, 0);
    lv_imgbtn_set_src(buttonHome, LV_BTN_STATE_REL, "F:/bmp_zero.bin");
    lv_imgbtn_set_src(buttonHome, LV_BTN_STATE_PR, "F:/bmp_zero.bin");
    lv_imgbtn_set_style(buttonHome, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonHome, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonLevel, event_handler, ID_T_LEVELING, NULL, 0);
    lv_imgbtn_set_src(buttonLevel, LV_BTN_STATE_REL, "F:/bmp_leveling.bin");
    lv_imgbtn_set_src(buttonLevel, LV_BTN_STATE_PR, "F:/bmp_leveling.bin");
    lv_imgbtn_set_style(buttonLevel, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonLevel, LV_BTN_STATE_REL, &tft_style_label_rel);
    

    lv_obj_set_event_cb_mks(buttonFilament, event_handler,ID_T_FILAMENT,NULL,0);
    lv_imgbtn_set_src(buttonFilament, LV_BTN_STATE_REL, "F:/bmp_filamentchange.bin");
    lv_imgbtn_set_src(buttonFilament, LV_BTN_STATE_PR, "F:/bmp_filamentchange.bin");
    lv_imgbtn_set_style(buttonFilament, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonFilament, LV_BTN_STATE_REL, &tft_style_label_rel);

    #if ENABLED(CUSTOM_USER_MENUS)
      lv_obj_set_event_cb_mks(buttonMore, event_handler,ID_T_MORE,NULL,0);
      lv_imgbtn_set_src(buttonMore, LV_BTN_STATE_REL, "F:/bmp_more.bin");
      lv_imgbtn_set_src(buttonMore, LV_BTN_STATE_PR, "F:/bmp_more.bin");
      lv_imgbtn_set_style(buttonMore, LV_BTN_STATE_PR, &tft_style_label_pre);
      lv_imgbtn_set_style(buttonMore, LV_BTN_STATE_REL, &tft_style_label_rel);
    #endif

    lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_T_RETURN, NULL, 0);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_return.bin");
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_return.bin");
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
    
    lv_obj_set_pos(buttonPreHeat, INTERVAL_V, titleHeight);
    lv_obj_set_pos(buttonExtrusion, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight);
    lv_obj_set_pos(buttonMove, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight);
    lv_obj_set_pos(buttonHome, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);
    lv_obj_set_pos(buttonLevel, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
    lv_obj_set_pos(buttonFilament,BTN_X_PIXEL+INTERVAL_V*2,BTN_Y_PIXEL+INTERVAL_H+titleHeight);
    #if ENABLED(CUSTOM_USER_MENUS)
    lv_obj_set_pos(buttonMore ,BTN_X_PIXEL*2+INTERVAL_V*3, BTN_Y_PIXEL+INTERVAL_H+titleHeight);
    #endif
    lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

    /*Create a label on the Image button*/
    lv_btn_set_layout(buttonPreHeat, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonExtrusion, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonMove, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonHome, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonLevel, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonFilament, LV_LAYOUT_OFF);
    #if ENABLED(CUSTOM_USER_MENUS)
      lv_btn_set_layout(buttonMore, LV_LAYOUT_OFF);
    #endif
    lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

    lv_obj_t * labelPreHeat   = lv_label_create(buttonPreHeat, NULL);
    lv_obj_t * labelExtrusion = lv_label_create(buttonExtrusion, NULL);
    lv_obj_t * label_Move     = lv_label_create(buttonMove, NULL);
    lv_obj_t * label_Home     = lv_label_create(buttonHome, NULL);
    lv_obj_t * label_Level    = lv_label_create(buttonLevel, NULL);
    lv_obj_t * label_Filament = lv_label_create(buttonFilament, NULL);
    #if ENABLED(CUSTOM_USER_MENUS)
      lv_obj_t * label_More   = lv_label_create(buttonMore, NULL);
    #endif
    lv_obj_t * label_Back     = lv_label_create(buttonBack, NULL);

    if (gCfgItems.multiple_language != 0) {
      lv_label_set_text(labelPreHeat, tool_menu.preheat);
      lv_obj_align(labelPreHeat, buttonPreHeat, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(labelExtrusion, tool_menu.extrude);
      lv_obj_align(labelExtrusion, buttonExtrusion, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Move, tool_menu.move);
      lv_obj_align(label_Move, buttonMove, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Home, tool_menu.home);
      lv_obj_align(label_Home, buttonHome, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Level, tool_menu.TERN(AUTO_BED_LEVELING_BILINEAR, autoleveling, leveling));
      lv_obj_align(label_Level, buttonLevel, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Filament, tool_menu.filament);
      lv_obj_align(label_Filament, buttonFilament, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      #if ENABLED(CUSTOM_USER_MENUS)
        lv_label_set_text(label_More, tool_menu.more);
        lv_obj_align(label_More, buttonMore, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      #endif

      lv_label_set_text(label_Back, common_menu.text_back);
      lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    #if BUTTONS_EXIST(EN1, EN2, ENC)
        if (gCfgItems.encoder_enable == true) {
      lv_group_add_obj(g, buttonPreHeat);
      lv_group_add_obj(g, buttonExtrusion);
      lv_group_add_obj(g, buttonMove);
      lv_group_add_obj(g, buttonHome);
      lv_group_add_obj(g, buttonLevel);
      lv_group_add_obj(g, buttonFilament);
      #if ENABLED(CUSTOM_USER_MENUS)
        lv_group_add_obj(g, buttonMore);
      #endif
      lv_group_add_obj(g, buttonBack);
    }
    #endif // BUTTONS_EXIST(EN1, EN2, ENC)
  }
#endif

void lv_clear_tool() { 
	#if BUTTONS_EXIST(EN1, EN2, ENC)
    	if (gCfgItems.encoder_enable == true) {
		lv_group_remove_all_objs(g);
	}
  	#endif // BUTTONS_EXIST(EN1, EN2, ENC)
	lv_obj_del(scr); 
}

#endif // HAS_TFT_LVGL_UI
