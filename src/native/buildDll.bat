setlocal
del  /F /Q  win32-x86\*.dll
call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" x86
set linkOptions=/manifest:no  /LTCG /OPT:REF  /DEF:Source.def
set buildoptions=/Zi /EHsc /Od  /GL /MT /FAsc /arch:IA32
cl  -LD sse_repro.cpp %buildoptions% -Fewin32-x86\sse_repro.dll  /link %linkOptions%