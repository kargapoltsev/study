@echo off

set "build_dir=build1\"

if exist %build_dir% (
  rd /s/q %build_dir%
  rd %build_dir%
)

mkdir %build_dir%
cd %build_dir%

conan install ..\conanfile.txt --profile=msvc-x64-debug
cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A x64 ..

set /p DUMMY=Press ENTER to continue...
