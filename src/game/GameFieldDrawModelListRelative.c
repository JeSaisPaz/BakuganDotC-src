// bdc 0x088bdfd0 GameFieldDrawModelListRelative
#include "bdc.h"

/* Draws every model of the object list `*list` whose alpha (`ambient[3]`) is not <= 0 in
   camera-relative coordinates (`GameFieldCameraRecenterBegin`): subtracts the saved eye from each
   model's root-matrix translation, calls the model's draw method (vtable entry 8) with the address of
   the display-list cursor, then restores the translation from `pos`. Sets the model draw pass
   (`GfxSetModelDrawPass` `pass`) and returns the advanced display-list pointer (camera state written
   by `GfxCameraDlWrite` before and after). An empty list returns `dl` untouched. */

void *GameFieldDrawModelListRelative(void *dl, void **list, s32 pass)
{
  u32 *cursor = (u32 *)dl;
  GfxModel *m = (GfxModel *)*list;
  float savedEye[4] __attribute__((aligned(16)));

  if (m == NULL) {
    return cursor;
  }
  GameFieldCameraRecenterBegin(savedEye);
  cursor = GfxCameraDlWrite(g_gfxActiveCamera, cursor, 1);
  GfxSetModelDrawPass(pass);
  do {
    if (!(m->ambient[3] <= 0.0f)) {
      const VtblEntry *ve;
      float *t = &m->data->rootMatrix[12];

      /* translation.xyz -= savedEye.xyz (w kept) */
      t[0] = t[0] - savedEye[0];
      t[1] = t[1] - savedEye[1];
      t[2] = t[2] - savedEye[2];
      ve = &((const VtblEntry *)m->base.vtable)[8];
      ((void (*)(void *, u32 **))ve->fn)((u8 *)m + ve->delta, &cursor);
      /* translation = pos (all four lanes) */
      t = &m->data->rootMatrix[12];
      t[0] = m->pos[0];
      t[1] = m->pos[1];
      t[2] = m->pos[2];
      t[3] = m->pos[3];
    }
    m = (GfxModel *)m->base.next;
  } while (m != NULL);
  GameFieldCameraRecenterEnd(savedEye);
  cursor = GfxCameraDlWrite(g_gfxActiveCamera, cursor, 1);
  return cursor;
}
