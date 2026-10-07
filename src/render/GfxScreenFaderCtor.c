// bdc 0x089edf58 GfxScreenFaderCtor
#include "bdc.h"

/* Constructor of the 0x70-byte screen fader (base vtable `0x08af574c`; screen fader vtable
   `0x08af577c`, 0x70 bytes: `+0x0` active, `+0x1` done, `+0x4` duration, `+0x8` elapsed, `+0xc`
   progress, `+0x10` sort key, `+0x20` current RGBA, `+0x30` start RGBA, `+0x40` end RGBA, `+0x60`
   overlay rect): base fader (`GfxFaderBaseCtor`), vtable `g_gfxScreenFaderVtbl`, and a full-screen 480x272
   overlay rect at `+0x60` (`GfxRectCtor`, `GfxRectSetSize`). */

GfxScreenFader * GfxScreenFaderCtor(GfxScreenFader *self)

{
  bool fromLow;
  GfxRect *rect;
  GfxRect *rect_00;
  
  GfxFaderBaseCtor(&self->base);
  (self->base).vtbl = g_gfxScreenFaderVtbl;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  rect = MemAlloc(0x50,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  rect_00 = (GfxRect *)0x0;
  if (rect != (GfxRect *)0x0) {
    GfxRectCtor(rect,0);
    rect_00 = rect;
  }
  self->rect = rect_00;
  GfxRectSetSize(rect_00,0x1e0,0x110);
  return self;
}

