# Microsoft Developer Studio Generated NMAKE File, Based on Greta.dsp
!IF "$(CFG)" == ""
CFG=Greta - Win32 Release
!MESSAGE No configuration specified. Defaulting to Greta - Win32 Unicode Debug.
!ENDIF 

!IF "$(CFG)" != "Greta - Win32 Release" && "$(CFG)" != "Greta - Win32 Debug" && "$(CFG)" != "Greta - Win32 Unicode Debug" && "$(CFG)" != "Greta - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Greta.mak" CFG="Greta - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Greta - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Greta - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Greta - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Greta - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Greta - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Greta.lib"


CLEAN :
	-@erase "$(INTDIR)\regexpr2.obj"
	-@erase "$(INTDIR)\syntax2.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Greta.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\Greta.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Greta.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Greta.lib" 
LIB32_OBJS= \
	"$(INTDIR)\regexpr2.obj" \
	"$(INTDIR)\syntax2.obj"

"$(OUTDIR)\Greta.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Greta - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Greta.lib"


CLEAN :
	-@erase "$(INTDIR)\regexpr2.obj"
	-@erase "$(INTDIR)\syntax2.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Greta.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\Greta.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Greta.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Greta.lib" 
LIB32_OBJS= \
	"$(INTDIR)\regexpr2.obj" \
	"$(INTDIR)\syntax2.obj"

"$(OUTDIR)\Greta.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Greta - Win32 Unicode Debug"

OUTDIR=.\Unicode Debug
INTDIR=.\Unicode Debug
# Begin Custom Macros
OutDir=.\Unicode Debug
# End Custom Macros

ALL : "$(OUTDIR)\Greta.lib" "$(OUTDIR)\Greta.bsc"


CLEAN :
	-@erase "$(INTDIR)\regexpr2.obj"
	-@erase "$(INTDIR)\regexpr2.sbr"
	-@erase "$(INTDIR)\syntax2.obj"
	-@erase "$(INTDIR)\syntax2.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Greta.bsc"
	-@erase "$(OUTDIR)\Greta.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_LIB" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Greta.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Greta.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\regexpr2.sbr" \
	"$(INTDIR)\syntax2.sbr"

"$(OUTDIR)\Greta.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Greta.lib" 
LIB32_OBJS= \
	"$(INTDIR)\regexpr2.obj" \
	"$(INTDIR)\syntax2.obj"

"$(OUTDIR)\Greta.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Greta - Win32 Unicode Release"

OUTDIR=.\Unicode Release
INTDIR=.\Unicode Release
# Begin Custom Macros
OutDir=.\Unicode Release
# End Custom Macros

ALL : "$(OUTDIR)\Greta.lib"


CLEAN :
	-@erase "$(INTDIR)\regexpr2.obj"
	-@erase "$(INTDIR)\syntax2.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Greta.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)\Greta.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Greta.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Greta.lib" 
LIB32_OBJS= \
	"$(INTDIR)\regexpr2.obj" \
	"$(INTDIR)\syntax2.obj"

"$(OUTDIR)\Greta.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Greta.dep")
!INCLUDE "Greta.dep"
!ELSE 
!MESSAGE Warning: cannot find "Greta.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Greta - Win32 Release" || "$(CFG)" == "Greta - Win32 Debug" || "$(CFG)" == "Greta - Win32 Unicode Debug" || "$(CFG)" == "Greta - Win32 Unicode Release"
SOURCE=.\regexpr2.cpp

!IF  "$(CFG)" == "Greta - Win32 Release"


"$(INTDIR)\regexpr2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Greta - Win32 Debug"


"$(INTDIR)\regexpr2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Greta - Win32 Unicode Debug"


"$(INTDIR)\regexpr2.obj"	"$(INTDIR)\regexpr2.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Greta - Win32 Unicode Release"


"$(INTDIR)\regexpr2.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\syntax2.cpp

!IF  "$(CFG)" == "Greta - Win32 Release"


"$(INTDIR)\syntax2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Greta - Win32 Debug"


"$(INTDIR)\syntax2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Greta - Win32 Unicode Debug"


"$(INTDIR)\syntax2.obj"	"$(INTDIR)\syntax2.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Greta - Win32 Unicode Release"


"$(INTDIR)\syntax2.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

