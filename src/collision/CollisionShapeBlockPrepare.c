// bdc 0x089e585c CollisionShapeBlockPrepare
#include "bdc.h"

/* Rigid inverse of `src` into `inv` (the VFPU sequence lv.q x/y/z, vzero.t C130, vtfm3.t,
   vneg.t): the 3x3 part is transposed with a zero `w` lane, the translation becomes
   -(dot(x, t), dot(y, t), dot(z, t)) and `w.w` is copied from `src`. */
static void CollisionPartRigidInverse(ScePspFMatrix4 *inv, const ScePspFMatrix4 *src)
{
    ScePspFVector4 x = src->x;
    ScePspFVector4 y = src->y;
    ScePspFVector4 z = src->z;
    ScePspFVector4 t = src->w;
    float dx = x.x * t.x + x.y * t.y + x.z * t.z;
    float dy = y.x * t.x + y.y * t.y + y.z * t.z;
    float dz = z.x * t.x + z.y * t.y + z.z * t.z;

    inv->x.x = x.x;
    inv->x.y = y.x;
    inv->x.z = z.x;
    inv->x.w = 0.0f;
    inv->y.x = x.y;
    inv->y.y = y.y;
    inv->y.z = z.y;
    inv->y.w = 0.0f;
    inv->z.x = x.z;
    inv->z.y = y.z;
    inv->z.z = z.z;
    inv->z.w = 0.0f;
    inv->w.x = -dx;
    inv->w.y = -dy;
    inv->w.z = -dz;
    inv->w.w = t.w;
}

/* Brings a collider shape block up to date before a test and returns the shape object to test
   against. Mesh blocks (`shapeType` 8): when `dirty`, clears it and refreshes every part. With a
   block `matrix`, a part's `worldMatrix` becomes matrix x `localMatrix` (VFPU vmmul), `rotation`
   points at it and `invWorldMatrix` is its rigid inverse; without one, an identity `localMatrix`
   gives `rotation` = NULL, otherwise `rotation` = `localMatrix` and `invWorldMatrix` is its rigid
   inverse. Mesh blocks always return NULL. Other shapes: without a `matrix`, clears `dirty` and
   returns `desc` after calling its virtual refresh (vtable entry 9); with a `matrix`, returns the
   world-space copy `shape`, recomputed from `desc` through its virtual transform (vtable entry 6,
   args matrix and `shape`) only when `dirty`. */

void *CollisionShapeBlockPrepare(void *block)

{
    CollisionShapeBlock *self = (CollisionShapeBlock *)block;
    CollisionFacePart *part;
    const ScePspFMatrix4 *local;
    const u32 *m;
    const VtblEntry *entry;
    void *shape;
    s32 i;
    s32 j;
    s32 k;
    s32 identity;
    float prod[4][4];

    shape = (void *)0;
    if (self->shapeType == 8) {
        if (self->dirty == 0) {
            return shape;
        }
        self->dirty = 0;
        if (self->matrix != (float (*)[4])0) {
            for (i = 0; i < self->partCount; i++) {
                part = (CollisionFacePart *)self->parts[i];
                part->rotation = (const float *)&part->worldMatrix;
                local = part->localMatrix;
                /* vmmul.q M000, M100, M200 (M100 = matrix, M200 = localMatrix): column j =
                   sum over k of localMatrix[j][k] * matrix[k]. Both inputs are read first. */
                for (j = 0; j < 4; j++) {
                    const float *b = &(&local->x)[j].x;
                    for (k = 0; k < 4; k++) {
                        prod[j][k] = b[0] * self->matrix[0][k] + b[1] * self->matrix[1][k] +
                                     b[2] * self->matrix[2][k] + b[3] * self->matrix[3][k];
                    }
                }
                part->worldMatrix.x.x = prod[0][0];
                part->worldMatrix.x.y = prod[0][1];
                part->worldMatrix.x.z = prod[0][2];
                part->worldMatrix.x.w = prod[0][3];
                part->worldMatrix.y.x = prod[1][0];
                part->worldMatrix.y.y = prod[1][1];
                part->worldMatrix.y.z = prod[1][2];
                part->worldMatrix.y.w = prod[1][3];
                part->worldMatrix.z.x = prod[2][0];
                part->worldMatrix.z.y = prod[2][1];
                part->worldMatrix.z.z = prod[2][2];
                part->worldMatrix.z.w = prod[2][3];
                part->worldMatrix.w.x = prod[3][0];
                part->worldMatrix.w.y = prod[3][1];
                part->worldMatrix.w.z = prod[3][2];
                part->worldMatrix.w.w = prod[3][3];
                CollisionPartRigidInverse(&part->invWorldMatrix, &part->worldMatrix);
            }
        }
        else {
            for (i = 0; i < self->partCount; i++) {
                /* Raw word compare: identity means exact 1.0f diagonal and all-zero bits
                   elsewhere (-0.0f does not count). */
                m = (const u32 *)((CollisionFacePart *)self->parts[i])->localMatrix;
                if ((m[0] == 0x3f800000) && (m[5] == 0x3f800000) && (m[10] == 0x3f800000) &&
                    (m[15] == 0x3f800000)) {
                    identity = ((m[6] | m[8] | m[7] | m[9] | m[4] | m[1] | m[13] | m[3] | m[2] |
                                 m[12] | m[11] | m[14]) == 0);
                }
                else {
                    identity = 0;
                }
                part = (CollisionFacePart *)self->parts[i];
                if (identity) {
                    part->rotation = (const float *)0;
                }
                else {
                    part->rotation = (const float *)part->localMatrix;
                    CollisionPartRigidInverse(&part->invWorldMatrix, part->localMatrix);
                }
            }
        }
    }
    else if (self->matrix == (float (*)[4])0) {
        shape = self->desc;
        if (self->dirty != 0) {
            self->dirty = 0;
        }
        entry = &((CollisionShapeBlock *)shape)->vtbl[9];
        ((void (*)(void *))entry->fn)((char *)shape + entry->delta);
    }
    else {
        shape = self->shape;
        if (self->dirty != 0) {
            self->dirty = 0;
            entry = &((CollisionShapeBlock *)self->desc)->vtbl[6];
            ((void (*)(void *, float (*)[4], void *))entry->fn)(
                (char *)self->desc + entry->delta, self->matrix, shape);
        }
    }
    return shape;
}
