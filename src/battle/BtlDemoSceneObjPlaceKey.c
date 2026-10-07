// bdc 0x08906bb0 BtlDemoSceneObjPlaceKey
#include "bdc.h"

/* Applies the placement key of the current frame to the target model of a demo scene motion event
   (`BtlDemoSceneMotionEvent`): finds the first of the 8 `track0` slots of `ev->scene` whose
   `ordinal` equals `ev->trackId` (none → return) and, when `frame < keyCount` (signed), reads record
   `placeKeys[frame]` (`BtlDemoScbPlaceKey`). If `ev->target` is NULL nothing is written. Else its
   `pos` becomes `{key.pos × 0.1, 0}` plus the per-battle stage vector of `BtlGetStageAmbientColor`
   (VFPU `vadd.t`, xyz only, w = 0). Rotation: for target kinds (`base.unk08`) 0x58..0x6b, demo 0x16
   sets `rot[0] = key.rot[0] + pi` and `rot[1] = pi/2 - key.rot[1]` wrapped into (-pi, pi], other
   demos only `rot[2] = key.rot[0]`; every other kind copies `{key.rot, 0}` into `rot` and then sets
   `rot[1] = pi/2 - key.rot[1]` (wrapped). */

#define PI_F      3.1415927f
#define HALF_PI_F 1.5707964f
#define TWO_PI_F  6.2831855f

static inline float BtlDemoSceneObjPlaceKeyWrap(float r)
{
    if (!(r <= PI_F)) {
        r -= TWO_PI_F;
    } else if (r <= -PI_F) {
        r += TWO_PI_F;
    }
    return r;
}

void BtlDemoSceneObjPlaceKey(BtlDemoSceneMotionEvent *ev, s32 frame)
{
    float pos[4] __attribute__((aligned(16)));
    float rot[4] __attribute__((aligned(16)));
    float amb[4] __attribute__((aligned(16)));
    BtlDemoScene *scene = ev->scene;
    const BtlDemoScbPlaceKey *key;
    s32 slot;
    s32 found = -1;
    u32 kind;

    for (slot = 0; slot < 8; slot++) {
        if ((s32)scene->track0[slot].ordinal == ev->trackId) {
            found = slot;
            break;
        }
    }
    if (found < 0) {
        return;
    }
    if (!(frame < (s32)scene->track0[found].keyCount)) {
        return;
    }

    key = &scene->track0[found].placeKeys[frame];
    pos[0] = key->pos[0] * 0.1f;
    pos[1] = key->pos[1] * 0.1f;
    pos[2] = key->pos[2] * 0.1f;
    pos[3] = 0.0f;
    rot[0] = key->rot[0];
    rot[1] = key->rot[1];
    rot[2] = key->rot[2];
    rot[3] = 0.0f;
    if (ev->target == NULL) {
        return;
    }

    BtlGetStageAmbientColor(amb);
    pos[0] += amb[0];
    pos[1] += amb[1];
    pos[2] += amb[2];
    ev->target->pos[0] = pos[0];
    ev->target->pos[1] = pos[1];
    ev->target->pos[2] = pos[2];
    ev->target->pos[3] = pos[3];

    kind = ev->target->base.unk08;
    if (kind >= 0x58 && kind < 0x6c) {
        if (ev->demoId == 0x16) {
            ev->target->rot[0] = rot[0] + PI_F;
            ev->target->rot[1] = BtlDemoSceneObjPlaceKeyWrap(HALF_PI_F - rot[1]);
        } else {
            ev->target->rot[2] = rot[0];
        }
        return;
    }

    ev->target->rot[0] = rot[0];
    ev->target->rot[1] = rot[1];
    ev->target->rot[2] = rot[2];
    ev->target->rot[3] = rot[3];
    ev->target->rot[1] = BtlDemoSceneObjPlaceKeyWrap(HALF_PI_F - rot[1]);
}
