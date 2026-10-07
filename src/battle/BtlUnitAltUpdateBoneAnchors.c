// bdc 0x0885bddc BtlUnitAltUpdateBoneAnchors
#include "bdc.h"

/* Override of slot `+0xb8` (`BtlBakuganUpdateBoneAnchors`) in the unit subclass with vtable
   `0x08af1c94` (non-playable kinds): anchor matrix = model root matrix x local matrix of model
   node 0x1a (kind 0x18), 5 (kind 0x1e) or 1 (other kinds) (`GfxModelGetNode`). The anchor
   position (row 3, x/y/z) then moves by an offset transformed by the anchor matrix: (-50,0,0) for
   kinds 0x16/0x1c, (0,0,70) for 0x18, 0 for 0x19, (-60,0,0) for 0x1a, (0,0,40) for 0x1b,
   (-120,0,0) for 0x20; every other kind uses the zero vector (bank constant C720).
   Then marks the body collider's attach matrix dirty, moves the push collider's sphere to the
   model root position raised by its radius (and runs the sphere's update virtual), and mirrors
   state flag 0x200000 into body-collider flag bit 3. */

void BtlUnitAltUpdateBoneAnchors(BtlUnitAlt *self)
{
    BtlBakugan *unit = &self->base.base;
    const float *root;
    const float *local;
    GmoNode *node;
    s32 kind = (s32)unit->base.base.unk08;
    CollisionCollider *body;
    CollisionSphereQuery *sphere;
    const VtblEntry *update;
    float offset[4];
    float moved[4];
    s32 j;
    s32 r;

    root = unit->base.data->rootMatrix;
    if (kind == 0x18) {
        node = GfxModelGetNode(&unit->base, 0x1a);
    } else if (kind == 0x1e) {
        node = GfxModelGetNode(&unit->base, 5);
    } else {
        node = GfxModelGetNode(&unit->base, 1);
    }
    local = node->localMatrix;
    /* vmmul.q M000, M100(root), M200(local): row j of the anchor = sum over k of
       local[j][k] * root row k (column-major root * local) */
    for (j = 0; j < 4; j++) {
        for (r = 0; r < 4; r++) {
            unit->anchorMatrix[j][r] = local[j * 4 + 0] * root[0 * 4 + r] +
                                       local[j * 4 + 1] * root[1 * 4 + r] +
                                       local[j * 4 + 2] * root[2 * 4 + r] +
                                       local[j * 4 + 3] * root[3 * 4 + r];
        }
    }
    /* sv.q C720: the offset starts as the bank zero vector */
    offset[0] = 0.0f;
    offset[1] = 0.0f;
    offset[2] = 0.0f;
    offset[3] = 0.0f;
    switch ((s32)unit->base.base.unk08) {
    case 0x16:
    case 0x1c:
        offset[0] = -50.0f;
        break;
    case 0x18:
        offset[2] = 70.0f;
        break;
    case 0x19:
        break;
    case 0x1a:
        offset[0] = -60.0f;
        break;
    case 0x1b:
        offset[2] = 40.0f;
        break;
    case 0x20:
        offset[0] = -120.0f;
        break;
    default:
        break;
    }
    /* vtfm4.q C000, E100, C200: moved = anchor matrix (column-major) x offset */
    for (r = 0; r < 4; r++) {
        moved[r] = unit->anchorMatrix[0][r] * offset[0] + unit->anchorMatrix[1][r] * offset[1] +
                   unit->anchorMatrix[2][r] * offset[2] + unit->anchorMatrix[3][r] * offset[3];
    }
    /* vadd.t: anchor position xyz += moved xyz; its w is stored back unchanged */
    unit->anchorMatrix[3][0] = unit->anchorMatrix[3][0] + moved[0];
    unit->anchorMatrix[3][1] = unit->anchorMatrix[3][1] + moved[1];
    unit->anchorMatrix[3][2] = unit->anchorMatrix[3][2] + moved[2];
    body = unit->collider0;
    body->attachDirty = 1;
    sphere = unit->collider1->shapeDesc;
    root = unit->base.data->rootMatrix;
    /* sphere center = root matrix row 3 (lv.q/sv.q quad copy) */
    sphere->center.x = root[12];
    sphere->center.y = root[13];
    sphere->center.z = root[14];
    sphere->center.w = root[15];
    sphere->center.y = sphere->center.y + sphere->radius;
    update = &sphere->vtbl[9];
    ((void (*)(void *))update->fn)((u8 *)sphere + update->delta);
    body = unit->collider0;
    if ((unit->stateFlags & 0x200000) != 0) {
        body->flags |= 8;
    } else {
        body->flags &= ~8u;
    }
}
