// bdc 0x0894dde4 UiNetMenuGetSpriteTable
#include "bdc.h"

/* Returns the sprite table (UiScreen::data) of a UiNetMenu screen. */
GfxSprite **UiNetMenuGetSpriteTable(UiScreen *screen)
{
    return screen->data;
}
