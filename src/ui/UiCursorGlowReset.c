// bdc 0x089a5238 UiCursorGlowReset
#include "bdc.h"

/* Resets the shared cursor-glow oscillator (`g_uiCursorGlow`: `falling` and `level`) used by
   `UiCursorGlowStep` by zeroing all 8 bytes. */
void UiCursorGlowReset(void)
{
  memset(&g_uiCursorGlow, 0, sizeof(g_uiCursorGlow));
}
