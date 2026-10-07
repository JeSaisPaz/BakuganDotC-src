// bdc 0x0882e4a4 BtlHudUpdateTargetMarker
#include "bdc.h"

/* HUD lock-on marker. Stores the player Bakugan's (`BtlHudGetPlayerBakugan`) target
   (`BtlBakuganGetTarget`) in `markerTarget`; returns at once when there is no player Bakugan.
   No target: plays sound 0x20012c if a marker was shown (`markerShownTarget`) and the camera task
   (`BtlGetCameraTask`) is not alive in phase 2, hides the marker sprite `sprites[0x1b]` and
   `sprites[0xbc..0xbe]` and clears `markerStyle`/`markerPrevStyle`/`markerFade`/
   `markerShownTarget`. Target: anchors at the target `pos`, raised by 150 (vtable slot 11), by half
   the stat-table `height` (slot 14 false), or by 100 (slot 19 false and slot 16 or 15 true),
   projects it with `g_gfxActiveCamera` (`GfxCameraProjectPoint`, depth vector via
   `MathVfpuStoreC100`, then `GfxCameraProjectPointFacing`), picks style 1 (target
   `stateFlags & 0x30000000`, palette row 0) or style 2 (row 0 within 800 units of the player, else
   row 1) for the three `markerBlend`s, plays 0x20012c and restarts the animation (`markerStep` 0)
   when the target changed, restarts it on a style change to 2, then runs step `markerStep`:
   0 initialises (fade pi/2, spin/pulse 0, shownTarget = target) into 1 (fade pulse until the fade
   wraps, then step 4); 2 sets fade pi/2 into 3 (one fade pulse, then step 10); 4 sets fade 0 into
   10 (steady slow pulse); 5..9 and anything else do nothing. Each animating step positions the
   sprite at the projected point, sets its alpha (sin of `markerFade`, times the target's
   `ambient[3]`) and scale (sin of `markerPulse`), and shows it (flags bit 0) only when the
   projected depth is above -3 and `BtlHudIsVisible` returns false.
   The VFPU `vsin.s` of angle * 2/pi (bank S703) is sinf(angle). */

#define HALF_PI           1.57079637f   /* 0x3fc90fdb */
#define PI                3.14159274f   /* 0x40490fdb */
#define TWO_PI            6.28318548f   /* 0x40c90fdb */
#define SPIN_STEP         0.05f         /* 0x3d4ccccd */
#define DEPTH_MIN         -3.0f         /* 0xc0400000 */
#define NEAR_DISTANCE     800.0f        /* 0x44480000 */
#define MARKER_SOUND      0x20012c
#define MARKER_SPRITE     0x1b          /* sprites[0x1b], byte offset 0x6c */
#define MARKER_SUB_SPRITE 0xbc          /* sprites[0xbc..0xbe], byte offset 0x2f0 */

/* Calls BtlBakugan vtable slot `slot` (entries {s16 this delta; fn}). */
static inline s32 BtlBakuganVcall(BtlBakugan *unit, s32 slot)
{
    const VtblEntry *vtbl = (const VtblEntry *)unit->base.base.vtable;
    return ((s32 (*)(void *))vtbl[slot].fn)((u8 *)unit + vtbl[slot].delta);
}

/* One frame of marker animation: advances markerFade (wrapped by pi), markerPulse (wrapped by
   2pi) and markerSpin (+0.05, wrapped to [-pi, pi]), then places the marker sprite at `screen`
   with alpha (sin(fade) * alphaAmp + alphaBase) * targetAlpha and scale sin(pulse) * scaleAmp + 1.
   Returns 1 when the fade wrapped. */
static inline s32 BtlHudAnimateMarker(BtlHud *self, float fadeStep, float pulseStep,
                                      float alphaAmp, float alphaBase, float scaleAmp,
                                      float targetAlpha, const float *screen, const float *depth)
{
    s32 wrapped = 0;
    float alpha;
    float scale;
    GfxSprite *sprite;
    s32 i;

    self->markerFade += fadeStep;
    if (!(self->markerFade <= PI)) {
        wrapped = 1;
        self->markerFade -= PI;
    }
    self->markerPulse += pulseStep;
    if (!(self->markerPulse <= TWO_PI)) {
        self->markerPulse -= TWO_PI;
    }
    self->markerSpin += SPIN_STEP;
    if (!(self->markerSpin <= PI)) {
        self->markerSpin -= TWO_PI;
    } else if (self->markerSpin <= -PI) {
        self->markerSpin += TWO_PI;
    }

    alpha = __builtin_sinf(self->markerFade) * alphaAmp + alphaBase;
    scale = __builtin_sinf(self->markerPulse) * scaleAmp + 1.0f;
    alpha *= targetAlpha;

    for (i = 0; i < 1; i++) {
        if (!(depth[2] <= DEPTH_MIN) && !BtlHudIsVisible()) {
            sprite = self->sprites[MARKER_SPRITE + i];
            sprite->flags |= 1;
        } else {
            sprite = self->sprites[MARKER_SPRITE + i];
            sprite->flags &= ~1u;
        }
        sprite = self->sprites[MARKER_SPRITE + i];
        sprite->posX = screen[0];
        sprite->posY = screen[1];
        sprite->posZ = screen[2];
        sprite->posW = screen[3];
        self->sprites[MARKER_SPRITE + i]->alpha = alpha;
        GfxSpriteSetScaleRotation(self->sprites[MARKER_SPRITE + i], scale, scale, 0.0f, false);
    }
    return wrapped;
}

void BtlHudUpdateTargetMarker(BtlHud *self)
{
    float screen[4];
    float pos[4];
    float depth[4];
    float dx;
    float dy;
    float dz;
    float targetAlpha;
    float distance;
    BtlBakugan *player;
    BtlBakugan *target;
    bool playSound;
    s32 i;

    player = BtlHudGetPlayerBakugan(self);
    if (player == NULL) {
        return;
    }
    target = BtlBakuganGetTarget(player);
    self->markerTarget = target;

    if (target == NULL) {
        playSound = true;
        if (BtlCameraTaskExists() && ((BtlMain *)BtlGetCameraTask())->phase == 2) {
            playSound = false;
        }
        if (playSound && self->markerShownTarget != NULL && SndHasManager()) {
            SndManagerPlay(SndGetManager(), MARKER_SOUND, 0, 0);
        }
        for (i = 0; i < 1; i++) {
            self->sprites[MARKER_SPRITE + i]->flags &= ~1u;
        }
        for (i = 0; i < 3; i++) {
            self->sprites[MARKER_SUB_SPRITE + i]->flags &= ~1u;
        }
        self->markerPrevStyle = 0;
        self->markerStyle = 0;
        self->markerFade = 0.0f;
        self->markerShownTarget = NULL;
        return;
    }

    /* Marker anchor: the target position, raised by its kind */
    targetAlpha = self->markerTarget->base.ambient[3];
    pos[0] = self->markerTarget->base.pos[0];
    pos[1] = self->markerTarget->base.pos[1];
    pos[2] = self->markerTarget->base.pos[2];
    pos[3] = self->markerTarget->base.pos[3];
    if (BtlBakuganVcall(self->markerTarget, 11)) {
        pos[1] += 150.0f;
    } else if (!BtlBakuganVcall(self->markerTarget, 14)) {
        pos[1] += self->markerTarget->combat.stats->height * 0.5f;
    } else if (!BtlBakuganVcall(self->markerTarget, 19)) {
        if (BtlBakuganVcall(self->markerTarget, 16)) {
            pos[1] += 100.0f;
        } else if (BtlBakuganVcall(self->markerTarget, 15)) {
            pos[1] += 100.0f;
        }
    }
    MathVfpuStoreC100(depth, GfxCameraProjectPoint(g_gfxActiveCamera, screen, pos));
    GfxCameraProjectPointFacing(g_gfxActiveCamera, screen, pos);

    /* Style and palette tint */
    dx = player->base.pos[0] - self->markerTarget->base.pos[0];
    dy = player->base.pos[1] - self->markerTarget->base.pos[1];
    dz = player->base.pos[2] - self->markerTarget->base.pos[2];
    distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    if ((self->markerTarget->stateFlags & 0x30000000) != 0) {
        for (i = 0; i < 3; i++) {
            GfxPaletteBlendSetRow(self->markerBlend[i], 0);
        }
        self->markerStyle = 1;
    } else if (distance <= NEAR_DISTANCE) {
        for (i = 0; i < 3; i++) {
            GfxPaletteBlendSetRow(self->markerBlend[i], 0);
        }
        self->markerStyle = 2;
    } else {
        for (i = 0; i < 3; i++) {
            GfxPaletteBlendSetRow(self->markerBlend[i], 1);
        }
        self->markerStyle = 2;
    }

    if (self->markerShownTarget != self->markerTarget) {
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), MARKER_SOUND, 0, 0);
        }
        self->markerStep = 0;
    }
    if (self->markerStyle != self->markerPrevStyle) {
        self->markerPrevStyle = self->markerStyle;
        if (self->markerStyle != 1) {
            self->markerStep = 0;
        }
    }

    switch (self->markerStep) {
    case 0:
        self->markerFade = HALF_PI;
        self->markerSpin = 0.0f;
        self->markerPulse = 0.0f;
        self->markerShownTarget = self->markerTarget;
        self->markerStep++;
        /* fall through */
    case 1:
        /* fade 4 deg, pulse 16 deg per frame; alpha 0.7 +- 0.3, scale 1 +- 0.4 */
        if (BtlHudAnimateMarker(self, 0.06981317f, 0.27925268f, 0.3f, 0.7f, 0.4f, targetAlpha,
                                screen, depth)) {
            self->markerStep = 4;
        }
        return;
    case 2:
        self->markerFade = HALF_PI;
        self->markerStep++;
        /* fall through */
    case 3:
        /* fade 4 deg, pulse 22 deg per frame; alpha 0.8 +- 0.2, scale 1 +- 0.4 */
        if (!BtlHudAnimateMarker(self, 0.06981317f, 0.38397244f, 0.2f, 0.8f, 0.4f, targetAlpha,
                                 screen, depth)) {
            return;
        }
        self->markerStep++;
        /* fall through */
    case 4:
        self->markerFade = 0.0f;
        self->markerStep = 10;
        /* fall through */
    case 10:
        /* fade 8 deg, pulse 16 deg per frame; alpha 0.8 +- 0.2, scale 1 +- 0.05 */
        BtlHudAnimateMarker(self, 0.13962634f, 0.27925268f, 0.2f, 0.8f, 0.05f, targetAlpha, screen,
                            depth);
        return;
    default:
        return;
    }
}
