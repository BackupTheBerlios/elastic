# Microsoft Developer Studio Generated NMAKE File, Based on ec.dsp
!IF "$(CFG)" == ""
CFG=ec - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ec - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ec - Win32 Release" && "$(CFG)" != "ec - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ec.mak" CFG="ec - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ec - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ec - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ec - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ec.exe"

!ELSE 

ALL : "re - Win32 Release" "pcre - Win32 Release" "elastiC - Win32 Release" "$(OUTDIR)\ec.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"elastiC - Win32 ReleaseCLEAN" "pcre - Win32 ReleaseCLEAN" "re - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ec.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ec.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ec.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=elastic.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ec.pdb" /machine:I386 /out:"$(OUTDIR)\ec.exe" /libpath:"Release\\" 
LINK32_OBJS= \
	"$(INTDIR)\ec.obj" \
	"$(OUTDIR)\elastiC.lib" \
	"$(OUTDIR)\pcre.lib"

"$(OUTDIR)\ec.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ec - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ec.exe" "$(OUTDIR)\ec.bsc"

!ELSE 

ALL : "re - Win32 Debug" "pcre - Win32 Debug" "elastiC - Win32 Debug" "$(OUTDIR)\ec.exe" "$(OUTDIR)\ec.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"elastiC - Win32 DebugCLEAN" "pcre - Win32 DebugCLEAN" "re - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ec.obj"
	-@erase "$(INTDIR)\ec.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ec.bsc"
	-@erase "$(OUTDIR)\ec.exe"
	-@erase "$(OUTDIR)\ec.ilk"
	-@erase "$(OUTDIR)\ec.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "DEBUG" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ec.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ec.sbr"

"$(OUTDIR)\ec.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=elastic.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ec.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ec.exe" /pdbtype:sept /libpath:"Debug\\" 
LINK32_OBJS= \
	"$(INTDIR)\ec.obj" \
	"$(OUTDIR)\elastiC.lib" \
	"$(OUTDIR)\pcre.lib"

"$(OUTDIR)\ec.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ec.dep")
!INCLUDE "ec.dep"
!ELSE 
!MESSAGE Warning: cannot find "ec.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ec - Win32 Release" || "$(CFG)" == "ec - Win32 Debug"
SOURCE=..\..\src\ec.c

!IF  "$(CFG)" == "ec - Win32 Release"


"$(INTDIR)\ec.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ec - Win32 Debug"


"$(INTDIR)\ec.obj"	"$(INTDIR)\ec.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "ec - Win32 Release"

"elastiC - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Release" 
   cd "."

"elastiC - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ec - Win32 Debug"

"elastiC - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Debug" 
   cd "."

"elastiC - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ec - Win32 Release"

"pcre - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Release" 
   cd "."

"pcre - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ec - Win32 Debug"

"pcre - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Debug" 
   cd "."

"pcre - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ec - Win32 Release"

"re - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\re.mak" CFG="re - Win32 Release" 
   cd "."

"re - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\re.mak" CFG="re - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ec - Win32 Debug"

"re - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\re.mak" CFG="re - Win32 Debug" 
   cd "."

"re - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\re.mak" CFG="re - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 


!ENDIF 

