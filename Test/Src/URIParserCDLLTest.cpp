#include "stdafx.h"
#include "Test.h"
#include "URIFixture.h"
#include "..\..\Src\URIParser.h"

BOOST_AUTO_TEST_SUITE(URIParserCDLLTest)

///\TODO: Fix and add more test cases

BOOST_FIXTURE_TEST_CASE(TestDLL, URIFixture)
{
	const char * version = URIParserGetVersionNumber();
	URIParserHostContext hostContext = { 0 };
	const char* uri = "ftp://admin@127.0.0.1:21/resource.txt";
	//ParseURI(hostContext, uri);
}

BOOST_AUTO_TEST_SUITE_END()