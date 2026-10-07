// bdc 0x08832764 BtlHudUpdateGateCardCutIn
#include "bdc.h"

/* HUD widget: runs the gate-card cut-in state machine `gateCutInState` for `unit` (nothing when
   the state is 0 or `unit` is NULL). Skipped (state forced to 100, both sprites hidden) when
   profile flag 0 is set and the profile has any of the flags 0x4880. States: 1 waits for the
   ability cut-in (`cutInState == 0`); 2 stops the BGM (`SndBgmCancelChannel`, 0.5 s
   `SndBgmQueueStop`), plays the jingle `0x200020` and the unit's voice, tints the glow sprite
   (`sprites[31]`) with the colour of the unit's attribute (`g_btlGateCardColors`), pauses the
   battle main task (id 100) and shows the texture `"gate_card%02d"` (attribute) on the card
   sprite (`sprites[32]`); 3 slides both sprites along `g_btlGateCutInKeyFrames` /
   `g_btlGateCutInKeyPos` with a cosine ease; 4 starts the arena flash
   (`BtlStageStartMapFlash`); 5 fades the card out while zooming it and the glow until the
   card alpha is below 0.01; 6 hides both sprites. States 10-13 then run in the same frame: 10
   zeroes the countdown `gateCutInFrame`, 11 updates the stage effects and decrements it to -1
   (not > 0, so it never waits), 13 restarts BGM track 10 on player 0 (`SndBgmPlayerPlayTrack`).
   14 updates the stage effects; 15 updates them and then, like 16, falls into state 100, which
   resumes the main task and resets the state to 0. States 7-9 and 17-99 do nothing.
   The initial key/start positions are zeroed (VFPU bank C720 = 0) before the first key position
   is set; the ease factor is clamped to [0, 1]. */
void BtlHudUpdateGateCardCutIn(BtlHud *self, BtlBakugan *unit)
{
    s32 colors[6][4];
    s32 remap[7];
    char name[36];
    float pos[4];
    GfxSprite *card;
    GfxSprite *glow;
    const VtblEntry *vtbl;
    CoreTask *task;
    bool skip;
    bool slideDone;
    s32 state;
    s32 slot;
    s32 frame;
    s32 key;
    s32 span;
    float t;
    float k;
    s32 i;
    float fade;
    float zoom;
    float sinFade;
    float sinZoom;
    float scale;
    float grow;

    if (self->gateCutInState == 0) {
        return;
    }
    if (unit == NULL) {
        return;
    }
    skip = false;
    card = self->sprites[32];
    glow = self->sprites[31];
    if (SaveGetProfileFlag0() != 0 && SaveHasProfile() &&
        SaveProfileHasFlags(SaveGetProfile(), 0x4880)) {
        skip = true;
    }
    if (skip && self->gateCutInState > 0) {
        card->flags &= ~1u;
        glow->flags &= ~1u;
        self->gateCutInState = 100;
    }
    state = self->gateCutInState;
    if (state > 16) {
        if (state != 100) {
            return;
        }
    } else {
        if (state <= 0) {
            return;
        }
        switch (state) {
        case 1:
            if (self->cutInState != 0) {
                return;
            }
            self->gateCutInState++;
            /* fallthrough */
        case 2:
            SndBgmCancelChannel(0);
            SndBgmQueueStop(0.5f, 0);
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x200020, 0, 0);
            }
            self->gateCutInFrame = 0;
            self->gateCutInKeyFrame = 0;
            self->gateCutInPrevKeyFrame = 0;
            for (i = 0; i < 4; i++) {
                self->gateCutInKeyPos[i] = 0.0f;
            }
            for (i = 0; i < 4; i++) {
                self->gateCutInFromPos[i] = 0.0f;
            }
            self->gateCutInKeyPos[0] = (float)g_btlGateCutInKeyPos[0][0];
            self->gateCutInKeyPos[1] = (float)g_btlGateCutInKeyPos[0][1];
            self->gateCutInKeyFrame = g_btlGateCutInKeyFrames[0];
            self->gateCutInKey = 0;
            self->gateCutInGrow = 0.0f;
            memcpy(colors, g_btlGateCardColors, sizeof(colors));
            remap[0] = 0;
            remap[1] = 4;
            remap[2] = 1;
            remap[3] = 2;
            remap[4] = 3;
            remap[5] = 5;
            remap[6] = 0;
            vtbl = (const VtblEntry *)unit->base.base.vtable;
            slot = remap[((s32 (*)(void *))vtbl[20].fn)((u8 *)unit + vtbl[20].delta)];
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), ((u32)unit->voiceBank << 20) + 1, 0, 0);
            }
            card->alpha = 1.0f;
            card->flags |= 1;
            /* one quad store: tint[0..2] and alpha 1 (overwritten below) */
            glow->tint[0] = (float)colors[slot][0] * 0.00390625f;
            glow->tint[1] = (float)colors[slot][1] * 0.00390625f;
            glow->tint[2] = (float)colors[slot][2] * 0.00390625f;
            glow->alpha = 1.0f;
            glow->alpha = 0.5f;
            glow->flags |= 1;
            GfxSpriteSetScaleRotation(glow, 1.0f, 1.0f, 0.0f, false);
            task = CoreTaskFind(100);
            if (task != NULL) {
                CoreTaskSetFlags(task, 1);
            }
            vtbl = (const VtblEntry *)unit->base.base.vtable;
            sprintf(name, "gate_card%02d",
                    ((s32 (*)(void *))vtbl[20].fn)((u8 *)unit + vtbl[20].delta));
            card->texture = GfxFindTexture(name);
            BtlHudSetSpriteRect(0.0f, 0.0f, 128.0f, 192.0f, self, card);
            GfxSpriteCenterPivot(card);
            GfxSpriteSetScaleRotation(card, 1.0f, 1.0f, 0.0f, false);
            self->gateCutInState++;
            /* fallthrough */
        case 3:
            frame = self->gateCutInFrame + 1;
            self->gateCutInFrame = frame;
            slideDone = false;
            if (frame == g_btlGateCutInKeyFrames[self->gateCutInKey]) {
                key = self->gateCutInKey + 1;
                self->gateCutInKey = key;
                if (g_btlGateCutInKeyFrames[key] == -1) {
                    slideDone = true;
                } else {
                    self->gateCutInFromPos[0] = self->gateCutInKeyPos[0];
                    self->gateCutInFromPos[1] = self->gateCutInKeyPos[1];
                    self->gateCutInFromPos[2] = self->gateCutInKeyPos[2];
                    self->gateCutInFromPos[3] = self->gateCutInKeyPos[3];
                    key = self->gateCutInKey;
                    self->gateCutInKeyPos[0] = (float)g_btlGateCutInKeyPos[key][0];
                    self->gateCutInKeyPos[1] = (float)g_btlGateCutInKeyPos[key][1];
                    self->gateCutInPrevKeyFrame = self->gateCutInKeyFrame;
                    self->gateCutInKeyFrame = g_btlGateCutInKeyFrames[key];
                }
            }
            span = self->gateCutInKeyFrame - self->gateCutInPrevKeyFrame;
            if (span == 0) {
                t = 1.0f;
            } else if (self->gateCutInKeyFrame == 0) {
                t = 0.0f;
            } else {
                t = (float)(span - (self->gateCutInKeyFrame - self->gateCutInFrame) + 1) /
                    (float)span;
            }
            /* vmin with 1 then vmax with 0 (t is finite; a -0 tie gives +0) */
            t = t < 1.0f ? t : 1.0f;
            t = t > 0.0f ? t : 0.0f;
            if (t == 1.0f) {
                for (i = 0; i < 4; i++) {
                    pos[i] = self->gateCutInKeyPos[i];
                }
            } else {
                for (i = 0; i < 4; i++) {
                    pos[i] = self->gateCutInFromPos[i];
                }
                t = t - 1.0f;
                t = (1.0f - t * t) * 3.14159274f;
                k = (1.0f - __builtin_cosf(t)) * 0.5f;
                for (i = 0; i < 4; i++) {
                    pos[i] = pos[i] + (self->gateCutInKeyPos[i] - pos[i]) * k;
                }
            }
            card->posX = pos[0];
            card->posY = pos[1];
            glow->posX = pos[0];
            glow->posY = pos[1];
            if (!slideDone) {
                return;
            }
            self->gateCutInState++;
            /* fallthrough */
        case 4:
            vtbl = (const VtblEntry *)unit->base.base.vtable;
            BtlStageStartMapFlash(((s32 (*)(void *))vtbl[20].fn)((u8 *)unit + vtbl[20].delta));
            self->gateCutInPhaseA = 1.57079637f;
            self->gateCutInPhaseB = 0.0f;
            self->gateCutInState++;
            /* fallthrough */
        case 5:
            BtlUpdateStageEffects();
            fade = self->gateCutInPhaseA + -0.118682392f;
            self->gateCutInPhaseA = fade;
            self->gateCutInPhaseB = self->gateCutInPhaseB + 0.0610865243f;
            if (fade < 0.0f) {
                fade = 0.0f;
            } else if (!(fade <= 1.57079637f)) {
                fade = 1.57079637f;
            }
            self->gateCutInPhaseA = fade;
            zoom = self->gateCutInPhaseB;
            if (zoom < 0.0f) {
                zoom = 0.0f;
            } else if (!(zoom <= 1.57079637f)) {
                zoom = 1.57079637f;
            }
            self->gateCutInPhaseB = zoom;
            sinFade = __builtin_sinf(self->gateCutInPhaseA);
            sinZoom = __builtin_sinf(zoom);
            card->alpha = sinFade;
            scale = sinZoom + 1.0f;
            GfxSpriteSetScaleRotation(card, scale, scale, 0.0f, false);
            sinFade = __builtin_sinf(self->gateCutInPhaseA);
            glow->alpha = sinFade * 0.300000012f;
            grow = self->gateCutInGrow + 0.0299999993f;
            self->gateCutInGrow = grow;
            grow = scale * 1.20000005f + grow;
            GfxSpriteSetScaleRotation(glow, grow, grow, 0.0f, false);
            if (!(card->alpha < 0.00999999978f)) {
                return;
            }
            self->gateCutInState++;
            /* fallthrough */
        case 6:
            card->flags &= ~1u;
            glow->flags &= ~1u;
            self->gateCutInState = 10;
            /* fallthrough */
        case 10:
            self->gateCutInFrame = 0;
            self->gateCutInState++;
            /* fallthrough */
        case 11:
            BtlUpdateStageEffects();
            self->gateCutInFrame--;
            if (self->gateCutInFrame > 0) {
                return;
            }
            self->gateCutInState++;
            /* fallthrough */
        case 12:
            self->gateCutInState++;
            /* fallthrough */
        case 13:
            SndBgmCancelChannel(0);
            if (SndBgmPlayerExists(0)) {
                SndBgmPlayerPlayTrack(SndBgmPlayerGet(0), 10, 1, 0);
            }
            self->gateCutInState++;
            return;
        case 14:
            BtlUpdateStageEffects();
            self->gateCutInState++;
            return;
        case 15:
            BtlUpdateStageEffects();
            self->gateCutInState++;
            /* fallthrough */
        case 16:
            self->gateCutInState = 100;
            break;
        default:
            return;
        }
    }
    task = CoreTaskFind(100);
    if (task != NULL) {
        CoreTaskClearFlags(task, 1);
    }
    self->gateCutInState = 0;
}
