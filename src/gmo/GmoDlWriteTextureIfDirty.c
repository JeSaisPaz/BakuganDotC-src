// bdc 0x089dc6d8 GmoDlWriteTextureIfDirty
#include "bdc.h"

/* If bit 0x80000000 (texture) is dirty, writes the current texture `ctx[10]` into the list with
   `GmoTextureWriteDl(tex, &ptr, &remain, 0xffff)`; a zero result marks the list as overflowed. */

void GmoDlWriteTextureIfDirty(GmoDlContext *self, u32 dirty)

{
  int written;
  u32 **dl;
  u32 *local_dl;
  u32 remain;

  if ((dirty & 0x80000000) != 0) {
    local_dl = self->cur;
    dl = (u32 **)0;
    if (self->end != (u32 *)0x0) {
      dl = &local_dl;
    }
    remain = (u32)((uintptr_t)self->end - (uintptr_t)local_dl);
    written = GmoTextureWriteDl(self->texture, dl, &remain, 0xffff);
    self->cur = (u32 *)((u8 *)self->cur + written);
    if (written == 0) {
      self->end = (u32 *)0x0;
    }
  }
  return;
}
