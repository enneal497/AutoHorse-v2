#include "Events.h"
#include "Utility.h"
#include "InputEventHandler.h"

void SetMarkerDiscovered(RE::TESObjectREFR* marker) {

    auto data = marker ? marker->extraList.GetByType<RE::ExtraMapMarker>() : nullptr;
    if (data && data->mapData) {
        data->mapData->flags.set(RE::MapMarkerData::Flag::kCanTravelTo);
        data->mapData->flags.set(RE::MapMarkerData::Flag::kVisible);
    }
    else {
        logger::info("No map marker data found");
        return;
    }
}

void checkMarkers() {
    auto player = RE::PlayerCharacter::GetSingleton();
    auto& markers = player->GetPlayerRuntimeData().currentMapMarkers;

    float sDist = 4000.0f;
    if (markers.size() == 0) {
        return;
    }

    for (int i = markers.size() - 1; i >= 0; i--) {

        if (!markers[i]) {
            logger::info("Marker not found");
            continue;
        }

        auto marker = markers[i].get().get();
        auto distance = marker->GetDistance(player);
        //logger::info("Distance: {}", distance);
        if (distance < sDist) {
            auto data = marker->extraList.GetByType<RE::ExtraMapMarker>();
            if (!(data->mapData) || data->mapData->flags.all(RE::MapMarkerData::Flag::kCanTravelTo)) {
                logger::info("Marker discovered");
                continue;
            }
            SetMarkerDiscovered(marker);
        }
    }
}

namespace Events 
{
    RE::BSEventNotifyControl ActivateEventHandler::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>* a_eventSource) noexcept
    {
        UNREFERENCED_PARAMETER(a_eventSource);
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }
        if (a_event->actionRef.get() != RE::PlayerCharacter::GetSingleton()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (a_event->objectActivated.get()->GetBaseObject()->HasKeywordByEditorID("ActorTypeHorse")) {
            logger::info("Player activated horse");
            Utility::ShowPrompt();
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl CellEventHandler::ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>* a_eventSource) noexcept
    {
        UNREFERENCED_PARAMETER(a_eventSource);
        if (!a_event || !InputEventHandler::GetSingleton()->IsRunning()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto actor = a_event->actor.get().get();
        if (actor == RE::PlayerCharacter::GetSingleton()) {
            logger::info("Player changed cell");
            checkMarkers();
        }

        return RE::BSEventNotifyControl::kContinue;
    }
}