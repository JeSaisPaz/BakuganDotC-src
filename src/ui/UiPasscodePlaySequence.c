// bdc 0x0893f56c UiPasscodePlaySequence
#include "bdc.h"

/* Demonstration mode 1 of the sequence-code screen (task 374, `UiPasscodeCtor`; the player
   re-enters a sequence of up to 6 symbols from a 10-symbol pad and it is compared with the answer).
   One step per call, driven by `playStep`: plays the countdown beep 0x2c0001a three times (30 frames
   apart), then 0x2c0001b; then shows each answer symbol (`answer`, count `answerLen`) in the entry
   frame sprite (data sprite 14, placed on sprite 13) for 15 frames with sound 0x2c00020; then shows
   the prompt sprite (data sprite 34, sound 0x2c00021) and blinks it every 4 frames for 64 frames.
   Returns 0 while running, 1 once `playStep` is past the last step (>= 11). */

#define PASSCODE_SPRITE_VISIBLE 1u

#define PASSCODE_SPR_ANCHOR 13
#define PASSCODE_SPR_SHOW 14
#define PASSCODE_SPR_PROMPT 34

s32 UiPasscodePlaySequence(UiScreen *screen)
{
    UiPasscode *pc = (UiPasscode *)screen;
    GfxSprite *sprite;
    u8 symbol;

    switch (pc->playStep) {
    case 0:
        ((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW]->flags &= ~PASSCODE_SPRITE_VISIBLE;
        GfxSpriteCenterPivot(((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW]);
        ((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW]->alpha = 1.0f;
        ((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW]->posX =
            ((GfxSprite **)screen->data)[PASSCODE_SPR_ANCHOR]->posX;
        ((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW]->posY =
            ((GfxSprite **)screen->data)[PASSCODE_SPR_ANCHOR]->posY;
        UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW], 1.0f, 1.0f, 0.0f);
        pc->playTimer = 30;
        pc->playCount = 0;
        pc->playStep++;
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
    case 1:
    case 4:
    case 6:
    case 8:
        if (pc->playTimer != 0) {
            pc->playTimer--;
        } else {
            pc->playStep++;
        }
        break;
    case 5:
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x2c00020, 0, 0);
        }
        symbol = pc->answer[pc->playCount];
        GfxSpriteSetCell(((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW], (float)(symbol / 5),
                         (float)(symbol % 5));
        ((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW]->flags |= PASSCODE_SPRITE_VISIBLE;
        pc->playTimer = 15;
        pc->playStep++;
        break;
    case 7:
        ((GfxSprite **)screen->data)[PASSCODE_SPR_SHOW]->flags &= ~PASSCODE_SPRITE_VISIBLE;
        pc->playCount++;
        if (pc->playCount == pc->answerLen) {
            pc->playTimer = 15;
            pc->playStep++;
        } else {
            pc->playTimer = 15;
            pc->playStep = 4;
        }
        break;
    case 9:
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
    case 10:
        pc->playTimer++;
        sprite = ((GfxSprite **)screen->data)[PASSCODE_SPR_PROMPT];
        if (pc->playTimer == 64) {
            sprite->flags &= ~PASSCODE_SPRITE_VISIBLE;
            pc->playStep = 11;
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
