// bdc 0x089eaf28 UiTextFontReady
#include "bdc.h"

/* Returns whether the `wd_font16` font texture (name from `g_uiTextFontName`) is loaded
   (`GfxTryFindTexture`); `UiTextBoxCreatePrinter` waits for it. */

bool UiTextFontReady(void)

{
  void *tex;

  tex = GfxTryFindTexture(g_uiTextFontName);
  return tex != (void *)0x0;
}
