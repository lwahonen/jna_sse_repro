setlocal
del  /F /Q  *.dll
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" x86
set linkOptions=/manifest:no  /LTCG /OPT:REF  /DEF:Source.def
set buildoptions=/Zi /EHsc /Od  /GL /MT /FAsc
cl  -LD sse_repro.cpp %buildoptions% -Fesse_repro.dll  /link %linkOptions%
