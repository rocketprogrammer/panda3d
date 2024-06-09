@echo off
title - Panda3D Builder
color 4

set MAKEPANDA_THIRDPARTY=thirdparty_arm64
set PYTHONEXEC="%localappdata%\Programs\Python\Python312-arm64\python.exe"

:main
python.exe makepanda\makepanda.py --everything --wheel --msvc-version=14.3 --no-eigen --windows-sdk 10 --threads=16 --arch arm64 --python-libdir=%PYTHONEXEC% --no-nvidiacg --threads=4
echo Done!
pause
goto :main
