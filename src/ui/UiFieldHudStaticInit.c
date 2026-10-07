// bdc 0x088d3acc UiFieldHudStaticInit
#include "bdc.h"

/* Static initialiser (entry 18 of the static-constructor table `0x08af5bbc`) of the field HUD
   module: sets `0x08abebe0 = 6`, `0x08abebe4 = 150` and the float `0x08abebe8 = 16.0`, read by
   `UiFieldHudShowPromptA`, `UiFieldHudShowPromptB` and `UiFieldHudBuildGuideIcons`. */

void UiFieldHudStaticInit(void)

{
  g_uiFieldHudPromptParamA = 6;
  g_uiFieldHudPromptParamB = 0x96;
  g_uiFieldHudPromptScale = 16.0f;
  return;
}

