# Microsoft Developer Studio Generated NMAKE File, Based on re.dsp
!IF "$(CFG)" == ""
CFG=re - Win32 Debug
!MESSAGE No configuration specified. Defaulting to re - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "re - Win32 Release" && "$(CFG)" != "re - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "re.mak" CFG="re - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "re - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "re - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "re - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\re.dll"

!ELSE 

ALL : "pcre - Win32 Release" "elastiC - Win32 Release" "$(OUTDIR)\re.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"elastiC - Win32 ReleaseCLEAN" "pcre - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\re_m.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\re.dll"
	-@erase "$(OUTDIR)\re.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RE_MODULE_EXPORTS" /Fp"$(INTDIR)\re.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\re.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=elastic.lib pcre.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\re.pdb" /machine:I386 /nodefaultlib:"libc.lib" /out:"$(OUTDIR)\re.dll" /implib:"$(OUTDIR)\re.lib" /libpath:"Release\\" 
LINK32_OBJS= \
	"$(INTDIR)\re_m.obj" \
	"$(OUTDIR)\elastiC.lib" \
	"$(OUTDIR)\pcre.lib"

"$(OUTDIR)\re.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "re - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\re.dll"

!ELSE 

ALL : "pcre - Win32 Debug" "elastiC - Win32 Debug" "$(OUTDIR)\re.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"elastiC - Win32 DebugCLEAN" "pcre - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\re_m.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\re.dll"
	-@erase "$(OUTDIR)\re.exp"
	-@erase "$(OUTDIR)\re.ilk"
	-@erase "$(OUTDIR)\re.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RE_MODULE_EXPORTS" /D "DEBUG" /Fp"$(INTDIR)\re.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\re.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=elastic.lib pcre.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\re.pdb" /debug /machine:I386 /nodefaultlib:"libcd.lib" /out:"$(OUTDIR)\re.dll" /implib:"$(OUTDIR)\re.lib" /pdbtype:sept /libpath:"Debug\\" 
LINK32_OBJS= \
	"$(INTDIR)\re_m.obj" \
	"$(OUTDIR)\elastiC.lib" \
	"$(OUTDIR)\pcre.lib"

"$(OUTDIR)\re.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("re.dep")
!INCLUDE "re.dep"
!ELSE 
!MESSAGE Warning: cannot find "re.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "re - Win32 Release" || "$(CFG)" == "re - Win32 Debug"
SOURCE="..\..\src\modules-dynamic\re\re_m.c"

"$(INTDIR)\re_m.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "re - Win32 Release"

"elastiC - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Release" 
   cd "."

"elastiC - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "re - Win32 Debug"

"elastiC - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Debug" 
   cd "."

"elastiC - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\elastiC.mak" CFG="elastiC - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "re - Win32 Release"

"pcre - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Release" 
   cd "."

"pcre - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "re - Win32 Debug"

"pcre - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Debug" 
   cd "."

"pcre - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\pcre.mak" CFG="pcre - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 


!ENDIF 

