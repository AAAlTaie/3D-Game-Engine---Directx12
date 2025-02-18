@echo off

set download_dir=download/assimp
set build_dir=%download_dir%\build
set install_dir=External

if exist "%download_dir%\CMakeLists.txt" (
    echo Found Assimp...
) else (
    echo Cloning Assimp...
    git clone https://github.com/assimp/assimp.git %download_dir%
)

set flag=-DBUILD_SHARED_LIBS=ON ^
         -DASSIMP_BUILD_TESTS=OFF ^
         @REM -DASSIMP_BUILD_OBJ_IMPORTER=ON ^
         @REM -DASSIMP_BUILD_FBX_IMPORTER=ON ^
         -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=ON ^
         -DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=OFF ^
         -DASSIMP_NO_ZLIB=ON ^
         -DCMAKE_CXX_STANDARD=17

cmake -S %download_dir% -B %build_dir% -G "Visual Studio 17 2022" -A x64 %flag%
cmake --build %build_dir% --config Release
cmake --install %build_dir% --prefix %install_dir%
