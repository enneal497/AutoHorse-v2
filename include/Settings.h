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

    inline std::string espName{ "AutoHorse.esp" };
    inline int questID{ 0x0800 };
    inline int g_speedID{ 0x0808 };
    inline int g_tutorialID{ 0x809 };
    inline int g_isReadyID{ 0x803 };
    inline int markerID{ 0x0801 };

    inline RE::FormID clairvoyanceID{ 0x21143 };

    inline float thumbstickThreshold{ 0.5 };
    inline bool bShowTutorial{ true };
    inline bool bRequireClairvoyance{ false };

    inline RE::INPUT_DEVICE cDevice;
};

//Retrieve at runtime
inline uint32_t ForwardKey{ 17 };
inline uint32_t BackKey{ 31 };
inline uint32_t LeftKey{ 30 };
inline uint32_t RightKey{ 32 };
inline uint32_t ActivateKey{ 18 };
inline uint32_t AutoMoveKey{ 46 };
//Speed
inline uint32_t SprintKey{ 56 };
inline uint32_t WalkKey{ 58 };
inline uint32_t ShiftKey{ 42 };

//Retrieve from INI
inline uint32_t KStart{ 44 };
inline uint32_t GStart{ 1 };
inline uint32_t GCameraMod{ 128 };

//Platform-dependent variables
inline uint32_t StartKey;

static void ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);
static void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
static void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);