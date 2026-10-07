// bdc 0x088289a0 GfxPuffCtor
#include "bdc.h"

/* Constructor of the 0x1d0-byte sprite puff (a billboard particle; `GfxSpriteCtor` subclass,
   vtable `g_gfxPuffVtbl`): stores the kind `kind` (selects the behaviour in table
   `g_gfxPuffKindTable`), clears the step bytes `step`/`step2`, `unk160`, `unk180` and `layer`,
   enables linear filtering (flag 0x20), quad mode 1 (`GfxSpriteInitQuadMode`), billboard mode 0
   and clears the billboard params (`sv.q` of the constant-bank zero column C720). */

GfxPuff * GfxPuffCtor(GfxPuff *puff, u8 kind)

{
  GfxSpriteCtor(&puff->base);
  puff->base.vtable = g_gfxPuffVtbl;
  puff->kind = kind;
  puff->step = 0;
  puff->step2 = 0;
  puff->unk160 = 0;
  puff->unk180 = 0;
  puff->layer = NULL;
  puff->base.flags = puff->base.flags | 0x20;
  GfxSpriteInitQuadMode(&puff->base, 1);
  puff->base.billboardMode = 0;
  puff->base.maybe_billboardParams80[0] = 0.0f;
  puff->base.maybe_billboardParams80[1] = 0.0f;
  puff->base.maybe_billboardParams80[2] = 0.0f;
  puff->base.maybe_billboardParams80[3] = 0.0f;
  return puff;
}
