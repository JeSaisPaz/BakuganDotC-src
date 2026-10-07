// bdc 0x089dcbbc GmoDlWritePatchDivision
#include "bdc.h"

/* Writes the Bezier/spline patch division (`0x36`) from the model's level of detail `model+0x44`
   times 16, or times the mesh's own s/t factors (`mesh+8`, flag 0x80000), clamped to 1..64. */

void GmoDlWritePatchDivision(GmoDlContext *self)
{
    GmoMesh *mesh = self->mesh;
    float lod = self->model->lod;
    float scaleS;
    float scaleT;
    int s;
    int t;

    if ((mesh->flags & 0x80000) == 0) {
        scaleT = lod * 16.0f;
        scaleS = scaleT;
    } else {
        scaleT = lod * mesh->patchScale[1];
        scaleS = lod * mesh->patchScale[0];
    }
    s = (int)scaleS;
    t = (int)scaleT;
    if (s < 1) {
        s = 1;
    }
    if (t < 1) {
        t = 1;
    }
    if (s > 0x40) {
        s = 0x40;
    }
    if (t > 0x40) {
        t = 0x40;
    }
    *self->cur++ = (t << 8) | 0x36000000 | s;
}
