using System.Diagnostics;
using DiscordRPC;

internal static class Program
{
    private static DiscordRpcClient? client;
    private static DateTime sessionStart;
    private static async Task Main()
    {
        const string applicationId = "1533124871838503187";

        client = new DiscordRpcClient(applicationId);

        client.OnReady += (_, message) =>
        {
            Console.WriteLine(
                $"Connected to Discord as {message.User.Username}.");
        };

        client.Initialize();

        Console.WriteLine("SH3 Rich Presence monitor started.");
        Console.WriteLine("Waiting for Silent Hill 3...");
        Console.WriteLine("Press Ctrl+C to stop.");

        bool presenceActive = false;

        while (true)
        {
            Process? game = Process
                .GetProcessesByName("sh3")
                .FirstOrDefault();

            if (game != null)
            {
                using MemoryReader memory = new MemoryReader(game);

                float health =
                    memory.ReadFloatFromModuleOffset(0x498668);

                int locationGroup =
                    memory.ReadIntFromModuleOffset(0x32D284);

                int locationId =
                    memory.ReadIntFromModuleOffset(0x7577E8);

                Console.WriteLine($"Heather Health: {health:0##}%");
                Console.WriteLine($"Location Group: {locationGroup}");
                Console.WriteLine($"Location ID: {locationId}");
string locationName = (locationGroup, locationId) switch
{
    // Main Menu
    (0, 0) => "Main Menu",
    (220, 0) => "Main Menu",
    (220, 5) => "Main Menu",

    // Loading In
    (128, 5) => "Passing Through the Fog...",
    (128, 15) => "Passing Through the Fog...",

    // Central Square Shopping Center
    (128, 8) => "Central Square Shopping Center",
    (128, 9) => "Happy Burger",
    (170, 11) => "Central Square Shopping Center",
    (170, 12) => "Central Square Shopping Center",


    // Hazel Street Subway
    (150, 8)  => "Hazel Street Subway",
    (150, 13) => "Hazel Street Subway",

    // In the Train
    (90, 15)  => "In the train",
    (160, 15) => "In the train",

    // Sewers
    (160, 16) => "Sewers",
    (160, 17) => "Sewers",

    // Construction Site
    (170, 18) => "Construction Site",

    // Hilltop Center
    (160, 19) => "Hilltop Center",
    (160, 20) => "Hilltop Center",
    (160, 21) => "Hilltop Center",


    // Heather's Apartment
    (170, 27) => "Heather's Apartment",

    // Travel
    (170, 28) => "Driving to Silent Hill",
    (170, 29) => "Jack's Inn",

    // Silent Hill
    (160, 30) => "Silent Hill Streets",

    // Hilltop Center
    (_, 41) => "Fortune Teller",
    (_, 44) => "Belfry",
    (_, 45) => "The Church",

    // Unknown
    _ => $"Unknown Location ({locationGroup}, {locationId})"
};                
string healthImage;
string healthText;

if (health <= 0)
{
    healthImage = "health_fine";
    healthText = "Main Menu";
}
else if (health >= 70)
{
    healthImage = "health_fine";
    healthText = $"Fine ({health:0.#}%)";
}
else if (health >= 30)
{
    healthImage = "health_caution";
    healthText = $"Caution ({health:0.#}%)";
}
else
{
    healthImage = "health_danger";
    healthText = $"Danger ({health:0.#}%)";
}
                if (!presenceActive)
                {
                    Console.WriteLine(
                        $"Silent Hill 3 detected. PID: {game.Id}");

                    sessionStart = DateTime.UtcNow;
                    presenceActive = true;
                }

                    SetPresence(
                        locationName: locationName,
                        locationImage: "heather",
                        healthImage: healthImage,
                        healthText: healthText);

                }
            
            else
            {
                if (presenceActive)
                {
                    Console.WriteLine(
                        "Silent Hill 3 closed. Clearing presence.");

                    client.ClearPresence();
                    sessionStart = default;
                    presenceActive = false;
                }
            }

            await Task.Delay(2000);
        }
    }

    private static void SetPresence(
        string locationName,
        string locationImage,
        string healthImage,
        string healthText)
    {
        client?.SetPresence(new RichPresence
        {
            Details = locationName,
            State = "Playing as Heather Mason",

            Timestamps = new Timestamps
{
                Start = sessionStart
},
            Assets = new Assets
            {
                LargeImageKey = locationImage,
                LargeImageText = locationName,

                SmallImageKey = healthImage,
                SmallImageText = healthText
            }
        });
    }
}