// bdc 0x088589c0 ActorCrystalUpdate
#include "bdc.h"

/* Per-frame update of the crystal actor (vtable `0x08af1974` slot `+0x38`, the slot the Bakugan
   class fills with its unit update).
   - Broken (`fadeSkip` set): in score mode (`BtlIsScoreMode``(1)`) runs
     `ActorCrystalRegenerate`; otherwise copies `fade` into the stand's ambient alpha and makes the
     stand solid while `fade <= 0` (`ActorCrystalStandSetCollidable``(stand, 0)`), pass-through
     otherwise.
   - Dead: marks and releases its layout record (`ActorStageObjReleaseRecord`), sets `fadeSkip`
     and starts `ActorCrystalBreak`.
   - Otherwise: reads the input actions into `commands` (`BtlInputReadActions`), turns the node
     matrices `nodeMatrices` toward the active camera (yaw `-atan2f(dz, dx)` of the camera-to-crystal
     vector scaled by 10, or the camera's `yaw` when that is within 1 unit, wrapped to (-pi, pi],
     plus the model's `rot[1]`; tilted about Z by `(|angle| - pi/2, clamped at 0) * -0.0318`),
     then runs `ActorCrystalUpdateMatrices`, `ActorCrystalScrollUvs`, virtual slot 23,
     `ActorCrystalUpdateOcclusionFade` and, when `inBattle`, `ActorCrystalUpdateLogic`.
   The VFPU bank constant S703 (2/pi) scales both angles into quarter turns for `vrot`, so the
   rotations are plain cosf/sinf of the angles. */

/* Copies the first three columns of `r` into `m` (the `w` column is left as is). */
static void ActorCrystalCopyRotation(ScePspFMatrix4 *m, float r[4][4])
{
    m->x.x = r[0][0];
    m->x.y = r[0][1];
    m->x.z = r[0][2];
    m->x.w = r[0][3];
    m->y.x = r[1][0];
    m->y.y = r[1][1];
    m->y.z = r[1][2];
    m->y.w = r[1][3];
    m->z.x = r[2][0];
    m->z.y = r[2][1];
    m->z.z = r[2][2];
    m->z.w = r[2][3];
}

void ActorCrystalUpdate(ActorCrystal *self)
{
    float d[4];
    float a[4][4];
    float b[4][4];
    float r[4][4];
    float lenSq;
    float ca;
    float sa;
    float ct;
    float st;
    int i;
    int j;
    int n;
    const VtblEntry *entry;
    GfxCamera *cam;
    float heading;
    float tilt;
    float fade;

    if (self->fadeSkip != 0) {
        if (BtlIsScoreMode(1) != 0) {
            ActorCrystalRegenerate(self);
            return;
        }
        fade = self->fade;
        ((ActorCrystalStand *)self->stand)->base.ambient[3] = fade;
        if (fade <= 0.0f) {
            ActorCrystalStandSetCollidable(self->stand, 0);
        } else {
            ActorCrystalStandSetCollidable(self->stand, 1);
        }
        return;
    }
    if (self->base.combat.dead != 0) {
        if (self->record != NULL) {
            ((ActorStageObjRecord *)self->record)->doneFlags[0] = 1;
            ActorStageObjReleaseRecord(self->record);
            self->record = NULL;
        }
        self->fadeSkip = 1;
        ActorCrystalBreak(self);
        return;
    }
    self->base.commands = BtlInputReadActions(self->base.input);
    cam = g_gfxActiveCamera;
    heading = cam->yaw;
    /* Camera-to-crystal vector scaled by 10, flattened to the XZ plane. */
    d[0] = (self->base.base.pos[0] - cam->eye[0]) * 10.0f;
    d[1] = (self->base.base.pos[1] - cam->eye[1]) * 10.0f;
    d[2] = (self->base.base.pos[2] - cam->eye[2]) * 10.0f;
    d[1] = 0.0f;
    lenSq = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
    if (!(lenSq <= 1.0f)) {
        heading = -atan2f(d[2], d[0]);
    }
    if (!(heading <= 3.14159274f)) {
        heading = heading - 6.28318548f;
    } else if (heading <= -3.14159274f) {
        heading = heading + 6.28318548f;
    }
    heading = heading + self->base.base.rot[1];
    tilt = fabsf(heading) - 1.57079637f;
    if (tilt < 0.0f) {
        tilt = 0.0f;
    }
    tilt = tilt * -0.0318471342f;
    /* `a`: rotation about Y by `heading`, `b`: rotation about Z by `tilt`, one column per row
       index (vrot of the angle times 2/pi, in quarter turns). */
    ca = __builtin_cosf(heading);
    sa = __builtin_sinf(heading);
    ct = __builtin_cosf(tilt);
    st = __builtin_sinf(tilt);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            a[i][j] = (i == j) ? 1.0f : 0.0f;
            b[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    a[0][0] = ca;
    a[0][2] = -sa;
    a[2][0] = sa;
    a[2][2] = ca;
    b[0][0] = ct;
    b[0][1] = st;
    b[1][0] = -st;
    b[1][1] = ct;
    /* vmmul.q E200, E100, E000: r = b * transpose(a) (columns r[j][i]). */
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            r[j][i] = a[0][j] * b[0][i] + a[1][j] * b[1][i] + a[2][j] * b[2][i] + a[3][j] * b[3][i];
        }
    }
    for (n = 0; n < 3; n++) {
        if (self->nodeMatrices[n] != NULL) {
            ActorCrystalCopyRotation(self->nodeMatrices[n], r);
        }
    }
    ActorCrystalUpdateMatrices(self);
    ActorCrystalScrollUvs(self);
    entry = &((const VtblEntry *)self->base.base.base.vtable)[23];
    ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
    ActorCrystalUpdateOcclusionFade(self);
    if (self->inBattle != 0) {
        ActorCrystalUpdateLogic(self);
    }
}
