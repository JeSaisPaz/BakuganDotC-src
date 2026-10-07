// bdc 0x0893f9e4 UiPasscodeShowSequence
#include "bdc.h"

/* Demonstration mode of the sequence-code screen (task 374, `UiPasscodeCtor`; the player
   re-enters a sequence of up to 6 symbols from a 10-symbol pad and it is compared with the answer)
   that shows the whole answer at once. One step per call, driven by `playStep`: after the same
   three countdown beeps as `UiPasscodePlaySequence` (0x2c0001a, then 0x2c0001b) it lays out all
   answer symbols (up to 6, until a 0xff terminator in `answer`) side by side on the entry frame
   (data sprites 14.., 18 px apart, centred on sprite 13) with sound 0x2c00020, holds them 30 frames,
   hides them and shows the prompt sprite (data sprite 34, sound 0x2c00021), blinking it every
   4 frames for 64 frames. Returns 0 while running, 1 once `playStep` is past the last step (>= 10). */

#define PASSCODE_SPRITE_VISIBLE 1u

#define PASSCODE_SPR_ANCHOR 13
#define PASSCODE_SPR_SHOW 14
#define PASSCODE_SPR_PROMPT 34

s32 UiPasscodeShowSequence(UiScreen *screen)
{
    UiPasscode *pc = (UiPasscode *)screen;
    GfxSprite *sprite;
    u8 count;
    s32 i;
    s32 x;

    switch (pc->playStep) {
    case 0:
        pc->playTimer = 30;
        pc->playCount = 0;
        pc->playStep++;
        break;
    case 1:
    case 4:
    case 6:
        if (pc->playTimer != 0) {
            pc->playTimer--;
        } else {
            pc->playStep++;
        }
        break;
    case 2:
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x2c0001a, 0, 0);
        }
        pc->playTimer = 30;
        pc->playStep++;
        break;
    case 3:
        if (pc->playTimer != 0) {
            pc->playTimer--;
            break;
        }
        pc->playCount++;
        if (pc->playCount == 3) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x2c0001b, 0, 0);
            }
            pc->playTimer = 30;
            pc->playCount = 0;
            pc->playStep++;
        } else {
            pc->playStep = 2;
        }
        break;
    case 5:
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x2c00020, 0, 0);
        }
        count = 0;
        for (i = 0; i < 6; i++) {
            if (pc->answer[i] == 0xff) {
                break;
            }
            count++;
        }
        x = count * -9;
        for (i = 0; i < count; i++) {
            ((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW + i]->flags |= PASSCODE_SPRITE_VISIBLE;
            GfxSpriteCenterPivot(((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW + i]);
            ((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW + i]->alpha = 1.0f;
            ((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW + i]->posX =
                ((GfxSprite **)screen->data)[PASSCODE_SPR_ANCHOR]->posX + (float)x;
            ((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW + i]->posY =
                ((GfxSprite **)screen->data)[PASSCODE_SPR_ANCHOR]->posY;
            UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW + i], 1.0f, 1.0f,
                                     0.0f);
            GfxSpriteSetCell(((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW + i],
                             (float)(pc->answer[i] / 5), (float)(pc->answer[i] % 5));
            x += 18;
        }
        pc->playTimer = 30;
        pc->playStep++;
        break;
    case 7:
        for (i = 0; i < 6; i++) {
            ((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW + i]->flags &= ~PASSCODE_SPRITE_VISIBLE;
        }
        pc->playStep++;
        break;
    case 8:
        ((GfxSprite **)screen->data)[PASSCODE_SPR_PROMPT]->flags |= PASSCODE_SPRITE_VISIBLE;
        GfxSpriteCenterPivot(((GfxSprite **)screen->data)[PASSCODE_SPR_PROMPT]);
        ((GfxSprite **)screen->data)[PASSCODE_SPR_PROMPT]->alpha = 1.0f;
        ((GfxSprite **)screen->data)[PASSCODE_SPR_PROMPT]->posX =
            ((GfxSprite **)screen->data)[PASSCODE_SPR_ANCHOR]->posX;
        ((GfxSprite **)screen->data)[PASSCODE_SPR_PROMPT]->posY =
            ((GfxSprite **)screen->data)[PASSCODE_SPR_ANCHOR]->posY + 1.0f;
        UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[PASSCODE_SPR_PROMPT], 1.0f, 1.0f, 0.0f);
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x2c00021, 0, 0);
        }
        pc->playTimer = 0;
        pc->playStep++;
        break;
    case 9:
        pc->playTimer++;
        sprite = ((GfxSprite **)screen->data)[PASSCODE_SPR_PROMPT];
        if (pc->playTimer == 64) {
            sprite->flags &= ~PASSCODE_SPRITE_VISIBLE;
            pc->playStep = 10;
        } else if ((pc->playTimer & 4) != 0) {
            sprite->flags &= ~PASSCODE_SPRITE_VISIBLE;
        } else {
            sprite->flags |= PASSCODE_SPRITE_VISIBLE;
        }
        break;
    default:
        return 1;
    }
    return 0;
}
