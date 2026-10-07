// bdc 0x08826f0c GfxMeshObjStateDistortionLarge
#include "bdc.h"

/* State 3 state handler of the mesh object (`GfxMeshObjCtor`) (table `0x08ab9ebc`, run by
   `GfxMeshObjRunState`): screen-distortion sprite that
   follows its owner effect. Setup (step 0, returns right after) uses the built-in quad
   `g_gfxDistortionPatchVerts` with index table `g_gfxDistortionLargeIndices`, the screen copy
   texture (`GfxGetFeedbackTexture`) and the owner's blend mode (`GfxEffect.blendMode`); step 1
   copies the owner's position and colour each frame. After every path except setup the step is
   re-read and, when it is 1, the screen mapping is re-fitted with `GfxMeshObjFitScreenBillboard`
   at 4× the owner's `size[0]` (the original also computes 4× `size[1]` into the unused second
   float argument register). 0xdead hides (`GfxMeshObjKill`), 0xdeae frees `vertexBuffer` and
   deletes itself through the virtual destructor (the step is still re-read afterwards, as in the
   original). */

typedef struct GfxDtorEntry {
  short thisAdjust;
  short pad;
  void (*fn)(void *self, int flags);
} GfxDtorEntry;

typedef struct GfxDtorVtable {
  u32 hdr[2];
  GfxDtorEntry dtor;
} GfxDtorVtable;

void GfxMeshObjStateDistortionLarge(GfxMeshObj *self)
{
  GfxEffect *owner;
  int step;
  void *buf;
  const GfxDtorEntry *e;

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
      e = &((const GfxDtorVtable *)self->base.vtable)->dtor;
      e->fn((char *)self + e->thisAdjust, 3);
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
    self->vertices = g_gfxDistortionPatchVerts;
    self->drawKind = 1;
    self->primCmd = 0x1fd;
    self->texture = GfxGetFeedbackTexture();
    self->vertexType = 0x1200089a;
    self->indices = g_gfxDistortionLargeIndices;
    self->emissive = 1;
    self->step = self->step + 1;
    self->blendMode = owner->blendMode & 0xffff;
    return;
  }
  if (self->step == 1) {
    GfxMeshObjFitScreenBillboard(owner->size[0] * 4.0f, self);
  }
}
