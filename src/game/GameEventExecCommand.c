// bdc 0x088ee0a8 GameEventExecCommand
#include "bdc.h"

/* Executes one command of the field event task (task id 470, `GameEvent470Ctor`, base
   `GameEventCtor`; message pack `mes_f<area>_<room>_<lang>.bin`, command script `cmd_f%d_%02d.cut`):
   a `switch` (jump table `0x08a99088`) on opcodes 0x00..0x67 calls the opcode's handler
   (`GameEventOp01Wait` ... `GameEventOp67SetBlocking`) with the event task and, depending on the
   handler, either `(arg, flag)` or `(flag, arg)` (some handlers get `arg != 0`, `(s8)arg`, `(u16)arg`
   or `flag` twice). Opcodes 0x00, 0x07, 0x09, 0x10 and 0x2c do nothing. Any opcode >= 0x68 goes to
   the virtual extended dispatcher (vtable slot 11, `+0x58`; `GameEvent470ExecCommand` for task 470).
   Commands are 4-byte records `{u8 op, u8 flag, s16 arg}`. */

void GameEventExecCommand(GameEvent *self, u8 op, u8 flag, s16 arg)

{
  const VtblEntry *ext;

  switch (op) {
  case 0x00:
    return;
  case 0x01:
    GameEventOp01Wait(self, arg, flag);
    return;
  case 0x02:
    GameEventOp02PlayBgm(self, (u16)arg, flag);
    return;
  case 0x03:
    GameEventOp03StopBgm(self, arg, flag);
    return;
  case 0x04:
    GameEventOp04PlaySe(self, (u16)arg, flag);
    return;
  case 0x05:
    GameEventOp05StopSe(self, (u16)arg, flag);
    return;
  case 0x06:
    GameEventOp06PlayVoice(self, (u16)arg, flag);
    return;
  case 0x07:
    return;
  case 0x08:
    GameEventOp08Nop(self, (u16)arg, flag);
    return;
  case 0x09:
    return;
  case 0x0a:
    GameEventOp0ASetFadeColor(self, (s8)arg, flag);
    return;
  case 0x0b:
    GameEventOp0BFade(self, (u16)arg, flag);
    return;
  case 0x0c:
    GameEventOp0CNop(self, (s8)arg, flag);
    return;
  case 0x0d:
    GameEventOp0DNop(self, (u16)arg, flag);
    return;
  case 0x0e:
    GameEventOp0ENop(self, (s8)arg, flag);
    return;
  case 0x0f:
    GameEventOp0FNop(self, (u16)arg, flag);
    return;
  case 0x10:
    return;
  case 0x11:
    GameEventOp11SetCoordMode(self, arg, flag);
    return;
  case 0x12:
    GameEventOp12CamBegin(self, arg != 0, flag);
    return;
  case 0x13:
    GameEventOp13CamEnd(self, (u8)arg, flag);
    return;
  case 0x14:
    GameEventOp14CamBegin2(self, arg, flag);
    return;
  case 0x15:
    GameEventOp15CamBegin3(self, (u16)arg, flag);
    return;
  case 0x16:
    GameEventOp16SetCamEyeX(self, arg);
    return;
  case 0x17:
    GameEventOp17SetCamEyeY(self, arg, flag);
    return;
  case 0x18:
    GameEventOp18SetCamEyeZ(self, arg, flag);
    return;
  case 0x19:
    GameEventOp19CamEyeTween(self, (u16)arg, flag);
    return;
  case 0x1a:
    GameEventOp1AMoveCamEyeX(self, arg);
    return;
  case 0x1b:
    GameEventOp1BMoveCamEyeY(self, arg);
    return;
  case 0x1c:
    GameEventOp1CMoveCamEyeZ(self, arg);
    return;
  case 0x1d:
    GameEventOp1DCamEyeTween2(self, (u16)arg, flag);
    return;
  case 0x1e:
    GameEventOp1ESetCamTargetX(self, arg, flag);
    return;
  case 0x1f:
    GameEventOp1FSetCamTargetY(self, arg, flag);
    return;
  case 0x20:
    GameEventOp20SetCamTargetZ(self, arg, flag);
    return;
  case 0x21:
    GameEventOp21CamTargetTween(self, (u16)arg, flag);
    return;
  case 0x22:
    GameEventOp22MoveCamTargetX(self, arg, flag);
    return;
  case 0x23:
    GameEventOp23MoveCamTargetY(self, arg, flag);
    return;
  case 0x24:
    GameEventOp24MoveCamTargetZ(self, arg, flag);
    return;
  case 0x25:
    GameEventOp25CamTargetTween2(self, (u16)arg, flag);
    return;
  case 0x26:
    GameEventOp26SetCamFov(self, (u16)arg, flag);
    return;
  case 0x27:
    GameEventOp27CamFovTween(self, (u16)arg, flag);
    return;
  case 0x28:
    GameEventOp28CamResetKeys(self, arg, flag);
    return;
  case 0x29:
    GameEventOp29CamTweenAll(self, (u16)arg, flag);
    return;
  case 0x2a:
    GameEventOp2APropCreate(self, flag, arg);
    return;
  case 0x2b:
    GameEventOp2BPropDelete(self, flag, flag);
    return;
  case 0x2c:
    return;
  case 0x2d:
    GameEventOp2DSetPropX(self, flag, arg);
    return;
  case 0x2e:
    GameEventOp2ESetPropY(self, flag, arg);
    return;
  case 0x2f:
    GameEventOp2FSetPropZ(self, flag, arg);
    return;
  case 0x30:
    GameEventOp30PropMove(self, flag, (u16)arg);
    return;
  case 0x31:
    GameEventOp31MovePropX(self, flag, arg);
    return;
  case 0x32:
    GameEventOp32MovePropY(self, flag, arg);
    return;
  case 0x33:
    GameEventOp33MovePropZ(self, flag, arg);
    return;
  case 0x34:
    GameEventOp34PropMove2(self, flag, (u16)arg);
    return;
  case 0x35:
    GameEventOp35SetPropRotX(self, flag, arg);
    return;
  case 0x36:
    GameEventOp36SetPropRotY(self, flag, arg);
    return;
  case 0x37:
    GameEventOp37SetPropRotZ(self, flag, arg);
    return;
  case 0x38:
    GameEventOp38PropRotate(self, flag, (u16)arg);
    return;
  case 0x39:
    GameEventOp39AddPropRotX(self, flag, arg);
    return;
  case 0x3a:
    GameEventOp3AAddPropRotY(self, flag, arg);
    return;
  case 0x3b:
    GameEventOp3BAddPropRotZ(self, flag, arg);
    return;
  case 0x3c:
    GameEventOp3CPropRotate2(self, flag, (u16)arg);
    return;
  case 0x3d:
    GameEventOp3DMulPropScale(self, flag, (u16)arg);
    return;
  case 0x3e:
    GameEventOp3EDivPropScale(self, flag, (u16)arg);
    return;
  case 0x3f:
    GameEventOp3FPropScale(self, flag, (u16)arg);
    return;
  case 0x40:
    GameEventOp40SetPropMotion(self, arg, flag);
    return;
  case 0x41:
    GameEventOp41SetPropMotionOnce(self, arg != 0, flag);
    return;
  case 0x42:
    GameEventOp42PropPlayMotion(self, flag, (u8)arg);
    return;
  case 0x43:
    GameEventOp43SetPropWait(self, flag, (u16)arg);
    return;
  case 0x44:
    GameEventOp44PropWait(self, flag, (u16)arg);
    return;
  case 0x45:
    GameEventOp45PropStopActions(self, flag, flag);
    return;
  case 0x46:
    GameEventOp46PropNop(self, flag, arg != 0);
    return;
  case 0x47:
    GameEventOp47SetMessageAttr(self, arg, flag);
    return;
  case 0x48:
    GameEventOp48PushMessage(self, flag != 0, arg);
    return;
  case 0x49:
    GameEventOp49Nop(self, arg != 0, flag);
    return;
  case 0x4a:
    GameEventOp4ANop(self, arg, flag);
    return;
  case 0x4b:
    GameEventOp4BNop(self, arg, flag);
    return;
  case 0x4c:
    GameEventOp4CBeginMessages(self, arg, flag);
    return;
  case 0x4d:
    GameEventOp4DWaitMessages(self, arg, flag);
    return;
  case 0x4e:
    GameEventOp4ESetUiFlagA25(self, arg != 0, flag);
    return;
  case 0x4f:
    GameEventOp4FSetUiFlagA27(self, flag, arg);
    return;
  case 0x50:
    GameEventOp50Nop(self, arg != 0, flag);
    return;
  case 0x51:
    GameEventOp51SetUiFlagA26(self, (u8)arg, flag);
    return;
  case 0x52:
    GameEventOp52WaitMessagesAuto(self, arg, flag);
    return;
  case 0x53:
    GameEventOp53Nop(self, flag, flag);
    return;
  case 0x54:
    GameEventOp54Nop(self, flag, (u8)arg);
    return;
  case 0x55:
    GameEventOp55Nop(self, flag, flag);
    return;
  case 0x56:
    GameEventOp56Nop(self, flag, arg);
    return;
  case 0x57:
    GameEventOp57Nop(self, flag, arg);
    return;
  case 0x58:
    GameEventOp58Nop(self, flag, arg);
    return;
  case 0x59:
    GameEventOp59Nop(self, flag, (u16)arg);
    return;
  case 0x5a:
    GameEventOp5ANop(self, flag, arg);
    return;
  case 0x5b:
    GameEventOp5BNop(self, flag, arg);
    return;
  case 0x5c:
    GameEventOp5CNop(self, flag, arg);
    return;
  case 0x5d:
    GameEventOp5DNop(self, flag, (u16)arg);
    return;
  case 0x5e:
    GameEventOp5ENop(self, flag, arg);
    return;
  case 0x5f:
    GameEventOp5FNop(self, flag, arg);
    return;
  case 0x60:
    GameEventOp60Nop(self, flag, arg);
    return;
  case 0x61:
    GameEventOp61Nop(self, flag, (u16)arg);
    return;
  case 0x62:
    GameEventOp62Nop(self, flag, arg);
    return;
  case 0x63:
    GameEventOp63Nop(self, flag, arg);
    return;
  case 0x64:
    GameEventOp64Nop(self, flag, arg);
    return;
  case 0x65:
    GameEventOp65Nop(self, flag, (u16)arg);
    return;
  case 0x66:
    GameEventOp66Nop(self, flag, arg);
    return;
  case 0x67:
    GameEventOp67SetBlocking(self, arg != 0, flag);
    return;
  default:
    /* extended opcodes: virtual ExecCommand, vtable slot 11 (+0x58) */
    ext = &((const VtblEntry *)self->base.vtable)[11];
    ((void (*)(void *, u8, u8, s16))ext->fn)((u8 *)self + ext->delta, op, flag, arg);
    return;
  }
}
