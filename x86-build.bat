@echo off
title - Panda3D Builder
color 4
:main
"thirdparty_x86/win-python3.12/python.exe" makepanda\makepanda.py --everything --installer --msvc-version=14.3 --no-eigen --windows-sdk 10 --threads=16 --no-maya2022
echo Done!
pause
goto :main
