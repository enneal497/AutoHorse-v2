#include "InputEventHandler.h"
#include "Hooks.h"
#include "PCH.h"

namespace Hooks {

    float UpdateRotation_Hook(RE::Actor* actor)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        if (InputEventHandler::IsRunning() && player->GetActorRuntimeData().movementController.get()->GetAIDriven()) {
            return 0.0;
        }
        return UpdateRotationF(actor);
    }

    float GetAngleX_Hook(RE::Character* a_character)
    {
        //logger::info("Replacing rotation data for {} with player data", a_character->formID);
        if (InputEventHandler::IsRunning()) {
            return RE::PlayerCharacter::GetSingleton()->data.angle.x;
        }
        else {
            return GetAngleF(a_character);

        }
    }

    void HandleRotationX_Hook(RE::Character* character, float rot)
    {
        if (InputEventHandler::IsRunning()) {
            character->data.angle.x = rot + RE::PlayerCharacter::GetSingleton()->GetAngleX();
        }
        else {
            HandleRotationF(character, rot);
        }
    }

    void Install()
    {
        //HorseCameraState::UpdateRotation()
        REL::Relocation<uintptr_t> UpdateRotation{ REL::RelocationID(49840, 50771) };
        //HorseCameraState::HandleLookInput()
        REL::Relocation<uintptr_t> GetAngleX{ REL::RelocationID(49839, 50770) };
        //PlayerCamera::sub_*
        REL::Relocation<uintptr_t> HandleRotationX{ REL::RelocationID(49907, 50840) };

        auto& trampoline = SKSE::GetTrampoline();

        //Line 30 - Actor::sub()
        UpdateRotationF = trampoline.write_call<5>(UpdateRotation.address() + 132, &UpdateRotation_Hook);
        //Line 28 - Character::GetRotationX()
        GetAngleF = trampoline.write_call<5>(GetAngleX.address() + 140, &GetAngleX_Hook);
        //Line 59 - PlayerCharacter::ModifyRotationX()
        HandleRotationF = trampoline.write_call<5>(HandleRotationX.address() + 516, &HandleRotationX_Hook);
    }
}