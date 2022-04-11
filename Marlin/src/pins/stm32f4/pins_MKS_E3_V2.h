/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
// MKS Robin E3 V2 pinmap
//

// Avoid conflict with TIMER_TONE
#include "env_validate.h"

//
// Release PB4 (Z_DIR_PIN) from JTAG NRST role
//
// #define DISABLE_DEBUG

#define BOARD_INFO_NAME "MKS Robin Nano E3"

// Use one of these or SDCard-based Emulation will be used
//#define SRAM_EEPROM_EMULATION                   // Use BackSRAM-based EEPROM emulation
//#define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation
#if EITHER(NO_EEPROM_SELECTED, I2C_EEPROM)
  #define I2C_EEPROM
  #define MARLIN_EEPROM_SIZE              0x1000  // 4KB
  #define I2C_SCL_PIN                       PB6
  #define I2C_SDA_PIN                       PB7
#endif

//
// Release PB4 (Z_DIR_PIN) from JTAG NRST role
//
//#define DISABLE_DEBUG

//
// Servos
//
#define SERVO0_PIN                          PA3   // Enable BLTOUCH

//
// Limit Switches
//
#define X_DIAG_PIN                          PA12
#define Y_DIAG_PIN                          PA11
#define Z_DIAG_PIN                          PC6
#define E0_DIAG_PIN                         PB1

#define X_STOP_PIN                        X_DIAG_PIN
#define Y_STOP_PIN                        Y_DIAG_PIN
#define Z_MIN_PIN                         Z_DIAG_PIN
// #define Z_MAX_PIN                         C14

//
// Steppers
//
#define X_ENABLE_PIN                        PC13
#define X_STEP_PIN                          PC0
#define X_DIR_PIN                           PB2

#define Y_ENABLE_PIN                        PC13
#define Y_STEP_PIN                          PC2
#define Y_DIR_PIN                           PB9

#define Z_ENABLE_PIN                        PB8
#define Z_STEP_PIN                          PC14
#define Z_DIR_PIN                           PC15

#define E0_ENABLE_PIN                       PB5
#define E0_STEP_PIN                         PB4
#define E0_DIR_PIN                          PB3

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA0   // TH1
#define TEMP_BED_PIN                        PA1   // TB1

#define FIL_RUNOUT_PIN                      PB10  // MT_DET


//
// Power Supply Control
//
#if ENABLED(MKS_PWC)
  #define PS_ON_PIN                         PA14  // PW_OFF
  #define KILL_PIN                          PB10  // PW_DET
  #define KILL_PIN_STATE                    HIGH
#endif

/**
 *                ------                                    ------                                  ------
 *  (BEEPER) PC1 |10  9 | PC3 (BTN_ENC)        (MISO) PB14 |10  9 | PB13 (SD_SCK)    (BEEPER) PC1  |10  9 | PC3 (BTN_ENC)
 *  (LCD_EN) PA4 | 8  7 | PA5 (LCD_RS)      (BTN_EN1) PB11 | 8  7 | PA15 (SD_SS)    (BTN_EN1) PB11 | 8  7 | RESET
 *  (LCD_D4) PA6   6  5 | PA7 (LCD_D5)      (BTN_EN2)  PB0   6  5 | PB15 (SD_MOSI)  (BTN_EN2) PB0    6  5 | PA6  (LCD_D4)
 *  (LCD_D6) PC4 | 4  3 | PC5 (LCD_D7)    (SD_DETECT) PC10 | 4  3 | RESET            (LCD_RS) PA5  | 4  3 | PA4  (LCD_EN)
 *           GND | 2  1 | 5V                           GND | 2  1 | NC                        GND  | 2  1 | 5V
 *                ------                                    ------                                  ------
 *                 EXP1                                      EXP2                                "Ender-3 EXP1"
 */
#define EXP1_03_PIN                         PC5
#define EXP1_04_PIN                         PC4
#define EXP1_05_PIN                         PA7
#define EXP1_06_PIN                         PA6
#define EXP1_07_PIN                         PA5
#define EXP1_08_PIN                         PA4
#define EXP1_09_PIN                         PC3
#define EXP1_10_PIN                         PC1

#define EXP2_03_PIN                         -1   // RESET
#define EXP2_04_PIN                         PC10
#define EXP2_05_PIN                         PB15
#define EXP2_06_PIN                         PB0
#define EXP2_07_PIN                         PA15
#define EXP2_08_PIN                         PB12
#define EXP2_09_PIN                         PB13
#define EXP2_10_PIN                         PB14

// "Ender-3 EXP1"
#define E3_EXP1_03_PIN                      PA4
#define E3_EXP1_04_PIN                      PA5
#define E3_EXP1_05_PIN                      PA6
#define E3_EXP1_06_PIN                      PB0
#define E3_EXP1_07_PIN                      -1   // RESET
#define E3_EXP1_08_PIN                      PB12
#define E3_EXP1_09_PIN                      PC3
#define E3_EXP1_10_PIN                      PC1

#if HAS_WIRED_LCD

  #define BEEPER_PIN                 EXP1_10_PIN
  #define BTN_ENC                    EXP1_09_PIN
  #define LCD_PINS_ENABLE            EXP1_08_PIN
  #define LCD_PINS_RS                EXP1_07_PIN
  #define BTN_EN1                    EXP2_08_PIN
  #define BTN_EN2                    EXP2_06_PIN

  // MKS MINI12864 and MKS LCD12864B; If using MKS LCD12864A (Need to remove RPK2 resistor)
  #if ENABLED(MKS_MINI_12864)

    #define LCD_BACKLIGHT_PIN               -1
    #define LCD_RESET_PIN                   -1
    #define DOGLCD_A0                EXP1_04_PIN
    #define DOGLCD_CS                EXP1_05_PIN
    #define DOGLCD_SCK               EXP2_09_PIN
    #define DOGLCD_MOSI              EXP2_05_PIN

  #elif ENABLED(MKS_MINI_12864_V3)
    #define DOGLCD_CS                EXP1_08_PIN
    #define DOGLCD_A0                EXP1_07_PIN
    #define LCD_PINS_DC                DOGLCD_A0
    #define LCD_BACKLIGHT_PIN               -1
    #define LCD_RESET_PIN            EXP1_06_PIN
    #define NEOPIXEL_PIN             EXP1_05_PIN
    #define DOGLCD_MOSI              EXP2_05_PIN
    #define DOGLCD_SCK               EXP2_09_PIN
    #define FORCE_SOFT_SPI
    #define SOFTWARE_SPI
    //#define LCD_SCREEN_ROT_180

  #else

    #define LCD_PINS_D4              EXP1_06_PIN
    #if IS_ULTIPANEL
      #define LCD_PINS_D5            EXP1_05_PIN
      #define LCD_PINS_D6            EXP1_04_PIN
      #define LCD_PINS_D7            EXP1_03_PIN

      #if !defined(BTN_ENC_EN) && ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif

    #endif

  #endif // !MKS_MINI_12864

#endif // HAS_WIRED_LCD


//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC9   // HEATER1
#define HEATER_BED_PIN                      PC8   // HOT BED

#define FAN_PIN                             PA8  // FAN
// #define FAN1_PIN                            PA7   // FAN1

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

//
// Onboard SD card
//
// detect pin doesn't work when ONBOARD and NO_SD_HOST_DRIVE disabled
#if SD_CONNECTION_IS(ONBOARD)
  #define ENABLE_SPI3
  #define SD_SS_PIN                         -1
  #define SDSS                              PA15
  #define SD_SCK_PIN                        PB13
  #define SD_MISO_PIN                       PB14
  #define SD_MOSI_PIN                       PB15
  #define SD_DETECT_PIN                     PC10
#endif