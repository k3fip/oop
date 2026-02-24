@echo off

set PROGRAM="%~1"
set OUT="%TEMP%\out.txt" 

%PROGRAM% Normal.txt > %OUT%  || goto error
fc %OUT% Normal-out.txt || goto error

%PROGRAM% DifferentShape.txt > %OUT%  || goto error
fc %OUT% DifferentShape-out.txt || goto error

%PROGRAM% EmptyField.txt > %OUT%  || goto error
fc %OUT% EmptyField-out.txt || goto error

%PROGRAM% Extinction.txt > %OUT%  || goto error
fc %OUT% Extinction-out.txt || goto error

%PROGRAM% IncorrectData.txt > %OUT%  || goto error
fc %OUT% IncorrectData-out.txt || goto error

%PROGRAM% DiffLenght.txt > %OUT%  || goto error
fc %OUT% DiffLenght-out.txt || goto error

%PROGRAM% Overflow.txt > %OUT%  || goto error
fc %OUT% Overflow-out.txt || goto error

%PROGRAM% Minimum.txt > %OUT%  || goto error
fc %OUT% Minimum-out.txt || goto error

%PROGRAM% Minimum.txt Minimum.txt Minimum.txt > %OUT%  || goto error
fc %OUT% IncorrectInput-out.txt || goto error

%PROGRAM% EmptyFile.txt > %OUT%  || goto error
fc %OUT% EmptyFile-out.txt || goto error

echo All tests done
exit /B 0

:error
echo Testing failed
exit /B 1
