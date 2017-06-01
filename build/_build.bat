REM show logs
echo on

REM qmake.exe and mingw32-make.exe
set PATH=C:\Qt\Qt5.6.2\5.6\android_armv7\bin\;C:\Qt\Qt5.6.2\Tools\mingw492_32\bin\
set PATH=C:\Qt\Qt5.9.0\5.9\android_armv7\bin\;C:\Qt\Qt5.9.0\Tools\mingw530_32\bin\
REM adb.exe
set PATH=%PATH%;C:\Users\Florin\AppData\Local\Android\sdk\platform-tools\
REM sed.exe
set "PATH=%PATH%;C:\Program Files\Git\usr\bin\"

REM Android SDK and NDK
set ANDROID_SDK_ROOT=C:/Users/Florin/AppData/Local/Android/sdk
set ANDROID_NDK_ROOT=C:\Users\Florin\AppData\Local\Android\sdk\ndk-bundle
set ANDROID_NDK_HOST=windows-x86_64

for %%P in ("%~dp0\..") do set "_SRC_DIR=%%~fP"
set _BUILD_PATH=android-build


qmake.exe %_SRC_DIR%\firebird.pro -spec android-g++ "CONFIG+=debug"

REM Workaround for a Qt 5.8 bug:
REM .. // Scanning for QML imports. // qmlimportscanner: No such file or directory: "qml" // Invalid json output from qmlimportscanner.
REM .. we'll remove "qml-import-paths" and we'll provide it later
sed.exe -i -r "s/\"qml-import-paths\": \"+.+\",//g" android-libfirebird.so-deployment-settings.json
sed.exe -i -r "s/\"qml-root-path\": \"+.+\",//g"    android-libfirebird.so-deployment-settings.json

mingw32-make.exe -j8 qmake_all
mingw32-make.exe -j8 INSTALL_ROOT="%_BUILD_PATH%" install

androiddeployqt^
	--input android-libfirebird.so-deployment-settings.json^
	--output %_BUILD_PATH%^
	--deployment bundled^
	--android-platform android-25^
	--gradle^
	--verbose^
	--qml-import-paths %_SRC_DIR%\qml

REM one last thing to fix:
REM .. C:\Qt\Qt5.9.0\5.9\android_armv7\src\3rdparty\gradle\gradle\wrapper\gradle-wrapper.properties 
REM .. adds gradle-3.4 but it actually needs gradle-2.2
REM .. you should run this script again after Gradle.properties is fixed
sed.exe -i -r "s/gradle-[0-9].[0-9]-bin.zip/gradle-2.2-bin.zip/g" android-build\gradle\wrapper\gradle-wrapper.properties

REM adb install -r %_BUILD_PATH%\build\outputs\apk\android-build-debug.apk
