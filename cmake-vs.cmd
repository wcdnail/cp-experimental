@echo off
set VSBLD=build
set VCPKG_ROOT=C:/Dev/VCPkg
mkdir %VSBLD%
cd %VSBLD%
@echo on
cmake ^
-DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" ^
-DCMAKE_BUILD_TYPE=Debug ^
-G "Visual Studio 15 2017 Win64" ..
@echo off
cd ..
