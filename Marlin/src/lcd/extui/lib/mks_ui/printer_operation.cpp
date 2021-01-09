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

#include "lv_conf.h"
#include "draw_ui.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../sd/cardreader.h"
#include "../../../../gcode/queue.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#include "../../../../gcode/gcode.h"
#include "../../../../module/planner.h"

extern uint32_t To_pre_view;
extern uint8_t sel_id;
extern uint8_t flash_preview_begin, default_preview_flg, gcode_preview_over;

void printer_state_polling() {
  if (uiCfg.print_state == PAUSING) {
    lv_clear_cur_ui();
    lv_draw_dialog(DIALOG_TYPE_MACHINE_PAUSING_TIPS);
    #if ENABLED(SDSUPPORT)

      while(queue.length) {
        queue.advance();
      }
      planner.synchronize();

      gcode.process_subcommands_now_P(PSTR("M25"));

      //save the positon
      uiCfg.current_x_position_bak = current_position.x;
      uiCfg.current_y_position_bak = current_position.y;
      uiCfg.current_z_position_bak = current_position.z;
      
      if (gCfgItems.pausePosZ != (float)-1) {
        gcode.process_subcommands_now_P(PSTR("G91"));
        ZERO(public_buf_l);
        sprintf_P(public_buf_l, PSTR("G1 Z%.1f"), gCfgItems.pausePosZ);
        gcode.process_subcommands_now(public_buf_l);
        gcode.process_subcommands_now_P(PSTR("G90"));
      }
      if (gCfgItems.pausePosX != (float)-1 && gCfgItems.pausePosY != (float)-1) {
        ZERO(public_buf_l);
        sprintf_P(public_buf_l, PSTR("G1 X%.1f Y%.1f"), gCfgItems.pausePosX, gCfgItems.pausePosY);
        gcode.process_subcommands_now(public_buf_l);
      }
      uiCfg.print_state = PAUSED;
      uiCfg.current_e_position_bak = current_position.e;
      
      // #if ENABLED(POWER_LOSS_RECOVERY)
      //  if (recovery.enabled) recovery.save(true);
      // #endif
      gCfgItems.pause_reprint = 1;
      update_spi_flash();

      lv_clear_cur_ui();
      lv_draw_return_ui();
    #endif
  }

  if (uiCfg.print_state == PAUSED) {
  }

  if (uiCfg.print_state == RESUMING) {
    if (IS_SD_PAUSED()) {
      if (gCfgItems.pausePosX != (float)-1 && gCfgItems.pausePosY != (float)-1) {
        ZERO(public_buf_m);
        sprintf_P(public_buf_m, PSTR("G1 X%.1f Y%.1f"), uiCfg.current_x_position_bak, uiCfg.current_y_position_bak);
        gcode.process_subcommands_now(public_buf_m);
      }
      if (gCfgItems.pausePosZ != (float)-1) {
        // gcode.process_subcommands_now_P(PSTR("G91"));
        // ZERO(public_buf_l);
        // sprintf_P(public_buf_l, PSTR("G1 Z-%.1f"), gCfgItems.pausePosZ);
        // gcode.process_subcommands_now(public_buf_l);
        // gcode.process_subcommands_now_P(PSTR("G90"));
        ZERO(public_buf_m);
        sprintf_P(public_buf_m, PSTR("G1 Z%.1f"), uiCfg.current_z_position_bak);
        gcode.process_subcommands_now(public_buf_m);
      }
      gcode.process_subcommands_now_P(PSTR("M24"));
      uiCfg.print_state = WORKING;
      start_print_time();

      gCfgItems.pause_reprint = 0;
      update_spi_flash();
    }
  }
  #if ENABLED(POWER_LOSS_RECOVERY)
    if (uiCfg.print_state == REPRINTED) {
      ZERO(public_buf_m);
      #if HAS_HOTEND
        HOTEND_LOOP() {
          const int16_t et = recovery.info.target_temperature[e];
          if (et) {
            #if HAS_MULTI_HOTEND
              sprintf_P(public_buf_m, PSTR("T%i"), e);
              gcode.process_subcommands_now(public_buf_m);
            #endif
            sprintf_P(public_buf_m, PSTR("M109 S%i"), et);
            gcode.process_subcommands_now(public_buf_m);
          }
        }
      #endif
      recovery.resume();
      uiCfg.print_state = WORKING;
      start_print_time();
      gCfgItems.pause_reprint = 0;
      update_spi_flash();
    }
  #endif

  if (uiCfg.print_state == WORKING)
    filament_check();
	
  #if USE_WIFI_FUNCTION
    wifi_looping();
  #endif

  #if HAS_CUTTER
    if(uiCfg.calculateBoaderData) calculate_boader_data();
    if(uiCfg.needEngraveBoader) carving_boader();
  #endif
}

void filament_pin_setup() {
  #if PIN_EXISTS(MT_DET_1)
    pinMode(MT_DET_1_PIN, INPUT_PULLUP);
  #endif
  #if PIN_EXISTS(MT_DET_2)
    pinMode(MT_DET_2_PIN, INPUT_PULLUP);
  #endif
  #if PIN_EXISTS(MT_DET_3)
    pinMode(MT_DET_3_PIN, INPUT_PULLUP);
  #endif
}

void filament_check() {
  const int FIL_DELAY = 20;
  #if PIN_EXISTS(MT_DET_1)
    static int fil_det_count_1 = 0;
    if (!READ(MT_DET_1_PIN) && !MT_DET_PIN_INVERTING)
      fil_det_count_1++;
    else if (READ(MT_DET_1_PIN) && MT_DET_PIN_INVERTING)
      fil_det_count_1++;
    else if (fil_det_count_1 > 0)
      fil_det_count_1--;

    if (!READ(MT_DET_1_PIN) && !MT_DET_PIN_INVERTING)
      fil_det_count_1++;
    else if (READ(MT_DET_1_PIN) && MT_DET_PIN_INVERTING)
      fil_det_count_1++;
    else if (fil_det_count_1 > 0)
      fil_det_count_1--;
  #endif

  #if PIN_EXISTS(MT_DET_2)
    static int fil_det_count_2 = 0;
    if (!READ(MT_DET_2_PIN) && !MT_DET_PIN_INVERTING)
      fil_det_count_2++;
    else if (READ(MT_DET_2_PIN) && MT_DET_PIN_INVERTING)
      fil_det_count_2++;
    else if (fil_det_count_2 > 0)
      fil_det_count_2--;

    if (!READ(MT_DET_2_PIN) && !MT_DET_PIN_INVERTING)
      fil_det_count_2++;
    else if (READ(MT_DET_2_PIN) && MT_DET_PIN_INVERTING)
      fil_det_count_2++;
    else if (fil_det_count_2 > 0)
      fil_det_count_2--;
  #endif

  #if PIN_EXISTS(MT_DET_3)
    static int fil_det_count_3 = 0;
    if (!READ(MT_DET_3_PIN) && !MT_DET_PIN_INVERTING)
      fil_det_count_3++;
    else if (READ(MT_DET_3_PIN) && MT_DET_PIN_INVERTING)
      fil_det_count_3++;
    else if (fil_det_count_3 > 0)
      fil_det_count_3--;

    if (!READ(MT_DET_3_PIN) && !MT_DET_PIN_INVERTING)
      fil_det_count_3++;
    else if (READ(MT_DET_3_PIN) && MT_DET_PIN_INVERTING)
      fil_det_count_3++;
    else if (fil_det_count_3 > 0)
      fil_det_count_3--;
  #endif

  if (false
    #if PIN_EXISTS(MT_DET_1)
      || fil_det_count_1 >= FIL_DELAY
    #endif
    #if PIN_EXISTS(MT_DET_2)
      || fil_det_count_2 >= FIL_DELAY
    #endif
    #if PIN_EXISTS(MT_DET_3)
      || fil_det_count_3 >= FIL_DELAY
    #endif
  ) {
    lv_clear_cur_ui();
    card.pauseSDPrint();
    stop_print_time();
    uiCfg.print_state = PAUSING;

    if (gCfgItems.from_flash_pic == 1)
      flash_preview_begin = 1;
    else
      default_preview_flg = 1;

    lv_draw_printing();
  }
}

static void set_boader_value(float xmin, float xmax, float ymin, float ymax) {
	uiCfg.xmin = xmin;
	uiCfg.xmax = xmax;
	uiCfg.ymin = ymin;
	uiCfg.ymax = ymax;
}

void calculate_boader_data() {
  char buf[UDISKBUFLEN];
  bool gcode_have_G91 = false;
  char *p = &buf[0];
  char x_y_value[10];
  char *x_or_y = &x_y_value[0];
  float x_coordinates = 0, y_coordinates = 0;
	float xmin = 0, xmax = 0, ymin = 0, ymax = 0;
  bool continueConverX = false;
  bool continueConverY = false;
  if(uiCfg.alreadyGetBoaderData == false) { //excute one times
    char *cur_name;
    cur_name = strrchr(list_file.file_name[sel_id], '/');
    SdFile file, *curDir;
    const char * const fname = card.diveToFile(true, curDir, cur_name);
    if (!fname) return;
    if (file.open(curDir, fname, O_READ)) {	
      gCfgItems.curFilesize = file.fileSize();

      lv_clear_cur_ui();
      lv_draw_dialog(DIALOG_TYPE_BOADER_CALCULATE_CLUES); //tips
      lv_task_handler();

      filepos_t pos;
      while(pos.position < gCfgItems.curFilesize) {
        ZERO(buf);
        file.read(buf, UDISKBUFLEN);
        if(pos.position <= UDISKBUFLEN) {
          if(strstr(buf, "G91")) gcode_have_G91 = true; //find G91
          else {
            xmin = 20000; 
            ymin = 20000;
          }
        }
        while(p - buf < UDISKBUFLEN) {
          if(gcode_have_G91) {
            if(*p == 'X' || continueConverX) {
              if(!continueConverX) { p++; ZERO(x_y_value); x_or_y = &x_y_value[0];}
              while(*p != '\0' && *p != ' ' && *p != '\r' && *p != '\n') *x_or_y++ = *p++;
              if(p - buf == UDISKBUFLEN) continueConverX = true;
              else {
                continueConverX = false;
                x_coordinates += atof(x_y_value);
                xmax = x_coordinates > xmax ? x_coordinates : xmax;
                xmin = (x_coordinates < xmin) && (x_coordinates != 0) ? x_coordinates : xmin;
              }
            }
            if(*p == 'Y' || continueConverY) {
              if(!continueConverY) { p++; ZERO(x_y_value); x_or_y = &x_y_value[0]; }
              while(*p != '\0' && *p != ' ' && *p != '\r' && *p != '\n') *x_or_y++ = *p++;
              if(p - buf == UDISKBUFLEN) continueConverY = true;
              else {
                continueConverY = false;	
                y_coordinates += atof(x_y_value);
                ymax = y_coordinates > ymax ? y_coordinates : ymax;
                ymin = (y_coordinates < ymin) && (y_coordinates != 0) ? y_coordinates : ymin;
              }					
            }
          }
          else { //no G91
            if(*p == 'X' || continueConverX) {
              if(!continueConverX) { p++; ZERO(x_y_value); x_or_y = &x_y_value[0];}
              while(*p != '\0' && *p != ' ' && *p != '\r' && *p != '\n') *x_or_y++ = *p++;
              if(p - buf == UDISKBUFLEN) continueConverX = true;
              else {
                continueConverX = false;
                x_coordinates = atof(x_y_value);
                xmax = x_coordinates > xmax ? x_coordinates : xmax;
                xmin = (x_coordinates < xmin) && (x_coordinates != 0) ? x_coordinates : xmin;
              }
            }
            if(*p == 'Y' || continueConverY) {
              if(!continueConverY) { p++; ZERO(x_y_value); x_or_y = &x_y_value[0]; }
              while(*p != '\0' && *p != ' ' && *p != '\r' && *p != '\n') *x_or_y++ = *p++;
              if(p - buf == UDISKBUFLEN) continueConverY = true;
              else {
                continueConverY = false;	
                y_coordinates = atof(x_y_value);
                ymax = y_coordinates > ymax ? y_coordinates : ymax;
                ymin = (y_coordinates < ymin) && (y_coordinates != 0) ? y_coordinates : ymin;
              }
            }
          }
          p++;
        }
        p = &buf[0];
        file.getpos(&pos);
      }
      set_boader_value(xmin, xmax, ymin, ymax);
      file.close();
    }
    else {
      lv_clear_cur_ui();
      lv_draw_dialog(DIALOG_TYPE_NO_BOADER_DATA_TIPS);
      uiCfg.calculateBoaderData = false;
      return;
    }
  }
  uiCfg.calculateBoaderData  = false;
  uiCfg.alreadyGetBoaderData = true;
  uiCfg.needEngraveBoader    = true;
  lv_clear_cur_ui();
  lv_draw_dialog(DIALOG_TYPE_ENGRAVE_BOADER_TIPS);
}

void carving_boader() {
	char buf[30];

  gcode.process_subcommands_now_P(PSTR("G1 F3000\nG90\nG0 X0 Y0"));
	
	ZERO(buf);
	sprintf_P(buf, PSTR("G0 F3000 X%.3f Y%.3f"), uiCfg.xmin, uiCfg.ymin);
	gcode.process_subcommands_now_P(PSTR(buf));

  gcode.process_subcommands_now_P(PSTR("M3 S20"));

	ZERO(buf);
	sprintf(buf, PSTR("G1 F3000 X%.3f Y%.3f"), uiCfg.xmin, uiCfg.ymax);
  gcode.process_subcommands_now_P(PSTR(buf));

	ZERO(buf);
	sprintf_P(buf, PSTR("G1 F3000 X%.3f Y%.3f"), uiCfg.xmax, uiCfg.ymax);
	gcode.process_subcommands_now_P(PSTR(buf));

	ZERO(buf);
	sprintf_P(buf, PSTR("G1 F3000 X%.3f Y%.3f"), uiCfg.xmax, uiCfg.ymin);
	gcode.process_subcommands_now_P(PSTR(buf));

	ZERO(buf);
	sprintf_P(buf, PSTR("G1 F3000 X%.3f Y%.3f"),uiCfg.xmin,uiCfg.ymin);
	gcode.process_subcommands_now_P(PSTR(buf));

	gcode.process_subcommands_now_P(PSTR("M5"));

  uiCfg.needEngraveBoader = false;
  lv_clear_cur_ui();
	lv_draw_dialog(DIALOG_TYPE_PRINT_OR_ENGRAVE_FILE);
}

#endif // HAS_TFT_LVGL_UI
