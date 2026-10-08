// bdc 0x088c73ac GameFieldScreenFxDraw
#include "bdc.h"

/* Draws the field screen-effect holder (`task+0x610`, pointer to a `GameFieldScreenFx`, built by
   `GameFieldScreenFxCtor`): the effect manager (`GfxEffectMgrDrawModels`, `GfxSpriteLayerDrawWorld`
   with `g_gfxActiveCamera`), then a GE chunk that clears the stencil/alpha channel to 0 with 16
   full-height (272-line) 32-px sprites, `GfxMeshObjDrawList5`, and finally the sprite layer
   (`GfxSpriteLayerDraw`). */

void GameFieldScreenFxDraw(void **fx, void *packet)

{
  GfxGeColorVertex16 *verts;
  GfxGeColorVertex16 *v;
  u32 *dl;
  u32 *end;
  s32 i;

  GfxEffectMgrDrawModels(((GameFieldScreenFx *)*fx)->effects, packet, g_gfxActiveCamera);
  GfxSpriteLayerDrawWorld(&((GameFieldScreenFx *)*fx)->effects->base, packet, g_gfxActiveCamera,
                          NULL);
  dl = GfxPacketBeginChunk(packet);
  verts = (GfxGeColorVertex16 *)(dl + 2);
  end = (u32 *)((u8 *)verts + 32 * sizeof(GfxGeColorVertex16));
  dl[0] = ((PspAddr(end) >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
  dl[1] = (PspAddr(end) & 0xffffff) | 0x08000000;          /* JUMP */
  v = verts;
  for (i = 0; i < 32; i++) {
    v->colour = 0;
    v->x = (s16)((i / 2 + i % 2) * 32);
    v->y = (s16)((i % 2) * 272);
    v->z = 0;
    v++;
  }
  end[0] = 0xd3000201; /* CLEAR on, stencil/alpha only */
  end[1] = 0x1280011c; /* VTYPE through, 8888, s16 */
  end += 2;
  if (verts != NULL) {
    end[0] = ((PspAddr(verts) >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
    end[1] = (PspAddr(verts) & 0xffffff) | 0x01000000;          /* VADDR */
    end += 2;
  }
  end[0] = 0x04060020; /* PRIM sprites x32 */
  end[1] = 0xd3000000; /* CLEAR off */
  GfxPacketEndChunk(packet, end + 2);
  GfxMeshObjDrawList5(packet);
  GfxSpriteLayerDraw(((GameFieldScreenFx *)*fx)->layer, packet);
}
