# Microsoft Developer Studio Project File - Name="elastiC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=elastiC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "elastiC.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "elastiC - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\elastic" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "__i386__" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\.." /I "..\..\elastic" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ELASTIC_EXPORTS" /D "DEBUG" /D "__i386__" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "elastiC - Win32 Release"
# Name "elastiC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "arithmetic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\arithmetic\arithmetic.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\arithmetic"
# SUBTRACT CPP /X

!ENDIF 

# End Source File
# End Group
# Begin Group "modules"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\modules\array_m.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\modules\list_m.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\modules\string_m.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\modules"

!ENDIF 

# End Source File
# End Group
# Begin Group "types"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\types\array.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\types\char.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\types\class.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\types\compiled.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\types\file.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\types\handler.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\types\hash.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\types\object.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# PROP Intermediate_Dir "Release\types"
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\types\package.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# PROP Intermediate_Dir "Release\types"
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\types\stack.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\types\string.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# PROP Intermediate_Dir "Debug\types"

!ENDIF 

# End Source File
# End Group
# Begin Group "dl"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\src\dl\dl-win32.c"
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\ast.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\builtin.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# ADD CPP /I "../../src/"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\bytecode.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\compat.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\compile.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\debug.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dllmain.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\dstring.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\elastic.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\elastic_p.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\elastic_s.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\gc.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\hashdefs.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\hashfunc.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\hashtable.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\interp.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\lib.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\list.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\memory.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\numeric.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\object.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\oop.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\win32\os.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\package.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\packageio.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\prime.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\printf.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\sprintf.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\strtable.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\strtod.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\symbol.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\user.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\utility.c

!IF  "$(CFG)" == "elastiC - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "modules (.h)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\modules\array_m.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\modules\array_m.h

"..\..\elastic\modules\array_m.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\modules\array_m.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\modules\array_m.h

"..\..\elastic\modules\array_m.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\modules\array_m.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\modules\string_m.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\modules\string_m.h

"..\..\elastic\modules\string_m.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\modules\string_m.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\modules\string_m.h

"..\..\elastic\modules\string_m.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\modules\string_m.h

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\arithmetic.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\arithmetic.h

"..\..\elastic\arithmetic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\arithmetic.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\arithmetic.h

"..\..\elastic\arithmetic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\arithmetic.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\ast.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\ast.h

"..\..\elastic\ast.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\ast.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\ast.h

"..\..\elastic\ast.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\ast.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\basic.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\basic.h

"..\..\elastic\basic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\basic.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\basic.h

"..\..\elastic\basic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\basic.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\bitstring.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\bitstring.h

"..\..\elastic\bitstring.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\bitstring.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\bitstring.h

"..\..\elastic\bitstring.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\bitstring.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\bytecode.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\bytecode.h

"..\..\elastic\bytecode.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\bytecode.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\bytecode.h

"..\..\elastic\bytecode.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\bytecode.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\win32\cnf.h
# End Source File
# Begin Source File

SOURCE=..\..\src\compat.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\compat.h

"..\..\elastic\compat.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\compat.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\compat.h

"..\..\elastic\compat.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\compat.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\compile.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\compile.h

"..\..\elastic\compile.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\compile.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\compile.h

"..\..\elastic\compile.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\compile.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\conf.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\conf.h

"..\..\elastic\conf.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\conf.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\conf.h

"..\..\elastic\conf.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\conf.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\debug.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\debug.h

"..\..\elastic\debug.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\debug.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\debug.h

"..\..\elastic\debug.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\debug.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\dstring.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\dstring.h

"..\..\elastic\dstring.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\dstring.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\dstring.h

"..\..\elastic\dstring.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\dstring.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\elastic.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\elastic.h

"..\..\elastic\elastic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\elastic.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\elastic.h

"..\..\elastic\elastic.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\elastic.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\elastic_p.h
# End Source File
# Begin Source File

SOURCE=..\..\src\file.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\file.h

"..\..\elastic\file.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\file.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\file.h

"..\..\elastic\file.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\file.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\floatio.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\floatio.h

"..\..\elastic\floatio.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\floatio.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\floatio.h

"..\..\elastic\floatio.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\floatio.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\hash.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\hash.h

"..\..\elastic\hash.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\hash.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\hash.h

"..\..\elastic\hash.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\hash.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\hashdefs.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\hashdefs.h

"..\..\elastic\hashdefs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\hashdefs.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\hashdefs.h

"..\..\elastic\hashdefs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\hashdefs.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\hashfunc.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\hashfunc.h

"..\..\elastic\hashfunc.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\hashfunc.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\hashfunc.h

"..\..\elastic\hashfunc.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\hashfunc.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\hashtable.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\hashtable.h

"..\..\elastic\hashtable.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\hashtable.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\hashtable.h

"..\..\elastic\hashtable.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\hashtable.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\lib.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\lib.h

"..\..\elastic\lib.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\lib.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\lib.h

"..\..\elastic\lib.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\lib.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\list.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\list.h

"..\..\elastic\list.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\list.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\list.h

"..\..\elastic\list.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\list.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\memory.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\memory.h

"..\..\elastic\memory.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\memory.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\memory.h

"..\..\elastic\memory.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\memory.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\object.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\object.h

"..\..\elastic\object.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\object.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\object.h

"..\..\elastic\object.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\object.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\oop.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\oop.h

"..\..\elastic\oop.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\oop.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\oop.h

"..\..\elastic\oop.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\oop.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\os.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\os.h

"..\..\elastic\os.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\os.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\os.h

"..\..\elastic\os.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\os.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\prime.h
# End Source File
# Begin Source File

SOURCE=..\..\src\primes.h
# End Source File
# Begin Source File

SOURCE=..\..\src\private.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\private.h

"..\..\elastic\private.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\private.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\private.h

"..\..\elastic\private.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\private.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\stackrecycle.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\stackrecycle.h

"..\..\elastic\stackrecycle.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\stackrecycle.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\stackrecycle.h

"..\..\elastic\stackrecycle.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\stackrecycle.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\strtable.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\strtable.h

"..\..\elastic\strtable.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\strtable.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\strtable.h

"..\..\elastic\strtable.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\strtable.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\tsize.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\tsize.h

"..\..\elastic\tsize.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\tsize.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\tsize.h

"..\..\elastic\tsize.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\tsize.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\user.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\user.h

"..\..\elastic\user.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\user.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\user.h

"..\..\elastic\user.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\user.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\utility.h

!IF  "$(CFG)" == "elastiC - Win32 Release"

# Begin Custom Build
InputPath=..\..\src\utility.h

"..\..\elastic\utility.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\utility.h

# End Custom Build

!ELSEIF  "$(CFG)" == "elastiC - Win32 Debug"

# Begin Custom Build
InputPath=..\..\src\utility.h

"..\..\elastic\utility.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	COPY $(InputPath) ..\..\elastic\utility.h

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
