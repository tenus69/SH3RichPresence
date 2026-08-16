@echo off
echo Building SH3RichPresence x86...
dotnet publish -c Release -r win-x86 --self-contained true -p:PublishSingleFile=true

echo.
echo x86 build complete!
explorer "bin\Release\net10.0\win-x86\publish"
pause