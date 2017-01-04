/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  Copyright (c) 2015 Atmel Corporation/Thibaut VIARD.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <sam.h>
#include "board_definitions.h"

/**
 * \brief system_init() configures the needed clocks and according Flash Read Wait States.
 * At reset:
 * - OSC16M clock source is enabled
 * - Generic Clock Generator 0 (GCLKMAIN) is using OSC16M as source.
 * We need to:
 * 1) Enable XOSC32K clock (External on-board 32.768Hz oscillator), will be used as DFLL48M reference.
 * 2) Put XOSC32K as source of Generic Clock Generator 1
 * 3) Put Generic Clock Generator 1 as source for DFLL48M reference
 * 4) Enable DFLL48M clock
 * 5) Switch Generic Clock Generator 3 to DFLL48M.
 */
// Constants for Clock generators
#define GCLK_GENERATOR_0  (0u)
#define GCLK_GENERATOR_1  (1u)
#define GCLK_GENERATOR_3  (3u)

void board_init(void)
{

  /* Set 0 Flash Wait States */
  NVMCTRL->CTRLB.bit.RWS = 0;

  /* Turn on the digital interface clock */
  MCLK->APBAMASK.reg |= MCLK_APBAMASK_GCLK;

  /* ----------------------------------------------------------------------------------------------
   * 1) Enable XOSC32K clock (External on-board 32.768Hz oscillator)
   */
  OSC32KCTRL->XOSC32K.bit.STARTUP  = 6; // SYSTEM_XOSC32K_STARTUP_65536
  OSC32KCTRL->XOSC32K.bit.XTALEN   = 1;
  OSC32KCTRL->XOSC32K.bit.EN1K     = 0;
  OSC32KCTRL->XOSC32K.bit.EN32K    = 1;
  OSC32KCTRL->XOSC32K.bit.ONDEMAND = 1;
  OSC32KCTRL->XOSC32K.bit.RUNSTDBY = 0;
  OSC32KCTRL->XOSC32K.bit.WRTLOCK  = 0;

  OSC32KCTRL->XOSC32K.reg |= OSC32KCTRL_XOSC32K_ENABLE;

  while(! (OSC32KCTRL->STATUS.reg & OSC32KCTRL_STATUS_XOSC32KRDY) == OSC32KCTRL_STATUS_XOSC32KRDY)
  {
    /* Wait for oscillator stabilization */
  }

  /* Software reset the GCLK module to ensure it is re-initialized correctly */
  GCLK->CTRLA.reg = GCLK_CTRLA_SWRST;
  while (GCLK->CTRLA.reg & GCLK_CTRLA_SWRST) {}
  while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST) {}

  /* ----------------------------------------------------------------------------------------------
   * 2) Put XOSC32K as source of Generic Clock Generator 1
   */

  while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(1 << GCLK_GENERATOR_1)) {
    /* Wait for synchronization */
  };
  GCLK->GENCTRL[GCLK_GENERATOR_1].reg = GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_XOSC32K;

  while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(1 << GCLK_GENERATOR_1) ) {
    /* Wait for synchronization */
  }

  /* ----------------------------------------------------------------------------------------------
   * 3) Put Generic Clock Generator 1 as source for DFLL48M 
   */

  /* Disable the peripheral channel */
  GCLK->PCHCTRL[OSCCTRL_GCLK_ID_DFLL48].reg &= ~GCLK_PCHCTRL_CHEN;

  while (GCLK->PCHCTRL[OSCCTRL_GCLK_ID_DFLL48].reg & GCLK_PCHCTRL_CHEN) {
      /* Wait for clock synchronization */
  }

  /* Configure the peripheral channel */
  GCLK->PCHCTRL[OSCCTRL_GCLK_ID_DFLL48].reg = GCLK_PCHCTRL_GEN(GCLK_GENERATOR_1);

  /* Enable the peripheral channel */
  GCLK->PCHCTRL[OSCCTRL_GCLK_ID_DFLL48].reg |= GCLK_PCHCTRL_CHEN;

  while (!(GCLK->PCHCTRL[OSCCTRL_GCLK_ID_DFLL48].reg & GCLK_PCHCTRL_CHEN)) {
      /* Wait for clock synchronization */
  }

  /* ----------------------------------------------------------------------------------------------
   * 4) Enable DFLL48M clock
   */

  /* DFLL Configuration in Closed Loop mode */

  /* Disable ONDEMAND mode while writing configurations */
  OSCCTRL->DFLLCTRL.reg = OSCCTRL_DFLLCTRL_ENABLE;
  while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY)) {
      /* Wait for DFLL sync */
  }

  OSCCTRL->DFLLMUL.reg =
          OSCCTRL_DFLLMUL_CSTEP((0x1f / 8)) |
          OSCCTRL_DFLLMUL_FSTEP((0xff / 8)) |
          OSCCTRL_DFLLMUL_MUL((48000000 / 32768));

  /* Disable ONDEMAND mode while writing configurations */
  OSCCTRL->DFLLCTRL.reg = OSCCTRL_DFLLCTRL_ENABLE;
  while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY)) {
      /* Wait for DFLL sync */
  }

  /* Using DFLL48M COARSE CAL value from NVM Software Calibration Area Mapping 
     in DFLL.COARSE helps to output a frequency close to 48 MHz.*/
#define NVM_DFLL_COARSE_POS    26 /* DFLL48M Coarse calibration value bit position.*/
#define NVM_DFLL_COARSE_SIZE   6  /* DFLL48M Coarse calibration value bit size.*/

  uint32_t coarse =( *((uint32_t *)(NVMCTRL_OTP5)
          + (NVM_DFLL_COARSE_POS / 32))
      >> (NVM_DFLL_COARSE_POS % 32))
      & ((1 << NVM_DFLL_COARSE_SIZE) - 1);
  /* In some revision chip, the Calibration value is not correct */
  if (coarse == 0x3f) {
      coarse = 0x1f;
  }
    
  OSCCTRL->DFLLVAL.reg = 
          OSCCTRL_DFLLVAL_COARSE(coarse) |
          OSCCTRL_DFLLVAL_FINE(512);

  /* Write full configuration to DFLL control register */
  OSCCTRL->DFLLCTRL.reg = 0;
  while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY)) {
      /* Wait for DFLL sync */
  }

  OSCCTRL->DFLLCTRL.reg = 
          OSCCTRL_DFLLCTRL_MODE    | // SYSTEM_CLOCK_DFLL_LOOP_MODE_CLOSED
          OSCCTRL_DFLLCTRL_ENABLE;

  uint32_t mask = (OSCCTRL_STATUS_DFLLRDY |
                  OSCCTRL_STATUS_DFLLLCKF | OSCCTRL_STATUS_DFLLLCKC);
  while(!((OSCCTRL->STATUS.reg & mask) == mask))
  {
    /* Wait for synchronization */
  }

  /* ----------------------------------------------------------------------------------------------
   * 5) Switch Generic Clock Generator 3 to DFLL48M.
   */

  while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(1 << GCLK_GENERATOR_3)) {
    /* Wait for synchronization */
  };

  GCLK->GENCTRL[GCLK_GENERATOR_3].reg = GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_DFLL48M;

  while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(1 << GCLK_GENERATOR_3) ) {
    /* Wait for synchronization */
  }

  /* ----------------------------------------------------------------------------------------------
   * 6) Setup Generic Clock Generator 0 with source OSC16M
   */

  /* set OSC16M to 16MHz */
  OSCCTRL->OSC16MCTRL.bit.FSEL = 3;
  OSCCTRL->OSC16MCTRL.bit.ONDEMAND = 1;
  OSCCTRL->OSC16MCTRL.bit.RUNSTDBY = 0;
 
  while ((OSCCTRL->STATUS.reg & OSCCTRL_STATUS_OSC16MRDY) == OSCCTRL_STATUS_OSC16MRDY) {
    /* Wait for synchronization */
  };

  MCLK->BUPDIV.reg = MCLK_BUPDIV_BUPDIV_DIV1;
  MCLK->LPDIV.reg  = MCLK_LPDIV_LPDIV_DIV1;
  MCLK->CPUDIV.reg = MCLK_CPUDIV_CPUDIV_DIV1;

  while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(1 << GCLK_GENERATOR_0)) {
    /* Wait for synchronization */
  };

  GCLK->GENCTRL[GCLK_GENERATOR_0].reg = GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC16M;

  while ( GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(1 << GCLK_GENERATOR_0) ) {
    /* Wait for synchronization */
  }

}
