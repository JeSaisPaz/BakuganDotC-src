// bdc 0x08826f0c GfxMeshObjStateDistortionLarge
#include "bdc.h"

/* State 3 state handler of the mesh object (`GfxMeshObjCtor`) (table `0x08ab9ebc`, run by
   `GfxMeshObjRunState`): screen-distortion sprite that
   follows its owner effect. Setup (step 0, returns right after) uses the built-in 16×16 grid
   index list `g_gfxDistortionGridIndices` with vertex table `g_gfxDistortionLargeVertices`, the screen copy
   texture (`GfxGetFeedbackTexture`) and the owner's blend mode (`GfxEffect.blendMode`); step 1
   copies the owner's position and colour each frame. After every path except setup the step is
   re-read and, when it is 1, the screen mapping is re-fitted with `GfxMeshObjFitScreenBillboard`
   at 4× the owner's `size[0]` (the original also computes 4× `size[1]` into the unused second
   float argument register). 0xdead hides (`GfxMeshObjKill`), 0xdeae frees `vertexBuffer` and
   deletes itself through the virtual destructor (the step is still re-read afterwards, as in the
   original). */

void GfxMeshObjStateDistortionLarge(GfxMeshObj *self)
{
  GfxEffect *owner;
  int step;
  void *buf;
  const VtblEntry *e;

  owner = (GfxEffect *)self->owner;
  step = self->step;
  if (step == 0xdeae) {
    if (self->vertexBuffer != NULL) {
      buf = self->vertexBuffer;
      MemLock();
      MemFree(buf, NULL, 0);
      MemUnlock();
      self->vertexBuffer = NULL;
    }
    if (self != NULL) {
      e = &((const VtblEntry *)self->base.vtable)[1];
      ((void (*)(void *, s32))e->fn)((char *)self + e->delta, 3);
    }
  } else if (step == 0xdead) {
    self->visible = 0;
    self->step = self->step + 1;
  } else if (step == 1) {
    self->visible = 1;
    self->pos[0] = owner->pos[0];
    self->pos[1] = owner->pos[1];
    self->pos[2] = owner->pos[2];
    self->pos[3] = owner->pos[3];
    self->color[0] = owner->color[0];
    self->color[1] = owner->color[1];
    self->color[2] = owner->color[2];
    self->color[3] = owner->color[3];
  } else if (step == 0) {
    self->indices = g_gfxDistortionGridIndices;
    self->drawKind = 1;
    self->primCmd = 0x1fd;
    self->texture = GfxGetFeedbackTexture();
    self->vertexType = 0x1200089a;
    self->vertices = g_gfxDistortionLargeVertices;
    self->emissive = 1;
    self->step = self->step + 1;
    self->blendMode = owner->blendMode & 0xffff;
    return;
  }
  if (self->step == 1) {
    GfxMeshObjFitScreenBillboard(owner->size[0] * 4.0f, self);
  }
}
