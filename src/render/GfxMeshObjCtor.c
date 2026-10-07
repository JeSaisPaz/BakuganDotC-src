// bdc 0x0882536c GfxMeshObjCtor
#include "bdc.h"

/* Constructor of the 0x1c0-byte mesh object (an effect primitive drawn straight to the GE:
   `CoreObjectInit`, vtable `g_gfxMeshObjVtbl`): clears the index/vertex pointers `+0xf0`/`+0xf4`,
   `+0xf8`, initialises the effect chain at `+0x100` (`GfxEffectChainCtor`), clears the
   material/state fields `+0x138..+0x1a8` and `+0x1b8` (including `visible` `+0x13c = 0`), sets
   `splineEdgeV` `+0x188 = 3`, the byte `geE7Arg` `+0x1a1 = 1`, and the default stencil commands
   (`+0x1b0` = `0xdcff0001` STST always, `+0x1b4` = `0xdd000000` SOP keep). Returns `self`. */

void * GfxMeshObjCtor(GfxMeshObj *self)

{
  CoreObjectInit(&self->base,(CoreObject *)0x0);
  (self->base).vtable = g_gfxMeshObjVtbl;
  self->indices = (void *)0x0;
  self->vertices = (void *)0x0;
  self->buffer = (void *)0x0;
  GfxEffectChainCtor(&self->effectChain);
  self->reserved138 = 0;
  self->visible = 0;
  self->cullMode = 0;
  self->splineEdgeU = 0;
  self->splineEdgeV = 3;
  self->clut = 0;
  self->extraCmd = 0;
  self->vertexBuffer = (void *)0x0;
  self->geE7Arg = 1;
  self->alphaRef = 0;
  self->drawKind = 0;
  self->stencilTest = 0xdcff0001;
  self->stencilOp = 0xdd000000;
  self->flags = 0;
  return self;
}
