// bdc 0x089a5cf4 UiMenuFlagsTest
#include "bdc.h"

/* Returns bit `bit` of profile word 0x35 (non-zero when set); see `UiMenuFlagsModify`. */

u32 UiMenuFlagsTest(u32 bit)
{
  SaveProfile *self;
  u32 word;

  bit &= 0xffff;
  self = SaveGetProfile();
  word = SaveProfileGetWord(self, 0x35);
  return word & (1 << (bit & 0x1f));
}
