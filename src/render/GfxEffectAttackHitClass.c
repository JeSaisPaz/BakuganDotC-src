// bdc 0x0881db8c GfxEffectAttackHitClass
#include "bdc.h"

/* Returns the hit class of attack kind `kind` for `GfxEffectHitTest`: 2 for kinds 3–5, 9–0xb,
   0xf–0x11, 0x13, 0x15, 0x24, 0x26, 0x28, 0x2a–0x2e and 0xa7, 1 for everything else. The first
   argument is unused. */

s32 GfxEffectAttackHitClass(GfxEffect *effect, s32 kind)

{
  if (kind < 0x2f) {
    switch (kind) {
    case 3: case 4: case 5: case 9: case 10: case 0xb: case 0xf: case 0x10: case 0x11:
    case 0x13: case 0x15: case 0x24: case 0x26: case 0x28: case 0x2a: case 0x2b:
    case 0x2c: case 0x2d: case 0x2e:
      return 2;
    default:
      return 1;
    }
  }
  if (kind == 0xa7) {
    return 2;
  }
  return 1;
}
