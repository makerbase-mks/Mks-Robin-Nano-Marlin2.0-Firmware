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

#if BOTH(HAS_TFT_LVGL_UI, DUAL_X_CARRIAGE)

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../../module/planner.h"
#include "../../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_HOTEND_OFFSET_RETURN = 1,
  ID_HOTEND_OFFSET_X,
  ID_HOTEND_OFFSET_Y,
  ID_HOTEND_OFFSET_Z
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  lv_clear_hotend_offset_settings();
  switch (obj->mks_obj_id) {
    case ID_HOTEND_OFFSET_RETURN:
      lv_draw_return_ui();
      return;
    case ID_HOTEND_OFFSET_X:
      value = x_hotend_offset;
      break;
    case ID_HOTEND_OFFSET_Y:
      value = y_hotend_offset;
      break;
    case ID_HOTEND_OFFSET_Z:
      value = z_hotend_offset;
      break;
  }
  lv_draw_number_key();
}

void lv_draw_hotend_offset_settings(void) {
  char str_1[16];
  scr = lv_screen_create(HOTEND_OFFSET_UI, machine_menu.hotendOffsetConfigTitle);

  sprintf_P(public_buf_l, PSTR("%s"), dtostrf(hotend_offset[1].x, 1, 1, str_1));
  lv_screen_menu_item_1_edit(scr, machine_menu.hotendXoffset, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_HOTEND_OFFSET_X, 0, public_buf_l);

  sprintf_P(public_buf_l, PSTR("%s"), dtostrf(hotend_offset[1].y, 1, 1, str_1));
  lv_screen_menu_item_1_edit(scr, machine_menu.hotendYoffset, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_HOTEND_OFFSET_Y, 1, public_buf_l);

  sprintf_P(public_buf_l, PSTR("%s"), dtostrf(hotend_offset[1].z, 1, 1, str_1));
  lv_screen_menu_item_1_edit(scr, machine_menu.hotendZoffset, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_HOTEND_OFFSET_Z, 2, public_buf_l);

  lv_screen_menu_item_return(scr, event_handler, ID_HOTEND_OFFSET_RETURN);
}

void lv_clear_hotend_offset_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
