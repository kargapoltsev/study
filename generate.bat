@echo off

set "build_dir=build1\"

if exist %build_dir% (
    cd %build_dir%
    cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A x64 ..
)

set /p DUMMY=Press ENTER to continue...
