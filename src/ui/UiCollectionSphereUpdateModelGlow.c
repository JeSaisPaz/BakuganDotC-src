// bdc 0x0897b788 UiCollectionSphereUpdateModelGlow
#include "bdc.h"

/* While enabled, bobs every loaded cell model of `UiCollectionSphere`
   to `bobBaseY + (1 − cos(πt))/2 · 0.2` (t += 1/60) and copies the model position into its
   GMO root matrix translation row with w = 1. */

void UiCollectionSphereUpdateModelGlow(UiCollectionSphere *self)
{
    float c;
    GfxModel *m;
    int i;

    if (self->bobOn != 0) {
        self->bobT = self->bobT + 0.016666668f;
        for (i = 0; i < 7; i++) {
            if (self->models[i] != NULL) {
                /* vcos.s of (t·π)·S703 (2/π): cos in radians */
                c = __builtin_cosf(self->bobT * 3.1415927f);
                self->models[i]->pos[1] = self->bobBaseY + (1.0f - c) * 0.5f * 0.2f;
                /* lv.q/sv.q C000: quad copy of pos into rootMatrix[12..15] */
                m = self->models[i];
                m->data->rootMatrix[12] = m->pos[0];
                m->data->rootMatrix[13] = m->pos[1];
                m->data->rootMatrix[14] = m->pos[2];
                m->data->rootMatrix[15] = m->pos[3];
                self->models[i]->data->rootMatrix[15] = 1.0f;
            }
        }
    }
}
