// bdc 0x089f8390 GfxFeedbackTextureStaticInit
#include "bdc.h"

/* Static constructor (entry 47 of `g_cxxCtorTable`) of the global texture object
   `g_feedbackTexture` ("FeedBackTex", 0x140 bytes): runs the `CoreObject` base constructor
   (`CoreObjectInit``(&g_feedbackTexture, NULL)`), installs the texture class vtable
   (`g_gfxTextureVtbl`) at `+0x14`, clears the word at `+0x11c` and registers the object for
   destruction with `CxxRegisterGlobalObject` (record `g_feedbackTextureDtorRecord`, whose
   destructor slot holds the texture destructor `GfxTextureDtor`). The texture's name and pixel
   setup happen later, in the texture-system reset `GfxTextureSystemReset`. */

void GfxFeedbackTextureStaticInit(void)
{
  GfxTexture *t = (GfxTexture *)&g_feedbackTexture;

  CoreObjectInit((CoreObject *)t, (CoreObject *)0x0);
  t->vtbl = g_gfxTextureVtbl;
  t->unk11c = 0;
  CxxRegisterGlobalObject(&g_feedbackTextureDtorRecord);
}
