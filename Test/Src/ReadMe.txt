========================================================================
    CONSOLE APPLICATION : URIParserTest Project Overview
========================================================================

This project requires BOOST_1_59_0 to build and run.
It uses the boost unit module to facilitate test driven development via 
the create of test cases

Installation: Download and build BOOST_1_59_0
Windows builds: Add the system environment variable BOOST_ROOT and set 
to the installed path e.g. BOOST_ROOT=C:\boost_1_59_0 
Linux: TO DO

URIParserTest.cpp
    This is the suite of tests to directly test the parsing function within the DLL.

URIParserCDLLTest.cpp
	This is the suite of tests to check the parsing via the C DLL calls.

URIParserCppWrapperTest.cpp
	This is the suite of tests to check the parsing via the Cpp DLL wrapper.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named URIParserTest.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
