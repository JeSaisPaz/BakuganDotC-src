// bdc 0x088638cc BtlBakuganUpdateBoneAnchors
#include "bdc.h"

/* Per-frame virtual (slot `+0xb8`, called by `BtlBakuganUpdate`): sets the anchor matrix to the
   model root matrix times the `"Bip01_Pelvis"` node matrix (`GfxModelFindNodeMatrix`). For kinds
   10, 12 and 8 it then moves the anchor position (matrix row 3, x/y/z) by a fixed offset
   transformed by the anchor matrix: (0, 35, 0) for kind 10, (-35, 0, 0) for kind 12 and
   (-25, 0, 0) for kind 8, each a lazily initialised static. Marks the body collider's attach
   matrix dirty, moves the push collider's sphere to the model root position raised by its radius
   (and runs the sphere's update virtual), and mirrors state flag 0x200000 into body-collider flag
   bit 3. */

void BtlBakuganUpdateBoneAnchors(BtlBakugan *self)
{
    const float *root = self->base.data->rootMatrix;
    const float *pelvis;
    CollisionCollider *body;
    CollisionCollider *push;
    CollisionSphereQuery *sphere;
    const VtblEntry *update;
    const ScePspFVector4 *offset = NULL;
    float moved[4];
    s32 j;
    s32 r;

    pelvis = &GfxModelFindNodeMatrix(&self->base, "Bip01_Pelvis")->x.x;
    /* vmmul.q M000, M100(root), M200(pelvis): row j of the anchor = sum over k of
       pelvis[j][k] * root row k (column-major root * pelvis) */
    for (j = 0; j < 4; j++) {
        for (r = 0; r < 4; r++) {
            self->anchorMatrix[j][r] = pelvis[j * 4 + 0] * root[0 * 4 + r] +
                                       pelvis[j * 4 + 1] * root[1 * 4 + r] +
                                       pelvis[j * 4 + 2] * root[2 * 4 + r] +
                                       pelvis[j * 4 + 3] * root[3 * 4 + r];
        }
    }
    if (self->base.base.unk08 == 10) {
        if (g_btlAnchorOffsetKind10Ready == 0) {
            g_btlAnchorOffsetKind10Ready = 1;
            g_btlAnchorOffsetKind10.x = 0.0f;
            g_btlAnchorOffsetKind10.y = 35.0f;
            g_btlAnchorOffsetKind10.z = 0.0f;
            g_btlAnchorOffsetKind10.w = 0.0f;
        }
        offset = &g_btlAnchorOffsetKind10;
    } else if (self->base.base.unk08 == 12) {
        if (g_btlAnchorOffsetKind12Ready == 0) {
            g_btlAnchorOffsetKind12Ready = 1;
            g_btlAnchorOffsetKind12.x = -35.0f;
            g_btlAnchorOffsetKind12.y = 0.0f;
            g_btlAnchorOffsetKind12.z = 0.0f;
            g_btlAnchorOffsetKind12.w = 0.0f;
        }
        offset = &g_btlAnchorOffsetKind12;
    } else if (self->base.base.unk08 == 8) {
        if (g_btlAnchorOffsetKind8Ready == 0) {
            g_btlAnchorOffsetKind8Ready = 1;
            g_btlAnchorOffsetKind8.x = -25.0f;
            g_btlAnchorOffsetKind8.y = 0.0f;
            g_btlAnchorOffsetKind8.z = 0.0f;
            g_btlAnchorOffsetKind8.w = 0.0f;
        }
        offset = &g_btlAnchorOffsetKind8;
    }
    if (offset != NULL) {
        /* vtfm4.q C000, E100, C200: moved = anchor matrix (column-major) x offset */
        for (r = 0; r < 4; r++) {
            moved[r] = self->anchorMatrix[0][r] * offset->x + self->anchorMatrix[1][r] * offset->y +
                       self->anchorMatrix[2][r] * offset->z + self->anchorMatrix[3][r] * offset->w;
        }
        /* vadd.t: anchor position xyz += moved xyz; its w is stored back unchanged */
        self->anchorMatrix[3][0] = self->anchorMatrix[3][0] + moved[0];
        self->anchorMatrix[3][1] = self->anchorMatrix[3][1] + moved[1];
        self->anchorMatrix[3][2] = self->anchorMatrix[3][2] + moved[2];
    }
    body = self->collider0;
    body->attachDirty = 1;
    push = self->collider1;
    sphere = push->shapeDesc;
    root = self->base.data->rootMatrix;
    /* sphere center = root matrix row 3 (lv.q/sv.q quad copy) */
    sphere->center.x = root[12];
    sphere->center.y = root[13];
    sphere->center.z = root[14];
    sphere->center.w = root[15];
    sphere->center.y = sphere->center.y + sphere->radius;
    update = &sphere->vtbl[9];
    ((void (*)(void *))update->fn)((u8 *)sphere + update->delta);
    body = self->collider0;
    if ((self->stateFlags & 0x200000) != 0) {
        body->flags |= 8;
    } else {
        body->flags &= ~8u;
    }
}
