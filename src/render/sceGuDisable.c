// bdc 0x08a1f070 sceGuDisable
#include "bdc.h"

void sceGuDisable(s32 state)
{
  GfxDlWriteEnableDisable(g_guCurrentContext, state, 0);
  g_guEnableMask = g_guEnableMask & ~(1 << (state & 0x1f));
}
