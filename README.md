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
#define  X_DRIVER_TYPE A4988   //  A4988
#define  Y_DRIVER_TYPE A4988   //  A4988
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
