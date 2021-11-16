@echo off
rem Generate directories

if not exist build\ mkdir build
if not exist tmp\ mkdir tmp

rem Go to directories

cd tmp

rem Generate cmake files

cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..

rem Build projects

@REM There is an error with images when building from cmd
@REM "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" ".\project.sln"