// bdc 0x088113d8 ScriptOpSetStageObjPose
#include "bdc.h"

/* Script opcode: finds a stage object (`ActorStageObjFindByInstanceId`) and, when the camera
   task exists and the object was found, either (mode 0) snaps it to the ground below the operand
   position (`CollisionRaycastPoint` into its position) and sets its heading `rot[1]` to
   `pi/2 - yaw` (yaw from degrees) wrapped into (-pi, pi], or (mode 1) sets its HP
   (`ActorStageObjSetHp`, first float) and, when that HP is 0, calls its vtable entry 11.
   Other modes do nothing. Always returns 0. */

int ScriptOpSetStageObjPose(Script *script)
{
  s32 mode;
  u32 instanceId;
  ActorStageObjBase *obj;
  float x;
  float y;
  float z;
  float heading;
  const VtblEntry *vt;
  float pos[4] __attribute__((aligned(16)));

  mode = (s32)ScriptReadU16(script);
  instanceId = ScriptReadU16(script);
  x = ScriptReadFloat(script);
  y = ScriptReadFloat(script);
  z = ScriptReadFloat(script);
  pos[3] = ScriptReadFloat(script) * 0.0174532924f;
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;
  obj = ActorStageObjFindByInstanceId(instanceId);
  if (BtlCameraTaskExists() != 0 && obj != NULL) {
    if (mode < 1) {
      if (mode >= 0) {
        CollisionRaycastPoint(pos, obj->base.pos);
        heading = 1.57079637f - pos[3];
        if (!(heading <= 3.14159274f)) {
          heading = heading - 6.28318548f;
        } else if (heading <= -3.14159274f) {
          heading = heading + 6.28318548f;
        }
        obj->base.rot[1] = heading;
      }
    } else if (mode < 2) {
      ActorStageObjSetHp(pos[0], obj);
      if (pos[0] == 0.0f) {
        vt = (const VtblEntry *)obj->base.base.vtable + 11;
        ((void (*)(void *))vt->fn)((u8 *)obj + vt->delta);
      }
    }
  }
  return 0;
}
