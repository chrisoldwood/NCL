# Microsoft Developer Studio Project File - Name="NCL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=NCL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NCL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NCL.mak" CFG="NCL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NCL - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "NCL - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Win32/NCL", JRCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NCL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W4 /WX /GX /O2 /I "..\wcl" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX"ncl.hpp" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "NCL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /I "..\wcl" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX"ncl.hpp" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "NCL - Win32 Release"
# Name "NCL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ClientPipe.cpp
# End Source File
# Begin Source File

SOURCE=.\DDEClient.cpp
# End Source File
# Begin Source File

SOURCE=.\DDECltConv.cpp
# End Source File
# Begin Source File

SOURCE=.\DDEException.cpp
# End Source File
# Begin Source File

SOURCE=.\DDEServer.cpp
# End Source File
# Begin Source File

SOURCE=.\DDESvrConv.cpp
# End Source File
# Begin Source File

SOURCE=.\NamedPipe.cpp
# End Source File
# Begin Source File

SOURCE=.\PipeException.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerPipe.cpp
# End Source File
# Begin Source File

SOURCE=.\Socket.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketException.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpCltSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\TCPSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpSvrSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\UDPCltSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\UDPSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\UDPSvrSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\WinSock.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ClientPipe.hpp
# End Source File
# Begin Source File

SOURCE=.\DDEClient.hpp
# End Source File
# Begin Source File

SOURCE=.\DDECltConv.hpp
# End Source File
# Begin Source File

SOURCE=.\DDEConv.hpp
# End Source File
# Begin Source File

SOURCE=.\DDEData.hpp
# End Source File
# Begin Source File

SOURCE=.\DDEException.hpp
# End Source File
# Begin Source File

SOURCE=.\DDEInst.hpp
# End Source File
# Begin Source File

SOURCE=.\DDELink.hpp
# End Source File
# Begin Source File

SOURCE=.\DDEServer.hpp
# End Source File
# Begin Source File

SOURCE=.\DDEString.hpp
# End Source File
# Begin Source File

SOURCE=.\DDESvrConv.hpp
# End Source File
# Begin Source File

SOURCE=.\DefDDEClientListener.hpp
# End Source File
# Begin Source File

SOURCE=.\DefDDEServerListener.hpp
# End Source File
# Begin Source File

SOURCE=.\IDDEClientListener.hpp
# End Source File
# Begin Source File

SOURCE=.\IDDEConvData.hpp
# End Source File
# Begin Source File

SOURCE=.\IDDELinkData.hpp
# End Source File
# Begin Source File

SOURCE=.\IDDEServerListener.hpp
# End Source File
# Begin Source File

SOURCE=.\NamedPipe.hpp
# End Source File
# Begin Source File

SOURCE=.\NCL.hpp
# End Source File
# Begin Source File

SOURCE=.\PipeException.hpp
# End Source File
# Begin Source File

SOURCE=.\ServerPipe.hpp
# End Source File
# Begin Source File

SOURCE=.\Socket.hpp
# End Source File
# Begin Source File

SOURCE=.\SocketException.hpp
# End Source File
# Begin Source File

SOURCE=.\TcpCltSocket.hpp
# End Source File
# Begin Source File

SOURCE=.\TCPSocket.hpp
# End Source File
# Begin Source File

SOURCE=.\TcpSvrSocket.hpp
# End Source File
# Begin Source File

SOURCE=.\UDPCltSocket.hpp
# End Source File
# Begin Source File

SOURCE=.\UDPSocket.hpp
# End Source File
# Begin Source File

SOURCE=.\UDPSvrSocket.hpp
# End Source File
# Begin Source File

SOURCE=.\WinSock.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\TODO.txt
# End Source File
# End Target
# End Project
