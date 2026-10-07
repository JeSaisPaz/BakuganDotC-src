// bdc 0x088082f8 UiNameEntryColorsStaticInit
#include "bdc.h"

/* Static constructor (entry 1 of `g_cxxCtorTable`) of the name entry screen's colour constants:
   g_nameEntryColorDarkGreen (0, 0.50, 0, 1), g_nameEntryColorGreen (0, 1, 0, 1),
   g_nameEntryColorLightGreen (0.15, 1.0, 0.125, 1), g_nameEntryColorDimGreen (0.11, 0.22, 0.11, 1),
   g_nameEntryColorHighlight (0, 0.78, 0.20, 0.6), and copies the key-label table pointer
   g_uiKeyCharTable to g_nameEntryKeyTable. */

void UiNameEntryColorsStaticInit(void)

{
  g_nameEntryColorDarkGreen.x = 0.0f;
  g_nameEntryColorDarkGreen.y = 0.5019608f;
  g_nameEntryColorDarkGreen.z = 0.0f;
  g_nameEntryColorDarkGreen.w = 1.0f;
  g_nameEntryColorGreen.x = 0.0f;
  g_nameEntryColorGreen.y = 1.0f;
  g_nameEntryColorGreen.z = 0.0f;
  g_nameEntryColorGreen.w = 1.0f;
  g_nameEntryColorLightGreen.x = 0.15294118f;
  g_nameEntryColorLightGreen.y = 0.99607843f;
  g_nameEntryColorLightGreen.z = 0.1254902f;
  g_nameEntryColorLightGreen.w = 1.0f;
  g_nameEntryColorDimGreen.x = 0.10980392f;
  g_nameEntryColorDimGreen.y = 0.22352941f;
  g_nameEntryColorDimGreen.z = 0.10980392f;
  g_nameEntryColorDimGreen.w = 1.0f;
  g_nameEntryColorHighlight.x = 0.0f;
  g_nameEntryColorHighlight.y = 0.78431374f;
  g_nameEntryColorHighlight.z = 0.19607843f;
  g_nameEntryColorHighlight.w = 0.6f;
  g_nameEntryKeyTable = g_uiKeyCharTable;
}
