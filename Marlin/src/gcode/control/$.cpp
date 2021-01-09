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
#include "../../inc/MarlinConfigPre.h"
#include "../gcode.h"
#include "../queue.h"

#if HAS_CUTTER
	void GcodeSuite::$(const uint8_t code) {
		if(code == '$') {
			SERIAL_ECHOLNPGM("$0=10 (step pulse, usec)");
			SERIAL_ECHOLNPGM("$1=25 (step idle delay, msec)");
			SERIAL_ECHOLNPGM("$2=0 (step port invert mask:00000000)");
			SERIAL_ECHOLNPGM("$3=0 (dir port invert mask:00000000)");
			SERIAL_ECHOLNPGM("$4=0 (step enable invert, bool)");
			SERIAL_ECHOLNPGM("$5=0 (limit pins invert, bool)");
			SERIAL_ECHOLNPGM("$6=0 (probe pin invert, bool)");
			SERIAL_ECHOLNPGM("$10=3 (status report mask:00000011)");
			SERIAL_ECHOLNPGM("$11=0.010 (junction deviation, mm)");
			SERIAL_ECHOLNPGM("$12=0.002 (arc tolerance, mm)");
			SERIAL_ECHOLNPGM("$13=0 (report inches, bool)");
			SERIAL_ECHOLNPGM("$20=0 (soft limits, bool)");
			SERIAL_ECHOLNPGM("$21=0 (hard limits, bool)");
			SERIAL_ECHOLNPGM("$22=0 (homing cycle, bool)");
			SERIAL_ECHOLNPGM("$23=0 (homing dir invert mask:00000000)");
			SERIAL_ECHOLNPGM("$24=25.000 (homing feed, mm/min)");
			SERIAL_ECHOLNPGM("$25=500.000 (homing seek, mm/min)");
			SERIAL_ECHOLNPGM("$26=250 (homing debounce, msec)");
			SERIAL_ECHOLNPGM("$27=1.000 (homing pull-off, mm)");
			SERIAL_ECHOLNPGM("$100=250.000 (x, step/mm)");
			SERIAL_ECHOLNPGM("$101=250.000 (y, step/mm)");
			SERIAL_ECHOLNPGM("$102=250.000 (z, step/mm)");
			SERIAL_ECHOLNPGM("$110=500.000 (x max rate, mm/min)");
			SERIAL_ECHOLNPGM("$111=500.000 (y max rate, mm/min)");
			SERIAL_ECHOLNPGM("$112=500.000 (z max rate, mm/min)");
			SERIAL_ECHOLNPGM("$120=10.000 (x accel, mm/sec^2)");
			SERIAL_ECHOLNPGM("$121=10.000 (y accel, mm/sec^2)");
			SERIAL_ECHOLNPGM("$122=10.000 (z accel, mm/sec^2)");
			SERIAL_ECHOLNPGM("$130=200.000 (x max travel, mm)");
			SERIAL_ECHOLNPGM("$131=200.000 (y max travel, mm)");
			SERIAL_ECHOLNPGM("$132=200.000 (z max travel, mm)");
		}
		else if(code == '\0') {
			SERIAL_ECHOLNPGM("$$ (view Grbl settings)");
			SERIAL_ECHOLNPGM("$# (view # parameters)");
			SERIAL_ECHOLNPGM("$G (view parser state)");
			SERIAL_ECHOLNPGM("$I (view build info)");
			SERIAL_ECHOLNPGM("$N (view startup blocks)");
			SERIAL_ECHOLNPGM("$x=value (save Grbl setting)");
			SERIAL_ECHOLNPGM("$Nx=line (save startup block)");
			SERIAL_ECHOLNPGM("$C (check gcode mode)");
			SERIAL_ECHOLNPGM("$X (kill alarm lock)");
			SERIAL_ECHOLNPGM("$H (run homing cycle)");
			SERIAL_ECHOLNPGM("~ (cycle start)");
			SERIAL_ECHOLNPGM("! (feed hold)");
			SERIAL_ECHOLNPGM("? (current status)");
			SERIAL_ECHOLNPGM("ctrl-x (reset Grbl)");
		}
		else if(code == 'J') {
			if(BUFSIZE - queue.length >= 3) {
				char *q;
				char buf[MAX_CMD_SIZE];
				q = buf;
				parser.command_ptr += 3;
				//G91 or G90
				*q++ = *parser.command_ptr++;
				*q++ = *parser.command_ptr++;
				*q++ = *parser.command_ptr++;
				*q = '\0';
				queue.enqueue_one_P(PSTR(buf));

				q = buf;
				*q++ = 'G';
				*q++ = '1';
				
				for(uint8_t j = 0;j < (MAX_CMD_SIZE - 10) && *parser.command_ptr != '\0';j++) {
					*q++ = *parser.command_ptr++;
				}
				*q = '\0';
				queue.enqueue_one_P(PSTR(buf));
				queue.enqueue_one_P(PSTR("G90"));
			}
		}
	}
#endif
