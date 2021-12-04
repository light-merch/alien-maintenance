@echo off
rem Generate directories

if not exist build\ mkdir build

rem Move dlls and assets to build directory
xcopy dlls\* build\ /Y
xcopy assets\ build\ /E /Y

if not exist tmp\ mkdir tmp

rem Go to directories

cd tmp

rem Generate cmake files

cmake ..

rem Build projects

rem There is an error with images when building from cmd probably with Linker
rem Use VS manually to build
rem "C:\Program Files\Microsoft Visual Studio\2022\Preview\Msbuild\Current\Bin\MSBuild.exe" ".\Alien Maintenance.sln"