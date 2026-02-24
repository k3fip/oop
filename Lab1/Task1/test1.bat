@echo off

set PROGRAM="%~1"
set OUT="%TEMP%\out.txt" 

::файлы с одинаковым содержимым
%PROGRAM% normal1.txt normal2.txt > %OUT%  || goto error
fc %OUT% Equal-out.txt || goto error

::оба файлы пусты
%PROGRAM% empty1.txt empty2.txt > %OUT%  || goto error
fc %OUT% Empty-equal-out.txt || goto error

::файлы различны
%PROGRAM% normal1.txt normal3.txt > %OUT%  || goto error
fc %OUT% Difference1-out.txt || goto error

::проверка на различие подстрок
%PROGRAM% normal1.txt normal4.txt > %OUT%  || goto error
fc %OUT% Difference2-out.txt || goto error

::большие файлы
%PROGRAM% Onegin.txt OneginLeast.txt > %OUT%  || goto error
fc %OUT% Difference3-out.txt || goto error

::плохой ввод
%PROGRAM% f1.txt sdfsdfs  > %OUT%  || goto error
fc %OUT% Bad-input-out.txt || goto error

%PROGRAM% fsdfsd f1.txt  > %OUT%  || goto error
fc %OUT% Bad-input-out.txt || goto error

%PROGRAM% fsdfsd sdfsdfs  > %OUT%  || goto error
fc %OUT% Bad-input-out.txt || goto error


echo All tests done
exit /B 0

:error
echo Testing failed
exit /B 1e
