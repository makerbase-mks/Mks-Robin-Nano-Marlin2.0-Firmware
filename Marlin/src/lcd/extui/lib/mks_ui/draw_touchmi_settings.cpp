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

#include "../../../../MarlinCore.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/probe.h"

#include "draw_ui.h"

extern lv_group_t * g;
static lv_obj_t * scr, * zOffsetText;

enum {
  ID_TM_INIT = 1,
  ID_TM_ZOFFSETPOS,
  ID_TM_ZOFFSETNEG,
  ID_TM_SAVE,
  ID_TM_TEST,
  ID_TM_RETURN
 };

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_TM_INIT:
      queue.inject_P(PSTR("M851 Z0\nG28\nG1 Z0 F200\nM211 S0"));
      break;
    case ID_TM_ZOFFSETPOS:
      queue.inject_P(PSTR("M290 Z0.1"));
      break;
    case ID_TM_ZOFFSETNEG:
      queue.inject_P(PSTR("M290 Z-0.1"));
      break;
    case ID_TM_SAVE:
      queue.inject_P(PSTR("M211 S1\nM500\nG28 X Y"));
      break;
    case ID_TM_TEST:
      queue.inject_P(PSTR("G28\nG1 Z0"));
      break;
    case ID_TM_RETURN:
      lv_clear_touchmi_settings();
      lv_draw_return_ui();
      break;

  }
}

void lv_draw_touchmi_settings(void) {
  scr = lv_screen_create(TOUCHMI_UI, machine_menu.LevelingTouchmiConf);
  lv_big_button_create(scr, "F:/bmp_speed0.bin", machine_menu.TouchmiInit, INTERVAL_V, titleHeight, event_handler, ID_TM_INIT);
  lv_big_button_create(scr, "F:/bmp_zAdd.bin", machine_menu.TouchmiOffsetpos, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_TM_ZOFFSETPOS);
  lv_big_button_create(scr, "F:/bmp_zDec.bin", machine_menu.TouchmiOffsetneg, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_TM_ZOFFSETNEG);
  lv_big_button_create(scr, "F:/bmp_set.bin", machine_menu.TouchmiSave, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_TM_SAVE);
  lv_big_button_create(scr, "F:/bmp_in.bin", machine_menu.TouchmiTest, BTN_X_PIXEL * 3 + INTERVAL_V * 4,titleHeight, event_handler, ID_TM_TEST);
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_TM_RETURN);

  
  zOffsetText = lv_label_create(scr, 290, TITLE_YPOS, nullptr);
  disp_z_offset_value_TM();
}

void disp_z_offset_value_TM() {
  char buf[20];
  #if HAS_BED_PROBE
   char str_1[16];
  #endif
  sprintf_P(buf, PSTR("offset Z: %s mm"), TERN(HAS_BED_PROBE, dtostrf(probe.offset.z, 1, 2, str_1), "0"));
  lv_label_set_text(zOffsetText, buf);
}


void lv_clear_touchmi_settings() { 
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
