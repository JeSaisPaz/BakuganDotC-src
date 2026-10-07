// bdc 0x0891a87c UiAdvSelectMarkChosen
#include "bdc.h"

/* Records the chosen candidate: sets the story event flag whose id (low 16 bits) is
   `g_advSelectEventFlagIds[cursor]` in the bitset at `g_gameEventFlags + 8`, and sets bit 0 of the
   word at `g_gameEventFlags + 0x7c`. */

void UiAdvSelectMarkChosen(UiAdvSelect *self)
{
  u32 id;
  u32 *bits;

  id = g_advSelectEventFlagIds[self->cursor] & 0xffff;
  bits = (u32 *)&g_gameEventFlags[8];
  bits[(s32)id >> 5] |= 1u << (id & 0x1f);
  *(u32 *)&g_gameEventFlags[0x7c] |= 1;
}
