// bdc 0x08882d0c BtlAttackType3AUpdate
#include "bdc.h"

/* Per-frame handler of attack type 0x3a (entry 58 of the handler table run by
   `BtlAttackUpdate`). Once its age exceeds 60 frames it ends (`BtlAttackEnd`).
   Frame 0 (launch): clears the turn rate, fills the function-static variant table
   `g_btlAttackType3AOffsets` on first use, sets its matrix to the owner model's root matrix
   times the followed bone's local matrix, places itself at the variant's offset transformed by
   that matrix (w = 1), adds 0.2 x the variant's direction nudge to `dir` and renormalises it
   (each lane clamped to [-1, 1]; zero length gives a zero vector; w becomes 0), turns `dir`
   about y by -15 degrees (variants 1 and 3) or +15 degrees (variants 2 and 4), and copies `dir`
   to its attached effect's direction. Later frames: unless `BtlAttackResolveClash` (impact
   0xc5) consumed it, homes at speed 70 with a 60-unit height offset (`BtlAttackSteerToTarget`),
   sweeps for hits along its velocity (`BtlAttackSweepHit`, hit kind 0x5f for variant 5, 0x60
   for variant 6, else 0x5d) and either queues impact 0xc5 at the hit point
   (`BtlAttackSetPendingHit`) or advances its position by its velocity (w kept). The variant
   is `(s32)paramF2`. */

void BtlAttackType3AUpdate(BtlAttack *self)
{
    BtlAttackType3AOffset *variant;
    const float *a;
    const float *b;
    float scaled[3];
    float len2;
    float k;
    float angle;
    float c;
    float s;
    float x;
    float y;
    float z;
    s32 hitKind;
    s32 kind;
    u32 turn;
    int i;
    int j;

    if (self->age > 60) {
        BtlAttackEnd(self);
        return;
    }
    if (self->age != 0) {
        if (BtlAttackResolveClash(self, 0xc5) != 0) {
            return;
        }
        BtlAttackSteerToTarget(70.0f, 60.0f, self, 1, NULL);
        kind = (s32)self->paramF2;
        if (kind == 5) {
            hitKind = 0x5f;
        } else if (kind == 6) {
            hitKind = 0x60;
        } else {
            hitKind = 0x5d;
        }
        if (BtlAttackSweepHit(self->radius, self, self->pos, self->vel, hitKind, 3, 0, 0x31bf337e)
            != 0) {
            BtlAttackSetPendingHit(self, 0xc5, &g_btlAttackHitQuery.contact.x);
            return;
        }
        self->pos[0] = VfAdd(self->pos[0], self->vel[0]);
        self->pos[1] = VfAdd(self->pos[1], self->vel[1]);
        self->pos[2] = VfAdd(self->pos[2], self->vel[2]);
        return;
    }

    self->turnRate = 0.0f;
    if (g_btlAttackType3AOffsetsInit == 0) {
        g_btlAttackType3AOffsetsInit = 1;
        g_btlAttackType3AOffsets[0] = (BtlAttackType3AOffset){
            { 20.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };
        g_btlAttackType3AOffsets[1] = (BtlAttackType3AOffset){
            { 20.0f, 30.0f, 20.0f, 0.0f }, { 0.0f, 1.5f, 0.0f, 0.0f } };
        g_btlAttackType3AOffsets[2] = (BtlAttackType3AOffset){
            { 20.0f, -30.0f, 20.0f, 0.0f }, { 0.0f, 1.5f, 0.0f, 0.0f } };
        g_btlAttackType3AOffsets[3] = (BtlAttackType3AOffset){
            { 20.0f, 30.0f, -10.0f, 0.0f }, { 0.0f, 0.100000001f, 0.0f, 0.0f } };
        g_btlAttackType3AOffsets[4] = (BtlAttackType3AOffset){
            { 20.0f, -30.0f, -10.0f, 0.0f }, { 0.0f, 0.100000001f, 0.0f, 0.0f } };
        g_btlAttackType3AOffsets[5] = (BtlAttackType3AOffset){
            { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, -0.400000006f, 0.0f, 0.0f } };
        g_btlAttackType3AOffsets[6] = (BtlAttackType3AOffset){
            { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, -2.5f, 0.0f, 0.0f } };
    }
    variant = &g_btlAttackType3AOffsets[(s32)self->paramF2];

    /* mtx = owner root matrix x bone local matrix */
    a = self->owner->base.data->rootMatrix;
    b = self->bone->localMatrix;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            self->mtx[j][i] = VfDot4(b[j * 4 + 0], b[j * 4 + 1], b[j * 4 + 2], b[j * 4 + 3], a[i], a[4 + i], a[8 + i], a[12 + i]);
        }
    }
    /* pos = mtx x (variant->pos.xyz, 1) */
    x = variant->pos[0];
    y = variant->pos[1];
    z = variant->pos[2];
    for (i = 0; i < 4; i++) {
        self->pos[i] = VfDot4(self->mtx[0][i], self->mtx[1][i], self->mtx[2][i], self->mtx[3][i], x, y, z, 1.0f);
    }

    /* dir.xyz += 0.2 * variant->dirDelta.xyz */
    scaled[0] = VfMul(variant->dirDelta[0], 0.200000003f);
    scaled[1] = VfMul(variant->dirDelta[1], 0.200000003f);
    scaled[2] = VfMul(variant->dirDelta[2], 0.200000003f);
    self->dir[0] = VfAdd(self->dir[0], scaled[0]);
    self->dir[1] = VfAdd(self->dir[1], scaled[1]);
    self->dir[2] = VfAdd(self->dir[2], scaled[2]);

    /* dir = (clamp(normalise(dir.xyz), -1, 1), 0); zero length scales by 0 */
    x = self->dir[0];
    y = self->dir[1];
    z = self->dir[2];
    len2 = VfDot3(x, y, z, x, y, z);
    k = VfRsq(len2);
    if (VfIsZero(len2)) {
        k = 0.0f;
    }
    self->dir[0] = VfSat1(VfMul(x, k));
    self->dir[1] = VfSat1(VfMul(y, k));
    self->dir[2] = VfSat1(VfMul(z, k));
    self->dir[3] = 0.0f;

    turn = (u32)((s32)self->paramF2 & 0xff);
    if (turn == 1 || turn == 3 || turn == 2 || turn == 4) {
        angle = (turn == 1 || turn == 3) ? -0.261799395f : 0.261799395f;
        /* dir x/z rotated by angle about y: x' = c*x - s*z, z' = s*x + c*z (y, w kept) */
        c = VfCosQuarter(VfMul(angle, 0.636619747f));
        s = VfSinQuarter(VfMul(angle, 0.636619747f));
        x = self->dir[0];
        y = self->dir[1];
        z = self->dir[2];
        self->dir[0] = VfDot3(x, y, z, c, 0.0f, -s);
        self->dir[2] = VfDot3(x, y, z, s, 0.0f, c);
    }

    /* attached effect direction = dir */
    for (i = 0; i < 4; i++) {
        ((GfxEffect *)self->effect)->dir[i] = self->dir[i];
    }
}
