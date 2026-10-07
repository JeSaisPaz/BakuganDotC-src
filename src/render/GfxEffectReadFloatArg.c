// bdc 0x0881d898 GfxEffectReadFloatArg
#include "bdc.h"

/* Reads the next float argument of an effect command and advances `*cursor`; returns 0.0 past the
   last argument. Argument kinds (3 bits each in `cmd[0]`): 2 = random range `lo + (hi - lo) * rand`
   (VFPU `vrndf1` minus 1.0), 4 = effect register, the float at `effect + 0x1d0 + 4*index` where
   the argument word is the integer index; others are literal floats. */

float GfxEffectReadFloatArg(GfxEffect *effect, float *args, s32 *cursor, u16 *cmd)
{
  s32 idx = *cursor;
  s32 kind;
  float lo;
  float hi;

  if (idx >= (s32)(u8)cmd[1]) {
    return 0.0f;
  }
  kind = ((s32)cmd[0] >> ((idx * 3) & 0x1f)) & 7;
  if (kind < 3) {
    if (kind >= 2) {
      lo = args[idx];
      *cursor = idx + 1;
      hi = args[idx + 1];
      *cursor = idx + 2;
      hi = hi - lo;
      return lo + hi * (PlatformRandFloat12() - 1.0f);
    }
    lo = args[idx];
  } else {
    if (kind == 4) {
      lo = effect->vec1d0[((s32 *)args)[idx]];
      *cursor = idx + 1;
      return lo;
    }
    lo = args[idx];
  }
  *cursor = idx + 1;
  return lo;
}
