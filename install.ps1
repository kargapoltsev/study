$build_dir_name = "build"

if (Test-Path $build_dir_name) {
    rm -r $build_dir_name
    mkdir $build_dir_name
} 

cd $build_dir_name

conan install ..\conanfile.txt --profile=msvc

cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A x64 ..

Write-Host -NoNewLine 'Press any key to continue...';
$null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown');
