#pragma once

class InputEventHandler final : public RE::BSTEventSink<RE::InputEvent*> {
public:

    static InputEventHandler* GetSingleton();

    static void Register();

    static bool IsRunning();
    static void ForceStopAutopilot();

    RE::NiPointer<RE::Actor> mount;
    static inline RE::TESQuest* controlQuest;

private:
    RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;

    InputEventHandler() = default;
    InputEventHandler(const InputEventHandler&) = delete;
    InputEventHandler(InputEventHandler&&) = delete;
    ~InputEventHandler() override = default;

    static void StartAutopilot();
    static void StopAutopilot(bool dismount);

    static inline RE::TESGlobal* g_isReady;
    static inline RE::TESGlobal* g_tutorial;
    static inline RE::TESGlobal* g_speed;

    static inline bool isPaused;
    static inline bool isActive;

    static inline bool isWalking{ false };
    static inline bool isSprinting{ false };

    InputEventHandler& operator=(const InputEventHandler&) = delete;
    InputEventHandler& operator=(InputEventHandler&&) = delete;

};