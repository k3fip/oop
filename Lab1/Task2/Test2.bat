@echo off

set PROGRAM="%~1"
set OUT="%TEMP%\out.txt" 

%PROGRAM% 16 10 1F > %OUT%  || goto error
fc %OUT% Normal-out.txt || goto error

%PROGRAM% 8 12 -67 > %OUT%  || goto error
fc %OUT% Negative-out.txt || goto error

%PROGRAM% 2 10 1111111111111111111111111111111 > %OUT%  || goto error
fc %OUT% Max-out.txt || goto error

::добавить тест на максиммумы в 10
%PROGRAM% 10 2 2147483647 > %OUT%  || goto error
fc %OUT% Max10-out.txt || goto error

%PROGRAM% 10 2 -2147483648 > %OUT%  || goto error
fc %OUT% Min-out.txt || goto error

::добавить max+1
%PROGRAM% 10 2 2147483648 > %OUT%  || goto error
fc %OUT% Overflow-out.txt || goto error

%PROGRAM% 10 16 999999999999999999999 > %OUT%  || goto error
fc %OUT% Overflow-out.txt || goto error

%PROGRAM% 37 10 100 > %OUT%  || goto error
fc %OUT% Invalid-source-out.txt || goto error

%PROGRAM% 10 37 100 > %OUT%  || goto error
fc %OUT% Invalid-dest-out.txt || goto error

::единичные случаи проверить
%PROGRAM% 10 1 100 > %OUT%  || goto error
fc %OUT% Invalid-dest-out.txt || goto error

%PROGRAM% 1 23 100 > %OUT%  || goto error
fc %OUT% Invalid-source-out.txt || goto error

%PROGRAM% 12 13 0 > %OUT%  || goto error
fc %OUT% Zero-out.txt || goto error

echo All tests done
exit /B 0

:error
echo Testing failed
exit /B 1
