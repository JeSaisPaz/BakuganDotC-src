// bdc 0x08a1f018 sceGuEnable
#include "bdc.h"

void sceGuEnable(s32 state)
{
  GfxDlWriteEnableDisable(g_guCurrentContext, state, 1);
  g_guEnableMask = g_guEnableMask | (1 << (state & 0x1f));
}
