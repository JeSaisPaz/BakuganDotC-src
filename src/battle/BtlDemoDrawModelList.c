// bdc 0x088fe9ac BtlDemoDrawModelList
#include "bdc.h"

/* Draws every model of the linked list `*list` whose ambient alpha (`ambient[3]`) is above 0
   (`!(alpha <= 0)`, so NaN draws too) in draw pass `pass` (`GfxSetModelDrawPass`), with the
   camera moved to the origin (`BtlDemoCameraToOrigin`, restored by `BtlDemoCameraRestore`) and
   its view written to the display list (`GfxCameraDlWrite`, flags 1) before and after the list:
   each model's root-matrix translation xyz is shifted by the saved eye, its draw virtual (slot 8)
   is called with the display-list cursor, and the translation (all four lanes) is then reset from
   the model position `pos`. Returns the advanced cursor, or `dl` unchanged for an empty list.
   Same code as `BtlDrawModelListPass`. */

u32 *BtlDemoDrawModelList(u32 *dl, void **list, s32 pass)
{
    u32 *cursor = dl;
    float saved[4];
    GfxModel *model = (GfxModel *)*list;

    if (model == NULL) {
        return dl;
    }
    BtlDemoCameraToOrigin(saved);
    cursor = GfxCameraDlWrite(g_gfxActiveCamera, cursor, 1);
    GfxSetModelDrawPass(pass);
    do {
        if (!(model->ambient[3] <= 0.0f)) {
            const VtblEntry *draw;
            float *trans = &model->data->rootMatrix[12];

            trans[0] = trans[0] - saved[0];
            trans[1] = trans[1] - saved[1];
            trans[2] = trans[2] - saved[2];
            draw = &((const VtblEntry *)model->base.vtable)[8];
            ((void (*)(void *, u32 **))draw->fn)((u8 *)model + draw->delta, &cursor);
            trans = &model->data->rootMatrix[12];
            trans[0] = model->pos[0];
            trans[1] = model->pos[1];
            trans[2] = model->pos[2];
            trans[3] = model->pos[3];
        }
        model = (GfxModel *)model->base.next;
    } while (model != NULL);
    BtlDemoCameraRestore(saved);
    cursor = GfxCameraDlWrite(g_gfxActiveCamera, cursor, 1);
    return cursor;
}
