// bdc 0x08809070 UiLoadIconSetVisible
#include "bdc.h"

/* Shows (`visible != 0`) or hides the 'now loading' icon by writing `visible`; hiding also resets
   its spin speed `spinSpeed`. Called by `UiLoadIconShow` and `UiLoadIconHide`. */

void UiLoadIconSetVisible(UiLoadIcon *icon, u8 visible)
{
  icon->visible = visible;
  if (visible == 0) {
    icon->spinSpeed = 0.0f;
  }
}
