#pragma once

enum class KeyType
{
    Forward,
    Back,
    Left,
    Right,
    Activate,
    Start,
    Sprint,
    Walk,
    Shift,
    AutoMoveKey,
    CameraMod,
};

namespace Settings
{
    void LoadSettings() noexcept;
    void GetMappedControls(const RE::INPUT_DEVICE& device);

    uint32_t ReturnControls(KeyType keytype);

    static std::string espName{ "AutoHorse.esp" };
    static int questID{ 0x0800 };
    static int g_speedID{ 0x0808 };
    static int g_tutorialID{ 0x809 };
    static int g_isReadyID{ 0x803 };
    static int markerID{ 0x0801 };

    static float thumbstickThreshold{ 0.5 };
    static bool bShowTutorial{ true };

    static RE::INPUT_DEVICE cDevice;
};

//Retrieve at runtime
static uint32_t ForwardKey{ 17 };
static uint32_t BackKey{ 31 };
static uint32_t LeftKey{ 30 };
static uint32_t RightKey{ 32 };
static uint32_t ActivateKey{ 18 };
static uint32_t AutoMoveKey{ 46 };
//Speed
static uint32_t SprintKey{ 56 };
static uint32_t WalkKey{ 58 };
static uint32_t ShiftKey{ 42 };

//Retrieve from INI
static uint32_t KStart{ 44 };
static uint32_t GStart{ 1 };
static uint32_t GCameraMod{ 128 };

//Platform-dependent variables
static uint32_t StartKey;

static void ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);
static void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
static void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);