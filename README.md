# Mks-Robin-Nano-Marlin2.0-Firmware
## Features
The firmware of Mks Robin Nano, based on [Marlin2.0.x](https://github.com/MarlinFirmware/Marlin)(The based version is based on Marlin2.0.5), added the [LittlevGL](https://github.com/littlevgl/lvgl), supporting colourful GUI and touch screen. It is developed on PlatformIO, we hope more and more developers will participate the development of this repository.

![](https://github.com/makerbase-mks/Mks-Robin-Nano-Marlin2.0-Firmware/blob/master/Images/MKS_Robin_Nano_printing.png)

## Build
As the firmware is based on Marlin2.0.x which is built on the core of PlatformIO, the buid compiling steps are the same as Marlin2.0.x. You can directly using [PlatformIO Shell Commands](https://docs.platformio.org/en/latest/core/installation.html#piocore-install-shell-commands), or using IDEs contain built-in PlatformIO Core(CLI), for example, [VSCode](https://docs.platformio.org/en/latest/integration/ide/vscode.html#ide-vscode) and [Atom](https://docs.platformio.org/en/latest/integration/ide/atom.html). VSCode is recommended.

## About the gcode file preview
The images should be added to gcode file when slicing, and MKS has developed the [plugin for Cura](https://github.com/makerbase-mks/mks-wifi-plugin) to make it.

## About the image conversion
- Open [lvgl image convert](https://github.com/makerbase-mks/Software/tree/master/Lvgl_image_convert_tool) software. 
- Open bmp images.
- Enter the saved file name.
- Choose file output format:Bin_565.
- Start convertion.
- Save bin file.
- Copy the converted bin file to the mks_pic folder.
- Copy the mks_pic folder to the SD card.
- SD card is connected to the motherboard, and you can see the update interface after powering on.

## More information about the Robin Nano
Please refer to [MKS Robin Nano github](https://github.com/makerbase-mks/MKS-Robin-Nano).

