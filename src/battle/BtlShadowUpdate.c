// bdc 0x08885470 BtlShadowUpdate
#include "bdc.h"

/* Per-frame update of a ground-shadow object (`BtlShadow`, built by `BtlShadowCtorForUnit` /
   `BtlShadowCtorForActor` and `BtlShadowInit`). When the mode is not 3, the owner is set,
   `enabled` is set, the owner model is `visible` and its `ambient[3]` is not <= 0.001, it shows
   the visual (billboard `flags |= 1`, or mesh `visible = 1`) and places it:
   - ground point and normal by mode: 0 (a `BtlBakugan`) its `groundPoint` when bit 0x40000000
     of `stateFlags` is set, else bone 0 (`GfxModelGetNodeWorldPosByIndex`) with the model's y in
     state 15, else the model position; in a water stage (`BtlBakuganIsInWaterStage0CTo0F`) y =
     `groundPoint[1]` + `BtlGetStageWaterHeight` + 10; normal `groundNormal`. 1 (an Actor) its
     `groundPoint` when bit 0x40000000 of `flags` is set, else the model position; normal
     `groundNormal`. 2: bone 1 with y from `mtx[13]`; 4: the model position with y from `mtx[5]`;
     other modes: (0, 0, 0, 0); normal `g_vecUp` for 2, 4 and the others.
   - basis (billboard `matrix` or mesh `basis`): when the normal is (anti)parallel to Y
     (`x^2 + z^2 < 1e-5` and `y^2` not <= 1e-4) rows (cos yaw, 0, -sin yaw, 0), (sin yaw, 0,
     cos yaw, 0), (0, 0, 1, 0), (0, 0, 0, 1) with yaw = `pi/2 - rot[1]` wrapped into (-pi, pi];
     else the matrix of quaternion (Y -> normal tilt, as `MathQuatFromUpToDir`) * (yaw about Y),
     times `g_gfxSwapYZMatrix`; the along-Y case of that path multiplies `g_quatIdentity` by
     the yaw quaternion (the test is repeated, so it is never taken).
   - billboard: `width`/`height` = `size[0]`/`size[2]`, depth 1, `maybe_sizeW` 0, alpha 0.8 *
     `ambient[3]`, position = ground point. Mesh: `axis` = normal and `radius` = `size[0]` (stored
     before the basis is built), basis rows 0..1 scaled by `size[0]`, `size[2]` (row 2 by 1),
     `color[3]` = 0.8 * `ambient[3]`, position set with `GfxMeshObjSetPositionWithChildren`.
   Otherwise it hides the visual (billboard `flags &= ~1`, or mesh `visible = 0`). */

void BtlShadowUpdate(void *shadowObj)
{
    BtlShadow *shadow = (BtlShadow *)shadowObj;
    GfxModel *model;
    float pos[4];
    float normal[4];
    ScePspFVector4 nodePos;
    float quat[4];
    float yawQuat[4];
    float axis[4];
    float axisQuat[4];
    float a[4][4];
    float b[4][4];
    float m[4][4];
    float *basis;
    float yaw;
    float alpha;
    float angle;
    float waterY;
    float len2;
    float inv;
    float s;
    float c;
    int i;
    int j;
    int k;
    bool active;
    bool alongY;

    active = true;
    if (shadow->mode == 3) {
        active = false;
    } else if (shadow->owner == NULL) {
        active = false;
    } else if (shadow->enabled == 0) {
        active = false;
    } else if (((GfxModel *)shadow->owner)->visible == 0) {
        active = false;
    } else if (((GfxModel *)shadow->owner)->ambient[3] <= 0.001f) {
        active = false;
    }

    if (!active) {
        if (shadow->billboard != NULL) {
            shadow->billboard->flags = shadow->billboard->flags & ~1u;
        } else {
            shadow->mesh->visible = 0;
        }
        return;
    }

    if (shadow->billboard != NULL) {
        shadow->billboard->flags = shadow->billboard->flags | 1;
    } else {
        shadow->mesh->visible = 1;
    }

    model = (GfxModel *)shadow->owner;
    yaw = 1.5707964f - model->rot[1];
    if (!(yaw <= 3.1415927f)) {
        yaw = yaw - 6.2831855f;
    } else if (yaw <= -3.1415927f) {
        yaw = yaw + 6.2831855f;
    }
    alpha = ((GfxModel *)shadow->owner)->ambient[3] * 0.8f;

    if (shadow->mode == 0) {
        BtlBakugan *unit = (BtlBakugan *)shadow->owner;

        if ((unit->stateFlags & 0x40000000) != 0) {
            unit = (BtlBakugan *)shadow->owner;
            for (i = 0; i < 4; i++) {
                pos[i] = unit->groundPoint[i];
            }
        } else if (((BtlBakugan *)shadow->owner)->state == 15) {
            GfxModelGetNodeWorldPosByIndex(&((BtlBakugan *)shadow->owner)->base, &nodePos, 0);
            pos[0] = nodePos.x;
            pos[1] = nodePos.y;
            pos[2] = nodePos.z;
            pos[3] = nodePos.w;
            pos[1] = ((BtlBakugan *)shadow->owner)->base.pos[1];
        } else {
            unit = (BtlBakugan *)shadow->owner;
            for (i = 0; i < 4; i++) {
                pos[i] = unit->base.pos[i];
            }
        }
        if (BtlBakuganIsInWaterStage0CTo0F((BtlBakugan *)shadow->owner) != 0) {
            waterY = BtlGetStageWaterHeight();
            pos[1] = ((BtlBakugan *)shadow->owner)->groundPoint[1] + waterY + 10.0f;
        }
        unit = (BtlBakugan *)shadow->owner;
        for (i = 0; i < 4; i++) {
            normal[i] = unit->groundNormal[i];
        }
    } else if (shadow->mode == 1) {
        Actor *actor = (Actor *)shadow->owner;

        if ((actor->flags & 0x40000000) != 0) {
            actor = (Actor *)shadow->owner;
            for (i = 0; i < 4; i++) {
                pos[i] = actor->groundPoint[i];
            }
        } else {
            actor = (Actor *)shadow->owner;
            for (i = 0; i < 4; i++) {
                pos[i] = actor->base.pos[i];
            }
        }
        actor = (Actor *)shadow->owner;
        for (i = 0; i < 4; i++) {
            normal[i] = actor->groundNormal[i];
        }
    } else if (shadow->mode == 2) {
        GfxModelGetNodeWorldPosByIndex(&((Actor *)shadow->owner)->base, &nodePos, 1);
        pos[0] = nodePos.x;
        pos[1] = nodePos.y;
        pos[2] = nodePos.z;
        pos[3] = nodePos.w;
        normal[0] = g_vecUp.x;
        normal[1] = g_vecUp.y;
        normal[2] = g_vecUp.z;
        normal[3] = g_vecUp.w;
        pos[1] = ((Actor *)shadow->owner)->mtx[13];
    } else if (shadow->mode == 4) {
        Actor *actor = (Actor *)shadow->owner;

        for (i = 0; i < 4; i++) {
            pos[i] = actor->base.pos[i];
        }
        normal[0] = g_vecUp.x;
        normal[1] = g_vecUp.y;
        normal[2] = g_vecUp.z;
        normal[3] = g_vecUp.w;
        pos[1] = ((Actor *)shadow->owner)->mtx[5];
    } else {
        /* The bank's zero vector C720. */
        pos[0] = 0.0f;
        pos[1] = 0.0f;
        pos[2] = 0.0f;
        pos[3] = 0.0f;
        normal[0] = g_vecUp.x;
        normal[1] = g_vecUp.y;
        normal[2] = g_vecUp.z;
        normal[3] = g_vecUp.w;
    }

    if (shadow->billboard != NULL) {
        basis = shadow->billboard->matrix;
    } else {
        for (i = 0; i < 4; i++) {
            shadow->mesh->axis[i] = normal[i];
        }
        shadow->mesh->radius = shadow->size[0];
        basis = shadow->mesh->basis;
    }

    if (normal[0] * normal[0] + normal[2] * normal[2] < 1e-05f) {
        alongY = !(normal[1] * normal[1] <= 0.0001f);
    } else {
        alongY = false;
    }
    if (alongY) {
        /* Rotation by yaw; the angle is scaled to quarter turns by the bank's 2/pi. */
        c = __builtin_cosf(yaw);
        s = __builtin_sinf(yaw);
        basis[0] = c;
        basis[1] = 0.0f;
        basis[2] = -s;
        basis[3] = 0.0f;
        basis[4] = s;
        basis[5] = 0.0f;
        basis[6] = c;
        basis[7] = 0.0f;
        basis[8] = 0.0f;
        basis[9] = 0.0f;
        basis[10] = 1.0f;
        basis[11] = 0.0f;
        basis[12] = 0.0f;
        basis[13] = 0.0f;
        basis[14] = 0.0f;
        basis[15] = 1.0f;
    } else {
        /* The binary repeats the along-Y test here (inlined quaternion builder). */
        if (normal[0] * normal[0] + normal[2] * normal[2] < 1e-05f) {
            alongY = !(normal[1] * normal[1] <= 0.0001f);
        } else {
            alongY = false;
        }
        if (alongY) {
            /* yawQuat = (0, sin(yaw / 2), 0, cos(yaw / 2)); quat = identity * yawQuat */
            yawQuat[0] = 0.0f;
            yawQuat[1] = VfSinQuarter(yaw * 0.318309873f);
            yawQuat[2] = 0.0f;
            yawQuat[3] = VfCosQuarter(yaw * 0.318309873f);
            axisQuat[0] = g_quatIdentity.x;
            axisQuat[1] = g_quatIdentity.y;
            axisQuat[2] = g_quatIdentity.z;
            axisQuat[3] = g_quatIdentity.w;
        } else {
            axis[0] = normal[2];
            axis[1] = 0.0f;
            axis[2] = -normal[0];
            /* Normalise the axis (a zero length gives 0), clamped to [-1, 1]. */
            len2 = axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2];
            inv = VfRsq(len2);
            if (len2 == 0.0f) {
                inv = 0.0f;
            }
            axis[0] = VfSat1(axis[0] * inv);
            axis[1] = VfSat1(axis[1] * inv);
            axis[2] = VfSat1(axis[2] * inv);
            axis[3] = 0.0f;
            angle = acosf(normal[1]);
            /* axisQuat = (axis * sin(angle / 2), cos(angle / 2)) */
            c = VfCosQuarter(0.318309873f * angle);
            s = VfSinQuarter(0.318309873f * angle);
            axisQuat[0] = axis[0] * s;
            axisQuat[1] = axis[1] * s;
            axisQuat[2] = axis[2] * s;
            axisQuat[3] = c;
            yawQuat[0] = 0.0f;
            yawQuat[1] = VfSinQuarter(yaw * 0.318309873f);
            yawQuat[2] = 0.0f;
            yawQuat[3] = VfCosQuarter(yaw * 0.318309873f);
        }
        /* quat = axisQuat * yawQuat (Hamilton product, w last) */
        quat[0] = axisQuat[0] * yawQuat[3] + axisQuat[1] * yawQuat[2] - axisQuat[2] * yawQuat[1]
                  + axisQuat[3] * yawQuat[0];
        quat[1] = -axisQuat[0] * yawQuat[2] + axisQuat[1] * yawQuat[3] + axisQuat[2] * yawQuat[0]
                  + axisQuat[3] * yawQuat[1];
        quat[2] = axisQuat[0] * yawQuat[1] - axisQuat[1] * yawQuat[0] + axisQuat[2] * yawQuat[3]
                  + axisQuat[3] * yawQuat[2];
        quat[3] = -axisQuat[0] * yawQuat[0] - axisQuat[1] * yawQuat[1] - axisQuat[2] * yawQuat[2]
                  + axisQuat[3] * yawQuat[3];

        /* Matrix of quat (vmmul.q E000, E200, E100 = M100 · M200, column-major): row j,
           lane i = sum_k b[j][k] * a[k][i] over the two signed swizzles of quat; the w row and
           column are then set to identity. */
        a[0][0] = quat[3];  a[0][1] = quat[2];  a[0][2] = -quat[1]; a[0][3] = -quat[0];
        a[1][0] = -quat[2]; a[1][1] = quat[3];  a[1][2] = quat[0];  a[1][3] = -quat[1];
        a[2][0] = quat[1];  a[2][1] = -quat[0]; a[2][2] = quat[3];  a[2][3] = -quat[2];
        a[3][0] = quat[0];  a[3][1] = quat[1];  a[3][2] = quat[2];  a[3][3] = quat[3];
        b[0][0] = quat[3];  b[0][1] = quat[2];  b[0][2] = -quat[1]; b[0][3] = quat[0];
        b[1][0] = -quat[2]; b[1][1] = quat[3];  b[1][2] = quat[0];  b[1][3] = quat[1];
        b[2][0] = quat[1];  b[2][1] = -quat[0]; b[2][2] = quat[3];  b[2][3] = quat[2];
        b[3][0] = -quat[0]; b[3][1] = -quat[1]; b[3][2] = -quat[2]; b[3][3] = quat[3];
        for (j = 0; j < 3; j++) {
            for (i = 0; i < 3; i++) {
                m[j][i] = b[j][0] * a[0][i] + b[j][1] * a[1][i] + b[j][2] * a[2][i]
                          + b[j][3] * a[3][i];
            }
            m[j][3] = 0.0f;
        }
        m[3][0] = 0.0f;
        m[3][1] = 0.0f;
        m[3][2] = 0.0f;
        m[3][3] = 1.0f;

        /* basis = basis · swapYZ (vmmul.q M000, M100, M200, column-major):
           basis row j, lane i = sum_k swapYZ row j [k] * m row k [i] */
        for (j = 0; j < 4; j++) {
            const ScePspFVector4 *row = (j == 0) ? &g_gfxSwapYZMatrix.x
                                      : (j == 1) ? &g_gfxSwapYZMatrix.y
                                      : (j == 2) ? &g_gfxSwapYZMatrix.z
                                                 : &g_gfxSwapYZMatrix.w;
            for (i = 0; i < 4; i++) {
                basis[j * 4 + i] = row->x * m[0][i] + row->y * m[1][i] + row->z * m[2][i]
                                   + row->w * m[3][i];
            }
        }
    }

    if (shadow->billboard != NULL) {
        shadow->billboard->width = shadow->size[0];
        shadow->billboard->height = shadow->size[2];
        shadow->billboard->depth = 1.0f;
        shadow->billboard->maybe_sizeW = 0.0f;
        shadow->billboard->alpha = alpha;
        shadow->billboard->posX = pos[0];
        shadow->billboard->posY = pos[1];
        shadow->billboard->posZ = pos[2];
        shadow->billboard->posW = pos[3];
        return;
    }

    /* Scale basis rows 0..2 by (size[0], size[2], 1); row 2 times 1.0f is unchanged. */
    basis = shadow->mesh->basis;
    for (k = 0; k < 4; k++) {
        basis[k] = basis[k] * shadow->size[0];
    }
    for (k = 0; k < 4; k++) {
        basis[4 + k] = basis[4 + k] * shadow->size[2];
    }
    shadow->mesh->color[3] = alpha;
    GfxMeshObjSetPositionWithChildren(shadow->mesh, pos);
}
