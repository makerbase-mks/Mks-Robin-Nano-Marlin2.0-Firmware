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

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ready_print.h"
#include "draw_set.h"
#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../gcode/queue.h"
#include "../../../inc/MarlinConfig.h"

extern lv_group_t *g;

#ifndef USE_NEW_LVGL_CONF
static lv_obj_t *scr;
#endif

enum {
  ID_H_ALL = 1,
  ID_H_X,
  ID_H_Y,
  ID_H_Z,
  ID_H_RETURN,
  ID_H_OFF_ALL,
  ID_H_OFF_XY
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {

  bool is_rb_full = true;

  if(!queue.ring_buffer.full(1)) {
    is_rb_full = true;
  }else{
    is_rb_full = false;
  }

  if ((event == LV_EVENT_RELEASED) || (event == LV_EVENT_PRESS_LOST)) {

    switch (obj->mks_obj_id) {
      case ID_H_ALL:
        queue.inject_P(G28_STR);
        break;
      case ID_H_X:
        if(is_rb_full) {
          queue.inject_P(PSTR("G28X"));
        }
        break;
      case ID_H_Y:
        if(is_rb_full) {
          queue.inject_P(PSTR("G28Y"));
        }
        break;
      case ID_H_Z:
        if(is_rb_full) {
          queue.inject_P(PSTR("G28Z"));
        }
        break;
      case ID_H_OFF_ALL:
        if(is_rb_full) {
          queue.inject_P(PSTR("M84"));
        }
        break;
      case ID_H_OFF_XY:
        if(is_rb_full) {
          queue.inject_P(PSTR("M84XY"));
        }
        break;
      case ID_H_RETURN:
        lv_clear_home();
        lv_draw_tool();
        break;
    }
  }
}

void lv_draw_home() {

#ifdef USE_NEW_LVGL_CONF
  mks_ui.src_main = lv_set_scr_id_title(mks_ui.src_main, ZERO_UI, "");
  lv_big_button_create(mks_ui.src_main, "F:/bmp_zeroAll.bin", home_menu.home_all, INTERVAL_V, titleHeight, event_handler, ID_H_ALL);
  lv_big_button_create(mks_ui.src_main, "F:/bmp_zeroX.bin", home_menu.home_x, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_H_X);
  lv_big_button_create(mks_ui.src_main, "F:/bmp_zeroY.bin", home_menu.home_y, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_H_Y);
  lv_big_button_create(mks_ui.src_main, "F:/bmp_zeroZ.bin", home_menu.home_z, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_H_Z);
  lv_big_button_create(mks_ui.src_main, "F:/bmp_function1.bin", set_menu.motoroff, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_H_OFF_ALL);
  lv_big_button_create(mks_ui.src_main, "F:/bmp_function1.bin", set_menu.motoroffXY, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_H_OFF_XY);
  lv_big_button_create(mks_ui.src_main, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_H_RETURN);
#else
  scr = lv_screen_create(ZERO_UI);
  lv_big_button_create(scr, "F:/bmp_zeroAll.bin", home_menu.home_all, INTERVAL_V, titleHeight, event_handler, ID_H_ALL);
  lv_big_button_create(scr, "F:/bmp_zeroX.bin", home_menu.home_x, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_H_X);
  lv_big_button_create(scr, "F:/bmp_zeroY.bin", home_menu.home_y, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_H_Y);
  lv_big_button_create(scr, "F:/bmp_zeroZ.bin", home_menu.home_z, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_H_Z);
  lv_big_button_create(scr, "F:/bmp_function1.bin", set_menu.motoroff, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_H_OFF_ALL);
  lv_big_button_create(scr, "F:/bmp_function1.bin", set_menu.motoroffXY, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_H_OFF_XY);
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_H_RETURN);
#endif

}

void lv_clear_home() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif

#ifdef USE_NEW_LVGL_CONF
  lv_obj_clean(mks_ui.src_main);
#else
  lv_obj_del(scr);
#endif

}

#endif // HAS_TFT_LVGL_UI
