// bdc 0x0880d800 SaveProfileGetPlayerName
#include "bdc.h"

/* Copies the 0x19-byte player name stored in the profile data (`data->playerName`) into `out`.
   Returns 1 on success, 0 when `out` is NULL or the profile has no data. */

int SaveProfileGetPlayerName(SaveProfile *self, char *out)
{
  char *src;
  int i;
  int n;

  if ((out != (char *)0x0) && (self->data != (SaveProfileData *)0x0)) {
    src = self->data->playerName;
    i = 0;
    for (n = 12; n != 0; n--) {
      out[i] = src[i];
      out[i + 1] = src[i + 1];
      i += 2;
    }
    out[i] = src[i];
    return 1;
  }
  return 0;
}
