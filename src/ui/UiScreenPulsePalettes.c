// bdc 0x08909c90 UiScreenPulsePalettes
#include "bdc.h"

/* Animates the two palette-blend objects of a `UiScreen` (cursor/highlight glow): blends
   `paletteAnimA` from CLUT row `paletteRowA` and `paletteAnimB` from row `paletteRowB`
   toward row `paletteRowTo` with factor `1 - |cos(3° · t)|` (`GfxPaletteBlend`), then advances the
   timer `pulseTimer` by `frameSkip + 1`, resetting it to 0 once it has reached 60. */

void UiScreenPulsePalettes(UiScreen *screen)
{
    /* vcos.s of (radians · S703 = 2/π) quarter turns == cos(radians) */
    float c = __builtin_cosf((float)(screen->pulseTimer * 3) * 0.017453292f);
    float t = 1.0f - __builtin_fabsf(c);
    s32 next;

    if (screen->paletteAnimA != NULL) {
        GfxPaletteBlend(t, screen->paletteAnimA, screen->paletteRowA, screen->paletteRowTo);
    }
    if (screen->paletteAnimB != NULL) {
        GfxPaletteBlend(t, screen->paletteAnimB, screen->paletteRowB, screen->paletteRowTo);
    }
    next = 0;
    if (screen->pulseTimer < 0x3c) {
        next = g_gfxDisplay->frameSkip + screen->pulseTimer + 1;
    }
    screen->pulseTimer = next;
}
