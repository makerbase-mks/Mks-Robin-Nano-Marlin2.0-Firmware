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

#if BOTH(HAS_TFT_LVGL_UI, ADVANCED_PAUSE_FEATURE)

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../../feature/pause.h"
#include "../../../../inc/MarlinConfig.h"

void lv_draw_pause_message(const PauseMessage msg) {
  switch (msg) {
    case PAUSE_MESSAGE_PAUSING:  lv_clear_cur_ui(); lv_draw_dialog(DIALOG_PAUSE_MESSAGE_PAUSING); break;
    case PAUSE_MESSAGE_CHANGING: lv_clear_cur_ui(); lv_draw_dialog(DIALOG_PAUSE_MESSAGE_CHANGING); break;
    case PAUSE_MESSAGE_UNLOAD:   lv_clear_cur_ui(); lv_draw_dialog(DIALOG_PAUSE_MESSAGE_UNLOAD); break;
    case PAUSE_MESSAGE_WAITING:  lv_clear_cur_ui(); lv_draw_dialog(DIALOG_PAUSE_MESSAGE_WAITING); break;
    case PAUSE_MESSAGE_INSERT:   lv_clear_cur_ui(); lv_draw_dialog(DIALOG_PAUSE_MESSAGE_INSERT); break;
    case PAUSE_MESSAGE_LOAD:     lv_clear_cur_ui(); lv_draw_dialog(DIALOG_PAUSE_MESSAGE_LOAD); break;
    case PAUSE_MESSAGE_PURGE:    lv_clear_cur_ui(); lv_draw_dialog(DIALOG_PAUSE_MESSAGE_PURGE); break;
    case PAUSE_MESSAGE_RESUME:   lv_clear_cur_ui(); lv_draw_dialog(DIALOG_PAUSE_MESSAGE_RESUME); break;
    case PAUSE_MESSAGE_HEAT:     lv_clear_cur_ui(); lv_draw_dialog(DIALOG_PAUSE_MESSAGE_HEAT); break;
    case PAUSE_MESSAGE_HEATING:  lv_clear_cur_ui(); lv_draw_dialog(DIALOG_PAUSE_MESSAGE_HEATING); break;
    case PAUSE_MESSAGE_OPTION:   pause_menu_response = PAUSE_RESPONSE_WAIT_FOR;
      lv_clear_cur_ui(); lv_draw_dialog(DIALOG_PAUSE_MESSAGE_OPTION); break;
    case PAUSE_MESSAGE_STATUS:
    default: break;
  }
}

#endif // HAS_TFT_LVGL_UI && ADVANCED_PAUSE_FEATURE
