#include "MarkerHandler.h"
#include "Settings.h"

RE::NiPoint3 GetRealPosition(const RE::TESObjectREFR* a_objRef)
{
    RE::NiPoint3 position = a_objRef->GetPosition();

    if (const RE::TESWorldSpace* worldSpace = a_objRef->GetWorldspace())
    {
        RE::NiPoint3 worldSpaceOffset{ worldSpace->worldMapOffsetData.mapOffsetX,
                                              worldSpace->worldMapOffsetData.mapOffsetY,
                                              worldSpace->worldMapOffsetData.mapOffsetZ };
        position += worldSpaceOffset * worldSpace->worldMapOffsetData.mapScale;
    }

    return position;
}

namespace MarkerHandler
{
    bool GetMarker()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        RE::ObjectRefHandle markerRef = REL::Module::IsVR() ? player->GetVRInfoRuntimeData()->playerMapMarker : player->GetInfoRuntimeData().playerMapMarker;

        if (markerRef) {
            return GetPlayerMarker(markerRef);
        }

        return false;
    }

    //Get player-set marker location
    bool GetPlayerMarker(RE::ObjectRefHandle markerRef)
    {
        auto marker = markerRef.get().get();
        if (!marker) { return false; }

        auto markerPos = GetRealPosition(marker);
        //logger::info("x: {}", markerPos.x);

        return MoveTargetMarker(markerPos);
    }

    //Move the player-set marker
    //A custom marker is used so that the AI works even if the player removes the 'Custom Destination' marker
    bool MoveTargetMarker(RE::NiPoint3 markerPos)
    {

        auto ref = RE::TESDataHandler::GetSingleton()->LookupForm(Settings::markerID, Settings::espName)->As<RE::TESObjectREFR>();
        if (!ref) {
            logger::info("No ref found");
            return false;
        }
        else {
            logger::info("Ref found");
            ref->MoveTo(RE::PlayerCharacter::GetSingleton()->AsReference());
            ref->data.location = markerPos;
            ref->Update3DPosition(true);
            return true;
        }

    }
}