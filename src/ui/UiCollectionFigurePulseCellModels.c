// bdc 0x0898d8bc UiCollectionFigurePulseCellModels
#include "bdc.h"

/* Per-frame bob of the six cell models of `UiCollectionFigure` while
   `tintOn` is set: advances `tintTimer` by 1/60 s, sets each loaded model's `pos[1]` to
   `tintBase + (1 − cos(πt))/2 · 0.2` and copies the position into its GMO root matrix
   translation row with w = 1. */

void UiCollectionFigurePulseCellModels(UiCollectionFigure *self)
{
    int i;
    float c;
    GfxModel *m;
    GmoModel *g;

    if (self->tintOn != 0) {
        self->tintTimer = self->tintTimer + 0.016666668f;
        for (i = 0; i < 6; i++) {
            if (self->models[i] != NULL) {
                /* vcos.s of (t·π)·S703 (2/π): cos in radians */
                c = __builtin_cosf(self->tintTimer * 3.1415927f);
                self->models[i]->pos[1] = self->tintBase + (1.0f - c) * 0.5f * 0.2f;
                /* lv.q/sv.q C000: quad copy of pos into rootMatrix[12..15] */
                m = self->models[i];
                g = m->data;
                g->rootMatrix[12] = m->pos[0];
                g->rootMatrix[13] = m->pos[1];
                g->rootMatrix[14] = m->pos[2];
                g->rootMatrix[15] = m->pos[3];
                self->models[i]->data->rootMatrix[15] = 1.0f;
            }
        }
    }
}
