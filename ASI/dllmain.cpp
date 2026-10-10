#include <Windows.h>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include "discord_rpc.h"
#include <ctime>
const char* GetLocationName(int group, int id)
{
    // Main Menu
    if ((group == 0 && id == 0) ||
        (group == 220 && id == 0) ||
        (group == 220 && id == 5 || id == 7))
        return "Main Menu";

    // Loading In
    if (
        (group == 96 && id == 40)
        ||
        (group == 128 &&
            (id == 5 || id == 15 || id == 18 || id == 19 ||
                id == 20 || id == 23 || id == 25 || id == 32 || id == 36 || id == 37 || id == 38))
        ||
        (group == 220 && (id == 30 || id == 32 || id == 37))
        )
    {
        return "Passing Through the Fog...";
    }

    // Central Square Shopping Center
    if ((group == 128 && id == 8) ||
        (group == 170 && id == 11) ||
        (group == 170 && id == 12))
        return "Central Square Shopping Center";

    if ((group == 128 && (id == 9)) ||
        (group == 220 && id == 8))
        return "Happy Burger";

    // Hazel Street Subway
    if (group == 150 && (id == 8 || id == 13))
        return "Hazel Street Subway";

    // In the Train
    if ((group == 90 && id == 15) ||
        (group == 160 && id == 15))
        return "In the train";

    // Sewers
    if (group == 160 && (id == 16 || id == 17))
        return "Sewers";

    // Construction Site
    if (group == 170 && id == 18)
        return "Construction Site";

    // Hilltop Center
    if (group == 160 && (id == 19 || id == 20 || id == 21))
        return "Hilltop Center";

    // Hilltop Center (Otherworld)
    if (group == 190 &&
        (id == 22 || id == 23 || id == 24 || id == 25))
        return "Hilltop Center (Otherworld)";

    // Bergen Street
    if (group == 170 && id == 26)
        return "Bergen Street";

    // Daisy Villa Apartments
    if (group == 170 && id == 27)
        return "Daisy Villa Apartments";

    // Travel
    if (group == 34 && id == 28)
        return "Driving to Silent Hill";

    // Jack's Inn - Room 106
    if (group == 180 && id == 29)
        return "Jack's Inn - Room 106";

    // Silent Hill
    if (group == 160 && id == 30)
        return "Roaming through Silent Hill";

    if (group == 180 && id == 31)
        return "Heaven's Night";

    // Brookhaven Hospital
    if ((group == 160 && (id == 32 || id == 33 || id == 34 || id == 35)) ||
        (group == 200 && id == 36))
        return "Brookhaven Hospital";

    // Brookhaven Hospital (Otherworld)
    if ((group == 200 && id == 39) ||
        (group == 180 && (id == 37 || id == 38 || id == 39)))
        return "Brookhaven Hospital (Otherworld)";

    // Lakeside Amusement Park
    if ((group == 220 && id == 40 || id == 41 || id == 42) ||
        (group == 180 && id == 42))
        return "Lakeside Amusement Park";

    // Chapel
    if ((group == 128 && id == 43) ||
        (group == 190 && id == 44 || id == 45))
        return "Exploring The Chapel";

    if ((group == 220 && id == 44 || id == 45 || id == 46))
        return "Chapel (Otherworld)";

    return "Unknown Location";
}

constexpr const char* DISCORD_APP_ID = "1533124871838503187";

// Tracks when the game session started
static int64_t gameStartTimestamp = 0;

void InitializeDiscord()
{
    DiscordEventHandlers handlers{};

    Discord_Initialize(
        DISCORD_APP_ID,
        &handlers,
        0,
        nullptr
    );

    gameStartTimestamp = static_cast<int64_t>(std::time(nullptr));
    
    DiscordRichPresence presence{};

    presence.details = "SH3RichPresence ASI Test";
    presence.state = "Playing as Heather Mason";
    presence.startTimestamp = gameStartTimestamp;

    Discord_UpdatePresence(&presence);
}

void UpdateDiscordPresence(const char* locationName, float currentHealth)
{
    static char healthText[64];

    bool isMainMenu =
        strcmp(locationName, "Main Menu") == 0;

    const char* healthImage = nullptr;
    const char* healthStatus = nullptr;

    if (!isMainMenu)
    {
        healthImage = "health_fine";
        healthStatus = "Fine";

        if (currentHealth <= 25.0f)
        {
            healthImage = "health_danger";
            healthStatus = "Danger";
        }
        else if (currentHealth <= 60.0f)
        {
            healthImage = "health_caution";
            healthStatus = "Caution";
        }

        sprintf_s(
            healthText,
            "%s - %.1f%%",
            healthStatus,
            currentHealth
        );
    }

    DiscordRichPresence presence{};

    presence.details = locationName;

    if (!isMainMenu)
    {
        presence.state = healthText;
        presence.smallImageKey = healthImage;
        presence.smallImageText = healthText;
    }

    // Keep the timer running across location and health changes
    presence.startTimestamp = gameStartTimestamp;

    Discord_UpdatePresence(&presence);
}

DWORD WINAPI MainThread(LPVOID)
{
    // Give Silent Hill 3 a moment to finish starting.
    Sleep(2000);

    std::ofstream log("SH3RichPresence.log");

    if (!log.is_open())
        return 0;

    log << "SH3RichPresence ASI loaded successfully!" << std::endl;

    InitializeDiscord();
    log << "Discord RPC initialization requested." << std::endl;

    // Get the base address of sh3.exe.
    uintptr_t baseAddress =
        reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));

    log << "sh3.exe base address: 0x"
        << std::hex << baseAddress
        << std::dec << std::endl;

    // Your existing Location Group offset.
    constexpr uintptr_t LOCATION_GROUP_OFFSET = 0x32D284;

    // Calculate the actual memory address.
    uintptr_t locationGroupAddress =
        baseAddress + LOCATION_GROUP_OFFSET;

    // Read the 4-byte integer directly from SH3's memory.
    int locationGroup =
        *reinterpret_cast<int*>(locationGroupAddress);

    log << "Location Group: "
        << locationGroup
        << std::endl;


    // Read Location ID
    constexpr uintptr_t LOCATION_ID_OFFSET = 0x7577E8;

    uintptr_t locationIdAddress =
        baseAddress + LOCATION_ID_OFFSET;

    // Read Heather's Health
    constexpr uintptr_t HEALTH_OFFSET = 0x498668;

    uintptr_t healthAddress =
        baseAddress + HEALTH_OFFSET;

    // Track previous values so we only log changes
    int previousGroup = -1;
    int previousId = -1;
    float previousHealth = -1.0f;

    while (true)
    {
        Discord_RunCallbacks();

        int currentGroup =
            *reinterpret_cast<int*>(locationGroupAddress);

        int currentId =
            *reinterpret_cast<int*>(locationIdAddress);

        float currentHealth =
            *reinterpret_cast<float*>(healthAddress);


if (currentHealth != previousHealth)
{
    log << "Health changed: "
        << std::fixed
        << std::setprecision(1)
        << currentHealth
        << "%"
        << std::endl;

    log.flush();

    UpdateDiscordPresence(
        GetLocationName(currentGroup, currentId),
        currentHealth
    );

    previousHealth = currentHealth;
}

        if (currentGroup != previousGroup || currentId != previousId)
        {
            const char* locationName =
                GetLocationName(currentGroup, currentId);

            log << "Location changed: "
                << locationName
                << " ("
                << currentGroup
                << ", "
                << currentId
                << ")"
                << " | Health: "
                << std::fixed
                << std::setprecision(1)
                << currentHealth
                << "%"
                << std::endl;

            log.flush();

            UpdateDiscordPresence(locationName, currentHealth);

            previousGroup = currentGroup;
            previousId = currentId;
        }

        Sleep(1000);
    }


    

    return 0;
}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD reason,
    LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);

        HANDLE thread = CreateThread(
            nullptr,
            0,
            MainThread,
            nullptr,
            0,
            nullptr
        );

        if (thread)
            CloseHandle(thread);
    }

    return TRUE;
}