# Silent Hill 3 Discord Rich Presence

<img width="951" height="705" alt="Mod Promo" src="https://github.com/user-attachments/assets/5f83a45c-6816-4fe4-9e60-f220deb510bb" />


Bring live Discord Rich Presence to the original PC version of **Silent Hill 3**.

## Features

- 🎮 Automatic game detection
- 📍 Live location tracking
- ❤️ Heather health status (Fine / Caution / Danger)
- ⏱️ Session timer
- 🖼️ Custom Discord assets

## Installation

1. Download the latest release:
   https://github.com/tenus69/SH3RichPresence/releases/latest

2. Extract `SH3RichPresence.exe`.

3. Launch Discord.

4. Run `SH3RichPresence.exe`.

5. Start Silent Hill 3.

The Rich Presence activates automatically when the game is detected.

## Current Status

This project is currently in **beta**.

Most locations are working, but some are still being mapped.

If you encounter an **Unknown Location**, please create an Issue and include:

- Location Group
- Location ID

Example:

```
Unknown Location (170, 11)
```

## Planned Features

- Complete location database
- Boss encounter detection
- Better room accuracy
- Enhanced Edition compatibility testing
- Additional Rich Presence improvements

## Building

Requirements:

- .NET SDK 10 (or newer)

Build:

```powershell
dotnet publish -c Release -r win-x64 --self-contained true -p:PublishSingleFile=true
```

Or simply run:

```text
publish.bat
```

## Credits

Created by **tenus69**

Silent Hill 3 © Konami
Discord Rich Presence powered by Discord RPC.
