// bdc 0x0880d860 SaveProfileSetPlayerName
#include "bdc.h"

/* Copies the 25-byte player name (12 two-byte characters plus terminator, game charset) from `name`
   to `+0x470` of the profile's save block. No-op for a NULL name or without a block. Used by
   `UiNameEntryConfirmPhase`; read back with `SaveProfileGetPlayerName`. */

void SaveProfileSetPlayerName(SaveProfile *self, const u8 *name)
{
  char *dst;
  int i;
  int n;

  if ((name != (const u8 *)0x0) && (self->data != (SaveProfileData *)0x0)) {
    dst = self->data->playerName;
    i = 0;
    for (n = 12; n != 0; n--) {
      dst[i] = name[i];
      dst[i + 1] = name[i + 1];
      i += 2;
    }
    dst[i] = name[i];
  }
}
