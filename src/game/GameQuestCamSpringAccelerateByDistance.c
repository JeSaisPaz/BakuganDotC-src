// bdc 0x088fc4c0 GameQuestCamSpringAccelerateByDistance
#include "bdc.h"

/* Runs `GameQuestCamSpringAccelerate` with a smoothing time chosen from the distance between the
   spring's target `goal` and `pos`: `nearTime` below `nearDist`, `farTime` above `farDist`,
   linearly interpolated in between. */

void GameQuestCamSpringAccelerateByDistance(float dt, float nearTime, float farTime, float nearDist, float farDist, GameQuestCamSpring *self, float *pos)
{
  float dx = self->goal.x - pos[0];
  float dy = self->goal.y - pos[1];
  float dz = self->goal.z - pos[2];
  float dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
  float smoothTime;

  if (!(nearDist <= dist)) {
    smoothTime = nearTime;
  } else if (farDist < dist) {
    smoothTime = farTime;
  } else {
    float t = (dist - nearDist) / (farDist - nearDist);
    smoothTime = nearTime * (1.0f - t) + farTime * t;
  }
  GameQuestCamSpringAccelerate(dt, smoothTime, self, pos);
}
