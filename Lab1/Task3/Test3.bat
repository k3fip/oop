@echo off

set PROGRAM="%~1"
set OUT="%TEMP%\out.txt" 

:: ŒÕ—ŒÀ‹Õ€≈ “≈—“€
%PROGRAM% < Identity-matrix.txt > %OUT%  || goto error
fc %OUT% Identity-matrix-out.txt || goto error

%PROGRAM% < Int-matrix.txt > %OUT%  || goto error
fc %OUT% Int-matrix-out.txt || goto error

%PROGRAM% < Example.txt > %OUT%  || goto error
fc %OUT% Example-out.txt || goto error

%PROGRAM% < Fractional.txt > %OUT%  || goto error
fc %OUT% Fractional-out.txt || goto error

%PROGRAM% < Mixed.txt > %OUT%  || goto error
fc %OUT% Mixed-out.txt || goto error

%PROGRAM% < Non-invertable.txt > %OUT%  || goto error
fc %OUT% Non-invertable-out.txt || goto error

%PROGRAM% < Bad-input.txt > %OUT%  || goto error
fc %OUT% Bad-input-out.txt || goto error

%PROGRAM% < Less-input.txt > %OUT%  || goto error
fc %OUT% Less-input-out.txt || goto error

%PROGRAM% < Overflow-big.txt > %OUT%  || goto error
fc %OUT% Overflow-big-out.txt || goto error

%PROGRAM% < Overflow-low.txt > %OUT%  || goto error
fc %OUT% Overflow-low-out.txt || goto error

%PROGRAM% < Boundary.txt > %OUT%  || goto error
fc %OUT% Boundary-out.txt || goto error

::— œ¿–¿Ã≈“–¿Ã»
%PROGRAM% -h > %OUT%  || goto error
fc %OUT% Help-out.txt || goto error

::◊ÚÂÌËÂ Ò Ù‡ÈÎ‡
%PROGRAM% Example.txt > %OUT%  || goto error
fc %OUT% Example-out.txt || goto error

%PROGRAM% Fractional.txt > %OUT%  || goto error
fc %OUT% Fractional-out.txt || goto error

%PROGRAM% Mixed.txt > %OUT%  || goto error
fc %OUT% Mixed-out.txt || goto error

%PROGRAM% Non-invertable.txt > %OUT%  || goto error
fc %OUT% Non-invertable-out.txt || goto error

%PROGRAM% Overflow-big.txt > %OUT%  || goto error
fc %OUT% Overflow-big-out.txt || goto error

%PROGRAM% Overflow-low.txt > %OUT%  || goto error
fc %OUT% Overflow-low-out.txt || goto error

%PROGRAM% Boundary.txt > %OUT%  || goto error
fc %OUT% Boundary-out.txt || goto error

::ÕÂÒÛ˘ÂÒÚ‚Û˛˘ËÈ Ù‡ÈÎ
%PROGRAM% 12345.txt > %OUT%  || goto error
fc %OUT% Bad-input-out.txt || goto error

echo All tests done
exit /B 0

:error
echo Testing failed
exit /B 1
