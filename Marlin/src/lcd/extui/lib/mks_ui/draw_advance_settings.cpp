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

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;
#if ENABLED(DUAL_X_CARRIAGE)
  static lv_obj_t * xMode = nullptr;
  static lv_obj_t * hotendOffset = nullptr;
#endif

enum {
  ID_ADVANCE_RETURN = 1,
  ID_PAUSE_POS,
  ID_WIFI_PARA,
  ID_FILAMENT_SETTINGS,
  ID_ENCODER_SETTINGS,
  ID_X_MODE_SETTINGS,
  ID_T_OFFSET_SETTINGS,
  ID_DUAL_X_MODE_SET_UP,
  ID_DUAL_X_MODE_SET_DOWN
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_ADVANCE_RETURN:
      uiCfg.para_ui_page = 0;
      lv_clear_advance_settings();
      lv_draw_return_ui();
      break;
    case ID_PAUSE_POS:
      uiCfg.para_ui_page = 0;
      lv_clear_advance_settings();
      lv_draw_pause_position();
      break;
    case ID_FILAMENT_SETTINGS:
      uiCfg.para_ui_page = 0;
      lv_clear_advance_settings();
      lv_draw_filament_settings();
      break;
    #if ENABLED(MKS_WIFI_MODULE)
      case ID_WIFI_PARA:
      uiCfg.para_ui_page = 0;
      lv_clear_advance_settings();
      lv_draw_wifi_settings();
      break;
    #endif
    #if HAS_ROTARY_ENCODER
      case ID_ENCODER_SETTINGS:
        uiCfg.para_ui_page = 0;
        lv_clear_advance_settings();
        lv_draw_encoder_settings();
        break;
    #endif
    #if ENABLED(DUAL_X_CARRIAGE)
      case ID_X_MODE_SETTINGS:
        uiCfg.para_ui_page = 0;
        lv_clear_advance_settings();
        lv_draw_dual_x_carriage_mode();
        break;
      case ID_T_OFFSET_SETTINGS:
        uiCfg.para_ui_page = 0;
        lv_clear_advance_settings();
        lv_draw_hotend_offset_settings();
        break;
      case ID_DUAL_X_MODE_SET_UP:
        uiCfg.para_ui_page = 0;
        lv_clear_advance_settings();
        lv_draw_advance_settings();
        break;
      case ID_DUAL_X_MODE_SET_DOWN:
        uiCfg.para_ui_page = 1;
        lv_clear_advance_settings();
        lv_draw_advance_settings();
        break;
    #endif
  }
}

void lv_draw_advance_settings(void) {
  int item_index = 0;
  scr = lv_screen_create(ADVANCED_UI, machine_menu.AdvancedConfTitle);
  if (uiCfg.para_ui_page != 1) {
    #if ENABLED(DUAL_X_CARRIAGE)
      xMode = nullptr;
      hotendOffset = nullptr;
    #endif
    lv_screen_menu_item(scr, machine_menu.PausePosition, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_PAUSE_POS, item_index++);
    lv_screen_menu_item(scr, machine_menu.FilamentConf, PARA_UI_POS_X, PARA_UI_POS_Y * (item_index + 1), event_handler, ID_FILAMENT_SETTINGS, item_index);
    item_index++;
    #if ENABLED(MKS_WIFI_MODULE)
      lv_screen_menu_item(scr, machine_menu.WifiSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (item_index + 1), event_handler, ID_WIFI_PARA, item_index);
      item_index++;
      #if HAS_ROTARY_ENCODER
        lv_screen_menu_item(scr, machine_menu.EncoderSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (item_index + 1), event_handler, ID_ENCODER_SETTINGS, item_index);
        item_index++;
        #if ENABLED(DUAL_X_CARRIAGE)
          lv_screen_menu_item_turn_page(scr, machine_menu.next, event_handler, ID_DUAL_X_MODE_SET_DOWN);
        #endif
      #elif ENABLED(DUAL_X_CARRIAGE)
        xMode = lv_screen_menu_item(scr, machine_menu.xModeSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (item_index + 1), event_handler, ID_X_MODE_SETTINGS, item_index);
        item_index++;
        lv_screen_menu_item_turn_page(scr, machine_menu.next, event_handler, ID_DUAL_X_MODE_SET_DOWN);
      #endif
    #elif HAS_ROTARY_ENCODER
      lv_screen_menu_item(scr, machine_menu.EncoderSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (item_index + 1), event_handler, ID_ENCODER_SETTINGS, item_index);
      item_index++;
      #if ENABLED(DUAL_X_CARRIAGE)
        xMode = lv_screen_menu_item(scr, machine_menu.xModeSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (item_index + 1), event_handler, ID_X_MODE_SETTINGS, item_index);
        lv_screen_menu_item_turn_page(scr, machine_menu.next, event_handler, ID_DUAL_X_MODE_SET_DOWN);
      #endif
    #elif ENABLED(DUAL_X_CARRIAGE)
      xMode = lv_screen_menu_item(scr, machine_menu.xModeSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (item_index + 1), event_handler, ID_X_MODE_SETTINGS, item_index);
      item_index++;
      hotendOffset = lv_screen_menu_item(scr, machine_menu.hotendOffsetSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (item_index + 1), event_handler, ID_T_OFFSET_SETTINGS, item_index);
    #endif
  }
  else {
    #if ENABLED(DUAL_X_CARRIAGE)
      if (xMode == nullptr) {
        xMode = lv_screen_menu_item(scr, machine_menu.xModeSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (item_index + 1), event_handler, ID_X_MODE_SETTINGS, item_index);
        item_index++;
        hotendOffset = lv_screen_menu_item(scr, machine_menu.hotendOffsetSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (item_index + 1), event_handler, ID_T_OFFSET_SETTINGS, item_index);
      }
      else if (hotendOffset == nullptr) {
        hotendOffset = lv_screen_menu_item(scr, machine_menu.hotendOffsetSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (item_index + 1), event_handler, ID_T_OFFSET_SETTINGS, item_index);
      }
      lv_screen_menu_item_turn_page(scr, machine_menu.previous, event_handler, ID_DUAL_X_MODE_SET_UP);
    #endif
  }

  lv_screen_menu_item_return(scr, event_handler, ID_ADVANCE_RETURN);
}

void lv_clear_advance_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
