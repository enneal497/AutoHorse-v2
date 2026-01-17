#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <CLIBUtil/utils.hpp>

using namespace std::literals;

namespace logger = SKSE::log;
namespace string = clib_util::string;

template <typename TDerived, typename TEvent>
class EventHandler : public RE::BSTEventSink<TEvent>
{
protected:
    constexpr EventHandler() noexcept = default;
    constexpr ~EventHandler() noexcept override = default;

public:
    constexpr EventHandler(const EventHandler&) = delete;
    constexpr EventHandler(EventHandler&&) = delete;
    constexpr auto operator=(const EventHandler&) = delete;
    constexpr auto operator=(EventHandler&&) = delete;

    [[nodiscard]] static constexpr auto Get() noexcept
    {
        static TDerived handler;
        return std::addressof(handler);
    }

    static constexpr auto Deregister() noexcept
    {
        using TEventSource = RE::BSTEventSource<TEvent>;

        const std::string dirty_name{ typeid(TEvent).name() };
        const std::regex  p{ "class |struct |RE::|SKSE::| * __ptr64" };
        const auto        name{ std::regex_replace(dirty_name, p, "") };

        if constexpr (std::is_base_of_v<TEventSource, RE::ScriptEventSourceHolder>) {
            const auto holder{ RE::ScriptEventSourceHolder::GetSingleton() };
            holder->RemoveEventSink(Get());
            logger::info("Removed {} handler", name);
        }
    }

    static constexpr auto Register() noexcept
    {
        using TEventSource = RE::BSTEventSource<TEvent>;

        const std::string dirty_name{ typeid(TEvent).name() };
        const std::regex  p{ "class |struct |RE::|SKSE::| * __ptr64" };
        const auto        name{ std::regex_replace(dirty_name, p, "") };

        if constexpr (std::is_same_v<TEvent, SKSE::ActionEvent>) {
            SKSE::GetActionEventSource()->AddEventSink(Get());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_base_of_v<TEventSource, RE::ScriptEventSourceHolder>) {
            const auto holder{ RE::ScriptEventSourceHolder::GetSingleton() };
            holder->AddEventSink(Get());
            logger::info("Registered {} handler", name);
            return;
        }
        else if (name == "BGSActorCellEvent") {
            const auto holder{ RE::PlayerCharacter::GetSingleton()->AsBGSActorCellEventSource() };
            holder->AddEventSink(Get());
            logger::info("Registered {} handler", name);
            return;
        }
        const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
        const std::string errmessg = std::format("{}: Failed to register {} handler", plugin->GetName(), name);
        SKSE::stl::report_and_fail(errmessg);
    }
};

namespace stl 
{
    using namespace SKSE::stl;

    namespace detail
    {
        template <typename>
        struct is_chrono_duration : std::false_type
        {
        };

        template <typename Rep, typename Period>
        struct is_chrono_duration<std::chrono::duration<Rep, Period>> : std::true_type
        {
        };

        template <typename T>
        concept is_duration = is_chrono_duration<T>::value;
    }

    auto add_thread_task(const std::function<void()>& a_fn, const detail::is_duration auto a_wait_for) noexcept
    {
        std::jthread{ [=] {
            std::this_thread::sleep_for(a_wait_for);
            SKSE::GetTaskInterface()->AddTask(a_fn);
        } }.detach();
    }
}