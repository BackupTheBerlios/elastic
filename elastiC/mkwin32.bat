REM Create public header file directory
mkdir elastiC
copy src\*.h elastiC

REM Substitute platform specific public header files
REM (private ones are automatically found in win32 subdir)
del elastiC\cnf.h
del elastiC\conf.h
del elastiC\tsize.h
copy src\win32\cnf.h elastiC
copy src\win32\conf.h elastiC
copy src\win32\tsize.h elastiC

REM build
REM nmake /f win32\elastiC\elastiC.mak
