SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\Python36-32\Scripts\;C:\Program Files (x86)\Python36-32\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files\Git\cmd;C:\adb;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;C:\Program Files\MATLAB\R2016a\runtime\win64;C:\Program Files\MATLAB\R2016a\bin;C:\Program Files\MATLAB\R2016a\polyspace\bin;C:\Users\r22454\AppData\Local\Microsoft\WindowsApps;C:\Users\r22454\AppData\Local\Programs\Microsoft VS Code\bin
SET CPU_TYPE=HC32F460JEUA
SET CPU_VENDOR=HDSC
SET UV2_TARGET=hook_app
SET CPU_CLOCK=0x00B71B00
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\debug\system_hc32f460.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\output\debug\startup_hc32f460._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\debug\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\debug\hc32f4xx_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\debug\alink_handler.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\output\debug\hook_app.lnp"
