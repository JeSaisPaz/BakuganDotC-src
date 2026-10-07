// bdc 0x0882720c GfxMeshObjStateDistortion
#include "bdc.h"

/* State 6 state handler of the mesh object (`GfxMeshObjCtor`) (table `0x08ab9ebc`, run by
   `GfxMeshObjRunState`): like
   `GfxMeshObjStateDistortionLarge` (screen-copy textured quad following its owner effect, index
   table `g_gfxDistortionIndices`) but at 1× the owner's scale. Sub-state `step`: 0 set up
   (returns right after), 1 follow the owner effect (`owner`: blend mode, position and colour) and
   re-fit with `GfxMeshObjFitScreenBillboard` at `owner->size[0]`, 0xdead hide
   (`GfxMeshObjKill`), 0xdeae free `vertexBuffer` and delete itself through the virtual
   destructor. After every path except setup the step is re-read and the billboard re-fitted when
   it is 1 (also right after the self-delete, as in the original). */

typedef struct GfxDtorEntry {
  short thisAdjust;
  short pad;
  void (*fn)(void *self, int flags);
} GfxDtorEntry;

typedef struct GfxDtorVtable {
  u32 hdr[2];
  GfxDtorEntry dtor;
} GfxDtorVtable;

void GfxMeshObjStateDistortion(GfxMeshObj *self)
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
    self->blendMode = owner->blendMode & 0xffff;
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
    self->indices = g_gfxDistortionIndices;
    self->emissive = 1;
    self->step = self->step + 1;
    return;
  }
  if (self->step == 1) {
    GfxMeshObjFitScreenBillboard(owner->size[0], self);
  }
}
