// bdc 0x0890fa78 UiRepairSetupPhase
#include "bdc.h"

/* Phase 1 of the card repair screen. Step 0: creates the sprites of layout 6
   (`UiLayoutCreateSprites`) into the 21-entry sprite table `data`, hides sprites 2, 0 and 15,
   zeroes the alpha of all 21 sprites and the text alpha, sets sprite 14's texture to the
   `"repair_card_%03d"` picture of the repair kind (`UiRepairKindPictureId`, `GfxFindTexture`),
   and copies two lines of `"DMMesRepair_eu.bin"` (`CorePackChainFind`, `UiMesTableRelocate`)
   into `message` (line `UiRepairKindMessageIndex(kind)`) and `detail` (the same index in the second
   half of the table, `count / 2 + index`), then advances `phaseStep`. Any other step: sets the
   active fader from black transparent to black at alpha 0.5, starts a 5-frame fade
   (`GfxFaderStart`), resets `phaseStep` and advances `phase`. */

void UiRepairSetupPhase(UiScreen *screen)

{
  UiRepair *repair = (UiRepair *)screen;
  GfxSprite *picture;
  void *texture;
  void *table;
  u32 *lines;
  s32 count;
  GfxFader *fader;
  int i;
  char name[64];

  if (screen->phaseStep == 0) {
    UiLayoutCreateSprites(screen->spriteLayer, (GfxSprite **)screen->data, 6);
    ((GfxSprite **)screen->data)[2]->flags &= ~1u;
    ((GfxSprite **)screen->data)[0]->flags &= ~1u;
    ((GfxSprite **)screen->data)[15]->flags &= ~1u;
    for (i = 0; i < 21; i++) {
      ((GfxSprite **)screen->data)[i]->alpha = 0.0f;
    }
    repair->alpha = 0.0f;
    sprintf(name, "repair_card_%03d", UiRepairKindPictureId(screen, repair->value));
    picture = ((GfxSprite **)screen->data)[14];
    texture = GfxFindTexture(name);
    picture->texture = texture;
    table = CorePackChainFind(g_ioLzsPackages, "DMMesRepair_eu.bin");
    lines = table;
    count = UiMesTableRelocate(table);
    strcpy(repair->message, (const char *)PspPtr(lines[UiRepairKindMessageIndex(screen, repair->value)]));
    strcpy(repair->detail, (const char *)PspPtr(lines[count / 2 + UiRepairKindMessageIndex(screen, repair->value)]));
    screen->phaseStep = screen->phaseStep + 1;
  }
  else {
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 0.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 0.5f;
    fader = GfxGetActiveFader();
    GfxFaderStart(fader, 5);
    screen->phaseStep = 0;
    screen->phase = screen->phase + 1;
  }
  return;
}
