@echo off
echo **********************************************************************
echo **                                                                  **
echo **                  Start To Build ModelX Throw Project             **
echo **                                                                  **
echo **********************************************************************

rem 
rem C:\Keil_v5\UV4\
set BUILD_EXE=C:\Keil_v5\UV4\uVision.com
set LOG_PATH=.\build_log
set RELEASE_PATH=.\release
set BOOT_LOG_NAME=%LOG_PATH%\boot_building.log
set APP_LOG_NAME=%LOG_PATH%\app_building.log
set BOOT_PROJECT_PATHNAME=..\03_boot\projects\boot.uvprojx
set APP_PROJECT_PATHNAME=..\02_app\projects\app.uvprojx

if not exist %RELEASE_PATH% (md %RELEASE_PATH%)


echo ***********************step_1: clean old log ...**********************
if exist %LOG_PATH% RD /q /s %LOG_PATH%
md %LOG_PATH%
echo=> %BOOT_LOG_NAME%
echo=> %APP_LOG_NAME%
echo clean old log OK !

echo ***********************step_2: compling boot ...**********************
%BUILD_EXE% -j0 -r %BOOT_PROJECT_PATHNAME% -o %BOOT_LOG_NAME%
echo error_level: %ERRORLEVEL%

if %ERRORLEVEL% LSS 2 (
	type %BOOT_LOG_NAME%
	echo compile boot OK !
) else (
	type %BOOT_LOG_NAME%
	echo compile boot error !
	pause
	goto BUILD_END
)

echo ***********************step_3: compling app ...***********************
%BUILD_EXE% -j0 -r %APP_PROJECT_PATHNAME% -o %APP_LOG_NAME%
echo error_level: %ERRORLEVEL%
if %ERRORLEVEL% LSS 2 (
	type %APP_LOG_NAME%
	echo compile app OK !
) else (
	type %APP_LOG_NAME%
	echo compile app error !
	pause
	goto BUILD_END
)

echo *****************step_4: generate upg and bin file ...****************
call python all_make_bin_upg.py
echo generate upg and bin file OK !
:BUILD_END
echo *****************step_5: end and return a rlt to jenkins ! ***********
if %ERRORLEVEL% LSS 2 (
	echo all make OK !
	rem delay 3s
	echo Wscript.sleep 3000 > delay.vbs
	call delay.vbs & del delay.vbs
	exit(0)
)else (
	echo all make error !
	rem delay 3s
	echo Wscript.sleep 3000 > delay.vbs
	call delay.vbs & del delay.vbs
	exit(2)
)



