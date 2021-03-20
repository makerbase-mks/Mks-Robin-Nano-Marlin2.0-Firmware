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

enum {
  ID_T_EXTRUCT = 1,
  ID_T_FILAMENT,
  ID_T_RETURN
};

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_T_EXTRUCT:
      lv_clear_tool();
      lv_draw_extrusion();
    break;
    case ID_T_FILAMENT:
      //uiCfg.desireSprayerTempBak = thermalManager.temp_hotend[uiCfg.curSprayerChoose].target;
      lv_clear_tool();
      lv_draw_filament_change();
    break;
    case ID_T_RETURN:
        lv_clear_tool();
        lv_draw_tool();
      break;
    }
  }

  void lv_draw_extrusion_m(void) {
    scr = lv_screen_create(EXTRUSION_UI_M);

  // Create image buttons
  lv_obj_t *buttonFilament = lv_big_button_create(scr, "F:/bmp_filamentchange.bin", tool_menu.filament, INTERVAL_V, titleHeight, event_handler, ID_T_FILAMENT);
  lv_obj_clear_protect(buttonFilament, LV_PROTECT_FOLLOW);
  lv_big_button_create(scr, "F:/bmp_extruct.bin", tool_menu.extrude, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_T_EXTRUCT);
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_T_RETURN);
// #endif
}

void lv_clear_extrusion_m() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
