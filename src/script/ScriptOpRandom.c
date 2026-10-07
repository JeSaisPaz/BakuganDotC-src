// bdc 0x089cb220 ScriptOpRandom
#include "bdc.h"

/* Random number: stores into a variable either the raw VFPU random (`vrndi.s`, when the max is
   0) or `((rnd >> 16) * max) >> 16`, i.e. an integer in `[0, max)`. Returns 0. */

int ScriptOpRandom(Script *script)

{
  u32 *dst;
  u32 max;
  u32 rnd;
  u32 result;

  dst = ScriptReadRef(script,2);
  max = ScriptReadU32(script);
  rnd = PlatformRandU32();
  result = rnd;
  if (max != 0) {
    result = (rnd >> 16) * max >> 16;
  }
  *dst = result;
  return 0;
}
