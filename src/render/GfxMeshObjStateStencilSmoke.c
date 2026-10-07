// bdc 0x088264e0 GfxMeshObjStateStencilSmoke
#include "bdc.h"

/* State 7 state handler of the mesh object (`GfxMeshObjCtor`) (table `0x08ab9ebc`, run by
   `GfxMeshObjRunState`): the visible part of a smoke
   column. Sub-state `step` (`+0x1c`): 0 sets up a `"kemuri1"` textured quad from
   `g_gfxSmokeQuadVerts` (half-alpha black, axis = `g_vecUp`) and advances to 1; 1 (each
   frame) builds the rotation taking +Y onto `axis` (`MathQuatFromUpToDir`) into the basis of
   both stencil-mask children (`child0`, `child1`, see `GfxMeshObjStateStencilMask`) and scales
   them by `radius × 0.09` (the second one inverted and stretched 6× vertically); 2 tells both
   children to hide, hides itself and advances to 3; 3 deletes itself (virtual destructor, flags
   3). Negative or larger steps do nothing. */

void GfxMeshObjStateStencilSmoke(GfxMeshObj *self)
{
    float quat[4];
    float lq[3][4];
    float rq[3][4];
    const float *q;
    const VtblEntry *vt;
    GfxMeshObj *child0;
    float *m;
    float s;
    float ns;
    float s6;
    s32 step;
    s32 i;
    s32 j;

    step = self->step;
    if (step < 2) {
        if (step < 0) {
            return;
        }
        if (step <= 0) {
            self->indices = NULL;
            self->drawKind = 1;
            self->primCmd = 4;
            self->vertexType = 0x81;
            self->vertices = g_gfxSmokeQuadVerts;
            self->texScaleU = 1.0f;
            self->texScaleV = 1.0f;
            self->blendMode = 1;
            self->texture = GfxFindTexture("kemuri1");
            self->visible = 1;
            self->emissive = 0;
            self->texOffsetU = 0.0f;
            self->texOffsetV = 0.0f;
            self->color[0] = 0.0f;
            self->color[1] = 0.0f;
            self->color[2] = 0.0f;
            self->color[3] = 0.5f;
            self->step = self->step + 1;
            self->patchDivS = 1;
            self->patchDivT = 1;
            self->axis[0] = g_vecUp.x;
            self->axis[1] = g_vecUp.y;
            self->axis[2] = g_vecUp.z;
            self->axis[3] = g_vecUp.w;
            return;
        }
        s = self->radius * 0.09f;
        child0 = self->child0;
        q = MathQuatFromUpToDir(quat, self->axis);
        /* Rotation matrix of q = (x, y, z, w): rows 0..2 of the left quaternion-product matrix
           times columns 0..2 of the right one (vmmul.q E000, E200, E100 = M100 · M200);
           lane 3 of columns 0..2 is 0 and column 3 is (0, 0, 0, 1). */
        lq[0][0] = q[3];  lq[0][1] = -q[2]; lq[0][2] = q[1];  lq[0][3] = q[0];
        lq[1][0] = q[2];  lq[1][1] = q[3];  lq[1][2] = -q[0]; lq[1][3] = q[1];
        lq[2][0] = -q[1]; lq[2][1] = q[0];  lq[2][2] = q[3];  lq[2][3] = q[2];
        rq[0][0] = q[3];  rq[0][1] = q[2];  rq[0][2] = -q[1]; rq[0][3] = q[0];
        rq[1][0] = -q[2]; rq[1][1] = q[3];  rq[1][2] = q[0];  rq[1][3] = q[1];
        rq[2][0] = q[1];  rq[2][1] = -q[0]; rq[2][2] = q[3];  rq[2][3] = q[2];
        m = child0->basis;
        for (j = 0; j < 3; j++) {
            for (i = 0; i < 3; i++) {
                m[j * 4 + i] = lq[i][0] * rq[j][0] + lq[i][1] * rq[j][1] + lq[i][2] * rq[j][2] +
                               lq[i][3] * rq[j][3];
            }
            m[j * 4 + 3] = 0.0f;
        }
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
        /* child1->basis = child0->basis */
        for (i = 0; i < 16; i++) {
            self->u158.child1->basis[i] = self->child0->basis[i];
        }
        /* child0 basis columns 0..2 *= s */
        m = self->child0->basis;
        for (i = 0; i < 4; i++) {
            m[i] = m[i] * s;
            m[4 + i] = m[4 + i] * s;
            m[8 + i] = m[8 + i] * s;
        }
        /* child1 basis columns 0..2 *= (-s, -6s, -s) */
        m = self->u158.child1->basis;
        ns = -s;
        s6 = s * -6.0f;
        for (i = 0; i < 4; i++) {
            m[i] = m[i] * ns;
            m[4 + i] = m[4 + i] * s6;
            m[8 + i] = m[8 + i] * ns;
        }
    } else if (step >= 3) {
        if (step < 4 && self != NULL) {
            vt = (const VtblEntry *)self->base.vtable + 1;
            ((void (*)(void *, s32))vt->fn)((char *)self + vt->delta, 3);
        }
        return;
    }
    if (self->step == 2) {
        self->child0->step = 2;
        self->u158.child1->step = 2;
        self->visible = 0;
        self->step = self->step + 1;
    }
}
