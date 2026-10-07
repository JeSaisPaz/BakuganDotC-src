// bdc 0x08a1f1f8 sceGuFrontFace
#include "bdc.h"

void sceGuFrontFace(s32 order)

{
  GfxDlWriteCull(g_guCurrentContext, order);
  return;
}

