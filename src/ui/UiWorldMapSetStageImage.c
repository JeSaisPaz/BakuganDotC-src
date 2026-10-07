// bdc 0x08998c28 UiWorldMapSetStageImage
#include "bdc.h"

/* Sets the stage preview texture of `UiWorldMap` for `area` (0 intro
   `"stage_int"`, 1 Japan, 2 UK, 3 China, 4 Egypt, 5 USA, 6 Vexos `"stage_vex"`, 7 Core): outside
   rank mode a fixed picture per area (`"stage_jp_2"`, `"stage_uk"`, `"stage_ch_3"`,
   `"stage_egy_3"`, `"stage_us_3"`, `"stage_core_ba"`); in rank mode (`UiWorldMapIsRankMode`)
   picks variant `stage` 0..2 (`"stage_jp"`, `"stage_jp_2"`, `"stage_jp_3"`, …, `"stage_core"`).
   An `area` above 7 leaves the sprite untouched. */

void UiWorldMapSetStageImage(UiScreen *screen, GfxSprite *sprite, u8 area, u8 stage)
{
  char name[64];

  if (!UiWorldMapIsRankMode(screen)) {
    switch (area) {
    case 0: sprintf(name, "stage_int"); break;
    case 1: sprintf(name, "stage_jp_2"); break;
    case 2: sprintf(name, "stage_uk"); break;
    case 3: sprintf(name, "stage_ch_3"); break;
    case 4: sprintf(name, "stage_egy_3"); break;
    case 5: sprintf(name, "stage_us_3"); break;
    case 6: sprintf(name, "stage_vex"); break;
    case 7: sprintf(name, "stage_core_ba"); break;
    default: return;
    }
  } else {
    /* A `stage` of 3 or more formats nothing: the original then looks up the
       uninitialised buffer. */
    switch (area) {
    case 0:
      sprintf(name, "stage_int");
      break;
    case 1:
      if (stage == 0) sprintf(name, "stage_jp");
      else if (stage < 2) sprintf(name, "stage_jp_2");
      else if (stage < 3) sprintf(name, "stage_jp_3");
      break;
    case 2:
      if (stage == 0) sprintf(name, "stage_uk");
      else if (stage < 2) sprintf(name, "stage_uk_2");
      else if (stage < 3) sprintf(name, "stage_uk_3");
      break;
    case 3:
      if (stage == 0) sprintf(name, "stage_ch");
      else if (stage < 2) sprintf(name, "stage_ch_2");
      else if (stage < 3) sprintf(name, "stage_ch_3");
      break;
    case 4:
      if (stage == 0) sprintf(name, "stage_egy");
      else if (stage < 2) sprintf(name, "stage_egy_2");
      else if (stage < 3) sprintf(name, "stage_egy_3");
      break;
    case 5:
      if (stage == 0) sprintf(name, "stage_us");
      else if (stage < 2) sprintf(name, "stage_us_2");
      else if (stage < 3) sprintf(name, "stage_us_3");
      break;
    case 6:
      sprintf(name, "stage_vex");
      break;
    case 7:
      sprintf(name, "stage_core");
      break;
    default:
      return;
    }
  }
  sprite->texture = GfxFindTexture(name);
}
