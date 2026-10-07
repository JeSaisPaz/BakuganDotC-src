// bdc 0x0882c524 UiTalkUsesNarrowIndent
#include "bdc.h"

/* Returns 1 when the talk/HUD task (`UiGetTalkTask`) (id 0x6e, `BtlHudUpdate`)'s current window
   kind `windowKind` is 6–0xb or 0xf; `UiTalkLayoutText` then indents the text by 0x40 instead of
   0x78 (no face portrait, presumably). */

s32 UiTalkUsesNarrowIndent(void *win)
{
  s32 narrow = 0;
  switch (((UiTalkTask *)win)->windowKind) {
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 0xb:
  case 0xf:
    narrow = 1;
  }
  return narrow;
}
