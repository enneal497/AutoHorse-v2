#pragma once

RE::NiPoint3 GetRealPosition(const RE::TESObjectREFR* a_objRef);

namespace MarkerHandler 
{
    bool GetMarker();
    bool GetPlayerMarker(RE::ObjectRefHandle markerRef);
    bool MoveTargetMarker(RE::NiPoint3 markerPos);
}