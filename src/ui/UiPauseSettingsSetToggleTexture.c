// bdc 0x089acb98 UiPauseSettingsSetToggleTexture
#include "bdc.h"

/* Sets the advisor toggle sprite's texture to `option_sol00` (`on` set) or `option_sol01`, with a
   96x32 UV rect at (0,0). */

void UiPauseSettingsSetToggleTexture(UiPauseSettings *self, GfxSprite *sprite, u8 on)
{
    float rect[4];
    char name[64];

    if (on == 0) {
        sprintf(name, "option_sol01");
    } else {
        sprintf(name, "option_sol00");
    }
    sprite->texture = GfxFindTexture(name);
    rect[0] = 0.0f;
    rect[1] = 0.0f;
    rect[2] = 96.0f;
    rect[3] = 32.0f;
    GfxSpriteSetUvRectXYWH(sprite, rect);
}
