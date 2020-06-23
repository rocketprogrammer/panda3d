@echo off
title - Panda3D Builder
color 4
:main
"thirdparty/win-python3.6-x64/python.exe" makepanda/makepanda.py --everything --installer --msvc-version=14.2 --no-eigen --windows-sdk 10 --threads=4 --static
echo Done!
pause
goto :main