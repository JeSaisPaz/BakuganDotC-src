// bdc 0x0893ed80 UiPasscodeResetCursor
#include "bdc.h"

/* Moves the cursor of the sequence-code screen (task 374, `UiPasscodeCtor`; the player re-enters
   a sequence of up to 6 symbols from a 10-symbol pad and it is compared with the answer) onto the
   current selection and restores every button (symbol pad, icons, command buttons and labels) to
   scale 1 and its saved depth (`spriteDepth`). Resets the pulse factor `cursorPulse`. */

#define PASSCODE_SPRITE_VISIBLE 1u
#define PASSCODE_SPRITE_FLAG_20 0x20u

void UiPasscodeResetCursor(UiScreen *screen)
{
    UiPasscode *pc = (UiPasscode *)screen;
    GfxSprite **sprites;
    int i;

    pc->cursorPulse = 0.0f;
    if (pc->onCommandRow == 0) {
        /* Symbol-pad cursor (sprite 0) over symbol sprite focusSymbol+1; hide the command cursor. */
        memset(&pc->tween[0], 0, sizeof(UiPasscodePartTween));
        sprites = (GfxSprite **)screen->data;
        sprites[0]->flags |= PASSCODE_SPRITE_VISIBLE;
        GfxSpriteCenterPivot(sprites[0]);
        sprites[0]->flags |= PASSCODE_SPRITE_FLAG_20;
        UiSpriteSetScaleRotation(sprites[0], 1.0f, 1.0f, 0.0f);
        sprites[0]->alpha = 1.0f;
        sprites[0]->posX = sprites[pc->focusSymbol + 1]->posX;
        sprites[0]->posY = sprites[pc->focusSymbol + 1]->posY;
        sprites[37]->flags &= ~PASSCODE_SPRITE_VISIBLE;
    } else {
        /* Command-row cursor (sprite 37) over command sprite command+11; hide the pad cursor. */
        memset(&pc->tween[37], 0, sizeof(UiPasscodePartTween));
        sprites = (GfxSprite **)screen->data;
        sprites[37]->flags |= PASSCODE_SPRITE_VISIBLE;
        GfxSpriteCenterPivot(sprites[37]);
        sprites[37]->flags |= PASSCODE_SPRITE_FLAG_20;
        UiSpriteSetScaleRotation(sprites[37], 1.0f, 1.0f, 0.0f);
        sprites[37]->alpha = 1.0f;
        sprites[37]->posX = sprites[pc->command + 11]->posX;
        sprites[37]->posY = sprites[pc->command + 11]->posY;
        sprites[0]->flags &= ~PASSCODE_SPRITE_VISIBLE;
    }
    for (i = 1; i < 11; i++) { /* symbol pad */
        UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
        ((GfxSprite **)screen->data)[i]->posZ = pc->spriteDepth[i];
    }
    for (i = 0x14; i < 0x1e; i++) {
        UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
        ((GfxSprite **)screen->data)[i]->posZ = pc->spriteDepth[i];
    }
    for (i = 11; i < 13; i++) { /* command buttons */
        UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
        ((GfxSprite **)screen->data)[i]->posZ = pc->spriteDepth[i];
    }
    for (i = 0x20; i < 0x22; i++) {
        UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
        ((GfxSprite **)screen->data)[i]->posZ = pc->spriteDepth[i];
    }
}
