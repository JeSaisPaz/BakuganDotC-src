// bdc 0x08830a38 BtlHudAnimateEnemyArrow
#include "bdc.h"

/* Per-frame animation of arrow `dir` of the off-screen enemy arrows of the battle
   HUD (`BtlHudUpdate`) (four arrows, direction d = 0 front, 1 back, 2 left, 3 right; sprite `sprites[15 + 3d]`):
   steps `arrowAngle[d]` by 9 degrees towards shown (`arrowShown[d]`) or hidden, clamped to
   [-72, 90] degrees, and places the sprite at `arrowHiddenPos[d] + (arrowShownPos[d] -
   arrowHiddenPos[d]) * sin(angle)`. In warning style (`arrowWarn[d]`) the angle is forced to 90
   degrees, the scale pulses (`0.7 + 0.5 sin` / `0.4 + 0.2 sin` of `arrowPulse[d]`, the larger
   one on Y for d 0/1 and on X for d 2/3; 1 for any other d) and the warning sound 0x20012d is
   started (if not already playing) every 29 frames; otherwise the pulse resets and the scale is
   1. The alpha is always `0.8 + 0.3 sin(arrowFade[d])`.
   The VFPU `vsin.s` of `x * S703` (2/pi) is sin(x) in radians. */

#define HALF_PI     1.57079637f   /* 0x3fc90fdb */
#define PI          3.14159274f   /* 0x40490fdb */
#define ANGLE_STEP  0.157079637f  /* 0x3e20d97c, 9 degrees */
#define ANGLE_MIN   -1.2566371f   /* 0xbfa0d97c, -72 degrees */
#define PULSE_STEP  0.17453292f   /* 0x3e32b8c2, 10 degrees */
#define FADE_STEP   0.1134464f    /* 0x3de85696, 6.5 degrees */
#define WARN_SOUND  0x20012d

void BtlHudAnimateEnemyArrow(BtlHud *self, s32 dir)
{
    float pos[3][4];
    const float *hidden;
    const float *shown;
    float t;
    s32 k;
    GfxSprite *sprite;
    float scaleX;
    float scaleY;
    float angle;
    float wide;
    float narrow;
    s32 timer;
    s32 i;

    scaleX = 1.0f;
    scaleY = 1.0f;

    if (self->arrowShown[dir] != 0) {
        angle = self->arrowAngle[dir] + ANGLE_STEP;
    } else {
        angle = self->arrowAngle[dir] - ANGLE_STEP;
    }
    self->arrowAngle[dir] = angle;
    if (angle < ANGLE_MIN) {
        angle = ANGLE_MIN;
    } else if (!(angle <= HALF_PI)) {
        angle = HALF_PI;
    }
    self->arrowAngle[dir] = angle;

    if (self->arrowWarn[dir] != 0) {
        self->arrowAngle[dir] = HALF_PI;
        timer = self->arrowSoundTimer[dir];
        self->arrowSoundTimer[dir] = timer - 1;
        if (timer <= 0) {
            if (SndManagerIsSoundWordPlaying(SndGetManager(), WARN_SOUND) == 0 && SndHasManager()) {
                SndManagerPlay(SndGetManager(), WARN_SOUND, 0, 0);
            }
            self->arrowSoundTimer[dir] = 28;
        }

        self->arrowPulse[dir] = self->arrowPulse[dir] + PULSE_STEP;
        self->arrowFade[dir] = self->arrowFade[dir] + FADE_STEP;
        if (!(self->arrowPulse[dir] <= PI)) {
            self->arrowPulse[dir] = self->arrowPulse[dir] - PI;
        }
        if (!(self->arrowFade[dir] <= PI)) {
            self->arrowFade[dir] = self->arrowFade[dir] - PI;
        }

        wide = __builtin_sinf(self->arrowPulse[dir]) * 0.5f + 0.7f;
        narrow = __builtin_sinf(self->arrowPulse[dir]) * 0.2f + 0.4f;
        if (dir < 2) {
            if (dir >= 0) {
                scaleX = narrow;
                scaleY = wide;
            }
        } else if (dir < 4) {
            scaleX = wide;
            scaleY = narrow;
        }
    } else {
        self->arrowPulse[dir] = 1.0f;
        self->arrowFade[dir] = 0.0f;
        self->arrowSoundTimer[dir] = 0;
    }

    /* The binary's loop over the arrow's parts is bounded to the first one. */
    for (i = 0; i < 1; i++) {
        hidden = self->arrowHiddenPos[dir] + 4 * i;
        shown = self->arrowShownPos[dir] + 4 * i;
        for (k = 0; k < 4; k++) {
            pos[i][k] = hidden[k];
        }
        t = __builtin_sinf(self->arrowAngle[dir]);
        for (k = 0; k < 4; k++) {
            pos[i][k] = pos[i][k] + (shown[k] - pos[i][k]) * t;
        }
        sprite = self->sprites[15 + 3 * dir + i];
        sprite->posX = pos[i][0];
        sprite->posY = pos[i][1];
        sprite->posZ = pos[i][2];
        sprite->posW = pos[i][3];
        sprite->alpha = __builtin_sinf(self->arrowFade[dir]) * 0.3f + 0.8f;
        if (i != 2) {
            GfxSpriteSetScaleRotation(sprite, scaleX, scaleY, 0.0f, false);
        }
    }
}
