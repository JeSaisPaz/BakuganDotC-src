// bdc 0x08a1f220 sceGuShadeModel
#include "bdc.h"

void sceGuShadeModel(s32 mode)

{
  GfxDlWriteShademode(g_guCurrentContext, mode);
  return;
}

