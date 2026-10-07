// bdc 0x089f5c80 GfxSpriteLayerDraw2D
#include "bdc.h"

/* Renders a screen-space sprite layer into a render packet (called from the layer draw method
   `GfxSpriteLayerDraw`); does nothing unless the layer alpha is above 0 (a NaN alpha draws).
   Opens a chunk (`GfxPacketBeginChunk`), calls the 2D and sprite state lists, enables the stencil
   test, makes the layer's view matrix the screen camera's and writes that camera's state. Collects
   the visible sprites into g_gfxSpriteSortPairs (`GfxSpriteLayerCollect2D`), sorts them with
   `GfxCombSortByDepth` when the layer is `sorted`, then per sprite writes: the texture call when
   texture or slot changed (`GfxTextureWriteCall`), the world matrix
   (`GfxDlWriteSpriteWorldMatrix`), the blend state when `blendMode & 0xffff` changed (0 opaque,
   1 alpha, 2 additive, 3 subtractive, others write nothing), the texture filter (linear when
   `flags & 0x20`), the alpha test (`>= alphaRef`, or `> 0` when it is 0) and colour write mask
   (all RGB masked when `alphaRef` is set and `flags & 0x40`), the stencil state
   (`GfxSpriteWriteStencilState`), the material colour `tint`/`alpha` times the layer alpha
   (`GfxColorToRgba8Alpha`) and the specular colour `addColor`, then a 4-vertex triangle strip:
   quad modes 0/1 draw `vertices` in place (byte UVs), other modes copy the 4 float-UV vertices
   inline into the display list behind a JUMP. Closes the chunk with `GfxPacketEndChunk`.
   The specular colour uses the VFPU bank constant S701 = 255. */

void GfxSpriteLayerDraw2D(GfxSpriteLayer *self, void *packet)
{
  GfxSprite *head = self->head;
  GfxSpriteSortPair *pair;
  GfxSprite *sprite;
  void *texture;
  u32 *dl;
  u32 *p;
  u32 *verts;
  u32 *end;
  const u32 *src;
  u32 mode;
  u32 rgba;
  u32 spec;
  u32 addr;
  int blend;
  int slot;
  int count;
  int i;
  u32 k;

  if (self->alpha <= 0.0f) {
    return;
  }

  dl = GfxPacketBeginChunk((RenderPacket *)packet);
  dl = GfxDlCall2DState(dl);
  dl[0] = 0x24000001; /* STENCIL TEST enable */
  dl = GfxDlCallSpriteState(dl + 1);
  GfxScreenCameraSetView(&self->view);
  dl = GfxCameraDlWrite(g_gfxScreenCamera, dl, 0xffffffff);

  blend = -1;
  texture = NULL;
  slot = -1;
  count = GfxSpriteLayerCollect2D(self, head, g_gfxSpriteSortPairs);
  if (self->sorted != 0) {
    GfxCombSortByDepth(g_gfxSpriteSortPairs, count);
  }

  pair = g_gfxSpriteSortPairs;
  for (i = 0; i < count; i++, pair++) {
    sprite = pair->sprite;

    if (texture != sprite->texture || slot != sprite->textureSlot) {
      slot = sprite->textureSlot;
      texture = sprite->texture;
      dl = GfxTextureWriteCall(texture, dl, slot);
    }
    dl = GfxDlWriteSpriteWorldMatrix(dl, sprite->matrix, &sprite->posX);

    mode = sprite->blendMode & 0xffff;
    if (blend != (int)mode) {
      blend = (int)mode;
      switch (blend) {
      case 0: /* opaque: src * 1 + dst * 0 */
        dl[0] = 0xdf0000aa; /* BLEND fixA, fixB, add */
        dl[1] = 0xe0ffffff; /* FIXA */
        dl[2] = 0xe1000000; /* FIXB */
        dl += 3;
        break;
      case 1: /* alpha */
        dl[0] = 0xdf000032; /* BLEND src alpha, 1 - src alpha, add */
        dl[1] = 0xe0000000;
        dl[2] = 0xe1000000;
        dl += 3;
        break;
      case 2: /* additive */
        dl[0] = 0xdf0000a2; /* BLEND src alpha, fixB, add */
        dl[1] = 0xe0000000;
        dl[2] = 0xe1ffffff;
        dl += 3;
        break;
      case 3: /* subtractive */
        dl[0] = 0xdf0002a2; /* BLEND src alpha, fixB, reverse subtract */
        dl[1] = 0xe0000000;
        dl[2] = 0xe1ffffff;
        dl += 3;
        break;
      }
    }

    dl[0] = (sprite->flags & 0x20) != 0 ? 0xc6000101 : 0xc6000000; /* TFILTER linear / nearest */
    if (sprite->alphaRef != 0) {
      dl[1] = (sprite->alphaRef << 8) | 0xdbff0007;                  /* ATST >= ref */
      dl[2] = (sprite->flags & 0x40) != 0 ? 0xe8ffffff : 0xe8000000; /* PMSKC */
    } else {
      dl[1] = 0xdbff0006; /* ATST > 0 */
      dl[2] = 0xe8000000; /* PMSKC */
    }
    dl = GfxSpriteWriteStencilState(sprite, dl + 3);

    rgba = GfxColorToRgba8Alpha((const ScePspFVector4 *)sprite->tint, self->alpha);
    dl[0] = (rgba & 0xffffff) | 0x55000000; /* material colour */
    dl[1] = (rgba >> 24) | 0x58000000;      /* material alpha */
    dl += 2;

    /* Specular colour: clamp to [0, 1], scale by 255 (bank constant S701), truncate to bytes. */
    spec = (u32)VfI2uc(VfF2iz(VfSat0(sprite->addColor[0]) * 255.0f, 23)) |
           (u32)VfI2uc(VfF2iz(VfSat0(sprite->addColor[1]) * 255.0f, 23)) << 8 |
           (u32)VfI2uc(VfF2iz(VfSat0(sprite->addColor[2]) * 255.0f, 23)) << 16;
    dl[0] = (spec & 0xffffff) | 0x57000000; /* material specular colour */
    dl += 1;

    if (sprite->quadMode < 2) {
      src = (const u32 *)sprite->vertices;
      addr = (u32)(uintptr_t)src;
      dl[0] = 0x12000081; /* VTYPE u8 UVs, s8 positions */
      p = dl + 1;
      if (src != NULL) {
        p[0] = ((addr >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
        p[1] = (addr & 0xffffff) | 0x01000000;          /* VADDR */
        p += 2;
      }
      p[0] = 0x04040004; /* PRIM triangle strip x4 */
      dl = p + 1;
    } else {
      /* Copy the 4 vertices (48 bytes) inline and jump over them. */
      verts = (u32 *)(((uintptr_t)(dl + 2) + 15) & ~(uintptr_t)15);
      end = verts + 12;
      addr = (u32)(uintptr_t)end;
      dl[0] = ((addr >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
      dl[1] = (addr & 0xffffff) | 0x08000000;          /* JUMP */
      src = (const u32 *)sprite->vertices;
      for (k = 0; k < 12; k++) {
        verts[k] = src[k];
      }
      end[0] = 0x12000083; /* VTYPE float UVs, s8 positions */
      p = end + 1;
      if (verts != NULL) {
        addr = (u32)(uintptr_t)verts;
        p[0] = ((addr >> 24 & 0xf) << 16) | 0x10000000; /* BASE */
        p[1] = (addr & 0xffffff) | 0x01000000;          /* VADDR */
        p += 2;
      }
      p[0] = 0x04040004; /* PRIM triangle strip x4 */
      dl = p + 1;
    }
  }
  GfxPacketEndChunk((RenderPacket *)packet, dl);
}
