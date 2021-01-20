/*
 * This file is part of the stm32-island project.
 *
 * Copyright (C) 2021 Johannes Huebner <dev@johanneshuebner.com>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 //Define a version string of your firmware here
#define VER 1.00.R

/* Entries must be ordered as follows:
   1. Saveable parameters (id != 0)
   2. Temporary parameters (id = 0)
   3. Display values
 */
//Next param id (increase when adding new parameter!): 7
//Next value Id: 2005
/*              category      name         unit       min     max     default id */
#define PARAM_LIST \
    PARAM_ENTRY(CAT_INVERTER, pwmfrq,      PWMFRQS,   0,      2,      1,      13  ) \
    PARAM_ENTRY(CAT_INVERTER, pwmpol,      PWMPOLS,   0,      1,      0,      52  ) \
    PARAM_ENTRY(CAT_INVERTER, deadtime,    "dig",     0,      255,    63,     14  ) \
    PARAM_ENTRY(CAT_INVERTER, ocurlim,     "A",       -65536, 65536,  100,    22  ) \
    PARAM_ENTRY(CAT_INVERTER, il1gain,     "dig/A",   -100,   100,    4.7,    27  ) \
    PARAM_ENTRY(CAT_INVERTER, il2gain,     "dig/A",   -100,   100,    4.7,    28  ) \
    PARAM_ENTRY(CAT_INVERTER, udcgain,     "dig/V",   0,      4095,   6.175,  29  ) \
    PARAM_ENTRY(CAT_INVERTER, udcofs,      "dig",     0,      4095,   0,      77  ) \
    PARAM_ENTRY(CAT_INVERTER, udclim,      "V",       0,      1000,   540,    48  ) \
    PARAM_ENTRY(CAT_OPERATION,start,       OPMODES,   0,      1,      0,      0   ) \
    PARAM_ENTRY(CAT_OPERATION,udcspnt,     "V",       0,      600,    0,      3   ) \
    PARAM_ENTRY(CAT_OPERATION,udckp,       "",        0,      10000,  10,     4   ) \
    PARAM_ENTRY(CAT_OPERATION,udcki,       "",        0,      10000,  10,     5   ) \
    PARAM_ENTRY(CAT_OPERATION,minpwm,      "%",       0,      100,    0,      6   ) \
    PARAM_ENTRY(CAT_COMM,     canspeed,    CANSPEEDS, 0,      3,      1,      1   ) \
    PARAM_ENTRY(CAT_COMM,     canperiod,   CANPERIODS,0,      1,      0,      2   ) \
    VALUE_ENTRY(opmode,       OPMODES, 2000 ) \
    VALUE_ENTRY(udc,          "V",   2003 ) \
    VALUE_ENTRY(version,      VERSTR,  2001 ) \
    VALUE_ENTRY(lasterr,      errorListString,  2002 ) \
    VALUE_ENTRY(boosteramp,   "dig",   2003 ) \
    VALUE_ENTRY(cpuload,      "%",     2004 )


/***** Enum String definitions *****/
#define OPMODES       "0=Off, 1=Run"
#define PWMFRQS       "0=17.6kHz, 1=8.8kHz, 2=4.4KHz"
#define PWMPOLS       "0=ACTHIGH, 1=ACTLOW"
#define CANSPEEDS     "0=250k, 1=500k, 2=800k, 3=1M"
#define CANPERIODS    "0=100ms, 1=10ms"
#define CAT_TEST      "Testing"
#define CAT_COMM      "Communication"
#define CAT_INVERTER  "Inverter"
#define CAT_OPERATION "Operation"

#define VERSTR STRINGIFY(4=VER-name)

/***** enums ******/


enum _canspeeds
{
   CAN_PERIOD_100MS = 0,
   CAN_PERIOD_10MS,
   CAN_PERIOD_LAST
};

enum _modes
{
   MOD_OFF = 0,
   MOD_RUN,
   MOD_LAST
};

//Generated enum-string for possible errors
extern const char* errorListString;

