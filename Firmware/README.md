# About Robin_nnao_V2.0. fix the problem of After updating the firmware the screen display disorder

## Problem Description 
- No "Booting..." is displayed on the screen after the motherboard is powered on.
- The motherboard displays the following phenomena after power-on.
![](https://github.com/makerbase-mks/Mks-Robin-Nano-Marlin2.0-Firmware/blob/Restore-spi_falsh-config/Images/display-disorder.png)

- If the above two phenomena, please follow the steps below to fix the problem.

## Repair steps

- Copy the currentdirectory mks_pic, mks_font, Robin_nano.bin files to the SD card. 
- SD card is connected to the motherboard, and powering on.
- Then you can see that the screen will be black for a period of time. This motherboard is updating the firmware. When the update is complete, the buzzer will beep twice.
- After updating the program, you will still see the screen display disorder at this time. You need to restart the motherboard again, the motherboard will update the mks_pic and mks_font files.
- After the update is complete, the screen displays normally.
- You can update to the latest firmware, it has fixed this problem and added more features.
- The name of the updated firmware afterwards is "Robin_nano35.bin"