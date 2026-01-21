#include "InputEventHandler.h"
#include "Settings.h"
#include "Events.h"
#include "Hooks.h"

bool bPluginAllowed = true;

void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    if (!bPluginAllowed) {
        return;
    }
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {

        auto file = RE::TESDataHandler::GetSingleton()->LookupModByName("AutoHorse.esp");
        if (!file) {
            logger::critical("AutoHorse.esp not found. Aborting...");
            bPluginAllowed = false;
            return;
        }

        Settings::LoadSettings();
        Hooks::Install();

        InputEventHandler::Register();
        Events::ActivateEventHandler::Register();
        Events::CellEventHandler::Register();
        logger::info("Finished initialisation");
        logger::info("");
    }
    else if (message->type == SKSE::MessagingInterface::kSaveGame) {
        InputEventHandler::GetSingleton()->ForceStopAutopilot();
        return;
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);
    SKSE::AllocTrampoline(42);

	const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto name{ plugin->GetName() };
    const auto version{ plugin->GetVersion() };

    logger::init();
    logger::info("{} {} is loading...", name, version);

    if (const auto messaging{ SKSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener)) {
        return false;
    }

    logger::info("{} has finished loading.", name);
    logger::info("");

	return true;
}
