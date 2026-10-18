// bdc 0x089e68a4 CollisionPhysBoxApplyImpulse
#include "bdc.h"

/* Applies an impulse `dir` to the physics box: with no contact point every corner velocity is set
   to `dir` (all four lanes) and 0 is returned; otherwise each corner gets `dir` scaled by how much
   closer than `scale * maxDist` it is to `point` (squared distances, normalised so the weights sum
   to about 20) added to its velocity's x/y/z, corners with a weight `<= 0` are skipped (a NaN weight
   is applied), and the total weight applied is returned. */

float CollisionPhysBoxApplyImpulse(float scale, CollisionPhysBox *box, const float *dir,
                                   const ScePspFVector4 *point)
{
    float dist[8];
    float total = 0.0f;
    float maxDist;
    float sum;
    float norm;
    float limit;
    float w;
    float dx;
    float dy;
    float dz;
    float d;
    s32 i;

    if (point == NULL) {
        for (i = 0; i < 8; i++) {
            box->vel[i].x = dir[0];
            box->vel[i].y = dir[1];
            box->vel[i].z = dir[2];
            box->vel[i].w = dir[3];
        }
        return total;
    }

    sum = 0.0f;
    maxDist = sum;
    for (i = 0; i < 8; i++) {
        dx = VfSub(box->pos[i].x, point->x);
        dy = VfSub(box->pos[i].y, point->y);
        dz = VfSub(box->pos[i].z, point->z);
        d = VfDot3(dx, dy, dz, dx, dy, dz);
        dist[i] = d;
        if (maxDist < d) {
            maxDist = dist[i];
        }
        sum += dist[i];
    }

    norm = 20.0f / sum;
    limit = maxDist * scale;
    for (i = 0; i < 8; i++) {
        w = (limit - dist[i]) * norm;
        if (!(w <= 0.0f)) {
            box->vel[i].x = VfAdd(box->vel[i].x, VfMul(dir[0], w));
            box->vel[i].y = VfAdd(box->vel[i].y, VfMul(dir[1], w));
            box->vel[i].z = VfAdd(box->vel[i].z, VfMul(dir[2], w));
            total = w + total;
        }
    }
    return total;
}
