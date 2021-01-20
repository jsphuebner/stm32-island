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
#ifndef PWMGENERATION_H
#define PWMGENERATION_H

#include "my_fp.h"
#include "picontroller.h"

class PwmGeneration
{
   public:
      /** Default constructor */
      PwmGeneration(uint32_t timer);
      void SetPwmDigits(int digits);
      void SetPolarity(bool activeLow, uint32_t* banks, uint16_t* pins, int numBanks);
      void SetUdc(s32fp vtg) { udcController.SetRef(vtg); }
      void Start();
      void ConfigureUdcController(int kp, int ki);
      void Stop();
      int Run(s32fp udc);

   protected:

   private:
      uint32_t _timer;
      uint16_t angle;
      int shiftForTimer;
      PiController udcController;

      ///There are two update events per PWM period
      ///One when counter reaches top, one when it reaches bottom
      ///We set the repetition counter in a way, that the ISR
      ///Callback frequency is constant i.e. independent from PWM frequency
      ///- for 17.6 kHz: call ISR every four update events (that is every other period)
      ///- for 8.8kHz: call ISR every other update event (that is once per PWM period)
      ///- for 4.4kHz: call ISR on every update event (that is twice per period)
      const uint8_t repCounters[3] = { 3, 1, 0 };
};

#endif // PWMGENERATION_H
