@echo off

echo Please select the board for which you want to update firmware:

echo 1 - Official OpenDeck board
echo 2 - Arduino Mega
echo 3 - Arduino Uno
echo 4 - Arduino Leonardo
echo 5 - Arduino Pro Micro
echo 6 - Teensy++ 2.0

set /p board=""

IF "%board%"=="1" (
    ..\bin\dfu\hid_bootloader_loader_win.exe atmega32u4 ..\bin\compiled\fw\opendeck.hex
) ELSE IF "%board%"=="2" (
    ..\bin\dfu\hid_bootloader_loader_win.exe atmega2560 ..\bin\compiled\fw\mega2560.hex
) ELSE IF "%board%"=="3" (
    ..\bin\dfu\hid_bootloader_loader_win.exe atmega328p ..\bin\compiled\fw\uno.hex
) ELSE IF "%board%"=="4" (
    ..\bin\dfu\hid_bootloader_loader_win.exe atmega32u4 ..\bin\compiled\fw\leonardo.hex
) ELSE IF "%board%"=="5" (
    ..\bin\dfu\hid_bootloader_loader_win.exe atmega32u4 ..\bin\compiled\fw\promicro.hex
) ELSE IF "%board%"=="6" (
    ..\bin\dfu\hid_bootloader_loader_win.exe at90usb1286 ..\bin\compiled\fw\teensy2pp.hex
) ELSE (
    echo Incorrect board number selected.
)