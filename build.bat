@echo off
title - Panda3D Builder
color 4
:main
"thirdparty/win-python3.10-x64/python.exe" makepanda\makepanda.py --everything --installer --msvc-version=14.3 --no-eigen --windows-sdk 10 --threads=16
echo Done!
pause
goto :main