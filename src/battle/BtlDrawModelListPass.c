// bdc 0x0884b0d4 BtlDrawModelListPass
#include "bdc.h"

/* Draws every model of the linked list `*list` whose ambient alpha (`ambient[3]`) is above 0 in
   draw pass `pass` (`GfxSetModelDrawPass`), with the camera rebased to the origin
   (`BtlCameraRebaseToOrigin`) and its view written to the packet
   (`GfxCameraDlWrite`, flags 1) before and after the list: each model's root-matrix translation
   (x/y/z) is shifted by the saved eye, its draw virtual (slot 8) is called with the packet
   cursor, and the translation is then reset from the model position `pos` (all four lanes).
   Returns the advanced packet cursor, or `packet` unchanged for an empty list. */

void *BtlDrawModelListPass(void *packet, void **list, int pass)
{
    u32 *cursor = packet;
    float saved[4];
    GfxModel *model = (GfxModel *)*list;

    if (model == NULL) {
        return packet;
    }
    BtlCameraRebaseToOrigin(saved);
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
    BtlCameraRestoreFromOrigin(saved);
    cursor = GfxCameraDlWrite(g_gfxActiveCamera, cursor, 1);
    return cursor;
}
