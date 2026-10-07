// bdc 0x0880c6dc SaveProfileHolderCtor
#include "bdc.h"

/* Constructs the 0xc-byte profile holder that `g_playerProfile` points at: `+0` save block
   pointer = NULL (attached later by `SaveProfileAttachBlock`), `+4` = new 300-byte word table,
   `+8` = new 4-byte language word (both from the low heap); then clears the record area
   (`SaveProfileClearRecordArea`, a no-op while no block is attached) and initialises the language
   (`SaveProfileInitLanguage`). Returns `holder`. */

SaveProfile *SaveProfileHolderCtor(SaveProfile *self)

{
  bool savedLow;
  u32 *words;
  s32 *lang;
  
  self->data = (SaveProfileData *)0x0;
  MemLock();
  savedLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  words = MemAlloc(300,(char *)0x0,0);
  MemSetAllocFromLow(savedLow);
  MemUnlock();
  self->words = words;
  SaveProfileClearRecordArea(self);
  MemLock();
  savedLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  lang = MemAlloc(4,(char *)0x0,0);
  MemSetAllocFromLow(savedLow);
  MemUnlock();
  self->language = lang;
  SaveProfileInitLanguage(self);
  return self;
}

