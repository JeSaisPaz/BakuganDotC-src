// bdc 0x08858cf8 ActorCrystalUpdateBoneAnchors
#include "bdc.h"

/* Crystal override of slot 23 (`BtlBakuganUpdateBoneAnchors`): sets the anchor matrix to the
   model root matrix times node 0's matrix (`GfxModelGetNodeName`/`GfxModelFindNodeMatrix`),
   marks the attach matrix of the colliders `collider0`, `collider1`, `collider2` and `auxObject`
   dirty (each when present), and builds `facingMatrix`: a Y rotation by pi/2 - rot.y (wrapped into
   (-pi, pi]) with the model position (all four lanes) as row 3. */

void ActorCrystalUpdateBoneAnchors(ActorCrystal *self)
{
    const float *root = self->base.base.data->rootMatrix;
    const char *name;
    const float *node;
    float angle;
    float c;
    float s;
    s32 j;
    s32 r;

    name = GfxModelGetNodeName(&self->base.base, 0);
    node = &GfxModelFindNodeMatrix(&self->base.base, name)->x.x;
    /* vmmul.q M000, M100(root), M200(node): row j of the anchor = sum over k of
       node[j][k] * root row k (column-major root * node) */
    for (j = 0; j < 4; j++) {
        for (r = 0; r < 4; r++) {
            self->base.anchorMatrix[j][r] = node[j * 4 + 0] * root[0 * 4 + r] +
                                            node[j * 4 + 1] * root[1 * 4 + r] +
                                            node[j * 4 + 2] * root[2 * 4 + r] +
                                            node[j * 4 + 3] * root[3 * 4 + r];
        }
    }
    if (self->base.collider0 != NULL) {
        self->base.collider0->attachDirty = 1;
    }
    if (self->base.collider1 != NULL) {
        self->base.collider1->attachDirty = 1;
    }
    if (self->collider2 != NULL) {
        ((CollisionCollider *)self->collider2)->attachDirty = 1;
    }
    if (self->auxObject != NULL) {
        ((CollisionCollider *)self->auxObject)->attachDirty = 1;
    }
    angle = 1.57079637f - self->base.base.rot[1];
    if (!(angle <= 3.14159274f)) {
        angle = angle - 6.28318548f;
    } else if (angle <= -3.14159274f) {
        angle = angle + 6.28318548f;
    }
    /* angle * S703 (2/pi) fed to vrot: quarter turns, so plain cos/sin of the angle */
    c = __builtin_cosf(angle);
    s = __builtin_sinf(angle);
    self->facingMatrix[0][0] = c;
    self->facingMatrix[0][1] = 0.0f;
    self->facingMatrix[0][2] = -s;
    self->facingMatrix[0][3] = 0.0f;
    self->facingMatrix[1][0] = 0.0f;
    self->facingMatrix[1][1] = 1.0f;
    self->facingMatrix[1][2] = 0.0f;
    self->facingMatrix[1][3] = 0.0f;
    self->facingMatrix[2][0] = s;
    self->facingMatrix[2][1] = 0.0f;
    self->facingMatrix[2][2] = c;
    self->facingMatrix[2][3] = 0.0f;
    self->facingMatrix[3][0] = self->base.base.pos[0];
    self->facingMatrix[3][1] = self->base.base.pos[1];
    self->facingMatrix[3][2] = self->base.base.pos[2];
    self->facingMatrix[3][3] = self->base.base.pos[3];
}
