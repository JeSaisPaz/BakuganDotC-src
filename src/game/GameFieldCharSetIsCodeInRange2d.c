// bdc 0x088f407c GameFieldCharSetIsCodeInRange2d
#include "bdc.h"

/* True when the character code is 0x2d..0x32. */

s32 GameFieldCharSetIsCodeInRange2d(void *mgr, u8 code)

{
  if ((0x2c < code) && (code < 0x33)) {
    return 1;
  }
  return 0;
}

