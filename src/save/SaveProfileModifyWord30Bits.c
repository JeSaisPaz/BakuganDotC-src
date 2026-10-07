// bdc 0x089b1c6c SaveProfileModifyWord30Bits
#include "bdc.h"

/* Modifies the 16-bit flag word 0x30 of the player profile: `op` 0 clears the bits of `mask`, 1
   sets them, any other value clears the whole word (`SaveProfileGetWord`/`SaveProfileSetWord`).
    */

void SaveProfileModifyWord30Bits(u8 op, u16 mask)

{
  SaveProfile *profile;
  u32 word;
  
  if (op == '\0') {
    profile = SaveGetProfile();
    word = SaveProfileGetWord(profile,0x30);
    profile = SaveGetProfile();
    SaveProfileSetWord(profile,0x30,word & 0xffff & ~(uint)mask);
  }
  else if (op < 2) {
    profile = SaveGetProfile();
    word = SaveProfileGetWord(profile,0x30);
    profile = SaveGetProfile();
    SaveProfileSetWord(profile,0x30,word & 0xffff | (uint)mask);
  }
  else {
    profile = SaveGetProfile();
    SaveProfileSetWord(profile,0x30,0);
  }
  return;
}

