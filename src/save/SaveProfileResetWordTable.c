// bdc 0x0880d0dc SaveProfileResetWordTable
#include "bdc.h"

/* Clears the profile's 300-byte word table (`self->words`, the one `SaveProfileGetWord` /
   `SaveProfileSetWord` index): remembers word 0 (the profile flag bitset), zero-fills the whole
   table, optionally restores bit `0x40000000` of the old flags, then sets words 54..61 and 62..69
   (byte offsets `0xd8` and `0xf8`) to `0xff`, each write going through `SaveGetProfile` and
   skipped when its table is NULL. Does nothing when `self->words` is NULL.
   `ScriptOpResetProfile` calls it with `keepFlag = 1` right after `SaveProfileReset`. */

void SaveProfileResetWordTable(SaveProfile *self, bool keepFlag)

{
  u32 oldFlags;
  SaveProfile *profile;
  s32 i;
  s32 j;

  if (self->words != NULL) {
    oldFlags = SaveProfileGetFlags(self);
    memset(self->words, 0, 300);
    if (keepFlag) {
      SaveProfileSetFlags(self, oldFlags & 0x40000000);
    }
    for (i = 0; i < 4; i++) {
      for (j = 0; j < 2; j++) {
        profile = SaveGetProfile();
        if (profile->words != NULL) {
          profile->words[0x36 + i * 2 + j] = 0xff;
        }
      }
    }
    for (i = 0; i < 4; i++) {
      for (j = 0; j < 2; j++) {
        profile = SaveGetProfile();
        if (profile->words != NULL) {
          profile->words[0x3e + i * 2 + j] = 0xff;
        }
      }
    }
  }
}
