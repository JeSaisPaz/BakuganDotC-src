// bdc 0x08805598 UiNameEntryConfirmPhase
#include "bdc.h"

/* Phase 4 of the name entry screen (`UiNameEntry`), stepped by `phaseStep`:
   step 0: when `confirmed`, joins the labels of the typed characters (`g_nameEntryKeyTable`) into
   a UTF-8 string, encodes it and stores it as the player name (`SaveProfileSetPlayerName`), sets the
   avatar motion speed to 1.2 and plays motion 9, sets script global word 3 to 1, plays sound 4 and goes
   to step 1; otherwise sets script global word 3 to 0, plays sound 2 and goes to step 2.
   step 1: after 46 frames cancels BGM channel 0 and fades it out over 0.2 s, then step 2.
   step 2: starts a 10-frame fade to opaque black on the active fader (creating the fader slots with
   sort key 20000 if needed), then step 3.
   step 3: when the fade has finished, resets `phaseStep`, advances `phase` and sets `done`.
   Every call ends with `UiNameEntryAnimateKeyPress`. */

void UiNameEntryConfirmPhase(UiNameEntry *self)
{
  GfxModel *avatar;
  const GfxModelVtable *vt;
  GfxFader *fader;
  int i;
  char utf8[25] = {0};
  char prev[25];
  u8 encoded[27];

  switch (self->base.phaseStep) {
  case 0:
    if (self->confirmed != 0) {
      for (i = 0; i < 12; i++) {
        if (self->name[i] == -1) {
          break;
        }
        memcpy(prev, utf8, 0x19);
        sprintf(utf8, "%s%s", prev, g_nameEntryKeyTable[self->name[i]]);
      }
      UiTextEncodeUtf8(encoded, utf8);
      SaveProfileSetPlayerName(SaveGetProfile(), encoded);
      avatar = (GfxModel *)self->avatar;
      vt = (const GfxModelVtable *)avatar->base.vtable;
      vt->setMotionSpeed((u8 *)avatar + vt->setMotionSpeedAdjust, 1.2f);
      UiNameEntryPlayAvatarMotion(self, 9, 0);
      g_scriptGlobalVars[3] = 1;
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 4, 0, 0);
      }
      self->base.phaseStep = 1;
    } else {
      g_scriptGlobalVars[3] = 0;
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      self->base.phaseStep = 2;
    }
    break;
  case 1:
    self->confirmTimer++;
    if (self->confirmTimer >= 0x2e) {
      SndBgmCancelChannel(0);
      SndBgmQueueStop(0.2f, 0);
      self->base.phaseStep = 2;
    }
    break;
  case 2:
    if (!GfxFaderIsReady()) {
      GfxFaderSlotsInit(NULL);
      GfxGetActiveFader()->sortKey = 20000.0f;
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
    self->base.phaseStep = 3;
    break;
  case 3:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.phaseStep = 0;
      self->base.phase++;
      self->done = 1;
    }
    break;
  }
  UiNameEntryAnimateKeyPress(self);
}
