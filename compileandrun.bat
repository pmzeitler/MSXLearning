
@echo off
echo -------- Compilation of : 
echo %1
echo .
SET HEX2BINDIR=D:\Applications\Hex2bin-2.5\bin\Release\
SET HEX2BIN=hex2bin.exe 
SET ASM=sdasz80 
SET CC=sdcc 
SET DEST=dsk\
SET SRCDIR=src\
SET OUTDIR=build\
SET OPENMSXDIR=D:\Applications\OpenMSX\
SET MSXVMACHINE=Panasonic_FS-A1WSX

SET INCLUDEDIR=fusion-c\include\
SET LIBDIR=fusion-c\lib\
SET HEADERDIR=fusion-c\header\

SET proga=%1

IF NOT EXIST %OUTDIR% mkdir %OUTDIR%

IF NOT EXIST %OUTDIR%%proga%.ihx GOTO _clean_
del %OUTDIR%%proga%.com
del %OUTDIR%%proga%.asm
del %OUTDIR%%proga%.ihx
del %OUTDIR%%proga%.lk
del %OUTDIR%%proga%.lst
del %OUTDIR%%proga%.map
del %OUTDIR%%proga%.noi
del %OUTDIR%%proga%.sym
del %OUTDIR%%proga%.rel

:_clean_

REM  Standard crt0
SET INC1=%INCLUDEDIR%crt0_msxdos.rel

REM  use this crt0 if you want to pass parameters to your program
REM SET INC1=%INCLUDEDIR%crt0_msxdos_advanced.rel

REM SET INC2=%INCLUDEDIR%
REM SET INC3=%INCLUDEDIR%
REM SET INC4=%INCLUDEDIR%
REM SET INC5=%INCLUDEDIR%
REM SET INC6=%INCLUDEDIR%
REM SET INC7=%INCLUDEDIR%
REM SET INC8=%INCLUDEDIR%
REM SET INC9=%INCLUDEDIR%
REM SET INCA=%INCLUDEDIR%
REM SET INCB=%INCLUDEDIR%
REM SET INCC=%INCLUDEDIR%
REM SET INCD=%INCLUDEDIR%
REM SET INCE=%INCLUDEDIR%
REM SET INCF=%INCLUDEDIR%

REM  Standard Code-loc adress
SET ADDR_CODE=0x107

REM use this parameter if you are using crt0_msxdos_advanced
REM SET ADDR_CODE=0x180

SET ADDR_DATA=0x0



SDCC --code-loc %ADDR_CODE% --data-loc %ADDR_DATA% --disable-warning 196 -mz80 --no-std-crt0 --opt-code-size fusion.lib -L %LIBDIR% -o %OUTDIR% -I %HEADERDIR% %INC1% %INC2% %INC3% %INC4% %INC5% %INC6% %INC7% %INC8% %INC9% %INCA% %INCB% %INCC% %INCD% %INCE% %INCF% %SRCDIR%%proga%.c



SET cpath=%~dp0


IF NOT EXIST %OUTDIR%%proga%.ihx GOTO _badend_
echo ... Compilation OK

REM GOTO _end2_

@echo on

%HEX2BINDIR%hex2bin -e com %OUTDIR%%proga%.ihx

@echo off

copy %OUTDIR%%proga%.com DSK\%proga%.com /y



:_end_
echo Starting emulation...

:Emulator
%OPENMSXDIR%openmsx.exe -machine %MSXVMACHINE% -ext msxdos2 -ext ram512k -diska dsk\
GOTO _end2_

:_badend_
echo Compilation failed.

:_end2_
