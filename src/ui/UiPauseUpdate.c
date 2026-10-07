// bdc 0x08910488 UiPauseUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the pause menu (task id 410). When profile flag 0 is set, a
   net pad exists and no close is pending: outside a NetPlay session it points `pad`/`netPad` (and
   those of an open task 510 dialog) at `g_padState`; in a session, phase 0 resets the net sync,
   phase 2 waits for the lock-step handshake and only runs the phase once a remote pad record
   (`NetCharaReadSlot`) was fed into `netPad`. When the profile has flags 0x4880 during phase 2
   step 3 it sets `leaving`, starts a 10-frame fade to black, sets menu result 4 and jumps to
   step 5. Then runs the phase handler from `g_uiPausePhaseTable` and `UiScreenUpdateCommon`. */

void UiPauseUpdate(UiPause *self)
{
  bool runPhase = true;

  if (SaveGetProfileFlag0() != 0 && self->base.netPad != NULL &&
      self->base.closeRequested == 0) {
    if (NetPlayHasManager()) {
      s32 phase = self->base.phase;

      if (phase > 0) {
        if (phase == 2) {
          runPhase = false;
          if (NetPlayHasManager() && NetCharaIsSyncHandshakeDone() != 0) {
            PadSetExternalState(self->base.netPad, NULL, NULL);
          }
        } else {
          runPhase = true;
        }
      } else if (phase < 0) {
        runPhase = true;
      } else {
        NetPlayClearFlags(NetPlayGetManager(), 0x1000000);
        NetPlaySetFlags(NetPlayGetManager(), 0x8000000);
        NetCharaResetAllSync();
        runPhase = true;
      }
      if (!runPhase && NetPlayIsSynced(NetPlayGetManager())) {
        u32 record[10]; /* NetChara slot record: masks at +0x8, stick at +0x10 */
        NetChara *chara = NetCharaGetByIndex(0);

        if (chara != NULL && NetCharaReadSlot(chara, 0, record)) {
          PadSetExternalState(self->base.netPad, (u16 *)&record[2], (float *)&record[4]);
          runPhase = true;
        }
      }
    } else {
      PadState *pad = g_padState;
      UiScreen *dialog;

      self->base.pad = pad;
      self->base.netPad = pad;
      dialog = (UiScreen *)CoreTaskFind(0x1fe);
      if (dialog != NULL) {
        pad = self->base.pad;
        dialog->pad = pad;
        dialog->netPad = pad;
      }
    }
    if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x4880) &&
        self->base.phase == 2 && self->base.phaseStep == 3) {
      GfxFader *fader;

      self->leaving = 1;
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
      UiSetMenuResult(&self->base, 4);
      self->base.phaseStep = 5;
    }
  }
  if (runPhase) {
    s32 phase = self->base.phase;

    if (phase >= 0 && phase < 7) {
      const MemberFnPtr *member = &g_uiPausePhaseTable[phase];
      u8 *obj = (u8 *)self + member->delta;
      void *fn = member->pfn;

      if (member->index != 0) {
        const VtblEntry *entry =
            &(*(const VtblEntry **)(obj + (uintptr_t)member->pfn))[member->index];

        fn = entry->fn;
        obj += entry->delta;
      }
      ((void (*)(void *))fn)(obj);
    }
    UiScreenUpdateCommon(&self->base);
  }
}
