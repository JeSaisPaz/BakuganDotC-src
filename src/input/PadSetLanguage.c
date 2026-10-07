// bdc 0x089ce698 PadSetLanguage
#include "bdc.h"

/* Stores the requested display language in the pad state (`pad+0x50`); `PadApplyLanguageMode`
   passes it to `sceImposeSetLanguageMode`. Called by `SaveProfileSetLanguage`. */

void PadSetLanguage(PadState *pad, s32 language)

{
  pad->id50 = language;
  pad->flag58 = '\0';
  return;
}

