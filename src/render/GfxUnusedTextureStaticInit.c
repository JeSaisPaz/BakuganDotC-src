// bdc 0x08808be4 GfxUnusedTextureStaticInit
#include "bdc.h"

/* Static constructor (entry 2 of `g_cxxCtorTable`) of a statically allocated texture object at
   `0x08aac840` that nothing else uses: same shape as `GfxFeedbackTextureStaticInit` —
   `CoreObjectInit``(obj, NULL)`, texture vtable `0x08af5864` at `+0x14`, `+0x11c = 0`, then
   `CxxRegisterGlobalObject` with the record at `0x08aac99c` (destructor `GfxTextureDtor`). */

void GfxUnusedTextureStaticInit(void)
{
  GfxTexture *t = (GfxTexture *)g_unusedTexture;

  CoreObjectInit((CoreObject *)t, (CoreObject *)0x0);
  t->vtbl = g_gfxTextureVtbl;
  t->unk11c = 0;
  CxxRegisterGlobalObject(&g_unusedTextureDtorRecord);
}
