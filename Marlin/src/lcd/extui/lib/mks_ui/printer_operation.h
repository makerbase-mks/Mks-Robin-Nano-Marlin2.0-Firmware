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
#pragma once

#ifdef __cplusplus
  extern "C" { /* C-declarations for C++ */
#endif

#define MIN_FILE_PRINTED   100 //5000


typedef enum {

  F_STATUS_CHECK,       // 轮询检测断料
  F_STATUS_WAIT,        // 触发断料，等待延时
  F_STATUS_RUN,         // 执行断料该有的动作
  F_STATUS_WAIT_UP,     // 等待重新上料
  F_STATUS_END,         // 结束，重置
}filament_status_t;

typedef struct {

  filament_status_t status;
  uint32_t tick_start;
  uint32_t tick_end;
  uint32_t tick_delay;
}filament_check_t;

extern void printer_state_polling();
extern void filament_pin_setup();
extern void filament_check();
extern void filament_check_2();
extern bool get_filemant_pins(void);

#ifdef __cplusplus
  } /* C-declarations for C++ */
#endif
