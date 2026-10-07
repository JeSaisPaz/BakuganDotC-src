// bdc 0x08a150a8 GmoModelCreate
#include "bdc.h"

/* Allocates a GMO model data block (0xc0 bytes, 16-aligned, from pool 0 via `GmoHeapAlloc`) and
   initialises it: refcount 1, empty node/part/material/texture/motion tables, no motion selected
   (`motionIndex` -1, `motionBlend` -1.0), default flags (`flags28` 0xff, `flags2e` 0xfcff,
   `flags30` -1, `meshMask` 0x10001), white colour, `lod` 1.0, `scaleVec` (0,0,0,1) via
   `GmoVec4Set`, `uvTransform` (0,0,1,1) via `GmoVec4SetC` and an identity `rootMatrix` via
   `GmoMatrixScaleTranslate`. Returns the block, or NULL if the allocation failed. */

GmoModel *GmoModelCreate(void)
{
  GmoModel *model;

  model = GmoHeapAlloc(0, 0x10, 0xc0);
  if (model != NULL) {
    model->refCount = 1;
    model->motionIndex = -1;
    model->flags28 = 0xff;
    model->motionBlend = -1.0f;
    model->flags2e = 0xfcff;
    model->color = 0xffffffff;
    model->flags30 = 0xffffffff;
    model->meshMask = 0x10001;
    model->flags02 = 0;
    model->nodes = NULL;
    model->parts = NULL;
    model->materials = NULL;
    model->textures = NULL;
    model->motions = NULL;
    model->nodeCount = 0;
    model->partCount = 0;
    model->materialCount = 0;
    model->textureCount = 0;
    model->motionCount = 0;
    model->enableOverride = 0;
    model->bbox = NULL;
    model->chunk16 = NULL;
    model->lod = 1.0f;
    model->drawNodes = NULL;
    model->drawCount0 = 0;
    model->drawCount1 = 0;
    model->dlCache = NULL;
    model->ptrTables = NULL;
    GmoVec4Set(0.0f, 0.0f, 0.0f, 1.0f, model->scaleVec);
    GmoVec4SetC(0.0f, 0.0f, 1.0f, 1.0f, model->uvTransform);
    GmoMatrixScaleTranslate(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model->rootMatrix);
  }
  return model;
}
