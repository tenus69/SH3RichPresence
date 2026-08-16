@echo off
echo Building SH3 Rich Presence...
dotnet publish -c Release -r win-x64 --self-contained true -p:PublishSingleFile=true
echo.
echo Done!
pause