// bdc 0x08826354 GfxMeshObjStateStencilMask
#include "bdc.h"

/* State 5 state handler of the mesh object (`GfxMeshObjCtor`) (table `0x08ab9ebc`, run by
   `GfxMeshObjRunState`): an invisible stencil-mask
   mesh: setup draws the built-in spline mesh `g_gfxMaskSplineVerts` (indices
   `g_gfxMaskSplineIndices`, 7×4 patch) with zero colour and almost zero alpha, scales the X/Y/Z
   rows of its matrix by 16 and puts it at the origin (the VFPU bank zero C720), so only the
   stencil writes set by its creator take effect. Sub-state `+0x1c`: 0 set
   up, 1 and anything else nothing, 2 hide, 3 delete itself (virtual destructor, flags 3). Used in
   pairs by `GfxMeshObjCreateSmokeColumn`. */

void GfxMeshObjStateStencilMask(GfxMeshObj *self)
{
  s32 step;
  s32 i;

  step = self->step;
  if (step > 0) {
    if (step < 3) {
      if (self->step == 2) {
        self->visible = 0;
        self->step = self->step + 1;
      }
    } else if (step < 4 && self != NULL) {
      const VtblEntry *dtor = &((const VtblEntry *)self->base.vtable)[1];
      ((void (*)(void *, s32))dtor->fn)((u8 *)self + dtor->delta, 3);
    }
    return;
  }
  if (step < 0)
    return;

  self->vertices = g_gfxMaskSplineVerts;
  self->drawKind = 0;
  self->blendMode = 1;
  self->splineEdgeU = 0;
  self->splineEdgeV = 3;
  self->texture = NULL;
  self->cullMode = 1;
  self->patchCountU = 7;
  self->patchCountV = 4;
  self->patchDivS = 4;
  self->patchDivT = 6;
  self->vertexType = 0x12000980;
  self->buffer = NULL;
  self->indices = g_gfxMaskSplineIndices;
  self->emissive = 0;
  self->color[0] = 0.0f;
  self->color[1] = 0.0f;
  self->color[2] = 0.0f;
  self->color[3] = 0.004f;
  /* basis rows 0..2 *= 16 (all four lanes); position = bank C720 (0, 0, 0, 0) */
  for (i = 0; i < 12; i++)
    self->basis[i] = self->basis[i] * 16.0f;
  self->pos[0] = 0.0f;
  self->pos[1] = 0.0f;
  self->pos[2] = 0.0f;
  self->pos[3] = 0.0f;
  self->scaleA = 0.0f;
  self->visible = 1;
  self->step = self->step + 1;
}
