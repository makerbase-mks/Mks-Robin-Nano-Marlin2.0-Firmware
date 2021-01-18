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

#include <lv_conf.h>
#include "tft_lvgl_configuration.h"

#if ENABLED(MKS_WIFI_MODULE)

#include "draw_ui.h"

extern lv_group_t *g;
static lv_obj_t *scr, *labelModelAP = nullptr, *buttonModelAP = nullptr, *labelModelSTA = nullptr, *buttonModelSTA = nullptr, *btnCloudState = nullptr;

enum {
  ID_WIFI_RETURN = 1,
  ID_WIFI_AP,
  ID_WIFI_STA,
  ID_WIFI_NAME,
  ID_WIFI_PASSWORD,
  ID_WIFI_CLOUD,
  ID_WIFI_CONFIG
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_WIFI_RETURN:
      lv_clear_wifi_settings();
      lv_draw_return_ui();
      break;
    case ID_WIFI_AP:
      if (gCfgItems.wifi_mode_sel == AP_MODEL) return;
      gCfgItems.wifi_mode_sel = AP_MODEL;
      lv_btn_set_style_both(buttonModelAP, &style_para_back);
      lv_btn_set_style_both(buttonModelSTA, &style_para_value);
      update_spi_flash();
      break;
    case ID_WIFI_STA:
      if (gCfgItems.wifi_mode_sel == STA_MODEL) return;
      gCfgItems.wifi_mode_sel = STA_MODEL;
      lv_btn_set_style_both(buttonModelSTA, &style_para_back);
      lv_btn_set_style_both(buttonModelAP, &style_para_value);
      update_spi_flash();
      break;
    case ID_WIFI_NAME:
      keyboard_value = wifiName;
      lv_clear_wifi_settings();
      lv_draw_keyboard();
      break;
    case ID_WIFI_PASSWORD:
      keyboard_value = wifiPassWord;
      lv_clear_wifi_settings();
      lv_draw_keyboard();
      break;
    case ID_WIFI_CLOUD:
      gCfgItems.cloud_enable ^= true;
      lv_screen_menu_item_onoff_update(btnCloudState, gCfgItems.cloud_enable);
      update_spi_flash();
      break;
    case ID_WIFI_CONFIG:
      lv_clear_wifi_settings();
      lv_draw_dialog(DIALOG_WIFI_CONFIG_TIPS);
      break;
  }
}

void lv_draw_wifi_settings(void) {
  scr = lv_screen_create(WIFI_SETTINGS_UI, machine_menu.WifiConfTitle);

  lv_label_create(scr, PARA_UI_POS_X + PARA_UI_ITEM_TEXT_H, PARA_UI_POS_Y + 10, machine_menu.wifiMode);
  buttonModelAP = lv_btn_create(scr, nullptr);
  lv_obj_set_pos(buttonModelAP, PARA_UI_VALUE_POS_X_2, PARA_UI_POS_Y + PARA_UI_VALUE_V_2);
  lv_obj_set_size(buttonModelAP, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
  lv_obj_set_event_cb_mks(buttonModelAP, event_handler, ID_WIFI_AP, "", 0);
  lv_btn_use_label_style(buttonModelAP);
  lv_btn_set_layout(buttonModelAP, LV_LAYOUT_OFF);
  lv_btn_set_style_both(buttonModelAP, gCfgItems.wifi_mode_sel == AP_MODEL ? &style_para_back : &style_para_value);
  labelModelAP = lv_label_create_empty(buttonModelAP);
  lv_label_set_text(labelModelAP, WIFI_AP_TEXT);
  lv_obj_align(labelModelAP, buttonModelAP, LV_ALIGN_CENTER, 0, 0);
  if (TERN0(HAS_ROTARY_ENCODER, gCfgItems.encoder_enable)) lv_group_add_obj(g, buttonModelAP);

  buttonModelSTA = lv_btn_create(scr, nullptr);
  lv_obj_set_pos(buttonModelSTA, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V);
  lv_obj_set_size(buttonModelSTA, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
  lv_obj_set_event_cb_mks(buttonModelSTA, event_handler, ID_WIFI_STA, "", 0);
  lv_btn_use_label_style(buttonModelSTA);
  lv_btn_set_layout(buttonModelSTA, LV_LAYOUT_OFF);
  labelModelSTA = lv_label_create_empty(buttonModelSTA);
  lv_btn_set_style_both(buttonModelSTA, gCfgItems.wifi_mode_sel == STA_MODEL ? &style_para_back : &style_para_value);
  lv_label_set_text(labelModelSTA, WIFI_STA_TEXT);
  lv_obj_align(labelModelSTA, buttonModelSTA, LV_ALIGN_CENTER, 0, 0);
  if (TERN0(HAS_ROTARY_ENCODER, gCfgItems.encoder_enable)) lv_group_add_obj(g, buttonModelSTA);

  lv_obj_t *line1 = lv_line_create(scr, nullptr);
  lv_ex_line(line1, line_points[0]);

  strcpy_P(public_buf_m, PSTR(machine_menu.wifiName));
  strcat_P(public_buf_m, PSTR((const char *)uiCfg.wifi_name));
  lv_screen_menu_item_1_edit(scr, public_buf_m, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_WIFI_NAME, 1, machine_menu.wifiEdit);

  strcpy_P(public_buf_m, PSTR(machine_menu.wifiPassWord));
  strcat_P(public_buf_m, PSTR((const char *)uiCfg.wifi_key));
  lv_screen_menu_item_1_edit(scr, public_buf_m, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_WIFI_PASSWORD, 2, machine_menu.wifiEdit);

  btnCloudState = lv_screen_menu_item_onoff(scr, machine_menu.wifiCloud, PARA_UI_POS_X, PARA_UI_POS_Y * 4, event_handler, ID_WIFI_CLOUD, 3, gCfgItems.cloud_enable);
  lv_screen_menu_item_turn_page(scr, machine_menu.wifiConfig, event_handler, ID_WIFI_CONFIG);
  lv_screen_menu_item_return(scr, event_handler, ID_WIFI_RETURN);
}

void lv_clear_wifi_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // MKS_WIFI_MODULE
#endif // HAS_TFT_LVGL_UI
