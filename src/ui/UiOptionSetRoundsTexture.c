// bdc 0x08970e6c UiOptionSetRoundsTexture
#include "bdc.h"

/* Sets the value sprite of the rounds row of `UiOption`: texture
   `"option_battle_t_07"` when the value is -1 (none), else `"option_battle_t_03"` with cell
   `value`. */

void UiOptionSetRoundsTexture(UiOption *self, GfxSprite *sprite, s8 value)
{
    char name[64];

    if (value == -1) {
        sprintf(name, "option_battle_t_07");
        sprite->texture = GfxFindTexture(name);
    } else {
        sprintf(name, "option_battle_t_03");
        sprite->texture = GfxFindTexture(name);
        GfxSpriteSetCell(sprite, 0.0f, (float)value);
    }
}
