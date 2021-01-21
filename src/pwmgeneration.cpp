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
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "pwmgeneration.h"
#include "hwdefs.h"
#include "sine_core.h"

#define FRQ_TO_ANGLE(frq) FP_TOINT(((frq << SineCore::BITS) + (1 << (SineCore::BITS - 1))) / (rcc_apb2_frequency / 8192))

PwmGeneration::PwmGeneration(uint32_t timer)
{
   _timer = timer;

   /* Center aligned PWM */
   timer_set_alignment(_timer, TIM_CR1_CMS_CENTER_1);
   timer_enable_preload(_timer);

   for (int channel = TIM_OC1; channel <= TIM_OC3N; channel++)
   {
      timer_enable_oc_preload(_timer, (tim_oc_id)channel);
      timer_set_oc_mode(_timer, (tim_oc_id)channel, TIM_OCM_PWM1);
      timer_set_oc_idle_state_unset(_timer, (tim_oc_id)channel);
      timer_set_oc_value(_timer, (tim_oc_id)channel, 0);
   }

   timer_disable_break_automatic_output(_timer);
   timer_set_break_polarity_low(_timer);

   /*if (hwRev == HW_BLUEPILL || hwRev == HW_PRIUS || hwRev == HW_TESLAM3)
      timer_set_break_polarity_low(_timer);
   else
      timer_set_break_polarity_high(_timer);*/

   timer_enable_break(_timer);
   timer_set_enabled_off_state_in_run_mode(_timer);
   timer_set_enabled_off_state_in_idle_mode(_timer);
   timer_set_deadtime(_timer, 0);
   timer_clear_flag(_timer, TIM_SR_UIF | TIM_SR_BIF);
   timer_enable_irq(_timer, TIM_DIER_UIE | TIM_DIER_BIE);

   timer_set_prescaler(_timer, 0);
   /* PWM frequency */
   timer_set_period(_timer, 1U << MIN_PWM_DIGITS);
   timer_set_repetition_counter(_timer, repCounters[0]);

   //timer_generate_event(_timer, TIM_EGR_UG);

   udcController.SetCallingFrequency(rcc_apb2_frequency / 8192);
}

void PwmGeneration::SetPwmDigits(int digits)
{
   if (digits < MIN_PWM_DIGITS) return;

   timer_disable_counter(_timer);
   timer_set_period(_timer, 1U << digits);
   timer_set_repetition_counter(_timer, repCounters[digits - MIN_PWM_DIGITS]);
   timer_generate_event(_timer, TIM_EGR_UG);
   timer_enable_counter(_timer);
   shiftForTimer = SineCore::BITS - digits;
   udcController.SetMinMaxY(0, (1U << digits) - 100);
}

void PwmGeneration::SetPolarity(bool activeLow, uint32_t* banks, uint16_t* pins, int numBanks)
{
   const uint8_t outputMode = activeLow ? GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN : GPIO_CNF_OUTPUT_ALTFN_PUSHPULL;

   for (int channel = TIM_OC1; channel <= TIM_OC3N; channel++)
   {
      if (activeLow)
         timer_set_oc_polarity_low(_timer, (tim_oc_id)channel);
      else
         timer_set_oc_polarity_high(_timer, (tim_oc_id)channel);
   }

   for (int i = 0; i < numBanks; i++)
   {
      gpio_set_mode(banks[i], GPIO_MODE_OUTPUT_50_MHZ, outputMode, pins[i]);
   }
}

void PwmGeneration::Start()
{
   timer_set_oc_polarity_low(_timer, TIM_OC2N);
   timer_enable_oc_output(_timer, TIM_OC1);
   timer_enable_oc_output(_timer, TIM_OC1N);
   //Booster connected to OC2N on Prius Gen 2 board
   //Do not run phase 2!
   timer_enable_oc_output(_timer, TIM_OC2N);
   timer_enable_oc_output(_timer, TIM_OC3);
   timer_enable_oc_output(_timer, TIM_OC3N);
   timer_enable_break_main_output(_timer);
   udcController.ResetIntegrator();
   timer_enable_irq(_timer, TIM_DIER_UIE | TIM_DIER_BIE);
   timer_enable_counter(_timer);
}

void PwmGeneration::Stop()
{
   timer_disable_oc_output(_timer, TIM_OC1);
   timer_disable_oc_output(_timer, TIM_OC2);
   timer_disable_oc_output(_timer, TIM_OC3);
   timer_disable_oc_output(_timer, TIM_OC1N);
   timer_disable_oc_output(_timer, TIM_OC2N);
   timer_disable_oc_output(_timer, TIM_OC3N);
   timer_disable_break_main_output(_timer);
   timer_disable_counter(_timer);
}

void PwmGeneration::ConfigureUdcController(int kp, int ki)
{
   udcController.SetGains(kp, ki);
}

void PwmGeneration::SetFrequency(s32fp frq)
{
   phaseIncrement = FRQ_TO_ANGLE(frq);
}

int PwmGeneration::Run(s32fp udc)
{
   int dutyCycle = udcController.Run(udc);
   int val1 = SineCore::Sine(angle) + 32768;
   int val2 = -SineCore::Sine(angle) + 32768;

   timer_set_oc_value(_timer, TIM_OC2, dutyCycle);

   timer_set_oc_value(_timer, TIM_OC1, val1 >> shiftForTimer);
   timer_set_oc_value(_timer, TIM_OC3, val2 >> shiftForTimer);

   angle += phaseIncrement;

   return dutyCycle;
}
