// bdc 0x089dffec GfxModelInitNodeSortKeys
#include "bdc.h"

/* For every node of a GMO model object (`GfxModelGetNode`) stores in `drawGroup` (`+0x1e`) the
   average Z sort key of the materials of its visible meshes (parts whose bit is set in `visible`):
   the signed material state byte `info[2]` (`GmoModelGetMaterial`), which
   `GfxModelBindMaterials` sets from the `__Z<d>` material-name code as `d * 8` (24 = default
   `Z3` when the node has no such mesh). A part entry whose value + 1 fits in 16 bits is an index
   into the model's part table (NULL when out of range), otherwise a part pointer. */

void GfxModelInitNodeSortKeys(GfxModel *self)
{
    s32 i;

    for (i = 0; i < self->nodeCount; i++) {
        GmoNode *node = GfxModelGetNode(self, i);
        s32 sum = 0;
        s32 count = 0;
        s32 j;

        for (j = 0; j < node->partCount; j++) {
            GmoPart *part;
            s32 k;

            if (((1 << j) & node->visible) == 0) {
                continue;
            }
            part = (GmoPart *)node->parts[j];
            if ((((uintptr_t)part + 1) & 0xffff0000) == 0) {
                uintptr_t index = (uintptr_t)part;
                GmoModel *data = self->data;

                if ((index & 0xffff) < data->partCount) {
                    part = &((GmoPart *)data->parts)[index];
                } else {
                    part = NULL;
                }
            }
            if (part == NULL) {
                continue;
            }
            for (k = 0; k < part->meshCount; k++) {
                GmoMaterial *mat =
                    (GmoMaterial *)GmoModelGetMaterial(self->data, part->meshes[k].materialIndex);

                sum += (s8)mat->info[2];
                count++;
            }
        }
        if (count > 0) {
            node->drawGroup = (u16)(sum / count);
        } else {
            node->drawGroup = 0x18;
        }
    }
}
