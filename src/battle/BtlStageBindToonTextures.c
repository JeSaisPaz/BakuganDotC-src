// bdc 0x0889d7c0 BtlStageBindToonTextures
#include "bdc.h"

/* Binds the arena's shading textures: `tex_toon00..02` to toon slots 0..2 (`GfxSetTextureSlotA`)
   and `tex_ref00..05` to reflection slots 0..5 (`GfxSetTextureSlotB`), each looked up with
   `GfxFindTexture`. Called by `BtlStageLoadMap`. */

void BtlStageBindToonTextures(void)
{
    GfxSetTextureSlotA(GfxFindTexture("tex_toon00"), 0);
    GfxSetTextureSlotA(GfxFindTexture("tex_toon01"), 1);
    GfxSetTextureSlotA(GfxFindTexture("tex_toon02"), 2);
    GfxSetTextureSlotB(GfxFindTexture("tex_ref00"), 0);
    GfxSetTextureSlotB(GfxFindTexture("tex_ref01"), 1);
    GfxSetTextureSlotB(GfxFindTexture("tex_ref02"), 2);
    GfxSetTextureSlotB(GfxFindTexture("tex_ref03"), 3);
    GfxSetTextureSlotB(GfxFindTexture("tex_ref04"), 4);
    GfxSetTextureSlotB(GfxFindTexture("tex_ref05"), 5);
}
