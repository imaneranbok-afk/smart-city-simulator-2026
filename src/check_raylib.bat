@echo off
echo === VERIFICATION RAYLIB ===
echo.

echo 1. Verification des fichiers include...
if exist "C:\raylib\include\raylib.h" (
    echo ✅ raylib.h trouve dans C:\raylib\include\
) else (
    echo ❌ raylib.h introuvable dans C:\raylib\include\
    echo Recherche dans d'autres emplacements...
)

echo.
echo 2. Verification des fichiers library...
if exist "C:\raylib\lib\libraylib.a" (
    echo ✅ libraylib.a trouve dans C:\raylib\lib\
) else (
    echo ❌ libraylib.a introuvable dans C:\raylib\lib\
)

echo.
echo 3. Recherche dans C:\raylib\raylib\src\...
if exist "C:\raylib\raylib\src\raylib.h" (
    echo ✅ raylib.h trouve dans C:\raylib\raylib\src\
) else (
    echo ❌ raylib.h introuvable dans C:\raylib\raylib\src\
)

echo.
echo 4. Recherche dans C:\raylib\src\...
if exist "C:\raylib\src\raylib.h" (
    echo ✅ raylib.h trouve dans C:\raylib\src\
) else (
    echo ❌ raylib.h introuvable dans C:\raylib\src\
)

echo.
echo === SOLUTIONS ===
echo Si raylib n'est pas installe, telechargez-le depuis:
echo https://github.com/raysan5/raylib/releases
echo.
pause