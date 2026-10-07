// bdc 0x08a151d4 GmoModelReset
#include "bdc.h"

/* Empties a GMO model data block (`GmoModelDestroyContents`) and re-initialises every header
   field exactly like `GmoModelCreate` (empty tables, default flags, `motionBlend` -1.0, `lod` 1.0,
   default vectors and identity `rootMatrix`), then restores the reference count it had on entry.
   Does nothing for NULL. */

void GmoModelReset(GmoModel *self)
{
  s16 refCount;

  if (self != NULL) {
    refCount = self->refCount;
    GmoModelDestroyContents(self);
    self->refCount = 1;
    self->motionIndex = -1;
    self->flags28 = 0xff;
    self->motionBlend = -1.0f;
    self->flags2e = 0xfcff;
    self->color = 0xffffffff;
    self->flags30 = 0xffffffff;
    self->meshMask = 0x10001;
    self->flags02 = 0;
    self->nodes = NULL;
    self->parts = NULL;
    self->materials = NULL;
    self->textures = NULL;
    self->motions = NULL;
    self->nodeCount = 0;
    self->partCount = 0;
    self->materialCount = 0;
    self->textureCount = 0;
    self->motionCount = 0;
    self->enableOverride = 0;
    self->bbox = NULL;
    self->chunk16 = NULL;
    self->lod = 1.0f;
    self->drawNodes = NULL;
    self->drawCount0 = 0;
    self->drawCount1 = 0;
    self->dlCache = NULL;
    self->ptrTables = NULL;
    GmoVec4Set(0.0f, 0.0f, 0.0f, 1.0f, self->scaleVec);
    GmoVec4SetC(0.0f, 0.0f, 1.0f, 1.0f, self->uvTransform);
    GmoMatrixScaleTranslate(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, self->rootMatrix);
    self->refCount = refCount;
  }
}
