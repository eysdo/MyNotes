call "E:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
qmake -o makefile testmake.pro
jom /f makefile
pause