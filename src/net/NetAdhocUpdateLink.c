// bdc 0x089d4d24 NetAdhocUpdateLink
#include "bdc.h"

/* Per-frame link update in the connected state (from `NetAdhocUpdate`): runs the PDP object
   (`NetPdpStep`) and reads its long-silence flag (`NetPdpIsTimedOut`). Lobby mode (`mode` 1):
   a timed-out PDP sets phase 6. Game mode: a missing PDP object stops the link (phase 5). On a
   timeout without `peerConfirmed` it decrements `retriesLeft` and, while some remain, retries with
   phase 6 and shows the netplay status overlay (`NetStatusSetMessage`, `linkShown` = 1);
   otherwise it gives up: phase 5, overlay hidden, `retriesLeft` reset to 3. Without a timeout: if
   the overlay is not shown, the PDP short-silence byte (`shortTimedOut`) shows it; if it is shown,
   it stays while the PDP `packetReceived` is 0 and is hidden otherwise (or with no PDP object); a
   nonzero `packetReceived` also resets `retriesLeft` to 3 and sets `peerConfirmed`. Finally clears
   `g_netAdhoc``->linkFlag1e8` under the connection lock. */

void NetAdhocUpdateLink(NetAdhocConn *self)
{
  bool giveUp;
  bool hide;
  NetPdp *pdp;

  if (self->mode == 1) {
    if (NetPdpExists() != 0) {
      NetPdpStep((NetPdp *)NetPdpGet());
      if (NetPdpIsTimedOut((NetPdp *)NetPdpGet()) != 0) {
        NetAdhocSetPhase(self, 6);
      }
    }
  } else if (NetPdpExists() == 0) {
    NetAdhocSetPhase(self, 5);
  } else {
    NetPdpStep((NetPdp *)NetPdpGet());
    if (NetPdpIsTimedOut((NetPdp *)NetPdpGet()) != 0) {
      giveUp = true;
      if (self->peerConfirmed == 0) {
        self->retriesLeft = self->retriesLeft - 1;
        if (self->retriesLeft > 0) {
          NetAdhocSetPhase(self, 6);
          NetStatusSetMessage(1, 0);
          self->linkShown = 1;
          giveUp = false;
          self->peerConfirmed = 0;
        }
      }
      if (giveUp) {
        NetAdhocSetPhase(self, 5);
        NetStatusSetMessage(0, 0);
        self->linkShown = 0;
        self->retriesLeft = 3;
      }
    } else if (self->linkShown != 0) {
      hide = true;
      if (NetPdpExists() != 0) {
        pdp = (NetPdp *)NetPdpGet();
        if (pdp->packetReceived == 0) {
          hide = false;
        } else {
          if (self->retriesLeft != 3) {
            self->retriesLeft = 3;
          }
          self->peerConfirmed = 1;
        }
      }
      if (hide) {
        NetStatusSetMessage(0, 0);
        self->linkShown = 0;
      }
    } else if (NetPdpExists() != 0) {
      pdp = (NetPdp *)NetPdpGet();
      if (pdp->shortTimedOut != 0) {
        NetStatusSetMessage(1, 0);
        self->linkShown = 1;
      }
    }
  }
  CoreLockAcquire(self->lock);
  g_netAdhoc->linkFlag1e8 = 0;
  CoreLockRelease(self->lock);
}
