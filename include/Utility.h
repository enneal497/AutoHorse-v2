#pragma once

namespace Utility {

    bool ForceRefToAlias(RE::TESQuest* a_quest, uint32_t a_aliasID, RE::TESObjectREFR* a_reference);

    void ShowPrompt();
    void HidePrompt();
    void ForceMountToAlias(RE::ActorPtr a_actor);
}