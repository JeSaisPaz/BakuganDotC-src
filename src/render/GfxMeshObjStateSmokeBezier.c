// bdc 0x088270c4 GfxMeshObjStateSmokeBezier
#include "bdc.h"

/* State 4 state handler of the mesh object (`GfxMeshObjCtor`) (table `0x08ab9ebc`, run by
   `GfxMeshObjRunState`): a `"kemuri1"` (smoke) textured 4×4 bezier patch
   (`g_gfxSmokeBezierVertexData`, `g_gfxSmokeBezierIndexData`, opaque black material) that is
   visible while `g_gmoDrawDisabled` is 0. Sub-state `step` (`+0x1c`): 0 set up, 1 run, 2 hide,
   3 delete itself (virtual destructor, flags 3); any other value does nothing. */

void GfxMeshObjStateSmokeBezier(GfxMeshObj *self)
{
  const VtblEntry *dtor;

  switch (self->step) {
  case 0:
    self->vertices = (void *)g_gfxSmokeBezierVertexData;
    self->drawKind = 2;
    self->blendMode = 1;
    self->texture = GfxFindTexture("kemuri1");
    self->patchCountU = 4;
    self->patchCountV = 4;
    self->patchDivS = 4;
    self->patchDivT = 4;
    self->vertexType = 0x12000880;
    self->buffer = (void *)0x0;
    self->indices = (void *)g_gfxSmokeBezierIndexData;
    self->emissive = 0;
    self->color[0] = 0.0f;
    self->texScaleU = 1.0f;
    self->texScaleV = 1.0f;
    self->color[1] = 0.0f;
    self->color[2] = 0.0f;
    self->color[3] = 1.0f;
    self->visible = 1;
    self->step = self->step + 1;
    break;
  case 1:
    self->visible = g_gmoDrawDisabled == 0;
    break;
  case 2:
    self->visible = 0;
    self->step = 3;
    break;
  case 3:
    if (self != (GfxMeshObj *)0x0) {
      dtor = &((const VtblEntry *)self->base.vtable)[1];
      ((void (*)(void *, s32))dtor->fn)((u8 *)self + dtor->delta, 3);
    }
    break;
  }
}
