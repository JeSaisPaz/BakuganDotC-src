// bdc 0x08a00124 ScriptOpFade
#include "bdc.h"

/* Script opcode 0x42 (group 0x40): screen fade control on the active fader (`GfxGetActiveFader`).
   Operands: u16 `cmd`, u32 `value`. cmd 0 starts the fade for `value` frames (`GfxFaderStart`);
   cmd 1 waits for the fade to finish (returns 2 until `GfxFaderIsFinished` says done); cmd 2 sets
   `color` (`+0x20`, RGBA8 in `value` converted to float vec4) and copies it to `start` (`+0x30`);
   cmd 3 sets `end` (`+0x40`); cmd 4 enables/disables the overlay (`GfxFaderSetEnabled`); cmd 5
   stores `value` as float in `sortKey` (`+0x10`); cmd 6 copies the start (value 0) or end (value 1)
   colour into `g_gfxDisplay->clearColor`; cmd 7 calls `GfxFaderSelectSlot(value)`. Other commands
   return 0. If there is no fader (`GfxFaderIsReady() == 0`) it calls `GfxFaderSlotsInit(0)` and
   returns 2. */

/* RGBA8 `value` -> float vec4 (vuc2i.s + vi2f.q scale 2^-31): lane i = ((byte_i * 0x01010101) >> 1)
   / 2^31, i.e. byte / 255 (255 -> 1.0f). */
static void ScriptOpFadeRgba8ToVec4(float *out, u32 value)
{
  int i;

  for (i = 0; i < 4; i++) {
    u32 b = (value >> (i * 8)) & 0xff;
    out[i] = (float)(int)(b * 0x01010101u >> 1) / 2147483648.0f;
  }
}

static void ScriptOpFadeCopyVec4(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

int ScriptOpFade(Script *script)
{
  u32 cmd;
  u32 value;
  int result;
  GfxFader *fader;
  GfxFader *src;
  GfxDisplay *display;

  cmd = ScriptReadU16(script);
  value = ScriptReadU32(script);
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit(NULL);
    return 2;
  }
  result = 0;
  switch (cmd) {
  case 0:
    GfxFaderStart(GfxGetActiveFader(), (s32)value);
    break;
  case 1:
    if (!GfxFaderIsFinished(GfxGetActiveFader())) {
      result = 2;
    }
    break;
  case 2:
    ScriptOpFadeRgba8ToVec4(GfxGetActiveFader()->color, value);
    fader = GfxGetActiveFader();
    src = GfxGetActiveFader();
    ScriptOpFadeCopyVec4(fader->start, src->color);
    break;
  case 3:
    ScriptOpFadeRgba8ToVec4(GfxGetActiveFader()->end, value);
    break;
  case 4:
    GfxFaderSetEnabled(GfxGetActiveFader(), value != 0);
    break;
  case 5:
    GfxGetActiveFader()->sortKey = (float)value;
    break;
  case 6:
    if ((s32)value > 0) {
      if ((s32)value < 2) {
        display = g_gfxDisplay;
        ScriptOpFadeCopyVec4(display->clearColor, GfxGetActiveFader()->end);
      }
    } else if ((s32)value >= 0) {
      display = g_gfxDisplay;
      ScriptOpFadeCopyVec4(display->clearColor, GfxGetActiveFader()->start);
    }
    break;
  case 7:
    GfxFaderSelectSlot((int)value);
    break;
  }
  return result;
}
