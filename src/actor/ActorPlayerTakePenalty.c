// bdc 0x088e1494 ActorPlayerTakePenalty
#include "bdc.h"

/* Subtracts 300 from the profile's point counter (clamped to 0..9999999) when it is positive and
   plays the penalty sound `0x2c00036` (handle `penaltySound`). Called when a guard catches the
   player and by two event handlers. */

void ActorPlayerTakePenalty(ActorPlayer *self)
{
  SaveProfile *profile;
  int points;
  int newPoints;

  profile = SaveGetProfile();
  if (0 < profile->data->points) {
    profile = SaveGetProfile();
    points = profile->data->points - 300;
    newPoints = 9999999;
    if ((points < 10000000) && (newPoints = points, points < 0)) {
      newPoints = 0;
    }
    profile->data->points = newPoints;
    if (SndHasManager()) {
      self->penaltySound = SndManagerPlay(SndGetManager(), 0x2c00036, 0, 0);
    }
  }
}
