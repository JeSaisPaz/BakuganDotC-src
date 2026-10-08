// bdc 0x088eb708 GameEventUpdateRequestedMessage
#include "bdc.h"

/* Requested-message step of the field event task (wait type 4; step `msgRequestState`, timer
   `msgRequestTimer`): step 0 starts once `msgRequest` is set; step 1 loads the area message pack
   `mes_f%d_%02d_%s.bin` (area `g_gameEventFlags[0]`, room `g_gameEventFlags[2]`, language from
   `SaveGetLanguageName`), takes the message id from the next opcode 0x48 command without moving the
   command pointer, opens a held-open 3-line talk balloon for it (`UiTalkBalloonOpen`; `"ctllx"` when
   the id is out of range), resets `iconPos` and calls vtable slot 9 (show next icon); step 2 releases
   the balloon once it is closed (dead or state 10) and `iconPos` reached
   `g_gameEvent470IconCounts``[msgRequest]`, and on the skip button stops the BGM
   (`SndBgmQueueStop(0.1f, 1)`), shows the next icon and, when both held, moves to step 3; step 3 calls
   vtable slot 10 with the icon count and clears the request. */

void GameEventUpdateRequestedMessage(GameEvent *self)
{
  char name[64];
  u32 *table;
  u32 count;
  s16 msgId;
  u16 savedIndex;
  GameEventCommand *savedCmd;
  const char *text;
  CoreTask *window;
  const VtblEntry *slot;
  s32 closed;
  s32 iconsDone;
  s32 area;
  s32 room;
  s32 step;

  step = self->msgRequestState;
  if (step < 2) {
    if (step < 0) {
      return;
    }
    if (step <= 0) {
      if (self->msgRequest != 0) {
        self->msgRequestState = 1;
      }
      return;
    }
    area = g_gameEventFlags[0];
    room = g_gameEventFlags[2];
    sprintf(name, "mes_f%d_%02d_%s.bin", area, room, SaveGetLanguageName());
    table = CorePackChainFind(g_ioLzsPackages, name);
    savedIndex = self->cmdIndex;
    savedCmd = self->cmd;
    GameEventSkipToOpcode(self, 0x48);
    msgId = self->cmd->arg;
    self->cmdIndex = savedIndex;
    self->cmd = savedCmd;
    count = UiMesTableRelocate(table);
    g_uiTalkBalloonLineCount = 3;
    text = "ctllx";
    if ((s32)msgId < (s32)count) {
      text = (const char *)PspPtr(table[msgId]);
    }
    self->msgWindow = &UiTalkBalloonOpen((void *)text, msgId)->base;
    g_uiTalkBalloonLineCount = 0;
    if (self->msgWindow != NULL) {
      ((UiTalkBalloon *)self->msgWindow)->pageFlags[1] = 1;
      ((UiTalkBalloon *)self->msgWindow)->holdOpen = 1;
    }
    slot = &((const VtblEntry *)self->base.vtable)[9];
    self->iconPos = 0;
    ((void (*)(void *))slot->fn)((u8 *)self + slot->delta);
    self->msgRequestTimer = 0;
    self->msgRequestState = 2;
  }
  else if (step < 3) {
    if (self->msgRequestTimer > 0) {
      self->msgRequestTimer = self->msgRequestTimer - 1;
      return;
    }
    closed = 0;
    iconsDone = 0;
    if (CoreTaskIsAlive(self->msgWindow) == 0) {
      closed = 1;
      self->msgWindow = NULL;
    }
    window = self->msgWindow;
    if (window != NULL && ((UiTalkBalloon *)window)->state == 10) {
      closed = 1;
    }
    if (self->iconPos == (s32)g_gameEvent470IconCounts[self->msgRequest]) {
      iconsDone = 1;
    }
    if (closed && iconsDone && window != NULL) {
      ((UiTalkBalloon *)window)->pageFlags[1] = 0;
      ((UiTalkBalloon *)self->msgWindow)->holdOpen = 0;
    }
    if (GameEventIsSkipPressed() != 0) {
      self->msgRequestTimer = 4;
      SndBgmQueueStop(0.1f, 1);
      slot = &((const VtblEntry *)self->base.vtable)[9];
      ((void (*)(void *))slot->fn)((u8 *)self + slot->delta);
      if (closed && iconsDone) {
        self->msgRequestState = 3;
      }
    }
  }
  else if (step < 4) {
    if (self->msgRequestTimer > 0) {
      self->msgRequestTimer = self->msgRequestTimer - 1;
      return;
    }
    slot = &((const VtblEntry *)self->base.vtable)[10];
    ((void (*)(void *, s32))slot->fn)((u8 *)self + slot->delta,
                                      (s32)g_gameEvent470IconCounts[self->msgRequest]);
    self->msgRequestState = 0;
    self->msgRequest = 0;
  }
}
