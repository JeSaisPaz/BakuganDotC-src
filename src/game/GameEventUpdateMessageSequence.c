// bdc 0x088eb9c8 GameEventUpdateMessageSequence
#include "bdc.h"

/* Message-queue step of the field event task (state byte `msgState`; does nothing in skip mode,
   bit 0 of `flags`): state 0 opens the next queued message (`msgIds[msgPos]`, speaker
   `msgAttrs[msgPos]`, while `msgPos < msgCount`) from the area pack `mes_f%d_%02d_%s.bin` (area
   `g_gameEventFlags[0]`, room `g_gameEventFlags[2]`) as a 3-line talk balloon (`UiTalkBalloonOpen`;
   `"ctllx"` when the id is out of range) and goes to 1; 1 goes to 2; 2 (unless `msgAdvanceLock` is 2)
   waits for the skip button, or for the auto-advance countdown `msgAutoTimer` when `msgAutoTime` is
   non-zero, then stops the BGM (`SndBgmQueueStop(0.1f, 1)`), sets `msgCloseDelay` to 4 and goes to 4;
   3 counts `msgIdleTimer` down, wrapping to 200; 4 waits out `msgCloseDelay`, then goes back to 2
   while task 420 (0x1a4) still exists, else to 0 with `msgPos` advanced. */

void GameEventUpdateMessageSequence(GameEvent *self)
{
  char name[64];
  u32 *table;
  u32 count;
  const char *text;
  s32 advance;
  s32 area;
  s32 room;
  s16 idle;

  if ((self->flags & 1) != 0) {
    return;
  }
  switch (self->msgState) {
  case 0:
    if (self->msgPos < self->msgCount) {
      area = g_gameEventFlags[0];
      room = g_gameEventFlags[2];
      sprintf(name, "mes_f%d_%02d_%s.bin", area, room, SaveGetLanguageName());
      table = CorePackChainFind(g_ioLzsPackages, name);
      count = UiMesTableRelocate(table);
      g_uiTalkBalloonLineCount = 3;
      text = "ctllx";
      if ((s32)self->msgIds[self->msgPos] < (s32)count) {
        text = (const char *)(uintptr_t)table[self->msgIds[self->msgPos]];
      }
      self->msgWindow = &UiTalkBalloonOpen((void *)text, self->msgAttrs[self->msgPos])->base;
      g_uiTalkBalloonLineCount = 0;
      self->msgState = 1;
    }
    break;
  case 1:
    self->msgState = 2;
    break;
  case 2:
    if (self->msgAdvanceLock == 2) {
      break;
    }
    advance = 0;
    if (self->msgAutoTime == 0) {
      advance = GameEventIsSkipPressed();
    }
    else if (self->msgAutoTimer >= 0) {
      self->msgAutoTimer = self->msgAutoTimer - 1;
    }
    else {
      self->msgAutoTimer = self->msgAutoTime;
      advance = 1;
    }
    if (advance != 0) {
      self->msgCloseDelay = 4;
      self->msgState = 4;
      SndBgmQueueStop(0.1f, 1);
    }
    break;
  case 3:
    idle = (s16)(self->msgIdleTimer - 1);
    self->msgIdleTimer = idle;
    if (idle < 0) {
      self->msgIdleTimer = 200;
    }
    break;
  case 4:
    if (self->msgCloseDelay >= 0) {
      self->msgCloseDelay = self->msgCloseDelay - 1;
    }
    else if (CoreTaskExists(0x1a4) != 0) {
      self->msgState = 2;
    }
    else {
      self->msgState = 0;
      self->msgPos = self->msgPos + 1;
    }
    break;
  }
}
