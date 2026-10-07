// bdc 0x08809148 UiLoadIconHide
#include "bdc.h"

/* Hides the 'now loading' icon if it exists (`UiLoadIconSetVisible(g_loadIcon, 0)`). Counterpart of
   `UiLoadIconShow`; two callers, one is `GfxInitBootResources`. */

void UiLoadIconHide(void)

{
  if (g_loadIcon != (void *)0x0) {
    UiLoadIconSetVisible(g_loadIcon,'\0');
  }
  return;
}

