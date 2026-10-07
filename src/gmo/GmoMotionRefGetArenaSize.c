// bdc 0x08a323c4 GmoMotionRefGetArenaSize
#include "bdc.h"

/* `GmoMotionRef` virtual method (vtable `0x08af5424` entry 9, get arena size): returns the word
   `ref->arenaSize` points at (`*(s32 *)(bin + 0x50)`). */

s32 GmoMotionRefGetArenaSize(GmoMotionRef *ref)

{
  return *ref->arenaSize;
}

