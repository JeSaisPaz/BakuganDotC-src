// bdc 0x0882c2ec UiTalkHideCutInSprites
#include "bdc.h"

/* Hides the cut-in sprites of the talk/HUD task (`UiGetTalkTask`) (id 0x6e, `BtlHudUpdate`)
   (layout sprites `+0x19c` and `+0x39c`, visible bit cleared) and clears the three cut-in state
   words `+0x474..+0x47c`. Used by `BtlMainPhaseTalk`. */

void UiTalkHideCutInSprites(BtlHud *win)

{
  s32 i;
  s32 j;
  
  win->sprites[0x67]->flags &= ~1u;
  i = 0;
  do {
    win->sprites[0xe7 + i]->flags &= ~1u;
    i++;
  } while (i < 1);
  for (j = 0; j < 3; j++) {
    *(u32 *)win->cutInColor[j] = 0;
  }
}
