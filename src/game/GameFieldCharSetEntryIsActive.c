// bdc 0x088f50e4 GameFieldCharSetEntryIsActive
#include "bdc.h"

/* Decides whether a character-set entry is present: walks its up to 5 condition slots (`{u16 flag
   +0x18, u8 stop +0x1a, u8 mode +0x1b}` per 4 bytes) and, for the first slot whose story flag is
   set (or the -1 terminator), returns its enable byte; otherwise falls back to the entry's default
   byte `+0x16`. Used by `GameFieldLoadCharacterSet`. */

typedef struct CharSetCond {
  u16 flag;
  s8 stop;
  u8 mode;
} CharSetCond;

typedef struct CharSetEntryView {
  u8 _unk00[0x16];
  u8 defaultEnable;
  u8 _unk17;
  CharSetCond cond[5];
} CharSetEntryView;

bool GameFieldCharSetEntryIsActive(void *entry)
{
  CharSetEntryView *e = (CharSetEntryView *)entry; /* bdc: record-view ok: 0x2c-byte .bin character-set record, scalars only */
  u8 i;

  for (i = 0; i < 5; i++) {
    CharSetCond *c = &e->cond[i];

    if (c->stop == -1) {
      break;
    }
    if (GameEventFlagTest(c->flag) && c->mode != 2) {
      return c->mode != 0;
    }
  }
  return e->defaultEnable != 0;
}
