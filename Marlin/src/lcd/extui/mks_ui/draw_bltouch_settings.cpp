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

#include "../../../MarlinCore.h"
#include "draw_ui.h"

#include "../../../module/temperature.h"
#include "../../../gcode/queue.h"
#include "../../../gcode/gcode.h"
#include "../../../module/probe.h"
#if ENABLED(AUTO_BED_LEVELING_BILINEAR)
  #include "../../../feature/bedlevel/bedlevel.h"
#endif
#if ENABLED(EEPROM_SETTINGS)
  #include "../../../module/settings.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)
  extern bed_mesh_t z_values;
#endif

extern lv_group_t *g;
static lv_obj_t *scr, *labelV, *buttonV, *zOffsetText;
static lv_obj_t *labelExt1, *labelBed, *labelInit;

static float step_dist = 0.01;
static float zoffset_diff = 0;

enum {
  ID_BLTOUCH_INIT = 1,
  ID_BLTOUCH_ZOFFSETPOS,
  ID_BLTOUCH_ZOFFSETNEG,
  ID_BLTOUCH_SAVE,
  ID_BLTOUCH_TEST,
  ID_BLTOUCH_STEPS,
  ID_BLTOUCH_RETURN
 };

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  char baby_buf[30] = { 0 };
  char str_1[60];
  switch (obj->mks_obj_id) {
    case ID_BLTOUCH_INIT:
      bltouch_do_init(true);
      break;
    case ID_BLTOUCH_ZOFFSETPOS:
      sprintf_P(baby_buf, PSTR("M290 Z%s"), dtostrf(step_dist, 1, 3, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      zoffset_diff += step_dist;
      break;
    case ID_BLTOUCH_ZOFFSETNEG:
      sprintf_P(baby_buf, PSTR("M290 Z%s"), dtostrf(-step_dist, 1, 3, str_1));
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      zoffset_diff -= step_dist;
      break;
    case ID_BLTOUCH_SAVE:
      // if (queue.length <= (BUFSIZE - 2)) {
      if (!queue.ring_buffer.full(2)) {
        #if ENABLED(AUTO_BED_LEVELING_BILINEAR) && DISABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
          for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
            for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
              z_values[x][y] = z_values[x][y] + zoffset_diff;
        #endif
        queue.enqueue_now_P(PSTR("M500\nG28 X Y"));
        zoffset_diff = 0;
      }
      break;
    case ID_BLTOUCH_TEST:
      sprintf_P(str_1, PSTR("G28\nG1 Z10 F2400\nG1 X%d Y%d\nG0 Z0.3"), X_MAX_POS / 2, Y_MAX_POS / 2);
      // if (!queue.ring_buffer.empty()) {
      if((queue.ring_buffer.empty())) { 
        queue.enqueue_now_P(PSTR(str_1));
        // queue.inject_P(PSTR(str_1));
        zoffset_diff = 0;
      }
      break;
    case ID_BLTOUCH_STEPS:
      if (abs((int)(100 * step_dist)) == 1)
        step_dist = 0.05;
      else if (abs((int)(100 * step_dist)) == 5)
        step_dist = 0.1;
      else
        step_dist = 0.01;
      disp_step_dist();
      break;
    case ID_BLTOUCH_RETURN:
      TERN_(HAS_SOFTWARE_ENDSTOPS, soft_endstop._enabled = true);
      lv_clear_bltouch_settings();
      if (last_disp_state == DIALOG_UI) lv_draw_ready_print();
      else draw_return_ui();
      // queue.enqueue_now_P(PSTR("G28 X Y"));
      queue.inject_P(PSTR("G28 X Y"));
      break;
  }
}

void lv_draw_bltouch_settings(void) {
  scr = lv_screen_create(BLTOUCH_UI, machine_menu.BLTouchLevelingConfTitle);
  // Create image buttons
  lv_big_button_create(scr, "F:/bmp_Add.bin", machine_menu.BLTouchOffsetpos, INTERVAL_V, titleHeight, event_handler, ID_BLTOUCH_ZOFFSETPOS);

//  buttonInitstate = lv_img_create(scr, NULL);
  lv_obj_t *buttonInitstate = lv_imgbtn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonInitstate, event_handler, ID_BLTOUCH_INIT, NULL, 0);
  lv_imgbtn_set_src(buttonInitstate, LV_BTN_STATE_REL, "F:/bmp_init_state.bin");
  lv_obj_set_pos(buttonInitstate, 145, 50);


  lv_obj_t *buttonExt1 = lv_img_create(scr, nullptr);
  lv_img_set_src(buttonExt1, "F:/bmp_ext1_state.bin");
  lv_obj_set_pos(buttonExt1, 216, 50);

  lv_obj_t *buttonBedstate = lv_img_create(scr, nullptr);
  lv_img_set_src(buttonBedstate, "F:/bmp_bed_state.bin");
  lv_obj_set_pos(buttonBedstate, 287, 50);

  labelInit = lv_label_create(scr, 125, 115, nullptr);
  labelExt1 = lv_label_create(scr, 196, 115, nullptr);
  labelBed  = lv_label_create(scr, 267, 115, nullptr);

  lv_obj_align(labelInit, buttonInitstate, LV_ALIGN_IN_BOTTOM_MID, 2, 20);
  lv_obj_align(labelExt1, buttonExt1, LV_ALIGN_IN_BOTTOM_MID, 2, 20);
  lv_obj_align(labelBed, buttonBedstate, LV_ALIGN_IN_BOTTOM_MID, 2, 20);

  lv_label_set_text(labelInit, machine_menu.BLTouchInit);

  zOffsetText = lv_label_create(scr, 170, 140, nullptr);
  lv_big_button_create(scr, "F:/bmp_Dec.bin", machine_menu.BLTouchOffsetneg, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_BLTOUCH_ZOFFSETNEG);

  buttonV = lv_imgbtn_create(scr, nullptr, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BLTOUCH_STEPS);
  labelV  = lv_label_create_empty(buttonV);

  lv_big_button_create(scr, "F:/bmp_in.bin", machine_menu.BLTouchTest, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BLTOUCH_TEST);
  lv_big_button_create(scr, "F:/bmp_set.bin", machine_menu.BLTouchSave, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BLTOUCH_SAVE);
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BLTOUCH_RETURN);

  disp_step_dist();
  disp_bltouch_z_offset_value();

  zoffset_diff = 0;
}

void disp_step_dist() {
  if ((int)(100 * step_dist) == 1)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_01.bin");
  else if ((int)(100 * step_dist) == 5)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_05.bin");
  else if ((int)(100 * step_dist) == 10)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_1.bin");

  if (gCfgItems.multiple_language) {
    if ((int)(100 * step_dist) == 1) {
      lv_label_set_text(labelV, move_menu.step_001mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(100 * step_dist) == 5) {
      lv_label_set_text(labelV, move_menu.step_005mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(100 * step_dist) == 10) {
      lv_label_set_text(labelV, move_menu.step_01mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void disp_bltouch_z_offset_value() {
  char buf[20];
  char str_1[16];
  // sprintf_P(buf, PSTR("%s : %s mm"), move_menu.zoffset, dtostrf(probe.offset.z, 1, 2, str_1) );
  sprintf_P(buf, PSTR("Z: %s mm"), dtostrf(probe.offset.z, 1, 2, str_1) );
  lv_label_set_text(zOffsetText, buf);
  #if HAS_HOTEND
    sprintf(public_buf_l, printing_menu.temp1, (int)thermalManager.temp_hotend[0].celsius, (int)thermalManager.temp_hotend[0].target);
    lv_label_set_text(labelExt1, public_buf_l);
  #endif

  #if HAS_HEATED_BED
    sprintf(public_buf_l, printing_menu.bed_temp, (int)thermalManager.temp_bed.celsius, (int)thermalManager.temp_bed.target);
    lv_label_set_text(labelBed, public_buf_l);
  #endif
}

void bltouch_do_init(bool resetZoffset) {
  char str_1[50];
  //TERN_(HAS_BED_PROBE, probe.offset.z = 0);
  TERN_(HAS_SOFTWARE_ENDSTOPS, soft_endstop._enabled = false);
  //TERN_(HAS_LEVELING, reset_bed_level());
  //TERN_(EEPROM_SETTINGS, (void)settings.save());
  queue.clear();
  if (resetZoffset)
  {
    sprintf_P(str_1, PSTR("M851 Z0\nG28\nG1 Z10 F2400\nG1 X%d Y%d\nG0 Z0.3"), X_MAX_POS / 2, Y_MAX_POS / 2);
  }
  else
  {
    sprintf_P(str_1, PSTR("G28\nG1 Z10 F2400\nG1 X%d Y%d\nG0 Z0.3"), X_MAX_POS / 2, Y_MAX_POS / 2);
  }
  queue.enqueue_now_P(PSTR(str_1));
}

void lv_clear_bltouch_settings() { 
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
