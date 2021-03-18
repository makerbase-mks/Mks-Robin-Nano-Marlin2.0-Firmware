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

#include "../../../../inc/MarlinConfig.h"
#include "../../../../module/motion.h"
#include "../../../../gcode/gcode.h"

extern lv_group_t *g;
static lv_obj_t *scr;
static lv_obj_t *buttonCk[4];

enum {
  ID_FULL_CONTROL_MODE = 1,
  ID_AUTO_PARK_MODE,
  ID_DUPLICATION_MODE,
  ID_MIRROR_MODE,
  ID_DUAL_X_MODE_RETURN
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_DUAL_X_MODE_RETURN:
      lv_clear_dual_x_carriage_mode();
      lv_draw_return_ui();
      break;
    case ID_FULL_CONTROL_MODE:
        gcode.process_subcommands_now_P(PSTR("M605 S0"));
      break;
    case ID_AUTO_PARK_MODE:
        gcode.process_subcommands_now_P(PSTR("M605 S1"));
      break;
    case ID_DUPLICATION_MODE:
        gcode.process_subcommands_now_P(PSTR("M605 S2"));
      break;
    case ID_MIRROR_MODE:
        gcode.process_subcommands_now_P(PSTR("M605 S2"));
			  gcode.process_subcommands_now_P(PSTR("M605 S3"));
      break;
  }
  if (obj->mks_obj_id == ID_FULL_CONTROL_MODE || obj->mks_obj_id == ID_AUTO_PARK_MODE || obj->mks_obj_id == ID_DUPLICATION_MODE || obj->mks_obj_id == ID_MIRROR_MODE) {
    gcode.process_subcommands_now_P(PSTR("M500"));
    updateCheckboxState();
  }
}

void lv_draw_dual_x_carriage_mode(void) {
  scr = lv_screen_create(DUAL_X_CARRIAGE_MODE_UI, machine_menu.XmodeConfigTitle);
  for(uint8_t i= 0; i < 4; i++) {
    buttonCk[i] = lv_cb_create(scr, NULL);
    lv_cb_set_style(buttonCk[i], LV_BTN_STATE_PR, &tft_style_label_rel);
    lv_cb_set_style(buttonCk[i], LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_cb_set_style(buttonCk[i], LV_CB_STYLE_BOX_REL, &style_check_box_unselected);
    lv_cb_set_style(buttonCk[i], LV_CB_STYLE_BOX_TGL_REL, &style_check_box_selected);
    lv_obj_set_pos(buttonCk[i], PARA_UI_POS_X, PARA_UI_POS_Y * (i + 1));
    
    lv_obj_set_size(buttonCk[i], PARA_UI_SIZE_X, PARA_UI_SIZE_Y);
    lv_obj_set_event_cb_mks(buttonCk[i], event_handler, i + 1, NULL, 0);

    if (TERN0(HAS_ROTARY_ENCODER, gCfgItems.encoder_enable))
      lv_group_add_obj(g, buttonCk[i]);

    lv_obj_t *line1 = lv_line_create(scr, nullptr);
    lv_ex_line(line1, line_points[i]);
  }
  lv_cb_set_text(buttonCk[0], machine_menu.fullControl);
  lv_cb_set_text(buttonCk[1], machine_menu.autoPark);
  lv_cb_set_text(buttonCk[2], machine_menu.duplication);
  lv_cb_set_text(buttonCk[3], machine_menu.mirror);
  updateCheckboxState();

  lv_screen_menu_item_return(scr, event_handler, ID_DUAL_X_MODE_RETURN);
}

void updateCheckboxState() {
  switch (dual_x_carriage_mode) {
    case DXC_FULL_CONTROL_MODE:
      lv_cb_set_checked(buttonCk[0], true);
			lv_cb_set_checked(buttonCk[1], false);
			lv_cb_set_checked(buttonCk[2], false);
			lv_cb_set_checked(buttonCk[3], false);
		break;
    case DXC_AUTO_PARK_MODE:
      lv_cb_set_checked(buttonCk[0], false);
			lv_cb_set_checked(buttonCk[1], true);
			lv_cb_set_checked(buttonCk[2], false);
			lv_cb_set_checked(buttonCk[3], false);
    break;
    case DXC_DUPLICATION_MODE:
      lv_cb_set_checked(buttonCk[0], false);
			lv_cb_set_checked(buttonCk[1], false);
			lv_cb_set_checked(buttonCk[2], true);
			lv_cb_set_checked(buttonCk[3], false);
		break;
	  case DXC_MIRRORED_MODE:
      lv_cb_set_checked(buttonCk[0], false);
			lv_cb_set_checked(buttonCk[1], false);
			lv_cb_set_checked(buttonCk[2], false);
			lv_cb_set_checked(buttonCk[3], true);
    break;
  }
}

void lv_clear_dual_x_carriage_mode() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
