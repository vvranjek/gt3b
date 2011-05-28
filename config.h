/*
    config include file
    Copyright (C) 2011 Pavel Semerad

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef _CONFIG_INCLUDED
#define _CONFIG_INCLUDED


#include "gt3b.h"




// global config

// change MAGIC number when changing global config
// also add code to setting default values
// 29 bytes
#define CONFIG_GLOBAL_MAGIC  0xfd02
typedef struct {
    u8  steering_dead_zone;
    u8  throttle_dead_zone;
    u16 calib_steering_left;
    u16 calib_steering_mid;
    u16 calib_steering_right;
    u16 calib_throttle_fwd;
    u16 calib_throttle_mid;
    u16 calib_throttle_bck;
    u16 backlight_time;
    u16 magic_global;		// it is here because of problems when flashing
    u16 magic_model;		//  original fw back (1.byte was not overwrited)
    u8  model;			// selected model
    u16 battery_calib;		// raw ADC value for 10 Volts
    u8  battery_low;		// low battery threshold in .1 Volts
    u8  trim_step;
    u8  endpoint_max;
    u8  autorepeat;		// at what TRIM+DR is autorepeat on
    u8	key_beep:1;
    u8  ch3_momentary:1;
} config_global_s;

extern config_global_s config_global;
#define cg config_global


// threshold limits for steering/throttle
#define CALIB_ST_LOW_MID   300
#define CALIB_ST_MID_HIGH  750
#define CALIB_TH_LOW_MID   350
#define CALIB_TH_MID_HIGH  800




// model config

typedef struct {
    u8 function:7;
    u8 reverse:1;
    u8 step:5;
    u8 buttons:3;
} config_et_map_s;
#define ETB_LONG_OFF	0
#define ETB_AUTORPT	1
#define ETB_MOMENTARY	2
#define ETB_LONG_RESET	3
#define ETB_LONG_ENDVAL	4

typedef struct {
    u8 function:4;
    u8 function_long:4;
} config_key_map_s;
typedef struct {
    u8 momentary:3;	// bit for each of 3 buttons
    u8 et_off:5;	// bit for each of 3 buttons
} config_key_map2_s;

// change MAGIC number when changing model config
// also add code to setting default values
// 14 + channels * 3 bytes
#define CONFIG_MODEL_MAGIC  (0xff20 | (MAX_CHANNELS - 1))
typedef struct {
    u8 channels;		// number of channels for this model
    u8 name[3];
    u8 reverse;			// bit for each channel
    s8 subtrim[MAX_CHANNELS];
    u8 endpoint[MAX_CHANNELS][2];
    s8 trim[2];			// for steering and throttle
    u8 dualrate[3];		// for steering and throttle
    s8 expo[3];			// steering/forward/back
    u8 abs_type;
} config_model_s;

extern config_model_s config_model;
#define cm config_model
#define dr_steering	dualrate[0]
#define dr_forward	dualrate[1]
#define dr_back		dualrate[2]
#define expo_steering	expo[0]
#define expo_forward	expo[1]
#define expo_back	expo[2]



#include "eeprom.h"

#define CONFIG_MODEL_MAX  ((EEPROM_SIZE - sizeof(config_global_s)) / \
			    sizeof(config_model_s))

// when name[0] is 0xff, that model memory was empty
#define CONFIG_MODEL_EMPTY  0xff



// set default values
extern u8 config_global_set_default(void);
extern void config_model_set_default(void);


// read values from eeprom and set defaults when needed
extern void config_model_read(void);
extern u8 config_global_read(void);
extern u8 *config_model_name(u8 model);

// write values to eeprom
#define config_global_save()  eeprom_write_global()
#define config_model_save()   eeprom_write_model(config_global.model)


#endif

