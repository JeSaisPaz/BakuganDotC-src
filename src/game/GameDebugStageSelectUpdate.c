// bdc 0x088cd510 GameDebugStageSelectUpdate
#include "bdc.h"

/* Update of the developer stage-select menu (id 501): a 7-step machine. Step 0 waits for the title
   task 0x2774 (if any) to report state 3 through its vtable slot 6, then fades to black; step 1 tells
   that task to close (slot 5); steps 2-3 fade back in; step 4 reads the pad (repeat bits 0x10/0x40
   toggle the cursor, 0x20/0x80 change region or stage depending on the cursor, Start (pressed 0x8)
   advances) and pulses the highlight; steps 5-6 fade out, store region/stage in the save profile and
   script variable 1, set the jet-stage flags up to that stage, set variable 15 to the map id and
   remove the task. The highlight pulse is `0.3 + 0.7 * (1 - cos(pulse * pi)) / 2`. */

void GameDebugStageSelectUpdate(GameDebugStageSelect *self)

{
  void *task;
  const VtblEntry *slot;
  GfxFader *fader;
  PadState *pad;
  u8 profileByte;
  s32 count;
  s32 i;
  s32 j;
  float pulse;

  switch (self->step) {
  case 0:
    task = CoreTaskFind(0x2774);
    if (task == NULL) {
      self->step = self->step + 2;
      break;
    }
    if (!GfxFaderIsFinished(GfxGetActiveFader())) {
      break;
    }
    slot = &((const VtblEntry *)((CoreTask *)task)->vtable)[6];
    if (((s32 (*)(void *, s32))slot->fn)((u8 *)task + slot->delta, 3) != 3) {
      break;
    }
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 0.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 1.0f;
    GfxFaderStart(GfxGetActiveFader(), 10);
    self->step = self->step + 1;
    break;
  case 1:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      task = CoreTaskFind(0x2774);
      if (task != NULL) {
        slot = &((const VtblEntry *)((CoreTask *)task)->vtable)[5];
        ((void (*)(void *, s32, s32))slot->fn)((u8 *)task + slot->delta, 3, 4);
      }
      self->step = self->step + 1;
    }
    break;
  case 2:
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 1.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 0.0f;
    GfxFaderStart(GfxGetActiveFader(), 5);
    self->step = self->step + 1;
    break;
  case 3:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->step = self->step + 1;
    }
    break;
  case 4:
    pad = self->pad;
    if ((pad->repeat & 0x10) != 0) {
      self->cursor = (self->cursor + 1) % 2;
    } else if ((pad->repeat & 0x40) != 0) {
      self->cursor = (self->cursor + 3) % 2;
    } else if ((pad->repeat & 0x20) != 0) {
      if (self->cursor > 0) {
        if (self->cursor < 2) {
          self->stage = (self->stage + g_gameDebugStageCounts[self->region] + 1) %
                        g_gameDebugStageCounts[self->region];
        }
      } else if (self->cursor >= 0) {
        self->stage = 0;
        self->region = (self->region + 10) % 9;
      }
    } else if ((pad->repeat & 0x80) != 0) {
      if (self->cursor > 0) {
        if (self->cursor < 2) {
          self->stage = (self->stage + g_gameDebugStageCounts[self->region] - 1) %
                        g_gameDebugStageCounts[self->region];
        }
      } else if (self->cursor >= 0) {
        self->stage = 0;
        self->region = (self->region + 8) % 9;
      }
    } else if ((pad->pressed & 8) != 0) {
      self->step = self->step + 1;
    }
    pulse = self->pulse + 0.05f;
    self->pulse = pulse;
    self->highlight = (1.0f - __builtin_cosf(pulse * 3.1415927f)) * 0.5f * 0.7f + 0.3f;
    break;
  case 5:
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 0.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 1.0f;
    GfxFaderStart(GfxGetActiveFader(), 5);
    self->step = self->step + 1;
    break;
  case 6:
    if (!GfxFaderIsFinished(GfxGetActiveFader())) {
      break;
    }
    SaveGetProfile()->data->stageProfileByte = (u8)(self->region + (self->region >= 7));
    SaveGetProfile()->data->stageSlot = (u8)self->stage;
    profileByte = SaveGetProfile()->data->stageProfileByte;
    g_scriptGlobalVars[1] = profileByte * 4 + SaveGetProfile()->data->stageSlot;
    SaveProfileSetWord(SaveGetProfile(), 8, 0);
    SaveProfileSetWord(SaveGetProfile(), 9, 0);
    count = g_gameFieldJetStageCount;
    for (i = 0; i < count; i++) {
      if ((s32)g_gameFieldJetStageTable[i][1] == g_scriptGlobalVars[1]) {
        break;
      }
    }
    if (count != i && i >= 0) {
      for (j = 0; j <= i; j++) {
        CoreBitsetSet(g_gameFieldJetStageTable[j][0], g_scriptGlobalBits);
      }
    }
    g_scriptGlobalVars[15] = GameStageToMapId((u32)g_scriptGlobalVars[1]);
    CoreTaskRemove(&self->base, true);
    break;
  }
}
