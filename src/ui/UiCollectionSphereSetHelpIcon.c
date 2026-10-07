// bdc 0x0897e9a8 UiCollectionSphereSetHelpIcon
#include "bdc.h"

/* Sets the cell of the button guide sprite 0x28 of `UiCollectionSphere`: 5
   in the detail view (`grid` = 0), 0 back on the grid. */
void UiCollectionSphereSetHelpIcon(UiScreen *screen, u8 grid)
{
    /* The class data block starts with a GfxSprite * table; no struct definition yet. */
    GfxSprite *helpIcon = ((GfxSprite **)screen->data)[0x28];

    if (grid == 0) {
        GfxSpriteSetCell(helpIcon, 0.0f, 5.0f);
        return;
    }
    GfxSpriteSetCell(helpIcon, 0.0f, 0.0f);
}
