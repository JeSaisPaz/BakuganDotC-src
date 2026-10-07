// bdc 0x089ce5f4 PadApplyLanguageMode
#include "bdc.h"

/* Final step of pad setup, run by `BootEndOfFrame` until it succeeds: reads the system language
   and button mode with `sceImposeGetLanguageMode`, remembers the system language in `pad+0x54` if
   that slot is still -1, picks `pad+0x50` as the language when it is a valid language id (< 12) and
   the remembered system language otherwise, stores the choice back in `pad+0x50`, and applies it
   with `sceImposeSetLanguageMode``(language, 1)`. Returns 1 if both Impose calls succeeded, else
   0; `BootEndOfFrame` stores that in `pad->flag58`. */

u32 PadApplyLanguageMode(PadState *pad)
{
  u32 ok = 0;
  int lang[2];

  if (sceImposeGetLanguageMode(&lang[0], &lang[1]) == 0) {
    if (pad->id54 == -1) {
      pad->id54 = lang[0];
    }
    if ((u32)pad->id50 < 12) {
      lang[0] = pad->id50;
    } else {
      lang[0] = pad->id54;
    }
    pad->id50 = lang[0];
    if (sceImposeSetLanguageMode(lang[0], 1) == 0) {
      ok = 1;
    }
  }
  return ok;
}
