// bdc 0x08996204 UiUnlockCodeColorsStaticInit
#include "bdc.h"

/* Static constructor of the unlock-code screen translation unit (`UiUnlockCodeIntroPhase` ends
   right before it): fills five RGBA colours — g_unlockCodeColorDarkGreen (0, 0.50, 0, 1),
   g_unlockCodeColorGreen (0, 1, 0, 1), g_unlockCodeColorLightGreen (0.15, 1.0, 0.125, 1),
   g_unlockCodeColorDimGreen (0.11, 0.22, 0.11, 1), g_unlockCodeColorHighlight (0, 0.78, 0.20,
   0.6) — and copies the keyboard character-table pointer g_uiKeyCharTable to g_unlockKeyTable. */

void UiUnlockCodeColorsStaticInit(void)

{
  g_unlockCodeColorDarkGreen.x = 0.0f;
  g_unlockCodeColorDarkGreen.y = 0.5019608f;
  g_unlockCodeColorDarkGreen.z = 0.0f;
  g_unlockCodeColorDarkGreen.w = 1.0f;
  g_unlockCodeColorGreen.x = 0.0f;
  g_unlockCodeColorGreen.y = 1.0f;
  g_unlockCodeColorGreen.z = 0.0f;
  g_unlockCodeColorGreen.w = 1.0f;
  g_unlockCodeColorLightGreen.x = 0.15294118f;
  g_unlockCodeColorLightGreen.y = 0.99607843f;
  g_unlockCodeColorLightGreen.z = 0.1254902f;
  g_unlockCodeColorLightGreen.w = 1.0f;
  g_unlockCodeColorDimGreen.x = 0.10980392f;
  g_unlockCodeColorDimGreen.y = 0.22352941f;
  g_unlockCodeColorDimGreen.z = 0.10980392f;
  g_unlockCodeColorDimGreen.w = 1.0f;
  g_unlockCodeColorHighlight.x = 0.0f;
  g_unlockCodeColorHighlight.y = 0.78431374f;
  g_unlockCodeColorHighlight.z = 0.19607843f;
  g_unlockCodeColorHighlight.w = 0.6f;
  g_unlockKeyTable = g_uiKeyCharTable;
}
