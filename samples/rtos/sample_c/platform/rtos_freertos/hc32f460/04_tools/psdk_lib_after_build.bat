@echo off

cd ..

@del ..\..\..\..\..\..\psdk_lib\lib\libpsdk.lib

@copy .\projects\output\libpsdk.lib ..\..\..\..\..\..\psdk_lib\lib\libpsdk.lib || goto failed

:succeed
    @echo cp lib successfully
    @set /A result=0
    @exit /B %result%

:failed
    @echo cp lib failed
    @set /A result=1
    @exit /B %result%
