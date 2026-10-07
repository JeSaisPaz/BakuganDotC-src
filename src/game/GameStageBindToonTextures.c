// bdc 0x088d41c0 GameStageBindToonTextures
#include "bdc.h"

/* Binds the shading textures: `tex_toon00..02` to slots A 0..2 (`GfxSetTextureSlotA`) and
   `tex_ref00..05`, `figure_refmap` to slots B 0..6 (`GfxSetTextureSlotB`). */

void GameStageBindToonTextures(void)
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
  GfxSetTextureSlotB(GfxFindTexture("figure_refmap"), 6);
}
