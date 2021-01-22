/*
 * This file is part of the stm32-template project.
 *
 * Copyright (C) 2020 Johannes Huebner <dev@johanneshuebner.com>
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

/* This file contains a standard set of commands that are used by the
 * esp8266 web interface.
 * You can add your own commands if needed
 */
#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/memorymap.h>
#include "hwdefs.h"
#include "terminal.h"
#include "params.h"
#include "my_string.h"
#include "my_fp.h"
#include "printf.h"
#include "param_save.h"
#include "errormessage.h"
#include "stm32_can.h"
#include "terminalcommands.h"

#define NUM_BUF_LEN 15

static void LoadDefaults(char *arg);
static void Help(char *arg);
static void PrintSerial(char *arg);
static void PrintErrors(char *arg);

extern "C" const TERM_CMD TermCmds[] =
{
  { "set", TerminalCommands::ParamSet },
  { "get", TerminalCommands::ParamGet },
  { "flag", TerminalCommands::ParamFlag },
  { "stream", TerminalCommands::ParamStream },
  { "json", TerminalCommands::PrintParamsJson },
  { "can", TerminalCommands::MapCan },
  { "save", TerminalCommands::SaveParameters },
  { "load", TerminalCommands::LoadParameters },
  { "reset", TerminalCommands::Reset },
  { "fastuart", TerminalCommands::FastUart },
  { "defaults", LoadDefaults },
  { "help", Help },
  { "serial", PrintSerial },
  { "errors", PrintErrors },
  { NULL, NULL }
};

static void LoadDefaults(char *arg)
{
   arg = arg;
   Param::LoadDefaults();
   printf("Defaults loaded\r\n");
}

static void PrintErrors(char *arg)
{
   arg = arg;
   ErrorMessage::PrintAllErrors();
}

static void PrintSerial(char *arg)
{
   arg = arg;
   printf("%X%X%X\r\n", DESIG_UNIQUE_ID2, DESIG_UNIQUE_ID1, DESIG_UNIQUE_ID0);
}

static void Help(char *arg)
{
   //If you want you could print some instructions here
   //But since the terminal is mostly used by the web interface
   //it makes limited sense.
   arg = arg;
}
