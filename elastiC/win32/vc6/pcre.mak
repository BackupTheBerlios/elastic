# Microsoft Developer Studio Generated NMAKE File, Based on pcre.dsp
!IF "$(CFG)" == ""
CFG=pcre - Win32 Debug
!MESSAGE No configuration specified. Defaulting to pcre - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "pcre - Win32 Release" && "$(CFG)" != "pcre - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pcre.mak" CFG="pcre - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pcre - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "pcre - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "pcre - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\pcre.lib"


CLEAN :
	-@erase "$(INTDIR)\get.obj"
	-@erase "$(INTDIR)\maketables.obj"
	-@erase "$(INTDIR)\pcre.obj"
	-@erase "$(INTDIR)\study.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\pcre.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "STATIC" /Fp"$(INTDIR)\pcre.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pcre.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\pcre.lib" 
LIB32_OBJS= \
	"$(INTDIR)\get.obj" \
	"$(INTDIR)\maketables.obj" \
	"$(INTDIR)\pcre.obj" \
	"$(INTDIR)\study.obj"

"$(OUTDIR)\pcre.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "pcre - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\pcre.lib"


CLEAN :
	-@erase "$(INTDIR)\get.obj"
	-@erase "$(INTDIR)\maketables.obj"
	-@erase "$(INTDIR)\pcre.obj"
	-@erase "$(INTDIR)\study.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\pcre.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "STATIC" /Fp"$(INTDIR)\pcre.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pcre.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\pcre.lib" 
LIB32_OBJS= \
	"$(INTDIR)\get.obj" \
	"$(INTDIR)\maketables.obj" \
	"$(INTDIR)\pcre.obj" \
	"$(INTDIR)\study.obj"

"$(OUTDIR)\pcre.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("pcre.dep")
!INCLUDE "pcre.dep"
!ELSE 
!MESSAGE Warning: cannot find "pcre.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "pcre - Win32 Release" || "$(CFG)" == "pcre - Win32 Debug"
SOURCE="..\..\src\modules-dynamic\re\pcre\get.c"

"$(INTDIR)\get.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE="..\..\src\modules-dynamic\re\pcre\maketables.c"

"$(INTDIR)\maketables.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE="..\..\src\modules-dynamic\re\pcre\pcre.c"

"$(INTDIR)\pcre.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE="..\..\src\modules-dynamic\re\pcre\study.c"

"$(INTDIR)\study.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

