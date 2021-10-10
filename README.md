# Adattamento marlin 2.0.x Flyingbear Ghost 4s e 5 by Italy Makers

Adattamento a cura di Italy Makers del firmware Mks-Robin-Nano-Marlin2.0-Firmware per le stampanti Flying Bear Ghost 4s e 5

## _*firmware in fase di test*_

## Personalizzazioni necessarie

di seguito le personalizzazioni necessarie in base alla vostra stampante
nel file `Marlin/configuration.h`

### Driver

_in caso di utilizzo di **TMC2209** e **TMC2208** utilizzare `TMC2209_STANDALONE` e `TMC2208_STANDALONE`_

dalla riga 676

```C++
/**
 * Stepper Drivers
 *
 * These settings allow Marlin to tune stepper driver timing and enable advanced options for
 * stepper drivers that support them. You may also override timing options in Configuration_adv.h.
 *
 * A4988 is assumed for unspecified drivers.
 *
 * Use TMC2208/TMC2208_STANDALONE for TMC2225 drivers and TMC2209/TMC2209_STANDALONE for TMC2226 drivers.
 *
 * Options: A4988, A5984, DRV8825, LV8729, L6470, L6474, POWERSTEP01,
 *          TB6560, TB6600, TMC2100,
 *          TMC2130, TMC2130_STANDALONE, TMC2160, TMC2160_STANDALONE,
 *          TMC2208, TMC2208_STANDALONE, TMC2209, TMC2209_STANDALONE,
 *          TMC26X,  TMC26X_STANDALONE,  TMC2660, TMC2660_STANDALONE,
 *          TMC5130, TMC5130_STANDALONE, TMC5160, TMC5160_STANDALONE
 * :['A4988', 'A5984', 'DRV8825', 'LV8729', 'L6470', 'L6474', 'POWERSTEP01', 'TB6560', 'TB6600', 'TMC2100', 'TMC2130', 'TMC2130_STANDALONE', 'TMC2160', 'TMC2160_STANDALONE', 'TMC2208', 'TMC2208_STANDALONE', 'TMC2209', 'TMC2209_STANDALONE', 'TMC26X', 'TMC26X_STANDALONE', 'TMC2660', 'TMC2660_STANDALONE', 'TMC5130', 'TMC5130_STANDALONE', 'TMC5160', 'TMC5160_STANDALONE']
 */
#define  X_DRIVER_TYPE TMC2208_STANDALONE   //  A4988
#define  Y_DRIVER_TYPE TMC2208_STANDALONE   //  A4988
#define  Z_DRIVER_TYPE A4988   //  A4988
...
#define  E0_DRIVER_TYPE A4988   // A4988
```

### Step/mm

modificare gli step per mm in base alla vostra configurazione
riga 734

```C++
/**
 * Default Axis Steps Per Unit (steps/mm)
 * Override with M92
 *                                      X, Y, Z, E0 [, E1[, E2...]]
 */
#define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, 419}
```

per asse Z e Estrusore nel caso di modifica con TMC2208 o TMC2209
modificare a true i loro campi.

riga: 1122

```C++
// Invert the stepper direction. Change (or reverse the motor connector) if an axis goes the wrong way.
#define  INVERT_X_DIR false   // false
#define  INVERT_Y_DIR false   // true
#define  INVERT_Z_DIR false   // false

// @section extruder

// For direct drive extruder v9 set to true, for geared extruder set to false.
#define  INVERT_E0_DIR false   // false
```

branches:

```
master; //branch aggiornato all'ultima versione del firmware originale
fbGhost5
|    standard //branch con adattamenti per la stampante senza modifiche
|    bltouch; //branch con adattamento per l'utilizzo del BLtouch

fbGhost4s
|    standard //branch con adattamenti per la stampante senza modifiche

```

Credits:
fork from [Makerbase Robin Nano firmware](https://github.com/makerbase-mks/Mks-Robin-Nano-Marlin2.0-Firmware)
Italy Makers Flyingbear Ghost Group: [Italy Makers - Flying Bear Ghost](https://www.facebook.com/groups/907067056500590)


------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------

# Mks-Robin-Nano-Marlin2.0-Firmware
## Features
The firmware of MKS Robin Nano, based on [Marlin2.0.x](https://github.com/MarlinFirmware/Marlin), added the [LittlevGL](https://github.com/littlevgl/lvgl), supporting colourful GUI and touch screen. It is developed on PlatformIO, we hope more and more developers will participate the development of this repository.

![](https://github.com/makerbase-mks/Mks-Robin-Nano-Marlin2.0-Firmware/blob/master/Images/MKS_Robin_Nano_printing.png)

## Build
As the firmware is based on Marlin2.0.x which is built on the core of PlatformIO, the buid compiling steps are the same as Marlin2.0.x. You can directly using [PlatformIO Shell Commands](https://docs.platformio.org/en/latest/core/installation.html#piocore-install-shell-commands), or using IDEs contain built-in PlatformIO Core(CLI), for example, [VSCode](https://docs.platformio.org/en/latest/integration/ide/vscode.html#ide-vscode) and [Atom](https://docs.platformio.org/en/latest/integration/ide/atom.html). VSCode is recommended.

## About the gcode file preview
The images should be added to gcode file when slicing, and MKS has developed the [plugin for Cura](https://github.com/makerbase-mks/mks-wifi-plugin) to make it.

## About the image conversion
- Open [LVGL online image converter tool](https://lvgl.io/tools/imageconverter). 
- Open bmp images.
- Enter the saved file name.
- Choose color format:True color.
- Choose file output format:Binary RGB565.
- Start convertion.
- Save bin file.
- Copy the converted bin file to the assets folder.
- Copy the assets folder to the SD card.
- SD card is connected to the motherboard, and you can see the update interface after powering on.

## Firmware Can be run on Robin Nano V1.x / V2.x boards and V3.x boards
## MKS Robin Nano V1.x build and update firmware

1. Build config:
     
- platformio.ini: 
     
     default_envs = mks_robin_nano35    
- Configuation.h:  
     #define SERIAL_PORT 3  
     #define MKS_ROBIN_TFT35  
     #define MOTHERBOARD BOARD_MKS_ROBIN_NANO  
     #define TFT_LVGL_UI  
     #define TOUCH_SCREEN  

- Configuation_adv.h:  
     //#define USB_FLASH_DRIVE_SUPPORT  
     //#define MULTI_VOLUME

2. Update firmware:
   
- Enter the `.pio\build\mks_robin_nano35` directory, copy the `assets` folder and `Robin_nano35.bin` to the sd card
- Insert SD card to the motherboard, and you can see the update interface after power on.   

## MKS Robin Nano V2.x build and update firmware

1. Build config:
     
- platformio.ini: 
     
     default_envs = mks_robin_nano35    
- Configuation.h:   
     #define SERIAL_PORT 3  
     #define MKS_TS35_V2_0  
     #define MOTHERBOARD BOARD_MKS_ROBIN_NANO_V2     
     #define TFT_LVGL_UI  
     #define TOUCH_SCREEN  

- Configuation_adv.h:  
     //#define USB_FLASH_DRIVE_SUPPORT  
     //#define MULTI_VOLUME

2. Update firmware:
   
- Enter the `.pio\build\mks_robin_nano35` directory, copy the `assets` folder and `Robin_nano35.bin` to the sd card
- Insert SD card is to the motherboard, and you can see the update interface after power on.   

## MKS Robin Nano V3.x build and update firmware

1. Build config:
     
- platformio.ini: 
     
     default_envs = mks_robin_nano_v3_usb_flash_drive_msc
- Configuation.h:   
     #define SERIAL_PORT -1  
     #define MKS_TS35_V2_0  
     #define MOTHERBOARD BOARD_MKS_ROBIN_NANO_V3     
     #define TFT_LVGL_UI  
     #define TOUCH_SCREEN

- Configuation_adv.h:    
     After 2021.6.7, you can use the multi-volume function.     
     Use the TF card and USB disk together:   
     #define USB_FLASH_DRIVE_SUPPORT  
    Only use TF card:  
    // #define USB_FLASH_DRIVE_SUPPORT  

2. Update firmware:
   
- Enter the `.pio\build\mks_robin_nano35` directory, copy the `assets` folder and `Robin_nano_v3.bin` to the sd card or usb disk
- Insert sdcard or usb disk to the motherboard, and you can see the update interface after power on.  

3. Example build config:

- [Open the example configuration file](https://github.com/makerbase-mks/Mks-Robin-Nano-Marlin2.0-Firmware/tree/master/config/MKS%20Robin%20nano%20v3.0).
- Modify the parameters, replace configuration.h and configuration_adv.h in the Marlin path of the source code.
- Compile the firmware.

4. Prebuilt *.bin firmware for update

- We have prebuilt the robin nano v3 [firmware](https://github.com/makerbase-mks/MKS-Robin-Nano-V3.X/tree/main/firmware/Marlin-bugfix2.0.x-MKS-2.1.2) for some type of printers and some extended usage. 


## For more function configuration, please refer to Robin nano series Wiki
- [MKS Robin Nano V1.x Wiki](https://github.com/makerbase-mks/MKS-Robin-Nano-V1.X/wiki). 
- [MKS Robin Nano V2.x Wiki](https://github.com/makerbase-mks/MKS-Robin-Nano-V2.X/wiki). 
- [MKS Robin Nano V3.x Wiki](https://github.com/makerbase-mks/MKS-Robin-Nano-V3.X/wiki).

## More information about the Robin Nano V1.X
Please refer to [MKS Robin Nano github](https://github.com/makerbase-mks/MKS-Robin-Nano-V1.X).

##  More information about the Robin Nano V2.X
Please refer to [MKS Robin Nano V2 github](https://github.com/makerbase-mks/MKS-Robin-Nano-V2).

##  More information about the Robin Nano V3.X
Please refer to [MKS Robin Nano V3 github](https://github.com/makerbase-mks/MKS-Robin-Nano-V3.X).

