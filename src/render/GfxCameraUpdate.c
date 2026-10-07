// bdc 0x089e2ae4 GfxCameraUpdate
#include "bdc.h"

/* Recomputes a camera's matrices; `flags` selects the parts (about 40 callers: scene, battle, menu
   and actor cameras).
   - `flags & 1` (only when the eye→target vector has a non-zero x or z): stores the normalised
     view direction in `dir` (w = 0), builds the look-at `view` matrix from `eye`, `target` and
     `up`, the camera-facing `billboard` matrix from `dir` and `g_vecUp`, refreshes `yaw`
     (`GfxCameraUpdateYaw`), clears the cache flags, then copies `view` into `viewOrtho`,
     re-orthonormalises its rotation columns, multiplies it by the fixed `g_gfxCameraOrthoRot`
     and sets its translation column to `g_gfxCameraOrthoTrans` (both function-local statics
     filled once).
   - `flags & 2`: computes the near-plane frustum edges from `fov` (`tanf`), `nearZ`,
     `frustumScale`, `screenOffset` (on a 480x272 screen, aspect 30/17) and `frustumOffset`, builds
     the off-centre perspective `proj` matrix from them and `nearZ`/`farZ`, and clears the cache
     flags.
   - `flags & 3`: hands `proj x view` and `gmoEye` to `GmoSetViewMatrix`.
   A zero-length vector normalises to zero (the VFPU code uses the bank zero S713 as fallback
   scale); normalised components are clamped to [-1, 1]. */

/* 1/sqrt(d), or 0 when d is zero (vcmp EZ + vrsq + vcmovt). */
static float GfxCameraRsqOrZero(float d)
{
    return d == 0.0f ? 0.0f : VfRsq(d);
}

/* Column `j` of `a * b` (column-major): the sum over k of b.j[k] * column k of `a`. */
static void GfxCameraMulColumn(ScePspFVector4 *d, const ScePspFMatrix4 *a, const ScePspFVector4 *col)
{
    d->x = col->x * a->x.x + col->y * a->y.x + col->z * a->z.x + col->w * a->w.x;
    d->y = col->x * a->x.y + col->y * a->y.y + col->z * a->z.y + col->w * a->w.y;
    d->z = col->x * a->x.z + col->y * a->y.z + col->z * a->z.z + col->w * a->w.z;
    d->w = col->x * a->x.w + col->y * a->y.w + col->z * a->z.w + col->w * a->w.w;
}

static void GfxCameraMatMul(ScePspFMatrix4 *d, const ScePspFMatrix4 *a, const ScePspFMatrix4 *b)
{
    GfxCameraMulColumn(&d->x, a, &b->x);
    GfxCameraMulColumn(&d->y, a, &b->y);
    GfxCameraMulColumn(&d->z, a, &b->z);
    GfxCameraMulColumn(&d->w, a, &b->w);
}

void GfxCameraUpdate(GfxCamera *camera, u32 flags)
{
    float diff[3];
    float s;
    float f[3];
    float xa[3];
    float ya[3];
    float sx;
    float sy;
    float ex;
    float ey;
    float ez;
    float d[3];
    float bx[3];
    float c0[3];
    float c2[3];
    ScePspFMatrix4 tmp;
    ScePspFMatrix4 viewProj;
    float h;
    float halfW;
    float h2;
    float offX;
    float offY;
    float rl;
    float tb;
    float nf;
    float n2;

    if (flags & 1) {
        diff[0] = camera->target[0] - camera->eye[0];
        diff[1] = camera->target[1] - camera->eye[1];
        diff[2] = camera->target[2] - camera->eye[2];
        if (diff[0] != 0.0f || diff[2] != 0.0f) {
            /* dir = normalize(diff), w = S713 (bank 0) */
            s = GfxCameraRsqOrZero(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
            camera->dir[0] = VfSat1(diff[0] * s);
            camera->dir[1] = VfSat1(diff[1] * s);
            camera->dir[2] = VfSat1(diff[2] * s);
            camera->dir[3] = 0.0f;

            /* view = look-at: f = normalize(eye - target), x = up x f, y = f x x */
            f[0] = camera->eye[0] - camera->target[0];
            f[1] = camera->eye[1] - camera->target[1];
            f[2] = camera->eye[2] - camera->target[2];
            s = GfxCameraRsqOrZero(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);
            f[0] = VfSat1(f[0] * s);
            f[1] = VfSat1(f[1] * s);
            f[2] = VfSat1(f[2] * s);
            xa[0] = camera->up[1] * f[2] - camera->up[2] * f[1];
            xa[1] = camera->up[2] * f[0] - camera->up[0] * f[2];
            xa[2] = camera->up[0] * f[1] - camera->up[1] * f[0];
            ya[0] = f[1] * xa[2] - f[2] * xa[1];
            ya[1] = f[2] * xa[0] - f[0] * xa[2];
            ya[2] = f[0] * xa[1] - f[1] * xa[0];
            sx = GfxCameraRsqOrZero(xa[0] * xa[0] + xa[1] * xa[1] + xa[2] * xa[2]);
            sy = GfxCameraRsqOrZero(ya[0] * ya[0] + ya[1] * ya[1] + ya[2] * ya[2]);
            xa[0] = VfSat1(xa[0] * sx);
            xa[1] = VfSat1(xa[1] * sx);
            xa[2] = VfSat1(xa[2] * sx);
            ya[0] = VfSat1(ya[0] * sy);
            ya[1] = VfSat1(ya[1] * sy);
            ya[2] = VfSat1(ya[2] * sy);
            ex = -camera->eye[0];
            ey = -camera->eye[1];
            ez = -camera->eye[2];
            camera->view.x.x = xa[0];
            camera->view.x.y = ya[0];
            camera->view.x.z = f[0];
            camera->view.x.w = 0.0f;
            camera->view.y.x = xa[1];
            camera->view.y.y = ya[1];
            camera->view.y.z = f[1];
            camera->view.y.w = 0.0f;
            camera->view.z.x = xa[2];
            camera->view.z.y = ya[2];
            camera->view.z.z = f[2];
            camera->view.z.w = 0.0f;
            camera->view.w.x = xa[0] * ex + xa[1] * ey + xa[2] * ez + 0.0f;
            camera->view.w.y = ya[0] * ex + ya[1] * ey + ya[2] * ez + 0.0f;
            camera->view.w.z = f[0] * ex + f[1] * ey + f[2] * ez + 0.0f;
            camera->view.w.w = 1.0f;

            /* billboard = (normalize(g_vecUp x d), d x that, d = normalize(dir)), identity w */
            s = GfxCameraRsqOrZero(camera->dir[0] * camera->dir[0] + camera->dir[1] * camera->dir[1] +
                                   camera->dir[2] * camera->dir[2]);
            d[0] = VfSat1(camera->dir[0] * s);
            d[1] = VfSat1(camera->dir[1] * s);
            d[2] = VfSat1(camera->dir[2] * s);
            bx[0] = g_vecUp.y * d[2] - g_vecUp.z * d[1];
            bx[1] = g_vecUp.z * d[0] - g_vecUp.x * d[2];
            bx[2] = g_vecUp.x * d[1] - g_vecUp.y * d[0];
            s = GfxCameraRsqOrZero(bx[0] * bx[0] + bx[1] * bx[1] + bx[2] * bx[2]);
            bx[0] = VfSat1(bx[0] * s);
            bx[1] = VfSat1(bx[1] * s);
            bx[2] = VfSat1(bx[2] * s);
            camera->billboard.x.x = bx[0];
            camera->billboard.x.y = bx[1];
            camera->billboard.x.z = bx[2];
            camera->billboard.x.w = 0.0f;
            camera->billboard.y.x = d[1] * bx[2] - d[2] * bx[1];
            camera->billboard.y.y = d[2] * bx[0] - d[0] * bx[2];
            camera->billboard.y.z = d[0] * bx[1] - d[1] * bx[0];
            camera->billboard.y.w = 0.0f;
            camera->billboard.z.x = d[0];
            camera->billboard.z.y = d[1];
            camera->billboard.z.z = d[2];
            camera->billboard.z.w = 0.0f;
            camera->billboard.w.x = 0.0f;
            camera->billboard.w.y = 0.0f;
            camera->billboard.w.z = 0.0f;
            camera->billboard.w.w = 1.0f;

            GfxCameraUpdateYaw(camera);
            camera->viewProjValid = 0;
            camera->maybe_matrixValid = 0;
            if (g_gfxCameraOrthoTransInit == 0) {
                g_gfxCameraOrthoTransInit = 1;
                g_gfxCameraOrthoTrans.x = 0x1.8f5c28p-1f; /* 0.78 */
                g_gfxCameraOrthoTrans.y = 0x1.8f5c28p-1f;
                g_gfxCameraOrthoTrans.z = 0x1.19999ap+1f; /* 2.2 */
                g_gfxCameraOrthoTrans.w = 0.0f;
            }
            /* viewOrtho = view with its rotation columns re-orthonormalised:
               z = x' x y', x = y' x z, each scaled by 1/length (no zero check) */
            camera->viewOrtho = camera->view;
            c2[0] = camera->viewOrtho.x.y * camera->viewOrtho.y.z - camera->viewOrtho.x.z * camera->viewOrtho.y.y;
            c2[1] = camera->viewOrtho.x.z * camera->viewOrtho.y.x - camera->viewOrtho.x.x * camera->viewOrtho.y.z;
            c2[2] = camera->viewOrtho.x.x * camera->viewOrtho.y.y - camera->viewOrtho.x.y * camera->viewOrtho.y.x;
            c0[0] = camera->viewOrtho.y.y * c2[2] - camera->viewOrtho.y.z * c2[1];
            c0[1] = camera->viewOrtho.y.z * c2[0] - camera->viewOrtho.y.x * c2[2];
            c0[2] = camera->viewOrtho.y.x * c2[1] - camera->viewOrtho.y.y * c2[0];
            sx = VfRsq(c0[0] * c0[0] + c0[1] * c0[1] + c0[2] * c0[2]);
            sy = VfRsq(camera->viewOrtho.y.x * camera->viewOrtho.y.x + camera->viewOrtho.y.y * camera->viewOrtho.y.y +
                       camera->viewOrtho.y.z * camera->viewOrtho.y.z);
            s = VfRsq(c2[0] * c2[0] + c2[1] * c2[1] + c2[2] * c2[2]);
            camera->viewOrtho.x.x = c0[0] * sx;
            camera->viewOrtho.x.y = c0[1] * sx;
            camera->viewOrtho.x.z = c0[2] * sx;
            camera->viewOrtho.x.w = 0.0f;
            camera->viewOrtho.y.x = camera->viewOrtho.y.x * sy;
            camera->viewOrtho.y.y = camera->viewOrtho.y.y * sy;
            camera->viewOrtho.y.z = camera->viewOrtho.y.z * sy;
            camera->viewOrtho.z.x = c2[0] * s;
            camera->viewOrtho.z.y = c2[1] * s;
            camera->viewOrtho.z.z = c2[2] * s;
            camera->viewOrtho.z.w = 0.0f;
            if (g_gfxCameraOrthoRotInit == 0) {
                g_gfxCameraOrthoRotInit = 1;
                g_gfxCameraOrthoRot.x.x = 0x1.8836ecp-1f;  /* 0.766044 */
                g_gfxCameraOrthoRot.x.y = -0x1.f838c2p-2f; /* -0.492404 */
                g_gfxCameraOrthoRot.x.z = -0x1.a7179cp-2f; /* -0.413176 */
                g_gfxCameraOrthoRot.x.w = 0.0f;
                g_gfxCameraOrthoRot.y.x = 0.0f;
                g_gfxCameraOrthoRot.y.y = 0x1.491b6p-1f;   /* 0.642787 */
                g_gfxCameraOrthoRot.y.z = -0x1.8836ecp-1f; /* -0.766044 */
                g_gfxCameraOrthoRot.y.w = 0.0f;
                g_gfxCameraOrthoRot.z.x = 0x1.491b6p-1f;   /* 0.642787 */
                g_gfxCameraOrthoRot.z.y = 0x1.2c7432p-1f;  /* 0.586824 */
                g_gfxCameraOrthoRot.z.z = 0x1.f838c2p-2f;  /* 0.492404 */
                g_gfxCameraOrthoRot.z.w = 0.0f;
                g_gfxCameraOrthoRot.w.x = 0.0f;
                g_gfxCameraOrthoRot.w.y = 0.0f;
                g_gfxCameraOrthoRot.w.z = 0.0f;
                g_gfxCameraOrthoRot.w.w = 1.0f;
            }
            /* viewOrtho = g_gfxCameraOrthoRot * viewOrtho; viewOrtho.w = g_gfxCameraOrthoTrans */
            GfxCameraMatMul(&tmp, &g_gfxCameraOrthoRot, &camera->viewOrtho);
            camera->viewOrtho = tmp;
            camera->viewOrtho.w = g_gfxCameraOrthoTrans;
        }
    }
    if (flags & 2) {
        h = camera->nearZ * tanf(camera->fov * 0.5f * 3.14159274f / 180.0f) * camera->frustumScale;
        halfW = h * 1.76470590f;
        h2 = h * 2.0f;
        offX = camera->screenOffset[0] * h2 * 1.76470590f / 480.0f;
        offY = camera->screenOffset[1] * h2 / 272.0f;
        camera->frustumLeft = (-halfW - offX) + camera->frustumOffset[0];
        camera->frustumRight = (halfW - offX) + camera->frustumOffset[0];
        camera->frustumBottom = (offY - h) - camera->frustumOffset[1];
        camera->frustumTop = (h + offY) - camera->frustumOffset[1];
        /* proj = off-centre perspective from (right, left, top, bottom) and nearZ/farZ */
        n2 = camera->nearZ + camera->nearZ;
        nf = camera->nearZ * camera->farZ;
        nf = nf + nf;
        rl = 1.0f / (camera->frustumRight - camera->frustumLeft);
        tb = 1.0f / (camera->frustumTop - camera->frustumBottom);
        s = 1.0f / (camera->nearZ - camera->farZ);
        camera->proj.x.x = n2 * rl;
        camera->proj.x.y = 0.0f;
        camera->proj.x.z = 0.0f;
        camera->proj.x.w = 0.0f;
        camera->proj.y.x = 0.0f;
        camera->proj.y.y = n2 * tb;
        camera->proj.y.z = 0.0f;
        camera->proj.y.w = 0.0f;
        camera->proj.z.x = (camera->frustumRight + camera->frustumLeft) * rl;
        camera->proj.z.y = (camera->frustumTop + camera->frustumBottom) * tb;
        camera->proj.z.z = (camera->nearZ + camera->farZ) * s;
        camera->proj.z.w = -1.0f; /* vfim 0xbc00 */
        camera->proj.w.x = 0.0f;
        camera->proj.w.y = 0.0f;
        camera->proj.w.z = nf * s;
        camera->proj.w.w = 0.0f;
        camera->viewProjValid = 0;
        camera->maybe_matrixValid = 0;
    }
    if (flags & 3) {
        GfxCameraMatMul(&viewProj, &camera->proj, &camera->view);
        GmoSetViewMatrix((const float *)&viewProj, camera->gmoEye);
    }
}
