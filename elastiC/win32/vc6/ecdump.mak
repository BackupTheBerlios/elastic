# Microsoft Developer Studio Generated NMAKE File, Based on ecdump.dsp
!IF "$(CFG)" == ""
CFG=ecdump - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ecdump - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ecdump - Win32 Release" && "$(CFG)" != "ecdump - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ecdump.mak" CFG="ecdump - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ecdump - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ecdump - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ecdump - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ecdump.exe"

!ELSE 

ALL : "re - Win32 Release" "pcre - Win32 Release" "elastiC - Win32 Release" "$(OUTDIR)\ecdump.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"elastiC - Win32 ReleaseCLEAN" "pcre - Win32 ReleaseCLEAN" "re - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ecdump.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ecdump.exe"

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ecdump.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=elastic.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ecdump.pdb" /machine:I386 /out:"$(OUTDIR)\ecdump.exe" /libpath:"Release\\" 
LINK32_OBJS= \
	"$(INTDIR)\ecdump.obj" \
	"$(OUTDIR)\elastiC.lib" \
	"$(OUTDIR)\pcre.lib"

"$(OUTDIR)\ecdump.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ecdump - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\ecdump.exe" "$(OUTDIR)\ecdump.bsc"

!ELSE 

ALL : "re - Win32 Debug" "pcre - Win32 Debug" "elastiC - Win32 Debug" "$(OUTDIR)\ecdump.exe" "$(OUTDIR)\ecdump.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"elastiC - Win32 DebugCLEAN" "pcre - Win32 DebugCLEAN" "re - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ecdump.obj"
	-@erase "$(INTDIR)\ecdump.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ecdump.bsc"
	-@erase "$(OUTDIR)\ecdump.exe"
	-@erase "$(OUTDIR)\ecdump.ilk"
	-@erase "$(OUTDIR)\ecdump.pdb"

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ecdump.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ecdump.sbr"

"$(OUTDIR)\ecdump.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=elastic.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ecdump.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ecdump.exe" /pdbtype:sept /libpath:"Debug\\" 
LINK32_OBJS= \
	"$(INTDIR)\ecdump.obj" \
	"$(OUTDIR)\elastiC.lib" \
	"$(OUTDIR)\pcre.lib"

"$(OUTDIR)\ecdump.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ecdump.dep")
!INCLUDE "ecdump.dep"
!ELSE 
!MESSAGE Warning: cannot find "ecdump.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ecdump - Win32 Release" || "$(CFG)" == "ecdump - Win32 Debug"
SOURCE=..\..\src\ecdump.c

!IF  "$(CFG)" == "ecdump - Win32 Release"


"$(INTDIR)\ecdump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ecdump - Win32 Debug"


"$(INTDIR)\ecdump.obj"	"$(INTDIR)\ecdump.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "ecdump - Win32 Release"

"elastiC - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Release" 
   cd "."

"elastiC - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ecdump - Win32 Debug"

"elastiC - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Debug" 
   cd "."

"elastiC - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ecdump - Win32 Release"

"pcre - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Release" 
   cd "."

"pcre - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ecdump - Win32 Debug"

"pcre - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Debug" 
   cd "."

"pcre - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "ecdump - Win32 Release"

"re - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\re.mak" CFG="re - Win32 Release" 
   cd "."

"re - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\re.mak" CFG="re - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "ecdump - Win32 Debug"

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

