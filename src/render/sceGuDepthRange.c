// bdc 0x08a1f190 sceGuDepthRange
#include "bdc.h"

void sceGuDepthRange(s32 nearVal, s32 farVal)
{
  GfxDlWriteDepthRange(g_guCurrentContext, nearVal, farVal);
}
