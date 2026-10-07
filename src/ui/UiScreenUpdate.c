// bdc 0x08909c48 UiScreenUpdate
#include "bdc.h"

/* Update (vtable slot 2) of the `UiScreen` base class: empty. Screens that add no
   behaviour (ids 320's siblings 360, 365, 370, 372) keep it. */
void UiScreenUpdate(UiScreen *screen)
{
    (void)screen;
}
