@echo off
echo Bootstrapping vcpkg...
call vcpkg\bootstrap-vcpkg.bat

echo Installing dependencies...
call vcpkg\vcpkg install

echo Done!
pause
