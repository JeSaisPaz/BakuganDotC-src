// bdc 0x088cc818 GameFieldCameraAimViewPitch
#include "bdc.h"

/* Tilts the aim direction `dir` (`+0x30`) of the field camera's throw aim helper
   (`GameFieldCameraAimView`, `cam+0x400`) by ±0.0314 rad per frame with the analog stick Y
   (`g_padState`, beyond ±0.8; 0 when the player's `+0x4a8` lock byte is set): rotates `dir` about
   Z by that angle and stores the result back only when its |x| is not <= 0.62 (clamps the pitch). */

typedef struct {
  u8 pad[0x4a8];
  u8 aimLock;
} GameFieldAimPitchPlayer;

void GameFieldCameraAimViewPitch(GameFieldCameraAimView *aim)

{
  GameFieldAimPitchPlayer *player;
  float rotated[4];
  float stickY;
  float angle;
  float c;
  float s;
  float t0, t1, t2, t3;

  stickY = g_padState->stickY;
  angle = 0.0f;
  if (stickY < -0.8f) {
    angle = 0.03141593f;
  } else if (!(stickY <= 0.8f)) {
    angle = -0.03141593f;
  }
  player = (GameFieldAimPitchPlayer *)ActorFindPlayer();
  if (player->aimLock != 0) {
    angle = 0.0f;
  }
  /* vrot.q of angle * S703 (2/π): columns (c, s, 0, 0), (-s, c, 0, 0), then identity columns
     (0, 0, 1, 0), (0, 0, 0, 1); vtfm4.q E100 gives d = Σ_k dir[k] · column_k. */
  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
  t0 = aim->dir[0];
  t1 = aim->dir[1];
  t2 = aim->dir[2];
  t3 = aim->dir[3];
  rotated[0] = c * t0 + -s * t1 + 0.0f * t2 + 0.0f * t3;
  rotated[1] = s * t0 + c * t1 + 0.0f * t2 + 0.0f * t3;
  rotated[2] = 0.0f * t0 + 0.0f * t1 + 1.0f * t2 + 0.0f * t3;
  rotated[3] = 0.0f * t0 + 0.0f * t1 + 0.0f * t2 + 1.0f * t3;
  if (!(fabsf(rotated[0]) <= 0.62f)) {
    aim->dir[0] = rotated[0];
    aim->dir[1] = rotated[1];
    aim->dir[2] = rotated[2];
    aim->dir[3] = rotated[3];
  }
}
