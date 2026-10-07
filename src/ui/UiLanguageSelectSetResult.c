// bdc 0x0880968c UiLanguageSelectSetResult
#include "bdc.h"

/* Stores `value` in the script result word (`g_scriptGlobalVars[3]`, offset 0xc) so the boot
   script can read the language screen's outcome. The first argument (the screen) is unused. Called
   by `UiLanguageSelectCtor` and `UiLanguageSelectStateInput`. */

void UiLanguageSelectSetResult(void *self, s32 value)
{
  g_scriptGlobalVars[3] = value;
}
