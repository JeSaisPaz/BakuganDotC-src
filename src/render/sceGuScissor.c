// bdc 0x08a1f31c sceGuScissor
#include "bdc.h"

void sceGuScissor(s32 x, s32 y, s32 w, s32 h)
{
  GfxDlWriteScissorRegion(g_guCurrentContext, x, y, w, h);
}
