// bdc 0x0893f0d8 UiPasscodeShowJudgement
#include "bdc.h"

/* Plays the result stamp of the sequence-code screen (task 374, `UiPasscodeCtor`; the player
   re-enters a sequence of up to 6 symbols from a 10-symbol pad and it is compared with the answer)
   after a full entry, one step per call driven by `playStep`. Returns 1 once `playStep` is past the
   last step (> 4), 0 while it runs. `judgement` == 0 (entry matched) plays sound 0x2c0001f, tints
   data sprite 31 and shows stamp data sprite 35; otherwise sound 0x2c0001d, tints data sprite 30
   and shows stamp data sprite 36. The stamp's pop animation keeps its time and base scale in
   `tween[35 + judgement]`. */

#define PASSCODE_SPRITE_VISIBLE 1u
#define PASSCODE_SPRITE_MATRIX 0x20u

#define PASSCODE_SPR_ANCHOR 13
#define PASSCODE_SPR_ENTRY_FIRST 14
#define PASSCODE_SPR_ENTRY_END 20
#define PASSCODE_SPR_FRAME_WRONG 30
#define PASSCODE_SPR_FRAME_RIGHT 31
#define PASSCODE_SPR_STAMP 35

s32 UiPasscodeShowJudgement(UiScreen *screen)
{
    UiPasscode *pc = (UiPasscode *)screen;
    GfxSprite **sprites;
    GfxSprite *sprite;
    UiPasscodePartTween *tween;
    s32 i;
    float t;
    float start;
    float c;

    if (pc->playStep > 4) {
        return 1;
    }
    switch (pc->playStep) {
    case 1:
        for (i = PASSCODE_SPR_ENTRY_FIRST; i < PASSCODE_SPR_ENTRY_END; i++) {
            ((GfxSprite **)screen->data)[i]->flags &= ~PASSCODE_SPRITE_VISIBLE;
        }
        if (pc->judgement == 0) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x2c0001f, 0, 0);
            }
            sprite = ((GfxSprite **)screen->data)[PASSCODE_SPR_FRAME_RIGHT];
            sprite->addColor[0] = 0.0f;
            sprite->addColor[1] = 1.0f;
            sprite->addColor[2] = 1.0f;
            sprite->addColor[3] = 1.0f;
        } else {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x2c0001d, 0, 0);
            }
            sprite = ((GfxSprite **)screen->data)[PASSCODE_SPR_FRAME_WRONG];
            sprite->addColor[0] = 1.0f;
            sprite->addColor[1] = 0.0f;
            sprite->addColor[2] = 0.0f;
            sprite->addColor[3] = 1.0f;
        }
        ((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement]->flags |= PASSCODE_SPRITE_VISIBLE;
        GfxSpriteCenterPivot(((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement]);
        ((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement]->flags |= PASSCODE_SPRITE_MATRIX;
        UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement], 1.0f,
                                 1.0f, 0.0f);
        ((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement]->alpha = 1.0f;
        ((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement]->posX =
            ((GfxSprite **)screen->data)[PASSCODE_SPR_ANCHOR]->posX;
        ((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement]->posY =
            ((GfxSprite **)screen->data)[PASSCODE_SPR_ANCHOR]->posY + 1.0f;
        tween = &pc->tween[PASSCODE_SPR_STAMP + pc->judgement];
        tween->t = 0.0f;
        tween->startScale = ((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement]->scaleX;
        pc->playStep++;
        break;
    case 2:
        tween = &pc->tween[PASSCODE_SPR_STAMP + pc->judgement];
        t = tween->t + 0.125f;
        tween->t = t;
        start = tween->startScale;
        c = __builtin_cosf(t * 3.1415927f);
        ((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement]->scaleX =
            start + (1.0f - c) * 0.5f * 0.2f;
        sprite = ((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement];
        sprite->scaleY = sprite->scaleX;
        sprites = (GfxSprite **)screen->data;
        if (!(pc->tween[PASSCODE_SPR_STAMP + pc->judgement].t < 2.0f)) {
            sprites[PASSCODE_SPR_STAMP + pc->judgement]->scaleX = 1.0f;
            ((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement]->scaleY = 1.0f;
            pc->playTimer = 30;
            pc->playStep++;
            sprites = (GfxSprite **)screen->data;
        }
        sprite = sprites[PASSCODE_SPR_STAMP + pc->judgement];
        GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
        break;
    case 3:
        if (pc->playTimer != 0) {
            pc->playTimer--;
        } else {
            pc->playStep++;
        }
        break;
    case 4:
        ((GfxSprite **)screen->data)[PASSCODE_SPR_STAMP + pc->judgement]->flags &= ~PASSCODE_SPRITE_VISIBLE;
        pc->playStep++;
        break;
    default:
        if (pc->playTimer == 15) {
            pc->playTimer = 0;
            pc->playStep++;
        } else {
            pc->playTimer++;
        }
        break;
    }
    return 0;
}
