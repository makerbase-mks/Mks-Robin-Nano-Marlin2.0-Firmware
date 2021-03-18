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

#if NOT_TARGET(STM32F4, STM32F4xx)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 3 || E_STEPPERS > 3
  #error "MKS Robin Pro V2 supports up to 3 hotends / E-steppers."
#elif HAS_FSMC_TFT
  #error "MKS Robin Pro V2 doesn't support FSMC-based TFT displays."
#endif

#define BOARD_INFO_NAME "MKS Robin Pro V2"

// USB Flash Drive support
#define HAS_OTG_USB_HOST_SUPPORT

#define USB_POWER_CONTROL_PIN               PE15

// Avoid conflict with TIMER_TONE
#define STEP_TIMER                            10

// Use one of these or SDCard-based Emulation will be used
//#define SRAM_EEPROM_EMULATION                   // Use BackSRAM-based EEPROM emulation
//#define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation
#define I2C_EEPROM
#define MARLIN_EEPROM_SIZE                0x1000  // 4KB
#define I2C_SCL_PIN                         PB6
#define I2C_SDA_PIN                         PB7

//
// Release PB4 (Z_DIR_PIN) from JTAG NRST role
//
//#define DISABLE_DEBUG

//
// Servos
//
#define SERVO0_PIN                          PA8   // Enable BLTOUCH

//
// Limit Switches
//
#define X_DIAG_PIN                          PA15
#define Y_DIAG_PIN                          PA12
#define Z_DIAG_PIN                          PA11
#define E0_DIAG_PIN                         PG7
#define E1_DIAG_PIN                         PG8

#define X_MIN_PIN                           PA15
#define X_MAX_PIN                           PG7
#define Y_MIN_PIN                           PA12
#define Y_MAX_PIN                           PG8
#define Z_MIN_PIN                           PA11
#define Z_MAX_PIN                           PC4

//
// Steppers
//
#define X_ENABLE_PIN                        PE4
#define X_STEP_PIN                          PE3
#define X_DIR_PIN                           PE2
#ifndef X_CS_PIN
  #define X_CS_PIN                          PD8
#endif

#define Y_ENABLE_PIN                        PE1
#define Y_STEP_PIN                          PE0
#define Y_DIR_PIN                           PB9
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PD9
#endif

#define Z_ENABLE_PIN                        PB8
#define Z_STEP_PIN                          PB5
#define Z_DIR_PIN                           PB4
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PD10
#endif

#define E0_ENABLE_PIN                       PB3
#define E0_STEP_PIN                         PD6
#define E0_DIR_PIN                          PD3
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PD13
#endif

#define E1_ENABLE_PIN                       PD1
#define E1_STEP_PIN                         PC15
#define E1_DIR_PIN                          PA1
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PD14
#endif

#define E2_ENABLE_PIN                       PF0
#define E2_STEP_PIN                         PF2
#define E2_DIR_PIN                          PF1
#ifndef E2_CS_PIN
  #define E2_CS_PIN                         PD15
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                     PD4
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                     PD5
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                      PD7
  #endif
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial1
  //#define X2_HARDWARE_SERIAL Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Y2_HARDWARE_SERIAL Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define Z2_HARDWARE_SERIAL Serial1
  //#define E0_HARDWARE_SERIAL Serial1
  //#define E1_HARDWARE_SERIAL Serial1
  //#define E2_HARDWARE_SERIAL Serial1
  //#define E3_HARDWARE_SERIAL Serial1
  //#define E4_HARDWARE_SERIAL Serial1

  //
  // Software serial
  //

  #define X_SERIAL_TX_PIN                   PD8
  #define X_SERIAL_RX_PIN                   PD8

  #define Y_SERIAL_TX_PIN                   PD9
  #define Y_SERIAL_RX_PIN                   PD9

  #define Z_SERIAL_TX_PIN                   PD10
  #define Z_SERIAL_RX_PIN                   PD10

  #define E0_SERIAL_TX_PIN                  PD13
  #define E0_SERIAL_RX_PIN                  PD13

  #define E1_SERIAL_TX_PIN                  PD14
  #define E1_SERIAL_RX_PIN                  PD14
  
  #define E2_SERIAL_TX_PIN                  PD15
  #define E2_SERIAL_RX_PIN                  PD15

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                     19200
#endif // HAS_TMC_UART

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH0 for He0
#define TEMP_1_PIN                          PF4   // TH1 for HE1
#define TEMP_2_PIN                          PF3   // TH2 for He2
#define TEMP_BED_PIN                        PC0   // TB1 for H-BED

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PF9   // HE0
#define HEATER_1_PIN                        PF8   // HE1
#define HEATER_2_PIN                        PF7   // HE2
#define HEATER_BED_PIN                      PA0   // H-BED

#define FAN_PIN                             PB1   // FAN0
#define FAN1_PIN                            PE11  // FAN1
#define FAN2_PIN                            PE10  // FAN2

//
// Misc. Functions
//
#define MT_DET_1                            PE6
#define MT_DET_2                            PC13
#define MT_DET_3                            PG14
#define PW_DET                              PD0
#define PW_OFF                              PG11

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                MT_DET_1
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN               MT_DET_2
#endif

#ifndef FIL_RUNOUT3_PIN
  #define FIL_RUNOUT3_PIN               MT_DET_3
#endif

#define MT_DET_1_PIN                    MT_DET_1
#define MT_DET_2_PIN                    MT_DET_2
#define MT_DET_3_PIN                    MT_DET_3
#define MT_DET_PIN_INVERTING               false // LVGL UI filament RUNOUT PIN STATE

//#define POWER_LOSS_PIN                    PW_DET
//#define PS_ON_PIN                         PW_OFF

//#define MKSPWC
#ifdef MKSPWC
  #define SUICIDE_PIN                       PW_OFF   // Enable MKSPWC SUICIDE PIN
  #define SUICIDE_PIN_INVERTING             false // Enable MKSPWC PIN STATE
  #define KILL_PIN                          PW_DET   // Enable MKSPWC DET PIN
  #define KILL_PIN_STATE                    true  // Enable MKSPWC PIN STATE
#endif

//#define MKS_TEST

#if ENABLED(MKS_TEST)
  #define MKS_TEST_POWER_LOSS_PIN         PW_DET   // PW_DET
  #define MKS_TEST_PS_ON_PIN              PW_OFF   // PW_OFF
#endif

//#define LED_PIN                           PB2

// Random Info
#define USB_SERIAL              -1  // USB Serial
#define WIFI_SERIAL              3  // USART3
#define MKS_WIFI_MODULE_SERIAL   1  // USART1
#define MKS_WIFI_MODULE_SPI      2  // SPI2

#define WIFI_IO0_PIN                      PG9   // MKS ESP WIFI IO0 PIN
#define WIFI_IO1_PIN                      PC7   // MKS ESP WIFI IO1 PIN
#define WIFI_RESET_PIN                    PC14  // MKS ESP WIFI RESET PIN

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

//
// Onboard SD card
//
#define SDIO_D0_PIN                         PC8
#define SDIO_D1_PIN                         PC9
#define SDIO_D2_PIN                         PC10
#define SDIO_D3_PIN                         PC11
#define SDIO_SCK_PIN                        PC12
#define SDIO_CMD_PIN                        PD2
// detect pin dont work when ONBOARD and NO_SD_HOST_DRIVE disabled
#if SD_CONNECTION_IS(ONBOARD)
  #define SDIO_SUPPORT
  #define SD_DETECT_PIN                     PD12

  #ifndef SDIO_CLOCK
    #define SDIO_CLOCK                      18000000       /* 18 MHz */
  #endif

  // SDIO retries, configurable. Default is 3, from STM32F1
  #ifndef SDIO_READ_RETRIES
    #define SDIO_READ_RETRIES               3
  #endif

  #ifndef SDIO_SUPPORT
    #define SOFTWARE_SPI                          // Use soft SPI for onboard SD
    #define SDSS                            SDIO_D3_PIN
    #define SD_SCK_PIN                      SDIO_SCK_PIN
    #define SD_MISO_PIN                     SDIO_D0_PIN
    #define SD_MOSI_PIN                     SDIO_CMD_PIN
  #endif
//
// LCD SD
// 
#elif SD_CONNECTION_IS(LCD)
  #define CUSTOM_SPI_PINS
  #if ENABLED(CUSTOM_SPI_PINS)
    #define ENABLE_SPI1
    #define SDSS                            PA4
    #define SD_SCK_PIN                      PA5
    #define SD_MISO_PIN                     PA6
    #define SD_MOSI_PIN                     PA7
    #define SD_DETECT_PIN                   PG3
  #endif
#endif

// LCD / Controller
#define SPI_FLASH
#define HAS_SPI_FLASH                          1
#define SPI_DEVICE                             2
#define SPI_FLASH_SIZE                 0x1000000
#if ENABLED(SPI_FLASH)
  #define W25QXX_CS_PIN                     PB12
  #define W25QXX_MOSI_PIN                   PC3
  #define W25QXX_MISO_PIN                   PC2
  #define W25QXX_SCK_PIN                    PB13
#endif

//
// LCD / Controller
//
/**
 *                _____                                             _____
 *   (BEEPER)PC5 | · · | PG2(BTN_ENC)              (SPI1 MISO) PA6 | · · | PA5 (SPI1 SCK)
 *   (LCD_EN)PG0 | · · | PG1(LCD_RS)                 (BTN_EN1) PG5 | · · | PA4 (SPI1 CS)
 *  (LCD_D4)PF14 | · ·   PF15(LCD_D5)                (BTN_EN2) PG4 | · ·   PA7 (SPI1 MOSI)
 *  (LCD_D6)PF12 | · · | PF13(LCD_D7)                (SPI1_RS) PG3 | · · | RESET
 *           GND | · · | 5V                                    GND | · · | 3.3V
 *                ￣￣￣                                            ￣￣￣
 *                EXP1                                               EXP2
 */
#define BEEPER_PIN                          PC5
#define LCD_PINS_RS                         PG1
#define LCD_PINS_ENABLE                     PG0
#define LCD_PINS_D4                         PF14
#define LCD_PINS_D5                         PF15
#define LCD_PINS_D6                         PF12
#define LCD_PINS_D7                         PF13
#define BTN_EN1                             PG5
#define BTN_EN2                             PG4
#define BTN_ENC                             PG2 

#if ANY(TFT_COLOR_UI, TFT_LVGL_UI, TFT_CLASSIC_UI)
  #ifndef TOUCH_CALIBRATION_X
    #define TOUCH_CALIBRATION_X           -17253
  #endif
  #ifndef TOUCH_CALIBRATION_Y
    #define TOUCH_CALIBRATION_Y            11579
  #endif
  #ifndef TOUCH_OFFSET_X
    #define TOUCH_OFFSET_X                   514
  #endif
  #ifndef TOUCH_OFFSET_Y
    #define TOUCH_OFFSET_Y                   -24
  #endif
  #ifndef TOUCH_ORIENTATION
    #define TOUCH_ORIENTATION    TOUCH_LANDSCAPE
  #endif

  #define TFT_CS_PIN                        PF12
  #define TFT_SCK_PIN                       PA5
  #define TFT_MISO_PIN                      PA6
  #define TFT_MOSI_PIN                      PA7
  #define TFT_DC_PIN                        PF13
  #define TFT_RST_PIN                       PG1
  #define TFT_A0_PIN                  TFT_DC_PIN

  #define TFT_RESET_PIN                     PG1
  #define TFT_BACKLIGHT_PIN                 PG0

  #define TOUCH_BUTTONS_HW_SPI
  #define TOUCH_BUTTONS_HW_SPI_DEVICE          1

  #define LCD_BACKLIGHT_PIN                 PG0
  #ifndef TFT_WIDTH
    #define TFT_WIDTH                        480
  #endif
  #ifndef TFT_HEIGHT
    #define TFT_HEIGHT                       320
  #endif

  #define TOUCH_CS_PIN                      PF14  // SPI1_NSS
  #define TOUCH_SCK_PIN                     PA5   // SPI1_SCK
  #define TOUCH_MISO_PIN                    PA6   // SPI1_MISO
  #define TOUCH_MOSI_PIN                    PA7   // SPI1_MOSI

  #define BTN_EN1                           PG5
  #define BTN_EN2                           PG4
  #define BTN_ENC                           PG2 
  #define BEEPER_PIN                        PC5

  #define LCD_READ_ID                       0xD3
  #define LCD_USE_DMA_SPI

  #define TFT_BUFFER_SIZE                  14400

#elif HAS_SPI_LCD
  #define BEEPER_PIN                        PC5
  #define BTN_ENC                           PG2
  #define LCD_PINS_ENABLE                   PG0
  #define LCD_PINS_RS                       PG1
  #define BTN_EN1                           PG5
  #define BTN_EN2                           PG4
  #define LCD_BACKLIGHT_PIN                 -1

  // MKS MINI12864 and MKS LCD12864B; If using MKS LCD12864A (Need to remove RPK2 resistor)
  #if ENABLED(MKS_MINI_12864)
    //#define LCD_BACKLIGHT_PIN             -1
    //#define LCD_RESET_PIN                 -1
    #define DOGLCD_A0                       PF12
    #define DOGLCD_CS                       PF15

    // Required for MKS_MINI_12864 with this board
    //#define MKS_LCD12864B
    //#undef SHOW_BOOTSCREEN

  #else // !MKS_MINI_12864

    #define LCD_PINS_D4                     PF14
    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5                   PF15
      #define LCD_PINS_D6                   PF12
      #define LCD_PINS_D7                   PF13
    #endif

    #define BOARD_ST7920_DELAY_1    DELAY_NS(96)
    #define BOARD_ST7920_DELAY_2    DELAY_NS(48)
    #define BOARD_ST7920_DELAY_3    DELAY_NS(600)

  #endif // !MKS_MINI_12864
#endif // HAS_SPI_LCD

//
// ST7920 Delays
//
#ifndef BOARD_ST7920_DELAY_1
  #define BOARD_ST7920_DELAY_1      DELAY_NS(96)
#endif
#ifndef BOARD_ST7920_DELAY_2
  #define BOARD_ST7920_DELAY_2      DELAY_NS(48)
#endif
#ifndef BOARD_ST7920_DELAY_3
  #define BOARD_ST7920_DELAY_3     DELAY_NS(715)
#endif
