# Microsoft Developer Studio Generated NMAKE File, Based on elastiC.dsp
!IF "$(CFG)" == ""
CFG=elastiC - Win32 Debug
!MESSAGE No configuration specified. Defaulting to elastiC - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "elastiC - Win32 Release" && "$(CFG)" != "elastiC - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "elastiC.mak" CFG="elastiC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "elastiC - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "elastiC - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "elastiC - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\..\elastic\utility.h" "..\..\elastic\user.h" "..\..\elastic\strtable.h" "..\..\elastic\stackrecycle.h" "..\..\elastic\private.h" "..\..\elastic\os.h" "..\..\elastic\oop.h" "..\..\elastic\object.h" "..\..\elastic\modules\string_m.h" "..\..\elastic\modules\array_m.h" "..\..\elastic\memory.h" "..\..\elastic\list.h" "..\..\elastic\lib.h" "..\..\elastic\hashtable.h" "..\..\elastic\hashfunc.h" "..\..\elastic\hashdefs.h" "..\..\elastic\hash.h" "..\..\elastic\floatio.h" "..\..\elastic\file.h" "..\..\elastic\elastic.h" "..\..\elastic\dstring.h" "..\..\elastic\debug.h" "..\..\elastic\conf.h" "..\..\elastic\compile.h" "..\..\elastic\compat.h" "..\..\elastic\bytecode.h" "..\..\elastic\bitstring.h" "..\..\elastic\basic.h" "..\..\elastic\ast.h" "..\..\elastic\arithmetic.h" "$(OUTDIR)\elastiC.dll"


CLEAN :
	-@erase "$(INTDIR)\arithmetic.obj"
	-@erase "$(INTDIR)\array.obj"
	-@erase "$(INTDIR)\array_m.obj"
	-@erase "$(INTDIR)\ast.obj"
	-@erase "$(INTDIR)\builtin.obj"
	-@erase "$(INTDIR)\bytecode.obj"
	-@erase "$(INTDIR)\char.obj"
	-@erase "$(INTDIR)\class.obj"
	-@erase "$(INTDIR)\compat.obj"
	-@erase "$(INTDIR)\compile.obj"
	-@erase "$(INTDIR)\compiled.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\dl-win32.obj"
	-@erase "$(INTDIR)\dllmain.obj"
	-@erase "$(INTDIR)\dstring.obj"
	-@erase "$(INTDIR)\elastic.obj"
	-@erase "$(INTDIR)\elastic_p.obj"
	-@erase "$(INTDIR)\elastic_s.obj"
	-@erase "$(INTDIR)\file.obj"
	-@erase "$(INTDIR)\gc.obj"
	-@erase "$(INTDIR)\handler.obj"
	-@erase "$(INTDIR)\hash.obj"
	-@erase "$(INTDIR)\hashdefs.obj"
	-@erase "$(INTDIR)\hashfunc.obj"
	-@erase "$(INTDIR)\hashtable.obj"
	-@erase "$(INTDIR)\interp.obj"
	-@erase "$(INTDIR)\lib.obj"
	-@erase "$(INTDIR)\list.obj"
	-@erase "$(INTDIR)\list_m.obj"
	-@erase "$(INTDIR)\memory.obj"
	-@erase "$(INTDIR)\numeric.obj"
	-@erase "$(INTDIR)\object.obj"
	-@erase "$(INTDIR)\oop.obj"
	-@erase "$(INTDIR)\os.obj"
	-@erase "$(INTDIR)\package.obj"
	-@erase "$(INTDIR)\packageio.obj"
	-@erase "$(INTDIR)\prime.obj"
	-@erase "$(INTDIR)\printf.obj"
	-@erase "$(INTDIR)\sprintf.obj"
	-@erase "$(INTDIR)\stack.obj"
	-@erase "$(INTDIR)\string.obj"
	-@erase "$(INTDIR)\string_m.obj"
	-@erase "$(INTDIR)\strtable.obj"
	-@erase "$(INTDIR)\strtod.obj"
	-@erase "$(INTDIR)\symbol.obj"
	-@erase "$(INTDIR)\types\vc60.idb"
	-@erase "$(INTDIR)\user.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\elastiC.dll"
	-@erase "$(OUTDIR)\elastiC.exp"
	-@erase "$(OUTDIR)\elastiC.lib"
	-@erase ".\Release\types\object.obj"
	-@erase ".\Release\types\package.obj"
	-@erase "..\..\elastic\arithmetic.h"
	-@erase "..\..\elastic\ast.h"
	-@erase "..\..\elastic\basic.h"
	-@erase "..\..\elastic\bitstring.h"
	-@erase "..\..\elastic\bytecode.h"
	-@erase "..\..\elastic\compat.h"
	-@erase "..\..\elastic\compile.h"
	-@erase "..\..\elastic\conf.h"
	-@erase "..\..\elastic\debug.h"
	-@erase "..\..\elastic\dstring.h"
	-@erase "..\..\elastic\elastic.h"
	-@erase "..\..\elastic\file.h"
	-@erase "..\..\elastic\floatio.h"
	-@erase "..\..\elastic\hash.h"
	-@erase "..\..\elastic\hashdefs.h"
	-@erase "..\..\elastic\hashfunc.h"
	-@erase "..\..\elastic\hashtable.h"
	-@erase "..\..\elastic\lib.h"
	-@erase "..\..\elastic\list.h"
	-@erase "..\..\elastic\memory.h"
	-@erase "..\..\elastic\modules\array_m.h"
	-@erase "..\..\elastic\modules\string_m.h"
	-@erase "..\..\elastic\object.h"
	-@erase "..\..\elastic\oop.h"
	-@erase "..\..\elastic\os.h"
	-@erase "..\..\elastic\private.h"
	-@erase "..\..\elastic\stackrecycle.h"
	-@erase "..\..\elastic\strtable.h"
	-@erase "..\..\elastic\user.h"
	-@erase "..\..\elastic\utility.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\elastiC.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /dll /incremental:no /pdb:"$(OUTDIR)\elastiC.pdb" /machine:I386 /out:"$(OUTDIR)\elastiC.dll" /implib:"$(OUTDIR)\elastiC.lib" 
LINK32_OBJS= \
	"$(INTDIR)\arithmetic.obj" \
	"$(INTDIR)\array_m.obj" \
	"$(INTDIR)\list_m.obj" \
	"$(INTDIR)\string_m.obj" \
	"$(INTDIR)\array.obj" \
	"$(INTDIR)\char.obj" \
	"$(INTDIR)\class.obj" \
	"$(INTDIR)\compiled.obj" \
	"$(INTDIR)\file.obj" \
	"$(INTDIR)\handler.obj" \
	"$(INTDIR)\hash.obj" \
	".\Release\types\object.obj" \
	".\Release\types\package.obj" \
	"$(INTDIR)\stack.obj" \
	"$(INTDIR)\string.obj" \
	"$(INTDIR)\dl-win32.obj" \
	"$(INTDIR)\ast.obj" \
	"$(INTDIR)\builtin.obj" \
	"$(INTDIR)\bytecode.obj" \
	"$(INTDIR)\compat.obj" \
	"$(INTDIR)\compile.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\dllmain.obj" \
	"$(INTDIR)\dstring.obj" \
	"$(INTDIR)\elastic.obj" \
	"$(INTDIR)\elastic_p.obj" \
	"$(INTDIR)\elastic_s.obj" \
	"$(INTDIR)\gc.obj" \
	"$(INTDIR)\hashdefs.obj" \
	"$(INTDIR)\hashfunc.obj" \
	"$(INTDIR)\hashtable.obj" \
	"$(INTDIR)\interp.obj" \
	"$(INTDIR)\lib.obj" \
	"$(INTDIR)\list.obj" \
	"$(INTDIR)\memory.obj" \
	"$(INTDIR)\numeric.obj" \
	"$(INTDIR)\object.obj" \
	"$(INTDIR)\oop.obj" \
	"$(INTDIR)\os.obj" \
	"$(INTDIR)\package.obj" \
	"$(INTDIR)\packageio.obj" \
	"$(INTDIR)\prime.obj" \
	"$(INTDIR)\printf.obj" \
	"$(INTDIR)\sprintf.obj" \
	"$(INTDIR)\strtable.obj" \
	"$(INTDIR)\strtod.obj" \
	"$(INTDIR)\symbol.obj" \
	"$(INTDIR)\user.obj" \
	"$(INTDIR)\utility.obj"

"$(OUTDIR)\elastiC.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\elastic\stackrecycle.h" "..\..\elastic\modules\string_m.h" "..\..\elastic\modules\array_m.h" "..\..\elastic\memory.h" "..\..\elastic\list.h" "..\..\elastic\hashfunc.h" "..\..\elastic\floatio.h" "..\..\elastic\arithmetic.h" "$(OUTDIR)\elastiC.dll" "$(OUTDIR)\elastiC.bsc"


CLEAN :
	-@erase "$(INTDIR)\arithmetic\vc60.idb"
	-@erase "$(INTDIR)\arithmetic\vc60.pdb"
	-@erase "$(INTDIR)\ast.obj"
	-@erase "$(INTDIR)\ast.sbr"
	-@erase "$(INTDIR)\builtin.obj"
	-@erase "$(INTDIR)\builtin.sbr"
	-@erase "$(INTDIR)\bytecode.obj"
	-@erase "$(INTDIR)\bytecode.sbr"
	-@erase "$(INTDIR)\compat.obj"
	-@erase "$(INTDIR)\compat.sbr"
	-@erase "$(INTDIR)\compile.obj"
	-@erase "$(INTDIR)\compile.sbr"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\debug.sbr"
	-@erase "$(INTDIR)\dl-win32.obj"
	-@erase "$(INTDIR)\dl-win32.sbr"
	-@erase "$(INTDIR)\dllmain.obj"
	-@erase "$(INTDIR)\dllmain.sbr"
	-@erase "$(INTDIR)\dstring.obj"
	-@erase "$(INTDIR)\dstring.sbr"
	-@erase "$(INTDIR)\elastic.obj"
	-@erase "$(INTDIR)\elastic.sbr"
	-@erase "$(INTDIR)\elastic_p.obj"
	-@erase "$(INTDIR)\elastic_p.sbr"
	-@erase "$(INTDIR)\elastic_s.obj"
	-@erase "$(INTDIR)\elastic_s.sbr"
	-@erase "$(INTDIR)\gc.obj"
	-@erase "$(INTDIR)\gc.sbr"
	-@erase "$(INTDIR)\hashdefs.obj"
	-@erase "$(INTDIR)\hashdefs.sbr"
	-@erase "$(INTDIR)\hashfunc.obj"
	-@erase "$(INTDIR)\hashfunc.sbr"
	-@erase "$(INTDIR)\hashtable.obj"
	-@erase "$(INTDIR)\hashtable.sbr"
	-@erase "$(INTDIR)\interp.obj"
	-@erase "$(INTDIR)\interp.sbr"
	-@erase "$(INTDIR)\lib.obj"
	-@erase "$(INTDIR)\lib.sbr"
	-@erase "$(INTDIR)\list.obj"
	-@erase "$(INTDIR)\list.sbr"
	-@erase "$(INTDIR)\memory.obj"
	-@erase "$(INTDIR)\memory.sbr"
	-@erase "$(INTDIR)\modules\vc60.idb"
	-@erase "$(INTDIR)\modules\vc60.pdb"
	-@erase "$(INTDIR)\numeric.obj"
	-@erase "$(INTDIR)\numeric.sbr"
	-@erase "$(INTDIR)\object.obj"
	-@erase "$(INTDIR)\object.sbr"
	-@erase "$(INTDIR)\oop.obj"
	-@erase "$(INTDIR)\oop.sbr"
	-@erase "$(INTDIR)\os.obj"
	-@erase "$(INTDIR)\os.sbr"
	-@erase "$(INTDIR)\package.obj"
	-@erase "$(INTDIR)\package.sbr"
	-@erase "$(INTDIR)\packageio.obj"
	-@erase "$(INTDIR)\packageio.sbr"
	-@erase "$(INTDIR)\prime.obj"
	-@erase "$(INTDIR)\prime.sbr"
	-@erase "$(INTDIR)\printf.obj"
	-@erase "$(INTDIR)\printf.sbr"
	-@erase "$(INTDIR)\sprintf.obj"
	-@erase "$(INTDIR)\sprintf.sbr"
	-@erase "$(INTDIR)\strtable.obj"
	-@erase "$(INTDIR)\strtable.sbr"
	-@erase "$(INTDIR)\strtod.obj"
	-@erase "$(INTDIR)\strtod.sbr"
	-@erase "$(INTDIR)\symbol.obj"
	-@erase "$(INTDIR)\symbol.sbr"
	-@erase "$(INTDIR)\types\vc60.idb"
	-@erase "$(INTDIR)\types\vc60.pdb"
	-@erase "$(INTDIR)\user.obj"
	-@erase "$(INTDIR)\user.sbr"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\utility.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\elastiC.bsc"
	-@erase "$(OUTDIR)\elastiC.dll"
	-@erase "$(OUTDIR)\elastiC.exp"
	-@erase "$(OUTDIR)\elastiC.ilk"
	-@erase "$(OUTDIR)\elastiC.lib"
	-@erase "$(OUTDIR)\elastiC.pdb"
	-@erase ".\Debug\arithmetic\arithmetic.obj"
	-@erase ".\Debug\arithmetic\arithmetic.sbr"
	-@erase ".\Debug\modules\array_m.obj"
	-@erase ".\Debug\modules\array_m.sbr"
	-@erase ".\Debug\modules\list_m.obj"
	-@erase ".\Debug\modules\list_m.sbr"
	-@erase ".\Debug\modules\string_m.obj"
	-@erase ".\Debug\modules\string_m.sbr"
	-@erase ".\Debug\types\array.obj"
	-@erase ".\Debug\types\array.sbr"
	-@erase ".\Debug\types\char.obj"
	-@erase ".\Debug\types\char.sbr"
	-@erase ".\Debug\types\class.obj"
	-@erase ".\Debug\types\class.sbr"
	-@erase ".\Debug\types\compiled.obj"
	-@erase ".\Debug\types\compiled.sbr"
	-@erase ".\Debug\types\file.obj"
	-@erase ".\Debug\types\file.sbr"
	-@erase ".\Debug\types\handler.obj"
	-@erase ".\Debug\types\handler.sbr"
	-@erase ".\Debug\types\hash.obj"
	-@erase ".\Debug\types\hash.sbr"
	-@erase ".\Debug\types\object.obj"
	-@erase ".\Debug\types\object.sbr"
	-@erase ".\Debug\types\package.obj"
	-@erase ".\Debug\types\package.sbr"
	-@erase ".\Debug\types\stack.obj"
	-@erase ".\Debug\types\stack.sbr"
	-@erase ".\Debug\types\string.obj"
	-@erase ".\Debug\types\string.sbr"
	-@erase "..\..\elastic\arithmetic.h"
	-@erase "..\..\elastic\floatio.h"
	-@erase "..\..\elastic\hashfunc.h"
	-@erase "..\..\elastic\list.h"
	-@erase "..\..\elastic\memory.h"
	-@erase "..\..\elastic\modules\array_m.h"
	-@erase "..\..\elastic\modules\string_m.h"
	-@erase "..\..\elastic\stackrecycle.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\elastiC.bsc" 
BSC32_SBRS= \
	".\Debug\arithmetic\arithmetic.sbr" \
	".\Debug\modules\array_m.sbr" \
	".\Debug\modules\list_m.sbr" \
	".\Debug\modules\string_m.sbr" \
	".\Debug\types\array.sbr" \
	".\Debug\types\char.sbr" \
	".\Debug\types\class.sbr" \
	".\Debug\types\compiled.sbr" \
	".\Debug\types\file.sbr" \
	".\Debug\types\handler.sbr" \
	".\Debug\types\hash.sbr" \
	".\Debug\types\object.sbr" \
	".\Debug\types\package.sbr" \
	".\Debug\types\stack.sbr" \
	".\Debug\types\string.sbr" \
	"$(INTDIR)\dl-win32.sbr" \
	"$(INTDIR)\ast.sbr" \
	"$(INTDIR)\builtin.sbr" \
	"$(INTDIR)\bytecode.sbr" \
	"$(INTDIR)\compat.sbr" \
	"$(INTDIR)\compile.sbr" \
	"$(INTDIR)\debug.sbr" \
	"$(INTDIR)\dllmain.sbr" \
	"$(INTDIR)\dstring.sbr" \
	"$(INTDIR)\elastic.sbr" \
	"$(INTDIR)\elastic_p.sbr" \
	"$(INTDIR)\elastic_s.sbr" \
	"$(INTDIR)\gc.sbr" \
	"$(INTDIR)\hashdefs.sbr" \
	"$(INTDIR)\hashfunc.sbr" \
	"$(INTDIR)\hashtable.sbr" \
	"$(INTDIR)\interp.sbr" \
	"$(INTDIR)\lib.sbr" \
	"$(INTDIR)\list.sbr" \
	"$(INTDIR)\memory.sbr" \
	"$(INTDIR)\numeric.sbr" \
	"$(INTDIR)\object.sbr" \
	"$(INTDIR)\oop.sbr" \
	"$(INTDIR)\os.sbr" \
	"$(INTDIR)\package.sbr" \
	"$(INTDIR)\packageio.sbr" \
	"$(INTDIR)\prime.sbr" \
	"$(INTDIR)\printf.sbr" \
	"$(INTDIR)\sprintf.sbr" \
	"$(INTDIR)\strtable.sbr" \
	"$(INTDIR)\strtod.sbr" \
	"$(INTDIR)\symbol.sbr" \
	"$(INTDIR)\user.sbr" \
	"$(INTDIR)\utility.sbr"

"$(OUTDIR)\elastiC.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /dll /incremental:yes /pdb:"$(OUTDIR)\elastiC.pdb" /debug /machine:I386 /out:"$(OUTDIR)\elastiC.dll" /implib:"$(OUTDIR)\elastiC.lib" /pdbtype:sept 
LINK32_OBJS= \
	".\Debug\arithmetic\arithmetic.obj" \
	".\Debug\modules\array_m.obj" \
	".\Debug\modules\list_m.obj" \
	".\Debug\modules\string_m.obj" \
	".\Debug\types\array.obj" \
	".\Debug\types\char.obj" \
	".\Debug\types\class.obj" \
	".\Debug\types\compiled.obj" \
	".\Debug\types\file.obj" \
	".\Debug\types\handler.obj" \
	".\Debug\types\hash.obj" \
	".\Debug\types\object.obj" \
	".\Debug\types\package.obj" \
	".\Debug\types\stack.obj" \
	".\Debug\types\string.obj" \
	"$(INTDIR)\dl-win32.obj" \
	"$(INTDIR)\ast.obj" \
	"$(INTDIR)\builtin.obj" \
	"$(INTDIR)\bytecode.obj" \
	"$(INTDIR)\compat.obj" \
	"$(INTDIR)\compile.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\dllmain.obj" \
	"$(INTDIR)\dstring.obj" \
	"$(INTDIR)\elastic.obj" \
	"$(INTDIR)\elastic_p.obj" \
	"$(INTDIR)\elastic_s.obj" \
	"$(INTDIR)\gc.obj" \
	"$(INTDIR)\hashdefs.obj" \
	"$(INTDIR)\hashfunc.obj" \
	"$(INTDIR)\hashtable.obj" \
	"$(INTDIR)\interp.obj" \
	"$(INTDIR)\lib.obj" \
	"$(INTDIR)\list.obj" \
	"$(INTDIR)\memory.obj" \
	"$(INTDIR)\numeric.obj" \
	"$(INTDIR)\object.obj" \
	"$(INTDIR)\oop.obj" \
	"$(INTDIR)\os.obj" \
	"$(INTDIR)\package.obj" \
	"$(INTDIR)\packageio.obj" \
	"$(INTDIR)\prime.obj" \
	"$(INTDIR)\printf.obj" \
	"$(INTDIR)\sprintf.obj" \
	"$(INTDIR)\strtable.obj" \
	"$(INTDIR)\strtod.obj" \
	"$(INTDIR)\symbol.obj" \
	"$(INTDIR)\user.obj" \
	"$(INTDIR)\utility.obj"

"$(OUTDIR)\elastiC.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("elastiC.dep")
!INCLUDE "elastiC.dep"
!ELSE 
!MESSAGE Warning: cannot find "elastiC.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "elastiC - Win32 Release" || "$(CFG)" == "elastiC - Win32 Debug"
SOURCE=..\..\src\arithmetic\arithmetic.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\arithmetic.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\arithmetic/" /Fo"Debug\arithmetic/" /Fd"Debug\arithmetic/" /FD /GZ /c 

".\Debug\arithmetic\arithmetic.obj"	".\Debug\arithmetic\arithmetic.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\modules\array_m.c

!IF  "$(CFG)" == "elastiC - Win32 Release"


"$(INTDIR)\array_m.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"


".\Debug\modules\array_m.obj"	".\Debug\modules\array_m.sbr" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\modules\list_m.c

!IF  "$(CFG)" == "elastiC - Win32 Release"


"$(INTDIR)\list_m.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"


".\Debug\modules\list_m.obj"	".\Debug\modules\list_m.sbr" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\modules\string_m.c

!IF  "$(CFG)" == "elastiC - Win32 Release"


"$(INTDIR)\string_m.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"


".\Debug\modules\string_m.obj"	".\Debug\modules\string_m.sbr" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\types\array.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\array.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\types/" /Fo"Debug\types/" /Fd"Debug\types/" /FD /GZ /c 

".\Debug\types\array.obj"	".\Debug\types\array.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\types\char.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\char.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\types/" /Fo"Debug\types/" /Fd"Debug\types/" /FD /GZ /c 

".\Debug\types\char.obj"	".\Debug\types\char.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\types\class.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\class.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\types/" /Fo"Debug\types/" /Fd"Debug\types/" /FD /GZ /c 

".\Debug\types\class.obj"	".\Debug\types\class.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\types\compiled.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\compiled.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\types/" /Fo"Debug\types/" /Fd"Debug\types/" /FD /GZ /c 

".\Debug\types\compiled.obj"	".\Debug\types\compiled.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\types\file.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\file.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\types/" /Fo"Debug\types/" /Fd"Debug\types/" /FD /GZ /c 

".\Debug\types\file.obj"	".\Debug\types\file.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\types\handler.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\handler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\types/" /Fo"Debug\types/" /Fd"Debug\types/" /FD /GZ /c 

".\Debug\types\handler.obj"	".\Debug\types\handler.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\types\hash.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\hash.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\types/" /Fo"Debug\types/" /Fd"Debug\types/" /FD /GZ /c 

".\Debug\types\hash.obj"	".\Debug\types\hash.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\types\object.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"Release\types/" /Fd"Release\types/" /FD /c 

".\Release\types\object.obj" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\types/" /Fo"Debug\types/" /Fd"Debug\types/" /FD /GZ /c 

".\Debug\types\object.obj"	".\Debug\types\object.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\types\package.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"Release\types/" /Fd"Release\types/" /FD /c 

".\Release\types\package.obj" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\types/" /Fo"Debug\types/" /Fd"Debug\types/" /FD /GZ /c 

".\Debug\types\package.obj"	".\Debug\types\package.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\types\stack.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stack.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\types/" /Fo"Debug\types/" /Fd"Debug\types/" /FD /GZ /c 

".\Debug\types\stack.obj"	".\Debug\types\stack.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\types\string.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\string.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"Debug\types/" /Fo"Debug\types/" /Fd"Debug\types/" /FD /GZ /c 

".\Debug\types\string.obj"	".\Debug\types\string.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE="..\..\src\dl\dl-win32.c"

!IF  "$(CFG)" == "elastiC - Win32 Release"


"$(INTDIR)\dl-win32.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"


"$(INTDIR)\dl-win32.obj"	"$(INTDIR)\dl-win32.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\debug.h" "..\..\elastic\hash.h" "..\..\elastic\oop.h" "..\..\elastic\object.h" "..\..\elastic\dstring.h" "..\..\elastic\cnf.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\bytecode.h" "..\..\elastic\conf.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\os.h" "..\..\elastic\hashtable.h" "..\..\elastic\user.h" "..\..\elastic\basic.h" "..\..\elastic\hashdefs.h" "..\..\elastic\config.h"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\src\ast.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ast.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ast.obj"	"$(INTDIR)\ast.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\src\debug.h" "..\..\elastic\basic.h" "..\..\elastic\cnf.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\src\ast.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\compile.h" "..\..\src\elastic.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\src\compat.h" "..\..\elastic\bitstring.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\builtin.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\builtin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /I "../../src/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\builtin.obj"	"$(INTDIR)\builtin.sbr" : $(SOURCE) "$(INTDIR)" "..\..\src\basic.h" "..\..\elastic\hash.h" "..\..\elastic\compile.h" "..\..\src\elastic.h" "..\..\elastic\elastic.h" "..\..\elastic\hashdefs.h" "..\..\elastic\user.h" "..\..\elastic\utility.h" "..\..\src\debug.h" "..\..\elastic\hashtable.h" "..\..\elastic\tsize.h" "..\..\elastic\lib.h" "..\..\src\private.h" "..\..\elastic\os.h" "..\..\elastic\bytecode.h" "..\..\elastic\ast.h" "..\..\elastic\dstring.h" "..\..\elastic\file.h" "..\..\elastic\config.h" "..\..\elastic\object.h" "..\..\elastic\strtable.h" "..\..\elastic\bitstring.h" "..\..\elastic\basic.h" "..\..\elastic\oop.h" "..\..\elastic\conf.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\bytecode.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\bytecode.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\bytecode.obj"	"$(INTDIR)\bytecode.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\src\bytecode.h" "..\..\elastic\cnf.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\compat.h" "..\..\elastic\bytecode.h" "..\..\src\basic.h" "..\..\src\debug.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\compat.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\compat.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\compat.obj"	"$(INTDIR)\compat.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\compat.h" "..\..\src\basic.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\compile.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\compile.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\compile.obj"	"$(INTDIR)\compile.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\cnf.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\src\ast.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\compile.h" "..\..\src\elastic.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\compat.h" "..\..\src\basic.h" "..\..\elastic\bytecode.h" "..\..\src\debug.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\debug.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\debug.obj"	"$(INTDIR)\debug.sbr" : $(SOURCE) "$(INTDIR)" "..\..\src\basic.h" "..\..\elastic\hash.h" "..\..\elastic\compile.h" "..\..\src\elastic.h" "..\..\elastic\elastic.h" "..\..\elastic\hashdefs.h" "..\..\elastic\user.h" "..\..\elastic\utility.h" "..\..\src\debug.h" "..\..\elastic\hashtable.h" "..\..\elastic\tsize.h" "..\..\elastic\lib.h" "..\..\src\private.h" "..\..\elastic\os.h" "..\..\elastic\bytecode.h" "..\..\elastic\ast.h" "..\..\elastic\dstring.h" "..\..\elastic\file.h" "..\..\elastic\config.h" "..\..\elastic\object.h" "..\..\elastic\strtable.h" "..\..\elastic\bitstring.h" "..\..\elastic\basic.h" "..\..\elastic\oop.h" "..\..\elastic\conf.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\dllmain.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\dllmain.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\dllmain.obj"	"$(INTDIR)\dllmain.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\dstring.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\dstring.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\dstring.obj"	"$(INTDIR)\dstring.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\dstring.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\compat.h" "..\..\elastic\bytecode.h" "..\..\src\basic.h" "..\..\src\debug.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\elastic.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\elastic.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\elastic.obj"	"$(INTDIR)\elastic.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\debug.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\src\ast.h" "..\..\src\bitstring.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\src\stackrecycle.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\elastic\private.h" "..\..\src\elastic.h" "..\..\elastic\user.h" "..\..\elastic\dstring.h" "..\..\src\private.h" "..\..\elastic\conf.h" "..\..\elastic\bitstring.h" "..\..\src\debug.h" "..\..\src\basic.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\elastic_p.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\elastic_p.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\elastic_p.obj"	"$(INTDIR)\elastic_p.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\cnf.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\src\ast.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\elastic.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\compat.h" "..\..\src\basic.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\elastic_s.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\elastic_s.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\elastic_s.obj"	"$(INTDIR)\elastic_s.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\src\ast.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\compat.h" "..\..\src\basic.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\gc.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\gc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\gc.obj"	"$(INTDIR)\gc.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\src\debug.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\src\object.h" "..\..\src\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\src\compat.h" "..\..\elastic\bitstring.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\hashdefs.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\hashdefs.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\hashdefs.obj"	"$(INTDIR)\hashdefs.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\elastic.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\src\compat.h" "..\..\elastic\bitstring.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\hashfunc.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\hashfunc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\hashfunc.obj"	"$(INTDIR)\hashfunc.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\src\hashfunc.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\elastic\bitstring.h" "..\..\src\compat.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\hashtable.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\hashtable.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\hashtable.obj"	"$(INTDIR)\hashtable.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\src\debug.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\src\hashfunc.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\elastic.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\src\compat.h" "..\..\elastic\bitstring.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h" "..\..\src\hashtable.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\interp.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\interp.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\interp.obj"	"$(INTDIR)\interp.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\debug.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\src\stackrecycle.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\elastic\private.h" "..\..\src\elastic.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\debug.h" "..\..\src\basic.h" "..\..\elastic\bytecode.h" "..\..\src\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\lib.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\lib.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\lib.obj"	"$(INTDIR)\lib.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\src\debug.h" "..\..\elastic\basic.h" "..\..\elastic\cnf.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\elastic.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\elastic\bitstring.h" "..\..\src\compat.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\list.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\list.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\list.obj"	"$(INTDIR)\list.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\utility.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\debug.h" "..\..\src\basic.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\memory.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\memory.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\memory.obj"	"$(INTDIR)\memory.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\src\debug.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\src\conf.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\src\compat.h" "..\..\elastic\bitstring.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\numeric.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\numeric.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\numeric.obj"	"$(INTDIR)\numeric.sbr" : $(SOURCE) "$(INTDIR)" "..\..\src\basic.h" "..\..\elastic\hash.h" "..\..\elastic\compile.h" "..\..\src\elastic.h" "..\..\elastic\elastic.h" "..\..\elastic\hashdefs.h" "..\..\elastic\user.h" "..\..\elastic\utility.h" "..\..\src\debug.h" "..\..\elastic\hashtable.h" "..\..\elastic\tsize.h" "..\..\elastic\lib.h" "..\..\src\private.h" "..\..\elastic\os.h" "..\..\elastic\bytecode.h" "..\..\elastic\ast.h" "..\..\elastic\dstring.h" "..\..\elastic\file.h" "..\..\elastic\config.h" "..\..\elastic\object.h" "..\..\elastic\strtable.h" "..\..\elastic\bitstring.h" "..\..\elastic\basic.h" "..\..\elastic\oop.h" "..\..\elastic\conf.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\object.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\object.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\object.obj"	"$(INTDIR)\object.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\src\debug.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\src\object.h" "..\..\src\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\hashfunc.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\src\compat.h" "..\..\elastic\bitstring.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\oop.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\oop.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\oop.obj"	"$(INTDIR)\oop.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\debug.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\src\debug.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\src\stackrecycle.h" "..\..\elastic\compile.h" "..\..\elastic\private.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\elastic.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\src\compat.h" "..\..\elastic\bitstring.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\win32\os.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\os.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\os.obj"	"$(INTDIR)\os.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\private.h" "..\..\elastic\hashdefs.h" "..\..\elastic\compat.h" "..\..\elastic\tsize.h" "..\..\elastic\debug.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\cnf.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\elastic\user.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\package.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\package.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\package.obj"	"$(INTDIR)\package.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\src\debug.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\elastic.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\elastic\bitstring.h" "..\..\src\compat.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\packageio.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\packageio.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\packageio.obj"	"$(INTDIR)\packageio.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\debug.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\src\stackrecycle.h" "..\..\src\hashdefs.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\elastic\private.h" "..\..\src\elastic.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\compat.h" "..\..\src\debug.h" "..\..\src\basic.h" "..\..\elastic\bytecode.h" "..\..\src\hashtable.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\prime.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\prime.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\prime.obj"	"$(INTDIR)\prime.sbr" : $(SOURCE) "$(INTDIR)" "..\..\src\basic.h" "..\..\elastic\hash.h" "..\..\elastic\compile.h" "..\..\elastic\elastic.h" "..\..\elastic\hashdefs.h" "..\..\elastic\user.h" "..\..\elastic\utility.h" "..\..\elastic\hashtable.h" "..\..\elastic\tsize.h" "..\..\elastic\lib.h" "..\..\src\private.h" "..\..\elastic\os.h" "..\..\elastic\bytecode.h" "..\..\elastic\ast.h" "..\..\elastic\dstring.h" "..\..\elastic\file.h" "..\..\elastic\config.h" "..\..\elastic\object.h" "..\..\elastic\strtable.h" "..\..\elastic\bitstring.h" "..\..\elastic\basic.h" "..\..\elastic\oop.h" "..\..\elastic\conf.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\printf.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\printf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\printf.obj"	"$(INTDIR)\printf.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\src\debug.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\src\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\src\compat.h" "..\..\elastic\bitstring.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\sprintf.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\sprintf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\sprintf.obj"	"$(INTDIR)\sprintf.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\src\floatio.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\dstring.h" "..\..\src\elastic.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\compat.h" "..\..\src\basic.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\strtable.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\strtable.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\strtable.obj"	"$(INTDIR)\strtable.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\src\debug.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\src\object.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\hashfunc.h" "..\..\src\private.h" "..\..\elastic\user.h" "..\..\src\compat.h" "..\..\elastic\bitstring.h" "..\..\elastic\dstring.h" "..\..\src\basic.h" "..\..\elastic\conf.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\strtod.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\strtod.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\strtod.obj"	"$(INTDIR)\strtod.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\basic.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\symbol.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\symbol.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\symbol.obj"	"$(INTDIR)\symbol.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\src\utility.h" "..\..\elastic\object.h" "..\..\src\elastic.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\debug.h" "..\..\src\compat.h" "..\..\elastic\bytecode.h" "..\..\src\basic.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\user.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\user.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\user.obj"	"$(INTDIR)\user.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\elastic\object.h" "..\..\src\elastic.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\debug.h" "..\..\src\compat.h" "..\..\elastic\bytecode.h" "..\..\src\basic.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\utility.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\utility.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\utility.obj"	"$(INTDIR)\utility.sbr" : $(SOURCE) "$(INTDIR)" "..\..\elastic\hashtable.h" "..\..\elastic\oop.h" "..\..\elastic\hashdefs.h" "..\..\elastic\tsize.h" "..\..\elastic\file.h" "..\..\elastic\hash.h" "..\..\elastic\elastic.h" "..\..\elastic\lib.h" "..\..\elastic\basic.h" "..\..\elastic\config.h" "..\..\elastic\compile.h" "..\..\elastic\ast.h" "..\..\elastic\strtable.h" "..\..\elastic\utility.h" "..\..\elastic\os.h" "..\..\src\utility.h" "..\..\elastic\object.h" "..\..\elastic\user.h" "..\..\src\private.h" "..\..\elastic\dstring.h" "..\..\elastic\bitstring.h" "..\..\elastic\conf.h" "..\..\src\compat.h" "..\..\src\debug.h" "..\..\src\basic.h" "..\..\elastic\bytecode.h"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\src\modules\array_m.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\modules\array_m.h

"..\..\elastic\modules\array_m.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\modules\array_m.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\modules\array_m.h

"..\..\elastic\modules\array_m.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\modules\array_m.h
<< 
	

!ENDIF 

SOURCE=..\..\src\modules\string_m.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\modules\string_m.h

"..\..\elastic\modules\string_m.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\modules\string_m.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\modules\string_m.h

"..\..\elastic\modules\string_m.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\modules\string_m.h
<< 
	

!ENDIF 

SOURCE=..\..\src\arithmetic.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\arithmetic.h

"..\..\elastic\arithmetic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\arithmetic.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\arithmetic.h

"..\..\elastic\arithmetic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\arithmetic.h
<< 
	

!ENDIF 

SOURCE=..\..\src\ast.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\ast.h

"..\..\elastic\ast.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\ast.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\ast.h

"..\..\elastic\ast.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\ast.h
<< 
	

!ENDIF 

SOURCE=..\..\src\basic.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\basic.h

"..\..\elastic\basic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\basic.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\basic.h

"..\..\elastic\basic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\basic.h
<< 
	

!ENDIF 

SOURCE=..\..\src\bitstring.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\bitstring.h

"..\..\elastic\bitstring.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\bitstring.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\bitstring.h

"..\..\elastic\bitstring.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\bitstring.h
<< 
	

!ENDIF 

SOURCE=..\..\src\bytecode.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\bytecode.h

"..\..\elastic\bytecode.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\bytecode.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\bytecode.h

"..\..\elastic\bytecode.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\bytecode.h
<< 
	

!ENDIF 

SOURCE=..\cnf.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\cnf.h

"..\..\elastic\cnf.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\cnf.h
<< 
	

!ENDIF 

SOURCE=..\..\src\compat.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\compat.h

"..\..\elastic\compat.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\compat.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\compat.h

"..\..\elastic\compat.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\compat.h
<< 
	

!ENDIF 

SOURCE=..\..\src\compile.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\compile.h

"..\..\elastic\compile.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\compile.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\compile.h

"..\..\elastic\compile.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\compile.h
<< 
	

!ENDIF 

SOURCE=..\..\src\conf.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\conf.h

"..\..\elastic\conf.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\conf.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\conf.h

"..\..\elastic\conf.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\conf.h
<< 
	

!ENDIF 

SOURCE=..\config.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\config.h

"..\..\elastic\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\config.h
<< 
	

!ENDIF 

SOURCE=..\..\src\debug.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\debug.h

"..\..\elastic\debug.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\debug.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\debug.h

"..\..\elastic\debug.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\debug.h
<< 
	

!ENDIF 

SOURCE=..\..\src\dstring.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\dstring.h

"..\..\elastic\dstring.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\dstring.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\dstring.h

"..\..\elastic\dstring.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\dstring.h
<< 
	

!ENDIF 

SOURCE=..\..\src\elastic.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\elastic.h

"..\..\elastic\elastic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\elastic.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\elastic.h

"..\..\elastic\elastic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\elastic.h
<< 
	

!ENDIF 

SOURCE=..\..\src\file.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\file.h

"..\..\elastic\file.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\file.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\file.h

"..\..\elastic\file.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\file.h
<< 
	

!ENDIF 

SOURCE=..\..\src\floatio.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\floatio.h

"..\..\elastic\floatio.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\floatio.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\floatio.h

"..\..\elastic\floatio.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\floatio.h
<< 
	

!ENDIF 

SOURCE=..\..\src\hash.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\hash.h

"..\..\elastic\hash.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\hash.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\hash.h

"..\..\elastic\hash.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\hash.h
<< 
	

!ENDIF 

SOURCE=..\..\src\hashdefs.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\hashdefs.h

"..\..\elastic\hashdefs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\hashdefs.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\hashdefs.h

"..\..\elastic\hashdefs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\hashdefs.h
<< 
	

!ENDIF 

SOURCE=..\..\src\hashfunc.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\hashfunc.h

"..\..\elastic\hashfunc.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\hashfunc.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\hashfunc.h

"..\..\elastic\hashfunc.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\hashfunc.h
<< 
	

!ENDIF 

SOURCE=..\..\src\hashtable.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\hashtable.h

"..\..\elastic\hashtable.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\hashtable.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\hashtable.h

"..\..\elastic\hashtable.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\hashtable.h
<< 
	

!ENDIF 

SOURCE=..\..\src\lib.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\lib.h

"..\..\elastic\lib.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\lib.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\lib.h

"..\..\elastic\lib.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\lib.h
<< 
	

!ENDIF 

SOURCE=..\..\src\list.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\list.h

"..\..\elastic\list.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\list.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\list.h

"..\..\elastic\list.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\list.h
<< 
	

!ENDIF 

SOURCE=..\..\src\memory.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\memory.h

"..\..\elastic\memory.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\memory.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\memory.h

"..\..\elastic\memory.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\memory.h
<< 
	

!ENDIF 

SOURCE=..\..\src\object.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\object.h

"..\..\elastic\object.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\object.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\object.h

"..\..\elastic\object.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\object.h
<< 
	

!ENDIF 

SOURCE=..\..\src\oop.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\oop.h

"..\..\elastic\oop.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\oop.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\oop.h

"..\..\elastic\oop.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\oop.h
<< 
	

!ENDIF 

SOURCE=..\..\src\os.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\os.h

"..\..\elastic\os.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\os.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\os.h

"..\..\elastic\os.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\os.h
<< 
	

!ENDIF 

SOURCE=..\..\src\private.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\private.h

"..\..\elastic\private.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\private.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\private.h

"..\..\elastic\private.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\private.h
<< 
	

!ENDIF 

SOURCE=..\..\src\stackrecycle.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\stackrecycle.h

"..\..\elastic\stackrecycle.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\stackrecycle.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\stackrecycle.h

"..\..\elastic\stackrecycle.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\stackrecycle.h
<< 
	

!ENDIF 

SOURCE=..\..\src\strtable.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\strtable.h

"..\..\elastic\strtable.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\strtable.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\strtable.h

"..\..\elastic\strtable.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\strtable.h
<< 
	

!ENDIF 

SOURCE=..\tsize.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\tsize.h

"..\..\elastic\tsize.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\tsize.h
<< 
	

!ENDIF 

SOURCE=..\..\src\user.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\user.h

"..\..\elastic\user.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\user.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\user.h

"..\..\elastic\user.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\user.h
<< 
	

!ENDIF 

SOURCE=..\..\src\utility.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

InputPath=..\..\src\utility.h

"..\..\elastic\utility.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\utility.h
<< 
	

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

InputPath=..\..\src\utility.h

"..\..\elastic\utility.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	COPY $(InputPath) ..\..\elastic\utility.h
<< 
	

!ENDIF 


!ENDIF 

