// bdc 0x0880e3ec ScriptOpSetCpuClock
#include "bdc.h"

/* Script opcode handler: reads a u32 MHz value and sets the PSP CPU/bus clock with
   `CorePowerSetCpuClock(mhz)` (calls `scePowerSetClockFrequency(mhz, mhz, mhz/2)` if
   `scePowerGetCpuClockFrequencyInt()` differs). Always returns 0. */

int ScriptOpSetCpuClock(Script *script)

{
  u32 mhz;
  
  mhz = ScriptReadU32(script);
  CorePowerSetCpuClock(mhz);
  return 0;
}

