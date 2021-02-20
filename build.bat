@echo off
title - Panda3D Builder
:main
"thirdparty/win-python/python.exe" makepanda/makepanda.py --everything --installer --msvc-version=14.2 --no-eigen --windows-sdk 10 --threads=4
echo Done!
pause
goto :main