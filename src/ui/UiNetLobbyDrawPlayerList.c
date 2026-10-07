// bdc 0x08941f40 UiNetLobbyDrawPlayerList
#include "bdc.h"

/* Redraws the player/host list text box of the ad-hoc network lobby screen (task 2000,
   `UiNetLobbyCtor`, created by the network menu task 1999). Does nothing (returns 0) without a
   text box; before the text printer reaches step 2 it only fades the list out. Otherwise, with no
   NetPlay peers it shows the mode tab panel (SetVisible on the two `UiNetLobbySlideAnim` panels:
   join mode = panel 0, host mode = panel 1) and fades out. With peers it shows the tab, then for
   each row (join: 4 rows from y 0x4b, 24 px apart; host: 2 rows from y 0x75, 29 px apart) prints,
   when `visible`, the row number (`"%d"`) and, for rows below the peer count, the peer name; in
   join mode it also looks the peer's MAC up in the received net character headers 1..15
   (`NetCharaGetRecvHeader`) and prints `"<peerCount> / 2"` for the last match. The highlight
   rectangle is moved to the row `joinCursor`. With `showCursor` the list alpha fades in
   (`fadeIn` is forced to 1 when the alpha was 0), otherwise it fades out, by 0.04 per frame.
   Returns 1 when the fade-out reached 0 or the fade-in alpha is non-zero, else 0;
   `UiNetLobbyPulseHighlight` is called with `fadeIn` as its reset flag in every case. */

bool UiNetLobbyDrawPlayerList(UiNetLobby *self, bool visible, bool joinMode, bool showCursor, bool fadeIn)
{
  char line[32];
  u8 encoded[64];
  NetCharaPacketHeader hdr;
  NetPlayPeer *peer;
  const VtblEntry *entry;
  s32 state;
  s32 peerCount;
  s32 rows;
  s32 y;
  s32 i;
  s32 idx;
  s32 found;
  float alpha;
  u8 done;

  done = 0;
  state = 0;
  if (self->textBox != NULL) {
    state = -1;
    if (self->printerStep >= 2) {
      peerCount = 0;
      if (NetPlayHasManager()) {
        peerCount = NetPlayGetPeerCount((NetPlay *)NetPlayGetManager());
      }
      if (peerCount == 0) {
        state = -1;
        if (joinMode) {
          entry = &((const VtblEntry *)self->panels[0]->vtable)[5];
          ((void (*)(void *, u8))entry->fn)((u8 *)self->panels[0] + entry->delta, 1);
          entry = &((const VtblEntry *)self->panels[1]->vtable)[5];
          ((void (*)(void *, u8))entry->fn)((u8 *)self->panels[1] + entry->delta, 0);
        }
        else {
          entry = &((const VtblEntry *)self->panels[0]->vtable)[5];
          ((void (*)(void *, u8))entry->fn)((u8 *)self->panels[0] + entry->delta, 0);
          entry = &((const VtblEntry *)self->panels[1]->vtable)[5];
          ((void (*)(void *, u8))entry->fn)((u8 *)self->panels[1] + entry->delta, 1);
        }
      }
      else {
        y = 0x4b;
        rows = 4;
        if (!joinMode) {
          rows = 2;
          y = 0x75;
          entry = &((const VtblEntry *)self->panels[0]->vtable)[5];
          ((void (*)(void *, u8))entry->fn)((u8 *)self->panels[0] + entry->delta, 0);
          entry = &((const VtblEntry *)self->panels[1]->vtable)[5];
          ((void (*)(void *, u8))entry->fn)((u8 *)self->panels[1] + entry->delta, 1);
        }
        else {
          entry = &((const VtblEntry *)self->panels[0]->vtable)[5];
          ((void (*)(void *, u8))entry->fn)((u8 *)self->panels[0] + entry->delta, 1);
          entry = &((const VtblEntry *)self->panels[1]->vtable)[5];
          ((void (*)(void *, u8))entry->fn)((u8 *)self->panels[1] + entry->delta, 0);
        }
        for (i = 0; i < rows; i++) {
          if (visible) {
            memset(line, 0, sizeof(line));
            memset(encoded, 0, sizeof(encoded));
            sprintf(line, "%d", i + 1);
            UiTextEncodeSjis(encoded, line);
            UiTextBoxPrint((UiTextBox *)self->textBox, 0x5e, y, (char *)encoded, 0, 0);
            if (i < peerCount) {
              peer = (NetPlayPeer *)NetPlayGetPeer((NetPlay *)NetPlayGetManager(), i);
              UiTextBoxPrint((UiTextBox *)self->textBox, 0x7e, y, (char *)peer->info, 0, 0);
              if (joinMode) {
                found = -1;
                for (idx = 1; idx < 0x10; idx++) {
                  if (NetCharaGetRecvHeader(&hdr, idx) == 0) {
                    break;
                  }
                  if (hdr.mac[0] == peer->mac[0] && hdr.mac[1] == peer->mac[1] &&
                      hdr.mac[2] == peer->mac[2] && hdr.mac[3] == peer->mac[3] &&
                      hdr.mac[4] == peer->mac[4] && hdr.mac[5] == peer->mac[5]) {
                    found = hdr.peerCount;
                  }
                }
                if (found >= 0) {
                  memset(line, 0, sizeof(line));
                  memset(encoded, 0, sizeof(encoded));
                  sprintf(line, "%d / %d", found, 2);
                  UiTextEncodeSjis(encoded, line);
                  UiTextBoxPrint((UiTextBox *)self->textBox, 0x14e, y, (char *)encoded, 0, 0);
                }
              }
            }
          }
          if (self->joinCursor == i) {
            ((GfxRect *)self->highlightRect)->pos[0] = 90.0f;
            ((GfxRect *)self->highlightRect)->pos[1] = (float)y;
          }
          y += 0x18;
          if (!joinMode) {
            y += 5;
          }
        }
        if (showCursor) {
          if (self->listAlpha == 0.0f) {
            fadeIn = 1;
          }
          state = 1;
        }
        else {
          state = -1;
        }
      }
    }
  }

  if (state < 0) {
    alpha = self->listAlpha;
    if (!(alpha <= 0.0f)) {
      alpha = alpha - 0.04f;
      self->listAlpha = alpha;
    }
    if (alpha < 0.0f) {
      self->listAlpha = 0.0f;
      alpha = 0.0f;
    }
    if (alpha == 0.0f) {
      done = 1;
    }
  }
  else if (state > 0) {
    alpha = self->listAlpha;
    if (alpha < 1.0f) {
      alpha = alpha + 0.04f;
      self->listAlpha = alpha;
    }
    if (!(alpha <= 1.0f)) {
      self->listAlpha = 1.0f;
      alpha = 1.0f;
    }
    if (alpha != 0.0f) {
      done = 1;
    }
  }
  UiNetLobbyPulseHighlight(self, fadeIn);
  return done;
}
